	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh0:
	adrp	x8, l_STR0@PAGE
	ldrsb	w0, [x8, l_STR0@PAGEOFF]
Lloh1:
	adrp	x8, l_STR0@PAGE
Lloh2:
	add	x8, x8, l_STR0@PAGEOFF
	str	x8, [sp, #8]
	strb	w0, [sp, #7]
	bl	_putchar
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	mov	w0, wzr
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh1, Lloh2
	.loh AdrpAdrp	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_STR0:                                 ; @STR0
	.asciz	"abcd"

.subsections_via_symbols
