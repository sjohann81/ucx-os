#include <hal.h>
#include <uart.h>

#define RX_BUFFER_SIZE		32
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
	UBRRH = (uint8_t)(ubrr_val >> 8);
	UBRRL = (uint8_t)(ubrr_val & 0xff);
	/* set frame format to 8 data bits, no parity, 1 stop bit */
	UCSRC = (0 << USBS) | (3 << UCSZ0);
	/* enable receiver, transmitter and receiver interrupt */
	if (!uart_p->polled) {
		UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
	} else {
		UCSRB = (1 << RXEN) | (1 << TXEN);
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
		
	if (UCSRA & (1 << RXC))
		return 1;
	else
		return 0;
}

void uart_tx(uint8_t data)
{
	/* wait if a byte is being transmitted */
	while ((UCSRA & (1 << UDRE)) == 0);
		
	/* transmit data */
	UDR = data;
}

uint8_t uart_rx(void)
{
	uint8_t data;
	
	if (!uart_p->polled) {	
		// wait for data...
		while (uart_p->rx_head == uart_p->rx_tail);
		
		// disable RX interrupts
		UCSRB &= ~(1 << RXCIE);
		
		// fetch data from fifo
		data = uart_p->rx_buffer[uart_p->rx_head];
		uart_p->rx_head = (uart_p->rx_head + 1) & RX_BUFFER_MASK;
		uart_p->rx_size--;
		
		// RX fifo is half empty, re-enable interrupts
		if (uart_p->rx_size < RX_BUFFER_SIZE >> 1)
			UCSRB |= (1 << RXCIE);
	} else {
		// wait until a byte has been received
		while ((UCSRA & (1 << RXC)) == 0);

		// return received data
		data = UDRE;
	}
	
	return data;
}

ISR(USART_RXC_vect)
{
	uint16_t tail;

	// if there is space, put data in rx fifo
	tail = (uart_p->rx_tail + 1) & RX_BUFFER_MASK;
	if (tail != uart_p->rx_head) {
		uart_p->rx_buffer[uart_p->rx_tail] = UDR;
		uart_p->rx_tail = tail;
		uart_p->rx_size++;
	} else {
		// fifo is full, disable RX interrupts
		UCSRB &= ~(1 << RXCIE);
		
		uart_p->rx_errors++;
	}
}
