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

/**
 * \file semaphore.c
 * \brief Provides ARINC653 API functionnalities for semaphore management.
 */

#include "config.h"

#ifdef POK_NEEDS_ARINC653_SEMAPHORE

#include "semaphore.h"

#include <arinc653/types.h>
#include <arinc653/semaphore.h>
#include <utils.h>

#include <kernel_shared_data.h>
#include <../include/core/assert_os.h>

#include <string.h>
#include <arinc_config.h>
#include "arinc_process_queue.h"

static size_t nsemaphores_used = 0;

/* Find semaphore by name (in UPPERCASE). Returns NULL if not found. */
static struct arinc_semaphore* find_semaphore(const char* name)
{
   for(int i = 0; i < nsemaphores_used; i++)
   {
      struct arinc_semaphore* semaphore = &arinc_semaphores[i];
      if(strncasecmp(semaphore->semaphore_name, name, MAX_NAME_LENGTH) == 0)
         return semaphore;
   }

   return NULL;
}

void CREATE_SEMAPHORE (SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
                       SEMAPHORE_VALUE_TYPE CURRENT_VALUE,
                       SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE,
                       QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
                       SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
                       RETURN_CODE_TYPE *RETURN_CODE )
{
   if(kshd.partition_mode == POK_PARTITION_MODE_NORMAL) {
      // Cannot create semaphore in NORMAL mode
      *RETURN_CODE = INVALID_MODE;
      return;
   }

   if(find_semaphore(SEMAPHORE_NAME) != NULL) {
      // Semaphore with given name already exists.
      *RETURN_CODE = NO_ACTION;
      return;
   }

   if(nsemaphores_used == arinc_config_nsemaphores) {
      // Too many semaphores
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   if(MAXIMUM_VALUE < 0 || MAXIMUM_VALUE > MAX_SEMAPHORE_VALUE) {
      // Maximum value is negative or too high.
      // DEV: Current implementation SIGNAL_SEMAPHORE allows to signal semaphore with maximum value, if there are waiters.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   if(CURRENT_VALUE < 0 || CURRENT_VALUE > MAXIMUM_VALUE) {
      // Current value is negative or too high.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   if((QUEUING_DISCIPLINE != FIFO) && (QUEUING_DISCIPLINE != PRIORITY)) {
      // Incorrect discipline value.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_semaphore* semaphore = &arinc_semaphores[nsemaphores_used];

   memcpy(semaphore->semaphore_name, SEMAPHORE_NAME, MAX_NAME_LENGTH);
   semaphore->maximum_value = MAXIMUM_VALUE;
   semaphore->current_value = CURRENT_VALUE;
   semaphore->discipline = QUEUING_DISCIPLINE;

   msection_init(&semaphore->section);
   msection_wq_init(&semaphore->process_queue);

   *SEMAPHORE_ID = nsemaphores_used + 1;// Avoid 0 value.

   nsemaphores_used++;

   *RETURN_CODE = NO_ERROR;
}

void WAIT_SEMAPHORE (SEMAPHORE_ID_TYPE SEMAPHORE_ID,
                     SYSTEM_TIME_TYPE TIME_OUT,
                     RETURN_CODE_TYPE *RETURN_CODE )
{
   if (SEMAPHORE_ID <= 0 || SEMAPHORE_ID > nsemaphores_used) {
      // Incorrect semaphore identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_semaphore* semaphore = &arinc_semaphores[SEMAPHORE_ID - 1];

   msection_enter(&semaphore->section);

   if(semaphore->current_value > 0) {
      // Current value is positive.
      semaphore->current_value--;
      *RETURN_CODE = NO_ERROR;
   }

   else if(TIME_OUT == 0) {
      // Current value is 0 but waiting is not requested.
      *RETURN_CODE = NOT_AVAILABLE;
   }

   else {
      // Current value is 0 and waiting is *requested* by the caller.
      // (whether waiting is *allowed* will be checked by the kernel.)
      pok_thread_id_t t = kshd.current_thread_id;

      arinc_process_queue_add_common(&semaphore->process_queue, semaphore->discipline);

      switch(msection_wait(&semaphore->section, TIME_OUT))
      {
      case POK_ERRNO_OK:
         // Current value is already decremented for us by the notifier.
         *RETURN_CODE = NO_ERROR;
         break;
      case POK_ERRNO_MODE: // Waiting is not allowed
      case POK_ERRNO_CANCELLED: // Thread has been STOP()-ed or [IPPC] server thread has been cancelled.
         msection_wq_del(&semaphore->process_queue, t);
         *RETURN_CODE = INVALID_MODE;
         break;
      case POK_ERRNO_TIMEOUT:
         // Timeout
         msection_wq_del(&semaphore->process_queue, t);
         *RETURN_CODE = TIMED_OUT;
         break;
      default:
         assert_os(FALSE);
      }
   }

   msection_leave(&semaphore->section);
}

void SIGNAL_SEMAPHORE (SEMAPHORE_ID_TYPE SEMAPHORE_ID,
                       RETURN_CODE_TYPE *RETURN_CODE )
{
   if (SEMAPHORE_ID <= 0 || SEMAPHORE_ID > nsemaphores_used) {
      // Incorrect semaphore identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_semaphore* semaphore = &arinc_semaphores[SEMAPHORE_ID - 1];

   msection_enter(&semaphore->section);

   if(msection_wq_notify(&semaphore->section,
      &semaphore->process_queue, FALSE) == POK_ERRNO_OK) {
      // There are waiters on semaphore. We have already awoken the first of them.
      /*
       * DEV: ARINC says to emit error if current_value == maximum_value.
       * 
       * But we allow to signal such semaphore if someone waits on it.
       */
      pok_thread_id_t t_awoken = semaphore->process_queue.first;
      msection_wq_del(&semaphore->process_queue, t_awoken);
      *RETURN_CODE = NO_ERROR;
   }

   else if(semaphore->current_value == semaphore->maximum_value) {
      // Current value achive the maximum.
      *RETURN_CODE = NO_ACTION;
   }

   else {
      // No one waits on semaphore. Just increment its current value.
      semaphore->current_value ++;
      *RETURN_CODE = NO_ERROR;
   }

   msection_leave(&semaphore->section);
}

void GET_SEMAPHORE_ID (SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
                       SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
                       RETURN_CODE_TYPE *RETURN_CODE )
{

   struct arinc_semaphore* semaphore = find_semaphore(SEMAPHORE_NAME);
   if(semaphore == NULL) {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   *SEMAPHORE_ID = (semaphore - arinc_semaphores) + 1;
   *RETURN_CODE = NO_ERROR;
}

void GET_SEMAPHORE_STATUS (SEMAPHORE_ID_TYPE SEMAPHORE_ID,
                           SEMAPHORE_STATUS_TYPE *SEMAPHORE_STATUS,
                           RETURN_CODE_TYPE *RETURN_CODE )
{
   if (SEMAPHORE_ID <= 0 || SEMAPHORE_ID > nsemaphores_used) {
      // Incorrect semaphore identificator.
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   struct arinc_semaphore* semaphore = &arinc_semaphores[SEMAPHORE_ID - 1];

   SEMAPHORE_STATUS->MAXIMUM_VALUE = semaphore->maximum_value;

   msection_enter(&semaphore->section);

   SEMAPHORE_STATUS->CURRENT_VALUE = semaphore->current_value;
   SEMAPHORE_STATUS->WAITING_PROCESSES = msection_wq_size(&semaphore->section,
      &semaphore->process_queue);

   msection_leave(&semaphore->section);

   *RETURN_CODE = NO_ERROR;
}

#endif
