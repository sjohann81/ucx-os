#include <ucx.h>
#include <device.h>
#include <i2c_bitbang.h>

/* I2C master (bit bang) API function wrappers */
int i2c_init(const struct device_s *dev)
{
	return dev->api->dev_init(dev);
}

int i2c_deinit(const struct device_s *dev)
{
	return dev->api->dev_deinit(dev);
}

int i2c_open(const struct device_s *dev, int mode)
{
	return dev->api->dev_open(dev, mode);
}

int i2c_close(const struct device_s *dev)
{
	return dev->api->dev_close(dev);
}

size_t i2c_read(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_read(dev, buf, count);
}

size_t i2c_write(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_write(dev, buf, count);
}

/* I2C master low-level bus access implementation */
static int i2c_start(const struct device_s *dev)
{
	struct i2c_config_s *config;
	
	config = (struct i2c_config_s *)dev->config;
	
	// lost bus arbitration
	if (config->gpio_sda(-1) == 0)
		return -1;

	config->gpio_sda(0);
	_delay_us(config->sig_delay);
	config->gpio_scl(0);
	_delay_us(config->sig_delay);
	
	return 0;
}

static int i2c_restart(const struct device_s *dev, char cs)
{
	struct i2c_config_s *config;
	
	config = (struct i2c_config_s *)dev->config;

	config->gpio_sda(1);
	_delay_us(config->sig_delay);
	config->gpio_scl(1);
	
	// clock stretching
	if (cs)
		while (config->gpio_scl(-1) == 0);
	_delay_us(config->sig_delay);
	
	// lost bus arbitration
	if (config->gpio_sda(-1) == 0)
		return -1;

	config->gpio_sda(0);
	_delay_us(config->sig_delay);
	config->gpio_scl(0);
	_delay_us(config->sig_delay);
	
	return 0;
}

static int i2c_stop(const struct device_s *dev, char cs)
{
	struct i2c_config_s *config;
	
	config = (struct i2c_config_s *)dev->config;

	config->gpio_sda(0);
	_delay_us(config->sig_delay);
	config->gpio_scl(1);
	
	// clock stretching
	if (cs)
		while (config->gpio_scl(-1) == 0);
	_delay_us(config->sig_delay);
	
	config->gpio_sda(1);
	_delay_us(config->sig_delay);
	
	// lost bus arbitration
	if (config->gpio_sda(-1) == 0)
		return -1;

	return 0;
}	

static int i2c_write_bit(const struct device_s *dev, char bit, char cs)
{
	struct i2c_config_s *config;
	
	config = (struct i2c_config_s *)dev->config;
	
	bit ? config->gpio_sda(1) : config->gpio_sda(0);
	_delay_us(config->sig_delay);
	config->gpio_scl(1);
	_delay_us(config->sig_delay);
	
	// clock stretching
	if (cs)
		while (config->gpio_scl(-1) == 0);
	
	// arbitration lost
	if (bit && config->gpio_sda(-1) == 0)
		return -1;
		
	config->gpio_scl(0);
	
	return 0;
}

static int i2c_read_bit(const struct device_s *dev, char cs)
{
	char bit;
  	struct i2c_config_s *config;
	
	config = (struct i2c_config_s *)dev->config;

	config->gpio_sda(1);
	_delay_us(config->sig_delay);
	config->gpio_scl(1);
	
	// clock stretching
	if (cs)
		while (config->gpio_scl(-1) == 0);
	_delay_us(config->sig_delay);
	
	bit = config->gpio_sda(-1);
	
	config->gpio_scl(0);
	
	return bit;
}

static int i2c_write_byte(const struct device_s *dev, unsigned char byte)
{
	unsigned bit;
	int val;

	for (bit = 0; bit < 8; ++bit) {
		val = i2c_write_bit(dev, (byte & 0x80), 1);
		if (val < 0)
			return val;
			
		byte <<= 1;
	}

	val = i2c_read_bit(dev, 1);

	return val;
}

static unsigned char i2c_read_byte(const struct device_s *dev, char nack)
{
	unsigned bit;
	unsigned char byte = 0;
	int val;

	for (bit = 0; bit < 8; ++bit) {
		val = i2c_read_bit(dev, 1);
		if (val < 0)
			return val;
		
		byte = (byte << 1) | val;
	}

	val = i2c_write_bit(dev, nack, 1);
	
	if (val < 0)
		return val;

	return byte;
}

static void i2c_reset_bus(const struct device_s *dev)
{
	int i;

	i2c_start(dev);
	
	for (i = 0; i < 9; i++)
		i2c_write_bit(dev, 1, 0);
	
	i2c_restart(dev, 0);
	i2c_stop(dev, 0);
	
	_delay_us(100);
}

/* I2C master (bit bang) device driver implementation */
static int i2c_driver_init(const struct device_s *dev)
{
	struct i2c_config_s *config;
	struct i2c_data_s *data;
	int val;
	
	config = (struct i2c_config_s *)dev->config;
	data = (struct i2c_data_s *)dev->data;

	data->busy = 0;
	data->init = 0;
	
	val = config->gpio_configpins();
	if (val < 0)
		return val;
	
	config->gpio_scl(1);
	config->gpio_sda(1);
	_delay_us(config->sig_delay);
	data->init = 1;
	
	printf("I2C: %s, i2c_init()\n", dev->name);
	
	return 0;
}

static int i2c_driver_deinit(const struct device_s *dev)
{
	struct i2c_data_s *data;
	
	data = (struct i2c_data_s *)dev->data;
	
	data->init = 0;

	printf("I2C: %s, i2c_deinit()\n", dev->name);
	
	return 0;
}

static int i2c_driver_open(const struct device_s *dev, int mode)
{
	struct i2c_data_s *data;
	int retval = 0, val;
	
	data = (struct i2c_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	CRITICAL_ENTER();
	if (!data->busy)
		data->busy = 1;
	else
		retval = -1;
	CRITICAL_LEAVE();
	
	if (mode)
		i2c_reset_bus(dev);
	
	val = i2c_start(dev);
	if (val < 0)
		retval = -1;

	return retval;
}

static int i2c_driver_close(const struct device_s *dev)
{
	struct i2c_config_s *config;
	struct i2c_data_s *data;
	
	config = (struct i2c_config_s *)dev->config;
	data = (struct i2c_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	i2c_stop(dev, 1);
	_delay_us(config->sig_delay);

	CRITICAL_ENTER();
	data->busy = 0;
	CRITICAL_LEAVE();
	
	return 0;
}

static size_t i2c_driver_read(const struct device_s *dev, void *buf, size_t count)
{
	struct i2c_data_s *data;
	char *p;
	int i, val = 0;
	
	data = (struct i2c_data_s *)dev->data;
	p = (char *)buf;
	
	if (!data->init)
		return -1;

	NOSCHED_ENTER();
	if (!count)
		val = i2c_restart(dev, 1);
	
	for (i = 0; i < count; i++) {
		if (val < 0) break;
		// ack on all reads except the last one.
		val = i < count - 1 ? i2c_read_byte(dev, 0) : i2c_read_byte(dev, 1);
		p[i] = val;
	}
	NOSCHED_LEAVE();
	
	return i;
}

static size_t i2c_driver_write(const struct device_s *dev, void *buf, size_t count)
{
	struct i2c_data_s *data;
	char *p;
	int i, val = 0;
	
	data = (struct i2c_data_s *)dev->data;
	p = (char *)buf;
	
	if (!data->init)
		return -1;
		
	NOSCHED_ENTER();
	if (!count)
		val = i2c_restart(dev, 1);
	
	for (i = 0; i < count; i++) {
		if (val < 0) break;
		val = i2c_write_byte(dev, p[i]);
	}
	NOSCHED_LEAVE();
	
	return i;
}

/* device driver function mapping for generic API */
struct device_api_s i2c_api = {
	.dev_init = i2c_driver_init,
	.dev_deinit = i2c_driver_deinit,
	.dev_open = i2c_driver_open,
	.dev_close = i2c_driver_close,
	.dev_read = i2c_driver_read,
	.dev_write = i2c_driver_write
};
