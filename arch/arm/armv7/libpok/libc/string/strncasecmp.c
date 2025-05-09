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
 
#include <core/dependencies.h>
#include <string.h>

#include <ctype.h>

#ifdef POK_CONFIG_NEEDS_FUNC_STRNCASECMP

__attribute__ ((weak))
int strncasecmp(const char *s1, const char *s2, size_t n)
{
  unsigned int i;
  for (i = 0; i < n; i++)
  { 
      if (s1[i] == '\0' && s2[i] == '\0')
          return 0;
      
      int diff = tolower(s1[i]) - tolower(s2[i]); // As POSIX.1-2008 requires.
      
      if(diff) return diff;
  }
  return 0;
}

#endif


