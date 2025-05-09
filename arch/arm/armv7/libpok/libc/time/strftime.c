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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "time_internal.h"

/* 
 * Return week number (from zero) in the year.
 * 
 * It is assumed that every week starts with 'first_week_day', and the
 * first such day in the year determines the first week in that year.
 */
static int week_number(const struct tm* timeptr,
    int first_week_day)
{
    /* Day in the year when the first week starts. */
    int week_starts = (timeptr->tm_yday - timeptr->tm_wday + 7) % 7;
    
    if(timeptr->tm_yday >= week_starts) {
        // Week in the current year.
        return (timeptr->tm_yday - week_starts) / 7;
    }
    
    int days_in_prev_year = days_in_year(timeptr->tm_year + tm0_year - 1); 
    // Week corresponds to previous year.
    int week_prev_starts = (days_in_prev_year + 7 - week_starts) % 7;
    
    return (timeptr->tm_yday + days_in_prev_year - week_prev_starts) / 7;
}

/* 
 * Return year in weak-based coordinates. 
 * 
 * In these coordinates:
 * 
 *  - If the first Monday of January is the 2nd, 3rd, or 4th,
 * the preceding days are part of the last week of the preceding year
 * 
 *  - If December 29th, 30th, or 31st is a Monday, it and any following
 * days are part of week 1 of the following year.
 */
static int weak_based_year(const struct tm* timeptr)
{
    if(timeptr->tm_yday < 3)
    {
        /* 
         * January, 1st, 2nd or 3rd.
         * 
         * Check that year is not actually the preceeding one.
         */
        
        /* Compute day when the first Monday occures.
         *  +7 is used for force it to be non-negative.
         */
        int monday_first = (timeptr->tm_yday + 1 + 7 - timeptr->tm_wday) % 7;
        
        switch(monday_first) {
            case 1: //Monday is at January, 2nd
            case 2: //Monday is at January, 3nd
            case 3: //Monday is at January, 4nd
                if(timeptr->tm_yday < monday_first) return timeptr->tm_year - 1;
        }
    }
    else if(timeptr->tm_yday >= 362) {
        /* 
         * Definitely December. Possibly at December, 29 or after.
         * 
         * Check that year is not actually the following one.
         */

        /* 
         * Compute week day of December, 31.
         */
        int wday_dec_31 = timeptr->tm_wday + (31 - timeptr->tm_mday);
        
        switch(wday_dec_31) {
            case 1: // Monday is at December, 31
            case 2: // Monday is at December, 30
            case 3: // Monday is at December, 29
                if((timeptr->tm_mday + wday_dec_31) >= 32) return timeptr->tm_year + 1;
        }
    }
    return timeptr->tm_year;
}

/*
 * Similar for weak_based_year, but returns week number instead of year.
 * 
 *  - weeks begin on a Monday
 */
static int weak_based_week(const struct tm* timeptr)
{
    /* 
     * Compute day when the first Monday occures.
     *  +7 is used for force it to be non-negative.
     */
    int monday_first = (timeptr->tm_yday + 1 + 7 - timeptr->tm_wday) % 7;
    
    /* Monday when the first week starts.*/
    int week_first;
    
    switch(monday_first) {
        case 0: //Monday is at January, 1nd
            week_first = monday_first;
            break;
        case 1: //Monday is at January, 2nd
            if(timeptr->tm_yday < monday_first) return 51;
            week_first = monday_first;
            break;
        case 2: //Monday is at January, 3nd
            if(timeptr->tm_yday < monday_first)
                // Weak number depends whether previous year is leap or not.
                return leap_year(timeptr->tm_year + tm0_year - 1) ? 52 : 51;
            week_first = monday_first;
            break;
        case 3: //Monday is at January, 4nd
            if(timeptr->tm_yday < monday_first) return 52;
            week_first = monday_first;
            break;
        case 4:
        case 5:
        case 6:
            week_first = monday_first - 7;
    }
    
    if(timeptr->tm_yday >= 362) {
        /* 
         * Definitely December. Possibly at December, 29 or after.
         * 
         * Check that year is not actually the following one.
         */

        /* 
         * Compute week day of December, 31.
         */
        int wday_dec_31 = timeptr->tm_wday + (31 - timeptr->tm_mday);
        
        switch(wday_dec_31) {
            case 1: // Monday is at December, 31
            case 2: // Monday is at December, 30
            case 3: // Monday is at December, 29
                if((timeptr->tm_mday + wday_dec_31) >= 32) return 0;
        }
    }
    
    return (timeptr->tm_yday - week_first) / 7 + 1;
}


struct time_format_state {
    char* out; // String for current output
    size_t n; // Limit for bytes rest
    
    const char* format; // Symbol in the format to process next.
    
    size_t bytes_used_total; // Total number of bytes used.
};

/* 
 * Produce string for formatted time accordinf to printf-like format.
 * 
 * Return 0 on success, 1 if bytes limit has been encountered.
 */
static int time_printf(struct time_format_state* tfs,
    const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    size_t bytes_needed = vsnprintf(tfs->out, tfs->n, format, args);
    
    va_end(args);
    
    if(bytes_needed >= tfs->n) return 1;
    tfs->bytes_used_total += bytes_needed;
    // Advance position
    tfs->out += bytes_needed;
    tfs->n -= bytes_needed;
    
    return 0;
}


/* 
 * Produce string for formatted time.
 * 
 * Return 0 on success, 1 if bytes limit has been encountered.
 */
static int time_print_string(struct time_format_state* tfs, const char* s)
{
    size_t bytes_needed = strlen(s);
    
    if(bytes_needed == 0) return 0; // Nothing to do
    if(bytes_needed >= tfs->n) return 1;
    
    memcpy(tfs->out, s, bytes_needed);
    tfs->bytes_used_total += bytes_needed;
    // Advance position
    tfs->out += bytes_needed;
    tfs->n -= bytes_needed;
    
    return 0;
}

/* 
 * Produce string for formatted time according to rules of strftime
 * except appending terminating '\0'.
 * 
 * Return 0 on success, 1 if insufficient space.
 */
static int time_format(struct time_format_state* tfs,
    const struct tm * timeptr)
{
    while(1) {
        size_t bytes_used_tmp;
        
        char* first_spec = strchr(tfs->format, '%');
        
        if(first_spec == NULL) {
            // No needs to update format at the end.
            return time_print_string(tfs, tfs->format);
        }

        // first_spec points to '%'.
        bytes_used_tmp = first_spec - tfs->format;
        if(bytes_used_tmp > 0) {
            if(bytes_used_tmp > tfs->n) return 0;
            
            memcpy(tfs->out, tfs->format, bytes_used_tmp);
            tfs->bytes_used_total += bytes_used_tmp;
            // Advance position.
            tfs->format += bytes_used_tmp;
            tfs->out += bytes_used_tmp;
            tfs->n -= bytes_used_tmp;
        }
        
        tfs->format++; // First character after '%'
        
        /* Whether is continued after the character. */
        int format_continue;

        do {
            // By default the character is assumed to terminate the format.
            format_continue = 0;
            switch(*(tfs->format++)) {
                case '\0':
                    // Missed actual specifier after '%'
                    return 0;
                case 'E':
                    // Currently 'E' modifier is ignored
                    format_continue = 1;
                    break;
                case 'O':
                    // Currently 'O' modifier is ignored
                    format_continue = 1;
                    break;
                case 'a':
                    if(time_print_string(tfs, 
                        current_locale_time->weekday_abbrev[timeptr->tm_wday])) {
                        return 1;
                    }
                    break;
                case 'A':
                    if(time_print_string(tfs, 
                        current_locale_time->weekday[timeptr->tm_wday])) {
                        return 1;
                    }
                    break;
                case 'b':
                    if(time_print_string(tfs, 
                        current_locale_time->month_abbrev[timeptr->tm_mon])) {
                        return 1;
                    }
                    break;
                case 'B':
                    if(time_print_string(tfs, 
                        current_locale_time->month[timeptr->tm_mon])) {
                        return 1;
                    }
                    break;
                case 'c':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = current_locale_time->date_time_format;
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'C': // 00-99
                    if(time_printf(tfs, "%2d", (timeptr->tm_year + tm0_year) % 100)) {
                        return 1;
                    }
                    break;
                case 'd': // 01-31
                    if(time_printf(tfs, "%02d", timeptr->tm_mday)) {
                        return 1;
                    }
                    break;
                case 'D':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = "%m/%d/%y";
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'e': // 1-31, single digit prepended by space.
                    if(time_printf(tfs, "%.2d", timeptr->tm_mday)) {
                        return 1;
                    }
                    break;
                case 'F':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = "%Y−%m−%d";
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'g': // 00-99, Weak-based.
                    if(time_printf(tfs, "%02d", weak_based_year(timeptr) % 100)) {
                        return 1;
                    }
                    break;
                case 'G': // Weak-based.
                    if(time_printf(tfs, "%d", weak_based_year(timeptr) % 100)) {
                        return 1;
                    }
                    break;
                case 'h':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = "%b";
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'H': // 00-23.
                    if(time_printf(tfs, "%02d", timeptr->tm_hour)) {
                        return 1;
                    }
                    break;
                case 'I': // 01-12.
                    if(time_printf(tfs, "%02d", (timeptr->tm_hour % 12) + 1)) {
                        return 1;
                    }
                    break;
                case 'j': // 001-366.
                    if(time_printf(tfs, "%03d", timeptr->tm_yday)) {
                        return 1;
                    }
                    break;
                case 'm': // 01-12.
                    if(time_printf(tfs, "%02d", timeptr->tm_mon)) {
                        return 1;
                    }
                    break;
                case 'M': // 00-59.
                    if(time_printf(tfs, "%02d", timeptr->tm_min)) {
                        return 1;
                    }
                    break;
                case 'n':
                    if(time_print_string(tfs, "\n")) {
                        return 1;
                    }
                    break;
                case 'p':
                    if(time_print_string(tfs, current_locale_time->am_pm[timeptr->tm_hour / 12])) {
                        return 1;
                    }
                    break;
                case 'r':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = current_locale_time->time_12_format;
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'R':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = "%H:%M";
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'S': // 00-60.
                    if(time_printf(tfs, "%02d", timeptr->tm_sec)) {
                        return 1;
                    }
                    break;
                case 't':
                    if(time_print_string(tfs, "\t")) {
                        return 1;
                    }
                    break;
                case 'T':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = "%H:%M:%S";
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'u': // 1-7, since Monday
                    if(time_printf(tfs, "%d", ((timeptr->tm_wday + 6) % 7) + 1)) {
                        return 1;
                    }
                    break;
                case 'U': // 00-53, the first Sunday is the first day of the first weak.
                    if(time_printf(tfs, "%d", week_number(timeptr, 0))) {
                        return 1;
                    }
                    break;
                case 'V': // 01-53.
                    if(time_printf(tfs, "%02d", weak_based_week(timeptr) + 1)) {
                        return 1;
                    }
                    break;
                case 'w': // 0-6, since Sunday
                    if(time_printf(tfs, "%d", timeptr->tm_wday)) {
                        return 1;
                    }
                    break;
                case 'W': // 00-53, the first Monday is the first day of the first weak.
                    if(time_printf(tfs, "%d", week_number(timeptr, 1))) {
                        return 1;
                    }
                    break;
                case 'x':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = current_locale_time->date_format;
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'X':
                    {
                        const char* format_old = tfs->format;
                        tfs->format = current_locale_time->time_format;
                        if(time_format(tfs, timeptr)) return 1;

                        tfs->format = format_old;
                    }
                    break;
                case 'y': // 00-99.
                    if(time_printf(tfs, "%02d", (timeptr->tm_year + tm0_year) % 100)) {
                        return 1;
                    }
                    break;
                case 'Y':
                    if(time_printf(tfs, "%d", timeptr->tm_year + tm0_year)) {
                        return 1;
                    }
                    break;
                case 'z':
                    {
                        int tz_hours, tz_minutes_abs;
                        if(timezone >= 0) {
                            tz_hours = timezone / 60;
                            tz_minutes_abs = timezone - tz_hours * 60;
                        }
                        else {
                            tz_hours = - (-timezone) / 60;
                            tz_minutes_abs = tz_hours * 60 - timezone;
                        }
                        if(time_printf(tfs, "%+02d%02d", tz_hours, tz_minutes_abs)) {
                            return 1;
                        }
                    }
                    break;
                case 'Z':
                    if(time_printf(tfs, "%s", current_locale_time->tz_name)) {
                        return 1;
                    }
                    break;
                case '%':
                    if(time_print_string(tfs, "%")) {
                        return 1;
                    }
                default: // Behaviour is undefined.
                    if(time_print_string(tfs, "??")) {
                        return 1;
                    }
                    break;
            }
        } while(format_continue);

    }
}

size_t strftime(char * restrict s, size_t maxsize, const char * restrict format, const struct tm * restrict timeptr)
{
    struct time_format_state tfs = {
        .out = s,
        .n = maxsize,
        
        .format = format,
        
        .bytes_used_total = 0
    };
    
    if(tfs.n == 0) return 0;
    
    if(time_format(&tfs, timeptr)) return 0;
    
    // Append terminating null.
    *tfs.out = '\0';
    
    return tfs.bytes_used_total;
}
