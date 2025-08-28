/* Describes the target system's memory map. */
MEMORY
{
	FLASH (rx)    : ORIGIN = 0x08000000, LENGTH = 256K
	RAM (rwx)     : ORIGIN = 0x20000000, LENGTH = 64K
	CCM_RAM (rwx) : ORIGIN = 0x10000000, LENGTH = 0K
}

/* Entry Point */
ENTRY(Reset_Handler)

/* Defines beginning and ending of stack. */
_stack_size  = 8K;
_stack_start = ORIGIN(RAM) + LENGTH(RAM);
_stack_end   = _stack_start - _stack_size;

/* Defines beginning and ending of heap. */
_heap_start = _ebss;
_heap_end   = _stack_end;
_heap_size  = _stack_end - _ebss;

/* Describes the placement of each output section, including the input sections which are inserted into them */
SECTIONS
{
	/* The startup code goes first into FLASH. */
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector)) /* Startup code */
		. = ALIGN(4);
	} > FLASH

	/* The ".text" section is used for the reset vector, the code, and only (.rodata) data */
	.text :
	{
		/* CREATE_OBJECT_SYMBOLS */
		*(.text)
		*(.text.*)
		*(.rodata)        /* .rodata sections (constants, strings, etc.) */
		*(.rodata.*)
		*(.strings)
		*(.glue_7)
		*(.glue_7t)
		*(.eh_frame_hdr)
		*(.eh_frame)

		KEEP (*(.init))
		KEEP (*(.fini))

		. = ALIGN(4);
		_etext = .;
	} > FLASH

	/* Used by the startup to initialize data, position in which the contents of .data section
	 * must be located.
	 */
	_sidata = .;

	/* The ".data" section is used for initialized data
 	 * section contents (initial values) -> FLASH
 	 * section alocated block -> ram
	 */
	.data : AT (_sidata)
	{
		. = ALIGN(4);
		_sdata = .;

		*(.data)
		*(.data.*)

		. = ALIGN(4);
		_edata = .;
	} > RAM

	/* The ".bss" section, which is cleared by the startup code, is used for storing uninitialized data */
	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		_sbss = .;

		*(.bss)
		*(.bss.*)
		*(COMMON)

		. = ALIGN(4);
		_ebss = .;
	} > RAM

	/* The ".heap" section is the area reserved for the HEAP memory. It starts right after the BSS section, in RAM. */
	. = _heap_start;
	.heap . (NOLOAD) :
	{
		. = ALIGN(4);
		PROVIDE(end = .);
		PROVIDE(_end = .);
		. = . + _heap_size;
		. = ALIGN(4);
	} > RAM

	/* The ".stack" section is the area reserved for the STACK memory. It starts at the end of the RAM memory. */
	. = _stack_end;
	.stack . (NOLOAD) :
	{
		. = ALIGN(4);
		. = . + _stack_size;
		. = ALIGN(4);
	} > RAM
}
