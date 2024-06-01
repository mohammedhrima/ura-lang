.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main
STR0:
	.string	"abcdef"

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	lea		rax, STR0[rip]
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call	strlen@PLT
	add		rax, 1
	mov		esi, 1 		/*second argument*/
	mov		rdi, rax 	/*first arguemnt*/
	call	calloc@PLT
	/*	return of calloc */
	mov		QWORD PTR -16[rbp], rax

	mov		rax, QWORD PTR -16[rbp]
	mov		rdx, QWORD PTR -8[rbp]
	/* destination */
	mov		rdi, rax
	/* source */
	mov		rsi, rdx
	call	strcpy@PLT
	/* free's argument */
	mov		rax, QWORD PTR -16[rbp]
	mov		rdi, rax
	call	free@PLT
	mov		eax, 0
	leave
	ret

