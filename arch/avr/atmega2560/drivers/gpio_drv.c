#include <hal.h>
#include <device.h>
#include <gpio.h>
#include <gpio_drv.h>


int gpio_ll_setup(struct gpio_config_values_s *cfg)
{
	uint32_t modesel, pullsel;
	
	for (int i = 0; i < 8; i++) {
		if (cfg->pinsel & (1 << i)) {
			modesel = (cfg->mode & (3 << (i << 1))) >> (i << 1);
			pullsel = (cfg->pull & (3 << (i << 1))) >> (i << 1);
			switch (modesel) {
			case GPIO_INPUT:
				switch (cfg->port) {
				case GPIO_PORTA:
					if (pullsel == GPIO_PULLUP) PORTA |= (1 << i);
					DDRA &= ~(1 << i); break;
				case GPIO_PORTB: 
					if (pullsel == GPIO_PULLUP) PORTB |= (1 << i);
					DDRB &= ~(1 << i); break;
				case GPIO_PORTC:
					if (pullsel == GPIO_PULLUP) PORTC |= (1 << i);
					DDRC &= ~(1 << i); break;
				case GPIO_PORTD:
					if (pullsel == GPIO_PULLUP) PORTD |= (1 << i);
					DDRD &= ~(1 << i); break;
				case GPIO_PORTE:
					if (pullsel == GPIO_PULLUP) PORTE |= (1 << i);
					DDRE &= ~(1 << i); break;
				case GPIO_PORTF:
					if (pullsel == GPIO_PULLUP) PORTF |= (1 << i);
					DDRF &= ~(1 << i); break;
				case GPIO_PORTG:
					if (pullsel == GPIO_PULLUP) PORTG |= (1 << i);
					DDRG &= ~(1 << i); break;
				case GPIO_PORTH:
					if (pullsel == GPIO_PULLUP) PORTH |= (1 << i);
					DDRH &= ~(1 << i); break;
				case GPIO_PORTJ:
					if (pullsel == GPIO_PULLUP) PORTJ |= (1 << i);
					DDRJ &= ~(1 << i); break;
				case GPIO_PORTK:
					if (pullsel == GPIO_PULLUP) PORTK |= (1 << i);
					DDRK &= ~(1 << i); break;
				case GPIO_PORTL:
					if (pullsel == GPIO_PULLUP) PORTL |= (1 << i);
					DDRL &= ~(1 << i); break;
				default: break;
				}
				break;
			case GPIO_OUTPUT:
				switch (cfg->port) {
				case GPIO_PORTA: DDRA |= (1 << i); break;
				case GPIO_PORTB: DDRB |= (1 << i); break;
				case GPIO_PORTC: DDRC |= (1 << i); break;
				case GPIO_PORTD: DDRD |= (1 << i); break;
				case GPIO_PORTE: DDRE |= (1 << i); break;
				case GPIO_PORTF: DDRF |= (1 << i); break;
				case GPIO_PORTG: DDRG |= (1 << i); break;
				case GPIO_PORTH: DDRH |= (1 << i); break;
				case GPIO_PORTJ: DDRJ |= (1 << i); break;
				case GPIO_PORTK: DDRK |= (1 << i); break;
				case GPIO_PORTL: DDRL |= (1 << i); break;
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
	case GPIO_PORTA: val = PINA; break;
	case GPIO_PORTB: val = PINB; break;
	case GPIO_PORTC: val = PINC; break;
	case GPIO_PORTD: val = PIND; break;
	case GPIO_PORTE: val = PINE; break;
	case GPIO_PORTF: val = PINF; break;
	case GPIO_PORTG: val = PING; break;
	case GPIO_PORTH: val = PINH; break;
	case GPIO_PORTJ: val = PINJ; break;
	case GPIO_PORTK: val = PINK; break;
	case GPIO_PORTL: val = PINL; break;
	default: break;
	}
	
	return val & cfg->pinsel;
}

int gpio_ll_set(struct gpio_config_values_s *cfg, int val)
{
	uint8_t mask = val & cfg->pinsel;
	
	switch (cfg->port) {
	case GPIO_PORTA: PORTA |= mask; break;
	case GPIO_PORTB: PORTB |= mask; break;
	case GPIO_PORTC: PORTC |= mask; break;
	case GPIO_PORTD: PORTD |= mask; break;
	case GPIO_PORTE: PORTE |= mask; break;
	case GPIO_PORTF: PORTF |= mask; break;
	case GPIO_PORTG: PORTG |= mask; break;
	case GPIO_PORTH: PORTH |= mask; break;
	case GPIO_PORTJ: PORTJ |= mask; break;
	case GPIO_PORTK: PORTK |= mask; break;
	case GPIO_PORTL: PORTL |= mask; break;
	default: break;
	}
	
	return 0;
}

int gpio_ll_clear(struct gpio_config_values_s *cfg, int val)
{
	uint8_t mask = val & cfg->pinsel;
	
	switch (cfg->port) {
	case GPIO_PORTA: PORTA &= ~mask; break;
	case GPIO_PORTB: PORTB &= ~mask; break;
	case GPIO_PORTC: PORTC &= ~mask; break;
	case GPIO_PORTD: PORTD &= ~mask; break;
	case GPIO_PORTE: PORTE &= ~mask; break;
	case GPIO_PORTF: PORTF &= ~mask; break;
	case GPIO_PORTG: PORTG &= ~mask; break;
	case GPIO_PORTH: PORTH &= ~mask; break;
	case GPIO_PORTJ: PORTJ &= ~mask; break;
	case GPIO_PORTK: PORTK &= ~mask; break;
	case GPIO_PORTL: PORTL &= ~mask; break;
	default: break;
	}
	
	return 0;
}

int gpio_ll_toggle(struct gpio_config_values_s *cfg, int val)
{
	uint8_t mask = val & cfg->pinsel;
	
	switch (cfg->port) {
	case GPIO_PORTA: PORTA ^= mask; break;
	case GPIO_PORTB: PORTB ^= mask; break;
	case GPIO_PORTC: PORTC ^= mask; break;
	case GPIO_PORTD: PORTD ^= mask; break;
	case GPIO_PORTE: PORTE ^= mask; break;
	case GPIO_PORTF: PORTF ^= mask; break;
	case GPIO_PORTG: PORTG ^= mask; break;
	case GPIO_PORTH: PORTH ^= mask; break;
	case GPIO_PORTJ: PORTJ ^= mask; break;
	case GPIO_PORTK: PORTK ^= mask; break;
	case GPIO_PORTL: PORTL ^= mask; break;
	default: break;
	}
	
	return 0;
}
