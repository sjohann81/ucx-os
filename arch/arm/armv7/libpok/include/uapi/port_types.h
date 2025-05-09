/*
 * COPIED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify original one (kernel/include/uapi/port_types.h).
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
 */

#ifndef __JET_UAPI_PORT_TYPES_H__
#define __JET_UAPI_PORT_TYPES_H__

#include <uapi/types.h>

typedef enum
{
	 POK_PORT_DIRECTION_IN   = 1,
	 POK_PORT_DIRECTION_OUT  = 2
} pok_port_directions_t;

/* Status for queuing port, for return into user space. */
typedef struct
{
   pok_port_size_t      nb_message;
   pok_port_size_t      max_nb_message;
   pok_port_size_t      max_message_size;
   pok_port_direction_t direction;
   uint8_t              waiting_processes;
} pok_port_queuing_status_t;

/* 
 * Packaging for arguments passed to syscall
 * POK_SYSCALL_MIDDLEWARE_QUEUEING_CREATE.
 */
typedef struct
{
    pok_port_size_t message_size;
    pok_port_size_t max_nb_message;
    pok_port_direction_t direction;
    pok_queuing_discipline_t discipline;
} pok_port_queuing_create_arg_t;

/* Status for sampling port, for return into user space. */
typedef struct
{
   pok_port_size_t      size;
   pok_port_direction_t direction;
   uint64_t             refresh;
   pok_bool_t           validity;
}pok_port_sampling_status_t;

#endif /* __JET_UAPI_PORT_TYPES_H__ */
