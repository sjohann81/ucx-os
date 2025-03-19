#include <ucx.h>
#include <device.h>
#include <i2c.h>
#include <i2c_ll.h>

/* I2C API function wrappers */
int i2c_hw_init(const struct device_s *dev)
{
	return dev->api->dev_init(dev);
}

int i2c_hw_deinit(const struct device_s *dev)
{
	return dev->api->dev_deinit(dev);
}

int i2c_hw_open(const struct device_s *dev, int mode)
{
	return dev->api->dev_open(dev, mode);
}

int i2c_hw_close(const struct device_s *dev)
{
	return dev->api->dev_close(dev);
}

size_t i2c_hw_read(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_read(dev, buf, count);
}

size_t i2c_hw_write(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_write(dev, buf, count);
}

/* I2C device driver implementation */
static int i2c_hw_driver_init(const struct device_s *dev)
{
	struct i2c_hw_config_s *config;
	struct i2c_hw_data_s *data;
	
	config = (struct i2c_hw_config_s *)dev->config;
	data = (struct i2c_hw_data_s *)dev->data;

	data->init = 0;
	data->busy = 0;

	i2c_ll_init(&config->config_values);
	data->init = 1;
	
	printf("I2C: %s, i2c_hw_init()\n", dev->name);
	
	return 0;
}

static int i2c_hw_driver_deinit(const struct device_s *dev)
{
	struct i2c_hw_data_s *data;
	
	data = (struct i2c_hw_data_s *)dev->data;
	
	if (!data->init)
		return -1;
	
	data->init = 0;

	printf("I2C: %s, i2c_deinit()\n", dev->name);
	
	return 0;
}

static int i2c_hw_driver_open(const struct device_s *dev, int mode)
{
	struct i2c_hw_config_s *config;
	struct i2c_hw_data_s *data;
	int retval = 0, val;
	
	config = (struct i2c_hw_config_s *)dev->config;
	data = (struct i2c_hw_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	CRITICAL_ENTER();
	if (!data->busy)
		data->busy = 1;
	else
		retval = -1;
	CRITICAL_LEAVE();
	
	val = i2c_ll_start(&config->config_values);
	if (val < 0)
		retval = -1;

	return retval;
}

static int i2c_hw_driver_close(const struct device_s *dev)
{
	struct i2c_hw_config_s *config;
	struct i2c_hw_data_s *data;
	
	config = (struct i2c_hw_config_s *)dev->config;
	data = (struct i2c_hw_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	i2c_ll_stop(&config->config_values);

	CRITICAL_ENTER();
	data->busy = 0;
	CRITICAL_LEAVE();

	return 0;
}

static size_t i2c_hw_driver_read(const struct device_s *dev, void *buf, size_t count)
{
	struct i2c_hw_config_s *config;
	struct i2c_hw_data_s *data;
	char *p;
	int i, val = 0;
	
	config = (struct i2c_hw_config_s *)dev->config;
	data = (struct i2c_hw_data_s *)dev->data;
	p = (char *)buf;
	
	if (!data->init)
		return -1;

	NOSCHED_ENTER();
	if (!count)
		val = i2c_ll_restart(&config->config_values);
	
	for (i = 0; i < count; i++) {
		if (val < 0) break;
		// ack on all reads except the last one.
		val = i < count - 1 ? i2c_ll_read(&config->config_values, 0) : 
			i2c_ll_read(&config->config_values, 1);
		p[i] = val;
	}
	NOSCHED_LEAVE();
	
	return i;
}

static size_t i2c_hw_driver_write(const struct device_s *dev, void *buf, size_t count)
{
	struct i2c_hw_config_s *config;
	struct i2c_hw_data_s *data;
	char *p;
	int i, val = 0;

	config = (struct i2c_hw_config_s *)dev->config;	
	data = (struct i2c_hw_data_s *)dev->data;
	p = (char *)buf;
	
	if (!data->init)
		return -1;
		
	NOSCHED_ENTER();
	if (!count) {
		val = i2c_ll_restart(&config->config_values);
		NOSCHED_LEAVE();
		
		return 0;
	}
	
	if (config->config_values.addr_mode == I2C_ADDR10BIT) {
		if (p[0] & 1)
			val = i2c_ll_rd_addr(&config->config_values, (p[0] << 8) | p[1]);
		else
			val = i2c_ll_wr_addr(&config->config_values, (p[0] << 8) | p[1]);
		i = 2;
	} else {
		if (p[0] & 1)
			val = i2c_ll_rd_addr(&config->config_values, p[0]);
		else
			val = i2c_ll_wr_addr(&config->config_values, p[0]);
		i = 1;
	}

	for (; i < count; i++) {
		if (val < 0) break;
		val = i2c_ll_write(&config->config_values, p[i]);
	}
	NOSCHED_LEAVE();
	
	return i;
}

/* device driver function mapping for generic API */
struct device_api_s i2c_hw_api = {
	.dev_init = i2c_hw_driver_init,
	.dev_deinit = i2c_hw_driver_deinit,
	.dev_open = i2c_hw_driver_open,
	.dev_close = i2c_hw_driver_close,
	.dev_read = i2c_hw_driver_read,
	.dev_write = i2c_hw_driver_write
};
