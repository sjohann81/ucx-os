/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for ATMEGA32
 * date:          12/2022
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <console.h>
#include <lib/libc.h>
#include <kernel/kernel.h>
#include <uart.h>

#define TIMER_CLK2		F_CPU / 1024
#define IRQ_FREQ2		100					// irq frequency, in Hz

static int __putchar(int value)		// polled putchar()
{
	uart_tx(value);
	
	return value;
}

static int __kbhit(void)
{
	if (uart_rxsize())
		return 1;
	else
		return 0;
}

static int __getchar(void)			// polled getch()
{
	return uart_rx();
}

char _interrupt_set(char s)
{
	static char int_status = 1;
	
	if (s) {
		int_status = 1;
		sei();
	} else {
		int_status = 0;
		cli();
	}

	return int_status;
}

/* delay routines */
#undef _delay_us

void delay_us(uint16_t us)
{
	while (us >= 16) {
		_delay_us(16);
		us -= 16;
	}

	if(us & 0x8) _delay_us(8);
	if(us & 0x4) _delay_us(4);
	if(us & 0x2) _delay_us(2);
	if(us & 0x1) _delay_us(1);
}

static volatile uint32_t timer = 0;

uint64_t _read_us(void)
{
	return (timer * (1000000 / IRQ_FREQ2));
}

/* kernel auxiliary routines */
ISR(TIMER2_COMP_vect)
{
	timer++;
	krnl_dispatcher();
}

void _hardware_init(void)
{
	/* disable interrupts */
	cli();
	
	uart_init(USART_BAUD, 1);
	
	_stdout_install(__putchar);
	_stdin_install(__getchar);
	_stdpoll_install(__kbhit);

	TCNT2 = 0;
	TCCR2 = 0;
	OCR2 = (TIMER_CLK2 / IRQ_FREQ2) - 1;
	TCCR2 |= (1 << WGM21) | (1 << CS22) | (1 << CS21) | (1 << CS20);		// clk / 1024 (prescaler), clear-on-match
}

void _timer_enable(void)
{
	/* enable timer2 mask */
	TIMSK |= (1 << OCIE2);
}

void _timer_disable(void)
{
	/* disable timer2 mask */
	TIMSK &= ~(1 << OCIE2);
}

void _interrupt_tick(void)
{
}

void _dispatch_init(jmp_buf env)
{
	if ((kcb->preemptive == 'y')) {
		/* clear pending output compare 2 interrupt */
		TIFR = (1 << OCF2);
		/* enable timer2 interrupt mask */
		TIMSK |= (1 << OCIE2);
	}
	
	longjmp(env, 1);
}

void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	uint8_t *ctx_p;
	int i;
	
	ctx_p = (uint8_t *)ctx;

	for (i = 0; i < sizeof(jmp_buf); i++)
		ctx_p[i] = 0;

	ctx_p[CONTEXT_SP] = (sp + ss) & 0xff;
	ctx_p[CONTEXT_SP + 1] = (sp + ss) >> 8;
	ctx_p[CONTEXT_SR] = 0x80;			/* status register, global interrupts enabled */
	ctx_p[CONTEXT_RA] = ra & 0xff;
	ctx_p[CONTEXT_RA + 1] = ra >> 8;
}
