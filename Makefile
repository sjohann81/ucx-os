#ARCH = avr/atmega328p
#ARCH = avr/atmega2560
#ARCH = mips/hf-risc
#ARCH = riscv/hf-riscv
ARCH = riscv/hf-riscv-e
#ARCH = riscv/hf-riscv-llvm
#ARCH = riscv/riscv32-qemu
#ARCH = riscv/riscv32-qemu-llvm
#ARCH = riscv/riscv64-qemu
#ARCH = riscv/riscv64-qemu-llvm

SERIAL_BAUD=57600
SERIAL_DEVICE=/dev/ttyUSB0

SRC_DIR = .

include $(SRC_DIR)/arch/$(ARCH)/arch.mak

INC_DIRS += -I $(SRC_DIR)/include

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
delay: hal ucx
	$(CC) $(CFLAGS) -o delay.o app/delay.c
	@$(MAKE) --no-print-directory link

hello: hal ucx
	$(CC) $(CFLAGS) -o hello.o app/hello.c
	@$(MAKE) --no-print-directory link

hello_p: hal ucx
	$(CC) $(CFLAGS) -o hello_preempt.o app/hello_preempt.c
	@$(MAKE) --no-print-directory link

mutex: hal ucx
	$(CC) $(CFLAGS) -o mutex.o app/mutex.c
	@$(MAKE) --no-print-directory link
	
pipes: hal ucx
	$(CC) $(CFLAGS) -o pipes.o app/pipes.c
	@$(MAKE) --no-print-directory link

pipes_s: hal ucx
	$(CC) $(CFLAGS) -o pipes_small.o app/pipes_small.c
	@$(MAKE) --no-print-directory link

pipes_struct: hal ucx
	$(CC) $(CFLAGS) -o pipes_struct.o app/pipes_struct.c
	@$(MAKE) --no-print-directory link

prodcons: hal ucx
	$(CC) $(CFLAGS) -o prodcons.o app/prodcons.c
	@$(MAKE) --no-print-directory link

progress: hal ucx
	$(CC) $(CFLAGS) -o progress.o app/progress.c
	@$(MAKE) --no-print-directory link
	
suspend: hal ucx
	$(CC) $(CFLAGS) -o suspend.o app/suspend.c
	@$(MAKE) --no-print-directory link

test_fixed: hal ucx
	$(CC) $(CFLAGS) -o test_fixed.o app/test_fixed.c
	@$(MAKE) --no-print-directory link

timer: hal ucx
	$(CC) $(CFLAGS) -o timer.o app/timer.c
	@$(MAKE) --no-print-directory link


clean:
	rm -rf *.o *~ *.elf *.bin *.cnt *.lst *.sec *.txt *.map *.hex
