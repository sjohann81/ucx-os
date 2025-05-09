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
void *memcpy(void * restrict dest, const void * restrict src, size_t n)
{
    const unsigned char *s = (const unsigned char*) src;
    unsigned char *d = (unsigned char*) dest;

    while (n--)
    {
        *d++ = *s++;
    }

    return dest;
}
