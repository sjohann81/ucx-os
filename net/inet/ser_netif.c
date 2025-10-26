#include <ucx.h>
#include <device.h>
#include <inet.h>

static int tty_write(uint8_t *frame, uint16_t len)
{
	uint16_t i;

	_putchar(0x7e);
	for (i = 0; i < len && i < MIN_FRAMELEN; i++) {
		if (frame[i] == 0x7e || frame[i] == 0x7d) {
			_putchar(0x7d);
			_putchar(frame[i] ^ 0x20);
		} else {
			_putchar(frame[i]);
		}
	}
	_putchar(0x7e);
		
	return len;
}

static int tty_read(uint8_t *frame)
{
	uint16_t i = 0;
	uint8_t data;

	if (!(_kbhit()))
		return 0;
		
	data = _getchar();
	if (data != 0x7e) {
		while (_kbhit() && data != 0x7e)
			data = _getchar();
	}

	for (i = 0; i < MIN_FRAMELEN; i++) {
		data = _getchar();
		if (data == 0x7e)
			break;
		if (data == 0x7d) {
			data = _getchar();
			data ^= 0x20;
		}
		frame[i] = data;
	}
	
	return i;
}


/*
 * ethernet send() / receive()
 *
 * -abstract the link layer
 * -handle interface specific protocols
 * -handle broadcast frames
 * -handle ARP protocol
 */
int ser_send(struct netif_config_s *netif, uint8_t *packet, uint16_t len)
{
	uint8_t mac[6], ip[4];
	int32_t l, arp_r = 0;
	uint8_t *frame = (uint8_t *)(packet - sizeof(struct eth_s));
	struct frame_netif_s *frame_s = (struct frame_netif_s *)(packet - sizeof(struct eth_s));

	memcpy(ip, &frame_s->payload.ip.dst_addr, 4);

	if (ip_addr_isany(ip) || ip_addr_isbroadcast(ip, netif->netmask)) {
		memset(mac, 0xff, 6);
		arp_r = 1;
	} else {
		if (ip_addr_ismulticast(ip)){
		    /* Hash IP multicast address to MAC address. */
		    mac[0] = 0x01; mac[1] = 0x0; mac[2] = 0x5e;
		    mac[3] = ip[1] & 0x7f; mac[4] = ip[2]; mac[5] = ip[3];
		    arp_r = 1;
		} else {
			if ((netif->ip[0] | netif->ip[1] | netif->ip[2] | netif->ip[3]) == 0) {
				arp_r = 1;
			} else {
//				if (!ip_addr_maskcmp(ip, netif->ip, netif->gateway))
//					memcpy(ip, netif->gateway, 4);

				arp_r = arp_check(ip, mac);
			}
		}
	}

	if (arp_r) {
		memcpy(&frame_s->ethernet.src_addr, netif->mac, 6);
		memcpy(&frame_s->ethernet.dst_addr, mac, 6);
		frame_s->ethernet.type = htons(FRAME_IP);

		tty_write(frame, len + sizeof(struct eth_s));

		return len + sizeof(struct eth_s);
	} else {
		l = arp_request(netif, ip, frame);

		tty_write(frame, l + sizeof(struct eth_s));

		return 0;
	}
}

int ser_recv(struct netif_config_s *netif, uint8_t *packet)
{
	int32_t len = 0, ll_len;
	uint16_t type;
	uint8_t *frame = (uint8_t *)(packet - sizeof(struct eth_s));
	struct frame_netif_s *frame_s = (struct frame_netif_s *)(packet - sizeof(struct eth_s));

	ll_len = tty_read(frame);

	if (ll_len > 0){
		if (is_local_mac(netif->mac, frame) || is_broadcast_mac(frame) || is_any_mac(frame)) {
			type = ntohs(frame_s->ethernet.type);

			switch(type){
			case FRAME_ARP:
				if (ntohs(frame_s->payload.arp.hw_type) == HARDW_ETH10){
					if (ntohs(frame_s->payload.arp.proto_type) == FRAME_IP){
						if (ntohs(frame_s->payload.arp.hw_len_proto_len) == IP_HLEN_PLEN){
							if (ntohs(frame_s->payload.arp.operation) == OP_ARP_REQUEST) {
								if (!memcmp(frame_s->payload.arp.target_pa, netif->ip, 4)){
									len = arp_reply(netif, frame);

									tty_write(frame, len);

									return 0;
								}
							}

							if (ntohs(frame_s->payload.arp.operation) == OP_ARP_ANSWER)
								arp_update(frame_s->payload.arp.sender_pa, frame_s->payload.arp.sender_ha);
						}
					}
				}
				break;
			case FRAME_IP:
				len = ntohs(frame_s->payload.ip.len);
				
				if (ip_addr_isany(netif->ip)){
					if (frame_s->payload.ip.proto == IP_PROTO_ICMP && len == IP_CFG_PING + sizeof(struct ip_icmp_s)){		/* configure the IP address */
						memcpy(netif->ip, frame_s->payload.ip.dst_addr, 4);
						memcpy(netif->gateway, frame_s->payload.ip.src_addr, 4);

						switch (netif->ip[0]) {
						case 1 ... 127:
							netif->netmask[0] = 255; netif->netmask[1] = 0; netif->netmask[2] = 0; netif->netmask[3] = 0;
							break;
						case 128 ... 191:
							netif->netmask[0] = 255; netif->netmask[1] = 255; netif->netmask[2] = 0; netif->netmask[3] = 0;
							break;
						case 192 ... 223:
							netif->netmask[0] = 255; netif->netmask[1] = 255; netif->netmask[2] = 255; netif->netmask[3] = 0;
							break;
						default:
							netif->netmask[0] = 0; netif->netmask[1] = 0; netif->netmask[2] = 0; netif->netmask[3] = 0;
						}
					}

					return 0;
				}
				break;
			case FRAME_IEEE:
				break;
			case FRAME_IPV6:
				break;
			case FRAME_TEST:
				break;
			default:
				break;
			}
		}
	}

	if (len < 0)
		return 0;
	else
		return len;
}


