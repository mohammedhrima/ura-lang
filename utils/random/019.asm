section .data
    msg db "Hello world %d", 0

section .bss
    a resd 1

section .text
    extern printf
    global main

main:
    ; Initialize a
    mov dword [a], 10

    ; Prepare arguments for printf
    push dword [a]
    push msg
    call printf
    add esp, 8

    ; Return from main
    xor eax, eax
    ret
