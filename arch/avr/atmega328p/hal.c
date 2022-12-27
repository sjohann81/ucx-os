/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for ATMEGA328p
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <lib/libc.h>
#include <kernel/kernel.h>
#include <uart.h>

#define TIMER_CLK2		F_CPU / 1024
#define IRQ_FREQ2		100					// irq frequency, in Hz

void _putchar(char value)
{
	uart_tx(value);
}

int32_t _kbhit(void)
{
	if (uart_rxsize())
		return 1;
	else
		return 0;
}

int32_t _getchar(void){
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

static volatile uint32_t timer = 0;

uint64_t _read_us(void)
{
	return (timer * (1000000 / IRQ_FREQ2));
}

/* kernel auxiliary routines */
ISR(TIMER2_COMPA_vect)
{
	timer++;
	krnl_dispatcher();
}

void _hardware_init(void)
{
	/* disable interrupts */
	cli();
	
	uart_init(USART_BAUD, 1);

	TCNT2 = 0;
	TCCR2A = 0;
	TCCR2B = 0;
	OCR2A = (TIMER_CLK2 / IRQ_FREQ2) - 1;
	TCCR2A |= (1 << WGM21);
   	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);		// clk / 1024 (prescaler), clear-on-match
}

void _timer_enable(void)
{
	/* enable timer2 mask */
	TIMSK2 |= (1 << OCIE2A);
}

void _timer_disable(void)
{
	/* enable timer2 mask */
	TIMSK2 &= ~(1 << OCIE2A);
}

void _interrupt_tick(void)
{
}

void _dispatch_init(jmp_buf env)
{
	extern struct kcb_s *kcb_p;
	
	longjmp(kcb_p->tcb_p->context, 1);
}

void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	uint8_t *ctx_p;
	
	ctx_p = (uint8_t *)ctx;

	ctx_p[CONTEXT_SP] = (sp + ss) & 0xff;
	ctx_p[CONTEXT_SP + 1] = (sp + ss) >> 8;
	ctx_p[CONTEXT_SR] = 0x80;			/* status register, global interrupts enabled */
	ctx_p[CONTEXT_RA] = ra & 0xff;
	ctx_p[CONTEXT_RA + 1] = ra >> 8;
}

