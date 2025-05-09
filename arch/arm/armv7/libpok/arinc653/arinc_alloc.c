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

#include "arinc_alloc.h"
#if defined(POK_NEEDS_ARINC653_BUFFER) || defined(POK_NEEDS_ARINC653_BLACKBOARD)

#include <arinc_config.h>
#include <utils.h>

size_t arinc_intra_heap_size_current = 0;

/* 
 * Allocate memory of given size.
 * 
 * Memory will have alignment suitable for any object which fits into
 * given size.
 */
void* arinc_alloc(size_t size, size_t alignment)
{
    size_t size_start = ALIGN(arinc_intra_heap_size_current, alignment);
    size_t size_end = size_start + size;

    if(size_end > arinc_config_messages_memory_size) return NULL;

    arinc_intra_heap_size_current = size_end;

    return arinc_intra_heap + size_start;
}

/*
 * Return current state of the allocator.
 */
arinc_allocator_state arinc_allocator_get_state(void)
{
    return arinc_intra_heap_size_current;
}

/*
 * Restore state of the arinc allocator.
 * State should be previously obtained with arinc_allocator_get_state().
 */
void arinc_allocator_reset_state(arinc_allocator_state state)
{
    arinc_intra_heap_size_current = state;
}

#endif /* defined(POK_NEEDS_ARINC653_BUFFER) || defined(POK_NEEDS_ARINC653_BLACKBOARD) */
