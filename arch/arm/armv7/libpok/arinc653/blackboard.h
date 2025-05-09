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

#ifndef __LIBJET_ARINC_BLACKBOARD_H__
#define __LIBJET_ARINC_BLACKBOARD_H__

#include <config.h>

#ifdef POK_NEEDS_ARINC653_BLACKBOARD
#include <arinc653/types.h>
#include <arinc653/blackboard.h>
#include <msection.h>
#include "include/types.h"

struct arinc_blackboard
{
    BLACKBOARD_NAME_TYPE blackboard_name;
    
    MESSAGE_SIZE_TYPE max_message_size;
    
    char* message;
    MESSAGE_SIZE_TYPE message_size; // 0 means absent of the message.
    
    struct msection section;
    struct msection_wq process_queue;
};

/* Preallocated array of blackboards. */
extern struct arinc_blackboard* arinc_blackboards;

#endif /* POK_NEEDS_ARINC653_BLACKBOARD */

#endif /* __LIBJET_ARINC_BLACKBOARD_H__ */
