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

struct int_source_s {
	uint8_t trigger;
	void (*int_cb)(void);
};

static struct int_source_s int_sources[MAX_INT_SOURCES] = { 0 };

static volatile uint8_t portb_status = 0;
static volatile uint8_t portj_status = 0;
static volatile uint8_t portk_status = 0;

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
	uint8_t portj_change;
	
	portj_change = PINJ ^ portj_status;
	portj_status = PINJ;
	
	for (int i = 0; i < 7; i++) {
		if (portj_change & (1 << i)) {
			if (int_sources[i + 9].trigger == GPIO_CHANGE) {
				(int_sources[i + 9].int_cb)();
				continue;
			}
				
			if (portj_status & (1 << i)) {
				if (int_sources[i + 9].trigger == GPIO_RISING)
					(int_sources[i + 9].int_cb)();
			} else {
				if (int_sources[i + 9].trigger == GPIO_FALLING)
					(int_sources[i + 9].int_cb)();
			}
		}
	}
}

ISR(PCINT2_vect)
{
	uint8_t portk_change;
	
	portk_change = PINK ^ portk_status;
	portk_status = PINK;
	
	for (int i = 0; i < 8; i++) {
		if (portk_change & (1 << i)) {
			if (int_sources[i + 16].trigger == GPIO_CHANGE) {
				(int_sources[i + 16].int_cb)();
				continue;
			}
				
			if (portk_status & (1 << i)) {
				if (int_sources[i + 16].trigger == GPIO_RISING)
					(int_sources[i + 16].int_cb)();
			} else {
				if (int_sources[i + 16].trigger == GPIO_FALLING)
					(int_sources[i + 16].int_cb)();
			}
		}
	}
}

ISR(INT0_vect)
{
	(int_sources[24].int_cb)();
}

ISR(INT1_vect)
{
	(int_sources[25].int_cb)();
}

ISR(INT2_vect)
{
	(int_sources[26].int_cb)();
}

ISR(INT3_vect)
{
	(int_sources[27].int_cb)();
}

ISR(INT4_vect)
{
	(int_sources[28].int_cb)();
}

ISR(INT5_vect)
{
	(int_sources[29].int_cb)();
}

ISR(INT6_vect)
{
	(int_sources[30].int_cb)();
}

ISR(INT7_vect)
{
	(int_sources[31].int_cb)();
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
	case GPIO_PORTD:
		switch (pin) {
		case GPIO_PIN0:
			switch (trigger) {
			case GPIO_LOW:		break;
			case GPIO_RISING:	EICRA |= (1 << ISC01) | (1 << ISC00); break;
			case GPIO_FALLING:	EICRA |= (1 << ISC01); break;
			case GPIO_CHANGE:	EICRA |= (1 << ISC00); break;
			default:
				return -1;
			}
			EIMSK |= (1 << INT0);
			int_sources[24].int_cb = callback;
			break;
		case GPIO_PIN1:
			switch (trigger) {
			case GPIO_LOW:		break;
			case GPIO_RISING:	EICRA |= (1 << ISC11) | (1 << ISC10); break;
			case GPIO_FALLING:	EICRA |= (1 << ISC11); break;
			case GPIO_CHANGE:	EICRA |= (1 << ISC10); break;
			default:
				return -1;
			}
			EIMSK |= (1 << INT1);
			int_sources[25].int_cb = callback;
			break;
		case GPIO_PIN2:
			switch (trigger) {
			case GPIO_LOW:		break;
			case GPIO_RISING:	EICRA |= (1 << ISC21) | (1 << ISC20); break;
			case GPIO_FALLING:	EICRA |= (1 << ISC21); break;
			case GPIO_CHANGE:	EICRA |= (1 << ISC20); break;
			default:
				return -1;
			}
			EIMSK |= (1 << INT2);
			int_sources[26].int_cb = callback;
			break;
		case GPIO_PIN3:
			switch (trigger) {
			case GPIO_LOW:		break;
			case GPIO_RISING:	EICRA |= (1 << ISC31) | (1 << ISC30); break;
			case GPIO_FALLING:	EICRA |= (1 << ISC31); break;
			case GPIO_CHANGE:	EICRA |= (1 << ISC30); break;
			default:
				return -1;
			}
			EIMSK |= (1 << INT3);
			int_sources[27].int_cb = callback;
			break;
		default:
			return -1;
		}
		break;
	case GPIO_PORTE:
		switch (pin) {
		case GPIO_PIN4:
			switch (trigger) {
			case GPIO_LOW:		break;
			case GPIO_RISING:	EICRB |= (1 << ISC41) | (1 << ISC40); break;
			case GPIO_FALLING:	EICRB |= (1 << ISC41); break;
			case GPIO_CHANGE:	EICRB |= (1 << ISC40); break;
			default:
				return -1;
			}
			EIMSK |= (1 << INT4);
			int_sources[28].int_cb = callback;
			break;
		case GPIO_PIN5:
			switch (trigger) {
			case GPIO_LOW:		break;
			case GPIO_RISING:	EICRB |= (1 << ISC51) | (1 << ISC50); break;
			case GPIO_FALLING:	EICRB |= (1 << ISC51); break;
			case GPIO_CHANGE:	EICRB |= (1 << ISC50); break;
			default:
				return -1;
			}
			EIMSK |= (1 << INT5);
			int_sources[29].int_cb = callback;
			break;
		case GPIO_PIN6:
			switch (trigger) {
			case GPIO_LOW:		break;
			case GPIO_RISING:	EICRB |= (1 << ISC61) | (1 << ISC60); break;
			case GPIO_FALLING:	EICRB |= (1 << ISC61); break;
			case GPIO_CHANGE:	EICRB |= (1 << ISC60); break;
			default:
				return -1;
			}
			EIMSK |= (1 << INT6);
			int_sources[30].int_cb = callback;
			break;
		case GPIO_PIN7:
			switch (trigger) {
			case GPIO_LOW:		break;
			case GPIO_RISING:	EICRB |= (1 << ISC71) | (1 << ISC70); break;
			case GPIO_FALLING:	EICRB |= (1 << ISC71); break;
			case GPIO_CHANGE:	EICRB |= (1 << ISC70); break;
			default:
				return -1;
			}
			EIMSK |= (1 << INT7);
			int_sources[31].int_cb = callback;
			break;
		default:
			return -1;
		}
		break;
	case GPIO_PORTJ:			// ??? not working right on simulide (only PJ6 works?!)
		portj_status = PINJ;
		PCICR |= (1 << PCIE1);		// enable interrupt vector for PJ6 .. PJ0
		switch (pin) {
		case GPIO_PIN0: PCMSK1 |= (1 << PCINT9); int_sources[9].trigger = trigger; int_sources[9].int_cb = callback; break;
		case GPIO_PIN1: PCMSK1 |= (1 << PCINT10); int_sources[10].trigger = trigger; int_sources[10].int_cb = callback; break;
		case GPIO_PIN2: PCMSK1 |= (1 << PCINT11); int_sources[11].trigger = trigger; int_sources[11].int_cb = callback; break;
		case GPIO_PIN3: PCMSK1 |= (1 << PCINT12); int_sources[12].trigger = trigger; int_sources[12].int_cb = callback; break;
		case GPIO_PIN4: PCMSK1 |= (1 << PCINT13); int_sources[13].trigger = trigger; int_sources[13].int_cb = callback; break;
		case GPIO_PIN5: PCMSK1 |= (1 << PCINT14); int_sources[14].trigger = trigger; int_sources[14].int_cb = callback; break;
		case GPIO_PIN6: PCMSK1 |= (1 << PCINT15); int_sources[15].trigger = trigger; int_sources[15].int_cb = callback; break;
		default:
			return -1;
		}
		break;
	case GPIO_PORTK:
		portk_status = PINK;
		PCICR |= (1 << PCIE2);		// enable interrupt vector for PK7 .. PK0
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
