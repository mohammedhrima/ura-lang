	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w0, wzr
Lloh0:
	adrp	x8, l_str_literal@PAGE
Lloh1:
	add	x8, x8, l_str_literal@PAGEOFF
	str	x8, [sp, #8]
LBB0_1:                                 ; %while
                                        ; =>This Inner Loop Header: Depth=1
	ldr	x8, [sp, #8]
	str	w0, [sp, #4]
	ldrb	w8, [x8, w0, sxtw]
	cbz	w8, LBB0_3
; %bb.2:                                ; %while_bloc
                                        ;   in Loop: Header=BB0_1 Depth=1
	add	w0, w0, #1
	b	LBB0_1
LBB0_3:                                 ; %end_while
	add	sp, sp, #16
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
l_str_literal:                          ; @str_literal
	.asciz	"abcd"

.subsections_via_symbols
