#include <ucx.h>
#include <device.h>
#include <inet.h>

static int (*ip_callback)(struct netif_config_s *netif, uint8_t *packet);

int32_t ip_out(struct netif_config_s *netif, uint8_t dst_addr[4], int16_t proto, uint8_t *packet, uint16_t len)
{
	uint16_t sum;
	int32_t val;
	struct ip_s *ip = (struct ip_s *)packet;

	ip->ver = IP_VER_IHL >> 8;
	ip->tos = 0x00;
	ip->len = htons(len);
	ip->id = htons(0);
	ip->flags_off = htons(0);
	ip->ttl = IP_DEFAULT_TTL;
	if (proto > -1)
		ip->proto = proto & 0xff;
	memcpy(&ip->src_addr, netif->ip, 4);
	memcpy(&ip->dst_addr, dst_addr, 4);
	
	ip->chksum = htons(0);
	sum = checksum(packet, 20);
	ip->chksum = htons(sum);

	val = netif->net_send(netif, packet, len);

	return val;
}

int32_t ip_in(struct netif_config_s *netif, uint8_t dst_addr[4], uint8_t *packet, uint16_t len)
{
	int32_t val = -1;
	uint16_t chksum;
	struct ip_s *ip = (struct ip_s *)packet;

	if (ip->ver != (IP_VER_IHL >> 8)) {
		return -1;							/* IP version / options error (not supported) */
	}
	if (ntohs(ip->flags_off) & (IP_FLAG_MOREFRAG | IP_FRAGOFS_MASK)) {
		return -1;							/* IP fragmented packets not supported */
	}
	if (ip->ttl == 0) {
		return -1;							/* TP TTL has expired */
	}

	if (!ip_addr_cmp((uint8_t *)&ip->dst_addr, dst_addr)) {
		if (!ip_addr_isbroadcast((uint8_t *)&ip->dst_addr, netif->netmask)) {
			if (!ip_addr_ismulticast((uint8_t *)&ip->dst_addr)) {
				return -1;					/* IP destination address error */
			}
		}
	}

	chksum = ntohs(ip->chksum);
	ip->chksum = htons(0);
	if (chksum != checksum(packet, 20)) {
		return -1;							/* IP checksum error */
	}
	ip->chksum = htons(chksum);

	switch (ip->proto) {
	case IP_PROTO_ICMP:
		val = icmp_echo_reply(netif, packet, len);
		break;
	case IP_PROTO_UDP:
		val = udp_in(netif, packet);
		break;
	default:
		if (ip_callback)
			val = ip_callback(netif, packet);
	}

	return val;
}

void ip_set_callback(int (*callback)(struct netif_config_s *netif, uint8_t *packet))
{
	ip_callback = callback;
}
