.intel_syntax noprefix
.include "/import/header.s"

.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 1 ;// assign [a]
    mov  DWORD PTR -8[rbp], 2 ;// assign [c]
    lea  rax, -4[rbp]
    mov  QWORD PTR -16[rbp], rax ;// assign [b] isref
    mov  rax, QWORD PTR -16[rbp]
    mov  edx, DWORD PTR -8[rbp]
    mov  DWORD PTR [rax], edx ;// assign [b] isref
    mov  eax, DWORD PTR -4[rbp]
    mov  edi, eax ;// assign [edi]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

