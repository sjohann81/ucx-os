#include <ucx.h>
#include <device.h>
#include <pwm.h>
#include <pwm_ll.h>

/* PWM API function wrappers */
long pwm_setup(const struct device_s *dev)
{
	struct pwm_api_s *api;

	api = (struct pwm_api_s *)dev->custom_api;
	return api->pwm_setup(dev);
}
	
void pwm_get(const struct device_s *dev, unsigned channel, unsigned *pulse)
{
	struct pwm_api_s *api;

	api = (struct pwm_api_s *)dev->custom_api;
	api->pwm_get(dev, channel, pulse);
}

void pwm_set(const struct device_s *dev, unsigned channel, unsigned pulse)
{
	struct pwm_api_s *api;

	api = (struct pwm_api_s *)dev->custom_api;
	api->pwm_set(dev, channel, pulse);
}


/* PWM device driver implementation */
static long driver_setup(const struct device_s *dev)
{
	struct pwm_config_s *config;
	long val;
	
	config = (struct pwm_config_s *)dev->config;
	
	val = pwm_ll_setup(&config->config_values);
	
	return val;
}
	
static void driver_get(const struct device_s *dev, unsigned channel, unsigned *pulse)
{
	struct pwm_config_s *config;
	int val;
	
	config = (struct pwm_config_s *)dev->config;
	
	val = pwm_ll_get(&config->config_values, channel, pulse);
	
	if (val < 0)
		*pulse = 0;
}

static void driver_set(const struct device_s *dev, unsigned channel, unsigned pulse)
{
	struct pwm_config_s *config;
	
	config = (struct pwm_config_s *)dev->config;
	
	pwm_ll_set(&config->config_values, channel, pulse);
}


/* PWM device driver function mapping */
struct pwm_api_s pwm_api = {
	.pwm_setup = driver_setup,
	.pwm_get = driver_get,
	.pwm_set = driver_set
};
