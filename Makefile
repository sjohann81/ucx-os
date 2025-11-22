VERSION = 1.00

TARGET_LIST = \
	'arm/stm32f401_blackpill' 'arm/stm32f401_nucleo' \
	'arm/stm32f411_blackpill' 'arm/stm32f407_discovery' \
	'arm/versatilepb' \
	'avr/atmega32' 'avr/atmega328p' 'avr/atmega2560' \
	'mips/hf-risc' 'riscv/hf-riscv' 'riscv/hf-riscv-e' \
	'riscv/hf-riscv-llvm' 'riscv/riscv32-qemu' 'riscv/riscv32-qemu-llvm' \
	'riscv/riscv64-qemu' 'riscv/riscv64-qemu-llvm'

#ARCH = none

SERIAL_BAUD=57600
SERIAL_DEVICE=/dev/ttyUSB0

SRC_DIR = .

BUILD_DIR = $(SRC_DIR)/build
BUILD_APP_DIR = $(BUILD_DIR)/app
BUILD_HAL_DIR = $(BUILD_DIR)/hal
BUILD_DRIVERS_DIR = $(BUILD_DIR)/drivers
BUILD_NET_DIR = $(BUILD_DIR)/net
BUILD_KERNEL_DIR = $(BUILD_DIR)/kernel
BUILD_TARGET_DIR = $(BUILD_DIR)/target

-include $(BUILD_TARGET_DIR)/target.mak
-include $(SRC_DIR)/arch/$(ARCH)/arch.mak
-include $(SRC_DIR)/drivers/drivers.mak
-include $(SRC_DIR)/net/net.mak
INC_DIRS += -I $(SRC_DIR)/include -I $(SRC_DIR)/include/lib \
	-I $(SRC_DIR)/drivers/bus/include -I $(SRC_DIR)/drivers/device/include \
	-I $(SRC_DIR)/net/include -I $(SRC_DIR)/arch/common
CFLAGS += -D__VER__=\"$(VERSION)\" \
#-DCONFIG_POWER_ALLOC \
#-DCONFIG_ALT_ALLOCATOR \
#-DCONFIG_SCHED_SIMPLE

incl:
ifeq ('$(ARCH)', 'none')
	@echo "You must specify a target architecture (ARCH=arch/target)."
	@echo "Supported targets are: $(TARGET_LIST)"
else
	@echo "ARCH = $(ARCH)" > $(BUILD_TARGET_DIR)/target.mak
endif

serial:
	stty ${SERIAL_BAUD} raw cs8 -parenb -crtscts clocal cread ignpar ignbrk -ixon -ixoff -ixany -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke -F ${SERIAL_DEVICE}

load: serial
	cat image.bin > $(SERIAL_DEVICE)

debug: serial
	cat ${SERIAL_DEVICE}

## kernel
ucx: incl hal libs ddrivers network kernel
	mv *.o $(SRC_DIR)/build/kernel
	$(AR) $(ARFLAGS) $(BUILD_TARGET_DIR)/libucxos.a \
		$(BUILD_KERNEL_DIR)/*.o

kernel: timer.o message.o pipe.o spinlock.o semaphore.o ecodes.o syscall.o coroutine.o ucx.o main.o

main.o: $(SRC_DIR)/init/main.c
	$(CC) $(CFLAGS) $(SRC_DIR)/init/main.c
ucx.o: $(SRC_DIR)/kernel/ucx.c
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/ucx.c
coroutine.o:
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/coroutine.c
syscall.o: $(SRC_DIR)/kernel/syscall.c
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/syscall.c
ecodes.o: $(SRC_DIR)/kernel/ecodes.c
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/ecodes.c
semaphore.o: $(SRC_DIR)/kernel/semaphore.c
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/semaphore.c
spinlock.o: $(SRC_DIR)/kernel/spinlock.c
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/spinlock.c
pipe.o: $(SRC_DIR)/kernel/pipe.c
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/pipe.c
message.o: $(SRC_DIR)/kernel/message.c
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/message.c
timer.o: $(SRC_DIR)/kernel/timer.c
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel/timer.c

libs: console.o libc.o fixed.o dump.o malloc.o list.o queue.o

queue.o: $(SRC_DIR)/lib/queue.c
	$(CC) $(CFLAGS) $(SRC_DIR)/lib/queue.c
list.o: $(SRC_DIR)/lib/list.c
	$(CC) $(CFLAGS) $(SRC_DIR)/lib/list.c
malloc.o: $(SRC_DIR)/lib/malloc.c
	$(CC) $(CFLAGS) $(SRC_DIR)/lib/malloc.c
dump.o: $(SRC_DIR)/lib/dump.c
	$(CC) $(CFLAGS) $(SRC_DIR)/lib/dump.c
fixed.o:$(SRC_DIR)/lib/fixed.c
	$(CC) $(CFLAGS) $(SRC_DIR)/lib/fixed.c
libc.o: $(SRC_DIR)/lib/libc.c
	$(CC) $(CFLAGS) $(SRC_DIR)/lib/libc.c
console.o: $(SRC_DIR)/lib/console.c
	$(CC) $(CFLAGS) $(SRC_DIR)/lib/console.c
		
## kernel + application link
link:
ifeq ('$(ARCH)', 'avr/atmega32')
	$(LD) $(LDFLAGS) -o $(BUILD_TARGET_DIR)/image.elf $(BUILD_APP_DIR)/*.o -L$(BUILD_TARGET_DIR) -lucxos
else ifeq ('$(ARCH)', 'avr/atmega328p')
	$(LD) $(LDFLAGS) -o $(BUILD_TARGET_DIR)/image.elf $(BUILD_APP_DIR)/*.o -L$(BUILD_TARGET_DIR) -lucxos
else ifeq ('$(ARCH)', 'avr/atmega2560')
	$(LD) $(LDFLAGS) -o $(BUILD_TARGET_DIR)/image.elf $(BUILD_APP_DIR)/*.o -L$(BUILD_TARGET_DIR) -lucxos
else 
	$(LD) $(LDFLAGS) -T$(LDSCRIPT) -Map $(BUILD_TARGET_DIR)/image.map -o $(BUILD_TARGET_DIR)/image.elf $(BUILD_APP_DIR)/*.o -L$(BUILD_TARGET_DIR) -lucxos
endif
	$(DUMP) --disassemble --reloc $(BUILD_TARGET_DIR)/image.elf > $(BUILD_TARGET_DIR)/image.lst
	$(DUMP) -h $(BUILD_TARGET_DIR)/image.elf > $(BUILD_TARGET_DIR)/image.sec
	$(DUMP) -s $(BUILD_TARGET_DIR)/image.elf > $(BUILD_TARGET_DIR)/image.cnt
	$(OBJ) -O binary $(BUILD_TARGET_DIR)/image.elf $(BUILD_TARGET_DIR)/image.bin
	$(OBJ) -R .eeprom -O ihex $(BUILD_TARGET_DIR)/image.elf $(BUILD_TARGET_DIR)/image.hex
	$(SIZE) $(BUILD_TARGET_DIR)/image.elf
	hexdump -v -e '4/1 "%02x" "\n"' $(BUILD_TARGET_DIR)/image.bin > $(BUILD_TARGET_DIR)/code.txt

## applications
coroutine_args: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/coroutine_args.o app/coroutine_args.c
	@$(MAKE) --no-print-directory link

coroutine_multicore: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/coroutine_multicore.o app/coroutine_multicore.c
	@$(MAKE) --no-print-directory link

coroutine_mq: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/coroutine_mq.o app/coroutine_mq.c
	@$(MAKE) --no-print-directory link

coroutine_pipe: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/coroutine_pipe.o app/coroutine_pipe.c
	@$(MAKE) --no-print-directory link

coroutine_task: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/coroutine_task.o app/coroutine_task.c
	@$(MAKE) --no-print-directory link

delay: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/delay.o app/delay.c
	@$(MAKE) --no-print-directory link

dht_sensor: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/dht_sensor.o app/dht_sensor.c
	@$(MAKE) --no-print-directory link

driver: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/driver.o app/driver/driver.c
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/app.o app/driver/app.c
	@$(MAKE) --no-print-directory link

echo: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/echo.o app/echo.c
	@$(MAKE) --no-print-directory link

gpio_blink: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/gpio_blink.o app/gpio_blink.c
	@$(MAKE) --no-print-directory link

gpio_blinkseq: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/gpio_blinkseq.o app/gpio_blinkseq.c
	@$(MAKE) --no-print-directory link

gpio_blinkseqkey: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/gpio_blinkseqkey.o app/gpio_blinkseqkey.c
	@$(MAKE) --no-print-directory link
	
gpio_int: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/gpio_int.o app/gpio_int.c
	@$(MAKE) --no-print-directory link

hello: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/hello.o app/hello.c
	@$(MAKE) --no-print-directory link

hello_multicore: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/hello_multicore.o app/hello_multicore.c
	@$(MAKE) --no-print-directory link

hello_p: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/hello_preempt.o app/hello_preempt.c
	@$(MAKE) --no-print-directory link

i2c_eeprom: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/i2c_eeprom.o app/i2c_eeprom.c
	@$(MAKE) --no-print-directory link
	
i2c_master: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/i2c_master.o app/i2c_master.c
	@$(MAKE) --no-print-directory link
	
i2c_simple: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/i2c_simple.o app/i2c_simple.c
	@$(MAKE) --no-print-directory link
	
messages: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/messages.o app/messages.c
	@$(MAKE) --no-print-directory link

messages_alloc: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/messages_alloc.o app/messages_alloc.c
	@$(MAKE) --no-print-directory link

messages_simple: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/messages_simple.o app/messages_simple.c
	@$(MAKE) --no-print-directory link
	
mutex: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/mutex.o app/mutex.c
	@$(MAKE) --no-print-directory link
	
pipes: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/pipes.o app/pipes.c
	@$(MAKE) --no-print-directory link

pipes_s: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/pipes_small.o app/pipes_small.c
	@$(MAKE) --no-print-directory link

pipes_struct: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/pipes_struct.o app/pipes_struct.c
	@$(MAKE) --no-print-directory link

prodcons: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/prodcons.o app/prodcons.c
	@$(MAKE) --no-print-directory link

progress: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/progress.o app/progress.c
	@$(MAKE) --no-print-directory link

pwm_blink: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/pwm_blink.o app/pwm_blink.c
	@$(MAKE) --no-print-directory link
	
rtsched: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/rtsched.o app/rtsched.c
	@$(MAKE) --no-print-directory link

serial_inet: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/serial_inet.o app/serial_inet.c
	@$(MAKE) --no-print-directory link
	
spi_master: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/spi_master.o app/spi_master.c
	@$(MAKE) --no-print-directory link
	
spi_slave: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/spi_slave.o app/spi_slave.c
	@$(MAKE) --no-print-directory link
	
spi_periph: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/spi_periph.o app/spi_periph.c
	@$(MAKE) --no-print-directory link
	
spi_eeprom: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/spi_eeprom.o app/spi_eeprom.c
	@$(MAKE) --no-print-directory link

spi_enc28j60: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/spi_enc28j60.o app/spi_enc28j60.c
	@$(MAKE) --no-print-directory link

spi_enc28j60_inet: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/spi_enc28j60_inet.o app/spi_enc28j60_inet.c
	@$(MAKE) --no-print-directory link
		
suspend: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/suspend.o app/suspend.c
	@$(MAKE) --no-print-directory link

test64: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/test64.o app/test64.c
	@$(MAKE) --no-print-directory link

test_fixed: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/test_fixed.o app/test_fixed.c
	@$(MAKE) --no-print-directory link
	
test_fp: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/test_fp.o app/test_fp.c
	@$(MAKE) --no-print-directory link

timer: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/timer.o app/timer.c
	@$(MAKE) --no-print-directory link
	
timer_systick: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/timer_systick.o app/timer_systick.c
	@$(MAKE) --no-print-directory link

timer_uptime: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/timer_uptime.o app/timer_uptime.c
	@$(MAKE) --no-print-directory link

timer_kill: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/timer_kill.o app/timer_kill.c
	@$(MAKE) --no-print-directory link

scall_suspend: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/scall_suspend.o app/scall_suspend.c
	@$(MAKE) --no-print-directory link
	
vt100_term: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/vt100_term.o app/vt100_term.c
	@$(MAKE) --no-print-directory link
	
vt100_term_ioctl: rebuild
	$(CC) $(CFLAGS) -o $(BUILD_APP_DIR)/vt100_term_ioctl.o app/vt100_term_ioctl.c
	@$(MAKE) --no-print-directory link
	
# network tunnel
tuntap_host:
	gcc -Wall app/tuntap_if_host.c -o tuntap_if_host

# needs setcap to allow non root access
# sudo apt-get install libcap2-bin
tuntap_cap:
	setcap cap_net_raw,cap_net_admin=eip tuntap_if_host
	
eth_up:
	stty -F ${SERIAL_DEV} ${SERIAL_BR} raw cs8 -echo
	./tuntap_if_host ${SERIAL_DEV}

# clean and rebuild rules
rebuild:
	find '$(BUILD_APP_DIR)' -type f -name '*.o' -delete

clean:
	find '$(BUILD_APP_DIR)' '$(BUILD_KERNEL_DIR)' -type f -name '*.o' -delete
	find '$(BUILD_TARGET_DIR)' -type f -name '*.o' -delete -o -name '*~' \
		-delete -o -name 'image.*' -delete -o -name 'code.*' -delete
	find '$(SRC_DIR)' -type f -name '*.o' -delete

veryclean: clean
	echo "ARCH = none" > $(BUILD_TARGET_DIR)/target.mak
	find '$(BUILD_TARGET_DIR)' -type f -name '*.a' -delete
