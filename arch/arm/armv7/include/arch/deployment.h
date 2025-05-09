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

/* Definitions which are used in deployment.c */

#ifndef __JET_ARMV7_DEPLOYMENT_H__
#define __JET_X86_DEPLOYMENT_H__

#include <stddef.h>
#include <stdint.h>

/*
 * Description of one user space.
 */
struct ja_armv7_space
{
    /*
     * Physical address of memory chunk.
     *
     * If it is set to 0 in deployment.c, it will be filled upon space initialization.
     */
    uint32_t   phys_base;

    /*
     * Size of the memory  (code and static data).
     *
     * Set in deployment.c.
     */
    size_t      size_normal;

    /*
     * Size of the memory  (heap).
     *
     * Set in deployment.c.
     */
    size_t      size_heap;

    /*
     * Size of the memory for stacks.
     *
     * Set in deployment.c.
     */
    size_t size_stack;

    /*
     * Total size for partition's use.
     *
     * Calculated upon space initialization.
     */
    size_t size_total;

    /*
     * Offset from the beginning of the space to the ending of the heap.
     *
     * Calculated upon space initialization.
     */
    size_t heap_end;
    /*
     * Memory currently used for stacks.
     */
    size_t size_stack_used;

    uint32_t mpu_base[11];
    uint32_t mpu_size[11];
    uint32_t mpu_reg;
    uint32_t mpu_type;
    uint32_t mpu_perm;
};

/*
 * Array of user space descriptions.
 *
 * Should be defined in deployment.c.
 */
extern struct ja_armv7_space ja_spaces[];
extern int ja_spaces_n;

#endif /* __JET_X86_DEPLOYMENT_H__ */
