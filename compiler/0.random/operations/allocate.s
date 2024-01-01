.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix

.bss
buffer: .zero	8
pos: .zero	8
len: .zero	8

.section .rodata
STR0: .string "has size: %d \n"

.text
.globl	main
	
allocate:
	push	rbp
	mov		rbp, rsp
	push	rbx
	sub		rsp, 24
	mov		QWORD PTR -24[rbp], rdi
	mov		rax, QWORD PTR buffer[rip]
	test	rax, rax
	jne		Label1
	mov		QWORD PTR len[rip], 10
	mov		rax, QWORD PTR len[rip]
	sal		rax, 3
	mov		rdi, rax
	call	malloc@PLT
	mov		QWORD PTR buffer[rip], rax
Label1:
	mov		rax, QWORD PTR pos[rip]
	lea		rdx, 1[rax]
	mov		rax, QWORD PTR len[rip]
	cmp		rdx, rax
	jne		Label2
	mov		rax, QWORD PTR len[rip]
	add		rax, rax
	mov		QWORD PTR len[rip], rax
	mov		rax, QWORD PTR len[rip]
	lea		rdx, 0[0+rax*8]
	mov		rax, QWORD PTR buffer[rip]
	mov		rsi, rdx
	mov		rdi, rax
	call	realloc@PLT
	mov		QWORD PTR buffer[rip], rax
Label2:
	mov		rax, QWORD PTR -24[rbp]
	add		rax, 4
	mov		rdx, QWORD PTR buffer[rip]
	mov		rcx, QWORD PTR pos[rip]
	sal		rcx, 3
	lea		rbx, [rdx+rcx]
	mov		esi, 1
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR [rbx], rax
	mov		rax, QWORD PTR buffer[rip]
	mov		rdx, QWORD PTR pos[rip]
	sal		rdx, 3
	add		rax, rdx
	mov		rax, QWORD PTR [rax]
	mov		rdx, QWORD PTR -24[rbp]
	mov		DWORD PTR [rax], edx
	mov		rcx, QWORD PTR buffer[rip]
	mov		rax, QWORD PTR pos[rip]
	lea		rdx, 1[rax]
	mov		QWORD PTR pos[rip], rdx
	sal		rax, 3
	add		rax, rcx
	mov		rax, QWORD PTR [rax]
	add		rax, 4
	mov		rbx, QWORD PTR -8[rbp]
	leave
	ret

size:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -8[rbp], rdi
	sub		QWORD PTR -8[rbp], 4
	mov		rax, QWORD PTR -8[rbp]
	mov		eax, DWORD PTR [rax]
	pop		rbp
	ret

free_buffer:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], 0
	jmp		Label3
Label4:
	mov		rax, QWORD PTR buffer[rip]
	mov		rdx, QWORD PTR -8[rbp]
	sal		rdx, 3
	add		rax, rdx
	mov		rax, QWORD PTR [rax]
	mov		rdi, rax
	call	free@PLT
	add		QWORD PTR -8[rbp], 1
Label3:
	mov		rax, QWORD PTR pos[rip]
	cmp		QWORD PTR -8[rbp], rax
	jb		Label4
	mov		rax, QWORD PTR buffer[rip]
	mov		rdi, rax
	call	free@PLT
	nop
	leave
	ret

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		edi, 50
	call	allocate
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call	size
	mov		esi, eax
	lea		rax, STR0[rip]
	mov		rdi, rax
	mov		eax, 0
	call	printf@PLT
	mov		eax, 0
	call	free_buffer
	mov		eax, 0
	leave
	ret
