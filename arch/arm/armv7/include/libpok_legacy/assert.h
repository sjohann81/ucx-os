 /* 
 *  Copyright (C) 2014 Maxim Malkov, ISPRAS <malkov@ispras.ru> 
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __POK_KERNEL_ASSERT_H__
#define __POK_KERNEL_ASSERT_H__

#include <libpok_legacy/debug.h>

__attribute__((__noreturn__))
static inline void pok_assertion_fail(const char *expression, const char *file, int line)
{
    printf("Assertion failed (%s) in %s:%d\n", expression, file, line);
    pok_fatal("");
}

#ifndef NDEBUG
    #define assert(expr) ( \
        (expr) ? \
        ((void) 0) : \
        pok_assertion_fail(#expr, __FILE__, __LINE__) \
    )
#else
    #define assert(expr) do {} while (0)
#endif

#define unreachable() assert(FALSE)

#endif
