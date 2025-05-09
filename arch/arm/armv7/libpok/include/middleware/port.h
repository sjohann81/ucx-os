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

#ifndef __POK_LIBPOK_PORTS_H__
#define __POK_LIBPOK_PORTS_H__

#include "config.h"

#include <core/dependencies.h>

#include <uapi/port_types.h>
#include <core/syscall.h>

// Wrapper around system call with packed parameters
#define pok_port_queuing_create(name, _message_size, _max_nb_message, _direction, _discipline, id) \
   ({pok_port_queuing_create_arg_t args = {\
         .message_size = _message_size,\
         .max_nb_message = _max_nb_message,\
         .direction = _direction,\
         .discipline = _discipline}; \
         pok_port_queuing_create_packed(name, &args, id);\
   })

#endif
