PATH_TO_TI_DIR = /home/user/ti
TI_ENV = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin 
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)

# core speed
F_CLK=10000000
# uart baud rate
SERIAL_BAUDRATE=19200
# timer interrupt frequency (100 -> 100 ints/s -> 10ms tick time. 0 -> timer0 fixed frequency)
F_TICK = 100


INC_DIRS  = -I $(ARCH_DIR)/include \
	-I $(ARCH_DIR)/include/arch \
	-I $(ARCH_DIR)/drivers/include \
	-I $(ARCH_DIR)/boards/armv7/include \
	-I $(ARCH_DIR)/boards/armv7/include/bsp \
	-I $(SRC_DIR)/include/kernel \
	-I $(SRC_DIR)/drivers/bus/include/ \
	-I $(SRC_DIR)/include/libpok_legacy \
	-I ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/lib/src \
	-I ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/lib/src/machine \
	-I $(ARCH_DIR)/common


CC = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl
AS = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin/armasm
LD = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin/armlnk
DUMP = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin/armobjdump
READ = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin/armreadelf
OBJ = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin/armobjcopy
SIZE = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin/armsize
AR = ${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/bin/armar

CFLAGS = -mv7R5 --code_state=32 --float_support=VFPv3D16 -Ooff -c      	\
         --opt_for_speed=0 -g --c99 --diag_warning=225 --diag_wrap=off 	\
         --display_error_number --enum_type=packed --abi=eabi          	\
         ${INC_DIRS} -Wall -O2 -c  -mmcu=armv7 $(MCU_DEFINES) \
		 -fverbose-asm -ffreestanding $(C_DEFINES)\
		 -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) \
		 -DF_TIMER=${F_TICK} -D UNKNOWN_HEAP -D LOW_MEM\
         
LDFLAGS = -i"${PATH_TO_TI_DIR}/compiler/ti-cgt-arm_20.2.7.LTS/lib"
.PHONY: hal
hal:
	$(CC) $(CFLAGS) -o cswitch.o $(ARCH_DIR)/source/cswitch.asm
	$(CC) $(CFLAGS) -o syscall.o $(ARCH_DIR)/source/syscall.asm
	$(CC) $(CFLAGS) -o fpu.o $(ARCH_DIR)/source/fpu.asm
	$(CC) $(CFLAGS) -o space_asm.o $(ARCH_DIR)/source/space_asm.asm
	$(CC) $(CFLAGS) -o HL_sys_core.o $(ARCH_DIR)/drivers/source/HL_sys_core.asm
	$(CC) $(CFLAGS) -o HL_sys_intvecs.o $(ARCH_DIR)/drivers/source/HL_sys_intvecs.asm
	$(CC) $(CFLAGS) -o HL_sys_mpu.o $(ARCH_DIR)/drivers/source/HL_sys_mpu.asm
	$(CC) $(CFLAGS) -o HL_sys_pmu.o $(ARCH_DIR)/drivers/source/HL_sys_pmu.asm
	
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/source/*.c \
		$(ARCH_DIR)/drivers/source/*.c \
		$(ARCH_DIR)/boards/armv7/*.c \
		$(LDFLAGS)/src/*.c	
	sh ./$(ARCH_DIR)/rename_obj_to_o.sh