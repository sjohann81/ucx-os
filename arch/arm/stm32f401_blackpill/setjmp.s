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
	/* Do not trash r4 .. r7 */
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
