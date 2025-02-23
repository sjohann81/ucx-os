/* GPIO driver API */
int gpio_config(const struct device_s *dev);
uint16_t gpio_get(const struct device_s *dev);
void gpio_set(const struct device_s *dev, uint16_t pins);
void gpio_setbits(const struct device_s *dev, uint16_t pins);
void gpio_clearbits(const struct device_s *dev, uint16_t pins);
void gpio_togglebits(const struct device_s *dev, uint16_t pins);

/* GPIO configuration definitions */
struct gpio_api_s {
	int (*gpio_config)(const struct device_s *dev);
	int (*gpio_get)(const struct device_s *dev);
	void (*gpio_set)(const struct device_s *dev, int val);
	void (*gpio_setbits)(const struct device_s *dev, int val);
	void (*gpio_clearbits)(const struct device_s *dev, int val);
	void (*gpio_togglebits)(const struct device_s *dev, int val);
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

struct gpio_config_values_s {
	uint8_t port;
	uint16_t pin;
	uint16_t direction;
	uint16_t pull;
	uint16_t pull_du;
};

struct gpio_config_s {
	struct gpio_config_values_s config_values;
	int (*gpio_ll_config)(struct gpio_config_values_s *cfg);
	int (*gpio_ll_get)(struct gpio_config_values_s *cfg);
	int (*gpio_ll_set)(struct gpio_config_values_s *cfg, int val);
};

extern struct gpio_api_s gpio_api;
