#include <ucx.h>
#include <device.h>
#include <gpio.h>
#include <spi_bitbang.h>

/* GPIO configuration: PB3 (sck), PB4 (miso), PB5 (mosi) and PB6 (cs_n) - port it! */
const struct gpio_config_s gpio_config = {
	.config_values.port	= GPIO_PORTB,
	.config_values.pinsel	= GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6,
	.config_values.mode	= GPIO_OUTPUT << GPIO_PIN3_OPT |
				GPIO_INPUT << GPIO_PIN4_OPT |
				GPIO_OUTPUT << GPIO_PIN5_OPT |
				GPIO_OUTPUT << GPIO_PIN6_OPT,
	.config_values.pull	= GPIO_NOPULL << GPIO_PIN3_OPT |
				GPIO_NOPULL << GPIO_PIN4_OPT |
				GPIO_NOPULL << GPIO_PIN5_OPT |
				GPIO_NOPULL << GPIO_PIN6_OPT
};

/* GPIO device driver instantiation */
const struct device_s gpio_device = {
	.name = "gpio_device",
	.config = &gpio_config,
	.custom_api = &gpio_api
};

const struct device_s *gpio = &gpio_device;
const struct gpio_api_s *gpio_dev_api = (const struct gpio_api_s *)(&gpio_device)->custom_api;


/* GPIO template callbacks - port them! */

/* configure CS, SCK, MOSI and MISO pins direction
 * 
 * CS   - output (master mode)
 * SCK  - output (master mode)
 * MOSI - output (master mode)
 * MISO - input (master mode)
 */
int gpio_configpins(void)
{
	printf("SPI: gpio_config()\n");
	gpio_dev_api->gpio_setup(gpio);
	
	return 0;
}

int gpio_sck(int val)
{
	// output SCK (master)
	switch (val) {
	case 0: gpio_dev_api->gpio_clear(gpio, GPIO_PIN3); return 0;
	case 1: gpio_dev_api->gpio_set(gpio, GPIO_PIN3); return 0;
	default: return -1;
	}

	return 0;
}

int gpio_miso(int val)
{
	// poll MISO (master)
	switch (val) {
	case -1: return ((gpio_dev_api->gpio_get(gpio) & GPIO_PIN4) >> 4);
	default: return -1;
	}

	return 0;
}

int gpio_mosi(int val)
{
	// output MOSI (master)
	switch (val) {
	case 0: gpio_dev_api->gpio_clear(gpio, GPIO_PIN5); return 0;
	case 1: gpio_dev_api->gpio_set(gpio, GPIO_PIN5); return 0;
	default: return -1;
	}
	
	return 0;
}

int gpio_cs(int val)
{
	// output CS (master)
	switch (val) {
	case 0: gpio_dev_api->gpio_clear(gpio, GPIO_PIN6); return 0;
	case 1: gpio_dev_api->gpio_set(gpio, GPIO_PIN6); return 0;
	default: return -1;
	}

	return 0;
}

/* SPI device driver configuration (master) */
const struct spi_config_s spi_config = {
	.device_mode = SPI_MASTER,
	.spi_mode = SPI_MODE0,
	.duplex_mode = SPI_HALF_DUPLEX,
	.bit_order = SPI_MSB,
	.cs_active = SPI_CS_LOW,
	.cs_delay = 1,
	.sig_delay = 1,
	.gpio_configpins = gpio_configpins,
	.gpio_cs = gpio_cs,
	.gpio_sck = gpio_sck,
	.gpio_mosi = gpio_mosi,
	.gpio_miso = gpio_miso
};

struct spi_data_s spi_data;

const struct device_s spi_device1 = {
	.name = "spidevice1",
	.config = &spi_config,
	.data = &spi_data,
	.api = &spi_api
};


void idle(void)
{
	for (;;);
}

char buf[100] = "hello world!";

void task0(void)
{
	const struct device_s *spi1 = &spi_device1;
	int bytes;
	
	while (1) {
		dev_open(spi1, 0);
		dev_write(spi1, buf, strlen(buf));
		dev_close(spi1);
		
		ucx_task_delay(500);
	}
}

int32_t app_main(void)
{
	const struct device_s *spi1 = &spi_device1;
	
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);

	dev_init(spi1);

	// start UCX/OS, preemptive mode
	return 1;
}
