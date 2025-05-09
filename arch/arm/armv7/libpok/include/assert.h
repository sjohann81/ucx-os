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

#ifndef __LIBJET_ASSERT_H__
#define __LIBJET_ASSERT_H__

#include <stdlib.h>

#ifdef NDEBUG
#define assert(expr) ((void)0)
#else
#define assert(expr) do {                                                   \
    if(!(expr)) {                                                           \
        printf("Assertion failed: `%s', function %s, file %s, line %d\n",   \
            #expr, __func__, __FILE__, __LINE__);                           \
        abort();                                                            \
    }} while(0)
#endif

#endif /* __LIBJET_ASSERT_H__ */
