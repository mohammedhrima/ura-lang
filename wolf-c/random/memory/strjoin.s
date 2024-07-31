.intel_syntax noprefix
.include "../../import/header.s"

.text
	.globl	main

.LC0:
	.string	"abc"
.LC1:
	.string	"def"
.LC2:
	.string	"ghi\n"

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 48
	lea		rax, .LC0[rip]
	mov		QWORD PTR -8[rbp], rax

	lea		rax, .LC1[rip]
	mov		QWORD PTR -16[rbp], rax
	_strjoin_macro "QWORD PTR -8[rbp]", "QWORD PTR -16[rbp]", "QWORD PTR -24[rbp]"

	lea		rax, .LC2[rip]
	mov		QWORD PTR -32[rbp], rax
	_strjoin_macro "QWORD PTR -24[rbp]", "QWORD PTR -32[rbp]", "QWORD PTR -40[rbp]"

	mov		rax, QWORD PTR -40[rbp]
	mov		rdi, rax
	call	_putstr
	mov		eax, 0
	call	_free_memory
	mov		eax, 0
	leave
	ret
.section	.note.GNU-stack,"",@progbits
