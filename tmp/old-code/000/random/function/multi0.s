	.file	"multi.c"
	.intel_syntax noprefix
	.text
	.globl	processIntegers
	.type	processIntegers, @function
processIntegers:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -20[rbp], edi
	mov	DWORD PTR -24[rbp], esi
	mov	DWORD PTR -28[rbp], edx
	mov	DWORD PTR -32[rbp], ecx
	mov	DWORD PTR -36[rbp], r8d
	mov	DWORD PTR -40[rbp], r9d
	mov	edx, DWORD PTR -20[rbp]
	mov	eax, DWORD PTR -24[rbp]
	add	edx, eax
	mov	eax, DWORD PTR -28[rbp]
	add	edx, eax
	mov	eax, DWORD PTR -32[rbp]
	add	edx, eax
	mov	eax, DWORD PTR -36[rbp]
	add	edx, eax
	mov	eax, DWORD PTR -40[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 16[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 24[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 32[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 40[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 48[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 56[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 64[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 72[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 80[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 88[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 96[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 104[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 112[rbp]
	add	edx, eax
	mov	eax, DWORD PTR 120[rbp]
	add	eax, edx
	mov	DWORD PTR -4[rbp], eax
	nop
	pop	rbp
	ret
	.size	processIntegers, .-processIntegers
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	push	20
	push	19
	push	18
	push	17
	push	16
	push	15
	push	14
	push	13
	push	12
	push	11
	push	10
	push	9
	push	8
	push	7
	mov	r9d, 6
	mov	r8d, 5
	mov	ecx, 4
	mov	edx, 3
	mov	esi, 2
	mov	edi, 1
	call	processIntegers
	add	rsp, 112
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
