/*
 * COPIED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify original one (kernel/include/uapi/thread_types.h).
 */
/*
 * Institute for System Programming of the Russian Academy of Sciences
 * Copyright (C) 2014, 2016 ISPRAS
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

#ifndef __JET_UAPI_THREAD_TYPES_H__
#define __JET_UAPI_THREAD_TYPES_H__

#include <uapi/types.h>

typedef enum
{
  // comments describe to what states of ARINC653 these correspond to
  POK_STATE_STOPPED = 0, // DORMANT (must be started first)
  POK_STATE_RUNNABLE = 1, // READY or RUNNING. When return status to user space it means READY.
  POK_STATE_WAITING = 2, // WAITING for any event except resuming(but for timed suspend exactly this state is used).
  POK_STATE_RUNNING = 3, // RUNNING. Used only for return status to user space.
} pok_state_t;

typedef enum {
    DEADLINE_SOFT,
    DEADLINE_HARD
} pok_deadline_t;

typedef struct
{
    uint8_t      	priority;
    pok_time_t	   	period;
    pok_deadline_t	deadline;
    pok_time_t   	time_capacity;
    uint32_t     	stack_size;
} pok_thread_attr_t;

typedef struct 
{
    pok_thread_attr_t   attributes;
    pok_time_t          deadline_time;
    pok_state_t         state;
    uint8_t             current_priority;
} pok_thread_status_t;


#endif /* __JET_UAPI_THREAD_TYPES_H__ */
