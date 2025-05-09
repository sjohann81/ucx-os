/*
 * space.h
 *
 *  Created on: 14 mai 2019
 *      Author: Alexy
 */

#ifndef SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_SPACE_H_
#define SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_SPACE_H_

#include <mpu.h>
/*
 * Beginning of the phys memory used for partitions.
 */
#define POK_PARTITION_MEMORY_PHYS_START URAM_BASE_ADDRESS


void ja_space_init(void);


#endif /* SOURCES_KERNEL_ARCH_ARMV7_INCLUDE_SPACE_H_ */
