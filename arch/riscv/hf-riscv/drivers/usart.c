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
	uint16_t d;

	d = (uint16_t)(F_CPU / USART_BAUD);
	UART0DIV = d;
	UART0 = 0;

	if (config->interrupt == INTENABLE) {
		UARTMASK |= MASK_UART0_DATAAVAIL;
	}

	PAALTCFG0 |= (MASK_UART0_TX | MASK_UART0_RX);

	return 0;
}

int usart0_poll(void)
{
	if (UARTCAUSE & MASK_UART0_DATAAVAIL)
		return 1;
	else
		return 0;
}

int usart0_tx(int val)
{
	while (UARTCAUSE & MASK_UART0_WRITEBUSY);
	UART0 = val;
	
	return 0;
}

int usart0_rx(void)
{
	uint8_t data;
	
	while (!usart0_poll());
	data = UART0;
	
	return data;
}

void uart0rx_handler(void)
{
	usart0_data.put_fifo_cb(&usart0_data, UART0);
}
