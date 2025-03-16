#include <hal.h>
#include <device.h>
#include <gpio.h>
#include <gpio_ll.h>


static uint8_t od_outputs[4];

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
				case GPIO_PORTB:
					od_outputs[1] &= ~(1 << i);
					if (pullsel == GPIO_PULLUP) PORTB |= (1 << i);
					DDRB &= ~(1 << i); break;
				case GPIO_PORTC:
					od_outputs[2] &= ~(1 << i);
					if (pullsel == GPIO_PULLUP) PORTC |= (1 << i);
					DDRC &= ~(1 << i); break;
				case GPIO_PORTD:
					od_outputs[3] &= ~(1 << i);
					if (pullsel == GPIO_PULLUP) PORTD |= (1 << i);
					DDRD &= ~(1 << i); break;
				default:
					return -1;
				}
				break;
			case GPIO_OUTPUT:
				switch (cfg->port) {
				case GPIO_PORTB: od_outputs[1] &= ~(1 << i); DDRB |= (1 << i); break;
				case GPIO_PORTC: od_outputs[2] &= ~(1 << i); DDRC |= (1 << i); break;
				case GPIO_PORTD: od_outputs[3] &= ~(1 << i); DDRD |= (1 << i); break;
				default:
					return -1;
				}
				break;
			case GPIO_OUTPUT_OD:
				switch (cfg->port) {
				case GPIO_PORTB: od_outputs[1] |= (1 << i); PORTB &= ~(1 << i); DDRB &= ~(1 << i); break;
				case GPIO_PORTC: od_outputs[2] |= (1 << i); PORTC &= ~(1 << i); DDRC &= ~(1 << i); break;
				case GPIO_PORTD: od_outputs[3] |= (1 << i); PORTD &= ~(1 << i); DDRD &= ~(1 << i); break;
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
	uint8_t val = 0;
	
	switch (cfg->port) {
	case GPIO_PORTB: val = PINB; break;
	case GPIO_PORTC: val = PINC; break;
	case GPIO_PORTD: val = PIND; break;
	default:
		return -1;
	}
	
	return val & cfg->pinsel;
}


static struct port_data_s {
	volatile uint8_t *dir;
	volatile uint8_t *port;
	volatile uint8_t *pin;
} port_data[4] = {
	{0, 0},
	{&DDRB, &PORTB, &PINB},
	{&DDRC, &PORTC, &PINC},
	{&DDRD, &PORTD, &PIND}
};

int gpio_ll_set(struct gpio_config_values_s *cfg, int val)
{
	uint8_t mask = val & cfg->pinsel;

	if (cfg->port < GPIO_PORTB || cfg->port > GPIO_PORTD)
		return -1;
	
	if (od_outputs[cfg->port]) {
		for (int i = 0; i < 8; i++) {
			if ((1 << i) & mask) {
				if (od_outputs[cfg->port] & (1 << i))
					*port_data[cfg->port].dir &= ~(1 << i);
				else
					*port_data[cfg->port].port |= (1 << i);
			}
		}
	} else {
		*port_data[cfg->port].port |= mask;
	}

	return 0;
}

int gpio_ll_clear(struct gpio_config_values_s *cfg, int val)
{
	uint8_t mask = val & cfg->pinsel;

	if (cfg->port < GPIO_PORTB || cfg->port > GPIO_PORTD)
		return -1;
	
	if (od_outputs[cfg->port]) {
		for (int i = 0; i < 8; i++) {
			if ((1 << i) & mask) {
				if (od_outputs[cfg->port] & (1 << i))
					*port_data[cfg->port].dir |= (1 << i);
				else
					*port_data[cfg->port].port &= ~(1 << i);
					
			}
		}
	} else {
		*port_data[cfg->port].port &= ~mask;
	}

	return 0;
}

int gpio_ll_toggle(struct gpio_config_values_s *cfg, int val)
{
	uint8_t mask = val & cfg->pinsel;

	if (cfg->port < GPIO_PORTB || cfg->port > GPIO_PORTD)
		return -1;
	
	if (od_outputs[cfg->port]) {
		for (int i = 0; i < 8; i++) {
			if ((1 << i) & mask) {
				if (od_outputs[cfg->port] & (1 << i))
					if (*port_data[cfg->port].pin & (1 << i))
						*port_data[cfg->port].dir |= (1 << i);
					else *port_data[cfg->port].dir &= ~(1 << i);
				else
					*port_data[cfg->port].port ^= (1 << i);
			}
		}
	} else {
		*port_data[cfg->port].port ^= mask;
	}

	return 0;
}


struct int_source_s {
	uint8_t trigger;
	void (*int_cb)(void);
};

static struct int_source_s int_sources[MAX_INT_SOURCES] = { 0 };

static volatile uint8_t portb_status = 0;
static volatile uint8_t portc_status = 0;
static volatile uint8_t portd_status = 0;

ISR(PCINT0_vect)
{
	uint8_t portb_change;
	
	portb_change = PINB ^ portb_status;
	portb_status = PINB;
	
	for (int i = 0; i < 8; i++) {
		if (portb_change & (1 << i)) {
			if (int_sources[i].trigger == GPIO_CHANGE) {
				(int_sources[i].int_cb)();
				continue;
			}
				
			if (portb_status & (1 << i)) {
				if (int_sources[i].trigger == GPIO_RISING)
					(int_sources[i].int_cb)();
			} else {
				if (int_sources[i].trigger == GPIO_FALLING)
					(int_sources[i].int_cb)();
			}
		}
	}
}

ISR(PCINT1_vect)
{
	uint8_t portc_change;
	
	portc_change = PINC ^ portc_status;
	portc_status = PINC;
	
	for (int i = 0; i < 8; i++) {
		if (portc_change & (1 << i)) {
			if (int_sources[i + 8].trigger == GPIO_CHANGE) {
				(int_sources[i + 8].int_cb)();
				continue;
			}
				
			if (portc_status & (1 << i)) {
				if (int_sources[i + 8].trigger == GPIO_RISING)
					(int_sources[i + 8].int_cb)();
			} else {
				if (int_sources[i + 8].trigger == GPIO_FALLING)
					(int_sources[i + 8].int_cb)();
			}
		}
	}
}

ISR(PCINT2_vect)
{
	uint8_t portd_change;
	
	portd_change = PIND ^ portd_status;
	portd_status = PIND;
	
	for (int i = 0; i < 8; i++) {
		if (portd_change & (1 << i)) {
			if (int_sources[i + 16].trigger == GPIO_CHANGE) {
				(int_sources[i + 16].int_cb)();
				continue;
			}
				
			if (portd_status & (1 << i)) {
				if (int_sources[i + 16].trigger == GPIO_RISING)
					(int_sources[i + 16].int_cb)();
			} else {
				if (int_sources[i + 16].trigger == GPIO_FALLING)
					(int_sources[i + 16].int_cb)();
			}
		}
	}
}

int gpio_ll_int_attach(struct gpio_config_values_s *cfg, int pin, void (*callback)(), int trigger)
{
	switch (cfg->port) {
	case GPIO_PORTB:
		portb_status = PINB;
		PCICR |= (1 << PCIE0);		// enable interrupt vector for PB7 .. PB0
		switch (pin) {
		case GPIO_PIN0: PCMSK0 |= (1 << PCINT0); int_sources[0].trigger = trigger; int_sources[0].int_cb = callback; break;
		case GPIO_PIN1: PCMSK0 |= (1 << PCINT1); int_sources[1].trigger = trigger; int_sources[1].int_cb = callback; break;
		case GPIO_PIN2: PCMSK0 |= (1 << PCINT2); int_sources[2].trigger = trigger; int_sources[2].int_cb = callback; break;
		case GPIO_PIN3: PCMSK0 |= (1 << PCINT3); int_sources[3].trigger = trigger; int_sources[3].int_cb = callback; break;
		case GPIO_PIN4: PCMSK0 |= (1 << PCINT4); int_sources[4].trigger = trigger; int_sources[4].int_cb = callback; break;
		case GPIO_PIN5: PCMSK0 |= (1 << PCINT5); int_sources[5].trigger = trigger; int_sources[5].int_cb = callback; break;
		case GPIO_PIN6: PCMSK0 |= (1 << PCINT6); int_sources[6].trigger = trigger; int_sources[6].int_cb = callback; break;
		case GPIO_PIN7: PCMSK0 |= (1 << PCINT7); int_sources[7].trigger = trigger; int_sources[7].int_cb = callback; break;
		default:
			return -1;
		}
		break;
	case GPIO_PORTC:
		portc_status = PINC;
		PCICR |= (1 << PCIE1);		// enable interrupt vector for PC6 .. PC0
		switch (pin) {
		case GPIO_PIN0: PCMSK1 |= (1 << PCINT8); int_sources[8].trigger = trigger; int_sources[8].int_cb = callback; break;
		case GPIO_PIN1: PCMSK1 |= (1 << PCINT9); int_sources[9].trigger = trigger; int_sources[9].int_cb = callback; break;
		case GPIO_PIN2: PCMSK1 |= (1 << PCINT10); int_sources[10].trigger = trigger; int_sources[10].int_cb = callback; break;
		case GPIO_PIN3: PCMSK1 |= (1 << PCINT11); int_sources[11].trigger = trigger; int_sources[11].int_cb = callback; break;
		case GPIO_PIN4: PCMSK1 |= (1 << PCINT12); int_sources[12].trigger = trigger; int_sources[12].int_cb = callback; break;
		case GPIO_PIN5: PCMSK1 |= (1 << PCINT13); int_sources[13].trigger = trigger; int_sources[13].int_cb = callback; break;
		case GPIO_PIN6: PCMSK1 |= (1 << PCINT14); int_sources[14].trigger = trigger; int_sources[14].int_cb = callback; break;
		default:
			return -1;
		}
		break;
	case GPIO_PORTD:
		portd_status = PIND;
		PCICR |= (1 << PCIE2);		// enable interrupt vector for PD7 .. PD0
		switch (pin) {
		case GPIO_PIN0: PCMSK2 |= (1 << PCINT16); int_sources[16].trigger = trigger; int_sources[16].int_cb = callback; break;
		case GPIO_PIN1: PCMSK2 |= (1 << PCINT17); int_sources[17].trigger = trigger; int_sources[17].int_cb = callback; break;
		case GPIO_PIN2: PCMSK2 |= (1 << PCINT18); int_sources[18].trigger = trigger; int_sources[18].int_cb = callback; break;
		case GPIO_PIN3: PCMSK2 |= (1 << PCINT19); int_sources[19].trigger = trigger; int_sources[19].int_cb = callback; break;
		case GPIO_PIN4: PCMSK2 |= (1 << PCINT20); int_sources[20].trigger = trigger; int_sources[20].int_cb = callback; break;
		case GPIO_PIN5: PCMSK2 |= (1 << PCINT21); int_sources[21].trigger = trigger; int_sources[21].int_cb = callback; break;
		case GPIO_PIN6: PCMSK2 |= (1 << PCINT22); int_sources[22].trigger = trigger; int_sources[22].int_cb = callback; break;
		case GPIO_PIN7: PCMSK2 |= (1 << PCINT23); int_sources[23].trigger = trigger; int_sources[23].int_cb = callback; break;
		default:
			return -1;
		}
		break;
	default:
		return -1;
	}
	
	return 0;
}
