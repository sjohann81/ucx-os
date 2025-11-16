#include <ucx.h>
#include <device.h>
#include <dht.h>

static int dht_poll(const struct device_s *dev)
{
	struct dht_config_s *config;
	
	config = (struct dht_config_s *)dev->config;
	
	return config->gpio_sdata(-1);
}

static int dht_wait(const struct device_s *dev, uint8_t logic, uint16_t timeout_us, uint16_t mult_us)
{
	int time = 0;
	
	/* wait for logic level to change on pin */
	while (dht_poll(dev) == logic) {
		_delay_us(1);
		if (++time > timeout_us * mult_us)
			return -1;
	}
	
	return 0;
}

static void dht_start(const struct device_s *dev)
{
	struct dht_config_s *config;
	
	config = (struct dht_config_s *)dev->config;	
	
	config->gpio_sdata(0);
	_delay_us(DHT_STARTLOW);
	config->gpio_sdata(1);
	_delay_us(DHT_STARTHIGH);
}

static int dht_read_byte(const struct device_s *dev)
{
	int i;
	uint8_t byte = 0;
	
	for (i = 0; i < 8; i++) {
		/* wait for the start of bit transmission (twice the time for tolerance) */
		if (dht_wait(dev, 0, (DHT_STARTXMIT << 1), 1))
			return ERR_TIMEOUT3;

		_delay_us(DHT_DATAZERO);
		
		/* sample data */
		if (dht_poll(dev))
			byte |= (1 << (7 - i));
			
		/* wait for the end of bit transmission (twice the time for tolerance) */
		if (dht_wait(dev, 1, (DHT_DATAONE << 1), 1))
			return ERR_TIMEOUT4;
	}
	
	return byte;
}

static int dht_read_data(const struct device_s *dev)
{
	struct dht_data_s *sensor;
	int i, val;
	
	sensor = (struct dht_data_s *)dev->data;
	
	dht_start(dev);
	
	/* wait for the sensor to wake up after a start */
	if (dht_wait(dev, 1, DHT_TIMEOUTMS, 1000))
		return ERR_TIMEOUT1;
	
	/* if the line is low, sensor is up */
	if (!dht_poll(dev)) {
		_delay_us(DHT_INITLOW);

		/* wait for sync (high) */
		if (dht_wait(dev, 0, DHT_INITLOW, 1))
			return ERR_TIMEOUT2;
		
		_delay_us(DHT_INITHIGH);

		/* wait for sync (low) */
		if (dht_wait(dev, 1, DHT_INITHIGH, 1))
			return ERR_TIMEOUT2;

		/* in sync, fetch data */
		for (i = 0; i < 5; i++) {
			val = dht_read_byte(dev);
			
			if (val < 0)
				return val;
				
			sensor->data[i] = val;
		}
		
		if (((sensor->data[0] + sensor->data[1] + 
		sensor->data[2] + sensor->data[3]) & 0xff) == 
		sensor->data[4])
			return ERR_NOERROR;
		else
			return ERR_CHECKSUM;
	}
	
	return ERR_TIMEOUT1;
}

static int dht_read_sensor(const struct device_s *dev)
{
	struct dht_data_s *sensor;
	struct dht_config_s *config;
	int val;
	
	sensor = (struct dht_data_s *)dev->data;
	config = (struct dht_config_s *)dev->config;
	
	NOSCHED_ENTER();
	val = dht_read_data(dev);
		
	if (val != ERR_NOERROR) {
		NOSCHED_LEAVE();
		
		return val;
	}

	switch (config->type) {
	case DHT11:
		val = (int)sensor->data[2];
		val = sensor->data[3] & 0x80 ? -val - 1 : val;
		val = val * 10;
		val += (int)(sensor->data[3] & 0x0f);
		sensor->temperature = val;
		sensor->humidity = ((int)sensor->data[0] * 10) + (int)sensor->data[1];
		break;
	case DHT12:
		val = (int)sensor->data[2];
		val += (int)(sensor->data[3] & 0x0f);
		sensor->temperature = sensor->data[2] & 0x80 ? -val * 10 : val * 10;
		sensor->humidity = ((int)sensor->data[0] * 10) + (int)sensor->data[1];
		break;
	case DHT22:
		val = (((int)(sensor->data[2] & 0x7f)) << 8) | sensor->data[3];
		sensor->temperature = sensor->data[2] & 0x80 ? -val : val;		
		val = (((int)sensor->data[0]) << 8) | sensor->data[1];
		sensor->humidity = val;
		break;
	default:
		NOSCHED_LEAVE();
		return ERR_ERROR;
	}
	
	NOSCHED_LEAVE();
	
	return ERR_NOERROR;
}

/* device driver function implementation */
static int dht_init(const struct device_s *dev)
{
	struct dht_data_s *pdata;
	struct dht_config_s *config;
	
	printf("DEV: device init\n");
	
	CRITICAL_ENTER();
	config = (struct dht_config_s *)dev->config;
	pdata = (struct dht_data_s *)dev->data;
	pdata->mode = 0;
	pdata->busy = 0;
	
	/* setup GPIO */
	config->gpio_configpin();
	
	CRITICAL_LEAVE();

	return 0;
}

static int dht_open(const struct device_s *dev, int mode)
{
	struct dht_data_s *pdata;
	int retval = 0;

	NOSCHED_ENTER();
	pdata = (struct dht_data_s *)dev->data;

	pdata->mode = mode;

	if (!pdata->busy) {
		pdata->busy = 1;
		
		if (pdata->mode)
			printf("DEV: device open\n");
	} else {
		if (pdata->mode)
			printf("DEV: device open failed\n");

		retval = -1;
	}
	NOSCHED_LEAVE();

	return retval;
}

static int dht_close(const struct device_s *dev)
{
	struct dht_data_s *pdata;
	int retval = 0;

	NOSCHED_ENTER();
	pdata = (struct dht_data_s *)dev->data;

	if (pdata->busy) {
		if (pdata->mode)
			printf("DEV: device close\n");
		
		pdata->mode = 0;
		pdata->busy = 0;
	} else {
		if (pdata->mode)
			printf("DEV: device close failed\n");
		
		retval = -1;
	}
	NOSCHED_LEAVE();

	return retval;
}

static size_t dht_read(const struct device_s *dev, void *buf, size_t count)
{
	struct dht_data_s *pdata;
	int val;
	
	NOSCHED_ENTER();
	pdata = (struct dht_data_s *)dev->data;
	
	val = dht_read_sensor(dev);

	if (val == ERR_OK) {
		memcpy(buf, pdata->data, 5);
		count = 5;
	} else {
		memset(buf, 0, 5);
	}
		
	NOSCHED_LEAVE();
	
	if (pdata->mode)
		printf("DEV: device read %d bytes\n", count);
	
	return count;
}

/* device driver function mapping for generic API */
const struct device_api_s dht_api = {
	.dev_init = dht_init,
	.dev_open = dht_open,
	.dev_close = dht_close,
	.dev_read = dht_read
};
