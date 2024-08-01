.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
sayHi:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32
    mov     QWORD PTR -8[rbp], rdi
    mov     rdi, QWORD PTR -8[rbp]
    call    .putnbr
    lea     rdi, .STR0[rip]
    call    .strdup
    mov     rdi, rax
    call    .putstr
    mov     rax, QWORD PTR -8[rbp]
    leave
    ret
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32
    /*declare a*/
    mov     QWORD PTR -16[rbp], 0
    /*assign a*/
    mov     QWORD PTR -16[rbp], 10
    /*declare b*/
    mov     QWORD PTR -24[rbp], 0
    /*assign b*/
    mov     QWORD PTR -24[rbp], 5
    mov     rax, QWORD PTR -16[rbp]
    add     rax, QWORD PTR -24[rbp]
    push    rax
    call    sayHi
    leave
    ret
.STR1: .string "\n"
.STR0: .string "\n"
.section	.note.GNU-stack,"",@progbits

