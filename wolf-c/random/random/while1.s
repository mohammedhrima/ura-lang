.intel_syntax noprefix
.text
	.section	.rodata

STR0: .string	"%d\n"

.text
	.globl	main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -4[rbp], 0
	jmp	L2
L5:
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, eax
	lea	rax, STR0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	cmp	DWORD PTR -4[rbp], 5
	je	L7
	add	DWORD PTR -4[rbp], 1
L2:
	cmp	DWORD PTR -4[rbp], 19
	jle	L5
	jmp	L4
L7:
	nop
L4:
	mov	eax, 0
	leave
	ret
.section	.note.GNU-stack,"",@progbits

