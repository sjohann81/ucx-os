/* 24lcxx driver API */
int eeprom_24lc_init(const struct device_s *dev);
int eeprom_24lc_deinit(const struct device_s *dev);
int eeprom_24lc_open(const struct device_s *dev, int mode);
int eeprom_24lc_close(const struct device_s *dev);
size_t eeprom_24lc_read(const struct device_s *dev, void *buf, size_t count);
size_t eeprom_24lc_write(const struct device_s *dev, void *buf, size_t count);
int eeprom_24lc_seek(const struct device_s *dev, long pos, int whence);

/* 24lcxx configuration definitions */
enum eeprom_24lc_config_values {
	EEPROM_24LC01, EEPROM_24LC02,
	EEPROM_24LC04, EEPROM_24LC08, EEPROM_24LC16,
	EEPROM_24LC32, EEPROM_24LC64,
	EEPROM_24LC128, EEPROM_24LC256,
	EEPROM_24LC512, EEPROM_24LC1024
};

struct eeprom_24lc_config_values_s {
	char type;
	char select;
};

struct eeprom_24lc_config_s {
	struct eeprom_24lc_config_values_s config_values;
	const struct device_s *i2c_dev;
};

/* 24lcxx data definitions */
struct eeprom_24lc_data_s {
	char init;
	char busy;
	char addr_size;
	unsigned page_size;
	long pos;
};

extern struct device_api_s eeprom_24lc_api;
