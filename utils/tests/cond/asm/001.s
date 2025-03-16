.intel_syntax noprefix
.include "/import/header.s"

.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 0 ;// declare [a]
    .if1:
    mov  eax, DWORD PTR -4[rbp]
    mov  ebx, 1
    cmp  eax, ebx
    sete al
        cmp  al, 1
        jne  .elif4
        lea  rdi, .STR1[rip] ;// assign [rdi]
        call putstr
    jmp  .endif3
    .elif4:
    mov  eax, DWORD PTR -4[rbp]
    mov  ebx, 0
    cmp  eax, ebx
    sete al
        cmp  al, 1
        jne  .else6
        lea  rdi, .STR2[rip] ;// assign [rdi]
        call putstr
    jmp  .endif3
    .else6:
        lea  rdi, .STR3[rip] ;// assign [rdi]
        call putstr
    .endif3:
    mov  eax, 0
    leave 
    ret  
.endmain:.STR1: .string "cond 1\n"
.STR2: .string "cond 2\n"
.STR3: .string "cond 3\n"
.section	.note.GNU-stack,"",@progbits

