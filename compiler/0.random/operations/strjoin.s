	.file	"strjoin.c"
	.intel_syntax noprefix
	.text
	.globl	head
	.bss
	.align 8
	.type	head, @object
	.size	head, 8
head:
	.zero	8
	.globl	curr
	.align 8
	.type	curr, @object
	.size	curr, 8
curr:
	.zero	8
	.globl	pos
	.align 8
	.type	pos, @object
	.size	pos, 8
pos:
	.zero	8
	.text
	.globl	_allocate
	.type	_allocate, @function
_allocate:
	push	rbp
	mov	rbp, rsp
	push	rbx
	sub	rsp, 40
	mov	QWORD PTR -40[rbp], rdi
	mov	rax, QWORD PTR head[rip]
	test	rax, rax
	jne	.L2
	mov	esi, 808
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR head[rip], rax
	mov	rax, QWORD PTR head[rip]
	mov	QWORD PTR curr[rip], rax
	jmp	.L3
.L2:
	mov	rax, QWORD PTR pos[rip]
	cmp	rax, 100
	jne	.L3
	mov	QWORD PTR pos[rip], 0
	mov	rbx, QWORD PTR curr[rip]
	mov	esi, 808
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR 800[rbx], rax
	mov	rax, QWORD PTR curr[rip]
	mov	rax, QWORD PTR 800[rax]
	mov	QWORD PTR curr[rip], rax
.L3:
	mov	rax, QWORD PTR -40[rbp]
	add	rax, 8
	mov	esi, 1
	mov	rdi, rax
	call	calloc@PLT
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR -40[rbp]
	mov	QWORD PTR [rax], rdx
	mov	rdx, QWORD PTR curr[rip]
	mov	rax, QWORD PTR pos[rip]
	lea	rcx, 1[rax]
	mov	QWORD PTR pos[rip], rcx
	mov	rcx, QWORD PTR -24[rbp]
	mov	QWORD PTR [rdx+rax*8], rcx
	mov	rax, QWORD PTR -24[rbp]
	add	rax, 8
	mov	rbx, QWORD PTR -8[rbp]
	leave
	ret
	.size	_allocate, .-_allocate
	.globl	_free_memory
	.type	_free_memory, @function
_free_memory:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	jmp	.L6
.L9:
	mov	DWORD PTR -4[rbp], 0
	jmp	.L7
.L8:
	mov	rax, QWORD PTR head[rip]
	mov	edx, DWORD PTR -4[rbp]
	movsx	rdx, edx
	mov	rax, QWORD PTR [rax+rdx*8]
	mov	rdi, rax
	call	free@PLT
	add	DWORD PTR -4[rbp], 1
.L7:
	cmp	DWORD PTR -4[rbp], 99
	jle	.L8
	mov	rax, QWORD PTR head[rip]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR head[rip]
	mov	rax, QWORD PTR 800[rax]
	mov	QWORD PTR head[rip], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	free@PLT
.L6:
	mov	rax, QWORD PTR head[rip]
	test	rax, rax
	jne	.L9
	nop
	nop
	leave
	ret
	.size	_free_memory, .-_free_memory
	.globl	_strlen
	.type	_strlen, @function
_strlen:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -8[rbp], 0
	jmp	.L11
.L13:
	add	QWORD PTR -8[rbp], 1
.L11:
	cmp	QWORD PTR -24[rbp], 0
	je	.L12
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	.L13
.L12:
	mov	rax, QWORD PTR -8[rbp]
	pop	rbp
	ret
	.size	_strlen, .-_strlen
	.globl	_strcpy
	.type	_strcpy, @function
_strcpy:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	DWORD PTR -4[rbp], 0
	jmp	.L16
.L18:
	mov	eax, DWORD PTR -4[rbp]
	movsx	rdx, eax
	mov	rax, QWORD PTR -32[rbp]
	add	rax, rdx
	mov	edx, DWORD PTR -4[rbp]
	movsx	rcx, edx
	mov	rdx, QWORD PTR -24[rbp]
	add	rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov	BYTE PTR [rdx], al
	add	DWORD PTR -4[rbp], 1
.L16:
	cmp	QWORD PTR -24[rbp], 0
	je	.L19
	cmp	QWORD PTR -32[rbp], 0
	je	.L19
	mov	eax, DWORD PTR -4[rbp]
	movsx	rdx, eax
	mov	rax, QWORD PTR -32[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	.L18
.L19:
	nop
	pop	rbp
	ret
	.size	_strcpy, .-_strcpy
	.globl	_strjoin
	.type	_strjoin, @function
_strjoin:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	_strlen
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -32[rbp]
	mov	rdi, rax
	call	_strlen
	mov	rdx, QWORD PTR -8[rbp]
	add	rax, rdx
	add	rax, 1
	mov	rdi, rax
	call	_allocate
	mov	QWORD PTR -16[rbp], rax
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -16[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	_strcpy
	mov	rdx, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR -8[rbp]
	add	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	rsi, rax
	mov	rdi, rdx
	call	_strcpy
	mov	rax, QWORD PTR -16[rbp]
	leave
	ret
	.size	_strjoin, .-_strjoin
	.globl	_putstr
	.type	_putstr, @function
_putstr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	QWORD PTR -8[rbp], rdi
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	_strlen
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	nop
	leave
	ret
	.size	_putstr, .-_putstr
	.section	.rodata
.LC0:
	.string	"aaaa"
.LC1:
	.string	"bbb\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32

	lea	rax, .LC0[rip]
	mov	QWORD PTR -8[rbp], rax
	
	lea	rax, .LC1[rip]
	mov	QWORD PTR -16[rbp], rax
	
	mov	rdx, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR -8[rbp]
	
	mov	rsi, rdx
	mov	rdi, rax
	call	_strjoin

	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	_putstr
	mov		eax, 0
	call	_free_memory
	mov		eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
