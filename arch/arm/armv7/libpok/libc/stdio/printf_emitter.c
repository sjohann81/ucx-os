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
 *
 * This file also incorporates work covered by POK License.
 * Copyright (c) 2007-2009 POK team
 */

#include "printf_emitter.h"

#include <stdio.h>
#include <string.h>

static const char digits[] = "0123456789abcdef";

#define PUTC(c) emit_character(c, private_data)

/* Emit characters, produced by a number with given modificators.*/
static void print_num(emit_character_t emit_character,
                      void *private_data,
                      unsigned long long value,
                      unsigned base,
                      int pad,
                      int neg,
                      int pad_with_zero)
{
    unsigned char digit_str[32]; //64-bit number in octal base is 32 digits long
    int size = 0;
    do {
        unsigned mod = value%base;
        value /= base;
        digit_str[size] = digits[mod];
        size++;

    } while (value);

    if (pad_with_zero) {
        if (neg) {
            PUTC('-');
            pad--;
        }
        while (size < pad--)
            PUTC('0');

    } else {
        if (neg)
            pad--;

        while (size < pad--)
            PUTC(' ');

        if (neg)
            PUTC('-');
    }

    while (size--)
        PUTC(digit_str[size]);
}

/* Emit characters, produced by a floating number with given modificators.*/
static void print_float(emit_character_t emit_character,
                        void *private_data,
                        long double value,
                        int pad,
                        int precision,
                        int neg,
                        int pad_with_zero)
{
    long long floor = value;
    long double fractional = 0;

    print_num(emit_character, private_data, floor, 10, pad-(precision+1), neg, pad_with_zero);

    PUTC('.');

    fractional = (value - floor);
    for (int i=0; i<precision; i++)
        fractional *= 10;
    print_num(emit_character, private_data, fractional, 10, precision, 0, 1);
}

/*
 * Parse format specifier (without leading '%') from format and emit
 * characters it produces.
 * 
 * Return pointer to the character followed format specifier.
 */
//XXX for now precision is used only for floating points
static const char * handle_fmt(
    emit_character_t emit_character, void *private_data,
    const char* format, va_list* parg)
{
    unsigned l = 0;
    int pad = 0;
    int pad_with_zero = 0; // if 1 than add zeros for padding
    int precision = 0;
    int was_dot = 0; //if 1 than there were '.' in fmt before
    while (*format) {
        switch (*format) {
            case '0':
                if (was_dot)
                    precision *= 10;
                else if (pad == 0)
                    pad_with_zero = 1;
                else
                    pad *= 10;
                format++;
                break;
            case '1'...'9':
                if (was_dot)
                    precision = *format - '0' + precision*10;
                else
                    pad = *format - '0' + pad*10;
                format++;
                break;
            case 'l':
                l++;
                format++;
                break;
           case '.':
                was_dot = 1;
                format++;
                break;
           case 'z':		// size_t modifier
                l = 0;
                format++;
                break;
           case 's':
                {
                    //TODO implement l!=0 case
                    const char *s = va_arg(*parg, const char *);
                    if (!s)
                        s = (const char *)"(null)";
                    int len = strlen(s);

                    if (precision != 0 && precision<len)
                        len = precision;

                    while (len < pad--)
                        PUTC(' ');
                    while (*s && len-- )
                        PUTC(*s++);
                    return ++format;
                }
           case 'p':
                PUTC('0');
                PUTC('x');
           case 'x':
                {
                    long long value;

                    if (l == 2)
                        value = va_arg(*parg, unsigned long long);
                    else if (l == 1)
                        value = va_arg(*parg, unsigned long);
                    else
                        value = va_arg(*parg, unsigned );

                    print_num(emit_character, private_data,
                        value, 16, pad, 0, pad_with_zero);
                    return ++format;
                }
           case 'd':
           case 'i':
                {
                    long long value;
                    if (l == 2)
                        value = va_arg(*parg, long long);
                    else if (l == 1)
                        value = va_arg(*parg, long);
                    else
                        value = va_arg(*parg, int);
                    int neg = value < 0;
                    if (neg)
                        value = -value;
                    print_num(emit_character, private_data,
                        value, 10, pad, neg, pad_with_zero);
                    return ++format;
                }
            case 'u':
                {
                    long long value;
                    if (l == 2)
                        value = va_arg(*parg, unsigned long long);
                    else if (l == 1)
                        value = va_arg(*parg, unsigned long);
                    else
                        value = va_arg(*parg, unsigned );

                    print_num(emit_character, private_data,
                        value, 10, pad, 0, pad_with_zero);
                    return ++format;
                }
            case 'f':
                {
                    long double value;
                    if (l == 2)
                        value = va_arg(*parg, long double);
                    else
                        value = va_arg(*parg, double);
                    int neg = value < 0;
                    if (neg)
                        value = -value;
                    if (precision == 0)
                        precision = 6;
                    print_float(emit_character, private_data,
                        value, pad, precision, neg, pad_with_zero);
                    return ++format;
                }
            case 'c':
                //TODO implement l!=0 case
                PUTC(va_arg(*parg, unsigned ));
                return ++format;
            case '%':
                PUTC('%');
                return ++format;
            case 0:
                return format;
            default:
                PUTC('%');
                PUTC(*format);
                return ++format;
        }
    }
    return format;
}

void printf_emitter(emit_character_t emit_character, void *private_data,
    const char* format, va_list arg)
{
    /* 
     * For some reason, we cannot pass reference to our 'va_list' argument
     * on PPC.
     * 
     * Because of that we create our copy of the arg.
     * 
     * TODO: rewrite this part by do not propagating 'arg' to non-single
     * inner calls.
     */
    va_list arg1;

    /* 
     * Avoid using __va_copy as it is a GNU extension.
     * 
     * After va_copy() do not forget to call va_end() at the end.
     */
    va_copy(arg1, arg);

    while(*format) {
        if (*format == '%') {
            format++;
            format = handle_fmt(emit_character, private_data, format, &arg1);
        }
        else
            PUTC(*format++);
    }

    va_end(arg1);
}
