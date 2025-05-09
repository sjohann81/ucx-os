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

char *strpbrk(const char *s1, const char *s2)
{
    char c1 = *s1;

    for(; c1; s1++, c1 = *s1) {
        const char* s2_iter = s2;
        char c2 = *s2_iter;
        for(; c2; s2_iter++, c2 = *s2_iter)
            if(c1 == c2) return (char*)s1;
    }

    return NULL;
}
