/* GPIO driver API */
int gpio_setup(const struct device_s *dev);
uint16_t gpio_get(const struct device_s *dev);
void gpio_set(const struct device_s *dev, uint16_t pins);
void gpio_clear(const struct device_s *dev, uint16_t pins);
void gpio_toggle(const struct device_s *dev, uint16_t pins);
int gpio_int_attach(const struct device_s *dev, uint16_t pin, void (*callback)(), uint8_t trigger);

/* GPIO configuration definitions */
struct gpio_api_s {
	int (*gpio_setup)(const struct device_s *dev);
	int (*gpio_get)(const struct device_s *dev);
	void (*gpio_set)(const struct device_s *dev, int val);
	void (*gpio_clear)(const struct device_s *dev, int val);
	void (*gpio_toggle)(const struct device_s *dev, int val);
	int (*gpio_int_attach)(const struct device_s *dev, int pin, void (*callback)(), int trigger);
};

enum port_names {
	GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, GPIO_PORTD,
	GPIO_PORTE, GPIO_PORTF, GPIO_PORTG, GPIO_PORTH,
	GPIO_PORTI, GPIO_PORTJ, GPIO_PORTK, GPIO_PORTL,
	GPIO_PORTM, GPIO_PORTN, GPIO_PORTO, GPIO_PORTP
};

enum pin_names {
	GPIO_PIN0 = (1 << 0), GPIO_PIN1 = (1 << 1),
	GPIO_PIN2 = (1 << 2), GPIO_PIN3 = (1 << 3),
	GPIO_PIN4 = (1 << 4), GPIO_PIN5 = (1 << 5),
	GPIO_PIN6 = (1 << 6), GPIO_PIN7 = (1 << 7),
	GPIO_PIN8 = (1 << 8), GPIO_PIN9 = (1 << 9),
	GPIO_PIN10 = (1 << 10), GPIO_PIN11 = (1 << 11),
	GPIO_PIN12 = (1 << 12), GPIO_PIN13 = (1 << 13),
	GPIO_PIN14 = (1 << 14), GPIO_PIN15 = (1 << 15)
};

enum pin_names_opt {
	GPIO_PIN0_OPT = 0, GPIO_PIN1_OPT = 2,
	GPIO_PIN2_OPT = 4, GPIO_PIN3_OPT = 6,
	GPIO_PIN4_OPT = 8, GPIO_PIN5_OPT = 10,
	GPIO_PIN6_OPT = 12, GPIO_PIN7_OPT = 14,
	GPIO_PIN8_OPT = 16, GPIO_PIN9_OPT = 18,
	GPIO_PIN10_OPT = 20, GPIO_PIN11_OPT = 22,
	GPIO_PIN12_OPT = 24, GPIO_PIN13_OPT = 26,
	GPIO_PIN14_OPT = 28, GPIO_PIN15_OPT = 30
};

enum pin_mode {
	GPIO_INPUT, GPIO_OUTPUT, GPIO_OUTPUT_OD, GPIO_ALT
};

enum pin_pull {
	GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
};

enum pin_irqmode {
	GPIO_LOW, GPIO_HIGH, GPIO_RISING, GPIO_FALLING, GPIO_CHANGE
};

struct gpio_config_values_s {
	uint8_t port;
	uint16_t pinsel;
	uint32_t mode;
	uint32_t pull;
};

struct gpio_config_s {
	struct gpio_config_values_s config_values;
};

extern struct gpio_api_s gpio_api;
