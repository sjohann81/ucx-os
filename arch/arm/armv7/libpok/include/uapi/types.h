/*
 * COPIED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify original one (kernel/include/uapi/types.h).
 */
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


#ifndef __JET_UAPI_TYPES_H__
#define __JET_UAPI_TYPES_H__

#include <stdint.h>
#include <stddef.h>

#define FALSE  0
#define TRUE   1
//typedef int bool_t;
typedef int pok_bool_t;

typedef uint32_t pok_port_size_t;
typedef uint8_t pok_port_direction_t;
typedef uint8_t pok_port_kind_t;
typedef uint8_t pok_port_id_t;
typedef uint8_t pok_size_t;
typedef uint8_t pok_range_t; // Range of messages is different.
typedef uint8_t pok_buffer_id_t;
typedef uint8_t pok_blackboard_id_t;
typedef uint16_t pok_sem_id_t;
typedef uint16_t pok_event_id_t;
typedef uint8_t pok_partition_id_t;
typedef uint8_t pok_thread_id_t;
typedef uint16_t pok_sem_value_t;

typedef uint16_t pok_message_size_t; // 0....8192
typedef uint16_t pok_message_range_t; // 0...512

typedef int64_t pok_time_t;

/* This constant should be used for *set* time as infinity(special). */
#define POK_TIME_INFINITY ((int64_t)(-1))

typedef enum {
    POK_QUEUING_DISCIPLINE_FIFO,
    POK_QUEUING_DISCIPLINE_PRIORITY,
} pok_queuing_discipline_t;

#define MAX_NAME_LENGTH 30

/*
 * Special value of type pok_thread_id_t, which cannot be id of actual thread.
 */
#define JET_THREAD_ID_NONE (pok_thread_id_t)(-1)


#endif /* __JET_UAPI_TYPES_H__ */
