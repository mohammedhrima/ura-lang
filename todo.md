- TODOS:
    - Data types:
        - bigInt
        - char
        - arrays
        - implement BigInt, BigFloat
            - shift the number in chnks of 4bytes
    - Build a costume printf

    - Operators:
        - math operators:
            - negative numbers
            + +, -, ()
            - *, /:
        - logic operators:
            - < > == !=
        - assignement:
            - += -= *= /=
    
    - Operators:
        - int  : + / * -
        - float: + / * -
        - char : + / * -


















- loops, statments

- functions:
    - function call
        - before calling function push rbp to the stack and pop it later
        - each function has its own rbp
    - with lot of parameters:
        - use the stack
    - built ins functions:
        - output
    - send params by refrence or value
- variables by level
- add tester to expected output
- set estimation for each lable how much will take from rsp
- data type methods
- OOP shit:
    - operators overloading
    - class data types: 
        - bigInt / bigfloat (optimized version of each known data type)
- memory management:
    heap / stack !!!
- web server using it
- 2nd version bdarija
- pacman, snack game (using volkan)
- web framework maybe !!

- Error handeling:
    - forgot " or '
    - unknown data type
    - tab level after :
    - protect rsp

=========================================================================
- NOTES:
    + testb : 'and' bitwise operator
    + write system call:
        rdi : fd
        rsi : pointer
        rdx : length

    + quad: 64bit
    + long: 32bit
    + word: 16bit
    + byte: 08bit


/*
.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare x */
   mov     QWORD PTR -4[rbp], 0
   /* assign to x */
   movss   xmm1, DWORD PTR FLT2[rip]
   movss   DWORD PTR -4[rbp], xmm1
   /* declare y */
   mov     QWORD PTR -8[rbp], 0
   /* assign to y */
   movss   xmm1, DWORD PTR FLT3[rip]
   movss   DWORD PTR -8[rbp], xmm1
   /* declare z */
   mov     QWORD PTR -12[rbp], 0
   movss   xmm1, DWORD PTR -8[rbp]
   addss   xmm1, DWORD PTR FLT4[rip]
   movss   DWORD PTR -16[rbp], xmm1
   movss   xmm1, DWORD PTR -4[rbp]
   addss   xmm1, DWORD PTR -16[rbp]
   movss   DWORD PTR -20[rbp], xmm1
   /* assign to z */
   movss   xmm1, DWORD PTR -20[rbp]
   movss   DWORD PTR -12[rbp], xmm1

	lea		rdi, .LC4[rip]

   movq		rax, DWORD PTR -4[rbp]
	movq	   xmm2, rax
	
	mov		rax, DWORD PTR -8[rbp]
	movq	   xmm1, rax
	
	mov		rax, DWORD PTR -12[rbp]
	movq	   xmm0, rax

	mov		eax, 3
	call	printf@PLT

   leave
   ret

.LC4:
	.string	"%f + %f + 5.0 = %f\n"

FLT2:/* 2.000000 */
   .long  1073741824
FLT3:/* -1.000000 */
   .long  3212836864
FLT4:/* 5.000000 */
   .long  1084227584

*/