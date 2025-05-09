/*
 * COPIED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify original one (kernel/include/uapi/blackboard_types.h).
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

#ifndef __JET_UAPI_BLACKBOARD_TYPES_H__
#define __JET_UAPI_BLACKBOARD_TYPES_H__

#include <uapi/types.h>

typedef struct
{
   pok_message_size_t   max_message_size;
   pok_bool_t           is_empty;
   pok_range_t          waiting_processes;
}pok_blackboard_status_t;

#endif /* __JET_UAPI_BLACKBOARD_TYPES_H__ */
