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
	//      pop [d]
	mov     DWORD PTR -16[rbp], ecx
	//      pop [e]
	mov     DWORD PTR -20[rbp], r8d
	//      pop [f]
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
	//      assign [a]
	mov     DWORD PTR -4[rbp], 10
	//      assign [b]
	mov     DWORD PTR -8[rbp], 11
	//      assign [c]
	mov     DWORD PTR -12[rbp], 12
	//      assign [d]
	mov     DWORD PTR -16[rbp], 13
	//      assign [e]
	mov     DWORD PTR -20[rbp], 14
	//      assign [f]
	mov     DWORD PTR -24[rbp], 15
	//      assign [g]
	mov     DWORD PTR -28[rbp], 14
	//      assign [h]
	mov     DWORD PTR -32[rbp], 15
	mov     edi, DWORD PTR -4[rbp]
	mov     esi, DWORD PTR -8[rbp]
	mov     edx, DWORD PTR -12[rbp]
	mov     ecx, DWORD PTR -16[rbp]
	mov     r8d, DWORD PTR -20[rbp]
	mov     r9d, DWORD PTR -24[rbp]
	mov     eax, DWORD PTR -28[rbp]
	push    rax
	call    foo
	mov     eax, 0
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

