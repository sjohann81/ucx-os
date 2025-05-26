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

#ifndef __POK_SYSCALL_H__
#define __POK_SYSCALL_H__

#include <libpok_legacy/config.h>

#include <libpok_legacy/types.h>
// #include <libpok_legacy/errno.h>
#include <libpok_legacy/common.h>

#include <libpok_legacy/syscall_types.h>

typedef struct
{
   uint32_t             nargs;
   uint32_t             arg1;
   uint32_t             arg2;
   uint32_t             arg3;
   uint32_t             arg4;
   uint32_t             arg5;
} pok_syscall_args_t;

typedef struct
{
   pok_partition_id_t   partition;
   uint32_t             thread;
   uint32_t             base_addr;
}pok_syscall_info_t;

// Should come after definitions of types 'pok_syscall_args_t' and 'pok_syscall_info_t'
#include <libpok_legacy/syscall_map_arinc.h>


/**
 *  Function that performs the syscall. It is called by the 
 *  architecture interruption handler.
 *
 *  @param syscall_id
 *  This param correspond to the syscal which should be performed.
 *  The list of available syscalls is available in
 *  the definition of the pok_syscall_id_t type
 *
 *  @param args
 *  Arguments of the syscall. It corresponds to data useful
 *  to perform the syscall.
 *
 *  @param infos
 *  Informations about the syscall: which partition/thread
 *  initiates the syscall, etc ...
 *
 *  @return
 *  Returns an error code, which is defined in include/errno.h
 */

int	pok_core_syscall (const pok_syscall_id_t     syscall_id,
                              const pok_syscall_args_t*  args,
                              const pok_syscall_info_t*  infos);

/*
 * Initiate syscalls.
 * This part is defined in low-level layers, each architecture/bsp
 * initiate the syscall in a different way.
 */
int pok_syscall_init();

#endif /* __POK_SYSCALL_H__ */
