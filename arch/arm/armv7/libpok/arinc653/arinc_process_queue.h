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

#ifndef __LIBJET_ARINC_PROCESS_QUEUE_H__
#define __LIBJET_ARINC_PROCESS_QUEUE_H__

#include <arinc653/types.h>
#include <msection.h>

/* 
 * ARINC Process queue is just a msection waitqueue plus kernel
 * shared data.
 */

/* Add current process to the end of the waitqueue. */
void arinc_process_queue_add(struct msection_wq* wq);

/* Add current process to the waitqueue according to discipline. */
void arinc_process_queue_add_common(struct msection_wq* wq,
    QUEUING_DISCIPLINE_TYPE discipline);


#endif /* __LIBJET_ARINC_PROCESS_QUEUE_H__ */
