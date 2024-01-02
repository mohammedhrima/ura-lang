.section	.note.GNU-stack,"",@progbits
.bss
	head:	.zero	8
	curr:	.zero	8
	pos:	.zero	8
.text
	.globl	main

allocate:
	push	rbp
	mov		rbp, rsp
	push	rbx
	sub		rsp, 40
	mov		QWORD PTR -40[rbp], rdi
	mov		rax, QWORD PTR head[rip]
	test	rax, rax
	jne		allocate0
	mov		esi, 808
	mov		edi, 1
	call	calloc@PLT
	mov		QWORD PTR head[rip], rax
	mov		rax, QWORD PTR head[rip]
	mov		QWORD PTR curr[rip], rax
	jmp		allocate1
allocate0:
	mov		rax, QWORD PTR pos[rip]
	cmp		rax, 100
	jne		allocate1
	mov		QWORD PTR pos[rip], 0
	mov		rbx, QWORD PTR curr[rip]
	mov		esi, 808
	mov		edi, 1
	call	calloc@PLT
	mov		QWORD PTR 800[rbx], rax
	mov		rax, QWORD PTR curr[rip]
	mov		rax, QWORD PTR 800[rax]
	mov		QWORD PTR curr[rip], rax
allocate1:
	mov		rax, QWORD PTR -40[rbp]
	add		rax, 8
	mov		esi, 1
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -24[rbp], rax
	mov		rax, QWORD PTR -24[rbp]
	mov		rdx, QWORD PTR -40[rbp]
	mov		QWORD PTR [rax], rdx
	mov		rdx, QWORD PTR curr[rip]
	mov		rax, QWORD PTR pos[rip]
	lea		rcx, 1[rax]
	mov		QWORD PTR pos[rip], rcx
	mov		rcx, QWORD PTR -24[rbp]
	mov		QWORD PTR [rdx+rax*8], rcx
	mov		rax, QWORD PTR -24[rbp]
	add		rax, 8
	mov		rbx, QWORD PTR -8[rbp]
	leave
	ret

free_memory:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	jmp		free_memory0
free_memory3:
	mov		DWORD PTR -4[rbp], 0
	jmp		free_memory1
free_memory2:
	mov		rax, QWORD PTR head[rip]
	mov		edx, DWORD PTR -4[rbp]
	movsx	rdx, edx
	mov		rax, QWORD PTR [rax+rdx*8]
	mov		rdi, rax
	call	free@PLT
	add		DWORD PTR -4[rbp], 1
free_memory1:
	cmp		DWORD PTR -4[rbp], 99
	jle		free_memory2
	mov		rax, QWORD PTR head[rip]
	mov		QWORD PTR -16[rbp], rax
	mov		rax, QWORD PTR head[rip]
	mov		rax, QWORD PTR 800[rax]
	mov		QWORD PTR head[rip], rax
	mov		rax, QWORD PTR -16[rbp]
	mov		rdi, rax
	call	free@PLT
free_memory0:
	mov		rax, QWORD PTR head[rip]
	test	rax, rax
	jne		free_memory3
	nop
	nop
	leave
	ret

ft_strlen:
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
	.size	ft_strlen, .-ft_strlen
	.globl	ft_strdup
	.type	ft_strdup, @function
ft_strdup:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	cmp	QWORD PTR -24[rbp], 0
	jne	.L16
	mov	edi, 1
	call	allocate
	jmp	.L17
.L16:
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	ft_strlen
	add	rax, 1
	mov	rdi, rax
	call	allocate
	mov	QWORD PTR -8[rbp], rax
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	strcpy@PLT
	mov	rax, QWORD PTR -8[rbp]
.L17:
	leave
	ret
	.size	ft_strdup, .-ft_strdup
	.section	.rodata
.LC0:
	.string	"abcdef"
.LC1:
	.string	"%zu\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, .LC0[rip]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	ft_strdup
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, 8
	mov	rax, QWORD PTR [rax]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	ft_strdup
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, 8
	mov	rax, QWORD PTR [rax]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	ft_strdup
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, 8
	mov	rax, QWORD PTR [rax]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	call	free_memory
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"

