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

char *strchr(const char *s, int c)
{
    for(; *s; s++) {
        if((int)*s == c) break;
    }

    return ((int)*s == c) ? (char*)s : NULL;
}

char *strrchr(const char *s, int c)
{
    // Diffirentiate case when search for null-character.
    if(c == 0) {
        while(*s) s++;
        return (char*)s;
    }

    const char *res = NULL;

    for(; *s; s++) {
        if((int)*s == c) res = s;
    }

    return (char*)res;
}
