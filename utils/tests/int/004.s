.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  rax, QWORD PTR -4[rbp]
    mov  DWORD PTR [rax], 1