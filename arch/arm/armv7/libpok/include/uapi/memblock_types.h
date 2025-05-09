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

#ifndef __JET_UAPI_MEMBLOCK_TYPES_H__
#define __JET_UAPI_MEMBLOCK_TYPES_H__

#include <uapi/types.h>

typedef enum {
    JET_MEMORY_BLOCK_READ,
    JET_MEMORY_BLOCK_READ_WRITE
} jet_memory_block_mode_t;

typedef struct {
    uintptr_t addr;
    jet_memory_block_mode_t mode;
    size_t size;
} jet_memory_block_status_t;


#endif /* __JET_UAPI_MEMBLOCK_TYPES_H__ */
