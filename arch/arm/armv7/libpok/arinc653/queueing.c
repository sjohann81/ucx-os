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
 *
 * This file also incorporates work covered by POK License.
 * Copyright (c) 2007-2009 POK team
 */

#include "config.h"

#include <core/dependencies.h>

#ifdef POK_NEEDS_ARINC653_QUEUEING

#include "include/types.h"
#include <middleware/port.h>
#include <arinc653/types.h>
#include <arinc653/queueing.h>
#include <utils.h>

#define MAP_ERROR(from, to) case (from): *RETURN_CODE = (to); break
#define MAP_ERROR_DEFAULT(to) default: *RETURN_CODE = (to); break

void CREATE_QUEUING_PORT (
      /*in */ QUEUING_PORT_NAME_TYPE    QUEUING_PORT_NAME,
      /*in */ MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE,
      /*in */ MESSAGE_RANGE_TYPE        MAX_NB_MESSAGE,
      /*in */ PORT_DIRECTION_TYPE       PORT_DIRECTION,
      /*in */ QUEUING_DISCIPLINE_TYPE   QUEUING_DISCIPLINE,
      /*out*/ QUEUING_PORT_ID_TYPE      *QUEUING_PORT_ID,
      /*out*/ RETURN_CODE_TYPE          *RETURN_CODE)
{
   pok_ret_t                        core_ret;
   pok_port_id_t                    core_id;
   pok_queuing_discipline_t         core_discipline;
   pok_port_direction_t             core_direction;

   switch (QUEUING_DISCIPLINE)
   {
     case PRIORITY:
         core_discipline = POK_QUEUING_DISCIPLINE_PRIORITY;
         break;

     case FIFO:
         core_discipline = POK_QUEUING_DISCIPLINE_FIFO;
         break;

      default:
         *RETURN_CODE = INVALID_CONFIG;
         return;
   }

   switch (PORT_DIRECTION)
   {
      case SOURCE:
         core_direction = POK_PORT_DIRECTION_OUT;
         break;
      
      case DESTINATION:
         core_direction = POK_PORT_DIRECTION_IN;
         break;

      default:
         *RETURN_CODE = INVALID_CONFIG;
         return;
   }

   core_ret = pok_port_queuing_create(QUEUING_PORT_NAME,
      MAX_MESSAGE_SIZE, MAX_NB_MESSAGE, core_direction, core_discipline,
      &core_id);

   *QUEUING_PORT_ID = core_id + 1;

   switch (core_ret) {
      MAP_ERROR(POK_ERRNO_OK, NO_ERROR);
      MAP_ERROR(POK_ERRNO_EXISTS, NO_ACTION);
      MAP_ERROR(POK_ERRNO_MODE, INVALID_MODE);
      MAP_ERROR_DEFAULT(INVALID_CONFIG);
   }
}

void SEND_QUEUING_MESSAGE (
      /*in */ QUEUING_PORT_ID_TYPE      QUEUING_PORT_ID,
      /*in */ MESSAGE_ADDR_TYPE         MESSAGE_ADDR,       /* by reference */
      /*in */ MESSAGE_SIZE_TYPE         LENGTH,
      /*in */ SYSTEM_TIME_TYPE          TIME_OUT,
      /*out*/ RETURN_CODE_TYPE          *RETURN_CODE)
{
    pok_ret_t core_ret;

    if (QUEUING_PORT_ID <= 0) {
        *RETURN_CODE = INVALID_PARAM;
        return;
    }

    core_ret = pok_port_queuing_send (QUEUING_PORT_ID - 1, MESSAGE_ADDR, LENGTH, &TIME_OUT);

    switch (core_ret) {
        MAP_ERROR(POK_ERRNO_OK, NO_ERROR);
        MAP_ERROR(POK_ERRNO_DIRECTION, INVALID_MODE);
        MAP_ERROR(POK_ERRNO_MODE, INVALID_MODE);
        MAP_ERROR(POK_ERRNO_FULL, NOT_AVAILABLE);
        MAP_ERROR(POK_ERRNO_TIMEOUT, TIMED_OUT); 
        MAP_ERROR(POK_ERRNO_PARAM, INVALID_CONFIG); 
        MAP_ERROR_DEFAULT(INVALID_CONFIG);
    }
}

void RECEIVE_QUEUING_MESSAGE (
      /*in */ QUEUING_PORT_ID_TYPE      QUEUING_PORT_ID,
      /*in */ SYSTEM_TIME_TYPE          TIME_OUT,
      /*out*/ MESSAGE_ADDR_TYPE         MESSAGE_ADDR,
      /*out*/ MESSAGE_SIZE_TYPE         *LENGTH,
      /*out*/ RETURN_CODE_TYPE          *RETURN_CODE )
{
   pok_ret_t core_ret;

   if (QUEUING_PORT_ID <= 0) {
       *RETURN_CODE = INVALID_PARAM;
       return;
   }
   
   core_ret = pok_port_queuing_receive (QUEUING_PORT_ID - 1, &TIME_OUT, MESSAGE_ADDR, (pok_port_size_t*)LENGTH);

   switch (core_ret) {
        MAP_ERROR(POK_ERRNO_OK, NO_ERROR);
        MAP_ERROR(POK_ERRNO_DIRECTION, INVALID_MODE);
        MAP_ERROR(POK_ERRNO_MODE, INVALID_MODE);
        MAP_ERROR(POK_ERRNO_EMPTY, NOT_AVAILABLE);
        MAP_ERROR(POK_ERRNO_TIMEOUT, TIMED_OUT);
        MAP_ERROR(POK_ERRNO_TOOMANY, INVALID_CONFIG);
        MAP_ERROR_DEFAULT(INVALID_CONFIG);
    }
}

void GET_QUEUING_PORT_ID (
      /*in */ QUEUING_PORT_NAME_TYPE    QUEUING_PORT_NAME,
      /*out*/ QUEUING_PORT_ID_TYPE      *QUEUING_PORT_ID,
      /*out*/ RETURN_CODE_TYPE          *RETURN_CODE)
{
    pok_ret_t core_ret;
    pok_port_id_t id;

    core_ret = pok_port_queuing_id(QUEUING_PORT_NAME, &id);

    if (core_ret == POK_ERRNO_OK) {
        *QUEUING_PORT_ID = id + 1;
    }
   
    switch (core_ret) {
        MAP_ERROR(POK_ERRNO_OK, NO_ERROR);
        MAP_ERROR_DEFAULT(INVALID_CONFIG);
    }
}

void GET_QUEUING_PORT_STATUS (
      /*in */ QUEUING_PORT_ID_TYPE      QUEUING_PORT_ID,
      /*out*/ QUEUING_PORT_STATUS_TYPE *QUEUING_PORT_STATUS,
      /*out*/ RETURN_CODE_TYPE          *RETURN_CODE)
{
  pok_ret_t core_ret;
  pok_port_queuing_status_t status;
  
    if (QUEUING_PORT_ID <= 0) {
       *RETURN_CODE = INVALID_PARAM;
       return;
    }

   core_ret = pok_port_queuing_status(QUEUING_PORT_ID - 1, &status);
   if (core_ret == POK_ERRNO_OK) {
       QUEUING_PORT_STATUS->NB_MESSAGE = status.nb_message;
       QUEUING_PORT_STATUS->MAX_NB_MESSAGE = status.max_nb_message;
       QUEUING_PORT_STATUS->MAX_MESSAGE_SIZE = status.max_message_size;
       switch (status.direction) {
         case POK_PORT_DIRECTION_OUT:
          QUEUING_PORT_STATUS->PORT_DIRECTION = SOURCE;
          break;
         case POK_PORT_DIRECTION_IN:
          QUEUING_PORT_STATUS->PORT_DIRECTION = DESTINATION;
          break;
         default:
          break; // XXX assert(0)
       }
       QUEUING_PORT_STATUS->WAITING_PROCESSES = status.waiting_processes;
       *RETURN_CODE = NO_ERROR;
   } else {
       *RETURN_CODE = INVALID_PARAM;
   }
}

void CLEAR_QUEUING_PORT (
      /*in */ QUEUING_PORT_ID_TYPE      QUEUING_PORT_ID,
      /*out*/ RETURN_CODE_TYPE          *RETURN_CODE)
{
   pok_ret_t core_ret = pok_port_queuing_clear(QUEUING_PORT_ID - 1);

   switch (core_ret) {
        MAP_ERROR(POK_ERRNO_OK, NO_ERROR);
        MAP_ERROR(POK_ERRNO_PORT, INVALID_PARAM);
        MAP_ERROR(POK_ERRNO_MODE, INVALID_MODE);
        MAP_ERROR_DEFAULT(INVALID_CONFIG);
    }
}

#endif
