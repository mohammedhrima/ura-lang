.intel_syntax noprefix
.section	.note.GNU-stack,"",@progbits
.text
	.globl	main

ft_putchar:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		rax, rdi
	mov		BYTE PTR -4[rbp], al
	lea		rax, -4[rbp]
	mov		rdi, 1		/* stdout  */
	mov		rdx, 1		/* length  */
	mov		rsi, rax	/* pointer */
	call	write@PLT
	leave
	ret

ft_putnbr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32

	/* mov the value pointer by rdi to rbp-24 */
	mov		QWORD PTR -24[rbp], rdi
	/* cmp rbp-24 to 0*/
	cmp		QWORD PTR -24[rbp], 0
	/* jump if not less than 0 to L3 */
	jns		L3
	/* putchar - */
	mov		rdi, 45
	call	ft_putchar
	/* rbp-24 = -rbp-24 */
	neg		QWORD PTR -24[rbp]
L3:
	/* cmp rbp-24 to 9 */
	cmp		QWORD PTR -24[rbp], 9
	/* jump if greater than 9 */
	jg		L4
	mov		rax, QWORD PTR -24[rbp]
	/* num + '0' */
	add		rax, 48
	mov		rdi, rax
	call	ft_putchar
	jmp	L6
L4:
	/* putnbr(num / 10) */
	mov		rcx, QWORD PTR -24[rbp]
	movabs	rdx, 7378697629483820647
	mov		rax, rcx
	imul	rdx
	
	mov		rax, rdx
	sar		rax, 2
	sar		rcx, 63
	
	mov		rdx, rcx
	sub		rax, rdx

	mov		rdi, rax
	call	ft_putnbr

	/* putnbr(num % 10) */
	mov		rcx, QWORD PTR -24[rbp]
	movabs	rdx, 7378697629483820647
	mov		rax, rcx
	imul	rdx
	
	mov		rax, rcx
	sar		rdx, 2
	sar		rax, 63

	sub		rdx, rax
	mov		rax, rdx
	sal		rax, 2
	add		rax, rdx
	add		rax, rax
	sub		rcx, rax
	mov		rdx, rcx
	
	mov		rdi, rdx
	call	ft_putnbr
L6:
	leave
	ret

main:
	push	rbp
	mov		rbp, rsp
	/*set argument for putnbr*/
	mov		rdi, -127
	call	ft_putnbr
	mov		rdi, 10
	call	ft_putchar
	mov		rax, 0
	pop		rbp
	ret
