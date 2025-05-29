.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 64
    mov  DWORD PTR -36[rbp], edi ;// assign [user.a]
    mov  DWORD PTR -40[rbp], esi ;// assign [user.b]
    mov  QWORD PTR -48[rbp], rdx ;// assign [user.info.name]
    mov  QWORD PTR -56[rbp], rcx ;// assign [user.info.i.name]
    mov  eax, 0
    leave 
    ret  
.endm:
.section	.note.GNU-stack,"",@progbits

