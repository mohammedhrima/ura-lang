	.file	"file.w"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$1, -4(%rsp)
	.p2align	4
.LBB0_1:                                # %while
                                        # =>This Inner Loop Header: Depth=1
	cmpl	$9, -4(%rsp)
	jg	.LBB0_4
# %bb.2:                                # %while_bloc
                                        #   in Loop: Header=BB0_1 Depth=1
	cmpl	$9, -4(%rsp)
	je	.LBB0_1
# %bb.3:                                # %else
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	-4(%rsp)
	jmp	.LBB0_1
.LBB0_4:                                # %end_while
	movl	-4(%rsp), %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
