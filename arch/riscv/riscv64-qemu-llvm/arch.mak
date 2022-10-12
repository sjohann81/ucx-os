# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS  = -I $(ARCH_DIR)

F_CLK=10000000

#remove unreferenced functions
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
ASFLAGS = -march=rv64i -mabi=lp64 #-fPIC
CFLAGS = -Wall --target=riscv64 -march=rv64im -mabi=lp64 -O2 -c -ffreestanding -nostdlib -fomit-frame-pointer -mcmodel=medany $(INC_DIRS) -DCPU_SPEED=${F_CLK} -DLITTLE_ENDIAN $(CFLAGS_STRIP) -DTERM_BAUD=$(SERIAL_BAUD)
ARFLAGS = r

LDFLAGS = -melf64lriscv $(LDFLAGS_STRIP)
LDSCRIPT = $(ARCH_DIR)/riscv64-qemu.ld

CC = clang
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
		$(ARCH_DIR)/hal.c 
