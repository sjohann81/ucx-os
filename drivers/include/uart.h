/* UART driver API */
int uart_init(const struct device_s *dev);
int uart_deinit(const struct device_s *dev);
int uart_open(const struct device_s *dev, int mode);
int uart_close(const struct device_s *dev);
size_t uart_read(const struct device_s *dev, void *buf, size_t count);
size_t uart_write(const struct device_s *dev, void *buf, size_t count);

/* UART configuration definitions */
enum uart_config_values {
	UARTPORT0, UARTPORT1, UARTPORT2, UARTPORT3,
	UARTPORT4, UARTPORT5, UARTPORT6, UARTPORT7,
	BAUD1200 = (0 << 4), BAUD2400 = (1 << 4), BAUD4800 = (2 << 4),
	BAUD9600 = (3 << 4), BAUD19200 = (4 << 4), BAUD_38400 = (5 << 4),
	BAUD57600 = (6 << 4), BAUD115200 = (7 << 4),
	DATABITS5 = (0 << 8), DATABITS6 = (1 << 8), DATABITS7 = (2 << 8),
	DATABITS8 = (3 << 8), DATABITS9 = (4 << 8),
	PARITYNONE = (0 << 11), PARITYEVEN = (1 << 11), PARITYODD = (2 << 11),
	STOPBITS1 = (0 << 13), STOPBITS2 = (1 << 13),
	INT_DISABLE = (0 << 14), INT_ENABLE = (1 << 14)
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
	int (*uart_rxsize)(void);
	int (*uart_tx)(int val);		// blocking tx
	int (*uart_rx)(void);			// blocking rx
};

/* UART data definitions */
struct uart_data_s {
	char *rx_buffer;
	unsigned rx_buffer_mask;
	unsigned rx_head, rx_tail, rx_size;
	void (*put_fifo_cb)(struct uart_data_s *uart_data, char c);
	char (*get_fifo_cb)(struct uart_data_s *uart_data);
	struct sem_s *mutex;
	char busy;
};

extern struct device_api_s uart_api;
