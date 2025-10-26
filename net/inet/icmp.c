#include <ucx.h>
#include <device.h>
#include <inet.h>

int32_t icmp_echo_reply(struct netif_config_s *netif, uint8_t *packet, uint16_t len)
{
	uint8_t dst_addr[4];
	uint16_t chksum, val;
	struct ip_icmp_s *icmp = (struct ip_icmp_s *)packet;

	switch(icmp->icmp.type) {
	case ICMP_ECHO :
		icmp->icmp.type = ICMP_ECHO_REPLY;
		chksum = ntohs(icmp->icmp.chksum);
		if (chksum > (0xffff - (ICMP_ECHO << 8))) {
			chksum += (ICMP_ECHO << 8) + 1;
		} else {
			chksum += (ICMP_ECHO << 8);
		}
		icmp->icmp.chksum = htons(chksum);
		memcpy(dst_addr, &icmp->ip.src_addr, 4);

		val = ip_out(netif, dst_addr, -1, packet, len);

		return val;
	case ICMP_ECHO_REPLY :
		return 0;
	default :						/* ICMP protocol error (other protocol / not implemented) */
		return -1;
	}
}

int32_t icmp_echo_request(struct netif_config_s *netif, uint8_t dst_addr[4], uint8_t *packet)
{
	uint16_t chksum, val;
	struct ip_icmp_s *icmp = (struct ip_icmp_s *)packet;

	icmp->icmp.type = ICMP_ECHO;
	icmp->icmp.code = 0;
	icmp->icmp.chksum = htons(0);
	icmp->icmp.id = htons(0x9669);
	icmp->icmp.seqno = htons(0x0001);
	memset(icmp + sizeof(struct ip_icmp_s), 0x55, 16);
	chksum = checksum((uint8_t *)&icmp->icmp, ICMP_HDR_SIZE + 16);
	icmp->icmp.chksum = htons(chksum);

	val = ip_out(netif, dst_addr, IP_PROTO_ICMP, packet, sizeof(struct ip_icmp_s) + 16);
	
	return val;
}
