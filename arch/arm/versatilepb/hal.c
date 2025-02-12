/* file:          hal.c
 * description:   hardware abstraction layer for ARM (Versatilepb)
 * date:          11/2022
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <console.h>
#include <lib/libc.h>
#include <kernel/kernel.h>


/* hardware platform dependent stuff */
static void (*isr[32])(void) = {[0 ... 31] = 0};

/*
interrupt management routines
*/
void _irq_register(uint32_t mask, void *ptr)
{
	int32_t i;

	for (i = 0; i < 32; ++i)
		if (mask & (1 << i)) {
			isr[i] = ptr;
			printf("HAL: registered irq handler at %08x\n", (uint32_t)ptr);
		}
}

void _irq_handler(void)
{
	int32_t i = 0;
	uint32_t irq;

	irq = VIC_RAWINTR;

	do {
		if (irq & 0x1){
			if (isr[i]) {
				isr[i]();
			}
		}
		irq >>= 1;
		++i;
	} while(irq);
}

static int __putchar(int value)		// polled putchar()
{
	while (UART0FR & UARTFR_TXFF);
	UART0DR = value;
	
	return value;
}

static int __kbhit(void)
{
	if (UART0FR & UARTFR_RXFF)
		return 1;
	else
		return 0;
}

static int __getchar(void)			// polled getch()
{
	while (~(UART0FR & UARTFR_RXFF));
	
	return UART0DR;
}

int32_t _interrupt_set(int32_t s)
{
	static char int_status = 1;
	
	if (s) {
		int_status = 1;
		_timer_enable();
	} else {
		int_status = 0;
		_timer_disable();
	}

	return int_status;
}

static void uart_init(uint32_t baud)
{
}

void _cpu_idle(void)
{
}

uint32_t _readcounter(void)
{
	return ~TIMER3_VALUE;
}

void _delay_ms(uint32_t msec)
{
	volatile uint32_t cur, last, delta, msecs;
	uint32_t cycles_per_msec;

	last = _readcounter();
	delta = msecs = 0;
	cycles_per_msec = TIMCLK / 1000;
	while (msec > msecs) {
		cur = _readcounter();
		if (cur < last)
			delta += (cur + (TIMCLK - last));
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

	last = _readcounter();
	delta = usecs = 0;
	cycles_per_usec = TIMCLK / 1000000;
	while (usec > usecs) {
		cur = _readcounter();
		if (cur < last)
			delta += (cur + (TIMCLK - last));
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

	if (tref == 0) _readcounter();
	if (_readcounter() < tref) tval2++;
	tref = _readcounter();
	timeref = ((uint64_t)tval2 << 32) + (uint64_t)_readcounter();

	return (timeref / (TIMCLK / 1000000));
}

void _panic(void)
{
	volatile int * const exit_device = (int* const)0x100000;
	*exit_device = 0x5555;
	while (1);
}

void _hardware_init(void)
{
	uart_init(USART_BAUD);
	
	_stdout_install(__putchar);
	_stdin_install(__getchar);
	_stdpoll_install(__kbhit);

#if F_TIMER == 0
	TIMER0_LOAD = 10000;
#else
	TIMER0_LOAD = F_CPU / F_TIMER;
#endif
	TIMER0_CONTROL = TIMER_EN | TIMER_PERIODIC | TIMER_32BIT | TIMER_INTEN;
	TIMER3_CONTROL = TIMER_EN | TIMER_32BIT;
	
	_irq_register(INTMASK_TIMERINT0_1, krnl_dispatcher);
	_timer_disable();
}

void _timer_enable(void)
{
	VIC_IRQENABLE = INTMASK_TIMERINT0_1;
}

void _timer_disable(void)
{
	VIC_IRQENABLE &= ~INTMASK_TIMERINT0_1;
}

void _interrupt_tick(void)
{
	if (TIMER0_MIS)
		TIMER0_INTCLR = 1;
}

extern void __dispatch_init(void);

void _dispatch_init(jmp_buf env)
{
	if (kcb->preemptive == 'y')
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
