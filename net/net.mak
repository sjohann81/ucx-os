NET_DIR			= $(SRC_DIR)/net
INET_DIR		= $(NET_DIR)/inet

network:
	$(CC) $(CFLAGS) \
		$(INET_DIR)/arp.c \
		$(INET_DIR)/bootp.c \
		$(INET_DIR)/eth_netif.c \
		$(INET_DIR)/ser_netif.c \
		$(INET_DIR)/icmp.c \
		$(INET_DIR)/ip.c \
		$(INET_DIR)/udp.c \
		$(INET_DIR)/utils.c
