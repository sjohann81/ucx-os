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

/* Unsorted arch-specific functions. */

#ifndef __JET_ASP_ARCH_H__
#define __JET_ASP_ARCH_H__

#include <libpok_legacy/types.h>

// TODO: Where should be that definition?
#define KERNEL_STACK_SIZE_DEFAULT 8192

/**
 * Disable interrupts
 */
void ja_preempt_disable (void);

/**
 * Enable interrupts
 */
void ja_preempt_enable (void);

/**
 * Returns true if interrupts are enabled
 */
pok_bool_t ja_preempt_enabled(void);

/**
 * Function that do nothing. Useful for the idle task for example.
 */
void ja_inf_loop(void);

/*
 * reset cpu
 */
void ja_cpu_reset(void);

#endif /* !__POK_ARCH_H__ */
