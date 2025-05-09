/*
 * COPIED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify original one (kernel/include/uapi/time.h).
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

#ifndef __JET_UAPI_TIME_H__
#define __JET_UAPI_TIME_H__

typedef long time_t;

// POSIX
typedef int clockid_t;

#define CLOCK_REALTIME 0
#define CLOCK_PROCESS_CPUTIME_ID 1

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};


#endif /* __JET_UAPI_TIME_H__ */
