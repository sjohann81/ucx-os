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

/* Emits characters, produced by printf-like format, in a generic way.*/

#ifndef __LIBJET_STDIO_PRINTF_EMITTER_H__
#define __LIBJET_STDIO_PRINTF_EMITTER_H__

#include <stdarg.h>

/* Emit single character. */
typedef void (*emit_character_t)(int c, void *private_data);

/* For each character, determined by format and args, call emit_character().*/
void printf_emitter(emit_character_t emit_character, void *private_data,
    const char* format, va_list arg);

#endif /* __LIBJET_STDIO_PRINTF_EMITTER_H_ */
