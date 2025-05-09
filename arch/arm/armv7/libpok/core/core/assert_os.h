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

#ifndef __LIBJET_ASSERT_OS_H__
#define __LIBJET_ASSERT_OS_H__

void assertion_os_fail(const char *expression, const char *file, int line);

#define assert_os(expr) ( \
        (expr) ? \
        ((void) 0) : \
        assertion_os_fail(#expr, __FILE__, __LINE__) \
    )

#endif /* __LIBJET_ASSERT_H__ */
