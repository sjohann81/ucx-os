#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <stm32f4xx_usart.h>
#include <hal.h>
#include <device.h>
#include <uart.h>
#include <usart_ll.h>
#include <usbd_cdc_vcp.h>


#if USART_BAUD==9600
#define UART_BAUD	BAUD9600
#endif
#if USART_BAUD==57600
#define UART_BAUD	BAUD57600
#endif
#if USART_BAUD==115200
#define UART_BAUD	BAUD115200
#endif

/* UART configuration and data for ports 1, 2 and 6 */
USB_OTG_CORE_HANDLE USB_OTG_dev;

static const struct uart_config_s uart0_cfg = {
	.config_values.port = UART_PORT0,
	.config_values.baudrate = UART_BAUD,
	.config_values.databits = DATABITS8,
	.config_values.parity = PARITYNONE,
	.config_values.stopbits = STOPBITS1,
	.config_values.interrupt = INTDISABLE,
	.rx_buffer_size = 0,
	.uart_config = uart_config,
	.uart_poll = uart0_poll,
	.uart_tx = uart0_tx,
	.uart_rx = uart0_rx
};

static struct uart_data_s uart0_data;

const struct device_s uart0_dev = {
	.name = "uart0",
	.config = &uart0_cfg,
	.data = &uart0_data,
	.api = &uart_api
};

static const struct uart_config_s uart1_cfg = {
	.config_values.port = UART_PORT1,
	.config_values.baudrate = UART_BAUD,
	.config_values.databits = DATABITS8,
	.config_values.parity = PARITYNONE,
	.config_values.stopbits = STOPBITS1,
	.config_values.interrupt = INTENABLE,
	.rx_buffer_size = 128,
	.uart_config = uart_config,
	.uart_poll = uart1_poll,
	.uart_tx = uart1_tx,
	.uart_rx = uart1_rx
};

static struct uart_data_s uart1_data;

const struct device_s uart1_dev = {
	.name = "uart1",
	.config = &uart1_cfg,
	.data = &uart1_data,
	.api = &uart_api
};


static const struct uart_config_s uart2_cfg = {
	.config_values.port = UART_PORT2,
	.config_values.baudrate = UART_BAUD,
	.config_values.databits = DATABITS8,
	.config_values.parity = PARITYNONE,
	.config_values.stopbits = STOPBITS1,
	.config_values.interrupt = INTENABLE,
	.rx_buffer_size = 128,
	.uart_config = uart_config,
	.uart_poll = uart2_poll,
	.uart_tx = uart2_tx,
	.uart_rx = uart2_rx
};

static struct uart_data_s uart2_data;

const struct device_s uart2_dev = {
	.name = "uart2",
	.config = &uart2_cfg,
	.data = &uart2_data,
	.api = &uart_api
};


static const struct uart_config_s uart6_cfg = {
	.config_values.port = UART_PORT6,
	.config_values.baudrate = UART_BAUD,
	.config_values.databits = DATABITS8,
	.config_values.parity = PARITYNONE,
	.config_values.stopbits = STOPBITS1,
	.config_values.interrupt = INTENABLE,
	.rx_buffer_size = 128,
	.uart_config = uart_config,
	.uart_poll = uart6_poll,
	.uart_tx = uart6_tx,
	.uart_rx = uart6_rx
};

static struct uart_data_s uart6_data;

const struct device_s uart6_dev = {
	.name = "uart6",
	.config = &uart6_cfg,
	.data = &uart6_data,
	.api = &uart_api
};


/* USART interrupt service routines */

void USART1_IRQHandler(void)
{
	uint8_t c;

	while (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		c = USART_ReceiveData(USART1);
		uart1_data.put_fifo_cb(&uart1_data, c);
	}
}

void USART2_IRQHandler(void)
{
	uint8_t c;

	while (USART_GetITStatus(USART2, USART_IT_RXNE)) {
		c = USART_ReceiveData(USART2);
		uart2_data.put_fifo_cb(&uart2_data, c);
	}
}

void USART6_IRQHandler(void)
{
	uint8_t c;

	while (USART_GetITStatus(USART6, USART_IT_RXNE)) {
		c = USART_ReceiveData(USART6);
		uart6_data.put_fifo_cb(&uart6_data, c);
	}
}


/* driver callbacks */

int uart_config(struct uart_config_values_s *config)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	int val = 0;
	
	switch (config->port) {
	case UART_PORT0:
		/* USB CDC UART */
		/* GPIOA Peripheral clock enable. */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/* Configure PA8 in output pushpull mode. */
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		GPIO_ResetBits(GPIOA, GPIO_Pin_15);
		_delay_ms(100);
		GPIO_SetBits(GPIOA, GPIO_Pin_15);
		_delay_ms(500);

		USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
		
		return 0;
	case UART_PORT1:
		// Enable clock for GPIOB
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		// Enable clock for USART1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		// Connect PB6 to USART1_Tx
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
		// Connect PB7 to USART1_Rx
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		// Initialization of GPIOB
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		// Initialization of USART1
		USART_InitStruct.USART_BaudRate = USART_BAUD;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART1, &USART_InitStruct);

		if (config->interrupt == INTENABLE) {
			// enable the USART1 receive interrupt
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}

		// Enable USART1
		USART_Cmd(USART1, ENABLE);
		break;
	case UART_PORT2:
		// Enable clock for GPIOA
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		// Enable clock for USART2
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		// Connect PA2 to USART2_Tx
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		// Connect PA3 to USART2_Rx
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

		// Initialization of GPIOA
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		// Initialization of USART2
		USART_InitStruct.USART_BaudRate = USART_BAUD;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART2, &USART_InitStruct);

		if (config->interrupt == INTENABLE) {
			// enable the USART2 receive interrupt
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}

		// Enable USART2
		USART_Cmd(USART2, ENABLE);
		break;
	case UART_PORT6:
		// Enable clock for GPIOC
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		// Enable clock for USART1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

		// Connect PA11 to USART6_Tx
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_USART6);
		// Connect PA12 to USART6_Rx
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_USART6);

		// Initialization of GPIOC
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStruct);

		// Initialization of USART6
		USART_InitStruct.USART_BaudRate = USART_BAUD;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART6, &USART_InitStruct);

		if (config->interrupt == INTENABLE) {
			// enable the USART6 receive interrupt
			USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}

		// Enable USART6
		USART_Cmd(USART6, ENABLE);
		break;
	default:
		val = -1;
		break;
	}
	
	return val;
}


int uart0_poll(void)
{
	return VCP_kbhit();
}

int uart1_poll(void)
{
	uint32_t data = 0;
	
	data = USART_GetFlagStatus(USART1, USART_FLAG_RXNE);
	
	if (data)
		return 1;
	else
		return 0;
}

int uart2_poll(void)
{
	uint32_t data = 0;
	
	data = USART_GetFlagStatus(USART2, USART_FLAG_RXNE);
	
	if (data)
		return 1;
	else
		return 0;
}

int uart6_poll(void)
{
	uint32_t data = 0;
	
	data = USART_GetFlagStatus(USART6, USART_FLAG_RXNE);
	
	if (data)
		return 1;
	else
		return 0;
}


int uart0_tx(int val)
{
	static int bytes = 0;
	uint8_t data = val;
	
	VCP_putchar(data);
	if (bytes++ == 100) {
		bytes = 0;
		_delay_ms(5);
	}
	
	return 0;
}

int uart1_tx(int val)
{
	uint8_t data = val;
	
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	USART_SendData(USART1, data);
	
	return 0;
}

int uart2_tx(int val)
{
	uint8_t data = val;
	
	while (!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
	USART_SendData(USART2, data);
	
	return 0;
}

int uart6_tx(int val)
{
	uint8_t data = val;
	
	while (!USART_GetFlagStatus(USART6, USART_FLAG_TXE));
	USART_SendData(USART6, data);
	
	return 0;
}


int uart0_rx(void)
{
	uint8_t data;
	
	while (1) {
		if (VCP_getchar(&data))
			return data;
	}
}

int uart1_rx(void)
{
	while (!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
	
	return USART_ReceiveData(USART1);
}

int uart2_rx(void)
{
	while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
	
	return USART_ReceiveData(USART2);
}

int uart6_rx(void)
{
	while (!USART_GetFlagStatus(USART6, USART_FLAG_RXNE));
	
	return USART_ReceiveData(USART6);
}
