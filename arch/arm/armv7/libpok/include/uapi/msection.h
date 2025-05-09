/*
 * COPIED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify original one (kernel/include/uapi/msection.h).
 */
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

#ifndef __JET_UAPI_MSECTION_H__
#define __JET_UAPI_MSECTION_H__

#include "include/types.h"
#include "uapi/errno.h"
/* 
 * Mutual section.
 * 
 * Provide critical section, when high-priority thread doesn't go into
 * waiting state when try to enter into busy section.
 * 
 * Instead, owner of the section is executed until it leaves the section.
 * Then, high-priority thread continues it execution.
 */

struct msection
{
    /* Owner thread or JET_THREAD_ID_NONE. */
    pok_thread_id_t owner;

    /* Flags which are set by the kernel and are checked by the user. */
    uint8_t msection_kernel_flags;
};

/* When leaving the section, need to call jet_resched(). */
#define MSECTION_KERNEL_FLAG_RESCHED_AFTER_LEAVE 1

/* 
 * Queue of waiting processes protected by waitqueue.
 * 
 * Linkage of elements is within 'struct jet_thread_shared_data'.
 */
struct msection_wq
{
    // The first thread in the queue. JET_THREAD_ID_NONE if queue is empty.
    pok_thread_id_t first;
    // The last thread in the queue. JET_THREAD_ID_NONE if queue is empty.
    pok_thread_id_t last;
};

#endif /* __JET_UAPI_MSECTION_H__ */
