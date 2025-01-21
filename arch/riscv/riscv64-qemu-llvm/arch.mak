# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS  = -I $(ARCH_DIR) \
	-I $(ARCH_DIR)/../riscv64-qemu

# core speed
F_CLK=10000000
# uart baud rate
SERIAL_BAUDRATE=57600
# timer interrupt frequency (100 -> 100 ints/s -> 10ms tick time. 0 -> timer0 fixed frequency)
F_TICK = 100

#remove unreferenced functions
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
ASFLAGS = -march=rv64imzicsr -mabi=lp64 #-fPIC
CFLAGS = -Wall --target=riscv64 -march=rv64im -mabi=lp64 -O2 -c -ffreestanding -nostdlib -fomit-frame-pointer -mcmodel=medany $(INC_DIRS) -DF_CPU=${F_CLK} -D USART_BAUD=$(SERIAL_BAUDRATE) -DF_TIMER=${F_TICK} -DLITTLE_ENDIAN
ARFLAGS = r

LDFLAGS = -melf64lriscv $(LDFLAGS_STRIP)
LDSCRIPT = $(ARCH_DIR)/../riscv64-qemu/riscv64-qemu.ld

CC = clang
AS = riscv64-unknown-elf-as
LD = riscv64-unknown-elf-ld
DUMP = riscv64-unknown-elf-objdump -Mno-aliases
READ = riscv64-unknown-elf-readelf
OBJ = riscv64-unknown-elf-objcopy
SIZE = riscv64-unknown-elf-size
AR = riscv64-unknown-elf-ar

hal:
	$(AS) $(ASFLAGS) -o crt0.o $(ARCH_DIR)/../riscv64-qemu/crt0.s
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/../riscv64-qemu/hal.c \
		$(ARCH_DIR)/../../common/muldiv.c \
		$(ARCH_DIR)/../../common/ieee754.c \
		$(ARCH_DIR)/../../common/math.c

run_riscv64:
	echo "hit Ctrl+a x to quit"
	qemu-system-riscv64 -machine virt -bios none -kernel $(BUILD_TARGET_DIR)/image.elf -nographic
