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

#ifndef __POK_COMMON_H__
#define __POK_COMMON_H__

#include <libpok_legacy/compiler.h>

/*
 * Mark pointer as pointer to user space.
 * 
 * Pointers of such types should be dereferenced only with special function
 * (see uaccess.h).
 * 
 * Some checkers can set this mark to something, which *actually*
 * prohibits incorrect usage of marked pointers.
 * 
 * Usage example:
 * 
 *    void a(char* __user buf);
 */
#define __user

/*
 * Kernel-space address of user memory area.
 * 
 * The address can be accessed only while being in corresponded space.
 */
#define __kuser

/**
 * Casts pointer to member of structure to pointer to structure itself.
 * 
 * @ptr: the pointer to the member
 * @type: the type of the container struct
 * @member: name of the member within the container.
 * 
 * Implementation is taken from Linux kernel.
 */
#define container_of(ptr, thetype, member) ({                   \
    const typeof( ((thetype *)0)->member)* __mptr = (ptr);      \
    (thetype *)( (char*) __mptr - offsetof(thetype, member) ); })

/*
 * Return minimal value, which is greater-or-equal than given value
 * and has corresponded alignment.
 * 
 * @align should be constant and be power of 2.
 */
static inline unsigned long ALIGN_VAL(unsigned long val, unsigned long align)
{
    return (val + align - 1) & (~(align - 1));
}

#endif /* !__POK_COMMON_H__ */
