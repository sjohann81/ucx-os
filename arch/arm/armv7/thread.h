/*
 * thread.h
 *
 *  Created on: 15 mai 2019
 *      Author: Alexy
 */

#ifndef SOURCES_KERNEL_ARCH_ARMV7_THREAD_H_
#define SOURCES_KERNEL_ARCH_ARMV7_THREAD_H_


#include <stdint.h>

struct jet_context
{
  uint32_t regs[13];
  uint32_t sp;
  uint32_t lr;
};

#endif /* SOURCES_KERNEL_ARCH_ARMV7_THREAD_H_ */
