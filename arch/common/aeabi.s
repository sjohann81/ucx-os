	.text
	.balign 4
	.globl __aeabi_idivmod
	.thumb_func
	.syntax unified
__aeabi_idivmod:
        push    {lr}
        sub     sp, sp, #4
        mov     r2, sp
        bl      __divmodsi4
        ldr     r1, [sp]
        add     sp, sp, #4
        pop     {pc}

	.text
	.balign 4
	.globl __aeabi_ldivmod
	.thumb_func
	.syntax unified
__aeabi_ldivmod:
        push    {r11, lr}
        sub     sp, sp, #16
        add     r12, sp, #8
        str     r12, [sp]
        bl      __divmoddi4
        ldr     r2, [sp, #8]
        ldr     r3, [sp, #12]
        add     sp, sp, #16
        pop     {r11, pc}

	.text
	.balign 4
	.globl __aeabi_uidivmod
	.thumb_func
	.syntax unified
__aeabi_uidivmod:
        push    {lr}
        sub     sp, sp, #4
        mov     r2, sp
        bl      __udivmodsi4
        ldr     r1, [sp]
        add     sp, sp, #4
        pop     {pc}

	.text
	.balign 4
	.globl __aeabi_uldivmod
	.thumb_func
	.syntax unified
__aeabi_uldivmod:
        push	{r11, lr}
        sub	sp, sp, #16
        add	r12, sp, #8
        str	r12, [sp]
        bl	__udivmoddi4
        ldr	r2, [sp, #8]
        ldr	r3, [sp, #12]
        add	sp, sp, #16
        pop	{r11, pc}
