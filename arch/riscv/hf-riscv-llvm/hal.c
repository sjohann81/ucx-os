/* file:          hal.c
 * description:   hardware abstraction layer for HF-RISC
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <lib/libc.h>

/*
libc basic I/O support
*/

#ifndef DEBUG_PORT
void _putchar(char value){		// polled putchar()
	while (UARTCAUSE & MASK_UART0_WRITEBUSY);
	UART0 = value;
}

int32_t _kbhit(void){
	return UARTCAUSE & MASK_UART0_DATAAVAIL;
}

int32_t _getchar(void){			// polled getch()
	while (!_kbhit());
	return UART0;
}
#else
void _putchar(char value){		// polled putchar()
	DEBUG_ADDR = value;
}

int32_t _kbhit(void){
	return 0;
}

int32_t _getchar(void){			// polled getch()
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
	cycles_per_msec = CPU_SPEED / 1000;
	while (msec > msecs) {
		cur = TIMER0;
		if (cur < last)
			delta += (cur + (CPU_SPEED - last));
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
	cycles_per_usec = CPU_SPEED / 1000000;
	while (usec > usecs) {
		cur = TIMER0;
		if (cur < last)
			delta += (cur + (CPU_SPEED - last));
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

	return (timeref / (CPU_SPEED / 1000000));
}

/* kernel auxiliary routines */

void timer1ctc_handler(void)
{
	krnl_dispatcher();
}

void _hardware_init(void)
{
	_di();
	TIMER1PRE = TIMERPRE_DIV16;

	/* unlock TIMER1 for reset */
	TIMER1 = TIMERSET;
	TIMER1 = 0;

	/* TIMER1 frequency: (39063 * 16) = 250000 cycles (10ms timer @ 25MHz) */
	TIMER1CTC = 15625;
}

void _timer_enable(void)
{
	TIMERMASK |= MASK_TIMER1CTC;		/* enable interrupt mask for TIMER1 CTC events */
}

void _timer_disable(void)
{
	TIMERMASK &= ~MASK_TIMER1CTC;		/* disable interrupt mask for TIMER1 CTC events */
}

void _interrupt_tick(void)
{
	_ei();
}

void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	uint32_t *ctx_p;
	
	ctx_p = (uint32_t *)ctx;
	
	ctx_p[CONTEXT_SP] = sp + ss;
	ctx_p[CONTEXT_RA] = ra;
}
