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

#ifndef __LIBJET_TIME_H__
#define __LIBJET_TIME_H__

#include <stddef.h>
#include <stdint.h>
#include <uapi/time.h>

#define CLOCKS_PER_SEC 1000000000

typedef int64_t clock_t;

struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

clock_t clock(void);
double difftime(time_t time1, time_t time0);
time_t mktime(struct tm *timeptr);
time_t time(time_t *timer);
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
size_t strftime(char * restrict s, size_t maxsize, const char * restrict format, const struct tm * restrict timeptr);

/* POSIX */

extern long timezone;

//Variable 'daylight' is currently not supported and is always assumed to be 0.

struct tm *gmtime_r(const time_t *timer, struct tm* timeptr);
struct tm *localtime_r(const time_t *timer, struct tm* timeptr);

int clock_gettime(clockid_t clock_id, struct timespec* tp);

#include "stdint.h"
uint64_t get_time_in_ns(void);
uint64_t get_time_in_us(void);
uint64_t get_time_in_ms(void);

#endif /* __LIBJET_TIME_H__ */
