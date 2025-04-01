#include <ucx.h>
#include <device.h>
#include <pwm.h>
#include <pwm_ll.h>

/* PWM API function wrappers */
int pwm_setup(const struct device_s *dev, int timer, int direction, int mode)
{
	struct pwm_api_s *api;

	api = (struct pwm_api_s *)dev->custom_api;
	return api->pwm_setup(dev, timer, direction, mode);
}
	
void pwm_get(const struct device_s *dev, int channel, unsigned long *period, unsigned long *pulse)
{
	struct pwm_api_s *api;

	api = (struct pwm_api_s *)dev->custom_api;
	api->pwm_get(dev, channel, period, pulse);
}

void pwm_set(const struct device_s *dev, int channel, unsigned long period, unsigned long pulse)
{
	struct pwm_api_s *api;

	api = (struct pwm_api_s *)dev->custom_api;
	api->pwm_set(dev, channel, period, pulse);
}


/* PWM device driver implementation */
static int driver_setup(const struct device_s *dev, int timer, int direction, int mode)
{
	struct pwm_config_s *config;
	int val;
	
	config = (struct pwm_config_s *)dev->config;
	
	val = pwm_ll_setup(&config->config_values);
	
	return val;
}
	
static void driver_get(const struct device_s *dev, int channel, unsigned long *period, unsigned long *pulse)
{
	struct pwm_config_s *config;
	int val;
	
	config = (struct pwm_config_s *)dev->config;
	
	val = pwm_ll_get(&config->config_values, channel, period, pulse);
	
	if (val < 0) {
		*period = 0;
		*pulse = 0;
	}
}

static void driver_set(const struct device_s *dev, int channel, unsigned long period, unsigned long pulse)
{
	struct pwm_config_s *config;
	
	config = (struct pwm_config_s *)dev->config;
	
	pwm_ll_set(&config->config_values, channel, period, pulse);
}


/* PWM device driver function mapping */
struct pwm_api_s pwm_api = {
	.pwm_setup = driver_setup,
	.pwm_get = driver_get,
	.pwm_set = driver_set
};
