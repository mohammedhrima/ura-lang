.intel_syntax noprefix
.text
	.globl	main

_strlen:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -8[rbp], 0
	jmp		_strlen0
_strlen2:
	add		QWORD PTR -8[rbp], 1
_strlen0:
	cmp		QWORD PTR -24[rbp], 0
	je		_strlen1
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne		_strlen2
_strlen1:
	mov		rax, QWORD PTR -8[rbp]
	pop		rbp
	ret

_strdup:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -24[rbp], rdi
	mov		rax, QWORD PTR -24[rbp]
	mov		rdi, rax
	call	_strlen
	add		rax, 1
	mov		esi, 1
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -16[rbp], rax
	mov		QWORD PTR -8[rbp], 0
	jmp		_strdup0
_strdup2:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	mov		rcx, QWORD PTR -16[rbp]
	mov		rdx, QWORD PTR -8[rbp]
	add		rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov		BYTE PTR [rdx], al
	add		QWORD PTR -8[rbp], 1
_strdup0:
	cmp		QWORD PTR -24[rbp], 0
	je		_strdup1
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne		_strdup2
_strdup1:
	mov		rax, QWORD PTR -16[rbp]
	leave
	ret

_putstr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], rdi
	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call	_strlen
	mov		rdx, rax
	mov		rax, QWORD PTR -8[rbp]
	mov		rsi, rax
	mov		edi, 1
	call	write@PLT
	nop
	leave
	ret

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	lea		rax, STR0[rip]
	mov		rdi, rax
	call	_strdup
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call 	_putstr

	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call	free@PLT
	mov		eax, 0
	leave
	ret

STR0: .string	"abcde"
.section	.note.GNU-stack,"",@progbits
