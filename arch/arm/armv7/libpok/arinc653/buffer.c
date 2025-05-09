/*
 * Institute for System Programming of the Russian Academy of Sciences
 * Copyright (C) 2016 ISPRAS
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, Version 3.
 *
 * This program is distributed in the hope # that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License version 3 for more details.
 */

#include <config.h>

#ifdef POK_NEEDS_ARINC653_BUFFER

#include "buffer.h"

#include <arinc653/types.h>
#include <arinc653/buffer.h>
#include "include/types.h"
#include <utils.h>

#include <kernel_shared_data.h>
#include <../include/core/assert_os.h>

#include <string.h>
#include "arinc_alloc.h"
#include <arinc_config.h>
#include "arinc_process_queue.h"

static size_t nbuffers_used = 0;

/* Find buffer by name (in UPPERCASE). Returns NULL if not found. */
static struct arinc_buffer* find_buffer(const char* name)
{
   for(int i = 0; i < nbuffers_used; i++)
   {
      struct arinc_buffer* buffer = &arinc_buffers[i];
      if(strncasecmp(buffer->buffer_name, name, MAX_NAME_LENGTH) == 0)
         return buffer;
   }

   return NULL;
}

/* Return message index corresponded to given offset. */
static MESSAGE_RANGE_TYPE
message_index(struct arinc_buffer* buffer, MESSAGE_RANGE_TYPE offset)
{
   size_t res = buffer->base_offset + offset;

   if(res >= buffer->max_nb_message) res -= buffer->max_nb_message;

   return res;
}

/* Return pointer to message at given index. */
static char*
message_at(struct arinc_buffer* buffer, MESSAGE_RANGE_TYPE index)
{
   return buffer->messages + buffer->message_stride * index;
}

void CREATE_BUFFER (
       /*in */ BUFFER_NAME_TYPE         BUFFER_NAME,
       /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
       /*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE,
       /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
       /*out*/ BUFFER_ID_TYPE           *BUFFER_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if(kshd.partition_mode == POK_PARTITION_MODE_NORMAL) {
      // Cannot create buffer in NORMAL mode
      *RETURN_CODE = INVALID_MODE;
      return;
   }


   if(find_buffer(BUFFER_NAME) != NULL) {
      // Buffer with given name already exists.
      *RETURN_CODE = NO_ACTION;
      return;
   }

   if(nbuffers_used == arinc_config_nbuffers) {
      // Too many buffers
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   if(MAX_MESSAGE_SIZE <= 0 || MAX_MESSAGE_SIZE > SYSTEM_LIMIT_MESSAGE_SIZE) {
      // Message size is non-positive or too high.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   if(MAX_NB_MESSAGE <= 0 || MAX_NB_MESSAGE > SYSTEM_LIMIT_NUMBER_OF_MESSAGES) {
      // Number of messages is non-positive or too high.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   if((QUEUING_DISCIPLINE != FIFO) && (QUEUING_DISCIPLINE != PRIORITY)) {
      // Incorrect discipline value.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }


   struct arinc_buffer* buffer = &arinc_buffers[nbuffers_used];

   buffer->max_message_size = MAX_MESSAGE_SIZE;
   // Optimize messages for copiing.
   buffer->message_stride = ALIGN(buffer->max_message_size, __alignof__(int));
   buffer->max_nb_message = MAX_NB_MESSAGE;

   arinc_allocator_state astate = arinc_allocator_get_state();

   buffer->messages = arinc_alloc(buffer->message_stride * buffer->max_nb_message, __alignof__(int));
   buffer->messages_size = arinc_alloc(buffer->max_nb_message * sizeof(*buffer->messages_size),
      __alignof__(*buffer->messages_size));

   if(buffer->messages == NULL || buffer->messages_size == NULL)
   {
      // Failed to allocate message queue.
      arinc_allocator_reset_state(astate);
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   memcpy(buffer->buffer_name, BUFFER_NAME, MAX_NAME_LENGTH);
   msection_init(&buffer->section);
   msection_wq_init(&buffer->process_queue);
   buffer->base_offset = 0;
   buffer->discipline = QUEUING_DISCIPLINE;

   *BUFFER_ID = nbuffers_used + 1;// Avoid 0 value.

   nbuffers_used++;

   *RETURN_CODE = NO_ERROR;
}

void SEND_BUFFER (
       /*in */ BUFFER_ID_TYPE           BUFFER_ID,
       /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
       /*in */ MESSAGE_SIZE_TYPE        LENGTH,
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if (BUFFER_ID <= 0 || BUFFER_ID > nbuffers_used) {
      // Incorrect buffer identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_buffer* buffer = &arinc_buffers[BUFFER_ID - 1];

   if(LENGTH <= 0 || LENGTH > buffer->max_message_size) {
      // LENGTH is non-positive or too big.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   msection_enter(&buffer->section);

   if(buffer->nb_message < buffer->max_nb_message)
   {
      // Buffer is not full.
      if(msection_wq_notify(&buffer->section,
         &buffer->process_queue, FALSE) == POK_ERRNO_OK) {
         // There are waiters on buffer. We have already awoken the first of them.
         pok_thread_id_t t_awoken = buffer->process_queue.first;

         char* dst = kshd.tshd[t_awoken].wq_buffer.dst;

         memcpy(dst, MESSAGE_ADDR, LENGTH);
         kshd.tshd[t_awoken].wq_len = LENGTH;

         msection_wq_del(&buffer->process_queue, t_awoken);
      }
      else {
         // No processes are waiting on buffer.
         MESSAGE_RANGE_TYPE index = message_index(buffer,
            buffer->nb_message);

         char* message_dest = message_at(buffer, index);

         memcpy(message_dest, MESSAGE_ADDR, LENGTH);
         buffer->messages_size[index] = LENGTH;

         buffer->nb_message++;
      }

      *RETURN_CODE = NO_ERROR;
   }
   else if(TIME_OUT == 0) {
      // Buffer is full but waiting is not requested.
      *RETURN_CODE = NOT_AVAILABLE;
   }
   else {
      // Buffer is full and waiting is *requested* by the caller.
      // (whether waiting is *allowed* will be checked by the kernel.)
      pok_thread_id_t t = kshd.current_thread_id;

      kshd.tshd[t].wq_buffer.src = MESSAGE_ADDR;
      kshd.tshd[t].wq_len = LENGTH;

      arinc_process_queue_add_common(&buffer->process_queue, buffer->discipline);

      switch(msection_wait(&buffer->section, TIME_OUT))
      {
      case POK_ERRNO_OK:
         // Message is already copied into the buffer by the notifier.
         *RETURN_CODE = NO_ERROR;
         break;
      case POK_ERRNO_MODE: // Waiting is not allowed
      case POK_ERRNO_CANCELLED: // Thread has been STOP()-ed or [IPPC] server thread has been cancelled.
         msection_wq_del(&buffer->process_queue, t);
         *RETURN_CODE = INVALID_MODE;
         break;
      case POK_ERRNO_TIMEOUT:
         // Timeout
         msection_wq_del(&buffer->process_queue, t);
         *RETURN_CODE = TIMED_OUT;
         break;
      default:
         assert_os(FALSE);
      }
   }

   msection_leave(&buffer->section);
}

void RECEIVE_BUFFER (
       /*in */ BUFFER_ID_TYPE           BUFFER_ID,
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
       /*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
       /*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if (BUFFER_ID <= 0 || BUFFER_ID > nbuffers_used) {
      // Incorrect buffer identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_buffer* buffer = &arinc_buffers[BUFFER_ID - 1];

   msection_enter(&buffer->section);

   if(buffer->nb_message > 0)
   {
      // Buffer is not empty.
      MESSAGE_RANGE_TYPE index = message_index(buffer, 0);
      const char* src = message_at(buffer, index);
      MESSAGE_SIZE_TYPE len = buffer->messages_size[index];

      memcpy(MESSAGE_ADDR, src, len);

      buffer->base_offset = message_index(buffer, 1);
      buffer->nb_message--;

      *RETURN_CODE = NO_ERROR;
      *LENGTH = len;

      if(msection_wq_notify(&buffer->section,
         &buffer->process_queue, FALSE) == POK_ERRNO_OK) {
         // There are waiters on buffer. We have already awoken the first of them.
         pok_thread_id_t t_awoken = buffer->process_queue.first;

         MESSAGE_RANGE_TYPE w_index = message_index(buffer, buffer->nb_message);
         char* w_dest = message_at(buffer, w_index);

         const char* w_src = kshd.tshd[t_awoken].wq_buffer.src;
         MESSAGE_SIZE_TYPE w_len = kshd.tshd[t_awoken].wq_len;

         memcpy(w_dest, w_src, w_len);
         buffer->messages_size[w_index] = w_len;

         buffer->nb_message++;

         msection_wq_del(&buffer->process_queue, t_awoken);
      }
   }
   else if(TIME_OUT == 0) {
      // Buffer is empty but waiting is not requested.
      *LENGTH = 0;
      *RETURN_CODE = NOT_AVAILABLE;
   }
   else {
      // Buffer is empty and waiting is *requested* by the caller.
      // (whether waiting is *allowed* will be checked by the kernel.)
      pok_thread_id_t t = kshd.current_thread_id;

      kshd.tshd[t].wq_buffer.dst = MESSAGE_ADDR;

      arinc_process_queue_add_common(&buffer->process_queue, buffer->discipline);

      switch(msection_wait(&buffer->section, TIME_OUT))
      {
      case POK_ERRNO_OK:
         // Message is already copied from the buffer by the notifier.
         *LENGTH = kshd.tshd[t].wq_len;
         *RETURN_CODE = NO_ERROR;
         break;
      case POK_ERRNO_MODE: // Waiting is not allowed
      case POK_ERRNO_CANCELLED: // Thread has been STOP()-ed or [IPPC] server thread has been cancelled.
         msection_wq_del(&buffer->process_queue, t);
         *LENGTH = 0;
         *RETURN_CODE = INVALID_MODE;
         break;
      case POK_ERRNO_TIMEOUT:
         // Timeout
         msection_wq_del(&buffer->process_queue, t);
         *LENGTH = 0;
         *RETURN_CODE = TIMED_OUT;
         break;
      default:
         assert_os(FALSE);
      }
   }

   msection_leave(&buffer->section);
}

void GET_BUFFER_ID (
       /*in */ BUFFER_NAME_TYPE         BUFFER_NAME,
       /*out*/ BUFFER_ID_TYPE           *BUFFER_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{

   struct arinc_buffer* buffer = find_buffer(BUFFER_NAME);
   if(buffer == NULL) {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   *BUFFER_ID = (buffer - arinc_buffers) + 1;
   *RETURN_CODE = NO_ERROR;
}

void GET_BUFFER_STATUS (
       /*in */ BUFFER_ID_TYPE           BUFFER_ID,
       /*out*/ BUFFER_STATUS_TYPE       *BUFFER_STATUS,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if (BUFFER_ID <= 0 || BUFFER_ID > nbuffers_used) {
      // Incorrect buffer identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_buffer* buffer = &arinc_buffers[BUFFER_ID - 1];

   msection_enter(&buffer->section);

   BUFFER_STATUS->NB_MESSAGE = buffer->nb_message;
   BUFFER_STATUS->MAX_NB_MESSAGE = buffer->max_nb_message;
   BUFFER_STATUS->MAX_MESSAGE_SIZE = buffer->max_message_size;
   BUFFER_STATUS->WAITING_PROCESSES = msection_wq_size(&buffer->section,
      &buffer->process_queue);

   msection_leave(&buffer->section);

   *RETURN_CODE = NO_ERROR;
}

#endif
