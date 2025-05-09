
	.ref pok_arch_init
	.ref jet_boot

	.text
	.arm

; Kernel entry point
	.def    _c_int00
	.asmfunc
_c_int00:
	bl _init_cpu_arch
	bl _cpu_reg_init
	bl pok_arch_init
	bl jet_boot

	.endasmfunc

; IDLE routine
	.def    _idle_loop
	.asmfunc
_idle_loop:
	WFI
    nop
    nop
    nop
    nop
    b _idle_loop

    .endasmfunc


; Reset CPU registers
	.def    _cpu_reg_init
	.asmfunc
_cpu_reg_init:
	; Init shared registers
    mov r0,  lr
    mov r1,  #0x0
    mov r2,  #0x0
    mov r3,  #0x0
    mov r4,  #0x0
    mov r5,  #0x0
    mov r6,  #0x0
    mov r7,  #0x0
    mov r8,  #0x0
    mov r9,  #0x0
    mov r10, #0x0
    mov r11, #0x0
    mov r12, #0x0

    ; Init SVC specific registers
    ldr sp, svc_sp

    mrs r1, cpsr
    msr spsr_cxsf, r1

    ; Init FIQ specific registers
    cps #17
    mov r8,  #0x0
    mov r9,  #0x0
    mov r10, #0x0
    mov r11, #0x0
    mov r12, #0x0

    mov lr, r0
    ldr sp, fiq_sp

    mrs r1,  cpsr
    msr spsr_cxsf, r1

    ; Init IRQ specific registers
    cps #18

    mov lr, r0
    ldr sp, irq_sp

    mrs r1,cpsr
    msr spsr_cxsf, r1

    ; Init Abort specific registers
    cps #23

    mov lr, r0
    ldr sp, abort_sp

    mrs r1,cpsr
    msr spsr_cxsf, r1

    ; Init Undefined specific registers
    cps #27

    mov lr, r0
    ldr sp, undef_sp

    mrs r1,cpsr
    msr spsr_cxsf, r1

    ; Init System/User specific registers
    cps #31

    mov lr, r0
    ldr sp, user_sp

    mrs r1,cpsr
    msr spsr_cxsf, r1

    ; Switch back to SCV
    cps #19

	bx lr

	.endasmfunc

; Initialize the CPU settings
	.def    _init_cpu_arch
	.asmfunc

_init_cpu_arch:

	; Enable VIC
	mrc   p15, #0, r0, c1, c0,  #0
    orr   r0,  r0, #0x01000000
    mcr   p15, #0, r0, c1, c0,  #0
; Enable Cache

    mov r0, #0

    mrc   p15, #0, R1, c1, c0, #0   ; Read System Control Register configuration data
    orr   r1, r1, #0x1 <<12         ; instruction cache enable
    orr   r1, r1, #0x1 <<2          ; data cache enable
    dsb
    mcr   p15, #0, r0, c15, c5, #0  ; Invalidate entire data cache
    dsb                             ; delay is required, manually added
    mcr   p15, #0, r0, c7, c5, #0   ; Invalidate entire instruction cache
    dsb                             ; delay is required, manually added
    mcr   p15, #0, r1, c1, c0, #0   ; enabled cache RAMs
    isb

	bx lr

    .endasmfunc

    .def    __asm_barier__
	.asmfunc
__asm_barier__:
	dmb
	bx lr

	.endasmfunc

;--------------------------------------
; Stacks
;--------------------------------------
user_sp  .word 0x08001000
svc_sp   .word 0x08002000
fiq_sp   .word 0x08002800
irq_sp   .word 0x08003000
abort_sp .word 0x08003800
undef_sp .word 0x08004000
