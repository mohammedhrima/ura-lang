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
