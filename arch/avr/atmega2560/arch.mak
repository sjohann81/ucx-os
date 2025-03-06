# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS  = -I $(ARCH_DIR) \
	-I $(ARCH_DIR)/../../common/avr \
	-I $(ARCH_DIR)/drivers

F_CLK=16000000
SERIAL_BAUDRATE=57600
F_TICK = 100

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
ASFLAGS = 
CFLAGS = -c -g -mmcu=atmega2560 -Wall -Os -fno-inline-small-functions -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-main -fomit-frame-pointer -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) -DF_TIMER=${F_TICK} $(INC_DIRS) -D UNKNOWN_HEAP -D LOW_MEM
ARFLAGS = r
LDFLAGS = -g -mmcu=atmega2560 -Wall -Os -fno-inline-small-functions -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-main -fomit-frame-pointer -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) -DF_TIMER=${F_TICK} $(INC_DIRS) -D UNKNOWN_HEAP -D LOW_MEM
LDSCRIPT = 

CC = avr-gcc
AS = avr-as
LD = avr-gcc
DUMP = avr-objdump
READ = avr-objread
OBJ = avr-objcopy
SIZE = avr-size
AR = avr-ar

# serial port
SERIAL_DEV = /dev/ttyACM0
# uart baud rate
SERIAL_BR = 57600

AVRDUDE_CONFIG=/usr/local/avr/gcc/etc/avrdude.conf
AVRDUDE_PART=m2560

#PROGRAMMER = bsd
#PROGRAMMER = usbtiny
#PROGRAMMER = dasa -P $(SERIAL_DEV)
#PROGRAMMER = usbasp
#PROGRAMMER = arduino -P $(SERIAL_DEV)
PROGRAMMER = wiring -P $(SERIAL_DEV) -D

hal:
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/hal.c \
		$(ARCH_DIR)/../../common/avr/ieee754.c \
		$(ARCH_DIR)/drivers/usart.c \
		$(ARCH_DIR)/drivers/gpio_ll.c

flash:
	avrdude -C $(AVRDUDE_CONFIG) -p $(AVRDUDE_PART) -U flash:w:$(BUILD_TARGET_DIR)/image.hex -c $(PROGRAMMER)

# external high frequency crystal
fuses:
	avrdude -C $(AVRDUDE_CONFIG) -p $(AVRDUDE_PART) -U lfuse:w:0xcf:m -U hfuse:w:0xd9:m -U efuse:w:0x07:m -c $(PROGRAMMER)

# internal rc osc @ 1MHz, original factory config
fuses_osc:
	avrdude -C $(AVRDUDE_CONFIG) -p $(AVRDUDE_PART) -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:0x07:m -c $(PROGRAMMER)

serial_sim:
	socat -d -d  pty,link=/tmp/ttyS10,raw,echo=0 pty,link=/tmp/ttyS11,raw,echo=0

test:
	avrdude -C $(AVRDUDE_CONFIG) -p $(AVRDUDE_PART) -c $(PROGRAMMER)

usb_load:
	stty -F ${SERIAL_DEV} ${SERIAL_BR} raw cs8 -echo
	cat ${SERIAL_DEV}
	
run_avr:
	echo "hit Ctrl+a x to quit"
	qemu-system-avr -machine mega2560 -bios $(BUILD_TARGET_DIR)/image.bin -nographic
