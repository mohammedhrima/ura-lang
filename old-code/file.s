.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main

hello:
    push rbp
    mov  rbp, rsp
    sub  rsp, 0
    lea  rdi, .STR1[rip] ;// assign [rdi]
    call putstr
    mov  eax, 1
    leave 
    ret  
.endhello:
main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 10 ;// assign [a]
    call hello
    mov  edi, DWORD PTR -4[rbp] ;// assign [edi]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:.STR1: .string "abc\n"
.section	.note.GNU-stack,"",@progbits

