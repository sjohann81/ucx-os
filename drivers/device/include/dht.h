/* DHT11 / DHT22 */
enum dht_error {
	ERR_NOERROR	= 0,
	ERR_ERROR	= -1,
	ERR_TIMEOUT1	= -2,
	ERR_TIMEOUT2	= -3,
	ERR_TIMEOUT3	= -4,
	ERR_TIMEOUT4	= -5,
	ERR_CHECKSUM	= -6
};

enum dht_timing {
	DHT_TIMEOUTMS	= 100,
	DHT_STARTLOW	= 20000,
	DHT_STARTHIGH	= 30,
	DHT_INITLOW	= 80,
	DHT_INITHIGH	= 80,
	DHT_STARTXMIT	= 50,
	DHT_DATAZERO	= 40,
	DHT_DATAONE	= 60
};

enum dht_type {
	DHT11, DHT12, DHT22
};

struct dht_config_s {
	int (*gpio_configpin)(void);	// set gpio pin to open drain
	int (*gpio_sdata)(int val);	// write: 0 or 1, read: -1
	uint8_t type;
};

struct dht_data_s {
	int mode;
	char busy;
	uint8_t data[5];
	uint16_t temperature;
	uint16_t humidity;
};

extern struct device_api_s dht_api;
