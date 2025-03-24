/* file:          24lcxxx.c
 * description:   24lcxxx I2C EEPROM chip driver
 *                EEPROMs compatible with the Microchip 24lcxx family.
 *                Address and page size varies according to EEPROM chip in use:
 *                24xx01, 24xx02: 1 byte address, 8 bytes page
 *                24xx04, 24xx08, 24xx16: 1 byte address, 16 bytes page
 *                24xx32, 24xx64: 2 bytes address, 32 bytes page
 *                24xx128, 24xx256: 2 bytes address, 64 bytes page
 *                24xx512, 24xx1024/1025: 2 bytes address, 128 bytes page
 */

#include <ucx.h>
#include <device.h>
#include <24lcxxx.h>


/* 24lcxxx API function wrappers */
int eeprom_24lc_init(const struct device_s *dev)
{
	return dev->api->dev_init(dev);
}

int eeprom_24lc_deinit(const struct device_s *dev)
{
	return dev->api->dev_deinit(dev);
}

int eeprom_24lc_open(const struct device_s *dev, int mode)
{
	return dev->api->dev_open(dev, mode);
}

int eeprom_24lc_close(const struct device_s *dev)
{
	return dev->api->dev_close(dev);
}

size_t eeprom_24lc_read(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_read(dev, buf, count);
}

size_t eeprom_24lc_write(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_write(dev, buf, count);
}

int eeprom_24lc_seek(const struct device_s *dev, long pos, int whence)
{
	return dev->api->dev_seek(dev, pos, whence);
}


/* 24lcxxx device driver implementation */

static int eeprom_24lc_driver_init(const struct device_s *dev)
{
	struct eeprom_24lc_config_s *config;
	struct eeprom_24lc_data_s *data;
	
	config = (struct eeprom_24lc_config_s *)dev->config;
	data = (struct eeprom_24lc_data_s *)dev->data;

	data->init = 0;
	data->busy = 0;
	data->addr_size = 0;
	data->page_size = 0;
	data->pos = 0;
	
	// configure EEPROM address bytes and page size
	// some chips share the same configuration
	switch (config->config_values.type) {
	case EEPROM_24LC01:
	case EEPROM_24LC02: data->addr_size = 1; data->page_size = 8; break;
	case EEPROM_24LC04:
	case EEPROM_24LC08:
	case EEPROM_24LC16: data->addr_size = 1; data->page_size = 16; break;
	case EEPROM_24LC32:
	case EEPROM_24LC64: data->addr_size = 2; data->page_size = 32; break;
	case EEPROM_24LC128:
	case EEPROM_24LC256: data->addr_size = 2; data->page_size = 64; break;
	case EEPROM_24LC512:
	case EEPROM_24LC1024: data->addr_size = 2; data->page_size = 128; break;
	default:
		return -1;
	};

	data->init = 1;
	
	printf("EEPROM: %s, eeprom_24lc_init()\n", dev->name);
	
	return 0;
}

static int eeprom_24lc_driver_deinit(const struct device_s *dev)
{
	struct eeprom_24lc_data_s *data;

	data = (struct eeprom_24lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;
	
	data->init = 0;
	data->busy = 0;
	data->addr_size = 0;
	data->page_size = 0;
	data->pos = 0;
	
	printf("EEPROM: %s, eeprom_24lc_deinit()\n", dev->name);

	return 0;
}

static int eeprom_24lc_driver_open(const struct device_s *dev, int mode)
{
	struct eeprom_24lc_data_s *data;
	int retval = 0;
	
	data = (struct eeprom_24lc_data_s *)dev->data;
	
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

static int eeprom_24lc_driver_close(const struct device_s *dev)
{
	struct eeprom_24lc_data_s *data;
	
	data = (struct eeprom_24lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	CRITICAL_ENTER();
	data->busy = 0;
	CRITICAL_LEAVE();

	return 0;
}

static int eeprom_24lc_select(const struct device_s *dev, uint8_t *device, uint8_t *cmd_size)
{
	struct eeprom_24lc_config_s *config;
	struct eeprom_24lc_data_s *data;
	
	config = (struct eeprom_24lc_config_s *)dev->config;
	data = (struct eeprom_24lc_data_s *)dev->data;
	
	// select EEPROM configuration
	switch (config->config_values.type) {
	case EEPROM_24LC01:
	case EEPROM_24LC02:
		*device = 0x00;
		*cmd_size = 2;
		break;
	case EEPROM_24LC04:
	case EEPROM_24LC08:
	case EEPROM_24LC16:
		*device = (data->pos >> 8) & 0x07;
		*cmd_size = 2;
		break;
	case EEPROM_24LC32:
	case EEPROM_24LC64:
	case EEPROM_24LC128:
	case EEPROM_24LC256:
	case EEPROM_24LC512:
		*device = config->config_values.select & 0x07;
		*cmd_size = 3;
		break;
	case EEPROM_24LC1024:
		*device = ((data->pos >> 14) & 0x04) | 
			(config->config_values.select & 0x03);
		*cmd_size = 3;
		break;
	default:
		return -1;
	};
	
	return 0;
}

static size_t eeprom_24lc_driver_read(const struct device_s *dev, void *buf, size_t count)
{
	struct eeprom_24lc_config_s *config;
	struct eeprom_24lc_data_s *data;	
	uint8_t device, cmd_size;
	uint8_t cmd[3], byte = 0;
	size_t size;
	
	config = (struct eeprom_24lc_config_s *)dev->config;
	data = (struct eeprom_24lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	if (eeprom_24lc_select(dev, &device, &cmd_size) < 0)
		return -1;
	
	byte = 0xA0 | (device << 1);
	cmd[0] = byte;
	
	cmd[1] = (data->pos & 0xff00) >> 8;
	cmd[2] = data->pos & 0x00ff;
	
	if (cmd_size == 2)
		cmd[1] = cmd[2];
	
	if (dev_open(config->i2c_dev, 0) < 0)
		return 0;
	
	// select peripheral and write memory address
	dev_write(config->i2c_dev, cmd, cmd_size);

	byte = 0xA1 | (device << 1);
	cmd[0] = byte;
	
	// restart (write size zero)
	dev_write(config->i2c_dev, cmd, 0);
	
	// select peripheral for reading
	dev_write(config->i2c_dev, cmd, 1);
	
	// read data
	size = dev_read(config->i2c_dev, buf, count);
	data->pos += size;
	
	dev_close(config->i2c_dev);
	
	return size;
}

static size_t eeprom_24lc_driver_write(const struct device_s *dev, void *buf, size_t count)
{
	struct eeprom_24lc_config_s *config;
	struct eeprom_24lc_data_s *data;	
	uint8_t device, cmd_size;
	uint8_t cmd[3], byte = 0;
	size_t size = 0;
	
	unsigned page_space;
	unsigned num_writes;
	unsigned first_write_size;
	unsigned last_write_size = 0;
	unsigned write_size;
	
	config = (struct eeprom_24lc_config_s *)dev->config;
	data = (struct eeprom_24lc_data_s *)dev->data;
	
	if (!data->init)
		return -1;

	if (eeprom_24lc_select(dev, &device, &cmd_size) < 0)
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
	// I2C device must be opened and closed for each write, as start
	// and stop sequences happen on dev_open() and dev_close() operations
	for (int page = 0; page < num_writes; page++) {
		write_size = data->page_size;
		
		if (page == 0)
			write_size = first_write_size;
		if (page == (num_writes - 1))
			write_size = last_write_size;
		
		byte = 0xA0 | (device << 1);
		cmd[0] = byte;
		cmd[1] = (data->pos & 0xff00) >> 8;
		cmd[2] = data->pos & 0x00ff;
	
		if (cmd_size == 2)
			cmd[1] = cmd[2];
	
		// open I2C EEPROM
		if (dev_open(config->i2c_dev, 0) < 0)
			return 0;
	
		// select peripheral and write memory address
		dev_write(config->i2c_dev, cmd, cmd_size);
	
		// write data
		size += dev_write(config->i2c_dev, buf + size, write_size);

		data->pos += write_size;
		
		// close I2C EEPROM and wait for write completion
		dev_close(config->i2c_dev);
		_delay_ms(5);
	}
	
	return size;
}

static int eeprom_24lc_driver_seek(const struct device_s *dev, long pos, int whence)
{
	struct eeprom_24lc_data_s *data;	

	data = (struct eeprom_24lc_data_s *)dev->data;
	
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
struct device_api_s eeprom_24lc_api = {
	.dev_init = eeprom_24lc_driver_init,
	.dev_deinit = eeprom_24lc_driver_deinit,
	.dev_open = eeprom_24lc_driver_open,
	.dev_close = eeprom_24lc_driver_close,
	.dev_read = eeprom_24lc_driver_read,
	.dev_write = eeprom_24lc_driver_write,
	.dev_seek = eeprom_24lc_driver_seek
};
