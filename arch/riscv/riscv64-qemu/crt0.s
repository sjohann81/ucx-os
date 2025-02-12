	.global _entry
	.section .text.prologue
_entry:
	la	a3, _sbss
	la	a2, _ebss
	la	gp, _gp
	la	sp, _stack
	la	tp, _end + 63
	and	tp, tp, -64

BSS_CLEAR:
	# clear the .bss
	sw	zero, 0(a3)
	addi	a3, a3, 4
	blt	a3, a2, BSS_CLEAR

	# configure system status and clear interrupts
	li	t0, 0x1800
	csrw	mstatus, t0		# set MPP to machine mode
	csrw	mie, zero		# clear interrupts
	csrw	mip, zero

	# disable S-Mode interrupt and exec handling
	csrw	mideleg, zero
	csrw	medeleg, zero

	# stop all harts (except 0)
	csrr	t0, mhartid
	beq	zero, t0, _boothart0
	wfi
_boothart0:

	# setup trap vector
	la	t0, _isr
	csrw	mtvec, t0

	# enable MIE for external interrupts
	li	t0, 0x800
	csrw	mie, t0

	# jump to main
	jal	ra, main

	j	_panic
L1:
	wfi
	beq	zero, zero, L1

# interrupt / exception service routine
	.global _isr	
	.org 0x100
_isr:
	addi	sp, sp, -160
	sd	ra, 0(sp)
	sd	t0, 8(sp)
	sd	t1, 16(sp)
	sd	t2, 24(sp)
	sd	a0, 32(sp)
	sd	a1, 40(sp)
	sd	a2, 48(sp)
	sd	a3, 56(sp)
	sd	a4, 64(sp)
	sd	a5, 72(sp)
	sd	a6, 80(sp)
	sd	a7, 88(sp)
	sd	t3, 96(sp)
	sd	t4, 104(sp)
	sd	t5, 112(sp)
	sd	t6, 120(sp)

	csrr    a0, mcause
	csrr    a1, mepc
	sd	a0, 128(sp)
	sd	a1, 136(sp)
	
	jal	ra, _irq_handler

	ld	a1, 136(sp)
	ld	a0, 128(sp)
	csrw	mepc, a1
	csrw	mcause, a0

	ld	ra, 0(sp)
	ld	t0, 8(sp)
	ld	t1, 16(sp)
	ld	t2, 24(sp)
	ld	a0, 32(sp)
	ld	a1, 40(sp)
	ld	a2, 48(sp)
	ld	a3, 56(sp)
	ld	a4, 64(sp)
	ld	a5, 72(sp)
	ld	a6, 80(sp)
	ld	a7, 88(sp)
	ld	t3, 96(sp)
	ld	t4, 104(sp)
	ld	t5, 112(sp)
	ld	t6, 120(sp)
	addi	sp, sp, 160
	mret

	.global   setjmp
setjmp:
	sd    s0, 0(a0)
	sd    s1, 8(a0)
	sd    s2, 16(a0)
	sd    s3, 24(a0)
	sd    s4, 32(a0)
	sd    s5, 40(a0)
	sd    s6, 48(a0)
	sd    s7, 56(a0)
	sd    s8, 64(a0)
	sd    s9, 72(a0)
	sd    s10,80(a0)
	sd    s11,88(a0)
	sd    gp, 96(a0)
	sd    tp, 104(a0)
	sd    sp, 112(a0)
	sd    ra, 120(a0)

	csrr    tp, mcause
	sd	tp, 128(a0)
	csrr    tp, mepc
	sd	tp, 136(a0)

	ori   a0, zero, 0
	ret

	.global   longjmp
longjmp:
	ld    s0, 0(a0)
	ld    s1, 8(a0)
	ld    s2, 16(a0)
	ld    s3, 24(a0)
	ld    s4, 32(a0)
	ld    s5, 40(a0)
	ld    s6, 48(a0)
	ld    s7, 56(a0)
	ld    s8, 64(a0)
	ld    s9, 72(a0)
	ld    s10,80(a0)
	ld    s11,88(a0)
	ld    gp, 96(a0)
	ld    tp, 104(a0)
	ld    sp, 112(a0)
	ld    ra, 120(a0)
	
	ld	tp, 128(a0)
	csrw	mepc, tp
	ld	tp, 136(a0)
	csrw	mcause, tp	
	
	ori   a0, a1, 0
	ret
	
	.global   __dispatch_init
__dispatch_init:
	ld    s0, 0(a0)
	ld    s1, 8(a0)
	ld    s2, 16(a0)
	ld    s3, 24(a0)
	ld    s4, 32(a0)
	ld    s5, 40(a0)
	ld    s6, 48(a0)
	ld    s7, 56(a0)
	ld    s8, 64(a0)
	ld    s9, 72(a0)
	ld    s10,80(a0)
	ld    s11,88(a0)
	ld    gp, 96(a0)
	ld    tp, 104(a0)
	ld    sp, 112(a0)
	ld    ra, 120(a0)
#	addi  a5, zero, 128
#	csrw  mie, a5
	ret

	.text
