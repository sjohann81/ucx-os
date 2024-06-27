	.text
	.balign 4
	.globl setjmp
	.thumb_func
setjmp:
	mov	r2, r0
	mov	r3, lr
	stmia	r0!, {r3, r4, r5, r6, r7}
	mov	r3, r8
	mov	r4, r9
	mov	r5, r10
	mov	r6, fp
	mov	r7, sp
	stmia	r0!, {r3, r4, r5, r6, r7}
	/* Do not trash r3 .. r7 */
	ldmia	r2!, {r3, r4, r5, r6, r7}
	mov	r0, #0
	bx	lr

	.text
	.balign 4
	.globl longjmp
	.thumb_func
longjmp:
	mov	r2, r0
	add	r0, #5*4
	ldmia	r0!, {r3, r4, r5, r6, r7}
	mov	r8, r3
	mov	r9, r4
	mov	r10, r5
	mov	fp, r6
	mov	sp, r7
	ldmia	r2!, {r3, r4, r5, r6, r7}
	mov	r0, r1
	bne	1f
	mov	r0, #1
1:	bx	r3


/* no fpu */
	.text
	.balign 4
	.globl PendSV_Handler
	.thumb_func
	.syntax unified
PendSV_Handler:
	mrs	r0, psp
	stmdb	r0!, {r4-r11}
	ldr	r1, =task_psp
	ldr	r1, [r1]
	str	r0, [r1]
	ldr	r1, =new_task_psp
	ldr	r1, [r1]
	ldr	r0, [r1]
	ldmia	r0!, {r4-r11}
	msr	psp, r0
	bx	lr


/* context + FPU regs */
/* FIXME: doesn't work. */
/*	.text
	.balign 4
	.globl PendSV_Handler
	.thumb_func
	.syntax unified
PendSV_Handler:
	mrs	r0, psp
	
	ldr	r1, =0xe000ef00
	ldr	r2, [r1, #52]
	bic.w	r2, r2, #1
	str	r2, [r1, #52]

	vstm	r0!, {s0-s31}
	stmdb	r0!, {r4-r11}
	ldr	r1, =task_psp
	ldr	r1, [r1]
	str	r0, [r1]
	ldr	r1, =new_task_psp
	ldr	r1, [r1]
	ldr	r0, [r1]
	ldmia	r0!, {r4-r11}
	vldm	r0, {s0-s31}

	ldr	r1, =0xe000ef00
	ldr	r2, [r1, #52]
	orr.w	r2, r2, #1
	str	r2, [r1, #52]

	msr	psp, r0
	bx lr
*/

/* context + FPU regs */
/* FIXME: this routine is testing if the FPU is being used by the current
   task, and optionally loads/stores the state on the stack. It doesn't work.
*/
/*	.text
	.balign 4
	.globl PendSV_Handler
	.thumb_func
	.syntax unified
PendSV_Handler:
	mrs	r0, psp
	isb
	tst	r14, #0x10
	it	eq
	vstmdbeq r0!, {s16-s31} 
	stmdb	r0!, {r4-r11, r14}
	ldr	r1, =task_psp
	ldr	r1, [r1]
	str	r0, [r1]
	ldr	r1, =new_task_psp
	ldr	r1, [r1]
	ldr	r0, [r1]
	ldmia	r0!, {r4-r11, r14}
	tst	r14, #0x10
	it	eq
	vldmiaeq r0!, {s16-s31}
	msr	psp, r0
	isb
	bx	lr
*/
