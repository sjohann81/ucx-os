extern const struct device_s uart1_dev;
extern const struct device_s uart2_dev;
extern const struct device_s uart6_dev;

int uart_config(struct uart_config_values_s *config);
int uart1_poll(void);
int uart1_tx(int val);
int uart1_rx(void);
int uart2_poll(void);
int uart2_tx(int val);
int uart2_rx(void);
int uart6_poll(void);
int uart6_tx(int val);
int uart6_rx(void);
