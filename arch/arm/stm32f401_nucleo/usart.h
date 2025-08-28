int16_t usart_init(uint8_t port, uint32_t baud, uint8_t polled);
void usart_flush(uint8_t port);
uint16_t usart_rxsize(uint8_t port);
void usart_tx(uint8_t port, uint8_t data);
uint8_t usart_rx(uint8_t port);
