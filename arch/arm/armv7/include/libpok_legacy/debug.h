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

#include <libpok_legacy/config.h>

#ifndef __POK_KERNEL_DEBUG_H_
#define __POK_KERNEL_DEBUG_H_

#ifdef POK_NEEDS_DEBUG

// #include <libpok_legacy/errno.h>
#include <libpok_legacy/types.h>
#include <libc.h>

__attribute__((__noreturn__))
void		pok_fatal(const char* message);

int      debug_strlen (const char* str);

void hexdump (const void *addr, int len);

/* Print formatted message if debugging enabled. Otherwise do nothing.*/
#define printf_debug printf

#define POK_FATAL(arg) pok_fatal(arg)

#define POK_DEBUG_PRINT_CURRENT_STATE pok_debug_print_current_state();

#else /* POK_NEEDS_DEBUG */
#define printf_debug(...) do {} while(0)
#define POK_DEBUG_PRINT_CURRENT_STATE
#define POK_FATAL(arg)
#endif /* POK_NEEDS_DEBUG */

#endif /* __POK_KERNELDEBUG_H__ */

