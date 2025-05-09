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

#include "stream.h"
#include <core/syscall.h>
#include <assert.h>

// Flush stream with *non-empty* buffer.
void stream_flush(struct stdio_stream* stream)
{
    assert(stream->buffer_pos > 0);

    stream->s_ops->write_bytes(stream, stream->buffer,
        stream->buffer_pos);
    // TODO: check errors
    stream->buffer_pos = 0;
}

void stdio_stream_emit_character(int c, void* _stream)
{
    struct stdio_stream* stream = _stream;
    // Unconditionally convert character into 'char'.
    char c_real = (char)c;

    if(stream->buffer_pos == stream->buffer_size) {
        // Buffer is full or doesn't exist.
        if(stream->buffer_pos) {
            // Buffer is full. Need to flush it before continue.
            stream_flush(stream);
        }
        else {
            // Buffer doesn't exist. Unconditionally use non-buffered mode.
            stream->s_ops->write_bytes(stream, &c_real, 1);
            stream->bytes_written++;
            // TODO: check errors
            return;
        }
    }

    // Buffer is available. Use it.
    stream->buffer[stream->buffer_pos++] = c_real;
    stream->bytes_written++;
    if(c_real == '\n' && stream->buffer_mode == _IOLBF) {
        // In case of newline and per-line mode, flush buffer immediately.
        stream_flush(stream);
    }
}

void stdio_stream_complete_operation(struct stdio_stream* stream)
{
    if(stream->buffer_pos > 0 && stream->buffer_mode == _IONBF) {
        // Flush buffer in non-buffered mode.
        stream_flush(stream);
    }
}

/*
 * Write bytes into the console.
 * 
 * Callback '.write_bytes' for the stream.
 */
static size_t console_write_bytes(struct stdio_stream* stream,
    const char* bytes, size_t size)
{
    (void)stream;

    pok_syscall2 (POK_SYSCALL_CONSWRITE, (unsigned long)bytes, (unsigned long)size);
    // TODO: Check errors.
    return size;
}

static const struct stdio_stream_ops console_stream_ops = {
    .write_bytes = &console_write_bytes
};

static char console_buffer[BUFSIZ];
/* Stream, which is connected to (output) console. */
static struct stdio_stream console_stream = {
    .buffer = console_buffer,
    .buffer_size = BUFSIZ,
    .buffer_mode = _IONBF,
    .s_ops = &console_stream_ops,
    .buffer_pos = 0,
    .bytes_written = 0,
};

FILE* stderr = &console_stream;
FILE* stdout = &console_stream;
