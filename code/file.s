.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 8
	//      assign [i]
	mov     DWORD PTR -4[rbp], 1
.while1:
	cmp     DWORD PTR -4[rbp], 10
	jge     .endwhile1
	//      assign [edi]
	mov     edi, 1
	//      assign [rsi]
	lea     rsi, .STR1[rip]
	//      assign [edx]
	mov     edx, 1
	call    write
	//      add_assign [i]
	add     DWORD PTR -4[rbp], 1
	jmp     .while1
.endwhile1:
	mov     eax, DWORD PTR -4[rbp]
	leave   
	ret     
.endmain:
.STR1: .string "x"
.section	.note.GNU-stack,"",@progbits

