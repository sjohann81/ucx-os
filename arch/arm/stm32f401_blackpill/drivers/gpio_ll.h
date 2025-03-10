#define LED_PORT	GPIO_PORTC
#define LED_PIN		GPIO_PIN13
#define LED_PIN_OPT	GPIO_PIN13_OPT
#define MAX_INT_SOURCES	16
	
int gpio_ll_setup(struct gpio_config_values_s *cfg);
int gpio_ll_get(struct gpio_config_values_s *cfg);
int gpio_ll_set(struct gpio_config_values_s *cfg, int val);
int gpio_ll_clear(struct gpio_config_values_s *cfg, int val);
int gpio_ll_toggle(struct gpio_config_values_s *cfg, int val);
int gpio_ll_int_attach(struct gpio_config_values_s *cfg, int pin, void (*callback)(), int trigger);
