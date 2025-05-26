#ifndef __POK_COMPILER_H__
#define __POK_COMPILER_H__
/*
 * Functions and macros, which are compiler-specific.
 * 
 * Normally, there are operations which disables some compiler
 * optimizations, which can invalidate semanic when interrupts occures.
 * 
 * Currently assume compiler to be gcc.
 */

extern void __asm_barier__(void);
#define barrier() (__asm_barier__())

/*
 * Access (read or write) given variable only once. Do not cache its value.
 * 
 * Applicable to simple types, which can be accessed using single asm instruction.
 */
#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

/* 
 * Set given variable as a signal of some operation performed before.
 */
#define flag_set(var) do {barrier(); ACCESS_ONCE(var) = 1; } while(0)

/*
 * Return current value of variable and reset it.
 * 
 * Variable is assumed to contain some operation's indicator, which is
 * "consumed" by given operation.
 * 
 * If variable is set(non-zero), clear it and return its previous value.
 * 
 * If variable is not set(zero), return 0.
 * 
 * Such way, if flag_set() has been called concurrently, then either
 * non-zero will be returned, or variable will become non-zero, so
 * it can be found to be set after.
 */
#define flag_test_and_reset(var) ({typeof(var) __val = ACCESS_ONCE(var); if(__val) ACCESS_ONCE(var) = 0; barrier(); __val; })


/* 
 * Reset given variable.
 * 
 * Note: Using flag_reset() and flag_test_and_reset() for *single*
 * variable has no sence.
 */
#define flag_reset(var) do {barrier(); ACCESS_ONCE(var) = 0; } while(0)


#endif /* !__POK_COMPILER_H__ */
