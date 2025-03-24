/* 25lcxx driver API */
int eeprom_25lc_init(const struct device_s *dev);
int eeprom_25lc_deinit(const struct device_s *dev);
int eeprom_25lc_open(const struct device_s *dev, int mode);
int eeprom_25lc_close(const struct device_s *dev);
size_t eeprom_25lc_read(const struct device_s *dev, void *buf, size_t count);
size_t eeprom_25lc_write(const struct device_s *dev, void *buf, size_t count);
int eeprom_25lc_seek(const struct device_s *dev, long pos, int whence);

/* 25lcxx commands */
enum eeprom_25lc_cmd {
	READ = 0x03, WRITE = 0x02,
	WRDI = 0x04, WREN = 0x06,
	RDSR = 0x05, WRSR = 0x01
};

/* 25lcxx configuration definitions */
enum eeprom_25lc_config_values {
	EEPROM_25LC010, EEPROM_25LC020,
	EEPROM_25LC040, EEPROM_25LC080, EEPROM_25LC160,
	EEPROM_25LC320, EEPROM_25LC640,
	EEPROM_25LC128, EEPROM_25LC256,
	EEPROM_25LC512, EEPROM_25LC1024
};

struct eeprom_25lc_config_values_s {
	char type;
};

struct eeprom_25lc_config_s {
	struct eeprom_25lc_config_values_s config_values;
	const struct device_s *spi_dev;
};

/* 25lcxx data definitions */
struct eeprom_25lc_data_s {
	char init;
	char busy;
	char addr_size;
	unsigned page_size;
	long pos;
};

extern struct device_api_s eeprom_25lc_api;
