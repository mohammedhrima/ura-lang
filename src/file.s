.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 64
    mov  DWORD PTR -4[rbp], edi ;// assign [user.a]
    mov  DWORD PTR -8[rbp], esi ;// assign [user.b]
    mov  QWORD PTR -20[rbp], rdx ;// assign [user.info.name]
    mov  QWORD PTR -28[rbp], rcx ;// assign [user.info.i.name]
    mov  DWORD PTR -64[rbp], 0 ;// assign [user1.a]
    mov  DWORD PTR -60[rbp], 0 ;// assign [user1.b]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -80[rbp], rax ;// assign [user1.info.name]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -88[rbp], rax ;// assign [user1.info.i.name]
    mov  eax, 0
    leave 
    ret  
.endm:
.STR0: .string ""
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

