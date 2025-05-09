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

#ifndef __LIBJET_ARINC_ALLOC_H__
#define __LIBJET_ARINC_ALLOC_H__

/* Allocator of memory for messages and message queues. */

#include <config.h>

#if defined(POK_NEEDS_ARINC653_BUFFER) || defined(POK_NEEDS_ARINC653_BLACKBOARD)

#include "include/types.h"

// Heap for ARINC messages and message queues. Used in arinc_init().
extern char* arinc_intra_heap;

/* 
 * Allocate memory of given size.
 * 
 * Memory will have alignment suitable for any object which fits into
 * given size.
 */
void* arinc_alloc(size_t size, size_t alignment);

/*
 * State of the arinc allocator.
 */
typedef size_t arinc_allocator_state;

/*
 * Return current state of the allocator.
 */
arinc_allocator_state arinc_allocator_get_state(void);

/*
 * Restore state of the arinc allocator.
 * State should be previously obtained with arinc_allocator_get_state().
 */
void arinc_allocator_reset_state(arinc_allocator_state state);

#endif /* defined(POK_NEEDS_ARINC653_BUFFER) || defined(POK_NEEDS_ARINC653_BLACKBOARD) */

#endif /* __LIBJET_ARINC_ALLOC_H__ */
