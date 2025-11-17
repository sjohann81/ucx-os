#include <ucx.h>
#include <device.h>
#include <gpio.h>
#include <gpio_ll.h> 	// for LED_PORT and LED_PIN defs
#include <inet.h>

/*
 * compile the serial tunnel with 'gcc app/tuntap_if_host.c -o tuntap_if_host'
 * and run it after uploading the firmware to the target and setting up
 * the serial port.
 */

/* GPIO configuration */
const struct gpio_config_s gpio_config = {
	.config_values.port = LED_PORT,
	.config_values.pinsel = LED_PIN,
	.config_values.mode = GPIO_OUTPUT << LED_PIN_OPT,
	.config_values.pull = GPIO_NOPULL << LED_PIN_OPT
};

/* device driver instantiation */
const struct device_s gpio_device = {
	.name = "gpio_device",
	.config = &gpio_config,
	.custom_api = &gpio_api
};

const struct device_s *gpio = &gpio_device;
const struct gpio_api_s *gpio_dev_api = (const struct gpio_api_s *)(&gpio_device)->custom_api;

/* network stack configuration */
struct netif_config_s net1 = {
	.net_send = ser_send,
	.net_recv = ser_recv,
	.framelen = MIN_FRAMELEN,
	.mac = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
};


/* application */
int app_udp_handler(struct netif_config_s *netif, uint8_t *packet)
{
	uint8_t dst_addr[4];
	uint16_t src_port, dst_port;
	struct ip_udp_s *udp = (struct ip_udp_s *)packet;
	uint8_t msg[] = "Hello world!";

	src_port = ntohs(udp->udp.src_port);
	dst_port = ntohs(udp->udp.dst_port);

	if (dst_port == UDP_DEFAULT_PORT) {
		memcpy(dst_addr, udp->ip.src_addr, 4);
		memcpy(packet + sizeof(struct ip_udp_s), msg, sizeof(msg));
		udp_out(netif, dst_addr, dst_port, src_port, packet, sizeof(struct udp_s) + sizeof(msg));
	}
	
	return 0;
}

void idle(void)
{
	for (;;);
}

void task0(void)
{
	struct netif_config_s *network = &net1;
	uint8_t *packet;
	int len;
	
	packet = network->frame + sizeof(struct eth_s);

	/* network stack loop */
	while (1) {
		len = network->net_recv(network, packet);

		if (len > 0) {
			ip_in(network, network->ip, packet, len);
		}
	}
}

void task1(void)
{
	/* blink loop */
	while (1) {
		gpio_dev_api->gpio_toggle(gpio, LED_PIN);
		ucx_task_delay(MS_TO_TICKS(500));
	}
}

int app_main(void)
{
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);

	gpio_dev_api->gpio_setup(gpio);

	net1.frame = malloc(net1.framelen);
	if (!net1.frame) {
		printf("frame malloc() fail\n");
		for (;;);
	}
	ip_config(net1.ip, "172.31.69.20");
	ip_config(net1.netmask, "255.255.255.0");
	ip_config(net1.gateway, "172.31.69.254");
	udp_set_callback(app_udp_handler);

	// start UCX/OS, preemptive mode
	return 1;
}
