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

#include "ctype_internal.h"

unsigned char ctype_table_classification_{{locale.c_name}}[CTYPE_TABLE_SIZE] = {
      0, // EOF(-1)
{%for l in range(32) %}
    {%+for c in range(8) %}{%if not loop.first%} {%endif%}{{"%#04x"| format(locale.get_mask(c + l * 8))}},{%endfor%}
 /* {{l * 8}}-{{l*8+7}}*/{{'\n'}}{%endfor%}
};

int ctype_table_lower_{{locale.c_name}}[CTYPE_TABLE_SIZE] = {
    -1, // EOF(-1)
{%for l in range(32) %}
    {%+for c in range(8) %}{%if not loop.first%} {%endif%}{{"%3s"| format(locale.tolower(c + l * 8))}},{%endfor%}
 /* {{l * 8}}-{{l*8+7}}*/{{'\n'}}{%endfor%}
};

int ctype_table_upper_{{locale.c_name}}[CTYPE_TABLE_SIZE] = {
    -1, // EOF(-1)
{%for l in range(32) %}
    {%+for c in range(8) %}{%if not loop.first%} {%endif%}{{"%3s"| format(locale.toupper(c + l * 8))}},{%endfor%}
 /* {{l * 8}}-{{l*8+7}}*/{{'\n'}}{%endfor%}
};
