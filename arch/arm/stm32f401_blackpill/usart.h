int16_t uart_init(uint8_t port, uint32_t baud, uint8_t polled);
void uart_flush(uint8_t port);
uint16_t uart_rxsize(uint8_t port);
void uart_tx(uint8_t port, uint8_t data);
uint8_t uart_rx(uint8_t port);
