/* UART driver API */
int uart_init(const struct device_s *dev);
int uart_deinit(const struct device_s *dev);
int uart_open(const struct device_s *dev, int mode);
int uart_close(const struct device_s *dev);
size_t uart_read(const struct device_s *dev, void *buf, size_t count);
size_t uart_write(const struct device_s *dev, void *buf, size_t count);

/* UART configuration definitions */
enum uart_config_values {
	UART_PORT0 = 0, UART_PORT1, UART_PORT2, UART_PORT3,
	UART_PORT4, UART_PORT5, UART_PORT6, UART_PORT7,
	BAUD1200 = 0, BAUD2400, BAUD4800,
	BAUD9600, BAUD19200, BAUD38400,
	BAUD57600, BAUD115200,
	DATABITS5 = 0, DATABITS6, DATABITS7,
	DATABITS8, DATABITS9,
	PARITYNONE = 0, PARITYEVEN, PARITYODD,
	STOPBITS1 = 0, STOPBITS2,
	INTDISABLE = 0, INTENABLE
};

struct uart_config_values_s {
	unsigned port : 4;
	unsigned baudrate : 4;
	unsigned databits : 3;
	unsigned parity : 2;
	unsigned stopbits : 2;
	unsigned interrupt : 1;
};

struct uart_config_s {
	struct uart_config_values_s config_values;
	unsigned rx_buffer_size;
	int (*uart_config)(struct uart_config_values_s *config);
	int (*uart_poll)(void);
	int (*uart_tx)(int val);		// blocking tx
	int (*uart_rx)(void);			// blocking rx
};

/* UART data definitions */
struct uart_data_s {
	char *rx_buffer;
	unsigned rx_buffer_mask;
	volatile unsigned rx_head, rx_tail, rx_size;
	void (*put_fifo_cb)(struct uart_data_s *uart_data, char c);
	char (*get_fifo_cb)(struct uart_data_s *uart_data);
	char busy;
};

extern struct device_api_s uart_api;
