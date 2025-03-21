#include <ucx.h>
#include <device.h>
#include <gpio.h>
#include <i2c_bitbang.h>

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


/* application */
void i2c_eeprom_bufread(uint8_t device, uint16_t addr, uint8_t *buf, uint8_t size)
{
	uint8_t byte = 0;
	char data[3];

	byte = 0xA0 | ((device & 0x07) << 1);

	data[0] = byte;
	data[1] = (addr & 0x7f00) >> 8;
	data[2] = addr & 0x00ff;

	dev_open(i2c1, 0);
	
	// select peripheral and write memory address
	dev_write(i2c1, data, 3);

	byte = 0xA0 | ((device & 0x07) << 1) | 0x01;
	data[0] = byte;
	
	// restart (write size zero)
	dev_write(i2c1, data, 0);
	
	// select peripheral for reading
	dev_write(i2c1, data, 1);
	
	// read data
	dev_read(i2c1, buf, size);
	
	dev_close(i2c1);
}

void i2c_eeprom_pagewrite(uint8_t device, uint16_t addr, uint8_t *buf, uint8_t size)
{
	uint8_t byte = 0;
	char data[35];

	byte = 0xA0 | ((device & 0x07) << 1);

	data[0] = byte;
	data[1] = (addr & 0x7f00) >> 8;
	data[2] = addr & 0x00ff;
	if (size > 32) size = 32;
	memcpy(data + 3, buf, size);

	dev_open(i2c1, 0);
	
	// select peripheral address (should be aligned to page boundary)
	// and write data
	dev_write(i2c1, data, size + 3);

	dev_close(i2c1);
	
	_delay_ms(5);
}

void idle(void)
{
	for (;;);
}

void task0(void)
{
	uint8_t buf[100];
	
	memset(buf, 0x69, sizeof(buf));
	i2c_eeprom_pagewrite(0x00, 0x1000, buf, 32);
	memset(buf, 0x22, sizeof(buf));
	i2c_eeprom_pagewrite(0x00, 0x1020, buf, 20);
	
	while (1) {
		memset(buf, 0, sizeof(buf));
		i2c_eeprom_bufread(0x00, 0x1000, buf, 64);
		hexdump((char *)buf, 64);
		printf("\n");

		ucx_task_delay(500);
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);

	dev_init(i2c1);

	// start UCX/OS, preemptive mode
	return 1;
}
