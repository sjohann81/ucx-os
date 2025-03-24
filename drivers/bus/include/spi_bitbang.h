/* SPI (bit bang) driver API */
int spi_init(const struct device_s *dev);
int spi_deinit(const struct device_s *dev);
int spi_open(const struct device_s *dev, int mode);
int spi_close(const struct device_s *dev);
size_t spi_read(const struct device_s *dev, void *buf, size_t count);
size_t spi_write(const struct device_s *dev, void *buf, size_t count);

enum spi_speed {
	SPI_SPEED_CS = 5,
	SPI_SPEED_FASTEST = 1,
	SPI_SPEED_FAST = 2,
	SPI_SPEED_NORMAL = 4,
	SPI_SPEED_SLOW = 16,
	SPI_SPEED_SLOWEST = 32
};

/* SPI (bit bang) configuration definitions */
enum spi_config_values {
	SPI_MASTER, SPI_SLAVE,
	SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3,
	SPI_HALF_DUPLEX, SPI_FULL_DUPLEX,
	SPI_MSB = 0x80, SPI_LSB = 0x01,
	SPI_CS_LOW = 0, SPI_CS_HIGH = 1
};

struct spi_config_s {
	char device_mode, spi_mode, duplex_mode;	// default: master, spi mode 0, half duplex
	char bit_order, cs_active;			// default: MSB first, cs active low
	unsigned cs_delay, sig_delay;			// default: 1us, 1us
	int (*gpio_configpins)(void);
	int (*gpio_cs)(int val);
	int (*gpio_sck)(int val);
	int (*gpio_mosi)(int val);
	int (*gpio_miso)(int val);
};

/* SPI (bit bang) data definitions */
struct spi_data_s {
	char init;
	char busy;
};

extern struct device_api_s spi_api;
