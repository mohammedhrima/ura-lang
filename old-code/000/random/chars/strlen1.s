.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

.LC0:
    .string "abc\n"

ft_strlen:
    push rbp
    mov rbp, rsp
    /*char *str*/
    mov QWORD PTR -16[rbp], rdi
    /*size_t i = 0*/
    mov QWORD PTR -24[rbp], 0
    jmp .L2
.L3:
    /*i++*/
    add QWORD PTR -24[rbp], 1
.L2:
    mov rax, QWORD PTR -16[rbp]
    mov rdx, QWORD PTR -24[rbp]
    add rax, rdx
    movzx rax, BYTE PTR [rax]
    cmp rax, 0
    jne .L3
    /*return i*/
    mov rax, QWORD PTR -24[rbp]
    pop rbp
    ret

main:
    push rbp
    mov rbp, rsp
    lea rax, .LC0[rip]
    /*char *str*/
    mov QWORD PTR -8[rbp], rax
    mov rdi, rax
    call ft_strlen
    leave
    ret


