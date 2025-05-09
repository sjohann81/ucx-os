/*
 * COPIED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify original one (kernel/include/uapi/kernel_shared_data.h).
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

#ifndef __JET_UAPI_KERNEL_SHARED_DATA_H__
#define __JET_UAPI_KERNEL_SHARED_DATA_H__

#include "include/types.h"
#include <uapi/partition_arinc_types.h>
#include <uapi/msection.h>

/* Data about the thread, shared between kernel and user spaces. */
struct jet_thread_shared_data
{
    /* 
     * Priority of the current thread.
     * 
     * This value may be changed by the kernel at any time.
     */
    volatile uint8_t priority;

    /* 
     * Count of currently entered msections.
     * 
     * This value is controlled from user space and checked in kernel.
     * 
     * DEV: Actually kernel needs only zero-or-not comparision.
     */
    uint16_t msection_count;
    /* 
     * If thread is currently entering msection, this is a pointer to it.
     * 
     * If thread is switched with this field is set, switching back
     * to the thread automatically enters into the critical section.
     * 
     * DEV: This field is needed only on single-CPU for omit CAS
     * instruction on section entering.
     */
    struct msection* volatile msection_entering;

    /* User space may "signal" kernel by setting these flags. */
    volatile uint8_t thread_kernel_flags;

    /* 
     * Next and previous threads in the waitqueue protected by msection
     * ('struct msection_wq').
     * 
     * When thread is removed from the queue, both fields should be set
     * to JET_THREAD_ID_NONE.
     * 
     * NOTE: Using these fields only it is impossible to distinguish
     * "single element" case from "thread doesn't belong to waitqueue".
     * One need to check waitqueue itself for that.
     */
    pok_thread_id_t wq_next; // JET_THREAD_ID_NONE for the last thread.
    pok_thread_id_t wq_prev; // JET_THREAD_ID_NONE for the first thread.

    /*
     * Pointer to source or destination buffers when implements
     * waiting on ARINC buffer/blackboard.
     * 
     * Used only by user space.
     */
    union
    {
        const void* src;
        void* dst;
    } wq_buffer;
    /* 
     * Length of copied message for ARINC purposes.
     * 
     * This could be an input or output parameter.
     * 
     * Used only by user space.
     */
    size_t wq_len;

    /* 
     * Priority of the waited thread for ARINC purposes.
     * 
     * Used only by user space.
     */
    volatile uint8_t wq_priority;

};

/* Thread is killed. When last msection is leaved, jet_sched() should be called. */
#define THREAD_KERNEL_FLAG_KILLED 1

/* Instance of this struct will be shared between kernel and user spaces. */
struct jet_kernel_shared_data
{
    /* 
     * Set by the kernel, read by the user.
     * 
     * Race while reading is impossible, as changing mode invalidates
     * all running threads.
     */
    pok_partition_mode_t partition_mode;

    /* 
     * Set by the kernel, read by the user.
     * 
     * NOTE: Any thread has its id, even main and error.
     * For ARINC requests, id is filtered out.
     * 
     * Each thread sees its own id, race is impossible.
     * 
     * DEV: When porting to multicore, this should be stored in the register.
     */
    pok_thread_id_t current_thread_id;

    /*
     * Actually, accessed only by the user.
     * 
     * Set once before partition's entry is executed.
     * Read when need to check process_id from ARINC.
     */
    pok_thread_id_t main_thread_id;

    /*
     * Set by the kernel when error thread is created.
     * Until that, it is JET_THREAD_ID_NONE.
     * 
     * Read by the user when it needs to determine whether error thread
     * currently is executed or when need to check process_id from ARINC.
     * 
     * Race is impossible, as the field is changed in INIT mode.
     */
    pok_thread_id_t error_thread_id;

    /* 
     * Maximum number of threads.
     * 
     * TODO: Does partition really need that info?
     */
    pok_thread_id_t max_n_threads;

    /* 
     * Start of the heap.
     * 
     * Set by the kernel in elf loader.
     * 
     * Read by the user for his allocator.
     */
    char* heap_start;

    /* 
     * End of the heap.
     * 
     * Set by the kernel according to configuration.
     * 
     * Read by the user for his allocator.
     */
    char* heap_end;

    /* Open-bounds array of thread shared data. */
    struct jet_thread_shared_data tshd[];
};

#endif /* __JET_UAPI_KERNEL_SHARED_DATA_H__ */
