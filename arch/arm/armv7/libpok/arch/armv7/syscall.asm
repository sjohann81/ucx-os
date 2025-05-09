;
; syscall.c
;
;  Created on: 15 mai 2019
;      Author: Alexy

.text
	.sect ".shared_code"
	.def    lja_do_syscall
	.asmfunc
lja_do_syscall:
	push {r1-r12, lr}
	swi #42
	pop {r1-r12, pc}
	.endasmfunc




