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

#include "arinc_process_queue.h"
#include <arinc653/process.h>
#include <kernel_shared_data.h>

/* Add current process to the end of the waitqueue. */
void arinc_process_queue_add(struct msection_wq* wq)
{
    msection_wq_add(wq, JET_THREAD_ID_NONE);
}

/* Add current process to the waitqueue according to discipline. */
void arinc_process_queue_add_common(struct msection_wq* wq,
    QUEUING_DISCIPLINE_TYPE discipline)
{
    if(discipline == PRIORITY)
    {
        PRIORITY_TYPE priority = kshd.tshd[kshd.current_thread_id].priority;
        
        pok_thread_id_t prev = wq->last;
        
        while(prev != JET_THREAD_ID_NONE)
        {
            if(kshd.tshd[prev].wq_priority >= priority) break;
            
            prev = kshd.tshd[prev].wq_prev;
        }
        
        msection_wq_add_after(wq, prev);
        
        kshd.tshd[kshd.current_thread_id].wq_priority = priority;
    }
    else
    {
        msection_wq_add(wq, JET_THREAD_ID_NONE);
    }
}
