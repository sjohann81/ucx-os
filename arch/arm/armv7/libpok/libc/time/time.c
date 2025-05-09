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
#include <core/syscall.h>

time_t time(time_t *timer)
{
    time_t ret;
    
    pok_syscall1(POK_SYSCALL_TIME, (unsigned long)&ret);
    
    if(timer)
        *timer = ret;
    
    return ret;
}

uint64_t get_time_in_ns(void)
{
    uint64_t ret;

    pok_syscall1(POK_SYSCALL_HPET, (unsigned long)&ret);

    return ret;
}
uint64_t get_time_in_us(void)
{
    return get_time_in_ns() / 1000;
}
uint64_t get_time_in_ms(void)
{
    return get_time_in_ns() / 1000000;
}
