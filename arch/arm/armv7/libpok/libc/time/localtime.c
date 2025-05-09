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

struct tm *localtime_r(const time_t *timer, struct tm* timeptr)
{
    // "Fake" timer, for which UTC time is same as local time for given timer.
    time_t local_timer = *timer - timezone;
    
    return gmtime_r(&local_timer, timeptr);
}

struct tm *localtime(const time_t *timer)
{
    return localtime_r(timer, &global_tm);
}
