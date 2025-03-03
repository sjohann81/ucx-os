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
	UBRRH = (uint8_t)(ubrr_val >> 8);
	UBRRL = (uint8_t)(ubrr_val & 0xff);
	/* set frame format to 8 data bits, no parity, 1 stop bit */
	UCSRC = (0 << USBS) | (3 << UCSZ0);
	/* enable receiver, transmitter and receiver interrupt */
	if (config->interrupt == INTENABLE) {
		UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
	} else {
		UCSRB = (1 << RXEN) | (1 << TXEN);
	}
	
	return 0;
}

int usart0_poll(void)
{
	if (UCSRA & (1 << RXC))
		return 1;
	else
		return 0;
}

int usart0_tx(int val)
{
	uint8_t data = val;
	
	/* wait if a byte is being transmitted */
	while ((UCSRA & (1 << UDRE)) == 0);
		
	/* transmit data */
	UDR = data;
	
	return 0;
}

int usart0_rx(void)
{
	uint8_t data;
	
	// wait until a byte has been received
	while ((UCSRA & (1 << RXC)) == 0);

	// return received data
	data = UDR;
	
	return data;
}

ISR(USART_RX_vect)
{
	usart0_data.put_fifo_cb(&usart0_data, UDR);
}
