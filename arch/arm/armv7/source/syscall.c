/*
 * syscall.c
 *
 *  Created on: 15 mai 2019
 *      Author: Alexy
 */

/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team
 *
 * Created by julien on Thu Jan 15 23:34:13 2009
 */


#include <errno.h>
#include <core/debug.h>
#include <core/syscall.h>
#include <core/partition.h>

#include "include/types.h"
#include <libc.h>

pok_ret_t pok_arch_sc_int(uint32_t num, pok_syscall_args_t* args)
{

   uint8_t              part_id;
   pok_syscall_info_t   syscall_info;
   pok_syscall_id_t     syscall_id;

   //part_id  = pok_current_partition;
   part_id  = 0; // One can use `current_partition` variable for get current partition.

   /* prepare syscall_info */
   syscall_info.partition = part_id;
   syscall_info.base_addr = 0; //pok_partitions[part_id].base_addr; // TODO need a cleaner way
   //syscall_info.thread    = POK_SCHED_CURRENT_THREAD;
   syscall_info.thread    = 0; // One can use `current_thread` variable for get current thread.

   /* prepare syscall_id */
   syscall_id = (pok_syscall_id_t) num;

   return pok_core_syscall (syscall_id, args, &syscall_info);
}
