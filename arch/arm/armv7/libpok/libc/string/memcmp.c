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

#include <string.h>

/* GCC requires this function even for freestanding environment. */
int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *mem1 = s1;
    const unsigned char *mem2 = s2;
    const unsigned char *mem1_end = mem1 + n;

    for(; mem1 != mem1_end; ++mem1, ++mem2)
    {
        int d = *mem1 - *mem2;
        if(d) return d;
    }

    return 0;
}
