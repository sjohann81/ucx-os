extern const struct device_s usart0_dev;

int usart0_config(struct uart_config_values_s *config);
int usart0_poll(void);
int usart0_tx(int val);
int usart0_rx(void);
