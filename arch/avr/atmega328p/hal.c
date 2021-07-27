#include <hal.h>
#include <libc.h>

#define TIMER_CLK		F_CPU / 256
#define IRQ_FREQ		10

#define UCSRA		UCSR0A
#define UCSRB		UCSR0B
#define UCSRC		UCSR0C
#define UBRRH		UBRR0H
#define UBRRL		UBRR0L
#define UDRE		UDRE0
#define UDR		UDR0
#define RXC		RXC0
#define RXEN		RXEN0
#define TXEN		TXEN0
#define UCSZ1		UCSZ01
#define UCSZ0		UCSZ00 
#define USBS		USBS0
#define RXCIE		RXCIE0

#define USART_UBBR_VALUE (((unsigned long)F_CPU/(unsigned long)(16*(unsigned long)USART_BAUD))-1)

void USART_Init(void){
	// Set baud rate
	UBRRH = (uint8_t)(USART_UBBR_VALUE >> 8);
	UBRRL = (uint8_t)USART_UBBR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSRC = (0 << USBS) | (3 << UCSZ0);
	// Enable receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
}

void _putchar(char value){
	// Wait if a byte is being transmitted
	while ((UCSRA & (1 << UDRE)) == 0);
	// Transmit data
	UDR = value;
}

int32_t _kbhit(void)
{
	if (UCSRA & (1 << RXC))
		return 1;
	else
		return 0;
}

int32_t _getchar(void){
	// Wait until a byte has been received
	while ((UCSRA & (1 << RXC)) == 0);
	// Return received data
	return UDR;
}

static char int_status;

char _interrupt_set(char s)
{
	char val = int_status;
	if (s) {
		int_status = 1;
		sei();
	} else {
		int_status = 0;
		cli();
	}

	return val;
}

/* kernel auxiliary routines */

ISR(TIMER1_COMPA_vect)
{
	dispatcher();
}

void _hardware_init(void)
{
	/* disable interrupts */
	cli();
	
	USART_Init();
	
	/* reset counters for TIMER1 */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	/* set output compare register */
	OCR1A = (TIMER_CLK / IRQ_FREQ) - 1;
	/* turn on CTC mode */
	TCCR1B |= (1 << WGM12);
	/* set CS10 and CS12 bits for 256 prescaler */
	TCCR1B |= (1 << CS12);
	
	/* enable interrupts */
	sei();
}

void _timer_enable(void)
{
	/* enable timer1 mask */
	TIMSK1 |= (1 << OCIE1A);
}

void _timer_disable(void)
{
	/* enable timer1 mask */
	TIMSK1 &= ~(1 << OCIE1A);
}

void _interrupt_tick(void)
{
}
