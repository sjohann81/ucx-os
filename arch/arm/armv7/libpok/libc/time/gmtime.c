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

#include <time.h>

#include "time_internal.h"

struct tm *gmtime_r(const time_t *timer, struct tm* timeptr)
{
    time_t time = *timer;
    int dayclock, dayno;
    int year = t0_year;
 
    /* Extract total number of days and number of seconds since day starts. */
    dayclock = (int)(time % SECS_DAY);
    dayno = (int)(time / SECS_DAY);
 
    timeptr->tm_sec = dayclock % 60;
    timeptr->tm_min = (dayclock % 3600) / 60;
    timeptr->tm_hour = dayclock / 3600;
    timeptr->tm_wday = (dayno + 4) % 7;       /* day 0 was a thursday */
 
    /* Just count years, one by one. */
    while (dayno >= days_in_year(year)) {
        dayno -= days_in_year(year);
        year++;
    }
 
    timeptr->tm_year = year - tm0_year;
    timeptr->tm_yday = dayno;
    timeptr->tm_mon = 0;
 
    /* Compute using tm_yday compute tm_mon and tm_mday. */
    while (dayno >= days_in_month[leap_year(year)][timeptr->tm_mon]) {
        dayno -= days_in_month[leap_year(year)][timeptr->tm_mon];
        timeptr->tm_mon++;
    }
 
    timeptr->tm_mday = dayno + 1;
    timeptr->tm_isdst = 0;

    return timeptr;
}

struct tm *gmtime(const time_t *timer)
{
    return gmtime_r(timer, &global_tm);
}
