/* file:          usart.c
 * description:   hardware supported USART port driver management and routines
 * date:          05/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <stm32f4xx_usart.h>
#include <hal.h>
#include <usart.h>


/* USART definitions, data structures and basic routines */

#define RX_BUFFER_SIZE		128
#define RX_BUFFER_MASK		(RX_BUFFER_SIZE - 1)

struct usart_s {
	volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
	volatile uint16_t rx_head, rx_tail, rx_size;
	volatile uint32_t rx_errors;
	uint8_t polled;
};

static struct usart_s uartfifo[3];
static struct usart_s *uart1 = &uartfifo[0];
static struct usart_s *uart2 = &uartfifo[1];
static struct usart_s *uart6 = &uartfifo[2];

static void put_fifo(struct usart_s *usart_p, uint8_t c)
{
	uint16_t tail;

	// if there is space, put data in rx fifo
	tail = (usart_p->rx_tail + 1) & RX_BUFFER_MASK;
	if (tail != usart_p->rx_head) {
		usart_p->rx_buffer[usart_p->rx_tail] = c;
		usart_p->rx_tail = tail;
		usart_p->rx_size++;
	} else {
		// fifo is full
		usart_p->rx_errors++;
	}
}

static uint8_t get_fifo(struct usart_s *usart_p)
{
	uint8_t data = 0;
	
	if (usart_p->rx_head != usart_p->rx_tail) {
		data = usart_p->rx_buffer[usart_p->rx_head];
		usart_p->rx_head = (usart_p->rx_head + 1) & RX_BUFFER_MASK;
		usart_p->rx_size--;
	}
	
	return data;
}


/* USART interrupt service routines */

void USART1_IRQHandler(void)
{
	uint8_t c;

	while (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		c = USART_ReceiveData(USART1);
		put_fifo(uart1, c);
	}
}

void USART2_IRQHandler(void)
{
	uint8_t c;

	while (USART_GetITStatus(USART2, USART_IT_RXNE)) {
		c = USART_ReceiveData(USART2);
		put_fifo(uart2, c);
	}
}

void USART6_IRQHandler(void)
{
	uint8_t c;

	while (USART_GetITStatus(USART6, USART_IT_RXNE)) {
		c = USART_ReceiveData(USART6);
		put_fifo(uart6, c);
	}
}


/* USART application API */

int16_t usart_init(uint8_t port, uint32_t baud, uint8_t polled){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	struct usart_s *usart_p = 0;
	
	switch (port) {
	case 1:
		usart_p = uart1;
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
		USART_InitStruct.USART_BaudRate = baud;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART1, &USART_InitStruct);

		if (!polled) {
			// enable the USART1 receive interrupt
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			usart_flush(1);
		}

		// Enable USART1
		USART_Cmd(USART1, ENABLE);
		break;
	case 2:
		usart_p = uart2;
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
		USART_InitStruct.USART_BaudRate = baud;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART2, &USART_InitStruct);

		if (!polled) {
			// enable the USART2 receive interrupt
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			usart_flush(2);
		}

		// Enable USART2
		USART_Cmd(USART2, ENABLE);
		break;
	case 6:
		usart_p = uart6;
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
		USART_InitStruct.USART_BaudRate = baud;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART6, &USART_InitStruct);

		if (!polled) {
			// enable the USART6 receive interrupt
			USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			usart_flush(6);
		}

		// Enable USART6
		USART_Cmd(USART6, ENABLE);
		break;
	default:
		break;
	}
	
	if (!usart_p)
		return -1;
		
	usart_p->polled = polled;
	
	return 0;
}

void usart_flush(uint8_t port)
{
	struct usart_s *usart_p = 0;
	
	switch (port) {
	case 1:
		usart_p = uart1;
		break;
	case 2:
		usart_p = uart2;
		break;
	case 6:
		usart_p = uart6;
		break;
	default:
		return;
	}
	
	usart_p->rx_head = 0;
	usart_p->rx_tail = 0;
	usart_p->rx_size = 0;
}

uint16_t usart_rxsize(uint8_t port)
{
	struct usart_s *usart_p = 0;
	uint32_t data = 0;
	
	switch (port) {
	case 1:
		usart_p = uart1;
		data = USART_GetFlagStatus(USART1, USART_FLAG_RXNE);
		break;
	case 2:
		usart_p = uart2;
		data = USART_GetFlagStatus(USART2, USART_FLAG_RXNE);
		break;
	case 6:
		usart_p = uart6;
		data = USART_GetFlagStatus(USART6, USART_FLAG_RXNE);
		break;
	default:
		return 0;
	}
	
	if (!usart_p->polled)
		return usart_p->rx_size;
	
	if (data)
		return 1;
	
	return 0;
}

void usart_tx(uint8_t port, uint8_t data)
{
	switch (port) {
	case 1:
		// Wait until transmit data register is empty
		while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		USART_SendData(USART1, data);
		break;
	case 2:
		while (!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
		USART_SendData(USART2, data);
		break;
	case 6:
		while (!USART_GetFlagStatus(USART6, USART_FLAG_TXE));
		USART_SendData(USART6, data);
		break;
	default:
		break;
	}

}

uint8_t usart_rx(uint8_t port)
{
	struct usart_s *usart_p = 0;
	uint8_t data;

	switch (port) {
	case 1:
		usart_p = uart1;
		if (!usart_p->polled) {
			// wait for data...
			while (usart_p->rx_head == usart_p->rx_tail);

			_di();
			// fetch data from fifo
			data = get_fifo(usart_p);
			_ei();
		} else {
			while (!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
			
			return USART_ReceiveData(USART1);
		}
		break;
	case 2:
		usart_p = uart2;
		if (!usart_p->polled) {
			// wait for data...
			while (usart_p->rx_head == usart_p->rx_tail);
			
			_di();
			// fetch data from fifo
			data = get_fifo(usart_p);
			_ei();
		} else {
			while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
			
			return USART_ReceiveData(USART2);
		}
		break;
	case 6:
		usart_p = uart6;
		if (!usart_p->polled) {
			// wait for data...
			while (usart_p->rx_head == usart_p->rx_tail);
			
			_di();
			// fetch data from fifo
			data = get_fifo(usart_p);
			_ei();
		} else {
			while (!USART_GetFlagStatus(USART6, USART_FLAG_RXNE));
			
			return USART_ReceiveData(USART6);
		}
		break;
	default:
		return 0;
	}

	return data;
}
