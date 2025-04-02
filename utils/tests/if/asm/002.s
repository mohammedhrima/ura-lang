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
    cmp  DWORD PTR -4[rbp], 1
    jne   .else10
        mov  DWORD PTR -4[rbp], 2 ;// assign [i]
        .if3:
        cmp  DWORD PTR -4[rbp], 2
        jne   .elif5
            mov  DWORD PTR -4[rbp], 3 ;// assign [i]
            jmp  .endif3
        .elif5:
        cmp  DWORD PTR -4[rbp], 3
        jne   .else7
            mov  DWORD PTR -4[rbp], 4 ;// assign [i]
            jmp  .endif3
        .else7:
            mov  DWORD PTR -4[rbp], 2 ;// assign [i]
            .if8:
            cmp  DWORD PTR -4[rbp], 2
            jne   .endif8
                mov  DWORD PTR -4[rbp], 5 ;// assign [i]
                .endif8:
        .endif3:
        jmp  .endif1
    .else10:
        mov  DWORD PTR -4[rbp], 4 ;// assign [i]
        .endif1:
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

