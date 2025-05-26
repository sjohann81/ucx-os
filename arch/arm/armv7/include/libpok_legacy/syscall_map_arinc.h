/*
 * GENERATED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify the one it generated from (kernel/include/uapi/syscall_map_arinc.h.in).
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


#include <libpok_legacy/types.h>
#include <libpok_legacy/thread_types.h>
#include <libpok_legacy/partition_types.h>
#include <libpok_legacy/syscall.h>
// #include <libpok_legacy/partition_arinc_types.h>
// #include <libpok_legacy/port_types.h>
// #include <libpok_legacy/error_arinc_types.h>
// #include <libpok_legacy/memblock_types.h>
// #include <libpok_legacy/msection.h>


typedef enum {
    JET_MEMORY_BLOCK_READ,
    JET_MEMORY_BLOCK_READ_WRITE
} jet_memory_block_mode_t;

typedef struct {
    uintptr_t addr;
    jet_memory_block_mode_t mode;
    size_t size;
} jet_memory_block_status_t;

/**
 * \enum pok_partition_mode_t
 * \brief The different modes of a partition
 */
 typedef enum
 { 
    /*
     * In init mode, only main thread (process) is run.
     * This's the only mode where one can create various resources.
     *
     * There's really no difference between cold and warm init.
     *   
     * When partition is initially started, it's in cold init.
     *
     * HM table and set_partition_mode function may restart 
     * partition into either cold or warm init mode.
     *
     * The exact type of init can be introspected by an application,
     * but otherwise, it makes no difference.
     */
    POK_PARTITION_MODE_INIT_COLD = 1, 
    POK_PARTITION_MODE_INIT_WARM = 2,
 
    /*
     * In normal mode, all threads except main can be run.
     *
     * No resources can be allocated.
     */
    POK_PARTITION_MODE_NORMAL    = 3, 
 
    /*
     * Partition is stopped.
     */
    POK_PARTITION_MODE_IDLE      = 4,
 }pok_partition_mode_t;
 
 typedef struct {
    pok_partition_id_t id;
    pok_time_t period;
    pok_time_t duration;
    int32_t lock_level;
    pok_partition_mode_t mode;
    pok_start_condition_t  start_condition;
 } pok_partition_status_t;
 
static inline int pok_thread_create(const char* name,
    void* entry,
    const pok_thread_attr_t* attr,
    pok_thread_id_t* thread_id)
{
    return pok_syscall4(POK_SYSCALL_THREAD_CREATE,
        (uint32_t)name,
        (uint32_t)entry,
        (uint32_t)attr,
        (uint32_t)thread_id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_CREATE

#ifdef POK_NEEDS_THREAD_SLEEP
static inline int pok_thread_sleep(const pok_time_t* time)
{
    return pok_syscall1(POK_SYSCALL_THREAD_SLEEP,
        (uint32_t)time);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_SLEEP
#endif

#ifdef POK_NEEDS_THREAD_SLEEP_UNTIL
static inline int pok_thread_sleep_until(const pok_time_t* time)
{
    return pok_syscall1(POK_SYSCALL_THREAD_SLEEP_UNTIL,
        (uint32_t)time);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_SLEEP_UNTIL
#endif
static inline int pok_sched_end_period(void)
{
    return pok_syscall0(POK_SYSCALL_THREAD_PERIOD);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_PERIOD

#if defined (POK_NEEDS_THREAD_SUSPEND) || defined (POK_NEEDS_ERROR_HANDLING)
static inline int pok_thread_suspend(const pok_time_t* time)
{
    return pok_syscall1(POK_SYSCALL_THREAD_SUSPEND,
        (uint32_t)time);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_SUSPEND
#endif

static inline int pok_thread_get_status(pok_thread_id_t thread_id,
    char* name,
    void** entry,
    pok_thread_status_t* status)
{
    return pok_syscall4(POK_SYSCALL_THREAD_STATUS,
        (uint32_t)thread_id,
        (uint32_t)name,
        (uint32_t)entry,
        (uint32_t)status);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_STATUS

static inline int pok_thread_delayed_start(pok_thread_id_t thread_id,
    const pok_time_t* time)
{
    return pok_syscall2(POK_SYSCALL_THREAD_DELAYED_START,
        (uint32_t)thread_id,
        (uint32_t)time);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_DELAYED_START

static inline int pok_thread_set_priority(pok_thread_id_t thread_id,
    uint32_t priority)
{
    return pok_syscall2(POK_SYSCALL_THREAD_SET_PRIORITY,
        (uint32_t)thread_id,
        (uint32_t)priority);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_SET_PRIORITY

static inline int pok_thread_resume(pok_thread_id_t thread_id)
{
    return pok_syscall1(POK_SYSCALL_THREAD_RESUME,
        (uint32_t)thread_id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_RESUME

static inline int pok_thread_suspend_target(pok_thread_id_t thread_id)
{
    return pok_syscall1(POK_SYSCALL_THREAD_SUSPEND_TARGET,
        (uint32_t)thread_id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_SUSPEND_TARGET

static inline int pok_thread_yield(void)
{
    return pok_syscall0(POK_SYSCALL_THREAD_YIELD);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_YIELD

static inline int pok_sched_replenish(const pok_time_t* budget)
{
    return pok_syscall1(POK_SYSCALL_THREAD_REPLENISH,
        (uint32_t)budget);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_REPLENISH

static inline int pok_thread_stop_target(pok_thread_id_t thread_id)
{
    return pok_syscall1(POK_SYSCALL_THREAD_STOP,
        (uint32_t)thread_id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_STOP

static inline int pok_thread_stop(void)
{
    return pok_syscall0(POK_SYSCALL_THREAD_STOPSELF);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_STOPSELF

static inline int pok_thread_find(const char* name,
    pok_thread_id_t* id)
{
    return pok_syscall2(POK_SYSCALL_THREAD_FIND,
        (uint32_t)name,
        (uint32_t)id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_THREAD_FIND


static inline int jet_resched(void)
{
    return pok_syscall0(POK_SYSCALL_RESCHED);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_RESCHED

static inline int jet_msection_enter_helper(struct msection* section)
{
    return pok_syscall1(POK_SYSCALL_MSECTION_ENTER_HELPER,
        (uint32_t)section);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MSECTION_ENTER_HELPER

static inline int jet_msection_wait(struct msection* section,
    const pok_time_t* timeout)
{
    return pok_syscall2(POK_SYSCALL_MSECTION_WAIT,
        (uint32_t)section,
        (uint32_t)timeout);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MSECTION_WAIT

static inline int jet_msection_notify(struct msection* section,
    pok_thread_id_t thread_id)
{
    return pok_syscall2(POK_SYSCALL_MSECTION_NOTIFY,
        (uint32_t)section,
        (uint32_t)thread_id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MSECTION_NOTIFY

static inline int jet_msection_wq_notify(struct msection* section,
    struct msection_wq* wq,
    pok_bool_t is_all)
{
    return pok_syscall3(POK_SYSCALL_MSECTION_WQ_NOTIFY,
        (uint32_t)section,
        (uint32_t)wq,
        (uint32_t)is_all);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MSECTION_WQ_NOTIFY

static inline int jet_msection_wq_size(struct msection* section,
    struct msection_wq* wq,
    size_t* size)
{
    return pok_syscall3(POK_SYSCALL_MSECTION_WQ_SIZE,
        (uint32_t)section,
        (uint32_t)wq,
        (uint32_t)size);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MSECTION_WQ_SIZE


#ifdef POK_NEEDS_PARTITIONS
static inline int pok_partition_set_mode_current(pok_partition_mode_t mode)
{
    return pok_syscall1(POK_SYSCALL_PARTITION_SET_MODE,
        (uint32_t)mode);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_PARTITION_SET_MODE

static inline int pok_current_partition_get_status(pok_partition_status_t* status)
{
    return pok_syscall1(POK_SYSCALL_PARTITION_GET_STATUS,
        (uint32_t)status);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_PARTITION_GET_STATUS

static inline int pok_current_partition_inc_lock_level(int32_t* lock_level)
{
    return pok_syscall1(POK_SYSCALL_PARTITION_INC_LOCK_LEVEL,
        (uint32_t)lock_level);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_PARTITION_INC_LOCK_LEVEL

static inline int pok_current_partition_dec_lock_level(int32_t* lock_level)
{
    return pok_syscall1(POK_SYSCALL_PARTITION_DEC_LOCK_LEVEL,
        (uint32_t)lock_level);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_PARTITION_DEC_LOCK_LEVEL
#endif


#ifdef POK_NEEDS_ERROR_HANDLING
static inline int pok_error_thread_create(uint32_t stack_size,
    void* entry)
{
    return pok_syscall2(POK_SYSCALL_ERROR_HANDLER_CREATE,
        (uint32_t)stack_size,
        (uint32_t)entry);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_ERROR_HANDLER_CREATE

static inline int pok_error_raise_application_error(const char* msg,
    size_t msg_size)
{
    return pok_syscall2(POK_SYSCALL_ERROR_RAISE_APPLICATION_ERROR,
        (uint32_t)msg,
        (uint32_t)msg_size);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_ERROR_RAISE_APPLICATION_ERROR

static inline int pok_error_get(pok_error_status_t* status,
    void* msg)
{
    return pok_syscall2(POK_SYSCALL_ERROR_GET,
        (uint32_t)status,
        (uint32_t)msg);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_ERROR_GET
#endif

static inline int pok_error_raise_os_error(const char* msg,
    size_t msg_size)
{
    return pok_syscall2(POK_SYSCALL_ERROR_RAISE_OS_ERROR,
        (uint32_t)msg,
        (uint32_t)msg_size);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_ERROR_RAISE_OS_ERROR


   /* Middleware syscalls */
#ifdef POK_NEEDS_PORTS_SAMPLING
static inline int pok_port_sampling_create(const char* name,
    pok_port_size_t size,
    pok_port_direction_t direction,
    const pok_time_t* refresh,
    pok_port_id_t* id)
{
    return pok_syscall5(POK_SYSCALL_MIDDLEWARE_SAMPLING_CREATE,
        (uint32_t)name,
        (uint32_t)size,
        (uint32_t)direction,
        (uint32_t)refresh,
        (uint32_t)id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_SAMPLING_CREATE

static inline int pok_port_sampling_write(pok_port_id_t id,
    const void* data,
    pok_port_size_t len)
{
    return pok_syscall3(POK_SYSCALL_MIDDLEWARE_SAMPLING_WRITE,
        (uint32_t)id,
        (uint32_t)data,
        (uint32_t)len);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_SAMPLING_WRITE

static inline int pok_port_sampling_read(pok_port_id_t id,
    void* data,
    pok_port_size_t* len,
    pok_bool_t* valid)
{
    return pok_syscall4(POK_SYSCALL_MIDDLEWARE_SAMPLING_READ,
        (uint32_t)id,
        (uint32_t)data,
        (uint32_t)len,
        (uint32_t)valid);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_SAMPLING_READ

static inline int pok_port_sampling_id(const char* name,
    pok_port_id_t* id)
{
    return pok_syscall2(POK_SYSCALL_MIDDLEWARE_SAMPLING_ID,
        (uint32_t)name,
        (uint32_t)id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_SAMPLING_ID

static inline int pok_port_sampling_status(pok_port_id_t id,
    pok_port_sampling_status_t* status)
{
    return pok_syscall2(POK_SYSCALL_MIDDLEWARE_SAMPLING_STATUS,
        (uint32_t)id,
        (uint32_t)status);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_SAMPLING_STATUS

static inline int pok_port_sampling_check(pok_port_id_t id)
{
    return pok_syscall1(POK_SYSCALL_MIDDLEWARE_SAMPLING_CHECK,
        (uint32_t)id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_SAMPLING_CHECK
#endif /* POK_NEEDS_PORTS_SAMPLING */

#ifdef POK_NEEDS_PORTS_QUEUEING
static inline int pok_port_queuing_create_packed(const char* name,
    const pok_port_queuing_create_arg_t* arg,
    pok_port_id_t* id)
{
    return pok_syscall3(POK_SYSCALL_MIDDLEWARE_QUEUEING_CREATE,
        (uint32_t)name,
        (uint32_t)arg,
        (uint32_t)id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_QUEUEING_CREATE

static inline int pok_port_queuing_send(pok_port_id_t id,
    const void* data,
    pok_port_size_t len,
    const pok_time_t* timeout)
{
    return pok_syscall4(POK_SYSCALL_MIDDLEWARE_QUEUEING_SEND,
        (uint32_t)id,
        (uint32_t)data,
        (uint32_t)len,
        (uint32_t)timeout);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_QUEUEING_SEND

static inline int pok_port_queuing_receive(pok_port_id_t id,
    const pok_time_t* timeout,
    void* data,
    pok_port_size_t* len)
{
    return pok_syscall4(POK_SYSCALL_MIDDLEWARE_QUEUEING_RECEIVE,
        (uint32_t)id,
        (uint32_t)timeout,
        (uint32_t)data,
        (uint32_t)len);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_QUEUEING_RECEIVE

static inline int pok_port_queuing_id(const char* name,
    pok_port_id_t* id)
{
    return pok_syscall2(POK_SYSCALL_MIDDLEWARE_QUEUEING_ID,
        (uint32_t)name,
        (uint32_t)id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_QUEUEING_ID

static inline int pok_port_queuing_status(pok_port_id_t id,
    pok_port_queuing_status_t* status)
{
    return pok_syscall2(POK_SYSCALL_MIDDLEWARE_QUEUEING_STATUS,
        (uint32_t)id,
        (uint32_t)status);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_QUEUEING_STATUS

static inline int pok_port_queuing_clear(pok_port_id_t id)
{
    return pok_syscall1(POK_SYSCALL_MIDDLEWARE_QUEUEING_CLEAR,
        (uint32_t)id);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MIDDLEWARE_QUEUEING_CLEAR

#endif /* POK_NEEDS_PORTS_QUEUEING */


static inline int pok_memory_block_get_status(const char* name,
    jet_memory_block_status_t* status)
{
    return pok_syscall2(POK_SYSCALL_MEMORY_BLOCK_GET_STATUS,
        (uint32_t)name,
        (uint32_t)status);
}
// Syscall should be accessed only by function
#undef POK_SYSCALL_MEMORY_BLOCK_GET_STATUS
