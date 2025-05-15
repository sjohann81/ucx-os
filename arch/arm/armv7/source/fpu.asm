;
; fpu.asm
;
;  Created on: 15 mai 2019
;      Author: Alexy

.text

	.def    ja_fp_save
	.asmfunc
ja_fp_save:
	fmrrd r1, r2, d0
	str r1, [r0, #0]
	str r2, [r0, #4]

	fmrrd r1, r2, d1
	str r1, [r0, #8]
	str r2, [r0, #12]

	fmrrd r1, r2, d2
	str r1, [r0, #16]
	str r2, [r0, #20]

	fmrrd r1, r2, d3
	str r1, [r0, #24]
	str r2, [r0, #28]

	fmrrd r1, r2, d4
	str r1, [r0, #32]
	str r2, [r0, #36]

	fmrrd r1, r2, d5
	str r1, [r0, #40]
	str r2, [r0, #48]

	fmrrd r1, r2, d6
	str r1, [r0, #52]
	str r2, [r0, #56]

	fmrrd r1, r2, d7
	str r1, [r0, #60]
	str r2, [r0, #64]

	fmrrd r1, r2, d8
	str r1, [r0, #68]
	str r2, [r0, #72]

	fmrrd r1, r2, d9
	str r1, [r0, #76]
	str r2, [r0, #80]

	fmrrd r1, r2, d10
	str r1, [r0, #84]
	str r2, [r0, #88]

	fmrrd r1, r2, d11
	str r1, [r0, #92]
	str r2, [r0, #96]

	fmrrd r1, r2, d12
	str r1, [r0, #100]
	str r2, [r0, #104]

	fmrrd r1, r2, d13
	str r1, [r0, #108]
	str r2, [r0, #112]

	fmrrd r1, r2, d14
	str r1, [r0, #116]
	str r2, [r0, #120]

	fmrrd r1, r2, d15
	str r1, [r0, #124]
	str r2, [r0, #128]

	bx lr

	.endasmfunc

	.def    ja_fp_restore
	.asmfunc
ja_fp_restore:
	ldr r1, [r0, #0]
	ldr r2, [r0, #4]

	fmdrr d0, r1, r2
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]

	fmdrr d1, r1, r2
	ldr r1, [r0, #16]
	ldr r2, [r0, #20]

	fmdrr d2, r1, r2
	ldr r1, [r0, #24]
	ldr r2, [r0, #28]

	fmdrr d3, r1, r2
	ldr r1, [r0, #32]
	ldr r2, [r0, #36]

	fmdrr d4, r1, r2
	ldr r1, [r0, #40]
	ldr r2, [r0, #48]

	fmdrr d5, r1, r2
	ldr r1, [r0, #52]
	ldr r2, [r0, #56]

	fmdrr d6, r1, r2
	ldr r1, [r0, #60]
	ldr r2, [r0, #64]

	fmdrr d7, r1, r2
	ldr r1, [r0, #68]
	ldr r2, [r0, #72]

	fmdrr d8, r1, r2
	ldr r1, [r0, #76]
	ldr r2, [r0, #80]

	fmdrr d9, r1, r2
	ldr r1, [r0, #84]
	ldr r2, [r0, #88]

	fmdrr d10, r1, r2
	ldr r1, [r0, #92]
	ldr r2, [r0, #96]

	fmdrr d11, r1, r2
	ldr r1, [r0, #100]
	ldr r2, [r0, #104]

	fmdrr d12, r1, r2
	ldr r1, [r0, #108]
	ldr r2, [r0, #112]

	fmdrr d13, r1, r2
	ldr r1, [r0, #116]
	ldr r2, [r0, #120]

	fmdrr d14, r1, r2
	ldr r1, [r0, #124]
	ldr r2, [r0, #128]

	fmdrr d15, r1, r2
	bx lr

	.endasmfunc

	.def    ja_fp_init
	.asmfunc
ja_fp_init:
	 mrc   p15,     #0x00,      r2,       c1, c0, #0x02
     orr   r2,      r2,         #0xF00000
     mcr   p15,     #0x00,      r2,       c1, c0, #0x02
     mov   r2,      #0x40000000
     fmxr  fpexc,   r2

     mov r1, #0

     fmdrr d0,         r1,     r1
     fmdrr d1,         r1,     r1
     fmdrr d2,         r1,     r1
     fmdrr d3,         r1,     r1
     fmdrr d4,         r1,     r1
     fmdrr d5,         r1,     r1
     fmdrr d6,         r1,     r1
     fmdrr d7,         r1,     r1
     fmdrr d8,         r1,     r1
     fmdrr d9,         r1,     r1
     fmdrr d10,        r1,     r1
     fmdrr d11,        r1,     r1
     fmdrr d12,        r1,     r1
     fmdrr d13,        r1,     r1
     fmdrr d14,        r1,     r1
     fmdrr d15,        r1,     r1

	bx lr

	.endasmfunc

