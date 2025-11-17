#include <ucx.h>
#include <device.h>
#include <gpio.h>
#include <spi_bitbang.h>
#include <enc28j60.h>

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


/* SPI (bitbang) configuration and driver instantiation */
const struct spi_config_s spi_config = {
	.device_mode = SPI_MASTER,
	.spi_mode = SPI_MODE0,
	.duplex_mode = SPI_HALF_DUPLEX,
	.bit_order = SPI_MSB,
	.cs_active = SPI_CS_LOW,
	.cs_delay = SPI_SPEED_CS,
	.sig_delay = SPI_SPEED_FAST,
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

const struct device_s *spi1 = &spi_device1;


/* enc28j60 configuration and driver instantiation */
const struct eth_enc28j60_config_s enc28j60_config = {
	.mac_addr = {
		ENC28J60_MAC0, ENC28J60_MAC1, ENC28J60_MAC2,
		ENC28J60_MAC3, ENC28J60_MAC4, ENC28J60_MAC5
	},
	.spi_dev = &spi_device1
};

struct eth_enc28j60_data_s enc28j60_data;

const struct device_s enc28j60_device1 = {
	.name = "enc28j60 ethernet",
	.config = &enc28j60_config,
	.data = &enc28j60_data,
	.api = &eth_enc28j60_api
};

const struct device_s *eth1 = &enc28j60_device1;


/* application */
void idle(void)
{
	for (;;);
}

uint8_t packet[MAX_FRAMELEN];

void task0(void)
{
	int val;
	int count = 0;
	
	dev_open(eth1, 1);
	
	while (1) {
		val = dev_read(eth1, packet, MAX_FRAMELEN);
		printf("read %d bytes\n", val);
		if (count++ == 1000) {
			memset(packet, 0x69, MAX_FRAMELEN);
			memset(packet, 0xff, 6);
			val = dev_write(eth1, packet, MAX_FRAMELEN);
			printf("write %d bytes\n", val);
			count = 0;
		}
	}
	
	// never reached
	dev_close(eth1);
}

int app_main(void)
{
	int val;
	
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);

	dev_init(spi1);
	
	val = dev_init(eth1);
	if (val < 0) {
		printf("error initializing ethernet device\n");
		for (;;);
	}

	// start UCX/OS, preemptive mode
	return 1;
}
