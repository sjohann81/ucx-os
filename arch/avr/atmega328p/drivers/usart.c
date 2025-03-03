#include <hal.h>
#include <device.h>
#include <uart.h>
#include <usart.h>

static const struct uart_config_s usart0_cfg = {
	.config_values.port = UART_PORT0,
	.config_values.baudrate = BAUD57600,
	.config_values.databits = DATABITS8,
	.config_values.parity = PARITYNONE,
	.config_values.stopbits = STOPBITS1,
	.config_values.interrupt = INTENABLE,
	.rx_buffer_size = 32,
	.uart_config = usart0_config,
	.uart_poll = usart0_poll,
	.uart_tx = usart0_tx,
	.uart_rx = usart0_rx
};

static struct uart_data_s usart0_data;

const struct device_s usart0_dev = {
	.name = "usart0",
	.config = &usart0_cfg,
	.data = &usart0_data,
	.api = &uart_api
};


int usart0_config(struct uart_config_values_s *config)
{
	uint32_t ubrr_val;
	
	/* set USART baud rate */
	ubrr_val = ((uint32_t)F_CPU / (16 * USART_BAUD)) - 1;
	UBRR0H = (uint8_t)(ubrr_val >> 8);
	UBRR0L = (uint8_t)(ubrr_val & 0xff);
	/* set frame format to 8 data bits, no parity, 1 stop bit */
	UCSR0C = (0 << USBS0) | (3 << UCSZ00);
	/* enable receiver, transmitter and receiver interrupt */
	if (config->interrupt == INTENABLE) {
		UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	} else {
		UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	}
	
	return 0;
}

int usart0_poll(void)
{
	if (UCSR0A & (1 << RXC0))
		return 1;
	else
		return 0;
}

int usart0_tx(int val)
{
	uint8_t data = val;
	
	/* wait if a byte is being transmitted */
	while ((UCSR0A & (1 << UDRE0)) == 0);
		
	/* transmit data */
	UDR0 = data;
	
	return 0;
}

int usart0_rx(void)
{
	uint8_t data;
	
	// wait until a byte has been received
	while ((UCSR0A & (1 << RXC0)) == 0);

	// return received data
	data = UDR0;
	
	return data;
}

ISR(USART_RX_vect)
{
	usart0_data.put_fifo_cb(&usart0_data, UDR0);
}
