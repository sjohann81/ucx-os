#include <ucx.h>
#include <device.h>
#include <gpio.h>
#include <gpio_ll.h> 	// for LED_PORT and LED_PIN defs

/* GPIO configuration */
const struct gpio_config_s gpio_config = {
	.config_values.port = LED_PORT,
	.config_values.pinsel = LED_PIN,
	.config_values.mode = GPIO_OUTPUT << LED_PIN_OPT,
	.config_values.pull = GPIO_NOPULL << LED_PIN_OPT
};

/* device driver instantiation */
const struct device_s gpio_device = {
	.name = "gpio_device",
	.config = &gpio_config,
	.custom_api = &gpio_api
};

const struct device_s *gpio = &gpio_device;
const struct gpio_api_s *gpio_dev_api = (const struct gpio_api_s *)(&gpio_device)->custom_api;


/* application tasks */
void task_idle(void)
{
	for (;;);
}

void task_blink(void)
{
	gpio_dev_api->gpio_setup(gpio);

	while (1) {
		gpio_dev_api->gpio_toggle(gpio, LED_PIN);
		ucx_task_delay(MS_TO_TICKS(500));
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task_blink, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task_idle, DEFAULT_STACK_SIZE);

	// start UCX/OS, preemptive mode
	return 1;
}
