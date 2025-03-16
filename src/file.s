.intel_syntax noprefix
.include "/import/header.s"

.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 0 ;// assign [i]
    .while1:
    mov  eax, DWORD PTR -4[rbp]
    mov  ebx, 5
    cmp  eax, ebx
    setl al
        cmp  al, 1
        jne  .endwhile2
        lea  rdi, .STR1[rip] ;// assign [rdi]
        call putstr
        mov  DWORD PTR -8[rbp], 0 ;// assign [j]
        .while4:
        mov  eax, DWORD PTR -8[rbp]
        mov  ebx, 2
        cmp  eax, ebx
        setl al
            cmp  al, 1
            jne  .endwhile5
            jmp  .while4
        .endwhile5:
        lea  rdi, .STR2[rip] ;// assign [rdi]
        call putstr
        mov  eax, DWORD PTR -8[rbp]
        add  eax, 1
        mov  DWORD PTR -8[rbp], eax ;// assign [j]
        lea  rdi, .STR3[rip] ;// assign [rdi]
        call putstr
        add  DWORD PTR -4[rbp], 1 ;// add_assign [i]
        jmp  .while1
    .endwhile2:
    mov  eax, 0
    leave 
    ret  
.endmain:.STR1: .string "a"
.STR2: .string "b"
.STR3: .string "\n"
.section	.note.GNU-stack,"",@progbits

