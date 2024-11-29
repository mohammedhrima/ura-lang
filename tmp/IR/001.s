	.file	"001.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	foo
	.type	foo, @function
foo:
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
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, eax
	lea	rdi, .LC0[rip]
	mov	eax, 0
	call	printf@PLT
	nop
	leave
	ret
	.size	foo, .-foo
	.globl	main
	.type	main, @function
main:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], 10
	mov	DWORD PTR -16[rbp], 11
	mov	DWORD PTR -12[rbp], 12
	mov	DWORD PTR -8[rbp], 13
	mov	DWORD PTR -4[rbp], 14
	mov	r9d, DWORD PTR -20[rbp]
	mov	r8d, DWORD PTR -20[rbp]
	mov	ecx, DWORD PTR -20[rbp]
	mov	edx, DWORD PTR -20[rbp]
	mov	esi, DWORD PTR -20[rbp]
	mov	eax, DWORD PTR -20[rbp]
	sub	rsp, 8
	mov	edi, DWORD PTR -20[rbp]
	push	rdi
	mov	edi, eax
	call	foo
	add	rsp, 16
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
