struct gpio_setup_s {
	uint16_t port;
	uint16_t direction;
	uint16_t mode;
	uint16_t pull;
	uint16_t pull_du;
};

void gpio_init(GPIO_Device *gpio, uint16_t port);
void gpio_deinit(GPIO_Device *gpio, uint16_t port);
void gpio_setup(GPIO_Device *gpio, uint16_t pin, uint16_t dir, uint16_t mode, uint16_t pullup);
void gpio_set(GPIO_Device *gpio, uint16_t pin);
void gpio_clear(GPIO_Device *gpio, uint16_t pin);
void gpio_toggle(GPIO_Device *gpio, uint16_t pin);
uint16_t gpio_read(GPIO_Device *gpio, uint16_t pin);
