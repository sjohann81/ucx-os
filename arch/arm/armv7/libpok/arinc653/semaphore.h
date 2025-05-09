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

#ifndef __LIBJET_ARINC_SEMAPHORE_H__
#define __LIBJET_ARINC_SEMAPHORE_H__

#include "config.h"

#ifdef POK_NEEDS_ARINC653_SEMAPHORE
#include <arinc653/types.h>
#include <arinc653/semaphore.h>
#include <msection.h>
#include "include/types.h"

struct arinc_semaphore
{
    SEMAPHORE_NAME_TYPE semaphore_name;
    
    SEMAPHORE_VALUE_TYPE current_value;
    SEMAPHORE_VALUE_TYPE maximum_value;

    QUEUING_DISCIPLINE_TYPE discipline;
    
    struct msection section;
    struct msection_wq process_queue;
};

/* Preallocated array of semaphores. */
extern struct arinc_semaphore* arinc_semaphores;

#endif /* POK_NEEDS_ARINC653_SEMAPHORE */

#endif /* __LIBJET_ARINC_SEMAPHORE_H__ */
