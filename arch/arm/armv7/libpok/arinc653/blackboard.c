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

#ifdef POK_NEEDS_ARINC653_BLACKBOARD

#include "blackboard.h"

#include <arinc653/types.h>
#include <arinc653/blackboard.h>
#include <utils.h>

#include <kernel_shared_data.h>
#include <../include/core/assert_os.h>

#include <string.h>
#include "arinc_alloc.h"
#include <arinc_config.h>
#include "arinc_process_queue.h"

static size_t nblackboards_used = 0;

/* Find blackboard by name (in UPPERCASE). Returns NULL if not found. */
static struct arinc_blackboard* find_blackboard(const char* name)
{
   for(int i = 0; i < nblackboards_used; i++)
   {
      struct arinc_blackboard* blackboard = &arinc_blackboards[i];
      if(strncasecmp(blackboard->blackboard_name, name, MAX_NAME_LENGTH) == 0)
         return blackboard;
   }

   return NULL;
}


void CREATE_BLACKBOARD (
       /*in */ BLACKBOARD_NAME_TYPE     BLACKBOARD_NAME,
       /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
       /*out*/ BLACKBOARD_ID_TYPE       *BLACKBOARD_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if(kshd.partition_mode == POK_PARTITION_MODE_NORMAL) {
      // Cannot create blackboard in NORMAL mode
      *RETURN_CODE = INVALID_MODE;
      return;
   }

   if(find_blackboard(BLACKBOARD_NAME) != NULL) {
      // Blackboard with given name already exists.
      *RETURN_CODE = NO_ACTION;
      return;
   }

   if(nblackboards_used == arinc_config_nblackboards) {
      // Too many blackboards
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   if(MAX_MESSAGE_SIZE <= 0 || MAX_MESSAGE_SIZE > SYSTEM_LIMIT_MESSAGE_SIZE) {
      // Message size is non-positive or too high.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_blackboard* blackboard = &arinc_blackboards[nblackboards_used];

   blackboard->max_message_size = MAX_MESSAGE_SIZE;

   // Optimize message for copiing.
   blackboard->message = arinc_alloc(blackboard->max_message_size, __alignof__(int));

   if(blackboard->message == NULL)
   {
      // Failed to allocate message.
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   memcpy(blackboard->blackboard_name, BLACKBOARD_NAME, MAX_NAME_LENGTH);
   blackboard->message_size = 0;
   msection_init(&blackboard->section);
   msection_wq_init(&blackboard->process_queue);

   *BLACKBOARD_ID = nblackboards_used + 1;// Avoid 0 value.

   nblackboards_used++;

   *RETURN_CODE = NO_ERROR;
}

void DISPLAY_BLACKBOARD (
       /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID,
       /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
       /*in */ MESSAGE_SIZE_TYPE        LENGTH,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if (BLACKBOARD_ID <= 0 || BLACKBOARD_ID > nblackboards_used) {
      // Incorrect blackboard identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_blackboard* blackboard = &arinc_blackboards[BLACKBOARD_ID - 1];

   if(LENGTH <= 0 || LENGTH > blackboard->max_message_size) {
      // LENGTH is non-positive or too big.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   msection_enter(&blackboard->section);

   memcpy(blackboard->message, MESSAGE_ADDR, LENGTH);
   blackboard->message_size = LENGTH;

   if(msection_wq_notify(&blackboard->section, &blackboard->process_queue, TRUE)
      == POK_ERRNO_OK) {
      // There are processes waiting on an empty blackboard.
      // We are already woken up them.
      pok_thread_id_t t = blackboard->process_queue.first;

      do {
         char* w_dest = kshd.tshd[t].wq_buffer.dst;
         memcpy(w_dest, blackboard->message, blackboard->message_size);
         kshd.tshd[t].wq_len = blackboard->message_size;

         msection_wq_del(&blackboard->process_queue, t);

         t = blackboard->process_queue.first;
      } while(t != JET_THREAD_ID_NONE);
   }

   msection_leave(&blackboard->section);
   *RETURN_CODE = NO_ERROR;
}

void READ_BLACKBOARD (
       /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID,
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
       /*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
       /*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if (BLACKBOARD_ID <= 0 || BLACKBOARD_ID > nblackboards_used) {
      // Incorrect blackboard identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_blackboard* blackboard = &arinc_blackboards[BLACKBOARD_ID - 1];

   msection_enter(&blackboard->section);

   if(blackboard->message_size > 0) {
      // There is a message in the blackboard.
      memcpy(MESSAGE_ADDR, blackboard->message, blackboard->message_size);
      *LENGTH = blackboard->message_size;
      *RETURN_CODE = NO_ERROR;
   }
   else if(TIME_OUT == 0)
   {
      // There is no message in blackboard but waiting is not requested.
      *LENGTH = 0;
      *RETURN_CODE = NOT_AVAILABLE;
   }
   else {
      // Blackboard is empty and waiting is *requested* by the caller.
      // (whether waiting is *allowed* will be checked by the kernel.)
      pok_thread_id_t t = kshd.current_thread_id;

      kshd.tshd[t].wq_buffer.dst = MESSAGE_ADDR;

      /*
       * ARINC explicitely says, that:
       * 
       * If processes were waiting on the empty blackboard, the
       * processes will be released on a priority followed by FIFO (when
       * priorities are equal) basis.
       */
      arinc_process_queue_add_common(&blackboard->process_queue, PRIORITY);

      switch(msection_wait(&blackboard->section, TIME_OUT))
      {
      case POK_ERRNO_OK:
         // Message is already copied from the blackboard by the notifier.
         *LENGTH = kshd.tshd[t].wq_len;
         *RETURN_CODE = NO_ERROR;
         break;
      case POK_ERRNO_MODE: // Waiting is not allowed
      case POK_ERRNO_CANCELLED: // Thread has been STOP()-ed or [IPPC] server thread has been cancelled.
         msection_wq_del(&blackboard->process_queue, t);
         *LENGTH = 0;
         *RETURN_CODE = INVALID_MODE;
         break;
      case POK_ERRNO_TIMEOUT:
         // Timeout
         msection_wq_del(&blackboard->process_queue, t);
         *LENGTH = 0;
         *RETURN_CODE = TIMED_OUT;
         break;
      default:
         assert_os(FALSE);
      }
   }

   msection_leave(&blackboard->section);
}

void CLEAR_BLACKBOARD (
       /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if (BLACKBOARD_ID <= 0 || BLACKBOARD_ID > nblackboards_used) {
      // Incorrect blackboard identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_blackboard* blackboard = &arinc_blackboards[BLACKBOARD_ID - 1];

   msection_enter(&blackboard->section);
   blackboard->message_size = 0;
   msection_leave(&blackboard->section);

   *RETURN_CODE = NO_ERROR;
}

void GET_BLACKBOARD_ID (
       /*in */ BLACKBOARD_NAME_TYPE     BLACKBOARD_NAME,
       /*out*/ BLACKBOARD_ID_TYPE       *BLACKBOARD_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{

   struct arinc_blackboard* blackboard = find_blackboard(BLACKBOARD_NAME);
   if(blackboard == NULL) {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   *BLACKBOARD_ID = (blackboard - arinc_blackboards) + 1;
   *RETURN_CODE = NO_ERROR;
}

void GET_BLACKBOARD_STATUS (
       /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID,
       /*out*/ BLACKBOARD_STATUS_TYPE   *BLACKBOARD_STATUS,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   if (BLACKBOARD_ID <= 0 || BLACKBOARD_ID > nblackboards_used) {
      // Incorrect blackboard identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_blackboard* blackboard = &arinc_blackboards[BLACKBOARD_ID - 1];

   BLACKBOARD_STATUS->MAX_MESSAGE_SIZE = blackboard->max_message_size;

   msection_enter(&blackboard->section);
   BLACKBOARD_STATUS->EMPTY_INDICATOR = (blackboard->message_size == 0)
      ? EMPTY: OCCUPIED;
   BLACKBOARD_STATUS->WAITING_PROCESSES =
      msection_wq_size(&blackboard->section, &blackboard->process_queue);
   msection_leave(&blackboard->section);

   *RETURN_CODE = NO_ERROR;
}
#endif
