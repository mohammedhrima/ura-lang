.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    /*declare str*/
    mov     QWORD PTR -8[rbp], 0
    mov     rdi, 10
    call    malloc
    /*assign_node str*/
    mov     QWORD PTR -8[rbp], rax
    mov     rax, 0
    leave
    ret
.endmain:
.section	.note.GNU-stack,"",@progbits

