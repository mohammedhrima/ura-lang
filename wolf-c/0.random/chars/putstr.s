.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main

STR21:
	.string	"abc\n"

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	lea		rax, STR21[rip]
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call	Putstr
	mov		eax, 0
	leave
	ret

Len:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -8[rbp], 0
	jmp	Label1
Label0:
	add	QWORD PTR -8[rbp], 1
Label1:
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	Label0
	mov	rax, QWORD PTR -8[rbp]
	mov rsp, rbp
	pop	rbp
	ret

Putstr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	Len
	mov	QWORD PTR -8[rbp], rax
	mov	rdx, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	mov rsp, rbp
	pop rbp
	ret
