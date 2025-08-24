	.file	"while_loop"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$0, -4(%rsp)
	cmpl	$4, -4(%rsp)
	jg	.LBB0_3
	.p2align	4
.LBB0_2:                                # %loop_body
                                        # =>This Inner Loop Header: Depth=1
	incl	-4(%rsp)
	cmpl	$4, -4(%rsp)
	jle	.LBB0_2
.LBB0_3:                                # %end
	movl	-4(%rsp), %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
