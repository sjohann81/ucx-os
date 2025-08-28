#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <hal.h>
#include <device.h>
#include <gpio.h>
#include <gpio_ll.h>


int gpio_ll_setup(struct gpio_config_values_s *cfg)
{
	uint32_t modesel, pullsel;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	for (int i = 0; i < 16; i++) {
		if (cfg->pinsel & (1 << i)) {
			modesel = (cfg->mode & (3 << (i << 1))) >> (i << 1);
			pullsel = (cfg->pull & (3 << (i << 1))) >> (i << 1);
			switch (modesel) {
			case GPIO_INPUT:
				switch (cfg->port) {
				case GPIO_PORTA:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					switch (pullsel) {
					case GPIO_NOPULL: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; break;
					case GPIO_PULLUP: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; break;
					case GPIO_PULLDOWN: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; break;
					default: return -1;
					}
					GPIO_Init(GPIOA, &GPIO_InitStructure);
					break;
				case GPIO_PORTB: 
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					switch (pullsel) {
					case GPIO_NOPULL: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; break;
					case GPIO_PULLUP: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; break;
					case GPIO_PULLDOWN: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; break;
					default: return -1;
					}
					GPIO_Init(GPIOB, &GPIO_InitStructure);
					break;
				case GPIO_PORTC:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					switch (pullsel) {
					case GPIO_NOPULL: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; break;
					case GPIO_PULLUP: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; break;
					case GPIO_PULLDOWN: GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; break;
					default: return -1;
					}
					GPIO_Init(GPIOC, &GPIO_InitStructure);
					break;
				default:
					return -1;
				}
				break;
			case GPIO_OUTPUT:
				switch (cfg->port) {
				case GPIO_PORTA:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
					GPIO_Init(GPIOA, &GPIO_InitStructure);
					break;
				case GPIO_PORTB:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
					GPIO_Init(GPIOB, &GPIO_InitStructure);
					break;
				case GPIO_PORTC:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
					GPIO_Init(GPIOC, &GPIO_InitStructure);
					break;
				default:
					return -1;
				}
				break;
			case GPIO_OUTPUT_OD:
				switch (cfg->port) {
				case GPIO_PORTA:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
					GPIO_Init(GPIOA, &GPIO_InitStructure);
					break;
				case GPIO_PORTB:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
					GPIO_Init(GPIOB, &GPIO_InitStructure);
					break;
				case GPIO_PORTC:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
					GPIO_InitStructure.GPIO_Pin   = (1 << i);
					GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
					GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
					GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
					GPIO_Init(GPIOC, &GPIO_InitStructure);
					break;
				default:
					return -1;
				}
				break;
			default:
				return -1;
			}
		}
	}

	return 0;
}

int gpio_ll_get(struct gpio_config_values_s *cfg)
{
	uint16_t val = 0;
	
	switch (cfg->port) {
	case GPIO_PORTA: val = GPIO_ReadInputData(GPIOA); break;
	case GPIO_PORTB: val = GPIO_ReadInputData(GPIOB); break;
	case GPIO_PORTC: val = GPIO_ReadInputData(GPIOC); break;
	default: 
		return -1;
	}
	
	return val & cfg->pinsel;
}

int gpio_ll_set(struct gpio_config_values_s *cfg, int val)
{
	uint16_t mask = val & cfg->pinsel;
	
	switch (cfg->port) {
	case GPIO_PORTA: GPIO_SetBits(GPIOA, mask); break;
	case GPIO_PORTB: GPIO_SetBits(GPIOB, mask); break;
	case GPIO_PORTC: GPIO_SetBits(GPIOC, mask); break;
	default:
		return -1;
	}
	
	return 0;
}

int gpio_ll_clear(struct gpio_config_values_s *cfg, int val)
{
	uint16_t mask = val & cfg->pinsel;
	
	switch (cfg->port) {
	case GPIO_PORTA: GPIO_ResetBits(GPIOA, mask); break;
	case GPIO_PORTB: GPIO_ResetBits(GPIOB, mask); break;
	case GPIO_PORTC: GPIO_ResetBits(GPIOC, mask); break;
	default:
		return -1;
	}
	
	return 0;
}

int gpio_ll_toggle(struct gpio_config_values_s *cfg, int val)
{
	uint16_t mask = val & cfg->pinsel;
	
	switch (cfg->port) {
	case GPIO_PORTA: GPIO_ToggleBits(GPIOA, mask); break;
	case GPIO_PORTB: GPIO_ToggleBits(GPIOB, mask); break;
	case GPIO_PORTC: GPIO_ToggleBits(GPIOC, mask); break;
	default:
		return -1;
	}
	
	return 0;
}


static void (*int_cb[MAX_INT_SOURCES])(void) = { 0 };

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		(int_cb[0])();
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		(int_cb[1])();
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		(int_cb[2])();
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
		(int_cb[3])();
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
		(int_cb[4])();
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
		(int_cb[5])();
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
		(int_cb[6])();
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
		(int_cb[7])();
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
		(int_cb[8])();
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
		(int_cb[9])();
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line10) != RESET) {
		(int_cb[10])();
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
		(int_cb[11])();
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
		(int_cb[12])();
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
		(int_cb[13])();
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if (EXTI_GetITStatus(EXTI_Line14) != RESET) {
		(int_cb[14])();
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if (EXTI_GetITStatus(EXTI_Line15) != RESET) {
		(int_cb[15])();
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

int gpio_ll_int_attach(struct gpio_config_values_s *cfg, int pin, void (*callback)(), int trigger)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	switch (trigger) {
	case GPIO_CHANGE:
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		break;
	case GPIO_RISING:
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		break;
	case GPIO_FALLING:
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		break;
	default:
		return -1;
	}
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	switch (cfg->port) {
	case GPIO_PORTA:
		switch (pin) {
		case GPIO_PIN0:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
			EXTI_InitStructure.EXTI_Line = EXTI_Line0;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
			int_cb[0] = callback;
			break;
		case GPIO_PIN1:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
			EXTI_InitStructure.EXTI_Line = EXTI_Line1;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
			int_cb[1] = callback;
			break;
		case GPIO_PIN2:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);
			EXTI_InitStructure.EXTI_Line = EXTI_Line2;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
			int_cb[2] = callback;
			break;
		case GPIO_PIN3:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);
			EXTI_InitStructure.EXTI_Line = EXTI_Line3;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
			int_cb[3] = callback;
			break;
		case GPIO_PIN4:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
			EXTI_InitStructure.EXTI_Line = EXTI_Line4;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
			int_cb[4] = callback;
			break;
		case GPIO_PIN5:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);
			EXTI_InitStructure.EXTI_Line = EXTI_Line5;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[5] = callback;
			break;
		case GPIO_PIN6:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);
			EXTI_InitStructure.EXTI_Line = EXTI_Line6;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[6] = callback;
			break;
		case GPIO_PIN7:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);
			EXTI_InitStructure.EXTI_Line = EXTI_Line7;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[7] = callback;
			break;
		case GPIO_PIN8:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);
			EXTI_InitStructure.EXTI_Line = EXTI_Line8;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[8] = callback;
			break;
		case GPIO_PIN9:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource9);
			EXTI_InitStructure.EXTI_Line = EXTI_Line9;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[9] = callback;
			break;
		case GPIO_PIN10:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);
			EXTI_InitStructure.EXTI_Line = EXTI_Line10;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[10] = callback;
			break;
		case GPIO_PIN11:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource11);
			EXTI_InitStructure.EXTI_Line = EXTI_Line11;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[11] = callback;
			break;
		case GPIO_PIN12:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource12);
			EXTI_InitStructure.EXTI_Line = EXTI_Line12;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[12] = callback;
			break;
		case GPIO_PIN13:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource13);
			EXTI_InitStructure.EXTI_Line = EXTI_Line13;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[13] = callback;
			break;
		case GPIO_PIN14:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource14);
			EXTI_InitStructure.EXTI_Line = EXTI_Line14;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[14] = callback;
			break;
		case GPIO_PIN15:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource15);
			EXTI_InitStructure.EXTI_Line = EXTI_Line15;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[15] = callback;
			break;
		default:
			return -1;
		}
		break;
	case GPIO_PORTB:
		switch (pin) {
		case GPIO_PIN0:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
			EXTI_InitStructure.EXTI_Line = EXTI_Line0;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
			int_cb[0] = callback;
			break;
		case GPIO_PIN1:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
			EXTI_InitStructure.EXTI_Line = EXTI_Line1;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
			int_cb[1] = callback;
			break;
		case GPIO_PIN2:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
			EXTI_InitStructure.EXTI_Line = EXTI_Line2;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
			int_cb[2] = callback;
			break;
		case GPIO_PIN3:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);
			EXTI_InitStructure.EXTI_Line = EXTI_Line3;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
			int_cb[3] = callback;
			break;
		case GPIO_PIN4:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
			EXTI_InitStructure.EXTI_Line = EXTI_Line4;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
			int_cb[4] = callback;
			break;
		case GPIO_PIN5:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
			EXTI_InitStructure.EXTI_Line = EXTI_Line5;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[5] = callback;
			break;
		case GPIO_PIN6:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);
			EXTI_InitStructure.EXTI_Line = EXTI_Line6;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[6] = callback;
			break;
		case GPIO_PIN7:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource7);
			EXTI_InitStructure.EXTI_Line = EXTI_Line7;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[7] = callback;
			break;
		case GPIO_PIN8:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);
			EXTI_InitStructure.EXTI_Line = EXTI_Line8;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[8] = callback;
			break;
		case GPIO_PIN9:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);
			EXTI_InitStructure.EXTI_Line = EXTI_Line9;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			int_cb[9] = callback;
			break;
		case GPIO_PIN10:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
			EXTI_InitStructure.EXTI_Line = EXTI_Line10;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[10] = callback;
			break;
		case GPIO_PIN11:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
			EXTI_InitStructure.EXTI_Line = EXTI_Line11;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[11] = callback;
			break;
		case GPIO_PIN12:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);
			EXTI_InitStructure.EXTI_Line = EXTI_Line12;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[12] = callback;
			break;
		case GPIO_PIN13:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13);
			EXTI_InitStructure.EXTI_Line = EXTI_Line13;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[13] = callback;
			break;
		case GPIO_PIN14:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);
			EXTI_InitStructure.EXTI_Line = EXTI_Line14;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[14] = callback;
			break;
		case GPIO_PIN15:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);
			EXTI_InitStructure.EXTI_Line = EXTI_Line15;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[15] = callback;
			break;
		default:
			return -1;
		}
		break;
	case GPIO_PORTC:
		switch (pin) {
		case GPIO_PIN13:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
			EXTI_InitStructure.EXTI_Line = EXTI_Line13;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[13] = callback;
			break;
		case GPIO_PIN14:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource14);
			EXTI_InitStructure.EXTI_Line = EXTI_Line14;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[14] = callback;
			break;
		case GPIO_PIN15:
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource15);
			EXTI_InitStructure.EXTI_Line = EXTI_Line15;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			int_cb[15] = callback;
			break;
		default:
			return -1;
		}
		break;
	default:
		return -1;
	}

	EXTI_Init(&EXTI_InitStructure);
//	NVIC_Init(&NVIC_InitStructure);		// must run in supervisor mode!
	syscall((void (*)(void *))NVIC_Init, &NVIC_InitStructure);
	
	return 0;
}
