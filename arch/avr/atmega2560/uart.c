#include <hal.h>
#include <uart.h>

#define RX_BUFFER_SIZE		64
#define RX_BUFFER_MASK		(RX_BUFFER_SIZE - 1)

struct uart_s {
	volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
	volatile uint16_t rx_head, rx_tail, rx_size;
	volatile uint32_t rx_errors;
	uint8_t polled;
};

static struct uart_s uart;
static struct uart_s *uart_p = &uart;

void uart_init(uint32_t baud, uint8_t polled)
{
	uint32_t ubrr_val;
	
	uart_p->polled = polled;
	
	/* set USART baud rate */
	ubrr_val = ((uint32_t)F_CPU / (16 * baud)) - 1;
	UBRR0H = (uint8_t)(ubrr_val >> 8);
	UBRR0L = (uint8_t)(ubrr_val & 0xff);
	/* set frame format to 8 data bits, no parity, 1 stop bit */
	UCSR0C = (0 << USBS0) | (3 << UCSZ00);
	/* enable receiver, transmitter and receiver interrupt */
	if (!uart_p->polled) {
		UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	} else {
		UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	}
}

void uart_flush(void)
{
	uart_p->rx_head = 0;
	uart_p->rx_tail = 0;
	uart_p->rx_size = 0;
}

uint16_t uart_rxsize(void)
{
	if (!uart_p->polled)
		return uart_p->rx_size;
		
	if (UCSR0A & (1 << RXC0))
		return 1;
	else
		return 0;
}

void uart_tx(uint8_t data)
{
	/* wait if a byte is being transmitted */
	while ((UCSR0A & (1 << UDRE0)) == 0);
		
	/* transmit data */
	UDR0 = data;
}

uint8_t uart_rx(void)
{
	uint8_t data;
	
	if (!uart_p->polled) {	
		// wait for data...
		while (uart_p->rx_head == uart_p->rx_tail);
		
		// disable RX interrupts
		UCSR0B &= ~(1 << RXCIE0);
		
		// fetch data from fifo
		data = uart_p->rx_buffer[uart_p->rx_head];
		uart_p->rx_head = (uart_p->rx_head + 1) & RX_BUFFER_MASK;
		uart_p->rx_size--;
		
		// RX fifo is half empty, re-enable interrupts
		if (uart_p->rx_size < RX_BUFFER_SIZE >> 1)
			UCSR0B |= (1 << RXCIE0);
	} else {
		// wait until a byte has been received
		while ((UCSR0A & (1 << RXC0)) == 0);

		// return received data
		data = UDR0;
	}
	
	return data;
}

ISR(USART0_RX_vect)
{
	uint16_t tail;

	// if there is space, put data in rx fifo
	tail = (uart_p->rx_tail + 1) & RX_BUFFER_MASK;
	if (tail != uart_p->rx_head) {
		uart_p->rx_buffer[uart_p->rx_tail] = UDR0;
		uart_p->rx_tail = tail;
		uart_p->rx_size++;
	} else {
		// fifo is full, disable RX interrupts
		UCSR0B &= ~(1 << RXCIE0);
		
		uart_p->rx_errors++;
	}
}
