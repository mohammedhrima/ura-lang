	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	add	x8, sp, #6
Lloh0:
	adrp	x3, l_filename@PAGE
Lloh1:
	add	x3, x3, l_filename@PAGEOFF
	mov	w0, #10                         ; =0xa
	mov	w1, #2                          ; =0x2
	mov	w2, #2                          ; =0x2
	str	x8, [sp, #8]
	bl	___bounds_check
	mov	w19, #51                        ; =0x33
	ldr	x20, [sp, #8]
Lloh2:
	adrp	x3, l_filename.1@PAGE
Lloh3:
	add	x3, x3, l_filename.1@PAGEOFF
	mov	w0, #100                        ; =0x64
	mov	w1, #2                          ; =0x2
	mov	w2, #3                          ; =0x3
	strb	w19, [sp, #16]
	bl	___bounds_check
	strb	w19, [x20, #100]
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	mov	w0, wzr
	add	sp, sp, #48
	ret
	.loh AdrpAdd	Lloh2, Lloh3
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	___bounds_check                 ; -- Begin function __bounds_check
	.p2align	2
___bounds_check:                        ; @__bounds_check
	.cfi_startproc
; %bb.0:                                ; %entry
	mov	w8, w0
                                        ; kill: def $w2 killed $w2 def $x2
                                        ; kill: def $w1 killed $w1 def $x1
	tbnz	w0, #31, LBB1_3
; %bb.1:                                ; %entry
	cmp	w8, w1
	b.ge	LBB1_3
; %bb.2:                                ; %ok
	ret
LBB1_3:                                 ; %error
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh4:
	adrp	x0, l_err_fmt@PAGE
Lloh5:
	add	x0, x0, l_err_fmt@PAGEOFF
	stp	x3, x2, [sp, #16]
	stp	x8, x1, [sp]
	bl	_printf
	mov	w0, #1                          ; =0x1
	bl	_exit
	brk	#0x1
	.loh AdrpAdd	Lloh4, Lloh5
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_STR0:                                 ; @STR0
	.space	1

l_err_fmt:                              ; @err_fmt
	.asciz	"Error: index %d out of bounds (size: %d) at %s:%d\n"

l_filename:                             ; @filename
	.asciz	"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

l_filename.1:                           ; @filename.1
	.asciz	"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

.subsections_via_symbols
