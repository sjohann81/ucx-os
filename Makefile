ARCH = riscv/hf-riscv

SERIAL_BAUD=57600
SERIAL_DEVICE=/dev/ttyUSB0

SRC_DIR = $(CURDIR)

include $(SRC_DIR)/arch/$(ARCH)/arch.mak

INC_DIRS += -I $(SRC_DIR)/include

serial:
	stty ${SERIAL_BAUD} raw cs8 -parenb -crtscts clocal cread ignpar ignbrk -ixon -ixoff -ixany -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke -F ${SERIAL_DEVICE}

load: serial
	cat image.bin > $(SERIAL_DEVICE)

debug: serial
	cat ${SERIAL_DEVICE}
	
ucx:
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/lib/libc.c \
		$(SRC_DIR)/lib/malloc.c \
		$(SRC_DIR)/kernel/list.c \
		$(SRC_DIR)/kernel/queue.c \
		$(SRC_DIR)/kernel/pipe.c \
		$(SRC_DIR)/kernel/ucx.c

link:
	$(LD) $(LDFLAGS) -T$(LDSCRIPT) -Map image.map -o image.elf *.o
	$(DUMP) --disassemble --reloc image.elf > image.lst
	$(DUMP) -h image.elf > image.sec
	$(DUMP) -s image.elf > image.cnt
	$(OBJ) -O binary image.elf image.bin
	$(SIZE) image.elf
	hexdump -v -e '4/1 "%02x" "\n"' image.bin > image.txt

hello: hal ucx
	$(CC) $(CFLAGS) -o hello.o app/hello.c
	@$(MAKE) --no-print-directory link

pipes: hal ucx
	$(CC) $(CFLAGS) -o pipes.o app/pipes.c
	@$(MAKE) --no-print-directory link

clean:
	rm -rf *.o *~ *.elf *.bin *.cnt *.lst *.sec *.txt *.map
