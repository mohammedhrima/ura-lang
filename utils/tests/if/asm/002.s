.intel_syntax noprefix
.include "/import/header.s"

.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 0 ;// assign [i]
    .if1:
    mov  eax, DWORD PTR -4[rbp]
    mov  ebx, 1
    cmp  eax, ebx
    sete al
        cmp  al, 1
        jne  .else13
        mov  DWORD PTR -4[rbp], 2 ;// assign [i]
        .if4:
        mov  eax, DWORD PTR -4[rbp]
        mov  ebx, 2
        cmp  eax, ebx
        sete al
            cmp  al, 1
            jne  .elif7
            mov  DWORD PTR -4[rbp], 3 ;// assign [i]
        jmp  .endif6
        .elif7:
        mov  eax, DWORD PTR -4[rbp]
        mov  ebx, 3
        cmp  eax, ebx
        sete al
            cmp  al, 1
            jne  .else9
            mov  DWORD PTR -4[rbp], 4 ;// assign [i]
        jmp  .endif6
        .else9:
            mov  DWORD PTR -4[rbp], 2 ;// assign [i]
            .if10:
            mov  eax, DWORD PTR -4[rbp]
            mov  ebx, 2
            cmp  eax, ebx
            sete al
                cmp  al, 1
                jne  .endif12
                mov  DWORD PTR -4[rbp], 5 ;// assign [i]
            .endif12:
        .endif6:
    jmp  .endif3
    .else13:
        mov  DWORD PTR -4[rbp], 4 ;// assign [i]
    .endif3:
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

