/*
 * stack.h
 *
 *  Created on: 14 mai 2019
 *      Author: Alexy
 */

#ifndef SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_STACK_H_
#define SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_STACK_H_


/*
 * Any stack returned by pok_stack_alloc() has two words *above* it
 * filled with zeros.
 *
 * So, whenever sp register points to the beginning of the stack,
 * it is treated as the last stack frame.
 */
typedef uint32_t jet_stack_t;


#endif /* SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_STACK_H_ */
