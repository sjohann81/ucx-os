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

// #include <asp/uaccess.h>
#include <libpok_legacy/assert.h>
#include <libpok_legacy/space.h>
// #include <libpok_legacy/deployment.h>
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

// TODO: Revisit (see also pok_space_* code in arch.h)
static pok_bool_t arm_check_access(const void* __user addr, size_t size,
    jet_space_id space_id)
{
    assert(space_id != 0);
    assert(size != 0);

    unsigned long start = (unsigned long)addr;
    unsigned long end = start + size;

    struct ja_armv7_space* space = &ja_spaces[space_id - 1];

    /*
     * Currently, there are 2 segments accessible to user:
     * 1. [POK_PARTITION_MEMORY_BASE; POK_PARTITION_MEMORY_BASE + space->heap_end)
     *    code,data and heap
     * 2. [POK_PARTITION_MEMORY_BASE + space->size_total - space->size_stack_used; POK_PARTITION_MEMORY_BASE + space->size_total)
     *    stacks
     */

    if(end < start) return FALSE; // Segments doesn't cross NULL.

    if(end <= space->heap_end + space->phys_base)
    {
        return (start >= space->phys_base);
    }
    else if(end <= space->phys_base + space->size_total)
    {
        return (start >= space->phys_base + space->size_total - space->size_stack_used);
    }
    else
    {
        return FALSE;
    }
}

#if 0
static uint32_t user_space_shift(jet_space_id space_id)
{
    assert(space_id != 0);
    struct ja_armv7_space* space = &ja_spaces[space_id - 1];

    return space->phys_base;
}
#endif

void* __kuser ja_user_to_kernel_space(void* __user addr, size_t size,
    jet_space_id space_id)
{
    if(arm_check_access(addr, size, space_id))
       return (void* __kuser)addr;
    else
       return NULL;
}

const void* __kuser ja_user_to_kernel_ro_space(const void* __user addr,
    size_t size, jet_space_id space_id)
{
    if(arm_check_access(addr, size, space_id))
       return (const void* __kuser)addr;
    else
       return NULL;
}

pok_bool_t ja_check_access_exec(void* __user addr, jet_space_id space_id)
{
    assert(space_id != 0);

    unsigned long start = (unsigned long)addr;

    struct ja_armv7_space* space = &ja_spaces[space_id - 1];

    /*
     * Only single segment could be executed by user:
     *   [POK_PARTITION_MEMORY_BASE; POK_PARTITION_MEMORY_BASE + space->size_normal)
     *    code and data
     */

    return (start < space->phys_base + space->size_normal)
        && (start >= space->phys_base);
}
