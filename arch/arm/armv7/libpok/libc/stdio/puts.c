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
#include "stream.h"

int puts(const char *s)
{
    stdout->bytes_written = 0; // Just for the case.
    for(; *s; s++)
        stdio_stream_emit_character(*s, stdout);
    stdio_stream_emit_character('\n', stdout);
    stdio_stream_complete_operation(stdout);

    return 0;
}
