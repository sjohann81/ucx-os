	.global _entry
	.section .text.prologue
_entry:
	mov	r0, #0x08
	ldr	r1, =interrupt_table
	ldr	r3, =interrupt_table_end
keep_loading:
	ldr	r2, [r1, #0x0]
	str	r2, [r0, #0x0]
	add	r0, r0, #0x4
	add	r1, r1, #0x4
	cmp	r1, r3
	bne	keep_loading

	ldr	sp, =_stack
	bl	main

interrupt_table:
	nop
	nop
	nop
	nop
	ldr	pc, isr_entry_addr
	isr_entry_addr: .word _isr
interrupt_table_end:

# interrupt / exception service routine
	.global _isr
_isr:
	sub	lr, lr, #4
	srsfd	sp!, #0x13

	cps	#0x13
	push	{r0-r3, r12}

	mov	r1, sp
	and	r1, r1, #4
	sub	sp, sp, r1
	push	{r1, lr}

	bl	_irq_handler

	pop	{r1, lr}
	add	sp, sp, r1

	pop	{r0-r3, r12}
	
	rfefd	sp!

	.global _enable_interrupts
_enable_interrupts:
	mrs	r0, cpsr
	bic	r0, r0, #0x80
	msr	cpsr_c, r0
	mov	pc, lr

/*	.global _di	
_di:
	mrs	r0, cpsr
	mov	r1, r0
	orr	r1, r1, #0x80
	msr	cpsr, r1
	mov	pc, lr

	.global _ei
_ei:
	mrs	r1, cpsr
	bic	r0, r1, r0
	msr	cpsr, r0
	mov	pc, lr
*/

	.global setjmp
setjmp:
	stmia	r0, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	mov	r0, #0
	bx	lr

	.global longjmp
longjmp:
	ldmia	r0, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	mov	r0, r1
	bne	lje
	mov	r0, #1
lje:	
	movs	pc,lr

	.global __dispatch_init
__dispatch_init:
	ldmia	r0, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	mrs	r0, cpsr
	bic	r0, r0, #0x80
	msr	cpsr_c, r0
	movs	pc,lr

	.text
