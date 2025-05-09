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

size_t strcspn(const char *s1, const char *s2)
{
    // Forward implementation, O(m^2)
    const char* s1_iter = s1;

    for(; *s1_iter; s1_iter++) {
       if(strchr(s2, *s1_iter)) break;
    }

    return s1_iter - s1;
}

size_t strspn(const char *s1, const char *s2)
{
    // Forward implementation, O(m^2)
    const char* s1_iter = s1;

    for(; *s1_iter; s1_iter++) {
        if(!strchr(s2, *s1_iter)) break;
    }

    return s1_iter - s1;
}
