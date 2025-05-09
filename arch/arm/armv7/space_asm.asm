 .text

	.sect ".shared_code"
 	.ref ja_space_switch

	.def    ja_user_space_jump
	.asmfunc
ja_user_space_jump:
	; R0 kernel SP
	; R1 space id
	; R2 User entry point (unsused)
	; R3 user SP

	; Set kernel SP
	mov sp, r0

	; Switch space
;	mov r0, r1
;	push {r2,r3}
;	bl ja_space_switch
;	pop {r2,r3}
	; Swith to user mode
	cpsie if
	cps #0x10

	; Set user SP
	mov sp, r3

	; Set user PC
	mov pc, r2

	; We should never return

	.endasmfunc
