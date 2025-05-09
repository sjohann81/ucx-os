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

#ifndef __POK_THREAD_H__
#define __POK_THREAD_H__

#include "config.h"

#include <core/dependencies.h>

#ifdef POK_NEEDS_THREADS

#define POK_THREAD_DEFAULT_PRIORITY 42

#define POK_DEFAULT_STACK_SIZE 2048

#include <uapi/thread_types.h>
#include <core/syscall.h>

// Renames for system calls
#define pok_thread_period pok_sched_end_period
#define pok_thread_status pok_thread_get_status

#endif /* __POK_NEEDS_THREADS */
#endif /* __POK_THREAD_H__ */
