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

#ifndef __LIBJET_KERNEL_SHARED_DATA_H__
#define __LIBJET_KERNEL_SHARED_DATA_H__

#include <uapi/kernel_shared_data.h>

/* This variable is defined by the linker. */
extern struct jet_kernel_shared_data kshd;

#endif /* __LIBJET_KERNEL_SHARED_DATA_H__ */
