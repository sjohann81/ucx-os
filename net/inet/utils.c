#include <ucx.h>
#include <device.h>
#include <inet.h>

uint16_t checksum(uint8_t *buf, int32_t len)
{
	uint32_t i, sum = 0;
	uint16_t val, *ptr = (uint16_t *)buf;

	for (i = 0; i < len - 1; i += 2) {
		val = *ptr++;
		sum += ntohs(val);
	}

	if (len & 1) {
		val = *ptr;
		sum += ntohs(val) & 0xff00;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return ~sum;
}

int32_t is_broadcast_mac(uint8_t *frame)
{
	uint8_t bcast_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	struct eth_s *ethernet = (struct eth_s *)frame;

	if (!memcmp(&ethernet->dst_addr, bcast_addr, 6))
		return 1;
	else
		return 0;
}

int32_t is_any_mac(uint8_t *frame)
{
	uint8_t bcast_addr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	struct eth_s *ethernet = (struct eth_s *)frame;

	if (!memcmp(&ethernet->dst_addr, bcast_addr, 6))
		return 1;
	else
		return 0;
}

int32_t is_local_mac(uint8_t mymac[6], uint8_t *frame)
{
	struct eth_s *ethernet = (struct eth_s *)frame;
	
	if (!memcmp(&ethernet->dst_addr, mymac, 6))
		return 1;
	else
		return 0;
}

int32_t ip_addr_maskcmp(uint8_t addr1[4], uint8_t addr2[4], uint8_t mask[4])
{
	int32_t i;
	uint8_t ad1[4], ad2[4];

	for (i = 0; i < 4; i++) {
		ad1[i] = addr1[i] & mask[i];
		ad2[i] = addr2[i] & mask[i];
	}

	if (!memcmp(ad1, ad2, 4))
		return 1;
	else
		return 0;
}

int32_t ip_addr_cmp(uint8_t addr1[4], uint8_t addr2[4])
{
	if (!memcmp(addr1, addr2, 4))
		return 1;
	else
		return 0;
}

int32_t ip_addr_isany(uint8_t addr[4])
{
	uint8_t addr_any[4] = {0x00, 0x00, 0x00, 0x00};

	if (!memcmp(addr, addr_any, 4))
		return 1;
	else
		return 0;
}

int32_t ip_addr_isbroadcast(uint8_t addr[4], uint8_t mask[4])
{
	int32_t i;
	uint8_t ad[4], msk[4];
	uint8_t addr_any[4] = {0x00, 0x00, 0x00, 0x00};
	uint8_t addr_all[4] = {0xff, 0xff, 0xff, 0xff};

	for (i = 0; i < 4; i++) {
		ad[i] = addr[i] & ~mask[i];
		msk[i] = ~mask[i] & 0xff;
	}

	if (!memcmp(ad, msk, 4) || !memcmp(ad, addr_any, 4) || !memcmp(ad, addr_all, 4))
		return 1;
	else
		return 0;
}

int32_t ip_addr_ismulticast(uint8_t addr[4])
{
	int32_t i;
	uint8_t ad[4];
	uint8_t ad1[4] = {0xf0, 0x00, 0x00, 0x00};
	uint8_t ad2[4] = {0xe0, 0x00, 0x00, 0x00};

	for (i = 0; i < 4; i++)
		ad[i] = addr[i] & ad1[i];

	if (!memcmp(ad, ad2, 4))
		return 1;
	else
		return 0;
}

int32_t ip_config(uint8_t addr[4], char *str)
{
	int32_t i;
	char *p;
	char buf[128];
	const char delim[] = ".";

	strncpy(buf, str, sizeof(buf)-1);
	p = strtok(buf, delim);
	for (i = 0; i < 4 && p != 0; i++) {
		addr[i] = atoi(p);
		p = strtok(NULL, delim);
	}
	
	if (i < 4)
		return -1;
	else
		return 0;
}
