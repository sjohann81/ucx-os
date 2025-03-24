/* file:          25lcxxx.c
 * description:   25lcxxx SPI EEPROM chip driver
 *                EEPROMs compatible with the Microchip 25lcxx family.
 *                Address and page size varies according to EEPROM chip in use:
 *                25xx010, 25xx020, 25xx040: 1 byte address, 16 bytes page
 *                25xx080, 25xx160: 2 bytes address, 16 bytes page
 *                25xx320, 25xx640: 2 bytes address, 32 bytes page
 *                25xx128, 25xx256: 2 bytes address, 64 bytes page
 *                25xx512: 2 bytes address, 128 bytes page
 *                25xx1024: 3 bytes address, 256 bytes page
 */

#include <ucx.h>
#include <device.h>
#include <25lcxxx.h>


/* 25lcxxx API function wrappers */
int eeprom_25lc_init(const struct device_s *dev)
{
	return dev->api->dev_init(dev);
}

int eeprom_25lc_deinit(const struct device_s *dev)
{
	return dev->api->dev_deinit(dev);
}

int eeprom_25lc_open(const struct device_s *dev, int mode)
{
	return dev->api->dev_open(dev, mode);
}

int eeprom_25lc_close(const struct device_s *dev)
{
	return dev->api->dev_close(dev);
}

size_t eeprom_25lc_read(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_read(dev, buf, count);
}

size_t eeprom_25lc_write(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_write(dev, buf, count);
}

int eeprom_25lc_seek(const struct device_s *dev, long pos, int whence)
{
	return dev->api->dev_seek(dev, pos, whence);
}


/* 25lcxxx device driver implementation */

static int eeprom_25lc_driver_init(const struct device_s *dev)
{
	struct eeprom_25lc_config_s *config;
	struct eeprom_25lc_data_s *data;
	
	config = (struct eeprom_25lc_config_s *)dev->config;
	data = (struct eeprom_25lc_data_s *)dev->data;

	data->init = 0;
	data->busy = 0;
	data->addr_size = 0;
	data->page_size = 0;
	data->pos = 0;
	
	// configure EEPROM address bytes and page size
	// some chips share the same configuration
	switch (config->config_values.type) {
	case EEPROM_25LC010:
	case EEPROM_25LC020:
	case EEPROM_25LC040: data->addr_size = 1; data->page_size = 16; break;
	case EEPROM_25LC080:
	case EEPROM_25LC160: data->addr_size = 2; data->page_size = 16; break;
	case EEPROM_25LC320:
	case EEPROM_25LC640: data->addr_size = 2; data->page_size = 32; break;
	case EEPROM_25LC128:
	case EEPROM_25LC256: data->addr_size = 2; data->page_size = 64; break;
	case EEPROM_25LC512: data->addr_size = 2; data->page_size = 128; break;
	case EEPROM_25LC1024: data->addr_size = 3; data->page_size = 256; break;
	default:
		return -1;
	};

	data->init = 1;
	
	printf("EEPROM: %s, eeprom_25lc_init()\n", dev->name);
	
	return 0;
}

static int eeprom_25lc_driver_deinit(const struct device_s *dev)
{
	struct eeprom_25lc_data_s *data;

	data = (struct eeprom_25lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;
	
	data->init = 0;
	data->busy = 0;
	data->addr_size = 0;
	data->page_size = 0;
	data->pos = 0;
	
	printf("EEPROM: %s, eeprom_25lc_deinit()\n", dev->name);

	return 0;
}

static int eeprom_25lc_driver_open(const struct device_s *dev, int mode)
{
	struct eeprom_25lc_data_s *data;
	int retval = 0;
	
	data = (struct eeprom_25lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	CRITICAL_ENTER();
	if (!data->busy)
		data->busy = 1;
	else
		retval = -1;
	CRITICAL_LEAVE();

	return retval;
}

static int eeprom_25lc_driver_close(const struct device_s *dev)
{
	struct eeprom_25lc_data_s *data;
	
	data = (struct eeprom_25lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	CRITICAL_ENTER();
	data->busy = 0;
	CRITICAL_LEAVE();

	return 0;
}

static int eeprom_25lc_select(const struct device_s *dev, uint8_t *cmd, uint8_t *cmd_size)
{
	struct eeprom_25lc_config_s *config;
	struct eeprom_25lc_data_s *data;
	
	config = (struct eeprom_25lc_config_s *)dev->config;
	data = (struct eeprom_25lc_data_s *)dev->data;
	
	// select EEPROM configuration
	switch (config->config_values.type) {
	case EEPROM_25LC010:
	case EEPROM_25LC020: *cmd_size = 2; break;
	case EEPROM_25LC040: *cmd |= ((data->pos >> 8) & 0x01) << 3; *cmd_size = 2; break;
	case EEPROM_25LC080:
	case EEPROM_25LC160:
	case EEPROM_25LC320:
	case EEPROM_25LC640:
	case EEPROM_25LC128:
	case EEPROM_25LC256:
	case EEPROM_25LC512: *cmd_size = 3; break;
	case EEPROM_25LC1024: *cmd_size = 4; break;
	default:
		return -1;
	};
	
	return 0;
}

static size_t eeprom_25lc_driver_read(const struct device_s *dev, void *buf, size_t count)
{
	struct eeprom_25lc_config_s *config;
	struct eeprom_25lc_data_s *data;	
	uint8_t cmd_size;
	uint8_t cmd[4];
	size_t size;
	
	config = (struct eeprom_25lc_config_s *)dev->config;
	data = (struct eeprom_25lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;
		
	memset(cmd, 0, 4);

	if (eeprom_25lc_select(dev, &cmd[0], &cmd_size) < 0)
		return -1;
	
	cmd[0] &= 0x08;
	cmd[0] |= READ;
	
	switch (cmd_size) {
	case 2:
		cmd[1] = data->pos & 0x00ff;
		break;
	case 3:
		cmd[1] = (data->pos & 0xff00) >> 8;
		cmd[2] = data->pos & 0x00ff;
		break;
	case 4:
		cmd[1] = (data->pos & 0xff0000) >> 16;
		cmd[2] = (data->pos & 0xff00) >> 8;
		cmd[3] = data->pos & 0x00ff;
		break;
	default:
		return 0;
	};
	
	if (dev_open(config->spi_dev, 0) < 0)
		return 0;
	
	// select peripheral and write memory address
	dev_write(config->spi_dev, cmd, cmd_size);
	
	// read data
	size = dev_read(config->spi_dev, buf, count);
	data->pos += size;
	
	dev_close(config->spi_dev);
	
	return size;
}

static size_t eeprom_25lc_driver_write(const struct device_s *dev, void *buf, size_t count)
{
	struct eeprom_25lc_config_s *config;
	struct eeprom_25lc_data_s *data;	
	uint8_t cmd_size;
	uint8_t cmd[4];
	size_t size = 0;
	
	unsigned page_space;
	unsigned num_writes;
	unsigned first_write_size;
	unsigned last_write_size = 0;
	unsigned write_size;
	
	config = (struct eeprom_25lc_config_s *)dev->config;
	data = (struct eeprom_25lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	memset(cmd, 0, 4);

	if (eeprom_25lc_select(dev, &cmd[0], &cmd_size) < 0)
		return -1;
	
	// calculate space available in first page
	page_space = (((data->pos / data->page_size) + 1) * data->page_size) - data->pos;
	
	// calculate first write size
	first_write_size = page_space - ((page_space / data->page_size) * data->page_size);
	if (first_write_size == 0) first_write_size = data->page_size;
	
	// calculate size of last write and number of writes
	if (count > first_write_size) {
		last_write_size = (count - first_write_size) % data->page_size;
		num_writes = ((count - first_write_size) / data->page_size) + 2;
	} else {
		last_write_size = count;
		num_writes = 1;
	}
	
	// write page data - first and last page may only have partial data
	// SPI device must be opened and closed multiple times, as chip select
	// and deselect sequences happen on dev_open() and dev_close() operations
	for (int page = 0; page < num_writes; page++) {
		write_size = data->page_size;
		
		if (page == 0)
			write_size = first_write_size;
		if (page == (num_writes - 1))
			write_size = last_write_size;
		
		cmd[0] &= 0x08;
		cmd[0] |= WREN;
		
		// open SPI EEPROM
		if (dev_open(config->spi_dev, 0) < 0)
			return 0;
	
		// disable write latch
		dev_write(config->spi_dev, cmd, 1);
	
		// close SPI EEPROM, write enabled
		dev_close(config->spi_dev);
		
		cmd[0] &= 0x08;
		cmd[0] |= WRITE;
		
		switch (cmd_size) {
		case 2:
			cmd[1] = data->pos & 0x00ff;
			break;
		case 3:
			cmd[1] = (data->pos & 0xff00) >> 8;
			cmd[2] = data->pos & 0x00ff;
			break;
		case 4:
			cmd[1] = (data->pos & 0xff0000) >> 16;
			cmd[2] = (data->pos & 0xff00) >> 8;
			cmd[3] = data->pos & 0x00ff;
			break;
		default:
			return 0;
		};
	
		// open SPI EEPROM for write operation
		if (dev_open(config->spi_dev, 0) < 0)
			return 0;
	
		// select peripheral and write memory address
		dev_write(config->spi_dev, cmd, cmd_size);
	
		// write data
		size += dev_write(config->spi_dev, buf + size, write_size);

		data->pos += write_size;
		
		// close SPI EEPROM and wait for write completion
		dev_close(config->spi_dev);
		_delay_ms(6);
	}
	
	return size;
}

static int eeprom_25lc_driver_seek(const struct device_s *dev, long pos, int whence)
{
	struct eeprom_25lc_data_s *data;	

	data = (struct eeprom_25lc_data_s *)dev->data;
	
	// seek to absolute (SEEK_SET) or relative (SEEK_CUR) position
	switch (whence) {
	case SEEK_SET: data->pos = pos < 0 ? 0 : pos; break;
	case SEEK_CUR: data->pos = data->pos + pos < 0 ? 0 : data->pos + pos; break;
	case SEEK_END:
	default:
		return 0;
	}
	
	return data->pos;
}


/* device driver function mapping for generic API */
struct device_api_s eeprom_25lc_api = {
	.dev_init = eeprom_25lc_driver_init,
	.dev_deinit = eeprom_25lc_driver_deinit,
	.dev_open = eeprom_25lc_driver_open,
	.dev_close = eeprom_25lc_driver_close,
	.dev_read = eeprom_25lc_driver_read,
	.dev_write = eeprom_25lc_driver_write,
	.dev_seek = eeprom_25lc_driver_seek
};
