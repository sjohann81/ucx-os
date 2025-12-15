# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS = -I $(ARCH_DIR) \
	-I $(ARCH_DIR)/drivers

# core speed
F_CLK=1000000
# uart baud rate
SERIAL_BAUDRATE=57600
# timer interrupt frequency (100 -> 100 ints/s -> 10ms tick time. 0 -> timer0 fixed frequency)
F_TICK = 100
# fixed tick frequency
F_TICK_FIXED = (${F_CLK} / 262144)

#remove unreferenced functions
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
CFLAGS = -Wall -O2 -c -march=armv6 -msoft-float -mabi=atpcs -marm -ffreestanding -nostdlib $(INC_DIRS) -DF_CPU=${F_CLK} -D USART_BAUD=$(SERIAL_BAUDRATE) -DF_TIMER=${F_TICK} -DF_TIMER_FIXED="${F_TICK_FIXED}" -DLITTLE_ENDIAN $(CFLAGS_STRIP) #-mthumb -mthumb-interwork
LDFLAGS = $(LDFLAGS_STRIP)
LDSCRIPT = $(ARCH_DIR)/versatilepb.ld
ARFLAGS = r

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
DUMP = arm-none-eabi-objdump
READ = arm-none-eabi-readelf
OBJ = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
AR = arm-none-eabi-ar

hal:
	$(CC) $(CFLAGS) -o crt0.o $(ARCH_DIR)/crt0.s
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/hal.c \
		$(ARCH_DIR)/../../common/muldiv.c \
		$(ARCH_DIR)/../../common/ieee754.c \
		$(ARCH_DIR)/../../common/math.c \
		$(ARCH_DIR)/drivers/uart_ll.c

run_versatilepb:
	echo "hit Ctrl+a x to quit"
	qemu-system-arm -cpu arm1176 -m 128 -M versatilepb -serial stdio -kernel $(BUILD_TARGET_DIR)/image.elf
