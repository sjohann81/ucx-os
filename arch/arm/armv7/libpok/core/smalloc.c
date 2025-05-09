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

#include <smalloc.h>
#include <kernel_shared_data.h>
#include <../include/core/assert_os.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

void* smalloc_aligned(size_t size, size_t alignment)
{
    assert_os(kshd.partition_mode != POK_PARTITION_MODE_NORMAL);

    if(size == 0) return NULL;

    char* obj_start = ALIGN_PTR(heap_current, alignment);
    char* obj_end = obj_start + size;

    if(obj_end > kshd.heap_end)
    {
        printf("ERROR: Failed to allocate memory from the heap.\n");
        printf("NOTE: Probably, you need to configure more heap for the partition.\n");
        abort();
    }

    heap_current = obj_end;

    return obj_start;
}

void* smalloc(size_t size)
{
    return smalloc_aligned(size, libja_mem_get_alignment(size));
}

void* scalloc(size_t nmemb, size_t size)
{
    if(size == 0 || nmemb == 0) return NULL;

    if(nmemb == 1) return smalloc(size);

    size_t alignment = libja_mem_get_alignment(size);
    size_t stride = ALIGN(size, alignment);

    char* mem_start = ALIGN_PTR(heap_current, alignment);
    char* mem_end = mem_start + stride * (nmemb - 1) + size;

    if(mem_end > kshd.heap_end)
    {
        printf("ERROR: Failed to allocate memory from the heap.\n");
        printf("NOTE: Probably, you need to configure more heap for the partition.\n");
        abort();
    }

    heap_current = mem_end;

    return mem_start;
}
