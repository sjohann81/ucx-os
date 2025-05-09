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

#ifndef __LIBJET_ASP_ALIGNMENT_H__
#define __LIBJET_ASP_ALIGNMENT_H__

#include "include/types.h"

/* 
 * Return maximum alignment for object with given-or-less size.
 */
size_t libja_mem_get_alignment(size_t obj_size);

#endif /* __LIBJET_ASP_ALIGNMENT_H__ */
