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

char *strcpy(char * restrict dest, const char * restrict src)
{
    char* dest_iter = dest;

    while(*src)
        *dest_iter++ = *src++;

    *dest_iter = '\0';

    return dest;
}

char *strncpy(char * restrict dest, const char * restrict src, size_t n)
{
    char* dest_iter = dest;
    int i = 0;

    while(i < n && *src) {
        *dest_iter++ = *src++;
        i++;
    }

    *dest_iter = '\0';
    // The rest bytes should also be filled
    for(; i < n; i++)
      *(++dest_iter) = '\0';

    return dest;
}
