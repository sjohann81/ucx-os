/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for ATMEGA2560
 * date:          01/2022
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <lib/libc.h>
#include <uart.h>

#define TIMER_CLK2		F_CPU / 1024
#define IRQ_FREQ2		100					// irq frequency, in Hz

void _putchar(char value)
{
	uart_tx(value);
}

int32_t _kbhit(void)
{
	return uart_rx_dataready();
}

int32_t _getchar(void){
	return uart_rx_polled();
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

/* kernel auxiliary routines */
ISR(TIMER2_COMPA_vect)
{
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
