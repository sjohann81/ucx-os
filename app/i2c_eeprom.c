#include <ucx.h>
#include <device.h>
#include <gpio.h>
#include <i2c_bitbang.h>
#include <24lcxxx.h>

/* GPIO configuration: PB6 (scl) and PB7 (sda) - port it! */
const struct gpio_config_s gpio_config = {
	.config_values.port	= GPIO_PORTB,
	.config_values.pinsel	= GPIO_PIN6 | GPIO_PIN7,
	.config_values.mode	= GPIO_OUTPUT_OD << GPIO_PIN6_OPT |
				GPIO_OUTPUT_OD << GPIO_PIN7_OPT,
	.config_values.pull	= GPIO_PULLUP << GPIO_PIN6_OPT |
				GPIO_PULLUP << GPIO_PIN7_OPT
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

/* configure SCL and SDA pins direction
 * 
 * SCL - open drain, output configured as logic low
 * SDA - open drain, output configured as logic low
 */
int gpio_configpins(void)
{
	printf("I2C: gpio_configpins()\n");
	gpio_dev_api->gpio_setup(gpio);

	return 0;
}

/* read or write SCL and SDA pins
 * if val == -1, read pin and return value (0 or 1)
 * if val == 0, write value low
 * if val == 1, write value high
 */
int gpio_scl(int val)
{
	switch (val) {
	case -1: return ((gpio_dev_api->gpio_get(gpio) & GPIO_PIN6) >> 6);
	case 0: gpio_dev_api->gpio_clear(gpio, GPIO_PIN6); return 0;
	case 1: gpio_dev_api->gpio_set(gpio, GPIO_PIN6); return 0;
	default: return -1;
	}
}

int gpio_sda(int val)
{
	switch (val) {
	case -1: return ((gpio_dev_api->gpio_get(gpio) & GPIO_PIN7) >> 7);
	case 0: gpio_dev_api->gpio_clear(gpio, GPIO_PIN7); return 0;
	case 1: gpio_dev_api->gpio_set(gpio, GPIO_PIN7); return 0;
	default: return -1;
	}
}


/* I2C (bitbang) configuration and driver instantiation */
const struct i2c_config_s i2c_config = {
	.sig_delay = I2C_SPEED_NORMAL,
	.gpio_configpins = gpio_configpins,
	.gpio_scl = gpio_scl,
	.gpio_sda = gpio_sda
};

struct i2c_data_s i2c_data;

const struct device_s i2c_device1 = {
	.name = "i2cdevice1",
	.config = &i2c_config,
	.data = &i2c_data,
	.api = &i2c_api
};

const struct device_s *i2c1 = &i2c_device1;


/* EEPROM configuration and driver instantiation */
const struct eeprom_24lc_config_s eeprom_config = {
	.config_values.type = EEPROM_24LC256,
	.config_values.select = 0,
	.i2c_dev = &i2c_device1
};

struct eeprom_24lc_data_s eeprom_data;

const struct device_s eeprom_device1 = {
	.name = "24lcxx",
	.config = &eeprom_config,
	.data = &eeprom_data,
	.api = &eeprom_24lc_api
};

const struct device_s *eeprom1 = &eeprom_device1;


/* application */
void idle(void)
{
	for (;;);
}


uint8_t buf[300] = "The quick brown fox jumps over the lazy dog THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG QUICK QUICK QUICK QUICK BROWN BROWN BROWN BROWN";

void task0(void)
{
	int size;
	
	if (!dev_open(eeprom1, 0)) {
		dev_seek(eeprom1, 0x2000, SEEK_SET);
		size = dev_write(eeprom1, buf, 130);
		printf("wrote %d bytes.\n", size);
		dev_seek(eeprom1, 0x20c9, SEEK_SET);
		size = dev_write(eeprom1, buf, 15);
		printf("wrote %d bytes.\n", size);
		dev_close(eeprom1);
	}
	
	while (1) {
		if (!dev_open(eeprom1, 0)) {
			printf("pos: %d\n", dev_seek(eeprom1, 0, SEEK_CUR));
			memset(buf, 0, sizeof(buf));
			dev_seek(eeprom1, 0x2000, SEEK_SET);
			printf("pos: %d\n", dev_seek(eeprom1, 0, SEEK_CUR));
			dev_read(eeprom1, buf, 256);
			dev_close(eeprom1);
			hexdump((char *)buf, 256);
			printf("\n");
		} else {
			printf("error opening device %d\n", eeprom1->name);
		}

		ucx_task_delay(500);
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);

	dev_init(i2c1);
	dev_init(eeprom1);

	// start UCX/OS, preemptive mode
	return 1;
}
