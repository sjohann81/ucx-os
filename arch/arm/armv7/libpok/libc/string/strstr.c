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

char *strstr(const char *s1, const char *s2)
{
    // Forward implementation, O(n*m)
    size_t s2_len = strlen(s2);
    if(s2_len == 0) return (char*)s1; // Nothing to search.
    
    for(; *s1; s1++) {
        if(memcmp(s1, s2, s2_len) == 0) return (char*)s1;
    }

    return NULL;
}
