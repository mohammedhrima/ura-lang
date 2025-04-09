	.file	"server.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"Hello\n"
.LC1:
	.string	"socket failed"
.LC2:
	.string	"setsockopt"
.LC3:
	.string	"bind failed"
.LC4:
	.string	"listen"
	.align 8
.LC5:
	.string	"Server listening on port %d...\n"
.LC6:
	.string	"accept"
.LC7:
	.string	"Client connected!"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 1072
	mov	edi, 8
	call	malloc@PLT
	mov	QWORD PTR -24[rbp], rax
	mov	DWORD PTR -36[rbp], 1
	mov	DWORD PTR -40[rbp], 16
	mov	QWORD PTR -1072[rbp], 0
	mov	QWORD PTR -1064[rbp], 0
	lea	rdx, -1056[rbp]
	mov	eax, 0
	mov	ecx, 126
	mov	rdi, rdx
	rep stosq
	lea	rax, .LC0[rip]
	mov	QWORD PTR -8[rbp], rax
	mov	edx, 0
	mov	esi, 1
	mov	edi, 2
	call	socket@PLT
	mov	DWORD PTR -12[rbp], eax
	cmp	DWORD PTR -12[rbp], 0
	jne	.L2
	lea	rax, .LC1[rip]
	mov	rdi, rax
	call	perror@PLT
	mov	edi, 1
	call	exit@PLT
.L2:
	lea	rdx, -36[rbp]
	mov	eax, DWORD PTR -12[rbp]
	mov	r8d, 4
	mov	rcx, rdx
	mov	edx, 2
	mov	esi, 1
	mov	edi, eax
	call	setsockopt@PLT
	test	eax, eax
	je	.L3
	lea	rax, .LC2[rip]
	mov	rdi, rax
	call	perror@PLT
	mov	edi, 1
	call	exit@PLT
.L3:
	mov	WORD PTR -32[rbp], 2
	mov	DWORD PTR -28[rbp], 0
	mov	edi, 8080
	call	htons@PLT
	mov	WORD PTR -30[rbp], ax
	lea	rcx, -32[rbp]
	mov	eax, DWORD PTR -12[rbp]
	mov	edx, 16
	mov	rsi, rcx
	mov	edi, eax
	call	bind@PLT
	test	eax, eax
	jns	.L4
	lea	rax, .LC3[rip]
	mov	rdi, rax
	call	perror@PLT
	mov	edi, 1
	call	exit@PLT
.L4:
	mov	eax, DWORD PTR -12[rbp]
	mov	esi, 3
	mov	edi, eax
	call	listen@PLT
	test	eax, eax
	jns	.L5
	lea	rax, .LC4[rip]
	mov	rdi, rax
	call	perror@PLT
	mov	edi, 1
	call	exit@PLT
.L5:
	mov	esi, 8080
	lea	rax, .LC5[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
.L7:
	lea	rdx, -40[rbp]
	lea	rcx, -32[rbp]
	mov	eax, DWORD PTR -12[rbp]
	mov	rsi, rcx
	mov	edi, eax
	call	accept@PLT
	mov	DWORD PTR -16[rbp], eax
	cmp	DWORD PTR -16[rbp], 0
	jns	.L6
	lea	rax, .LC6[rip]
	mov	rdi, rax
	call	perror@PLT
	mov	edi, 1
	call	exit@PLT
.L6:
	lea	rax, .LC7[rip]
	mov	rdi, rax
	call	puts@PLT
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	strlen@PLT
	mov	rdx, rax
	mov	rsi, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR -16[rbp]
	mov	ecx, 0
	mov	edi, eax
	call	send@PLT
	mov	eax, DWORD PTR -16[rbp]
	mov	edi, eax
	call	close@PLT
	jmp	.L7
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
