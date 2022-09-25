TARGET_LIST = \
	'avr/atmega328p' 'avr/atmega2560' 'mips/hf-risc' \
	'riscv/hf-riscv' 'riscv/hf-riscv-e' 'riscv/hf-riscv-llvm' \
	'riscv/riscv32-qemu' 'riscv/riscv32-qemu-llvm' \
	'riscv/riscv64-qemu' 'riscv/riscv64-qemu-llvm'

ARCH = none

SERIAL_BAUD=57600
SERIAL_DEVICE=/dev/ttyUSB0

SRC_DIR = .

ifneq ('$(ARCH)', 'none')
include $(SRC_DIR)/arch/$(ARCH)/arch.mak
INC_DIRS += -I $(SRC_DIR)/include
endif

incl:
ifeq ('$(ARCH)', 'none')
	@echo "You must specify a target architecture (ARCH='arch/target')."
	@echo "Supported targets are: $(TARGET_LIST)"
endif

serial:
	stty ${SERIAL_BAUD} raw cs8 -parenb -crtscts clocal cread ignpar ignbrk -ixon -ixoff -ixany -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke -F ${SERIAL_DEVICE}

load: serial
	cat image.bin > $(SERIAL_DEVICE)

debug: serial
	cat ${SERIAL_DEVICE}

## RISC-V / Qemu
run_riscv32:
	echo "hit Ctrl+a x to quit"
	qemu-system-riscv32 -machine virt -nographic -bios image.bin -serial mon:stdio

run_riscv64:
	echo "hit Ctrl+a x to quit"
	qemu-system-riscv64 -machine virt -nographic -bios image.bin -serial mon:stdio

## kernel
ucx:
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/lib/libc.c \
		$(SRC_DIR)/lib/dump.c \
		$(SRC_DIR)/lib/malloc.c \
		$(SRC_DIR)/lib/list.c \
		$(SRC_DIR)/lib/queue.c \
		$(SRC_DIR)/kernel/pipe.c \
		$(SRC_DIR)/kernel/semaphore.c \
		$(SRC_DIR)/kernel/task.c \
		$(SRC_DIR)/kernel/ucx.c
		
## kernel + application link
link:
ifeq ('$(ARCH)', 'avr/atmega328p')
	$(LD) $(LDFLAGS) -o image.elf *.o
else ifeq ('$(ARCH)', 'avr/atmega2560')
	$(LD) $(LDFLAGS) -o image.elf *.o
else 
	$(LD) $(LDFLAGS) -T$(LDSCRIPT) -Map image.map -o image.elf *.o
endif
	$(DUMP) --disassemble --reloc image.elf > image.lst
	$(DUMP) -h image.elf > image.sec
	$(DUMP) -s image.elf > image.cnt
	$(OBJ) -O binary image.elf image.bin
	$(OBJ) -R .eeprom -O ihex image.elf image.hex
	$(SIZE) image.elf
#	hexdump -v -e '4/1 "%02x" "\n"' image.bin > image.txt

## applications
delay: incl hal ucx
	$(CC) $(CFLAGS) -o delay.o app/delay.c
	@$(MAKE) --no-print-directory link

hello: incl hal ucx
	$(CC) $(CFLAGS) -o hello.o app/hello.c
	@$(MAKE) --no-print-directory link

hello_p: incl hal ucx
	$(CC) $(CFLAGS) -o hello_preempt.o app/hello_preempt.c
	@$(MAKE) --no-print-directory link

mutex: incl hal ucx
	$(CC) $(CFLAGS) -o mutex.o app/mutex.c
	@$(MAKE) --no-print-directory link
	
pipes: incl hal ucx
	$(CC) $(CFLAGS) -o pipes.o app/pipes.c
	@$(MAKE) --no-print-directory link

pipes_s: incl hal ucx
	$(CC) $(CFLAGS) -o pipes_small.o app/pipes_small.c
	@$(MAKE) --no-print-directory link

pipes_struct: incl hal ucx
	$(CC) $(CFLAGS) -o pipes_struct.o app/pipes_struct.c
	@$(MAKE) --no-print-directory link

prodcons: incl hal ucx
	$(CC) $(CFLAGS) -o prodcons.o app/prodcons.c
	@$(MAKE) --no-print-directory link

progress: incl hal ucx
	$(CC) $(CFLAGS) -o progress.o app/progress.c
	@$(MAKE) --no-print-directory link
	
suspend: incl hal ucx
	$(CC) $(CFLAGS) -o suspend.o app/suspend.c
	@$(MAKE) --no-print-directory link

test_fixed: incl hal ucx
	$(CC) $(CFLAGS) -o test_fixed.o app/test_fixed.c
	@$(MAKE) --no-print-directory link

timer: incl hal ucx
	$(CC) $(CFLAGS) -o timer.o app/timer.c
	@$(MAKE) --no-print-directory link

clean:
	rm -rf *.o *~ *.elf *.bin *.cnt *.lst *.sec *.txt *.map *.hex
