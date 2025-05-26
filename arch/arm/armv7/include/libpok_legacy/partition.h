/*
 * Institute for System Programming of the Russian Academy of Sciences
 * Copyright (C) 2013-2014, 2016 ISPRAS
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


#ifndef __POK_PARTITION_H__
#define __POK_PARTITION_H__

#include <libpok_legacy/config.h>

#include <libpok_legacy/types.h>
#include <libpok_legacy/error.h>
// #include <libpok_legacy/errno.h>
// #include <core/error.h>

// #include <libpok_legacy/partition_types.h>

#include <libpok_legacy/cswitch.h>

#include <libpok_legacy/space.h>
#include <libpok_legacy/partition_types.h>

// typedef enum
// {
//   POK_START_CONDITION_NORMAL_START          = 0,
//   POK_START_CONDITION_PARTITION_RESTART     = 1,
//   POK_START_CONDITION_HM_MODULE_RESTART     = 2,
//   POK_START_CONDITION_HM_PARTITION_RESTART  = 3
// }pok_start_condition_t;

typedef uint8_t pok_system_state_t;

struct _pok_partition;

/* Scheduling operations specific for given partition. */
struct pok_partition_sched_operations
{
    /*
     * Called when some async event about partition occures.
     * Event is encoded into partition's `.state` bits(bytes).
     *
     * During call to this handler, local preemption is disabled.
     *
     * The handler should either clear `.state` bytes, or enable local
     * preemption. Otherwise the handler will be called again.
     *
     * If local preemption is already disabled, handler is not called.
     * But corresponded `.state` bytes are set nevetheless.
     */
    void (*on_event)(void);

#ifdef POK_NEEDS_GDB
    /*
     * Return number of threads for given partition.
     *
     * NOTE: Called (possibly) outside of partition's timeslot, so
     * 'current_partition' doesn't correspond to given partition.
     */
    int (*get_number_of_threads)(struct _pok_partition* part);

    /*
     * Return index of current thread within partition.
     *
     * Returning negative value means that no current thread(is it possible?).
     *
     * If partition doesn't work with user space, never called.
     *
     * NOTE: Called (possibly) outside of partition's timeslot, so
     * 'current_partition' doesn't correspond to given partition.
     */
    int (*get_current_thread_index)(struct _pok_partition* part);

    /*
     * Return 0 if thread with given index exists. Fill 'private' with
     * value, which will be passed for callbacks below.
     */
    int (*get_thread_at_index)(struct _pok_partition* part,
        int index, void** private);

    /*
     * Get information about thread with given index.
     *
     * Information is written using 'print_cb' callback.
     *
     * NOTE: Called (possibly) outside of partition's timeslot, so
     * 'current_partition' doesn't correspond to given partition.
     */
    void (*get_thread_info)(struct _pok_partition* part, int index, void* private,
        print_cb_t print_cb, void* cb_data);

    /*
     * Get interrupt context for non-current thread. It is used for extract registers.
     *
     * Returning NULL means that thread has no registers assosiated with it.
     *
     * TODO: naming is bad.
     */
    struct jet_interrupt_context* (*get_thread_registers)(struct _pok_partition* part, int index, void* private);
#endif /* POK_NEEDS_GDB */
};

struct pok_partition_operations
{
    /*
     * Called when partition is (re)started.
     *
     * Local preemption is disabled.
     *
     * Cannot be NULL.
     */
    void (*start)(void);  /**< The entry-point for the partition's thread. */

    /*
     * Process sync error related to given partition.
     *
     * Local preemption is disabled.
     *
     * Previous local preemption state is passed as parameter.
     *
     * Cannot be NULL.
     */
    void (*process_partition_error)(pok_system_state_t partition_state,
        pok_error_id_t error_id,
        uint8_t state_byte_preempt_local,
        void* failed_address);
};

/* Non-zero number, which is incremented every time partition is started. */
typedef uint32_t pok_partition_generation_t;

/* Type of outer event for partition. */
enum jet_partition_event_type
{
    /* Partition's timer is expired. */
    JET_PARTITION_EVENT_TYPE_TIMER,
    /* Space is available for send message into the queuing port. */
    JET_PARTITION_EVENT_TYPE_PORT_SEND_AVAILABLE,
    /* Message is available for receive it from the queuing port. */
    JET_PARTITION_EVENT_TYPE_PORT_RECEIVE_AVAILABLE,
};

/* Outer event for partition. */
struct jet_partition_event
{
    uint16_t handler_id;
    enum jet_partition_event_type event_type;
};

/*!
 * \struct pok_partition_t
 * \brief This structure contains all needed information for partition management
 */
typedef struct _pok_partition
{
#ifdef POK_NEEDS_MONITOR
    pok_bool_t               is_paused;      /*Partition paused or not*/
#endif

    const struct pok_partition_sched_operations* part_sched_ops;
    const struct pok_partition_operations* part_ops;

    /*
     * Circular buffer with partition events.
     *
     * Allocated on initialization.
     */
    struct jet_partition_event* partition_events;

    /* Maximum number of incoming events. Set in deployment.c. */
    uint16_t partition_event_max;
    /* Index of the first event for receive. */
    uint16_t partition_event_begin;
    /* Index after the last event for receive. */
    uint16_t partition_event_end;

    /*
     * If this field is positive, partition will receive event
     * (with handler_id = 0) when current time will be equal-or-more
     * than this value.
     *
     * When timer event is fired, the field is reset to 0.
     *
     * DEV: There are 2 slots for timer events. So it is allowable to
     * set this field without preliminary reseting it and checking for
     * events.
     */
    volatile pok_time_t timer;

    /*
     * Whether event has been fired.
     *
     * This field is set when event is added to empty queue.
     *
     * The field should be reset to 0 by partition before last event is
     * consumed.
     */
    pok_bool_t is_event;

    /*
     * Whether local preemption is disabled.
     *
     * Flag can be read, modified or cleared by the partition itself.
     *
     * Flag is set and checked by the global scheduler,
     * when it need to call partition's callbacks.
     */
    uint8_t preempt_local_disabled;

    pok_partition_generation_t partition_generation;

    /*
     * Whether currently executed *user space* process is error handler.
     *
     * This field is used for determine system level in case when
     * error is catched via interrupt.
     *
     * Reseted to false when partition starts.
     */
    pok_bool_t               is_error_handler;

    const char               *name;          /**< Name of the partition */

    // Should be set in deployment.c.
    uint64_t                 period;         /**< Period of the partition, unused at this time */
    uint64_t                 duration;       /**< Duration of the partition, unused at this time */
    pok_partition_id_t       partition_id;

    /*
     * Kernel stack address which is used for enter into the partition.
     *
     * 0 value in this field means that partition needs to be (re)started.
     *
     */
    struct jet_context*     sp;

    /*
     * Initial value of kernel stack (when it was allocated).
     *
     * Used for restarting partition.
     *
     * Set by particular partition's implementation.
     */
    jet_stack_t            initial_sp;

    /*
     * Identificator of (user) space, corresponded to given partition.
     * Special value 0xff means that no user space is used by this partition.
     *
     * Set in deployment.c
     *
     * Used by scheduler when it switch into partition.
     */
    uint8_t                  space_id;

    /*
     * Pointer to area for save floating point registers for current
     * (user) thread.
     *
     * Local scheduler should set this field before jumping or returning
     * into user space.
     */
    struct jet_fp_store*    fp_store_current;

#ifdef POK_NEEDS_GDB
    /*
     * Pointer to the user space registers array, stored for given partition.
     *
     * Set by global scheduler, used (may be cleared) by partition.
     */
    struct jet_interrupt_context* entry_sp_user;

    /*
     * Pointer to the registers array, stored for given partition when switched off.
     *
     * Used only by global scheduler.
     */
    struct jet_interrupt_context* entry_sp;
#endif /* POK_NEEDS_GDB */

#ifdef POK_NEEDS_IO
  uint16_t		    io_min;             /**< If the partition is allowed to perform I/O, the lower bound of the I/O */
  uint16_t		    io_max;             /**< If the partition is allowed to perform I/O, the uppder bound of the I/O */
#endif

  pok_start_condition_t	    start_condition;

  /* Set if partition is restarted by outside.
   *
   * Partition may clear this flag in its `start()` callback. */
  pok_bool_t restarted_externally;

  /*
   * Pointer to Multi partition HM selector.
   *
   * Bit's value 0 means module level error, 1 - partition level error.
   *
   * Set in deployment.c
   */
  const pok_error_level_selector_t* multi_partition_hm_selector;
  /*
   * Pointer to Multi partition HM table.
   *
   * Set in deployment.c
   */
  const pok_error_module_action_table_t* multi_partition_hm_table;
} pok_partition_t;

/**
 * Pointer to the current partition.
 *
 * DEV: Readonly for all except scheduler-related stuff.
 */
extern pok_partition_t* current_partition;

/*
 * Add event to partition's queue and notify it if needed.
 *
 * Should be called with global preemption disabled.
 *
 * Note: May affect on scheduling, so preemption shouldn't be enabled using
 * __pok_preemption_enable().
 */
void pok_partition_add_event(pok_partition_t* part,
    enum jet_partition_event_type event_type,
    uint16_t handler_id);

/*
 * Consume event from current partition's queue.
 *
 * Return TRUE on success, FALSE if the queue is empty.
 *
 * Should be called with local preemption disabled.
 */
pok_bool_t pok_partition_get_event(struct jet_partition_event* event);

/*
 * Set timer for given partition.
 * Setting to 0 means reseting.
 *
 * Event queue should be drained before setting timer once more.
 */
void pok_partition_set_timer(pok_partition_t* part,
    pok_time_t timer_new);

/* Initialize partition. */
void pok_partition_init(pok_partition_t* part);

/**
 * Execute given function for each partition.
 */
void for_each_partition(void (*f)(pok_partition_t* part));

/*
 * Ready-made scheduler operations for partition with single kernel thread.
 *
 * '.start' function for such partition shouldn't enable local preemption.
 */
extern const struct pok_partition_sched_operations partition_sched_ops_kernel;

#endif /* __POK_PARTITION_H__ */
