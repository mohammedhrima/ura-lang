	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	adrp	x8, l_str_literal@PAGE
Lloh0:
	adrp	x9, l_str_literal@PAGE
Lloh1:
	add	x9, x9, l_str_literal@PAGEOFF
	ldrb	w8, [x8, l_str_literal@PAGEOFF]
	mov	w0, wzr
	str	x9, [sp, #24]
	str	w8, [sp, #20]
	add	w8, w8, #2
	stp	w8, w8, [sp, #12]
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
l_str_literal:                          ; @str_literal
	.asciz	"abc"

.subsections_via_symbols
