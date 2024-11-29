	.file	"int2.c"
	.intel_syntax noprefix
	.text
	.globl	putnbr
	.type	putnbr, @function
putnbr:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	nop
	pop	rbp
	ret
	.size	putnbr, .-putnbr
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	QWORD PTR -48[rbp], 11
	mov	QWORD PTR -40[rbp], 12
	mov	QWORD PTR -32[rbp], 13
	mov	QWORD PTR -24[rbp], 14
	mov	rax, QWORD PTR -40[rbp]
	mov	QWORD PTR -8[rbp], rax
	mov	DWORD PTR -12[rbp], 2
	mov	eax, DWORD PTR -12[rbp]
	cdqe
	mov	rax, QWORD PTR -48[rbp+rax*8]
	mov	rdi, rax
	call	putnbr
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits

/*
.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

.macro  printnbr
   mov     rdi, rax
   call    _putnbr
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
.endm

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 90
   mov     QWORD PTR -8[rbp], 0 
   mov     QWORD PTR -16[rbp], 11 
   mov     QWORD PTR -24[rbp], 22 
   mov     QWORD PTR -32[rbp], 33 
   mov     QWORD PTR -40[rbp], 44 
   mov     QWORD PTR -48[rbp], 55 
   mov     QWORD PTR -56[rbp], 66 

   lea     rax, -16[rbp] 
   mov     -64[rbp], rax 
   
   mov     rax, QWORD PTR -64[rbp]
   mov     QWORD PTR -8[rbp], rax

   mov     rax, 1
   mov     rax, QWORD PTR -56[rbp+rax*8]
   mov     rax, rax
   mov     QWORD PTR -72[rbp], rax

   mov     rax, QWORD PTR -72[rbp]
   printnbr

   leave
   ret
end_main:

STR1: .string "\n"
.section	.note.GNU-stack,"",@progbits
*/
