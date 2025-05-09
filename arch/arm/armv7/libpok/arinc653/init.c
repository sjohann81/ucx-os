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

#include <init_arinc.h>
#include "arinc_alloc.h"
#include <arinc_config.h>

#include <smalloc.h>

#include "buffer.h"
#include "blackboard.h"
#include "event.h"
#include "semaphore.h"

#ifdef POK_NEEDS_ARINC653_BUFFER
struct arinc_buffer* arinc_buffers;
#endif /* POK_NEEDS_ARINC653_BUFFER */

#ifdef POK_NEEDS_ARINC653_BLACKBOARD
struct arinc_blackboard* arinc_blackboards;
#endif /* POK_NEEDS_ARINC653_BLACKBOARD */

#ifdef POK_NEEDS_ARINC653_EVENT
struct arinc_event* arinc_events;
#endif /* POK_NEEDS_ARINC653_EVENT */

#ifdef POK_NEEDS_ARINC653_SEMAPHORE
struct arinc_semaphore* arinc_semaphores;
#endif /* POK_NEEDS_ARINC653_SEMAPHORE */


#if defined(POK_NEEDS_ARINC653_BUFFER) || defined(POK_NEEDS_ARINC653_BLACKBOARD)
char* arinc_intra_heap = NULL;
#endif /* defined(POK_NEEDS_ARINC653_BUFFER) || defined(POK_NEEDS_ARINC653_BLACKBOARD) */

void libjet_arinc_init(void)
{
#ifdef POK_NEEDS_ARINC653_BUFFER
    arinc_buffers = smalloc(arinc_config_nbuffers * sizeof(*arinc_buffers));
#endif /* POK_NEEDS_ARINC653_BUFFER */

#ifdef POK_NEEDS_ARINC653_BLACKBOARD
    arinc_blackboards = smalloc(arinc_config_nblackboards * sizeof(*arinc_blackboards));
#endif /* POK_NEEDS_ARINC653_BLACKBOARD */

#ifdef POK_NEEDS_ARINC653_SEMAPHORE
    arinc_semaphores = smalloc(arinc_config_nsemaphores * sizeof(*arinc_semaphores));
#endif /* POK_NEEDS_ARINC653_SEMAPHORE */

#ifdef POK_NEEDS_ARINC653_EVENT
    arinc_events = smalloc(arinc_config_nevents * sizeof(*arinc_events));
#endif /* POK_NEEDS_ARINC653_EVENT */


#if defined(POK_NEEDS_ARINC653_BUFFER) || defined(POK_NEEDS_ARINC653_BLACKBOARD)
    arinc_intra_heap = smalloc(arinc_config_messages_memory_size);
#endif /* defined(POK_NEEDS_ARINC653_BUFFER) || defined(POK_NEEDS_ARINC653_BLACKBOARD) */
}
