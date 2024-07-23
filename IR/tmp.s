.intel_syntax noprefix
.text
.section	.rodata
.LC0: .string	"%d\n"
.text

_putnbr:
	endbr64
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		DWORD PTR -4[rbp], edi
	mov		eax, DWORD PTR -4[rbp]
	mov		esi, eax
	lea		rdi, .LC0[rip]
	mov		eax, 0
	call	printf@PLT
	nop
	leave
	ret

func:
	endbr64
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		edi, DWORD PTR 16[rbp]
	call	_putnbr
	nop
	leave
	ret

.globl	main
.type	main, @function
main:
	endbr64
	push	rbp
	mov		rbp, rsp
	sub		rsp, 8
	push	10
	call	func
	add		rsp, 272
	mov		eax, 0
	leave
	ret
.section	.note.GNU-stack,"",@progbits
.section	.note.gnu.property,"a"
