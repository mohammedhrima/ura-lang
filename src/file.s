	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_isdigit                        ; -- Begin function isdigit
	.p2align	2
_isdigit:                               ; @isdigit
	.cfi_startproc
; %bb.0:                                ; %entry
	sxtb	w9, w0
	mov	w8, #58                         ; =0x3a
	cmp	w9, #47
	ccmp	w9, w8, #0, gt
	cset	w0, lt
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #49                         ; =0x31
	bl	_isdigit
Lloh0:
	adrp	x8, l_str_literal.1@PAGE
Lloh1:
	add	x8, x8, l_str_literal.1@PAGEOFF
	tst	w0, #0x1
Lloh2:
	adrp	x9, l_str_literal@PAGE
Lloh3:
	add	x9, x9, l_str_literal@PAGEOFF
	csel	x0, x9, x8, ne
	bl	_puts
	mov	w0, wzr
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh2, Lloh3
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
l_str_literal:                          ; @str_literal
	.asciz	"is digit"

l_str_literal.1:                        ; @str_literal.1
	.asciz	"is not digit"

.subsections_via_symbols
