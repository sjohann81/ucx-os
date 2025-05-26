TI_ENV = /home/victorgilbert/ti/compiler/ti-cgt-arm_20.2.7.LTS #Change for general localisation template
# TI_ENV_WIN = C:/Program Files/CodeComposer/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS
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
# C_OBJS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/build/*.obj))

CC = $(TI_ENV)/bin/armcl
AS = $(TI_ENV)/bin/armasm
LD = $(TI_ENV)/bin/armlnk
DUMP = $(TI_ENV)/bin/armobjdump
READ = $(TI_ENV)/bin/armreadelf
OBJ = $(TI_ENV)/bin/armobjcopy
SIZE = $(TI_ENV)/bin/armsize
AR = $(TI_ENV)/bin/armar

# MCU_DEFINES = -mabi=atpcs -mthumb -fsingle-precision-constant
# C_DEFINES = -D HSE_VALUE=25000000 -D USB_SERIAL

CFLAGS = -mv7R5 --code_state=32 --float_support=VFPv3D16 -Ooff -c      	\
         --opt_for_speed=0 -g --c99 --diag_warning=225 --diag_wrap=off 	\
         --display_error_number --enum_type=packed --abi=eabi          	\
         ${INC_DIRS} -Wall -O2 -c  -mmcu=armv7 $(MCU_DEFINES) \
		 -fverbose-asm -ffreestanding $(C_DEFINES) $(INC_DIRS) \
		 -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) \
		 -DF_TIMER=${F_TICK} $(INC_DIRS) -D UNKNOWN_HEAP -D LOW_MEM\
         
LDFLAGS = -mv7R5 --code_state=32 --float_support=VFPv3D16 -Ooff         \
          --opt_for_speed=0 -g --c99 --diag_warning=225 --diag_wrap=off \
           --display_error_number --enum_type=packed --abi=eabi -z      \
			-i"$(TI_ENV)/lib" 											\
           --reread_libs --diag_wrap=off --display_error_number         \
           --warn_sections 							    				\
		   -D F_CPU=$(F_CLK) -D USART_BAUD=$(SERIAL_BAUDRATE) 			\
		   -DF_TIMER=${F_TICK} $(INC_DIRS) -D UNKNOWN_HEAP -D LOW_MEM	\
           
.PHONY: hal
hal:
	$(CC) $(CFLAGS) -o cswitch.o $(ARCH_DIR)/source/cswitch.asm
	$(CC) $(CFLAGS) -o entry.o $(ARCH_DIR)/source/entry.asm
	$(CC) $(CFLAGS) -o fpu.o $(ARCH_DIR)/source/fpu.asm
	$(CC) $(CFLAGS) -o interrupt.o $(ARCH_DIR)/source/interrupt.asm
	$(CC) $(CFLAGS) -o mpu.o $(ARCH_DIR)/source/mpu.asm
	$(CC) $(CFLAGS) -o space_asm.o $(ARCH_DIR)/source/space_asm.asm
	$(CC) $(CFLAGS) -o HL_sys_core.o $(ARCH_DIR)/drivers/source/HL_sys_core.asm
	$(CC) $(CFLAGS) -o HL_sys_intvecs.o $(ARCH_DIR)/drivers/source/HL_sys_intvecs.asm
	$(CC) $(CFLAGS) -o HL_sys_mpu.o $(ARCH_DIR)/drivers/source/HL_sys_mpu.asm
	$(CC) $(CFLAGS) -o HL_sys_pmu.o $(ARCH_DIR)/drivers/source/HL_sys_pmu.asm
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/source/*.c \
		$(ARCH_DIR)/drivers/source/*.c \
		$(ARCH_DIR)/boards/armv7/*.c \