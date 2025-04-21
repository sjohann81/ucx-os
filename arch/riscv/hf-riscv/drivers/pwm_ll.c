#include <hal.h>
#include <device.h>
#include <pwm.h>
#include <pwm_ll.h>
#include <libc.h>


long pwm_ll_setup(struct pwm_config_values_s *cfg)
{
	unsigned prescaler = 0, factor = 1;
	unsigned long overflow;
	unsigned channels;

	if (cfg->frequency == 0)
		return -1;
	
	/* set prescaler and timer overflow */
	while (1) {
		overflow = F_CPU / factor / cfg->frequency;
		if (overflow < 65535) break;
		prescaler++;
		factor <<= 2;
	}
		
	if (cfg->direction == PWM_OUTPUT) {
		/* no fancy modes for PWM in hardware */
		switch (cfg->mode) {
		case PWM_FAST:
		case PWM_EDGE:
			break;
		default:
			return -1;
		}
		
		/* channels 0 ~ 3 are default, 8 ~ 11 are pin remaps */
		channels = PWM_CHANNEL0 | PWM_CHANNEL1 | PWM_CHANNEL2 | PWM_CHANNEL3 |
			PWM_CHANNEL8 | PWM_CHANNEL9 | PWM_CHANNEL10 | PWM_CHANNEL11;
		
		if (!(cfg->channels & channels))
			return -1;

		switch (cfg->timer) {
		case PWM_TIMER1:
			TIMER1PRE = prescaler;
			TIMER1CTC = overflow;

			/* default channel configuration */
			if (cfg->channels & PWM_CHANNEL0) {
				TIMER1OCR = 0;
				PAALTCFG0 |= MASK_TIM1_CH0;
				PADDR |= MASK_P0;
			}

			if (cfg->channels & PWM_CHANNEL1) {
				TIMER1OCR1 = 0;
				PAALTCFG0 |= MASK_TIM1_CH1;
				PADDR |= MASK_P1;
			}
			
			if (cfg->channels & PWM_CHANNEL2) {
				TIMER1OCR2 = 0;
				PAALTCFG0 |= MASK_TIM1_CH2;
				PADDR |= MASK_P4;
			}
			
			if (cfg->channels & PWM_CHANNEL3) {
				TIMER1OCR3 = 0;
				PAALTCFG0 |= MASK_TIM1_CH3;
				PADDR |= MASK_P5;
			}

			/* pin remap (high port part) channel configuration */
			if (cfg->channels & PWM_CHANNEL8) {
				TIMER1OCR = 0;
				PAALTCFG0 |= MASK_TIM1H_CH0;
				PADDR |= MASK_P8;
			}

			if (cfg->channels & PWM_CHANNEL9) {
				TIMER1OCR1 = 0;
				PAALTCFG0 |= MASK_TIM1H_CH1;
				PADDR |= MASK_P9;
			}
			
			if (cfg->channels & PWM_CHANNEL10) {
				TIMER1OCR2 = 0;
				PAALTCFG0 |= MASK_TIM1H_CH2;
				PADDR |= MASK_P12;
			}
			
			if (cfg->channels & PWM_CHANNEL11) {
				TIMER1OCR3 = 0;
				PAALTCFG0 |= MASK_TIM1H_CH3;
				PADDR |= MASK_P13;
			}
			
			break;
		case PWM_TIMER2:
			TIMER2PRE = prescaler;
			TIMER2CTC = overflow;

			/* default channel configuration */
			if (cfg->channels & PWM_CHANNEL0) {
				TIMER2OCR = 0;
				PAALTCFG0 |= MASK_TIM2_CH0;
				PADDR |= MASK_P0;
			}

			if (cfg->channels & PWM_CHANNEL1) {
				TIMER2OCR1 = 0;
				PAALTCFG0 |= MASK_TIM2_CH1;
				PADDR |= MASK_P1;
			}
			
			if (cfg->channels & PWM_CHANNEL2) {
				TIMER2OCR2 = 0;
				PAALTCFG0 |= MASK_TIM2_CH2;
				PADDR |= MASK_P6;
			}
			
			if (cfg->channels & PWM_CHANNEL3) {
				TIMER2OCR3 = 0;
				PAALTCFG0 |= MASK_TIM2_CH3;
				PADDR |= MASK_P7;
			}

			/* pin remap (high port part) channel configuration */
			if (cfg->channels & PWM_CHANNEL8) {
				TIMER2OCR = 0;
				PAALTCFG0 |= MASK_TIM2H_CH0;
				PADDR |= MASK_P8;
			}

			if (cfg->channels & PWM_CHANNEL9) {
				TIMER2OCR1 = 0;
				PAALTCFG0 |= MASK_TIM2H_CH1;
				PADDR |= MASK_P9;
			}
			
			if (cfg->channels & PWM_CHANNEL10) {
				TIMER2OCR2 = 0;
				PAALTCFG0 |= MASK_TIM2H_CH2;
				PADDR |= MASK_P14;
			}
			
			if (cfg->channels & PWM_CHANNEL11) {
				TIMER2OCR3 = 0;
				PAALTCFG0 |= MASK_TIM2H_CH3;
				PADDR |= MASK_P15;
			}
			
			break;
		default:
			return -1;
		}
	} else {
		return -1;
	}
	
	return overflow;
}

int pwm_ll_get(struct pwm_config_values_s *cfg, unsigned channel, unsigned *pulse)
{
	return -1;
}

int pwm_ll_set(struct pwm_config_values_s *cfg, unsigned channel, unsigned pulse)
{
	switch (cfg->timer) {
	case PWM_TIMER1:
		switch (channel) {
		case PWM_CHANNEL0: TIMER1OCR = pulse; break;
		case PWM_CHANNEL1: TIMER1OCR1 = pulse; break;
		case PWM_CHANNEL2: TIMER1OCR2 = pulse; break;
		case PWM_CHANNEL3: TIMER1OCR3 = pulse; break;
		case PWM_CHANNEL8: TIMER1OCR = pulse; break;
		case PWM_CHANNEL9: TIMER1OCR1 = pulse; break;
		case PWM_CHANNEL10: TIMER1OCR2 = pulse; break;
		case PWM_CHANNEL11: TIMER1OCR3 = pulse; break;
		default:
			return -1;
		}
		break;
	case PWM_TIMER2:
		switch (channel) {
		case PWM_CHANNEL0: TIMER2OCR = pulse; break;
		case PWM_CHANNEL1: TIMER2OCR1 = pulse; break;
		case PWM_CHANNEL2: TIMER2OCR2 = pulse; break;
		case PWM_CHANNEL3: TIMER2OCR3 = pulse; break;
		case PWM_CHANNEL8: TIMER2OCR = pulse; break;
		case PWM_CHANNEL9: TIMER2OCR1 = pulse; break;
		case PWM_CHANNEL10: TIMER2OCR2 = pulse; break;
		case PWM_CHANNEL11: TIMER2OCR3 = pulse; break;
		default:
			return -1;
		}
		break;

	default:
		return -1;
	}
	
	return 0;
}
