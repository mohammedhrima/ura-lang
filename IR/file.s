.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    /*declare s*/
    mov     QWORD PTR -8[rbp], 0
    /*assign s*/
    lea     rdi, .STR4[rip]
    call    .strdup
    mov     QWORD PTR -8[rbp], rax
    mov     rdi, rax
    call    free@PLT
    mov     rax, 0
    leave
    ret
.STR4: .string "abcd"
.section	.note.GNU-stack,"",@progbits

