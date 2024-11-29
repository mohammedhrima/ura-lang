.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main

.bss
	.align 8

Glob_Pointer: .zero	8
Glob_Size: 	  .zero	100
Glob_Pos: 	  .zero	4

.text
	.type	allocate, @function

allocate:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR Glob_Pointer[rip]
	test	rax, rax
	jne	Label0
	mov	rax, QWORD PTR Glob_Size[rip]
	sal	rax, 3
	mov	rdi, rax
	call	malloc@PLT
	mov	QWORD PTR Glob_Pointer[rip], rax
Label0:
	mov	rax, QWORD PTR -24[rbp]
	mov	esi, 1
	mov	rdi, rax
	call	calloc@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	rcx, QWORD PTR Glob_Pointer[rip]
	mov	eax, DWORD PTR Glob_Pos[rip]
	lea	edx, 1[rax]
	mov	DWORD PTR Glob_Pos[rip], edx
	cdqe
	sal	rax, 3
	lea	rdx, [rcx+rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR [rdx], rax
	mov	eax, DWORD PTR Glob_Pos[rip]
	add	eax, 10
	movsx	rdx, eax
	mov	rax, QWORD PTR Glob_Size[rip]
	cmp	rdx, rax
	jne	Label1
	mov	rax, QWORD PTR Glob_Size[rip]
	sal	rax, 4
	mov	rdi, rax
	call	malloc@PLT
	mov	QWORD PTR -16[rbp], rax
	mov	eax, DWORD PTR Glob_Pos[rip]
	cdqe
	lea	rdx, 0[0+rax*8]
	mov	rcx, QWORD PTR Glob_Pointer[rip]
	mov	rax, QWORD PTR -16[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	memcpy@PLT
	mov	rax, QWORD PTR Glob_Pointer[rip]
	mov	rdi, rax
	call	free@PLT
	mov	rax, QWORD PTR -16[rbp]
	mov	QWORD PTR Glob_Pointer[rip], rax
	mov	rax, QWORD PTR Glob_Size[rip]
	add	rax, rax
	mov	QWORD PTR Glob_Size[rip], rax
Label1:
	mov	rax, QWORD PTR -8[rbp]
	leave
	ret

free_pointer:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	QWORD PTR -8[rbp], 0
	jmp	Label3
Label2:
	mov	rcx, QWORD PTR Glob_Pointer[rip]
	mov	rax, QWORD PTR -8[rbp]
	lea	rdx, 1[rax]
	mov	QWORD PTR -8[rbp], rdx
	sal	rax, 3
	add	rax, rcx
	mov	rax, QWORD PTR [rax]
	mov	rdi, rax
	call	free@PLT
Label3:
	mov	eax, DWORD PTR Glob_Pos[rip]
	cdqe
	cmp	QWORD PTR -8[rbp], rax
	jb	Label2
	mov	rax, QWORD PTR Glob_Pointer[rip]
	mov	rdi, rax
	call	free@PLT
	nop
	leave
	ret
	
main:
	push	rbp
	mov	rbp, rsp
	mov	rax, QWORD PTR Glob_Size[rip]
	mov	rdi, rax
	call	allocate
	mov	eax, 0
	call	free_pointer
	mov	eax, 0
	pop	rbp
	ret
	
