DRIVERS_DIR = $(SRC_DIR)/drivers

ddrivers:
	$(CC) $(CFLAGS) \
		$(DRIVERS_DIR)/i2c/i2c_bitbang.c \
		$(DRIVERS_DIR)/spi/spi_bitbang.c \
		$(DRIVERS_DIR)/uart/uart.c \
		$(DRIVERS_DIR)/vt100/vt100.c
