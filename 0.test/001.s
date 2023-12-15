	.text
	.file	"001.c"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$232, %rsp
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movl	$0, -44(%rbp)
	movl	$1, -48(%rbp)
	movl	$2, -52(%rbp)
	movl	$3, -56(%rbp)
	movl	$4, -60(%rbp)
	movl	$5, -64(%rbp)
	movl	$6, -68(%rbp)
	movl	$7, -72(%rbp)
	movl	$8, -76(%rbp)
	movl	$9, -80(%rbp)
	movl	$10, -84(%rbp)
	movl	$11, -88(%rbp)
	movl	$12, -92(%rbp)
	movl	$13, -96(%rbp)
	movl	$14, -100(%rbp)
	movl	$15, -104(%rbp)
	movl	$16, -108(%rbp)
	movl	$17, -112(%rbp)
	movl	$18, -116(%rbp)
	movl	$19, -120(%rbp)
	movl	$20, -124(%rbp)
	movl	-48(%rbp), %edi
	movl	-52(%rbp), %esi
	movb	$0, %al
	callq	func
	movl	-48(%rbp), %edi
	movl	-52(%rbp), %esi
	movl	-56(%rbp), %edx
	movl	-60(%rbp), %ecx
	movl	-64(%rbp), %r8d
	movl	-68(%rbp), %r9d
	movl	-72(%rbp), %eax
	movl	%eax, -152(%rbp)                # 4-byte Spill
	movl	-76(%rbp), %eax
	movl	%eax, -148(%rbp)                # 4-byte Spill
	movl	-80(%rbp), %eax
	movl	%eax, -144(%rbp)                # 4-byte Spill
	movl	-84(%rbp), %eax
	movl	%eax, -140(%rbp)                # 4-byte Spill
	movl	-88(%rbp), %eax
	movl	%eax, -136(%rbp)                # 4-byte Spill
	movl	-92(%rbp), %eax
	movl	%eax, -132(%rbp)                # 4-byte Spill
	movl	-96(%rbp), %r13d
	movl	-100(%rbp), %r12d
	movl	-104(%rbp), %r15d
	movl	-108(%rbp), %r14d
	movl	-112(%rbp), %ebx
	movl	-116(%rbp), %r11d
	movl	-120(%rbp), %r10d
	movl	-124(%rbp), %eax
	movl	%eax, -128(%rbp)                # 4-byte Spill
	movl	-152(%rbp), %eax                # 4-byte Reload
	movl	%eax, (%rsp)
	movl	-148(%rbp), %eax                # 4-byte Reload
	movl	%eax, 8(%rsp)
	movl	-144(%rbp), %eax                # 4-byte Reload
	movl	%eax, 16(%rsp)
	movl	-140(%rbp), %eax                # 4-byte Reload
	movl	%eax, 24(%rsp)
	movl	-136(%rbp), %eax                # 4-byte Reload
	movl	%eax, 32(%rsp)
	movl	-132(%rbp), %eax                # 4-byte Reload
	movl	%eax, 40(%rsp)
	movl	-128(%rbp), %eax                # 4-byte Reload
	movl	%r13d, 48(%rsp)
	movl	%r12d, 56(%rsp)
	movl	%r15d, 64(%rsp)
	movl	%r14d, 72(%rsp)
	movl	%ebx, 80(%rsp)
	movl	%r11d, 88(%rsp)
	movl	%r10d, 96(%rsp)
	movl	%eax, 104(%rsp)
	movb	$0, %al
	callq	func
	xorl	%eax, %eax
	addq	$232, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.ident	"Ubuntu clang version 12.0.1-19ubuntu3"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym func
