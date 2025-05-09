/*
 * interrupt.h
 *
 *  Created on: 14 mai 2019
 *      Author: Alexy
 */

#ifndef SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_INTERRUPT_H_
#define SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_INTERRUPT_H_

void _enable_IRQ_interrupt_(void);
void _disable_IRQ_interrupt_(void);
void _enable_interrupt_(void);
void _disable_interrupt_(void);

#endif /* SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_INTERRUPT_H_ */
