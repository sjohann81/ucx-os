#include <ucx.h>
#include <device.h>
#include <gpio.h>

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

void gpio_setbits(const struct device_s *dev, uint16_t pins)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	api->gpio_setbits(dev, (int)pins);
}

void gpio_clearbits(const struct device_s *dev, uint16_t pins)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	api->gpio_clearbits(dev, (int)pins);
}

void gpio_togglebits(const struct device_s *dev, uint16_t pins)
{
	struct gpio_api_s *api;

	api = (struct gpio_api_s *)dev->custom_api;
	api->gpio_togglebits(dev, (int)pins);
}


/* GPIO device driver implementation */
static int driver_setup(const struct device_s *dev)
{
	struct gpio_config_s *config;
	int val;
	
	config = (struct gpio_config_s *)dev->config;
	
	val = config->gpio_ll_setup(&config->config_values);
	
	return val;
}

static int driver_get(const struct device_s *dev)
{
	struct gpio_config_s *config;
	int val;
	
	config = (struct gpio_config_s *)dev->config;
	
	val = config->gpio_ll_get(&config->config_values);
	
	if (val < 0)
		return val;
	else
		return val & 0xffff;
}

static void driver_set(const struct device_s *dev, int pins)
{
	struct gpio_config_s *config;
	
	config = (struct gpio_config_s *)dev->config;
	
	config->gpio_ll_set(&config->config_values, pins);
}

static void driver_setbits(const struct device_s *dev, int pins)
{
	uint16_t val;
	
	val = driver_get(dev) | pins;
	driver_set(dev, val);
}

static void driver_clearbits(const struct device_s *dev, int pins)
{
	uint16_t val;
	
	val = driver_get(dev) & ~pins;
	driver_set(dev, val);
}

static void driver_togglebits(const struct device_s *dev, int pins)
{
	uint16_t val;
	
	val = driver_get(dev) ^ pins;
	driver_set(dev, val);
}

/* GPIO device driver function mapping */
struct gpio_api_s gpio_api = {
	.gpio_setup = driver_setup,
	.gpio_get = driver_get,
	.gpio_set = driver_set,
	.gpio_setbits = driver_setbits,
	.gpio_clearbits = driver_clearbits,
	.gpio_togglebits = driver_togglebits
};
