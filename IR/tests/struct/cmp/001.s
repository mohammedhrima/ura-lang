.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    /*declare a1*/
    mov     DWORD PTR -12[rbp], 0
    /*declare a2*/
    mov     DWORD PTR -8[rbp], 0
    /*declare a3*/
    mov     DWORD PTR -4[rbp], 0
    mov     eax, 0
    leave
    ret
.endmain:
.section	.note.GNU-stack,"",@progbits

