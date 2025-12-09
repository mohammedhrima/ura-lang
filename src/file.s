	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_strlen                         ; -- Begin function strlen
	.p2align	2
_strlen:                                ; @strlen
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	x8, x0
	mov	w0, wzr
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
	.cfi_endproc
                                        ; -- End function
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
	adrp	x0, l_STR0@PAGE
Lloh1:
	add	x0, x0, l_STR0@PAGEOFF
	bl	_strlen
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	str	w0, [sp, #12]
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_STR0:                                 ; @STR0
	.asciz	"abc"

.subsections_via_symbols
