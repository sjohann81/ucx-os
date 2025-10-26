/* file:          tuntap_if.c
 * description:   tun/tap interface access / low level
 *                ethernet driver abstraction / tunnel via serial interface (host side)
 * date:          10/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stddef.h>


#define USTACK_IP_ADDR "172.31.69.20"
#define USTACK_NETMASK "255.255.255.0"
#define USTACK_GW_ADDR "172.31.69.254"
#define USTACK_TAP_ADDR "172.31.69.254/24"
#define USTACK_TAP_ROUTE "172.31.69.0/24"

#define FRAME_SIZE	600
#define SERIAL_TO	50000
#define TUN_TO		50000
#define TUN_DEV		"/dev/net/tun"

static int32_t tun_fd;
static char *dev;

char *tapaddr = USTACK_TAP_ADDR;
char *taproute = USTACK_TAP_ROUTE;

int fd;

uint8_t eth_frame[FRAME_SIZE];
uint8_t mymac[6];

int32_t tty_setup(char *uart){
	fd = open(uart, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		printf("[ERROR]	error opening %s\n", uart);
		
		return -1;
	}
	
	return 0;
}

int32_t tty_close(){
	close(fd);
	
	return 0;
}

int32_t tty_data_recv(void){
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	struct timeval timeout = {0, SERIAL_TO};
	/* ret == 0 timeout, ret == 1 descriptor is ready for reading, ret == -1 error */
	return select(fd + 1, &fds, NULL, NULL, &timeout);
}

int32_t tun_data_recv(void){
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(tun_fd, &fds);
	struct timeval timeout = {0, TUN_TO};
	/* ret == 0 timeout, ret == 1 descriptor is ready for reading, ret == -1 error */
	return select(tun_fd + 1, &fds, NULL, NULL, &timeout);
}

/*
static int32_t set_if_route(char *dev, char *cidr)
{
	char buf[256];

	sprintf(buf, "ip route add dev %s %s", dev, cidr);
	printf("[DEBUG] %s\n", buf);
	system(buf);
	
	return 0;
}

static int32_t set_if_address(char *dev, char *cidr)
{
	char buf[256];

	sprintf(buf, "ip address add dev %s local %s", dev, cidr);
	printf("[DEBUG] %s\n", buf);
	system(buf);
	
	return 0;
}

static int32_t set_if_up(char *dev)
{
	char buf[256];

	sprintf(buf, "ip link set dev %s up", dev);
	printf("[DEBUG] %s\n", buf);
	system(buf);
	
	return 0;
}
*/

static int32_t tun_alloc(char *dev)
{
	struct ifreq ifr;
	int32_t fd, err;
	struct ifreq s;

	if ((fd = open(TUN_DEV, O_RDWR | O_NOCTTY)) < 0) {
		printf("[FATAL] Cannot open TUN/TAP dev\nMake sure one exists with '$ mknod /dev/net/tun c 10 200'\n");
		exit(-1);
	}

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	if (*dev)
		strncpy(ifr.ifr_name, dev, IFNAMSIZ - 1);

	if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
		printf("[FATAL] Could not ioctl tun");
		close(fd);
		return err;
	}

	if ((err = ioctl(fd, SIOCGIFHWADDR, &s)) == 0) {
		memcpy(mymac, s.ifr_addr.sa_data, 6);
		printf("[DEBUG] tap interface configured\n");
	} else {
		printf("[FATAL] Could not get interface MAC address");
		close(fd);
		return err;
	}
	strcpy(dev, ifr.ifr_name);

	return fd;
}

#define ifreq_offsetof(x) offsetof(struct ifreq, x)

int32_t if_setup()
{
	dev = calloc(10, 1);
	tun_fd = tun_alloc(dev);

//	if (set_if_up(dev) != 0)
//		printf("[FATAL] Setting up interface failed.\n");

//	if (set_if_route(dev, taproute) != 0)
//		printf("[FATAL] Setting route for interface failed\n");

//	if (set_if_address(dev, tapaddr) != 0)
//		printf("[FATAL] Setting address for interface failed\n");
	struct ifreq ifr;
	struct sockaddr_in sai;
	int sockfd; /* socket fd we use to manipulate stuff with */
	char *p;

	/* Create a channel to the NET kernel. */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	/* get interface name */
	strncpy(ifr.ifr_name, "tap0", IFNAMSIZ);

	memset(&sai, 0, sizeof(struct sockaddr));
	sai.sin_family = AF_INET;
	sai.sin_port   = 0;

	sai.sin_addr.s_addr = inet_addr(USTACK_GW_ADDR);

	p = (char *) &sai;
	memcpy((((char *) &ifr + ifreq_offsetof(ifr_addr))), p,
	   sizeof(struct sockaddr));

	ioctl(sockfd, SIOCSIFADDR, &ifr);
	ioctl(sockfd, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
	// ifr.ifr_flags &= ~selector;  // unset something
	ioctl(sockfd, SIOCSIFFLAGS, &ifr);
	close(sockfd);
		
	return 0;
}

int32_t if_deinit()
{
	free(dev);
	
	return 0;
}

int32_t hexdump(uint8_t *buf, uint32_t size)
{
	uint32_t k, l;
	char ch;

	for (k = 0; k < size; k += 16) {
		for (l = 0; l < 16; l++) {
			printf("%02x ", buf[k + l]);
			if (l == 7) putchar(' ');
		}
		printf(" |");
		for (l = 0; l < 16; l++) {
			ch = buf[k + l];
			if ((ch >= 32) && (ch <= 126))
				putchar(ch);
			else
				putchar('.');
		}
		printf("|\n");
	}

	return 0;
}

int main(int32_t argc, char **argv)
{
	int32_t size, i;
	uint8_t data;
	uint8_t del = 0x7e, esc = 0x7d;

	if (argc != 2) {
		printf("Usage: tuntap_if_host </dev/ttyXX>\n");
		
		return -1;
	}

	if_setup();

	if (tty_setup(argv[1]))
		return -1;
	
	for (;;) {
		if (tun_data_recv() == 1) {
			size = read(tun_fd, eth_frame, FRAME_SIZE);
			
			if (size > 0) {
				write(fd, &del, 1);
				for (i = 0; i < size; i++) {
					if (eth_frame[i] == 0x7e || eth_frame[i] == 0x7d) {
						write(fd, &esc, 1);
						data = eth_frame[i] ^ 0x20;
					} else {
						data = eth_frame[i];
					}
					write(fd, &data, 1);
				}
				write(fd, &del, 1);
				
				printf("tun data size: %d\n", size);
//				hexdump(eth_frame, size);
			}
		}

		if (tty_data_recv() == 1) {
			read(fd, &data, 1);
			
			if (data != 0x7e) {
				for (i = 0; tty_data_recv() == 1 && data != 0x7e && i < FRAME_SIZE; i++)
					read(fd, &data, 1);
					
				if (i == FRAME_SIZE) {
					printf("[ERROR]	tty error\n");

					return -1;
				}

				if (tty_data_recv() != 1)
					continue;
			}
			
			for (i = 0; i < FRAME_SIZE; i++) {
				if (tty_data_recv() != 1) {
					usleep(10000);
					if (tty_data_recv() != 1)
						break;
				}
				
				
				read(fd, &data, 1);

				if (data == 0x7e)
					break;
				if (data == 0x7d) {
					if (tty_data_recv() != 1) {
						usleep(10000);
						if (tty_data_recv() != 1)
							break;
					}
					
					read(fd, &data, 1);
					data ^= 0x20;
				}
				eth_frame[i] = data;
			}

			printf("tty data size: %d\n", i);
			hexdump(eth_frame, i);
			write(tun_fd, eth_frame, i);
		}
	}
}
