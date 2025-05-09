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

static char *strtok_r(char * restrict s1, const char * restrict s2,
    char** save_ptr)
{
    // Forward implementation, O(n*m)
    if(!s1) {
        if(!(*save_ptr)) return NULL;
        s1 = *save_ptr;
    }

    // Look for the first character not in s2.
    for(; *s1; s1++) {
        if(!strchr(s2, *s1)) break;
    }

    if(!*s1) {
        // Not found.
        *save_ptr = NULL;
        return NULL;
    }

    char* s1_end = s1 + 1;

    // Search for the end of the token.
    for(; *s1_end; s1_end++) {
        if(strchr(s2, *s1_end)) break;
    }

    if(*s1_end) {
        // Found delimiter, replace it with null.
        *s1_end = '\0';
        // And store followed position for the future search.
        *save_ptr = s1_end + 1;
    }
    else {
        // Found end of string. This is the last token.
        *save_ptr = NULL;
    }

    return s1;
}

char *strtok(char * restrict s1, const char * restrict s2)
{
    static char* next_s1 = NULL;
    return strtok_r(s1, s2, &next_s1);
}
