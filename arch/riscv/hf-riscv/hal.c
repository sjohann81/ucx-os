/* file:          hal.c
 * description:   hardware abstraction layer for HF-RISC
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <console.h>
#include <lib/libc.h>
#include <kernel/kernel.h>
#include <device.h>
#include <uart.h>
#include <usart.h>

/*
libc basic I/O support
*/

const struct device_s *usart0 = &usart0_dev;

#ifndef DEBUG_PORT
static int __putchar(int value)		// polled putchar()
{
	usart0->api->dev_write(usart0, &value, 1);
	
	return value;
}

static int __kbhit(void)
{
	if (usart0->api->dev_read(usart0, 0, 0))
		return 1;
	else
		return 0;
}

static int __getchar(void)			// polled getch()
{
	char buf[2];
	
	usart0->api->dev_read(usart0, buf, 1);
	
	return buf[0];
}
#else
static int __putchar(int value)		// polled putchar()
{
	DEBUG_ADDR = value;
}

static int __kbhit(void)
{
	return 0;
}

static int __getchar(void)			// polled getch()
{
	return DEBUG_ADDR;
}
#endif


/* delay routines */

void _delay_ms(uint32_t msec)
{
	volatile uint32_t cur, last, delta, msecs;
	uint32_t cycles_per_msec;

	last = TIMER0;
	delta = msecs = 0;
	cycles_per_msec = F_CPU / 1000;
	while (msec > msecs) {
		cur = TIMER0;
		if (cur < last)
			delta += (cur + (F_CPU - last));
		else
			delta += (cur - last);
		last = cur;
		if (delta >= cycles_per_msec) {
			msecs += delta / cycles_per_msec;
			delta %= cycles_per_msec;
		}
	}
}

void _delay_us(uint32_t usec)
{
	volatile uint32_t cur, last, delta, usecs;
	uint32_t cycles_per_usec;

	last = TIMER0;
	delta = usecs = 0;
	cycles_per_usec = F_CPU / 1000000;
	while (usec > usecs) {
		cur = TIMER0;
		if (cur < last)
			delta += (cur + (F_CPU - last));
		else
			delta += (cur - last);
		last = cur;
		if (delta >= cycles_per_usec) {
			usecs += delta / cycles_per_usec;
			delta %= cycles_per_usec;
		}
	}
}

uint64_t _read_us(void)
{
	static uint64_t timeref = 0;
	static uint32_t tval2 = 0, tref = 0;

	if (tref == 0) TIMER0;
	if (TIMER0 < tref) tval2++;
	tref = TIMER0;
	timeref = ((uint64_t)tval2 << 32) + (uint64_t)TIMER0;

	return (timeref / (F_CPU / 1000000));
}

/* kernel auxiliary routines */

#if F_TIMER > 0
void timer1ctc_handler(void)
{
	krnl_dispatcher();
}

#else

void timer0b_handler(void)
{
	krnl_dispatcher();
}
#endif

void _hardware_init(void)
{
	_di();
	
#ifndef DEBUG_PORT
	usart0->api->dev_init(usart0);
	usart0->api->dev_open(usart0, 0);
#endif
	_stdout_install(__putchar);
	_stdin_install(__getchar);
	_stdpoll_install(__kbhit);

#if F_TIMER > 0
	TIMER1PRE = TIMERPRE_DIV16;

	/* unlock TIMER1 for reset */
	TIMER1 = TIMERSET;
	TIMER1 = 0;

	/* TIMER1 frequency: 100 interrupts/s (every 250000 cycles, 10ms timer @ 25MHz) */
	TIMER1CTC = (F_CPU / F_TIMER) / 16;
#endif
}

void _timer_enable(void)
{
#if F_TIMER > 0
	TIMERMASK |= MASK_TIMER1CTC;		/* enable interrupt mask for TIMER1 CTC events */
#else
	TIMERMASK |= MASK_TIMER0B;		/* enable interrupt mask for TIMER0B events */
#endif
}

void _timer_disable(void)
{
#if F_TIMER > 0
	TIMERMASK &= ~MASK_TIMER1CTC;		/* disable interrupt mask for TIMER1 CTC events */
#else
	TIMERMASK &= ~MASK_TIMER0B;		/* disable interrupt mask for TIMER0B events */
#endif
}

void _interrupt_tick(void)
{
	_read_us();
	_ei();
}

extern void __dispatch_init(void);

void _dispatch_init(jmp_buf env)
{
	if ((kcb->preemptive == 'y'))
		_timer_enable();
	
	__dispatch_init();
}

void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	uint32_t *ctx_p;
	
	ctx_p = (uint32_t *)ctx;
	
	ctx_p[CONTEXT_SP] = sp + ss;
	ctx_p[CONTEXT_RA] = ra;
}
