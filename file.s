.globl main
main:
    push   %rbp
    mov    %rsp, %rbp
    movl   $5, -4(%rbp)
    movl   $10, -4(%rbp)
    movl   $10, -8(%rbp)
    movl   $15, -4(%rbp)
    mov    $0, %rax
    pop    %rbp
    ret
