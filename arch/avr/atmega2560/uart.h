void uart_init(uint32_t baud, uint8_t polled);
void uart_flush(void);
uint16_t uart_rxsize(void);
void uart_tx(uint8_t data);
uint8_t uart_rx(void);
