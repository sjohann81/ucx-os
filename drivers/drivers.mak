DRIVERS_DIR = $(SRC_DIR)/drivers

ddrivers:
	$(CC) $(CFLAGS) \
		$(DRIVERS_DIR)/vt100/vt100.c
