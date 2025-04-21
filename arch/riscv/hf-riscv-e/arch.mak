# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS = -I $(ARCH_DIR)\
	-I $(ARCH_DIR)/../hf-riscv/drivers

# core speed
F_CLK = 25000000
# uart baud rate
SERIAL_BAUDRATE=57600
# timer interrupt frequency (100 -> 100 ints/s -> 10ms tick time. 0 -> timer0 fixed frequency)
F_TICK = 0
# fixed tick frequency
F_TICK_FIXED = (${F_CLK} / 262144)

# remove unreferenced functions
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
ASFLAGS = -march=rv32e -mabi=ilp32e #-fPIC
CFLAGS = -Wall -march=rv32e -mabi=ilp32e -O2 -c -mstrict-align -ffreestanding -nostdlib -ffixed-a5 $(INC_DIRS) -DF_CPU=${F_CLK} -D USART_BAUD=$(SERIAL_BAUDRATE) -DF_TIMER=${F_TICK} -DF_TIMER_FIXED="${F_TICK_FIXED}" -DLITTLE_ENDIAN $(CFLAGS_STRIP) #-mrvc -fPIC -DDEBUG_PORT
ARFLAGS = r

LDFLAGS = -melf32lriscv $(LDFLAGS_STRIP)
LDSCRIPT = $(ARCH_DIR)/hf-risc.ld

CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
LD = riscv64-unknown-elf-ld
DUMP = riscv64-unknown-elf-objdump -Mno-aliases
READ = riscv64-unknown-elf-readelf
OBJ = riscv64-unknown-elf-objcopy
SIZE = riscv64-unknown-elf-size
AR = riscv64-unknown-elf-ar

hal:
	$(AS) $(ASFLAGS) -o crt0.o $(ARCH_DIR)/crt0.s
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/../hf-riscv/hal.c \
		$(ARCH_DIR)/../hf-riscv/interrupt.c \
		$(ARCH_DIR)/../../common/muldiv.c \
		$(ARCH_DIR)/../../common/ieee754.c \
		$(ARCH_DIR)/../../common/math.c \
		$(ARCH_DIR)/../hf-riscv/drivers/usart.c \
		$(ARCH_DIR)/../hf-riscv/drivers/gpio_ll.c \
		$(ARCH_DIR)/../hf-riscv/drivers/pwm_ll.c

loadbin: serial
	echo "u" > ${SERIAL_DEVICE}
	sleep 1
	cat ${BUILD_TARGET_DIR}/image.bin > ${SERIAL_DEVICE}
	sleep 5
	echo "b" > ${SERIAL_DEVICE}
