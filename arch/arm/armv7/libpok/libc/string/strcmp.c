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

int strcmp(const char *s1, const char *s2)
{
    const unsigned char *str1 = (const unsigned char *)s1;
    const unsigned char *str2 = (const unsigned char *)s2;

    for(; *str1; ++str1, ++str2)
    {
        int d = *str1 - *str2;
        if(d) return d;
    }

    return *str1 - *str2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    const unsigned char *str1 = (const unsigned char *)s1;
    const unsigned char *str2 = (const unsigned char *)s2;
    const unsigned char *str1_end = str1 + n;

    for(; str1 != str1_end && *str1; ++str1, ++str2)
    {
        int d = *str1 - *str2;
        if(d) return d;
    }

    return (str1 != str1_end) ? (*str1 - *str2) : 0;
}
