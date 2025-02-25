#include <hal.h>
#include <device.h>
#include <gpio.h>
#include <gpio_drv.h>


int gpio_ll_setup(struct gpio_config_values_s *cfg)
{
	uint32_t pin_opt;
	
	for (int i = 0; i < 8; i++) {
		if (cfg->pinsel & (1 << i)) {
			pin_opt = (3 << (i << 1)) >> (i << 1);
			switch (cfg->mode & pin_opt) {
			case GPIO_INPUT:
				switch (cfg->port) {
				case GPIO_PORTA: return -1;
				case GPIO_PORTB: 
					if (cfg->pull == GPIO_PULLUP)
						PORTB |= (1 << i);
					DDRB &= ~(1 << i); break;
				case GPIO_PORTC:
					if (cfg->pull == GPIO_PULLUP)
						PORTC |= (1 << i);
					DDRC &= ~(1 << i); break;
				case GPIO_PORTD:
					if (cfg->pull == GPIO_PULLUP)
						PORTD |= (1 << i);
					DDRD &= ~(1 << i); break;
				default: break;
				}
				break;
			case GPIO_OUTPUT:
				switch (cfg->port) {
				case GPIO_PORTA: return -1;
				case GPIO_PORTB: DDRB |= (1 << i); break;
				case GPIO_PORTC: DDRC |= (1 << i); break;
				case GPIO_PORTD: DDRD |= (1 << i); break;
				default: break;
				}
				break;
			default: break;
			}
		}
	}

	return 0;
}

int gpio_ll_get(struct gpio_config_values_s *cfg)
{
	uint8_t val = 0;
	
	switch (cfg->port) {
	case GPIO_PORTA: return -1;
	case GPIO_PORTB: val = PINB; break;
	case GPIO_PORTC: val = PINC; break;
	case GPIO_PORTD: val = PIND; break;
	default: break;
	}
	
	return val;
}

int gpio_ll_set(struct gpio_config_values_s *cfg, int val)
{
	switch (cfg->port) {
	case GPIO_PORTA: return -1;
	case GPIO_PORTB: PORTB = val & 0xff; break;
	case GPIO_PORTC: PORTC = val & 0xff; break;
	case GPIO_PORTD: PORTD = val & 0xff; break;
	default: break;
	}
	
	return 0;
}
