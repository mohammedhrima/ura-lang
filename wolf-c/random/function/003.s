.intel_syntax noprefix
.text
	.globl	main

func:
	push	rbp
	mov		rbp, rsp
	mov		DWORD PTR -4[rbp], edi
	mov		eax, DWORD PTR -4[rbp]
	add		eax, 5
	pop		rbp
	ret

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		edi, 2
	call	func
	mov		DWORD PTR -4[rbp], eax
	leave
	ret
	.section	.note.GNU-stack,"",@progbits
