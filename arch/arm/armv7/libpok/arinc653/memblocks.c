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

#include "config.h"
#include <arinc653/memblocks.h>
#include <core/syscall.h>
#include <uapi/memblock_types.h>



void GET_MEMORY_BLOCK_STATUS (
        /*in */ MEMORY_BLOCK_NAME_TYPE   MEMORY_BLOCK_NAME,
        /*out*/ MEMORY_BLOCK_STATUS_TYPE *MEMORY_BLOCK_STATUS,
        /*out*/ RETURN_CODE_TYPE         *RETURN_CODE)
{
    pok_ret_t core_ret;
    jet_memory_block_status_t mb_status;


    core_ret = pok_memory_block_get_status(MEMORY_BLOCK_NAME,
        &mb_status);




    if (core_ret != POK_ERRNO_OK) {
        *RETURN_CODE = INVALID_CONFIG;
    } else {
        *RETURN_CODE = NO_ERROR;
        MEMORY_BLOCK_STATUS->MEMORY_BLOCK_ADDR = (void *)mb_status.addr;
        MEMORY_BLOCK_STATUS->MEMORY_BLOCK_MODE = mb_status.mode;
        MEMORY_BLOCK_STATUS->MEMORY_BLOCK_SIZE = mb_status.size;
    }

}
