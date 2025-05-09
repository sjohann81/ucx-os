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

#ifndef __LIBJET_TIME_TIME_INTERNAL_H__
#define __LIBJET_TIME_TIME_INTERNAL_H__

#include <time.h>

/* 
 * Number of days in month, dependent from month index (0-11),
 * and whether the year is LEAP(1) or not(0).
 */

static const int days_in_month[2][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

/* Base point for year in tm structure. */
static const int tm0_year = 1900;

/* Base point for year in time_t. */
static const int t0_year = 1970;

/* Number or seconds in single day. */
#define SECS_DAY (60 * 60 * 24)

/* Return 1 if year is LEAP and 0 otherwise. */
static inline int leap_year(int year)
{
    if(year % 4) return 0; // non-divisible on 4
    
    if(year % 100) return 1; // divisible on 4 but non-divisible on 100
    
    if(year % 400) return 0; // divisible on 100 but non-divisible on 400
    
    return 1; // divisible on 400.
}

/* Return number of the days in the given year.*/
static inline int days_in_year(int year)
{
    return leap_year(year) ? 366 : 365;
}

/* Global tm structure for non-reentrant functions.*/
extern struct tm global_tm;

struct locale_time {
    const char* weekday[7]; // Since Sunday
    const char* weekday_abbrev[7]; // Since Sunday

    const char* month[12]; // Since Junuary
    const char* month_abbrev[12]; // Since Junuary
    
    const char* date_time_format; // For '%c' specifier
    
    const char* am_pm[2]; // For '%p' specifier
    
    const char* time_12_format; // For '%r' specifier
    
    const char* date_format; // For '%x' specifier
    
    const char* time_format; // For '%X' specifier
    
    const char* tz_name; // For '%Z' specifier
};

extern struct locale_time* current_locale_time;

#endif /*__LIBJET_TIME_TIME_INTERNAL_H__*/
