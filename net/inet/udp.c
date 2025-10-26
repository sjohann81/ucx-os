#include <ucx.h>
#include <device.h>
#include <inet.h>

static int (*udp_callback)(struct netif_config_s *netif, uint8_t *packet);

static uint16_t udpchksum(uint8_t *packet, uint16_t len)
{
	uint32_t sum = 0;
	uint16_t i, val, *ptr = (uint16_t *)(packet + sizeof(struct ip_s) - 8);

	sum += IP_PROTO_UDP;
	sum += len;

	for (i = 0; i < len + 8 - 1; i += 2) {
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

int32_t udp_out(struct netif_config_s *netif, uint8_t dst_addr[4], uint16_t src_port, uint16_t dst_port, uint8_t *packet, uint16_t len)
{
	int32_t val;
	uint16_t chksum;
	struct ip_udp_s *udp = (struct ip_udp_s *)packet;
	
	udp->udp.src_port = htons(src_port);
	udp->udp.dst_port = htons(dst_port);
	udp->udp.len = htons(len);
	udp->udp.chksum = htons(0);
	memcpy(&udp->ip.src_addr, netif->ip, 4);
	memcpy(&udp->ip.dst_addr, dst_addr, 4);

	chksum = udpchksum(packet, len);
	udp->udp.chksum = htons(chksum);
	
	val = ip_out(netif, dst_addr, IP_PROTO_UDP, packet, len + sizeof(struct ip_s));

	return val;
}

int32_t udp_in(struct netif_config_s *netif, uint8_t *packet)
{
	int32_t val = -1;
	uint8_t dst_addr[4];
	uint16_t src_port, dst_port, datalen = 0, chksum;
	struct ip_udp_s *udp = (struct ip_udp_s *)packet;

	chksum = ntohs(udp->udp.chksum);
	datalen = ntohs(udp->udp.len);

	if (chksum) {
		udp->udp.chksum = htons(0);
		if (chksum != udpchksum(packet, datalen)) {
			return -1;
		}
	}

	src_port = ntohs(udp->udp.src_port);
	dst_port = ntohs(udp->udp.dst_port);
	
	switch (dst_port) {
	case PORT_ECHO:							/* Echo protocol, RFC862 */
		memcpy(dst_addr, &udp->ip.src_addr, 4);
		udp_out(netif, dst_addr, dst_port, src_port, packet, datalen);
		break;
	case PORT_DISCARD:						/* Discard protocol, RFC863 */
		break;
	default:
		if (udp_callback)
			val = udp_callback(netif, packet);

		return val;
	}

	return datalen;
}

void udp_set_callback(int (*callback)(struct netif_config_s *netif, uint8_t *packet))
{
	udp_callback = callback;
}
