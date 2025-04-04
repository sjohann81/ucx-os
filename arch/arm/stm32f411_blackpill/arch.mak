# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS  = -I $(ARCH_DIR) \
	-I $(ARCH_DIR)/../stm32f401_blackpill/drivers \
	-I $(ARCH_DIR)/../../common \
	-I $(ARCH_DIR)/../../common/stm32/cmsis/core \
	-I $(ARCH_DIR)/../../common/stm32/cmsis/device \
	-I $(ARCH_DIR)/../../common/stm32/usb_cdc

# serial port
SERIAL_DEV = /dev/ttyACM0
# uart baud rate
SERIAL_BR = 115200
# uart port
SERIAL_PORT = 0
# timer interrupt frequency (100 -> 100 ints/s -> 10ms tick time)
F_TICK = 100

#remove unreferenced functions
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
#MCU_DEFINES = -mcpu=cortex-m4 -mtune=cortex-m4 -mfloat-abi=hard -mthumb -fsingle-precision-constant -mfpu=fpv4-sp-d16 -Wdouble-promotion
MCU_DEFINES = -mcpu=cortex-m4 -mtune=cortex-m4 -mfloat-abi=soft -mabi=atpcs -mthumb -fsingle-precision-constant
C_DEFINES = -D STM32F411xE -D HSE_VALUE=25000000 -D USB_SERIAL
CFLAGS = -Wall -O2 -c $(MCU_DEFINES) -mapcs-frame -fverbose-asm -nostdlib -ffreestanding $(C_DEFINES) $(INC_DIRS) -D USART_BAUD=$(SERIAL_BR) -D USART_PORT=$(SERIAL_PORT) -DF_TIMER=${F_TICK} -DLITTLE_ENDIAN $(CFLAGS_STRIP)

LDFLAGS = $(LDFLAGS_STRIP)
LDSCRIPT = $(ARCH_DIR)/stm32f4_flash.ld
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
	$(CC) $(CFLAGS) -o setjmp.o $(ARCH_DIR)/../stm32f401_blackpill/setjmp.s
	$(CC) $(CFLAGS) -o aeabi.o $(ARCH_DIR)/../../common/aeabi.s
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/../stm32f401_blackpill/hal.c \
		$(ARCH_DIR)/../stm32f401_blackpill/usart.c \
		$(ARCH_DIR)/../stm32f401_blackpill/drivers/gpio_ll.c \
		$(ARCH_DIR)/../stm32f401_blackpill/drivers/i2c_ll.c \
		$(ARCH_DIR)/../stm32f401_blackpill/drivers/pwm_ll.c \
		$(ARCH_DIR)/../../common/muldiv.c \
		$(ARCH_DIR)/../../common/ieee754.c \
		$(ARCH_DIR)/../../common/math.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_rcc.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_gpio.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_tim.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_adc.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_i2c.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_spi.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_usart.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_syscfg.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/stm32f4xx_exti.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/misc.c \
		$(ARCH_DIR)/../../common/stm32/cmsis/device/system_stm32f4xx.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usb_bsp.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usb_core.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usb_dcd.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usb_dcd_int.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usbd_cdc_core.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usbd_cdc_vcp.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usbd_core.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usbd_desc.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usbd_ioreq.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usbd_req.c \
		$(ARCH_DIR)/../../common/stm32/usb_cdc/usbd_usr.c

usb_serial:
	stty -F ${SERIAL_DEV} ${SERIAL_BR} raw cs8 -echo

usb_load:
	echo '' > ${SERIAL_DEV}
	cat ${SERIAL_DEV}

flash:
	dfu-util -a 0 -s 0x08000000:leave -D build/target/image.bin
