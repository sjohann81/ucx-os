#include <hal.h>
#include <device.h>
#include <gpio.h>
#include <gpio_ll.h>


static uint16_t od_outputs[4];

int gpio_ll_setup(struct gpio_config_values_s *cfg)
{
	uint32_t modesel;
	
	for (int i = 0; i < 16; i++) {
		if (cfg->pinsel & (1 << i)) {
			modesel = (cfg->mode & (3 << (i << 1))) >> (i << 1);
			switch (modesel) {
			case GPIO_INPUT:
				switch (cfg->port) {
				case GPIO_PORTA: od_outputs[0] &= ~(1 << i); PADDR &= ~(1 << i); break;
				case GPIO_PORTB: od_outputs[1] &= ~(1 << i); PBDDR &= ~(1 << i); break;
				case GPIO_PORTC: od_outputs[2] &= ~(1 << i); PCDDR &= ~(1 << i); break;
				case GPIO_PORTD: od_outputs[3] &= ~(1 << i); PDDDR &= ~(1 << i); break;
				default:
					return -1;
				}
				break;
			case GPIO_OUTPUT:
				switch (cfg->port) {
				case GPIO_PORTA: od_outputs[0] &= ~(1 << i); PADDR |= (1 << i); break;
				case GPIO_PORTB: od_outputs[1] &= ~(1 << i); PBDDR |= (1 << i); break;
				case GPIO_PORTC: od_outputs[2] &= ~(1 << i); PCDDR |= (1 << i); break;
				case GPIO_PORTD: od_outputs[3] &= ~(1 << i); PDDDR |= (1 << i); break;
				default:
					return -1;
				}
				break;
			case GPIO_OUTPUT_OD:
				switch (cfg->port) {
				case GPIO_PORTA: od_outputs[0] |= (1 << i); PAOUT &= ~(1 << i); PADDR &= ~(1 << i); break;
				case GPIO_PORTB: od_outputs[1] |= (1 << i); PBOUT &= ~(1 << i); PBDDR &= ~(1 << i); break;
				case GPIO_PORTC: od_outputs[2] |= (1 << i); PCOUT &= ~(1 << i); PCDDR &= ~(1 << i); break;
				case GPIO_PORTD: od_outputs[3] |= (1 << i); PDOUT &= ~(1 << i); PDDDR &= ~(1 << i); break;
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
	case GPIO_PORTA: val = PAIN; break;
	case GPIO_PORTB: val = PBIN; break;
	case GPIO_PORTC: val = PCIN; break;
	case GPIO_PORTD: val = PDIN; break;
	default:
		return -1;
	}
	
	return val & cfg->pinsel;
}


static struct port_data_s {
	volatile uint32_t *dir;
	volatile uint32_t *port;
	volatile uint32_t *pin;
} port_data[4] = {
	{&PADDR, &PAOUT, &PAIN},
	{&PBDDR, &PBOUT, &PBIN},
	{&PCDDR, &PCOUT, &PCIN},
	{&PDDDR, &PDOUT, &PDIN}
};

int gpio_ll_set(struct gpio_config_values_s *cfg, int val)
{
	uint16_t mask = val & cfg->pinsel;

	if (cfg->port > GPIO_PORTD)
		return -1;
	
	if (od_outputs[cfg->port]) {
		for (int i = 0; i < 16; i++) {
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
	uint16_t mask = val & cfg->pinsel;

	if (cfg->port > GPIO_PORTD)
		return -1;
	
	if (od_outputs[cfg->port]) {
		for (int i = 0; i < 16; i++) {
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
	uint16_t mask = val & cfg->pinsel;

	if (cfg->port > GPIO_PORTD)
		return -1;
	
	if (od_outputs[cfg->port]) {
		for (int i = 0; i < 16; i++) {
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

static volatile uint16_t porta_status = 0;
static volatile uint16_t portb_status = 0;

void porta_handler(void)
{
	uint16_t porta_change;
	
	porta_change = PAIN ^ porta_status;
	porta_status = PAIN;
	
	for (int i = 0; i < 16; i++) {
		if (porta_change & (1 << i)) {
			if (int_sources[i].trigger == GPIO_CHANGE) {
				(int_sources[i].int_cb)();
				continue;
			}
				
			if (porta_status & (1 << i)) {
				if (int_sources[i].trigger == GPIO_RISING)
					(int_sources[i].int_cb)();
			} else {
				if (int_sources[i].trigger == GPIO_FALLING)
					(int_sources[i].int_cb)();
			}
		}
	}
}

void portb_handler(void)
{
	uint16_t portb_change;
	
	portb_change = PBIN ^ portb_status;
	portb_status = PBIN;
	
	for (int i = 0; i < 16; i++) {
		if (portb_change & (1 << i)) {
			if (int_sources[i + 16].trigger == GPIO_CHANGE) {
				(int_sources[i + 16].int_cb)();
				continue;
			}
				
			if (portb_status & (1 << i)) {
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
	case GPIO_PORTA:
		porta_status = PAIN;
		GPIOMASK |= MASK_PAIN;		// enable interrupt vector for PA15 .. PA0
		
		for (int i = 0; i < 16; i++) {
			if (pin & (1 << i)) {
				 PAINMASK |= (1 << i);
				 int_sources[i].trigger = trigger;
				 int_sources[i].int_cb = callback;
				 break;
			}
		}
		break;
	case GPIO_PORTB:
		portb_status = PBIN;
		GPIOMASK |= MASK_PBIN;		// enable interrupt vector for PB15 .. PB0
		
		for (int i = 0; i < 16; i++) {
			if (pin & (1 << i)) {
				 PBINMASK |= (1 << i);
				 int_sources[i + 16].trigger = trigger;
				 int_sources[i + 16].int_cb = callback;
				 break;
			}
		}
		break;
	default:
		return -1;
	}
	
	return 0;
}
