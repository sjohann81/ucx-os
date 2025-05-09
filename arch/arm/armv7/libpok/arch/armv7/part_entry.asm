

.text
	.ref __TI_auto_init
	.ref __pok_partition_start

	.def    _c_int00
	.asmfunc
_c_int00:
	bl __TI_auto_init
	bl __pok_partition_start

loop:
	bl loop
	.endasmfunc
