	.file	"tmp.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	_putnbr
	.type	_putnbr, @function
_putnbr:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -4[rbp], edi
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, eax
	lea	rdi, .LC0[rip]
	mov	eax, 0
	call	printf@PLT
	nop
	leave
	ret
	.size	_putnbr, .-_putnbr
	.globl	func
	.type	func, @function
func:
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
	mov	edi, eax
	call	_putnbr
	mov	eax, DWORD PTR -8[rbp]
	mov	edi, eax
	call	_putnbr
	mov	eax, DWORD PTR -12[rbp]
	mov	edi, eax
	call	_putnbr
	mov	eax, DWORD PTR -16[rbp]
	mov	edi, eax
	call	_putnbr
	mov	eax, DWORD PTR -20[rbp]
	mov	edi, eax
	call	_putnbr
	nop
	leave
	ret
	.size	func, .-func
	.globl	func2
	.type	func2, @function
func2:
	endbr64
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -4[rbp], edi
	mov	DWORD PTR -8[rbp], esi
	mov	DWORD PTR -12[rbp], edx
	mov	DWORD PTR -16[rbp], ecx
	mov	DWORD PTR -20[rbp], r8d
	mov	DWORD PTR -24[rbp], r9d
	nop
	pop	rbp
	ret
	.size	func2, .-func2
	.globl	main
	.type	main, @function
main:
	endbr64
	push	rbp
	mov	rbp, rsp
	push	40
	push	39
	push	38
	push	37
	push	36
	push	35
	push	34
	push	33
	push	32
	push	31
	push	30
	push	29
	push	28
	push	27
	push	26
	push	25
	push	24
	push	23
	push	22
	push	21
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
	call	func
	add	rsp, 272
	mov	r9d, 6
	mov	r8d, 5
	mov	ecx, 4
	mov	edx, 3
	mov	esi, 2
	mov	edi, 1
	call	func2
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
