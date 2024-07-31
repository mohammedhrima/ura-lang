.intel_syntax noprefix
.section	.note.GNU-stack,"",@progbits
.text
.globl	main

ft_putchar:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	eax, edi
	mov	BYTE PTR -4[rbp], al
	lea	rax, -4[rbp]
	/*length*/
	mov	edx, 1
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	nop
	leave
	ret

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	BYTE PTR -1[rbp], 97
	movsx	eax, BYTE PTR -1[rbp]
	mov	edi, eax
	call	ft_putchar
	mov	eax, 0
	leave
	ret
