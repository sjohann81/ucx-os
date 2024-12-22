#include <ucx.h>
#include <device.h>
#include <uart.h>

/* UART API function wrappers */
int uart_init(const struct device_s *dev)
{
	return dev->api->dev_init(dev);
}

int uart_deinit(const struct device_s *dev)
{
	return dev->api->dev_deinit(dev);
}

int uart_open(const struct device_s *dev, int mode)
{
	return dev->api->dev_open(dev, mode);
}

int uart_close(const struct device_s *dev)
{
	return dev->api->dev_close(dev);
}

size_t uart_read(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_read(dev, buf, count);
}

size_t uart_write(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_write(dev, buf, count);
}

/* UART device driver implementation */
static unsigned long nextpowerof2(unsigned long x)
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	
	return x;
}

static void put_fifo(struct uart_data_s *uart_data, char c)
{
	unsigned tail;

	CRITICAL_ENTER();
	tail = (uart_data->rx_tail + 1) & uart_data->rx_buffer_mask;
	/* if there is space, put data in rx fifo */
	if (tail != uart_data->rx_head) {
		uart_data->rx_buffer[uart_data->rx_tail] = c;
		uart_data->rx_tail = tail;
		uart_data->rx_size++;
	}
	CRITICAL_LEAVE();
}

static char get_fifo(struct uart_data_s *uart_data)
{
	char c = 0;
	
	CRITICAL_ENTER();
	/* if there is data on rx fifo, get it */
	if (uart_data->rx_head != uart_data->rx_tail) {
		c = uart_data->rx_buffer[uart_data->rx_head];
		uart_data->rx_head = (uart_data->rx_head + 1) & 
			uart_data->rx_buffer_mask;
		uart_data->rx_size--;
	}
	CRITICAL_LEAVE();
	
	return c;
}

static int uart_driver_init(const struct device_s *dev)
{
	struct uart_config_s *config;
	struct uart_data_s *data;
	
	config = (struct uart_config_s *)dev->config;
	data = (struct uart_data_s *)dev->data;
	data->busy = 0;
	
	if (config->config_values.interrupt == INTENABLE) {
		if (config->rx_buffer_size <= 32) {
			static char uartbuf[32];
			
			data->rx_buffer = uartbuf;
		} else {
			data->rx_buffer = malloc(nextpowerof2(config->rx_buffer_size));
		}

		data->rx_buffer_mask = nextpowerof2(config->rx_buffer_size) - 1;
		data->put_fifo_cb = put_fifo;
		data->get_fifo_cb = get_fifo;
	} else {
		data->rx_buffer_mask = 0;
		data->put_fifo_cb = 0;
		data->get_fifo_cb = 0;
	}	
	data->rx_head = data->rx_tail = data->rx_size = 0;
	config->uart_config(&config->config_values);
	
	return 0;
}

static int uart_driver_deinit(const struct device_s *dev)
{
	struct uart_config_s *config;
	struct uart_data_s *data;
	
	config = (struct uart_config_s *)dev->config;
	data = (struct uart_data_s *)dev->data;
	
	if (config->config_values.interrupt == INTENABLE)
		if (config->rx_buffer_size > 32)
			free(data->rx_buffer);

	data->rx_buffer_mask = 0;
	data->rx_head = data->rx_tail = data->rx_size = 0;
	data->put_fifo_cb = 0;
	data->get_fifo_cb = 0;

	return 0;
}

static int uart_driver_open(const struct device_s *dev, int mode)
{
	struct uart_data_s *data;
	int retval = 0;
	
	data = (struct uart_data_s *)dev->data;
	
	CRITICAL_ENTER();
	if (!data->busy)
		data->busy = 1;
	else
		retval = -1;
	CRITICAL_LEAVE();
	
	return retval;
}

static int uart_driver_close(const struct device_s *dev)
{
	struct uart_data_s *data;
	
	data = (struct uart_data_s *)dev->data;
	
	CRITICAL_ENTER();
	data->busy = 0;
	CRITICAL_LEAVE();

	return 0;
}

static size_t uart_driver_read(const struct device_s *dev, void *buf, size_t count)
{
	struct uart_config_s *config;
	struct uart_data_s *data;
	char *p;
	int i, val = 0;
	
	config = (struct uart_config_s *)dev->config;
	data = (struct uart_data_s *)dev->data;
	p = (char *)buf;
	
	if (config->config_values.interrupt == INTENABLE) {
		if (count == 0)
			return data->rx_size;
			
		for (i = 0; i < count; i++) {
			while (data->rx_size == 0);
			p[i] = get_fifo(data);
		}
	} else {
		if (count == 0)
			return config->uart_poll();
			
		for (i = 0; i < count; i++) {
			val = config->uart_rx();
			if (val < 0) break;
			p[i] = val;
		}
	}
	
	return i;
}

static size_t uart_driver_write(const struct device_s *dev, void *buf, size_t count)
{
	struct uart_config_s *config;
	char *p;
	int i, val = 0;
	
	config = (struct uart_config_s *)dev->config;
	p = (char *)buf;
	
	for (i = 0; i < count; i++) {
		val = config->uart_tx(p[i]);
		if (val < 0) break;
	}

	return i;
}

/* device driver function mapping for generic API */
struct device_api_s uart_api = {
	.dev_init = uart_driver_init,
	.dev_deinit = uart_driver_deinit,
	.dev_open = uart_driver_open,
	.dev_close = uart_driver_close,
	.dev_read = uart_driver_read,
	.dev_write = uart_driver_write
};
