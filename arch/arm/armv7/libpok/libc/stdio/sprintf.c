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

#include <stdio.h>
#include "printf_emitter.h"

struct string_stream
{
    char* str;
    int bytes_written;
    int bytes_max;
};

static void string_stream_emit_character(int c, void *_sstream)
{
    struct string_stream* sstream = _sstream;

    if(sstream->bytes_written < sstream->bytes_max) {
        sstream->str[sstream->bytes_written] = (char)c;
    }

    sstream->bytes_written++;
}

int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg)
{
    int bytes_max = n > 0 ? (n - 1): 0;

    struct string_stream sstream = {
        .str = s,
        .bytes_written = 0,
        .bytes_max = bytes_max
    };

    printf_emitter(&string_stream_emit_character, &sstream, format, arg);

    int res = sstream.bytes_written;

    if(n > 0) {
        s[(res < bytes_max)? res : bytes_max] = '\0';
    }

    return res;
}

int snprintf(char * restrict s, size_t n, const char * restrict format, ...)
{
    va_list arg;

    va_start(arg, format);
    int res = vsnprintf (s, n, format, arg);
    va_end(arg);

    return res;
}
