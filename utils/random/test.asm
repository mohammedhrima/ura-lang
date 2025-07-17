section .data
	text db "hello, World", 10
    len equ $ - text

section .text
	global _start

_start:
	mov rax, 1      ; sys_write
	mov rdi, 1      ; STDOUT
	mov rsi, text	; pointer
	mov rdx, len	; length
	syscall

	mov rax, 60	; exit
	mov rdi, 0	; exit code
	syscall
