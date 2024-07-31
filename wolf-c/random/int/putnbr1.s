.intel_syntax noprefix
.section	.note.GNU-stack,"",@progbits
.text 
.globl main

ft_putchar:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov eax, edi
    mov BYTE -4[rbp], al
    lea rax, -4[rbp]
    /*length*/
    mov edx, 1
    /*pointer*/
    mov rsi, rax
    /*write syscall*/
    mov rax, 1
    syscall
    ret

main:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    /*declare num*/
    mov QWORD PTR -8[rbp], 65
    mov rax, QWORD PTR -8[rbp]
    mov rdi, rax
    call ft_putchar
    pop rbp
    ret
