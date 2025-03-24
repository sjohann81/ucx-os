DRIVERS_DIR		= $(SRC_DIR)/drivers
BUS_DRIVERS_DIR		= $(DRIVERS_DIR)/bus
DEVICE_DRIVERS_DIR	= $(DRIVERS_DIR)/device

ddrivers:
	$(CC) $(CFLAGS) \
		$(BUS_DRIVERS_DIR)/gpio/gpio.c \
		$(BUS_DRIVERS_DIR)/i2c/i2c_bitbang.c \
		$(BUS_DRIVERS_DIR)/spi/spi_bitbang.c \
		$(BUS_DRIVERS_DIR)/i2c/i2c.c \
		$(BUS_DRIVERS_DIR)/uart/uart.c \
		$(DEVICE_DRIVERS_DIR)/device/device.c \
		$(DEVICE_DRIVERS_DIR)/24lcxxx/24lcxxx.c \
		$(DEVICE_DRIVERS_DIR)/25lcxxx/25lcxxx.c \
		$(DEVICE_DRIVERS_DIR)/vt100/vt100.c

