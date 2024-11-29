	.file	"002.c"
	.intel_syntax noprefix
	.text
	.globl	processIntegers
	.type	processIntegers, @function
processIntegers:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	QWORD PTR -40[rbp], rdx
	mov	QWORD PTR -48[rbp], rcx
	mov	QWORD PTR -56[rbp], r8
	mov	QWORD PTR -64[rbp], r9
	mov	rax, QWORD PTR -24[rbp]
	mov	QWORD PTR [rax], 77
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR [rax]
	mov	rax, QWORD PTR -32[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR -48[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR -64[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR 16[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR 24[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR 32[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR 40[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR 48[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR 56[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR 64[rbp]
	mov	rax, QWORD PTR [rax]
	add	rdx, rax
	mov	rax, QWORD PTR 72[rbp]
	mov	rax, QWORD PTR [rax]
	add	rax, rdx
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	pop	rbp
	ret
	.size	processIntegers, .-processIntegers
	.section	.rodata
.LC0:
	.string	">>>> %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 112
	mov	QWORD PTR -8[rbp], 1
	mov	QWORD PTR -16[rbp], 2
	mov	QWORD PTR -24[rbp], 3
	mov	QWORD PTR -32[rbp], 4
	mov	QWORD PTR -40[rbp], 5
	mov	QWORD PTR -48[rbp], 6
	mov	QWORD PTR -56[rbp], 7
	mov	QWORD PTR -64[rbp], 8
	mov	QWORD PTR -72[rbp], 9
	mov	QWORD PTR -80[rbp], 10
	mov	QWORD PTR -88[rbp], 11
	mov	QWORD PTR -96[rbp], 12
	mov	QWORD PTR -104[rbp], 13
	mov	QWORD PTR -112[rbp], 14
	lea	r9, -48[rbp]
	lea	r8, -40[rbp]
	lea	rcx, -32[rbp]
	lea	rdx, -24[rbp]
	lea	rsi, -16[rbp]
	lea	rax, -8[rbp]
	lea	rdi, -112[rbp]
	push	rdi
	lea	rdi, -104[rbp]
	push	rdi
	lea	rdi, -96[rbp]
	push	rdi
	lea	rdi, -88[rbp]
	push	rdi
	lea	rdi, -80[rbp]
	push	rdi
	lea	rdi, -72[rbp]
	push	rdi
	lea	rdi, -64[rbp]
	push	rdi
	lea	rdi, -56[rbp]
	push	rdi
	mov	rdi, rax
	call	processIntegers
	add	rsp, 64
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	lea	rax, .LC0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
