MEMORY
{
	ram (rwx)     : ORIGIN = 0x80000000, LENGTH = 128M
}

/* Entry Point */
ENTRY(_entry)

/* Defines beginning and ending of stack. */
_stack_size  = 4M;
_stack = _stack_start;
_stack_start = ORIGIN(ram) + LENGTH(ram);
_stack_end   = _stack_start - _stack_size;

/* Defines beginning and ending of heap. */
_heap_start = _ebss;
_heap_end   = _stack_end;
_heap_size  = _stack_end - _ebss;

SECTIONS
{
	.text : {
		_text = .;
		*(.text.prologue);
		*(.text.init)
		*(.text)
		*(.text.*)
		_etext = .;
	} > ram
	
	.rodata : {
		. = ALIGN(16);
		_rodata = .;
		*(.rdata)
		*(.rodata)
		*(.rodata*)
		. = ALIGN(16);
		_erodata = .;
	} > ram

	.data : {
		. = ALIGN(16);
		_data = .;
		*(.data)
		*(.data*)
		_gp = . + 0x800;
		*(.sdata)
		*(.sdata.*)
		. = ALIGN(16);
		_edata = .;
	} > ram
	
	.bss : {
		. = ALIGN(16);
		_sbss = .;
		*(.sbss)
		*(.sbss.*)
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(16);
		_ebss = .;
	} > ram
	
	_end = .;
}
