#include <ucx.h>
#include <device.h>
#include <spi_bitbang.h>

/* SPI (bit bang) API function wrappers */
int spi_init(const struct device_s *dev)
{
	return dev->api->dev_init(dev);
}

int spi_deinit(const struct device_s *dev)
{
	return dev->api->dev_deinit(dev);
}

int spi_open(const struct device_s *dev, int mode)
{
	return dev->api->dev_open(dev, mode);
}

int spi_close(const struct device_s *dev)
{
	return dev->api->dev_close(dev);
}

size_t spi_read(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_read(dev, buf, count);
}

size_t spi_write(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_write(dev, buf, count);
}

/* SPI (bit bang) device driver implementation */
static int spi_driver_init(const struct device_s *dev)
{
	struct spi_config_s *config;
	struct spi_data_s *data;
	int val;
	
	config = (struct spi_config_s *)dev->config;
	data = (struct spi_data_s *)dev->data;

	data->init = 0;
	data->busy = 0;
	
	val = config->gpio_configpins();
	if (val < 0)
		return val;
	
	config->gpio_cs(config->cs_active ^ SPI_CS_HIGH);
	if (config->spi_mode == SPI_MODE0 || config->spi_mode == SPI_MODE1)
		config->gpio_sck(0);
	else
		config->gpio_sck(1);
	
	if (config->device_mode == SPI_MASTER)
		config->gpio_mosi(1);
	else
		config->gpio_miso(1);
		
	data->init = 1;
	
	printf("SPI: %s, spi_init()\n", dev->name);
	
	return 0;
}

static int spi_driver_deinit(const struct device_s *dev)
{
	struct spi_data_s *data;
	
	data = (struct spi_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	printf("SPI: %s, spi_deinit()\n", dev->name);
	
	return 0;
}

static int spi_driver_open(const struct device_s *dev, int mode)
{
	struct spi_config_s *config;
	struct spi_data_s *data;
	int retval = 0;
	
	config = (struct spi_config_s *)dev->config;
	data = (struct spi_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	CRITICAL_ENTER();
	if (!data->busy)
		data->busy = 1;
	else
		retval = -1;
	CRITICAL_LEAVE();

	if (!retval && config->device_mode == SPI_MASTER) {
		config->gpio_cs(config->cs_active);
		_delay_us(config->cs_delay);
	}
	
	return retval;
}

static int spi_driver_close(const struct device_s *dev)
{
	struct spi_config_s *config;
	struct spi_data_s *data;
	
	config = (struct spi_config_s *)dev->config;
	data = (struct spi_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	CRITICAL_ENTER();
	data->busy = 0;
	CRITICAL_LEAVE();

	if (config->device_mode == SPI_MASTER) {
		config->gpio_cs(config->cs_active ^ SPI_CS_HIGH);
		_delay_us(config->cs_delay);
		config->gpio_mosi(1);
	} else {
		config->gpio_miso(1);
	}
	
	return 0;
}

static char spi_master_transfer(const struct device_s *dev, char data)
{
	struct spi_config_s *config;
	int i;
	char newdata = 0;

	config = (struct spi_config_s *)dev->config;
	
	if (config->spi_mode == SPI_MODE0 || config->spi_mode == SPI_MODE2) {
		for (i = 0; i < 8; i++) {
			// write MOSI
			config->gpio_mosi((data & config->bit_order) ? 1 : 0);
			data = config->bit_order == SPI_LSB ? data >> 1 : data << 1;
			_delay_us(config->sig_delay);
						
			// update SCK
			config->gpio_sck(config->spi_mode == SPI_MODE0 ? 1 : 0);
			_delay_us(config->sig_delay);
			
			// read MISO
			newdata = config->bit_order == SPI_LSB ? newdata >> 1 : newdata << 1;
			if (config->gpio_miso(-1))
				newdata |= config->bit_order == SPI_LSB ? 0x80 : 0x01;
			
			// update SCK
			config->gpio_sck(config->spi_mode == SPI_MODE0 ? 0 : 1);
		}
	} else {
		for (i = 0; i < 8; i++) {
			// update SCK
			config->gpio_sck(config->spi_mode == SPI_MODE1 ? 1 : 0);
			
			// write MOSI
			config->gpio_mosi((data & config->bit_order) ? 1 : 0);
			data = config->bit_order == SPI_LSB ? data >> 1 : data << 1;
			_delay_us(config->sig_delay);
			
			// update SCK
			config->gpio_sck(config->spi_mode == SPI_MODE1 ? 0 : 1);
			_delay_us(config->sig_delay);
			
			// read MISO
			newdata = config->bit_order == SPI_LSB ? newdata >> 1 : newdata << 1;
			if (config->gpio_miso(-1))
				newdata |= config->bit_order == SPI_LSB ? 0x80 : 0x01;
		}
	}
	
	return newdata;
}

static char spi_slave_transfer(const struct device_s *dev, char data)
{
	struct spi_config_s *config;
	int i;
	char newdata = 0;

	config = (struct spi_config_s *)dev->config;
	
	if (config->spi_mode == SPI_MODE0 || config->spi_mode == SPI_MODE2) {
		for (i = 0; i < 8; i++) {
			// wait SCK
			while (config->gpio_sck(0) != (config->spi_mode == SPI_MODE0 ? 1 : 0))
				if (config->gpio_cs(0) != config->cs_active) break;
			
			// read MOSI
			newdata = config->bit_order == SPI_LSB ? newdata >> 1 : newdata << 1;
			if (config->gpio_mosi(-1))
				newdata |= config->bit_order == SPI_LSB ? 0x80 : 0x01;
			
			// write MISO
			config->gpio_miso((data & config->bit_order) ? 1 : 0);
			data = config->bit_order == SPI_LSB ? data >> 1 : data << 1;
			
			// wait SCK
			while (config->gpio_sck(0) != (config->spi_mode == SPI_MODE0 ? 0 : 1))
				if (config->gpio_cs(0) != config->cs_active) break;
		}
	} else {
		for (i = 0; i < 8; i++) {
			// wait SCK
			while (config->gpio_sck(0) == (config->spi_mode == SPI_MODE1 ? 1 : 0))
				if (config->gpio_cs(0) != config->cs_active) break;
			
			// read MOSI
			newdata = config->bit_order == SPI_LSB ? newdata >> 1 : newdata << 1;
			if (config->gpio_mosi(-1))
				newdata |= config->bit_order == SPI_LSB ? 0x80 : 0x01;

			// write MISO
			config->gpio_miso((data & config->bit_order) ? 1 : 0);
			data = config->bit_order == SPI_LSB ? data >> 1 : data << 1;
			
			// wait SCK
			while (config->gpio_sck(0) == (config->spi_mode == SPI_MODE1 ? 0 : 1))
				if (config->gpio_cs(0) != config->cs_active) break;
		}
	}
	
	return newdata;
}

static size_t spi_driver_read(const struct device_s *dev, void *buf, size_t count)
{
	struct spi_config_s *config;
	struct spi_data_s *data;
	char *p;
	int i;
	
	config = (struct spi_config_s *)dev->config;
	data = (struct spi_data_s *)dev->data;
	p = (char *)buf;
	
	if (!data->init)
		return -1;
		
	if (config->device_mode == SPI_MASTER) {
		NOSCHED_ENTER();
		for (i = 0; i < count; i++)
			p[i] = spi_master_transfer(dev, 0x00);
		NOSCHED_LEAVE();
	} else {
		if (config->gpio_cs(0) != config->cs_active)
			return 0;
		
		NOSCHED_ENTER();
		for (i = 0; i < count; i++) {
			p[i] = spi_slave_transfer(dev, 0x00);
			if (config->gpio_cs(0) != config->cs_active) break;
		}
		NOSCHED_LEAVE();
		
		//while (config->gpio_cs(0) == config->cs_active);

		return i;
	}
	
	return count;
}

static size_t spi_driver_write(const struct device_s *dev, void *buf, size_t count)
{
	struct spi_config_s *config;
	struct spi_data_s *data;
	char *p;
	char newdata;
	int i;
	
	config = (struct spi_config_s *)dev->config;
	data = (struct spi_data_s *)dev->data;
	p = (char *)buf;
	
	if (!data->init)
		return -1;
		
	if (config->device_mode == SPI_MASTER) {
		NOSCHED_ENTER();
		for (i = 0; i < count; i++) {
			newdata = spi_master_transfer(dev, p[i]);
			if (config->duplex_mode == SPI_FULL_DUPLEX)
				p[i] = newdata;
		}
		NOSCHED_LEAVE();
	} else {
		if (config->gpio_cs(0) != config->cs_active)
			return 0;
		
		NOSCHED_ENTER();
		for (i = 0; i < count; i++) {
			newdata = spi_slave_transfer(dev, p[i]);
			if (config->gpio_cs(0) != config->cs_active) break;

			if (config->duplex_mode == SPI_FULL_DUPLEX)
				p[i] = newdata;
		}
		NOSCHED_LEAVE();
		
		//while (config->gpio_cs(0) == config->cs_active);
		
		return i;
	}
	
	return count;
}

/* device driver function mapping for generic API */
struct device_api_s spi_api = {
	.dev_init = spi_driver_init,
	.dev_deinit = spi_driver_deinit,
	.dev_open = spi_driver_open,
	.dev_close = spi_driver_close,
	.dev_read = spi_driver_read,
	.dev_write = spi_driver_write
};
