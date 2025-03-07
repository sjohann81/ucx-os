#include <ucx.h>
#include <device.h>
#include <i2c.h>
#include <i2c_ll.h>

/* I2C configuration */
const struct i2c_hw_config_s i2c_config = {
	.config_values.port = I2C_PORT1,
	.config_values.speed = 10000,
	.config_values.mode = I2C_MASTER,
	.config_values.ack = I2C_NACK,
	.config_values.addr_mode = I2C_ADDR7BIT
};

struct i2c_hw_data_s i2c_data;

const struct device_s i2c_device1 = {
	.name = "i2cdevice1",
	.config = &i2c_config,
	.data = &i2c_data,
	.api = &i2c_hw_api
};

const struct device_s *i2c1 = &i2c_device1;

/* application */
uint8_t i2c_eeprom_read(uint8_t device, uint16_t addr)
{
	uint8_t byte = 0;
	char data[3];

	byte = 0xA0 | ((device & 0x07) << 1);

	data[0] = byte;
	data[1] = (addr & 0x7f00) >> 8;
	data[2] = addr & 0x00ff;

	i2c1->api->dev_open(i2c1, 0);
	
	// select peripheral and write memory address
	i2c1->api->dev_write(i2c1, data, 3);

	byte = 0xA0 | ((device & 0x07) << 1) | 0x01;
	data[0] = byte;
	
	// restart (write size zero)
	i2c1->api->dev_write(i2c1, data, 0);
	
	// select peripheral for reading
	i2c1->api->dev_write(i2c1, data, 1);
	
	// read data
	i2c1->api->dev_read(i2c1, data, 1);
	byte = data[0];
	
	i2c1->api->dev_close(i2c1);

	return byte;
}

uint8_t i2c_eeprom_write(uint8_t device, uint16_t addr, uint8_t val)
{
	uint8_t byte = 0;
	char data[4];

	byte = 0xA0 | ((device & 0x07) << 1);

	data[0] = byte;
	data[1] = (addr & 0x7f00) >> 8;
	data[2] = addr & 0x00ff;
	data[3] = val;

	i2c1->api->dev_open(i2c1, 0);
	
	// select peripheral and write 1 byte to memory address
	i2c1->api->dev_write(i2c1, data, 4);

	i2c1->api->dev_close(i2c1);
	
	_delay_ms(5);

	return byte;
}

void i2c_eeprom_bufread(uint8_t device, uint16_t addr, uint8_t *buf, uint8_t size)
{
	uint8_t byte = 0;
	char data[3];

	byte = 0xA0 | ((device & 0x07) << 1);

	data[0] = byte;
	data[1] = (addr & 0x7f00) >> 8;
	data[2] = addr & 0x00ff;

	i2c1->api->dev_open(i2c1, 0);
	
	// select peripheral and write memory address
	i2c1->api->dev_write(i2c1, data, 3);

	byte = 0xA0 | ((device & 0x07) << 1) | 0x01;
	data[0] = byte;
	
	// restart (write size zero)
	i2c1->api->dev_write(i2c1, data, 0);
	
	// select peripheral for reading
	i2c1->api->dev_write(i2c1, data, 1);
	
	// read data
	i2c1->api->dev_read(i2c1, buf, size);
	
	i2c1->api->dev_close(i2c1);
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

	i2c1->api->dev_open(i2c1, 0);
	
	// select peripheral address (should be aligned to page boundary)
	// and write data
	i2c1->api->dev_write(i2c1, data, size + 3);

	i2c1->api->dev_close(i2c1);
	
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
	
	while (1) {
		memset(buf, 0, sizeof(buf));
		i2c_eeprom_bufread(0x00, 0x1000, buf, 40);

		ucx_task_delay(500);
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);

	i2c1->api->dev_init(i2c1);

	// start UCX/OS, preemptive mode
	return 1;
}
