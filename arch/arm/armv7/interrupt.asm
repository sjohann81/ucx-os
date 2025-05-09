 .text
	.def    defaultint
	.asmfunc
defaultint:
	b defaultint

	.endasmfunc


;-------------------------------------------------------------------------------
; Disable IRQ interrupt
; SourceId : CORE_SourceId_025
; DesignId : CORE_DesignId_021
; Requirements: HL_CONQ_CORE_SR11

        .def _disable_IRQ_interrupt_
        .asmfunc

_disable_IRQ_interrupt_

        cpsid i
        bx    lr

        .endasmfunc

;-------------------------------------------------------------------------------
; Enable interrupts - CPU IRQ
; SourceId : CORE_SourceId_026
; DesignId : CORE_DesignId_022
; Requirements: HL_CONQ_CORE_SR8

       .def _enable_IRQ_interrupt_
       .asmfunc

_enable_IRQ_interrupt_

        cpsie i
        bx    lr

        .endasmfunc
;-------------------------------------------------------------------------------
; Enable interrupts - CPU IRQ & FIQ
; SourceId : CORE_SourceId_027
; DesignId : CORE_DesignId_024
; Requirements: HL_CONQ_CORE_SR10

       .def _enable_interrupt_
       .asmfunc

_enable_interrupt_

        cpsie if
        bx    lr

        .endasmfunc

;-------------------------------------------------------------------------------
; Disable interrupts - CPU IRQ & FIQ
; SourceId : CORE_SourceId_027
; DesignId : CORE_DesignId_024
; Requirements: HL_CONQ_CORE_SR10

       .def _disable_interrupt_
       .asmfunc

_disable_interrupt_

        cpsid if
        bx    lr

        .endasmfunc

	.def _get_cspr_
    .asmfunc
_get_cspr_:
	mrs r0, CPSR
	bx lr

	 .endasmfunc


	.ref pok_arch_sc_int
	.def pok_arch_sc_wrapper
	.asmfunc
pok_arch_sc_wrapper:
	srsfd	sp!, #0x13
	push    {r1-R12,lr}

	stmfd  r13, {r13,r14}^
	nop
	sub r13, r13, #8

	bl pok_arch_sc_int

	ldmfd r13, {r13, r14}^
	add r13, r13, #8

	pop	{r1-r12,lr}
	rfefd	sp!
	.endasmfunc


    .sect ".intvecs"
    .arm

	.ref _c_int00
; Reset Vector, jump to init
	b _c_int00
	b defaultint
	b pok_arch_sc_wrapper
	b defaultint
	b defaultint
	b defaultint
	b defaultint
	b defaultint


