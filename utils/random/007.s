	.file	"007.c"
	.intel_syntax noprefix
	.text
	.globl	handle_client
	.type	handle_client, @function
handle_client:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 96
	mov	DWORD PTR -84[rbp], edi
	movabs	rax, 3543824036068086856
	movabs	rdx, 957946345412375072
	mov	QWORD PTR -80[rbp], rax
	mov	QWORD PTR -72[rbp], rdx
	movabs	rax, 8389754706581209866
	movabs	rdx, 8367752315007489069
	mov	QWORD PTR -64[rbp], rax
	mov	QWORD PTR -56[rbp], rdx
	movabs	rax, 7593469675811666021
	movabs	rdx, 7308900669414051182
	mov	QWORD PTR -48[rbp], rax
	mov	QWORD PTR -40[rbp], rdx
	movabs	rax, 7142773254999602275
	movabs	rdx, 724246167729434476
	mov	QWORD PTR -32[rbp], rax
	mov	QWORD PTR -24[rbp], rdx
	movabs	rax, 3201897072895199757
	movabs	rdx, 9399091170596640
	mov	QWORD PTR -18[rbp], rax
	mov	QWORD PTR -10[rbp], rdx
	lea	rcx, -80[rbp]
	mov	eax, DWORD PTR -84[rbp]
	mov	edx, 77
	mov	rsi, rcx
	mov	edi, eax
	call	write@PLT
	mov	eax, DWORD PTR -84[rbp]
	mov	edi, eax
	call	close@PLT
	nop
	leave
	ret
	.size	handle_client, .-handle_client
	.section	.rodata
.LC0:
	.string	"Socket creation failed"
.LC1:
	.string	"Bind failed"
.LC2:
	.string	"Listen failed"
	.align 8
.LC3:
	.string	"Server is listening on port %d...\n"
.LC4:
	.string	"Accept failed"
.LC5:
	.string	"Client connected"
	.text
	.globl	start_server
	.type	start_server, @function
start_server:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 80
	mov	DWORD PTR -68[rbp], edi
	mov	DWORD PTR -52[rbp], 16
	mov	edx, 0
	mov	esi, 1
	mov	edi, 2
	call	socket@PLT
	mov	DWORD PTR -4[rbp], eax
	cmp	DWORD PTR -4[rbp], -1
	jne	.L3
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	perror@PLT
	mov	edi, 1
	call	exit@PLT
.L3:
	mov	WORD PTR -32[rbp], 2
	mov	DWORD PTR -28[rbp], 0
	mov	eax, DWORD PTR -68[rbp]
	movzx	eax, ax
	mov	edi, eax
	call	htons@PLT
	mov	WORD PTR -30[rbp], ax
	lea	rcx, -32[rbp]
	mov	eax, DWORD PTR -4[rbp]
	mov	edx, 16
	mov	rsi, rcx
	mov	edi, eax
	call	bind@PLT
	cmp	eax, -1
	jne	.L4
	lea	rax, .LC1[rip]
	mov	rdi, rax
	call	perror@PLT
	mov	eax, DWORD PTR -4[rbp]
	mov	edi, eax
	call	close@PLT
	mov	edi, 1
	call	exit@PLT
.L4:
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, 5
	mov	edi, eax
	call	listen@PLT
	cmp	eax, -1
	jne	.L5
	lea	rax, .LC2[rip]
	mov	rdi, rax
	call	perror@PLT
	mov	eax, DWORD PTR -4[rbp]
	mov	edi, eax
	call	close@PLT
	mov	edi, 1
	call	exit@PLT
.L5:
	mov	eax, DWORD PTR -68[rbp]
	mov	esi, eax
	lea	rax, .LC3[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
.L8:
	lea	rdx, -52[rbp]
	lea	rcx, -48[rbp]
	mov	eax, DWORD PTR -4[rbp]
	mov	rsi, rcx
	mov	edi, eax
	call	accept@PLT
	mov	DWORD PTR -8[rbp], eax
	cmp	DWORD PTR -8[rbp], -1
	jne	.L6
	lea	rax, .LC4[rip]
	mov	rdi, rax
	call	perror@PLT
	jmp	.L7
.L6:
	lea	rax, .LC5[rip]
	mov	rdi, rax
	call	puts@PLT
	mov	eax, DWORD PTR -8[rbp]
	mov	edi, eax
	call	handle_client
.L7:
	jmp	.L8
	.size	start_server, .-start_server
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -4[rbp], 8080
	mov	eax, DWORD PTR -4[rbp]
	mov	edi, eax
	call	start_server
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
