/*
 * arch.c
 *
 *  Created on: 10 mai 2019
 *      Author: Alexy
 */

#include <space.h>
#include <interrupt.h>
// #include "include/types.h": #include "./uapi/types.h"
#include <stdint.h>
#include <stddef.h>
#include <kernel/errno.h>
#include <bsp/bsp.h>
#include <space.h>
#include <libpok_legacy/libpok_legacy.h>

extern unsigned int _get_cspr_(void);
extern void _idle_loop(void);
extern void ja_fp_init(void);

void pok_arch_init(void)
{
    ja_fp_init();
    ja_bsp_init();
    ja_space_init();
}

void ja_preempt_disable(void)
{
    _disable_interrupt_();
}

void ja_preempt_enable(void)
{
    _enable_interrupt_();
}

void ja_inf_loop(void)
{
    _idle_loop();
}

pok_bool_t ja_preempt_enabled(void)
{
  return (0 == (_get_cspr_() & 0x80));
}

#include <ioports.h>
void ja_cpu_reset(void)
{
    bsp_soft_reset();
}
