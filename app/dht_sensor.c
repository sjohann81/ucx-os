#include <ucx.h>
#include <device.h>
#include <gpio.h>
#include <dht.h>

#define DEBUG		1

/* GPIO configuration: PB0 (data) - port it! */
const struct gpio_config_s gpio_config = {
	.config_values.port	= GPIO_PORTB,
	.config_values.pinsel	= GPIO_PIN0,
	.config_values.mode	= GPIO_OUTPUT_OD << GPIO_PIN0_OPT,
	.config_values.pull	= GPIO_PULLUP << GPIO_PIN0_OPT
};

/* GPIO device driver instantiation */
const struct device_s gpio_device = {
	.name = "gpio_device",
	.config = &gpio_config,
	.custom_api = &gpio_api
};

const struct device_s *gpio = &gpio_device;
const struct gpio_api_s *gpio_dev_api = (const struct gpio_api_s *)(&gpio_device)->custom_api;


/* GPIO template callbacks - port them! */

int gpio_configpin(void)
{
	printf("DHT: gpio_configpins()\n");
	gpio_dev_api->gpio_setup(gpio);

	return 0;
}

/* read or write DATA pin
 * if val == -1, read pin and return value (0 or 1)
 * if val == 0, write value low
 * if val == 1, write value high
 */
int gpio_sdata(int val)
{
	switch (val) {
	case -1: return ((gpio_dev_api->gpio_get(gpio) & GPIO_PIN0) >> 0);
	case 0: gpio_dev_api->gpio_clear(gpio, GPIO_PIN0); return 0;
	case 1: gpio_dev_api->gpio_set(gpio, GPIO_PIN0); return 0;
	default: return -1;
	}
}

/* DHT configuration and driver instantiation */
const struct dht_config_s dht_config = {
	.gpio_configpin = gpio_configpin,
	.gpio_sdata = gpio_sdata,
	.type = DHT11
};

struct dht_data_s dht_data;

const struct device_s dht_device1 = {
	.name = "dhtdevice1",
	.config = &dht_config,
	.data = &dht_data,
	.api = &dht_api
};

const struct device_s *dht1 = &dht_device1;


void idle(void)
{
	while (1) {
	}
}

void task_sensor(void)
{
	int val, count;
	char buf[10];
	struct dht_data_s *pdata;
	
	pdata = (struct dht_data_s *)dht1->data;
	
	while (1) {
		val = dev_open(dht1, DEBUG);
		
		if (val == ERR_OK) {
			count = dev_read(dht1, buf, 5);
			
			if (count > 0) {
				printf("sensor1: %d %d %d %d %d\n",
					buf[0], buf[1], buf[2], buf[3], buf[4]);
				printf("sensor1: temp %d.%dC, humidity: %d.%d\n",
				pdata->temperature / 10, pdata->temperature % 10,
				pdata->humidity / 10, pdata->humidity % 10);
			}
			
			dev_close(dht1);
		}
		
		/* poll every 2s - not too fast, according to the datasheet */
		ucx_task_delay(200);
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task_sensor, DEFAULT_STACK_SIZE);

	/* init sensor (DHT11) */
	dev_init(dht1);

	// start UCX/OS, preemptive mode
	return 1;
}
