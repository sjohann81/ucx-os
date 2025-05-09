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

/* 
 * Generic definition of the stream.
 */

#ifndef __LIBJET_STDIO_STREAM_H__
#define __LIBJET_STDIO_STREAM_H__

#include <stdio.h>

struct stdio_stream_ops;

struct stdio_stream
{
    /* Pointer to the buffer used by the stream. May be NULL. */
    char* buffer;
    /* Size of the buffer used by the stream. May be 0.*/
    size_t buffer_size;
    /* Mode of the buffer (_IOFBF, _IOLBF or _IONBF). */
    int buffer_mode;
    /* Relation of the buffer with underline object. */
    const struct stdio_stream_ops* s_ops;
    /* Current position in the buffer. */
    size_t buffer_pos;
    /* 
     * Number of bytes written into the stream.
     * 
     * This counter may be reseted manually when needed.
     */
    size_t bytes_written;
    /* 
     * TODO: there is should be a msection,
     * which protect stream from concurrent accesses.
     * But this requires some initialization function.
     */
};

struct stdio_stream_ops
{
    /* 
     * Write bytes into underline object.
     * 
     * Return number of bytes which has been actually writen.
     */
    size_t (*write_bytes)(struct stdio_stream* stream,
        const char* bytes, size_t size);
    // Reading streams are currently unsupported.
};

/* Callback of type 'emit_character_t' for emit characters into stream.*/
void stdio_stream_emit_character(int c, void* _stream);
/* 
 * Complete operation for the stream.
 * 
 * Buffer may be used by 'stdio_stream_emit_character' even in _IONBF mode;
 * this function flushes the buffer.
 */
void stdio_stream_complete_operation(struct stdio_stream* stream);

#endif /* __LIBJET_STDIO_STREAM_H_ */
