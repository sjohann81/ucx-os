/*
 * syscall.h
 *
 *  Created on: 15 mai 2019
 *      Author: Alexy
 */

#ifndef SOURCES_LIBPOK_ARCH_ARMV7_INCLUDE_SYSCALL_H_
#define SOURCES_LIBPOK_ARCH_ARMV7_INCLUDE_SYSCALL_H_

#include <core/syscall.h>
#include <core/partition.h>

#include "include/types.h"
#include "uapi/errno.h"


#define LIBJET_ARCH_DECLARE_SYSCALL 1

typedef struct
{
     uint32_t             nargs;
     uint32_t             arg1;
     uint32_t             arg2;
     uint32_t             arg3;
     uint32_t             arg4;
     uint32_t             arg5;
} pok_syscall_args_t;



pok_ret_t lja_do_syscall (pok_syscall_id_t syscall_id, pok_syscall_args_t* args);

#define lja_syscall0(sid) \
             lja_do_syscall(sid,&((pok_syscall_args_t){0,0,0,0,0,0}))

#define lja_syscall1(sid,arg1) \
             lja_do_syscall(sid,&((pok_syscall_args_t){1,arg1,0,0,0,0}))

#define lja_syscall2(sid,arg1,arg2) \
             lja_do_syscall(sid,&((pok_syscall_args_t){2,arg1,arg2,0,0,0}))

#define lja_syscall3(sid,arg1,arg2,arg3) \
             lja_do_syscall(sid,&((pok_syscall_args_t){3,arg1,arg2,arg3,0,0}))

#define lja_syscall4(sid,arg1,arg2,arg3,arg4) \
             lja_do_syscall(sid,&((pok_syscall_args_t){4,arg1,arg2,arg3,arg4,0}))

#define lja_syscall5(sid,arg1,arg2,arg3,arg4,arg5) \
             lja_do_syscall(sid,&((pok_syscall_args_t){5,arg1,arg2,arg3,arg4,arg5}))


#endif /* SOURCES_LIBPOK_ARCH_ARMV7_INCLUDE_SYSCALL_H_ */
