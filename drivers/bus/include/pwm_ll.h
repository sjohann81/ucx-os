long pwm_ll_setup(struct pwm_config_values_s *cfg);
int pwm_ll_get(struct pwm_config_values_s *cfg, unsigned channel, unsigned *pulse);
int pwm_ll_set(struct pwm_config_values_s *cfg, unsigned channel, unsigned pulse);
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

 #ifndef __JET_ASP_SPACE_H__
 #define __JET_ASP_SPACE_H__
 
 #include <libpok_legacy/types.h>
 #include <libpok_legacy/cswitch.h>
 #include <libpok_legacy/common.h>
 #include <errno.h>
 #include <libpok_legacy/kernel_shared_data.h>
 #include <arch/space.h>
 
 /* 
  * Arch header should define:
  * 
  * type 'jet_ustack_t' - pointer to allocated user stack.
  *   Value 0 should be assignable and comparable to given type, and
  *   never corresponds to allocated stack.
  */
 #include <space.h>
 
 /* 
  * Identificator for the (memory) space.
  * 
  * Value 0 corresponds to kernel space.
  * Non-zero values correspond to user spaces.
  */
 typedef uint8_t jet_space_id;
 
 /* Layout of the user space for load partition's code and data. */
 struct jet_space_layout
 {
     /* 
      * Currently whole space is treated as single chunk.
      * 
      * TODO: There should be division for execute-only, read-only and
      * other sections. But this requires arch support.
      */
     char* kernel_addr;
     char* user_addr;
     size_t size;
 };
 
 /* Fill layout for given user space. */
 void ja_space_layout_get(jet_space_id space_id,
     struct jet_space_layout* space_layout);
 
 /* Return pointer to the heap for given space. */
 void* ja_space_get_heap(jet_space_id space_id);
 
 /* Return pointer to the kernel shared data for given space. */
 struct jet_kernel_shared_data* __kuser ja_space_shared_data(jet_space_id space_id);
 
 /**
  * Jump to the user space.
  * 
  * Kernel stack passed as 'sp' will be used in interrupts/syscalls.
  */
 void ja_user_space_jump(
     jet_stack_t stack_kernel,
     jet_space_id space_id, /* Actually, unused (should already be set with ja_space_switch). */
     void (__user * entry_user)(void),
     jet_ustack_t stack_user);
 
 /**
  * Switch to given space.
  */
 void   ja_space_switch (jet_space_id new_space_id);
 
 /*
  * Return id of current space.
  */
 jet_space_id ja_space_get_current (void);
 
 /* 
  * (Re)initialize user stacks allocator for given space.
  * 
  * 'space_id' shouldn't be 0.
  */
 void ja_ustack_init (jet_space_id space_id);
 
 /**
  * Allocates user stack for given space.
  * 
  * In case of insufficient memory returns 0.
  * 
  * 'space_id' shouldn't be 0.
  */
 jet_ustack_t ja_ustack_alloc (jet_space_id space_id, size_t stack_size);
 
 /*
  * Place for store floating point registers.
  * 
  * Kernel code doesn't use floating point operations,
  * so floating point registers needs to be stored only when switching
  * to other user space thread.
  */
 struct jet_fp_store;
 
 /* 
  * Allocate place for store floating point registers.
  * 
  * May be called only during OS init.
  */
 struct jet_fp_store* ja_alloc_fp_store(void);
 
 /* Save floating point registers into given place. */
 void ja_fp_save(struct jet_fp_store* fp_store);
 
 /* Restore floating point registers into given place. */
 void ja_fp_restore(struct jet_fp_store* fp_store);
 
 /* Initialize floating point registers with zero. */
 void ja_fp_init(void);
 
 
 #endif /* __JET_ASP_SPACE_H__ */
 //TODO: Move line 4 to end to another file