/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team
 *
 * Created by julien on Thu Jan 15 23:34:13 2009
 */
#include "config.h"

#include <errno.h>

#include "Cioports.h"
#include "libc.h"
#include <core/debug.h>
#include <asp/cons.h>

#include "armv7/include/bsp/bsp.h"
#include "armv7/include/bsp/serial.h"

#if defined (POK_NEEDS_CONSOLE) || defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_INSTRUMENTATION) || defined (POK_NEEDS_COVERAGE_INFOS)

static size_t iostream_write_main(const char* s, size_t length)
{
    serial_write(s, length);
    return length;
}

static size_t iostream_write_debug(const char* s, size_t length)
{
    serial_write(s, length);
    return length;
}

static size_t iostream_read_main(char* s, size_t length)
{
    return 0;
}
static size_t iostream_read_debug(char* s, size_t length)
{
    return 0;
}

struct jet_iostream arm_stream_main =
{
    .write = &iostream_write_main,
    .read  = &iostream_read_main
};
struct jet_iostream arm_stream_debug =
{
    .write = &iostream_write_debug,
    .read  = &iostream_read_debug
};

struct jet_iostream* ja_stream_default_read = &arm_stream_main;
struct jet_iostream* ja_stream_default_write = &arm_stream_main;
struct jet_iostream* ja_stream_default_read_debug = &arm_stream_debug;
struct jet_iostream* ja_stream_default_write_debug = &arm_stream_debug;

#else

struct jet_iostream* ja_stream_default_read = NULL;
struct jet_iostream* ja_stream_default_write = NULL;
struct jet_iostream* ja_stream_default_read_debug = NULL;
struct jet_iostream* ja_stream_default_write_debug = NULL;

#endif
