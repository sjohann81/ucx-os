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

#ifndef __LIBJET_ASP_SYSCALL_H__
#define __LIBJET_ASP_SYSCALL_H__

#include "config.h"

#include <uapi/syscall_types.h>

#include <arch/syscall.h>
#include "uapi/errno.h"

#ifndef LIBJET_ARCH_DECLARE_SYSCALL

/* 
 * Call syscall with given id and number of arguments.
 * 
 * If arch defines macro 'LIBJET_ARCH_DECLARE_SYSCALL', it should provide
 * definitions with same usage.
 */
pok_ret_t lja_syscall0  (pok_syscall_id_t syscall_id);

pok_ret_t lja_syscall1  (pok_syscall_id_t syscall_id,
        uint32_t arg1);

pok_ret_t lja_syscall2  (pok_syscall_id_t syscall_id,
        uint32_t         arg1,
        uint32_t         arg2);

pok_ret_t lja_syscall3 (pok_syscall_id_t  syscall_id,
        uint32_t          arg1,
        uint32_t          arg2,
        uint32_t          arg3);

pok_ret_t lja_syscall4 (pok_syscall_id_t  syscall_id,
        uint32_t          arg1,
        uint32_t          arg2,
        uint32_t          arg3,
        uint32_t          arg4);

pok_ret_t lja_syscall5 (pok_syscall_id_t  syscall_id,
        uint32_t arg1,
        uint32_t arg2,
        uint32_t arg3,
        uint32_t arg4,
        uint32_t arg5);

#endif /* LIBJET_ARCH_DECLARE_SYSCALL */

#endif /* __LIBJET_ASP_SYSCALL_H__ */
