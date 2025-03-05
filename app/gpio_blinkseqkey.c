#include <ucx.h>
#include <device.h>
#include <gpio.h>

/* GPIO configuration */
const struct gpio_config_s gpio_config = {
	.config_values.port	= GPIO_PORTB,
	.config_values.pinsel	= GPIO_PIN0 | GPIO_PIN1 |
				GPIO_PIN2 | GPIO_PIN3 |
				GPIO_PIN6,
	.config_values.mode	= GPIO_OUTPUT << GPIO_PIN0_OPT |
				GPIO_OUTPUT << GPIO_PIN1_OPT |
				GPIO_OUTPUT << GPIO_PIN2_OPT |
				GPIO_OUTPUT << GPIO_PIN3_OPT |
				GPIO_INPUT << GPIO_PIN6_OPT,
	.config_values.pull	= GPIO_NOPULL << GPIO_PIN0_OPT |
				GPIO_NOPULL << GPIO_PIN1_OPT |
				GPIO_NOPULL << GPIO_PIN2_OPT |
				GPIO_NOPULL << GPIO_PIN3_OPT |
				GPIO_PULLUP << GPIO_PIN6_OPT
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

	gpio_dev_api->gpio_set(gpio, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3);
	ucx_task_delay(MS_TO_TICKS(1000));
	gpio_dev_api->gpio_clear(gpio, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3);
	ucx_task_delay(MS_TO_TICKS(1000));

	while (1) {
		if (!(gpio_dev_api->gpio_get(gpio) & GPIO_PIN6)) {
			for (int i = 0; i < 4; i++) {
				gpio_dev_api->gpio_set(gpio, 1 << i);
				ucx_task_delay(MS_TO_TICKS(500));
				gpio_dev_api->gpio_clear(gpio, 1 << i);
			}
		}
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task_blink, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task_idle, DEFAULT_STACK_SIZE);

	// start UCX/OS, preemptive mode
	return 1;
}
