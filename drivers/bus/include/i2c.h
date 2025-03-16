/* I2C driver API */
int i2c_hw_init(const struct device_s *dev);
int i2c_hw_deinit(const struct device_s *dev);
int i2c_hw_open(const struct device_s *dev, int mode);
int i2c_hw_close(const struct device_s *dev);
size_t i2c_hw_read(const struct device_s *dev, void *buf, size_t count);
size_t i2c_hw_write(const struct device_s *dev, void *buf, size_t count);

/* I2C configuration definitions */
enum i2c_hw_config_values {
	I2C_PORT0, I2C_PORT1, I2C_PORT2, I2C_PORT3,
	I2C_PORT4, I2C_PORT5, I2C_PORT6, I2C_PORT7,
	I2C_PORT0_ALT, I2C_PORT1_ALT, I2C_PORT2_ALT, I2C_PORT3_ALT,
	I2C_PORT4_ALT, I2C_PORT5_ALT, I2C_PORT6_ALT, I2C_PORT7_ALT,
	I2C_MASTER, I2C_SLAVE,
	I2C_ACK, I2C_NACK,
	I2C_ADDR7BIT, I2C_ADDR10BIT
};

struct i2c_hw_config_values_s {
	unsigned long speed;
	char port, mode, ack, addr_mode;
};

struct i2c_hw_config_s {
	struct i2c_hw_config_values_s config_values;
};

/* I2C data definitions */
struct i2c_hw_data_s {
	char init;
	char busy;
};

extern struct device_api_s i2c_hw_api;
