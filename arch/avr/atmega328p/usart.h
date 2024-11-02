void usart_init(uint32_t baud, uint8_t polled);
void usart_flush(void);
uint16_t usart_rxsize(void);
void usart_tx(uint8_t data);
uint8_t usart_rx(void);
