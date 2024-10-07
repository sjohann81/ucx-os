#include <ucx.h>
#include <i2c_bitbang.h>

/* GPIO template callbacks - port them! */

/* configure SCL and SDA pins direction
 * 
 * SCL - open drain, output configured as logic low
 * SDA - open drain, output configured as logic low
 */
int gpio_config(void)
{
	printf("I2C: gpio_config()\n");

	return 0;
}

/* read or write SCL and SDA pins
 * if val == -1, read pin and return val
 * if val == 0, write value low
 * if val == 1, write value high
 */
int gpio_scl(int val)
{
	printf("I2C: gpio_scl() %d\n", val);

	return 1;
}

int gpio_sda(int val)
{
	printf("I2C: gpio_sda() %d\n", val);

	return 1;
}

const struct i2c_config_s i2c_config = {
	.sig_delay = 4,
	.gpio_config = gpio_config,
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


uint8_t i2c_eeprom_read(uint8_t device, uint16_t addr)
{
	uint8_t byte = 0;
	char data[3];

	byte = 0xA0 | ((device & 0x07) << 1);

	data[0] = byte;
	data[1] = (addr & 0x7f00) >> 8;
	data[2] = addr & 0x00ff;

	i2c_open(&i2c_device1, 0);
	
	// select peripheral and write memory address
	i2c_write(&i2c_device1, data, 3);

	byte = 0xA0 | ((device & 0x07) << 1) | 0x01;
	data[0] = byte;
	
	// restart (write size zero)
	i2c_write(&i2c_device1, data, 0);
	
	// select peripheral for reading
	i2c_write(&i2c_device1, data, 1);
	
	// read data
	i2c_read(&i2c_device1, data, 1);
	byte = data[0];
	
	i2c_close(&i2c_device1);

	return byte;
}

void idle(void)
{
	for (;;);
}

void task0(void)
{
	char *msg = "hello world!";
	char buf[100];
	int bytes;
	
	while (1) {
		memset(buf, 0, sizeof(buf));
		
		// device address (write operation, LSB clear)
		buf[0] = 0x50;
		// payload
		memcpy(&buf[1], msg, strlen(msg) + 1);

		// a) write raw I2C data
		i2c_open(&i2c_device1, 0);
		i2c_write(&i2c_device1, buf, strlen(msg) + 1);
		i2c_close(&i2c_device1);
		
		// b) read raw I2C data
		i2c_open(&i2c_device1, 0);
		// write device address (read operation, LSB set)
		buf[0] = 0x51;
		i2c_write(&i2c_device1, buf, 1);
		// read data
		bytes = i2c_read(&i2c_device1, buf, 10);
		i2c_close(&i2c_device1);

		// c) read EEPROM
		i2c_eeprom_read(0x00, 0x1234);
		
		ucx_task_delay(500);
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);

	i2c_init(&i2c_device1);

	// start UCX/OS, preemptive mode
	return 1;
}
