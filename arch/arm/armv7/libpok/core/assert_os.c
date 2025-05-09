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

#include <core/assert_os.h>

#include <core/syscall.h>
#include <stdio.h>

void assertion_os_fail(const char *expression, const char *file, int line)
{
    static char msg[128];
    
    snprintf(msg, sizeof(msg), "Assertion failed (%s) in %s:%d\n",
        expression, file, line);
    
    pok_error_raise_os_error(msg, sizeof(msg));
}
