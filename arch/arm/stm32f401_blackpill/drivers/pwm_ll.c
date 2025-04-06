#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <hal.h>
#include <device.h>
#include <pwm.h>
#include <pwm_ll.h>
#include <libc.h>


#define SCALE1			1000000
#define SCALE2			2000000
#define SCALE4			4000000
#define SCALE8			8000000
#define SCALE16			16000000
#define SCALE32			32000000

long pwm_ll_setup(struct pwm_config_values_s *cfg)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	unsigned prescaler, overflow;
	unsigned channels;

	if (cfg->frequency == 0)
		return -1;

	if (cfg->frequency < 1000) {
		prescaler = (SystemCoreClock / SCALE1) - 1;
		overflow = (SCALE1 / cfg->frequency) - 1;
	} else if (cfg->frequency < 2000) {
		prescaler = (SystemCoreClock / SCALE2) - 1;
		overflow = (SCALE2 / cfg->frequency) - 1;
	} else if (cfg->frequency < 4000) {
		prescaler = (SystemCoreClock / SCALE4) - 1;
		overflow = (SCALE4 / cfg->frequency) - 1;
	} else if (cfg->frequency < 8000) {
		prescaler = (SystemCoreClock / SCALE8) - 1;
		overflow = (SCALE8 / cfg->frequency) - 1;
	} else if (cfg->frequency < 16000) {
		prescaler = (SystemCoreClock / SCALE16) - 1;
		overflow = (SCALE16 / cfg->frequency) - 1;
	} else {
		prescaler = (SystemCoreClock / SCALE32) - 1;
		overflow = (SCALE32 / cfg->frequency) - 1;
	}
	
	if (cfg->direction == PWM_OUTPUT) {
		switch (cfg->timer) {
		case PWM_TIMER2:
			/* Enable clock for TIM2 */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			
			TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;
			TIM_TimeBaseInitStruct.TIM_Period = overflow;
			TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
			
			switch (cfg->mode) {
			case PWM_FAST:
			case PWM_EDGE:
				TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
				break;
			case PWM_PHASE_CORRECT:
			case PWM_CENTER:
				TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned2;
				break;
			default:
				return -1;
			}
			
			/* TIM2 initialize */
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
			/* Enable TIM2 interrupt */
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
			/* Start TIM2 */
			TIM_Cmd(TIM2, ENABLE);
			
			/* set OC mode */
			TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
			TIM_OCStruct.TIM_Pulse = 0;
			
			channels = PWM_CHANNEL1 | PWM_CHANNEL2 | PWM_CHANNEL3 | PWM_CHANNEL4;
			
			if (!(cfg->channels & channels))
				return -1;

			/* Clock for GPIOA */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			GPIO_InitStruct.GPIO_Pin = 0;
				
			if (cfg->channels & PWM_CHANNEL1) {
				if (cfg->inv_polarity & PWM_CHANNEL1)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM2 CH1, 0% duty cycle */
				TIM_OC1Init(TIM2, &TIM_OCStruct);
				TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_0;
			}
			
			if (cfg->channels & PWM_CHANNEL2) {
				if (cfg->inv_polarity & PWM_CHANNEL2)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM2 CH2, 0% duty cycle */
				TIM_OC2Init(TIM2, &TIM_OCStruct);
				TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_1;
			}
			
			if (cfg->channels & PWM_CHANNEL3) {
				if (cfg->inv_polarity & PWM_CHANNEL3)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
				
				/* set TIM2 CH3, 0% duty cycle */
				TIM_OC3Init(TIM2, &TIM_OCStruct);
				TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_2;
			}
			
			if (cfg->channels & PWM_CHANNEL4) {
				if (cfg->inv_polarity & PWM_CHANNEL4)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM2 CH4, 0% duty cycle */
				TIM_OC4Init(TIM2, &TIM_OCStruct);
				TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_3;
			}

			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
			GPIO_Init(GPIOA, &GPIO_InitStruct);

			break;
		case PWM_TIMER3:
			/* Enable clock for TIM3 */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			
			TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;
			TIM_TimeBaseInitStruct.TIM_Period = overflow;
			TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;

			switch (cfg->mode) {
			case PWM_FAST:
			case PWM_EDGE:
				TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
				break;
			case PWM_PHASE_CORRECT:
			case PWM_CENTER:
				TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned2;
				break;
			default:
				return -1;
			}
			
			/* TIM3 initialize */
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
			/* Enable TIM3 interrupt */
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			/* Start TIM3 */
			TIM_Cmd(TIM3, ENABLE);
			
			/* set OC mode */
			TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
			TIM_OCStruct.TIM_Pulse = 0;
			
			channels = PWM_CHANNEL1 | PWM_CHANNEL2 | PWM_CHANNEL3 | PWM_CHANNEL4;
			
			if (!(cfg->channels & channels))
				return -1;

			/* Clock for GPIOB */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			GPIO_InitStruct.GPIO_Pin = 0;
				
			if (cfg->channels & PWM_CHANNEL1) {
				if (cfg->inv_polarity & PWM_CHANNEL1)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM3 CH1, 0% duty cycle */
				TIM_OC1Init(TIM3, &TIM_OCStruct);
				TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_4;
			}
			
			if (cfg->channels & PWM_CHANNEL2) {
				if (cfg->inv_polarity & PWM_CHANNEL2)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM3 CH2, 0% duty cycle */
				TIM_OC2Init(TIM3, &TIM_OCStruct);
				TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_5;
			}
			
			if (cfg->channels & PWM_CHANNEL3) {
				if (cfg->inv_polarity & PWM_CHANNEL3)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
				
				/* set TIM3 CH3, 0% duty cycle */
				TIM_OC3Init(TIM3, &TIM_OCStruct);
				TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_0;
			}
			
			if (cfg->channels & PWM_CHANNEL4) {
				if (cfg->inv_polarity & PWM_CHANNEL4)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM3 CH4, 0% duty cycle */
				TIM_OC4Init(TIM3, &TIM_OCStruct);
				TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_1;
			}

			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
			GPIO_Init(GPIOB, &GPIO_InitStruct);

			break;
		case PWM_TIMER4:
			/* Enable clock for TIM4 */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			
			TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;
			TIM_TimeBaseInitStruct.TIM_Period = overflow;
			TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;

			switch (cfg->mode) {
			case PWM_FAST:
			case PWM_EDGE:
				TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
				break;
			case PWM_PHASE_CORRECT:
			case PWM_CENTER:
				TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned2;
				break;
			default:
				return -1;
			}
			
			/* TIM4 initialize */
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
			/* Enable TIM4 interrupt */
			TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
			/* Start TIM4 */
			TIM_Cmd(TIM4, ENABLE);
			
			/* set OC mode */
			TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
			TIM_OCStruct.TIM_Pulse = 0;
			
			channels = PWM_CHANNEL1 | PWM_CHANNEL2 | PWM_CHANNEL3 | PWM_CHANNEL4;
			
			if (!(cfg->channels & channels))
				return -1;

			/* Clock for GPIOB */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			GPIO_InitStruct.GPIO_Pin = 0;
				
			if (cfg->channels & PWM_CHANNEL1) {
				if (cfg->inv_polarity & PWM_CHANNEL1)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM4 CH1, 0% duty cycle */
				TIM_OC1Init(TIM4, &TIM_OCStruct);
				TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_6;
			}
			
			if (cfg->channels & PWM_CHANNEL2) {
				if (cfg->inv_polarity & PWM_CHANNEL2)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM4 CH2, 0% duty cycle */
				TIM_OC2Init(TIM4, &TIM_OCStruct);
				TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_7;
			}
			
			if (cfg->channels & PWM_CHANNEL3) {
				if (cfg->inv_polarity & PWM_CHANNEL3)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
				
				/* set TIM4 CH3, 0% duty cycle */
				TIM_OC3Init(TIM4, &TIM_OCStruct);
				TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_8;
			}
			
			if (cfg->channels & PWM_CHANNEL4) {
				if (cfg->inv_polarity & PWM_CHANNEL4)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM4 CH4, 0% duty cycle */
				TIM_OC4Init(TIM4, &TIM_OCStruct);
				TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_9;
			}

			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
			GPIO_Init(GPIOB, &GPIO_InitStruct);

			break;
		case PWM_TIMER9:
			/* Enable clock for TIM9 */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			
			TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;
			TIM_TimeBaseInitStruct.TIM_Period = overflow;
			TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;

			switch (cfg->mode) {
			case PWM_FAST:
			case PWM_EDGE:
				TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
				break;
			case PWM_PHASE_CORRECT:
			case PWM_CENTER:
				TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned2;
				break;
			default:
				return -1;
			}
			
			/* TIM9 initialize */
			TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStruct);
			/* Enable TIM9 interrupt */
			TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
			/* Start TIM9 */
			TIM_Cmd(TIM9, ENABLE);
			
			/* set OC mode */
			TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
			TIM_OCStruct.TIM_Pulse = 0;
			
			channels = PWM_CHANNEL1 | PWM_CHANNEL2;
			
			if (!(cfg->channels & channels))
				return -1;

			/* Clock for GPIOA */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			GPIO_InitStruct.GPIO_Pin = 0;
				
			if (cfg->channels & PWM_CHANNEL1) {
				if (cfg->inv_polarity & PWM_CHANNEL1)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM9 CH1, 0% duty cycle */
				TIM_OC1Init(TIM9, &TIM_OCStruct);
				TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_2;
			}
			
			if (cfg->channels & PWM_CHANNEL2) {
				if (cfg->inv_polarity & PWM_CHANNEL2)
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
				else
					TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
					
				/* set TIM9 CH2, 0% duty cycle */
				TIM_OC2Init(TIM9, &TIM_OCStruct);
				TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);
				
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);
				GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_3;
			}

			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
			GPIO_Init(GPIOA, &GPIO_InitStruct);

			break;
		default:
			return -1;
		}
	} else {
		return -1;
	}
	
	return overflow + 1;
}

int pwm_ll_get(struct pwm_config_values_s *cfg, unsigned channel, unsigned *pulse)
{
	return -1;
}

int pwm_ll_set(struct pwm_config_values_s *cfg, unsigned channel, unsigned pulse)
{
	switch (cfg->timer) {
	case PWM_TIMER2:
		switch (channel) {
		case PWM_CHANNEL1: TIM2->CCR1 = pulse; break;
		case PWM_CHANNEL2: TIM2->CCR2 = pulse; break;
		case PWM_CHANNEL3: TIM2->CCR3 = pulse; break;
		case PWM_CHANNEL4: TIM2->CCR4 = pulse; break;
		default:
			return -1;
		}
		break;
	case PWM_TIMER3:
		switch (channel) {
		case PWM_CHANNEL1: TIM3->CCR1 = pulse; break;
		case PWM_CHANNEL2: TIM3->CCR2 = pulse; break;
		case PWM_CHANNEL3: TIM3->CCR3 = pulse; break;
		case PWM_CHANNEL4: TIM3->CCR4 = pulse; break;
		default:
			return -1;
		}
		break;
	case PWM_TIMER4:
		switch (channel) {
		case PWM_CHANNEL1: TIM4->CCR1 = pulse; break;
		case PWM_CHANNEL2: TIM4->CCR2 = pulse; break;
		case PWM_CHANNEL3: TIM4->CCR3 = pulse; break;
		case PWM_CHANNEL4: TIM4->CCR4 = pulse; break;
		default:
			return -1;
		}
		break;
	case PWM_TIMER9:
		switch (channel) {
		case PWM_CHANNEL1: TIM9->CCR1 = pulse; break;
		case PWM_CHANNEL2: TIM9->CCR2 = pulse; break;
		default:
			return -1;
		}
		break;
	default:
		return -1;
	}
	
	return 0;
}
