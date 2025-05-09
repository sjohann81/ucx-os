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

#ifndef __LIBJET_ARINC_BUFFER_H__
#define __LIBJET_ARINC_BUFFER_H__

#include <config.h>

#ifdef POK_NEEDS_ARINC653_BUFFER
#include <arinc653/types.h>
#include <arinc653/buffer.h>
#include <msection.h>
#include "include/types.h"

struct arinc_buffer
{
    BUFFER_NAME_TYPE buffer_name;
    
    MESSAGE_SIZE_TYPE max_message_size;
    MESSAGE_RANGE_TYPE max_nb_message;
    MESSAGE_RANGE_TYPE nb_message;
    
    MESSAGE_SIZE_TYPE message_stride;
    
    char* messages;
    MESSAGE_SIZE_TYPE* messages_size;
    
    MESSAGE_RANGE_TYPE base_offset;
    
    QUEUING_DISCIPLINE_TYPE discipline;
    
    struct msection section;
    struct msection_wq process_queue;
};

/* Preallocated array of buffers. */
extern struct arinc_buffer* arinc_buffers;

#endif /* POK_NEEDS_ARINC653_BUFFER */

#endif /* __LIBJET_ARINC_BUFFER_H__ */
