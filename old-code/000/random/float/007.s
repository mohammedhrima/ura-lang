.intel_syntax noprefix
.include "../../import/header.s"
.text
	.globl	main

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	movss	xmm0, DWORD PTR .LC3[rip]
	movss	DWORD PTR -4[rbp], xmm0
	mov		eax, DWORD PTR -4[rbp]
	movd	xmm0, eax
	call	_putfloat
	mov		eax, 0
	leave
	ret

.LC3:
	.long	1106247680
.section	.note.GNU-stack,"",@progbits
