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
void *memmove(void* dest, const void* src, size_t n)
{
    ptrdiff_t distance = (unsigned int)dest - (unsigned int)src;

    if((distance >= 0) // dest comes after or at src
        || (distance + n < 0)) {// regions are not overlapped
        // Copy bytes from the beginning.
        const unsigned char *s = (const unsigned char*) src;
        unsigned char *d = (unsigned char*) dest;

        while(n--)
        {
            *d++ = *s++;
        }
    }
    else {
        // Copy bytes from the end.
        const unsigned char* s_end = (const unsigned char*) src + n;
        unsigned char* d_end = (unsigned char*) dest + n;
        while(n--)
        {
            *(--d_end) = *(--s_end);
        }
    }
    return dest;
}
