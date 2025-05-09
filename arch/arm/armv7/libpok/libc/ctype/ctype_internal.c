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

#include <ctype.h>
#include "ctype_internal.h"

#define GET_CTYPE_CLASS(c) ((ctype_table_classification + 1)[c])

int isalnum(int c)
{
    return GET_CTYPE_CLASS(c) & CTB_ALNUM;
}
int isalpha(int c)
{
    return (GET_CTYPE_CLASS(c) & (CTB_ALNUM | CTB_ALPHA_OR_BLANK))
        == (CTB_ALNUM | CTB_ALPHA_OR_BLANK);
}
int isblank(int c)
{
    return (GET_CTYPE_CLASS(c) & (CTB_ALNUM | CTB_ALPHA_OR_BLANK))
        == CTB_ALPHA_OR_BLANK;
}
int iscntrl(int c)
{
    return (GET_CTYPE_CLASS(c) & (CTB_ALNUM | CTB_DIGIT_OR_CNTRL))
        == CTB_DIGIT_OR_CNTRL;
}
int isdigit(int c)
{
    return (GET_CTYPE_CLASS(c) & (CTB_ALNUM | CTB_DIGIT_OR_CNTRL))
        == (CTB_ALNUM | CTB_DIGIT_OR_CNTRL);
}
int isgraph(int c)
{
    return GET_CTYPE_CLASS(c) & CTB_GRAPH;
}
int islower(int c)
{
    return (GET_CTYPE_CLASS(c) & (CTB_ALNUM | CTB_LOWER_OR_PUNCT))
        == (CTB_ALNUM | CTB_LOWER_OR_PUNCT);
}
int isprint(int c)
{
    return GET_CTYPE_CLASS(c) & CTB_PRINT;
}
int ispunct(int c)
{
    return (GET_CTYPE_CLASS(c) & (CTB_ALNUM | CTB_LOWER_OR_PUNCT))
        == CTB_LOWER_OR_PUNCT;
}
int isspace(int c)
{
    return (GET_CTYPE_CLASS(c) & (CTB_ALNUM | CTB_UPPER_OR_SPACE))
        == CTB_UPPER_OR_SPACE;
}
int isupper(int c)
{
    return (GET_CTYPE_CLASS(c) & (CTB_ALNUM | CTB_UPPER_OR_SPACE))
        == (CTB_ALNUM | CTB_UPPER_OR_SPACE);
}
int isxdigit(int c)
{
    return GET_CTYPE_CLASS(c) & CTB_XDIGIT;
}
int tolower(int c)
{
    return (ctype_table_lower + 1)[c];
}
int toupper(int c)
{
    return (ctype_table_upper + 1)[c];
}

/* Currently, the only possible locale is "C" */
#include "ctype_tables_c.h"

unsigned char* ctype_table_classification = ctype_table_classification_C;

int* ctype_table_lower = ctype_table_lower_C;
int* ctype_table_upper = ctype_table_upper_C;
