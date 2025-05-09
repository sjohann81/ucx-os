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

#ifndef __LIBPOK_SYSCALL_H__
#define __LIBPOK_SYSCALL_H__

#include "config.h"

#include <core/dependencies.h>

#include "include/types.h"
#include <errno.h>

#include <uapi/syscall_types.h>


#include <asp/syscall.h>

#define pok_syscall0 lja_syscall0
#define pok_syscall1 lja_syscall1
#define pok_syscall2 lja_syscall2
#define pok_syscall3 lja_syscall3
#define pok_syscall4 lja_syscall4
#define pok_syscall5 lja_syscall5

// This should come after pok_syscall* definitions
#include <uapi/syscall_map_arinc.h>

#endif /* __LIBPOK_SYSCALL_H__ */
