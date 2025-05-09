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

#include <kernel_shared_data.h>
#include <init_arinc.h>
#include <smalloc.h>

int main();

char* heap_current = NULL;

int __pok_partition_start (void)
{
   // Setup user-only fields of kernel shared data.
   kshd.main_thread_id = kshd.current_thread_id;
   kshd.error_thread_id = JET_THREAD_ID_NONE;

   heap_current = kshd.heap_start;

   libjet_arinc_init();

   main(); /* main loop from user */
   return (0);
}
