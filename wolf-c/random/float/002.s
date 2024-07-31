.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix

.section	.rodata
.LC0:
	.string	"%f + %f = %f\n"
.LC1:
	.long	1092616192
.LC2:
	.long	1088421888

.section .text
    .globl main

main:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    
    /*float x = 10*/
    movss xmm1, DWORD PTR .LC1[rip]
    movss DWORD PTR -4[rbp], xmm1
    
    /*float y = 7*/
    movss xmm1, DWORD PTR .LC2[rip]
    movss DWORD PTR -8[rbp], xmm1
    
    /*float z = x + y*/
    movss xmm1, DWORD PTR -4[rbp]
    addss xmm1, DWORD PTR -8[rbp]
    movss DWORD PTR -12[rbp], xmm1

    /*clean xmms registers*/
    pxor xmm2, xmm2
    pxor xmm1, xmm1
    pxor xmm0, xmm0

    cvtss2sd xmm0, DWORD PTR -4[rbp]
    cvtss2sd xmm1, DWORD PTR -8[rbp]
    cvtss2sd xmm2, DWORD PTR -12[rbp]

    lea	rax, .LC0[rip]
    mov	rdi, rax
	mov	eax, 3
	call	printf@PLT
	mov	eax, 0
	leave
	ret
