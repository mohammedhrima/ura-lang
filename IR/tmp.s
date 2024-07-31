	.file	"tmp.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"The 20 integers are:"
	.align 8
.LC1:
	.string	"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n"
	.text
	.globl	printTwentyIntegers
	.type	printTwentyIntegers, @function
printTwentyIntegers:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	DWORD PTR -4[rbp], edi
	mov	DWORD PTR -8[rbp], esi
	mov	DWORD PTR -12[rbp], edx
	mov	DWORD PTR -16[rbp], ecx
	mov	DWORD PTR -20[rbp], r8d
	mov	DWORD PTR -24[rbp], r9d
	lea	rdi, .LC0[rip]
	call	puts@PLT
	mov	r8d, DWORD PTR -20[rbp]
	mov	edi, DWORD PTR -16[rbp]
	mov	ecx, DWORD PTR -12[rbp]
	mov	edx, DWORD PTR -8[rbp]
	mov	eax, DWORD PTR -4[rbp]
	sub	rsp, 8
	mov	esi, DWORD PTR 120[rbp]
	push	rsi
	mov	esi, DWORD PTR 112[rbp]
	push	rsi
	mov	esi, DWORD PTR 104[rbp]
	push	rsi
	mov	esi, DWORD PTR 96[rbp]
	push	rsi
	mov	esi, DWORD PTR 88[rbp]
	push	rsi
	mov	esi, DWORD PTR 80[rbp]
	push	rsi
	mov	esi, DWORD PTR 72[rbp]
	push	rsi
	mov	esi, DWORD PTR 64[rbp]
	push	rsi
	mov	esi, DWORD PTR 56[rbp]
	push	rsi
	mov	esi, DWORD PTR 48[rbp]
	push	rsi
	mov	esi, DWORD PTR 40[rbp]
	push	rsi
	mov	esi, DWORD PTR 32[rbp]
	push	rsi
	mov	esi, DWORD PTR 24[rbp]
	push	rsi
	mov	esi, DWORD PTR 16[rbp]
	push	rsi
	mov	esi, DWORD PTR -24[rbp]
	push	rsi
	mov	r9d, r8d
	mov	r8d, edi
	mov	esi, eax
	lea	rdi, .LC1[rip]
	mov	eax, 0
	call	printf@PLT
	sub	rsp, -128
	nop
	leave
	ret
	.size	printTwentyIntegers, .-printTwentyIntegers
	.globl	main
	.type	main, @function
main:
	endbr64
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
	call	printTwentyIntegers
	add	rsp, 112
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 10.5.0-1ubuntu1~22.04) 10.5.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
