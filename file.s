.text
.globl main
main:
    push  %rbp
    movq   %rsp, %rbp
    movl $10, -4(%rbp)
    movl $11, -8(%rbp)
    movl $12, -12(%rbp)
    pop  %rbp
    ret
