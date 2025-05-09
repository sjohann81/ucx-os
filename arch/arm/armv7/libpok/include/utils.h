#ifndef __POK_UTILS_H__
#define __POK_UTILS_H__

/* 
 * Useful functions for enforce alignment.
 * 
 * By analogy with Linux kernel.
 */

/* 
 * Return minimal value, which is equal-or-more than given one
 * and has required alignment.
 */
static inline unsigned long ALIGN(unsigned long val, unsigned long align)
{
    unsigned long mask = align - 1;
    return (val + mask) & ~mask;
}

/* 
 * Return minimal pointer, which is equal-or-more than
 * given one and has required alignment.
 */
static inline void* ALIGN_PTR(void* ptr, unsigned long align)
{
    return (void*)ALIGN((unsigned long)ptr, align);
}

/* Return number of elements in the array.
 * 
 * The array should be declared as '<type> arr[<num>];'.
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0])

// TODO: this should be removed as it transforms possible read-only string.
void strtoupper(char* s);

#endif
