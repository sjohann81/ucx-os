# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS  = -I $(ARCH_DIR)

F_CLK=16000000
SERIAL_BAUDRATE=57600

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
ASFLAGS = 
CFLAGS = -c -g -mmcu=atmega2560 -Wall -Os -fno-inline-small-functions -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-main -fomit-frame-pointer -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) $(INC_DIRS) -D UCX_OS_HEAP_SIZE=1024

LDFLAGS = -g -mmcu=atmega2560 -Wall -Os -fno-inline-small-functions -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-main -fomit-frame-pointer -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) $(INC_DIRS) -D UCX_OS_HEAP_SIZE=1024
LDSCRIPT = 

CC = avr-gcc
AS = avr-as
LD = avr-gcc
DUMP = avr-objdump
READ = avr-objread
OBJ = avr-objcopy
SIZE = avr-size

hal:
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/uart.c \
		$(ARCH_DIR)/hal.c 
