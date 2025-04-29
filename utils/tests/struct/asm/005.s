.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    mov  DWORD PTR -4[rbp], edi ;// assign [user.a]
    mov  DWORD PTR -8[rbp], esi ;// assign [user.b]
    mov  QWORD PTR -16[rbp], rdx ;// assign [user.info.name]
    mov  QWORD PTR -24[rbp], rcx ;// assign [user.info.i.name]
    mov  eax, 0
    leave 
    ret  
.endm:
.section	.note.GNU-stack,"",@progbits

