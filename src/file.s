	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_to_ascii                       ; -- Begin function to_ascii
	.p2align	2
_to_ascii:                              ; @to_ascii
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w8, w0
	mov	w0, #-1                         ; =0xffffffff
	sxtb	w9, w8
	strb	w8, [sp, #15]
	str	w9, [sp, #8]
	tbnz	w8, #7, LBB0_3
; %bb.1:                                ; %entry
	cmp	w9, #128
	b.ge	LBB0_3
; %bb.2:                                ; %end_if
	ldr	w8, [sp, #8]
	mov	w9, #123                        ; =0x7b
	mov	w10, #91                        ; =0x5b
	cmp	w8, #96
	ccmp	w8, w9, #0, gt
	cset	w9, lt
	cmp	w8, #64
	ccmp	w8, w10, #0, gt
	mov	w10, #58                        ; =0x3a
	csinc	w9, w9, wzr, ge
	cmp	w8, #47
	ccmp	w8, w10, #0, gt
	csinc	w9, w9, wzr, ge
	cmp	w8, #32
	csinc	w9, w9, wzr, ne
	cmp	w8, #0
	csinc	w9, w9, wzr, ne
	cmp	w9, #0
	csinv	w0, w8, wzr, ne
LBB0_3:                                 ; %common.ret
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_strncmp                        ; -- Begin function strncmp
	.p2align	2
_strncmp:                               ; @strncmp
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	sub	sp, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	stp	x1, x0, [x29, #-32]
	stp	wzr, w2, [x29, #-40]
	cbz	w2, LBB1_6
LBB1_1:                                 ; %while
                                        ; =>This Inner Loop Header: Depth=1
	ldp	w8, w9, [x29, #-40]
                                        ; kill: def $w8 killed $w8 def $x8
	sxtw	x8, w8
	cmp	w8, w9
	b.ge	LBB1_5
; %bb.2:                                ; %while
                                        ;   in Loop: Header=BB1_1 Depth=1
	ldp	x10, x9, [x29, #-32]
	ldrb	w9, [x9, x8]
	ldrb	w10, [x10, x8]
	cmp	w9, w10
	b.ne	LBB1_5
; %bb.3:                                ; %while
                                        ;   in Loop: Header=BB1_1 Depth=1
	cbz	w9, LBB1_5
; %bb.4:                                ; %while_bloc
                                        ;   in Loop: Header=BB1_1 Depth=1
	add	w8, w8, #1
	stur	w8, [x29, #-40]
	b	LBB1_1
LBB1_5:                                 ; %end_while
	ldur	w9, [x29, #-36]
	cmp	w8, w9
	b.ne	LBB1_7
LBB1_6:
	mov	w0, wzr
	b	LBB1_8
LBB1_7:                                 ; %end_if25
	mov	x19, sp
	sub	x8, x19, #16
	mov	sp, x8
	ldur	x8, [x29, #-24]
	ldursw	x9, [x29, #-40]
	ldrb	w0, [x8, x9]
	bl	_to_ascii
	mov	x20, sp
	stur	w0, [x19, #-16]
	sub	x8, x20, #16
	mov	sp, x8
	ldur	x8, [x29, #-32]
	ldursw	x9, [x29, #-40]
	ldrb	w0, [x8, x9]
	bl	_to_ascii
	ldur	w8, [x19, #-16]
	stur	w0, [x20, #-16]
	sub	w0, w8, w0
LBB1_8:                                 ; %common.ret
	sub	sp, x29, #16
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
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
Lloh2:
	adrp	x1, l_STR1@PAGE
Lloh3:
	add	x1, x1, l_STR1@PAGEOFF
	mov	w2, #2                          ; =0x2
	bl	_strncmp
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	str	w0, [sp, #12]
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh2, Lloh3
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_STR0:                                 ; @STR0
	.asciz	"abd"

l_STR1:                                 ; @STR1
	.asciz	"abc"

.subsections_via_symbols
