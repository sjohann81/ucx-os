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

#ifndef __LIBJET_STDLIB_H__
#define __LIBJET_STDLIB_H__

#include <stdint.h>
#include <stddef.h>

#define RAND_MAX 32767

void abort(void);

int rand (void);
void srand(unsigned int seed);

#endif /* __LIBJET_STDLIB_H__*/
