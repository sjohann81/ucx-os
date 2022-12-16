# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS  = -I $(ARCH_DIR)

F_CLK=16000000
SERIAL_BAUDRATE=57600

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
ASFLAGS = 
CFLAGS = -c -g -mmcu=atmega328p -Wall -Os -fno-inline-small-functions -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-main -fomit-frame-pointer -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) $(INC_DIRS) -D UNKNOWN_HEAP
ARFLAGS = r
LDFLAGS = -g -mmcu=atmega328p -Wall -Os -fno-inline-small-functions -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-main -fomit-frame-pointer -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) $(INC_DIRS) -D UNKNOWN_HEAP
LDSCRIPT = 

CC = avr-gcc
AS = avr-as
LD = avr-gcc
DUMP = avr-objdump
READ = avr-objread
OBJ = avr-objcopy
SIZE = avr-size
AR = avr-ar

SERIAL_PROG = /dev/ttyACM0
AVRDUDE_CONFIG=/usr/local/avr/gcc/etc/avrdude.conf
AVRDUDE_PART=m328p

#PROGRAMMER = bsd
#PROGRAMMER = usbtiny
#PROGRAMMER = dasa -P $(SERIAL_PROG)
#PROGRAMMER = usbasp
PROGRAMMER = arduino -P $(SERIAL_PROG)
#PROGRAMMER = wiring -P $(SERIAL_PROG) -D

hal:
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/uart.c \
		$(ARCH_DIR)/hal.c 

flash:
	avrdude -C $(AVRDUDE_CONFIG) -p $(AVRDUDE_PART) -U flash:w:code.hex -y -c $(PROGRAMMER)

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
