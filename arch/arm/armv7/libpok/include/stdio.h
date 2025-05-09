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

#ifndef __LIBJET_STDIO_H__
#define __LIBJET_STDIO_H__

#include <stddef.h>
#include <stdarg.h>

// Buffer size use by setbuf(). (Not, that function itself is not accessible).
#define BUFSIZ 128

// Input/output is fully buffered.
#define _IOFBF 1
// Input/output is line-buffered.
#define _IOLBF 2
// Input/output is not buffered at all.
#define _IONBF 3

#define EOF ((int)(-1))

typedef struct stdio_stream FILE;

extern FILE* stderr;
extern FILE* stdout;
extern FILE* stdin;

typedef size_t fpos_t;

int printf(const char * restrict format, ...)
    __attribute__ ((format(printf, 1, 2)));
int vprintf(const char * restrict format, va_list arg)
    __attribute__ ((format(printf, 1, 0)));

int snprintf(char * restrict s, size_t n, const char * restrict format, ...)
    __attribute__ ((format(printf, 3, 4)));
int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg)
    __attribute__ ((format(printf, 3, 0)));

int putchar(int c);
int puts(const char *s);

#endif /* __LIBJET_STDIO_H_ */
