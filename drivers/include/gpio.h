void gpio_init(const struct device_s *dev, uint8_t port);
void gpio_deinit(const struct device_s *dev);
void gpio_setup(const struct device_s *dev, struct gpio_config_values_s *new_config);
int gpio_get(const struct device_s *dev, uint16_t pin);
void gpio_set(const struct device_s *dev, uint16_t pin);
void gpio_clear(const struct device_s *dev, uint16_t pin);
void gpio_toggle(const struct device_s *dev, uint16_t pin);
uint16_t gpio_read(const struct device_s *dev);
void gpio_write(const struct device_s *dev, uint16_t pins);

struct gpio_config_values_s {
	uint16_t pin;
	uint16_t direction;
	uint16_t pull;
	uint16_t pull_du;
};

struct gpio_config_s {
	int (*gpio_init)(uint8_t port);
	int (*gpio_deinit)(uint8_t port);
	int (*gpio_config)(struct gpio_config_values_s *config);
	int (*gpio_get)(void);
	int (*gpio_set)(int val);
	int (*gpio_read)(void);
	int (*gpio_write)(int val);
};
