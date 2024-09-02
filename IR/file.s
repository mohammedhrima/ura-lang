.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
foo:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 32
	//      pop [a]
	mov     DWORD PTR -4[rbp], edi
	//      pop [b]
	mov     DWORD PTR -8[rbp], esi
	//      pop [c]
	mov     DWORD PTR -12[rbp], edx
	//      pop [e]
	mov     DWORD PTR -16[rbp], ecx
	//      pop [f]
	mov     DWORD PTR -20[rbp], r8d
	//      pop [g]
	mov     DWORD PTR -24[rbp], r9d
	mov     edi, DWORD PTR -4[rbp]
	call    putnbr
	mov     eax, 0
	leave   
	ret     
.endfoo:
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 32
	//      declare [a]
	mov     DWORD PTR -4[rbp], 0
	//      assign [a]
	mov     DWORD PTR -4[rbp], 10
	//      declare [b]
	mov     DWORD PTR -8[rbp], 0
	//      assign [b]
	mov     DWORD PTR -8[rbp], 11
	//      declare [c]
	mov     DWORD PTR -12[rbp], 0
	//      assign [c]
	mov     DWORD PTR -12[rbp], 12
	//      declare [d]
	mov     DWORD PTR -16[rbp], 0
	//      assign [d]
	mov     DWORD PTR -16[rbp], 13
	//      declare [e]
	mov     DWORD PTR -20[rbp], 0
	//      assign [e]
	mov     DWORD PTR -20[rbp], 14
	mov     edi, DWORD PTR -4[rbp]
	mov     esi, DWORD PTR -4[rbp]
	mov     edx, DWORD PTR -4[rbp]
	mov     ecx, DWORD PTR -4[rbp]
	mov     r8d, DWORD PTR -4[rbp]
	mov     r9d, DWORD PTR -4[rbp]
	mov     eax, DWORD PTR -4[rbp]
	push    rax
	call    foo
	mov     eax, 0
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

