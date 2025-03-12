#include <ucx.h>
#include <device.h>
#include <gpio.h>
#include <gpio_ll.h>

/* GPIO API function wrappers */
int gpio_setup(const struct device_s *dev)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	return api->gpio_setup(dev);
}
	
uint16_t gpio_get(const struct device_s *dev)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	return api->gpio_get(dev);
}

void gpio_set(const struct device_s *dev, uint16_t pins)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	api->gpio_set(dev, (int)pins);
}

void gpio_clear(const struct device_s *dev, uint16_t pins)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	api->gpio_clear(dev, (int)pins);
}

void gpio_toggle(const struct device_s *dev, uint16_t pins)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	api->gpio_toggle(dev, (int)pins);
}

int gpio_int_attach(const struct device_s *dev, uint16_t pin, void (*callback)(), uint8_t trigger)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	return api->gpio_int_attach(dev, (int)pin, callback, (int)trigger);
}


/* GPIO device driver implementation */
static int driver_setup(const struct device_s *dev)
{
	struct gpio_config_s *config;
	int val;
	
	config = (struct gpio_config_s *)dev->config;
	
	val = gpio_ll_setup(&config->config_values);
	
	return val;
}

static int driver_get(const struct device_s *dev)
{
	struct gpio_config_s *config;
	int val;
	
	config = (struct gpio_config_s *)dev->config;
	
	val = gpio_ll_get(&config->config_values);
	
	if (val < 0)
		return val;
	else
		return val & 0xffff;
}

static void driver_set(const struct device_s *dev, int pins)
{
	struct gpio_config_s *config;
	
	config = (struct gpio_config_s *)dev->config;
	
	gpio_ll_set(&config->config_values, pins);
}

static void driver_clear(const struct device_s *dev, int pins)
{
	struct gpio_config_s *config;
	
	config = (struct gpio_config_s *)dev->config;
	
	gpio_ll_clear(&config->config_values, pins);
}

static void driver_toggle(const struct device_s *dev, int pins)
{
	struct gpio_config_s *config;
	
	config = (struct gpio_config_s *)dev->config;
	
	gpio_ll_toggle(&config->config_values, pins);
}

static int driver_int_attach(const struct device_s *dev, int pin, void (*callback)(), int trigger)
{
	struct gpio_config_s *config;
	
	config = (struct gpio_config_s *)dev->config;
	
	return gpio_ll_int_attach(&config->config_values, pin, callback, trigger);
}

/* GPIO device driver function mapping */
struct gpio_api_s gpio_api = {
	.gpio_setup = driver_setup,
	.gpio_get = driver_get,
	.gpio_set = driver_set,
	.gpio_clear = driver_clear,
	.gpio_toggle = driver_toggle,
	.gpio_int_attach = driver_int_attach
};
