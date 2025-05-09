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

#include <msection.h>
#include <core/syscall.h>
#include <kernel_shared_data.h>

#include <arinc653/types.h>
#include <utils.h>

void msection_init(struct msection* section)
{
    section->owner = JET_THREAD_ID_NONE;
    section->msection_kernel_flags = 0;
}

void msection_enter(struct msection* section)
{
    struct jet_thread_shared_data* tshd_current = kshd.tshd + kshd.current_thread_id;

    tshd_current->msection_count++;
    tshd_current->msection_entering = section;

    if(section->owner == JET_THREAD_ID_NONE)
    {
        section->owner = kshd.current_thread_id;
    }
    else if(section->owner != kshd.current_thread_id)
    {
        jet_msection_enter_helper(section); //syscall
    }

    tshd_current->msection_entering = NULL;
}

void msection_leave(struct msection* section)
{
    struct jet_thread_shared_data* tshd_current = kshd.tshd + kshd.current_thread_id;
    // TODO: assert(tshd_current->msection_count > 0);

    section->owner = JET_THREAD_ID_NONE;
    tshd_current->msection_count--;

    if(tshd_current->msection_count == 0
        && tshd_current->thread_kernel_flags & THREAD_KERNEL_FLAG_KILLED)
    {
        // Let us being killed.
        jet_resched();
    }
    else if(section->msection_kernel_flags & MSECTION_KERNEL_FLAG_RESCHED_AFTER_LEAVE)
    {
        // Someone waits on the section.
        jet_resched();
    }
}

pok_ret_t msection_wait(struct msection* section, pok_time_t timeout)
{
    return jet_msection_wait(section, &timeout);
}

pok_ret_t msection_notify(struct msection* section, pok_thread_id_t thread_id)
{
    return jet_msection_notify(section, thread_id);
}

void msection_wq_add(struct msection_wq* wq, pok_thread_id_t next)
{
    pok_thread_id_t *pnext = (next != JET_THREAD_ID_NONE)
        ? &kshd.tshd[next].wq_prev
        : &wq->last;

    pok_thread_id_t prev = *pnext;

    pok_thread_id_t* pprev = (prev != JET_THREAD_ID_NONE)
        ? &kshd.tshd[prev].wq_next
        : &wq->first;

    struct jet_thread_shared_data* tshd_current = &kshd.tshd[kshd.current_thread_id];

    tshd_current->wq_next = next;
    tshd_current->wq_prev = prev;

    *pnext = *pprev = kshd.current_thread_id;
}

void msection_wq_add_after(struct msection_wq* wq, pok_thread_id_t prev)
{
    pok_thread_id_t* pprev = (prev != JET_THREAD_ID_NONE)
        ? &kshd.tshd[prev].wq_next
        : &wq->first;

    pok_thread_id_t next = *pprev;

    pok_thread_id_t *pnext = (next != JET_THREAD_ID_NONE)
        ? &kshd.tshd[next].wq_prev
        : &wq->last;

    struct jet_thread_shared_data* tshd_current = &kshd.tshd[kshd.current_thread_id];

    tshd_current->wq_next = next;
    tshd_current->wq_prev = prev;

    *pnext = *pprev = kshd.current_thread_id;
}

void msection_wq_del(struct msection_wq* wq, pok_thread_id_t thread)
{
    struct jet_thread_shared_data* tshd_t = &kshd.tshd[thread];

    pok_thread_id_t next = tshd_t->wq_next;
    pok_thread_id_t prev = tshd_t->wq_prev;

    pok_thread_id_t *pnext = (next != JET_THREAD_ID_NONE)
        ? &kshd.tshd[next].wq_prev
        : &wq->last;

    pok_thread_id_t *pprev = (prev != JET_THREAD_ID_NONE)
        ? &kshd.tshd[prev].wq_next
        : &wq->first;

    *pnext = prev;
    *pprev = next;

    tshd_t->wq_next = tshd_t->wq_prev = JET_THREAD_ID_NONE;
}


pok_ret_t msection_wq_notify(struct msection* section,
    struct msection_wq* wq,
    pok_bool_t is_all)
{
    return jet_msection_wq_notify(section, wq, is_all);
}

size_t msection_wq_size(struct msection* section,
   struct msection_wq* wq)
{
    size_t size;

    jet_msection_wq_size(section, wq, &size);

    return size;
}
