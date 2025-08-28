#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <hal.h>
#include <device.h>
#include <i2c.h>
#include <i2c_ll.h>
#include <libc.h>

#define I2C_TO_US	5000

int i2c_ll_init(struct i2c_hw_config_values_s *config_values)
{
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	switch (config_values->port) {
	case I2C_PORT1:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

		I2C_InitStruct.I2C_ClockSpeed = config_values->speed;
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0xe0;
		I2C_InitStruct.I2C_Ack = config_values->ack == I2C_ACK ? I2C_Ack_Enable : I2C_Ack_Disable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_Init(I2C1, &I2C_InitStruct);
		I2C_Cmd(I2C1, ENABLE);
		break;
	case I2C_PORT1_ALT:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

		I2C_InitStruct.I2C_ClockSpeed = config_values->speed;
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0xe0;
		I2C_InitStruct.I2C_Ack = config_values->ack == I2C_ACK ? I2C_Ack_Enable : I2C_Ack_Disable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_Init(I2C1, &I2C_InitStruct);
		I2C_Cmd(I2C1, ENABLE);
		break;
	case I2C_PORT2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF9_I2C2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);

		I2C_InitStruct.I2C_ClockSpeed = config_values->speed;
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0xe2;
		I2C_InitStruct.I2C_Ack = config_values->ack == I2C_ACK ? I2C_Ack_Enable : I2C_Ack_Disable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_Init(I2C2, &I2C_InitStruct);
		I2C_Cmd(I2C2, ENABLE);
		break;
	case I2C_PORT3:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3, DISABLE);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF9_I2C3);

		I2C_InitStruct.I2C_ClockSpeed = config_values->speed;
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0xe4;
		I2C_InitStruct.I2C_Ack = config_values->ack == I2C_ACK ? I2C_Ack_Enable : I2C_Ack_Disable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_Init(I2C3, &I2C_InitStruct);
		I2C_Cmd(I2C3, ENABLE);
	default:
		return -1;
	}
	
	return 0;
}

int i2c_ll_deinit(struct i2c_hw_config_values_s *config_values)
{
	switch (config_values->port) {
	case I2C_PORT1:
		I2C_DeInit(I2C1);
		break;
	case I2C_PORT2:
		I2C_DeInit(I2C2);
		break;
	case I2C_PORT3:
		I2C_DeInit(I2C3);
	default:
		return -1;
	}
	
	return 0;
}

int i2c_ll_start(struct i2c_hw_config_values_s *config_values)
{
	uint32_t to = 0;

	switch (config_values->port) {
	case I2C_PORT1:
		I2C_GenerateSTART(I2C1, ENABLE);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT2:
		I2C_GenerateSTART(I2C2, ENABLE);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT3:
		I2C_GenerateSTART(I2C3, ENABLE);
		while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	default:
		return -1;
	}

	return 0;
}

int i2c_ll_restart(struct i2c_hw_config_values_s *config_values)
{
	return i2c_ll_start(config_values);
}

int i2c_ll_stop(struct i2c_hw_config_values_s *config_values)
{
	uint32_t to = 0;

	switch (config_values->port) {
	case I2C_PORT1:
		I2C_GenerateSTOP(I2C1, ENABLE);
		while (I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT2:
		I2C_GenerateSTOP(I2C2, ENABLE);
		while (I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT3:
		I2C_GenerateSTOP(I2C3, ENABLE);
		while (I2C_GetFlagStatus(I2C3, I2C_FLAG_STOPF))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	default:
		return -1;
	}
	
	return 0;	
}

int i2c_ll_wr_addr(struct i2c_hw_config_values_s *config_values, unsigned short addr)
{
	uint32_t to = 0;

	switch (config_values->port) {
	case I2C_PORT1:
		I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT2:
		I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT3:
		I2C_Send7bitAddress(I2C3, addr, I2C_Direction_Transmitter);
		while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	default:
		return -1;
	}
	
	return 0;
}

int i2c_ll_rd_addr(struct i2c_hw_config_values_s *config_values, unsigned short addr)
{
	uint32_t to = 0;

	switch (config_values->port) {
	case I2C_PORT1:
		I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT2:
		I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Receiver);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT3:
		I2C_Send7bitAddress(I2C3, addr, I2C_Direction_Receiver);
		while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	default:
		return -1;
	}
	
	return 0;
}

int i2c_ll_write(struct i2c_hw_config_values_s *config_values, unsigned char val)
{
	uint32_t to = 0;

	switch (config_values->port) {
	case I2C_PORT1:
		I2C_SendData(I2C1, val);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT2:
		I2C_SendData(I2C2, val);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	case I2C_PORT3:
		I2C_SendData(I2C3, val);
		while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		break;
	default:
		return -1;
	}
	
	return 0;
}

int i2c_ll_read(struct i2c_hw_config_values_s *config_values, uint8_t nack)
{
	uint32_t to = 0;

	switch (config_values->port) {
	case I2C_PORT1:
		if (nack)
			I2C_AcknowledgeConfig(I2C1, DISABLE);
		else
			I2C_AcknowledgeConfig(I2C1, ENABLE);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }
		return I2C_ReceiveData(I2C1);
	case I2C_PORT2:
		if (nack)
			I2C_AcknowledgeConfig(I2C2, DISABLE);
		else
			I2C_AcknowledgeConfig(I2C2, ENABLE);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }

		return I2C_ReceiveData(I2C2);
	case I2C_PORT3:
		if (nack)
			I2C_AcknowledgeConfig(I2C3, DISABLE);
		else
			I2C_AcknowledgeConfig(I2C3, ENABLE);
		while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_RECEIVED))
			{ _delay_us(1); to++; if (to > I2C_TO_US) return -2; }

		return I2C_ReceiveData(I2C3);
	default:
		break;
	}

	return -1;
}
