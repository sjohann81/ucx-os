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
#include "stream.h"

/* This function is part of C99, but we don't expose it to the user. */
static int vfprintf(FILE * restrict stream, const char * restrict format, va_list arg)
{
    stream->bytes_written = 0;

    printf_emitter(stdio_stream_emit_character, stream, format, arg);
    stdio_stream_complete_operation(stream);

    return stream->bytes_written;
}

int vprintf(const char * restrict format, va_list arg)
{
    return vfprintf(stdout, format, arg);
}


int printf(const char * restrict format, ...)
{
    va_list arg;

    va_start(arg, format);
    int res = vprintf (format, arg);
    va_end(arg);

    return res;
}
