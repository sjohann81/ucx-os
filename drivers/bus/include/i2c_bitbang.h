/* I2C master (bit bang) driver API */
int i2c_init(const struct device_s *dev);
int i2c_deinit(const struct device_s *dev);
int i2c_open(const struct device_s *dev, int mode);
int i2c_close(const struct device_s *dev);
size_t i2c_read(const struct device_s *dev, void *buf, size_t count);
size_t i2c_write(const struct device_s *dev, void *buf, size_t count);

enum i2c_speed {
	I2C_SPEED_FASTEST = 1,
	I2C_SPEED_FAST = 2,
	I2C_SPEED_NORMAL = 5,
	I2C_SPEED_SLOW = 20,
	I2C_SPEED_SLOWEST = 50
};

/* I2C master (bit bang) configuration definitions */
struct i2c_config_s {
	unsigned sig_delay;		// default: 4us
	int (*gpio_configpins)(void);	// set gpio pins to open drain
	int (*gpio_scl)(int val);	// write: 0 or 1, read: -1
	int (*gpio_sda)(int val);	// write: 0 or 1, read: -1
};

/* I2C master (bit bang) data definitions */
struct i2c_data_s {
	char init;
	char busy;
};

extern struct device_api_s i2c_api;
