#include <ucx.h>
#include <device.h>
#include <spi_bitbang.h>

/* GPIO template callbacks - port them! */

/* configure CS, SCK, MOSI and MISO pins direction
 * 
 * CS   - output (master mode), input (slave mode)
 * SCK  - output (master mode), input (slave mode)
 * MOSI - output (master mode), input (slave mode)
 * MISO - input (master mode), output (slave mode)
 */
int gpio_configpins(void)
{
	printf("SPI: gpio_config()\n");
	
	return 0;
}

int gpio_cs(int val)
{
	// output CS (master) or poll CS (slave)
	printf("SPI: gpio_cs() %d\n", val);

	return 0;
}

int gpio_sck(int val)
{
	// output SCK (master) or poll SCK (slave)
	printf("SPI: gpio_sck() %d\n", val);

	return 0;
}

int gpio_mosi(int val)
{
	// output MOSI (master) or poll MOSI (slave)
	printf("SPI: gpio_mosi() %d\n", val);
	
	return 0;
}

int gpio_miso(int val)
{
	// poll MISO (master) or output MISO (slave)
	printf("SPI: gpio_miso()\n");

	return 0;
}

/* SPI device driver configuration (master)
 * 
 * In this example, the SPI driver is configured in master mode.
 * cs_delay and sig_delay are dependent on the application, and are
 * generally limited to the slave capabilities. In software SPI,
 * if the slave is also a software implementation (such as spi_slave.c)
 * cs_delay should account to thread latency (time where the thread
 * isn't running, which is dependent on tick time and the number of
 * threads). sig_delay should account to software latency on the slave.
 * If the slave is a hardware implementation, cs_active can be 1 to 1000us
 * and sig_delay can be < 1us.
 * Software SPI disables the scheduler during a data transfer.
 */
const struct spi_config_s spi_config = {
	.device_mode = SPI_MASTER,
	.spi_mode = SPI_MODE0,
	.duplex_mode = SPI_HALF_DUPLEX,
	.bit_order = SPI_MSB,
	.cs_active = SPI_CS_LOW,
	.cs_delay = 1000,
	.sig_delay = 100,
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

void task0(void)
{
	const struct device_s *spi1 = &spi_device1;
	char *msg = "hello world!";
	char buf[100];
	int bytes;
	
	while (1) {
		memset(buf, 0, sizeof(buf));
		dev_open(spi1, 0);
		dev_write(spi1, msg, strlen(msg) + 1);
		dev_close(spi1);
		
		ucx_task_delay(10);
		
		dev_open(spi1, 0);
		bytes = dev_read(spi1, buf, sizeof(buf));
		dev_close(spi1);
		
		if (bytes)
			printf("%s\n", buf);
		
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
