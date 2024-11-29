	.file	"001.c"
	.intel_syntax noprefix
	.text
	.weak	__stack_chk_guard
	.bss
	.align 8
	.type	__stack_chk_guard, @object
	.size	__stack_chk_guard, 8
__stack_chk_guard:
	.zero	8
	.text
	.type	__construct_stk_chk_guard, @function
__construct_stk_chk_guard:
	endbr64
	push	rbp
	mov	rbp, rsp
	mov	rax, QWORD PTR __stack_chk_guard[rip]
	test	rax, rax
	jne	.L3
	call	__stack_chk_guard_init
	mov	QWORD PTR __stack_chk_guard[rip], rax
.L3:
	nop
	pop	rbp
	ret
	.size	__construct_stk_chk_guard, .-__construct_stk_chk_guard
	.section	.init_array,"aw"
	.align 8
	.quad	__construct_stk_chk_guard
	.text
	.weak	__stack_chk_guard_init
	.type	__stack_chk_guard_init, @function
__stack_chk_guard_init:
	endbr64
	push	rbp
	mov	rbp, rsp
	movabs	rax, -2401053089837972110
	pop	rbp
	ret
	.size	__stack_chk_guard_init, .-__stack_chk_guard_init
	.section	.rodata
	.align 8
.LC0:
	.string	"Stack overflow detected! exiting the program."
	.text
	.weak	__stack_chk_fail
	.type	__stack_chk_fail, @function
__stack_chk_fail:
	endbr64
	push	rbp
	mov	rbp, rsp
	lea	rdi, .LC0[rip]
	call	puts@PLT
	mov	edi, 1
	call	exit@PLT
	.size	__stack_chk_fail, .-__stack_chk_fail
	.globl	buffer_long
	.section	.rodata
	.align 8
.LC1:
	.string	"This is a long lontartartertertrtertretgggggggggggggggggggggagagfartawrgggggggggggggggggg string"
	.section	.data.rel.local,"aw"
	.align 8
	.type	buffer_long, @object
	.size	buffer_long, 8
buffer_long:
	.quad	.LC1
	.section	.rodata
.LC2:
	.string	"Overflow case run."
	.text
	.globl	stack_overflows_here
	.type	stack_overflows_here, @function
stack_overflows_here:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	mov	rdx, QWORD PTR buffer_long[rip]
	lea	rax, -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	strcpy@PLT
	lea	rdi, .LC2[rip]
	mov	eax, 0
	call	printf@PLT
	nop
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, QWORD PTR fs:40
	je	.L8
	call	__stack_chk_fail@PLT
.L8:
	leave
	ret
	.size	stack_overflows_here, .-stack_overflows_here
	.section	.rodata
	.align 8
.LC3:
	.string	"Running stack overflow test program:"
	.text
	.globl	main
	.type	main, @function
main:
	endbr64
	push	rbp
	mov	rbp, rsp
	lea	rdi, .LC3[rip]
	call	puts@PLT
	mov	eax, 0
	call	stack_overflows_here
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 10.5.0-1ubuntu1~22.04) 10.5.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
