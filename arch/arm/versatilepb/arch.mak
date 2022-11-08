# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS  = -I $(ARCH_DIR)

F_CLK=250000000

#remove unreferenced functions
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
#CFLAGS = -Wall -O2 -c -march=armv6 -msoft-float -mabi=atpcs -fPIC -marm -ffreestanding -nostdlib -fomit-frame-pointer $(INC_DIRS) -DCPU_SPEED=${F_CLK} -DLITTLE_ENDIAN $(CFLAGS_STRIP) -DTERM_BAUD=$(SERIAL_BAUD)
CFLAGS = -Wall -O2 -c -march=armv6 -msoft-float -mabi=atpcs -marm -ffreestanding -nostdlib -fomit-frame-pointer $(INC_DIRS) -DCPU_SPEED=${F_CLK} -DLITTLE_ENDIAN $(CFLAGS_STRIP) -DTERM_BAUD=$(SERIAL_BAUD)
LDFLAGS = $(LDFLAGS_STRIP)
LDSCRIPT = $(ARCH_DIR)/versatilepb.ld
ARFLAGS = r

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
DUMP = arm-none-eabi-objdump -Mno-aliases
READ = arm-none-eabi-readelf
OBJ = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
AR = arm-none-eabi-ar

hal:
	$(CC) $(CFLAGS) -o crt0.o $(ARCH_DIR)/crt0.s
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/hal.c 
