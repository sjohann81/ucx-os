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

/* Way for storing information about characters classification. */

/* 
 * According to C99, only ints transformed into 'unsigned char' or EOF
 * should be classified.
 * 
 * 256(unsigned char) + 1(EOF) = 257(total).
 */
#define CTYPE_TABLE_SIZE 257

/* 
 * In total, there are 12 character classes:
 * 
 * 1)  upper
 * 2)  lower
 * 3)  alpha
 * 4)  digit
 * 5)  xdigit
 * 6)  space
 * 7)  print
 * 8)  graph
 * 9)  blank
 * 10) cntrl
 * 11) punct
 * 12) alnum
 * 
 * Using bit-per class doesn't fit into byte(8 bit) of information about
 * single character.
 * 
 * We use "packaging" for reduce number of bits:
 * 
 * 0. Assumption (does not directly follow from C99):
 *   "digit" cannot be "cntrl".
 * 
 * 1. According to C99, "alnum" can be neither "space" nor "punct".
 * 
 * 2. According to C99, "blank" is subclass of "space", so "alnum" cannot
 * be "blank".
 * 
 * 3. According to C99, "alpha" cannot be "cntrl".
 * 
 * 4. According to C99, "alnum" is combination of "alpha" and "digit". As
 * "alpha" cannot be "cntrl" (p.3), "digit" cannot be "cntrl" (p. 0), then
 * "alnum" cannot be "cntrl".
 * 
 * 5. According to C99, "alnum" includes "upper", "lower", "alpha", "digit".
 *
 * So 9 classes noted above may be encoded into 5 bits:
 *  - bit_alnum
 *  - bit_upper_or_space (depended from bit_alnum)
 *  - bit_lower_or_punct (depended from bit_alnum)
 *  - bit_alpha_or_blank (depended from bit_alnum)
 *  - bit_digit_or_control (depended from bit_alnum)
 * 
 * Using some other assumptions about digit, xdigit and alpha,
 * number of bits may be reduced futher.
 */

#define CTB_ALNUM 1
#define CTB_UPPER_OR_SPACE 2
#define CTB_LOWER_OR_PUNCT 4
#define CTB_ALPHA_OR_BLANK 8
#define CTB_DIGIT_OR_CNTRL 16
#define CTB_PRINT 32
#define CTB_GRAPH 64
#define CTB_XDIGIT 128

/* 
 * Pointer to the array of size CTYPE_TABLE_SIZE, which contains
 * classifications bits for the characters.
 */
extern unsigned char* ctype_table_classification;

/* 
 * Pointer to the array of size CTYPE_TABLE_SIZE, which contains results
 * of "tolower" conversion for the characters.
 */
extern int* ctype_table_lower;

/* 
 * Pointer to the array of size CTYPE_TABLE_SIZE, which contains results
 * of "toupper" conversion for the characters.
 */
extern int* ctype_table_upper;
