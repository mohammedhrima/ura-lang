	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_bounds_check                   ; -- Begin function bounds_check
	.p2align	2
_bounds_check:                          ; @bounds_check
	.cfi_startproc
; %bb.0:                                ; %entry
                                        ; kill: def $w2 killed $w2 def $x2
                                        ; kill: def $w0 killed $w0 def $x0
	tbnz	w0, #31, LBB0_3
; %bb.1:                                ; %entry
	cmp	w0, w1
	b.ge	LBB0_3
; %bb.2:                                ; %valid
	ret
LBB0_3:                                 ; %error
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
Lloh0:
	adrp	x8, l_err_header@PAGE
Lloh1:
	add	x8, x8, l_err_header@PAGEOFF
	mov	x19, x0
	mov	x0, x8
	str	x2, [sp]
	mov	x20, x4
	bl	_printf
Lloh2:
	adrp	x0, l_err_msg@PAGE
Lloh3:
	add	x0, x0, l_err_msg@PAGEOFF
	stp	x20, x19, [sp]
	bl	_printf
Lloh4:
	adrp	x0, l_err_pointer@PAGE
Lloh5:
	add	x0, x0, l_err_pointer@PAGEOFF
	bl	_printf
	mov	w0, #1                          ; =0x1
	bl	_exit
	brk	#0x1
	.loh AdrpAdd	Lloh4, Lloh5
	.loh AdrpAdd	Lloh2, Lloh3
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_sum                            ; -- Begin function sum
	.p2align	2
_sum:                                   ; @sum
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	mov	w8, wzr
	add	x9, sp, #32
	str	w0, [sp, #28]
	str	x9, [sp, #16]
	str	wzr, [sp, #12]
LBB1_1:                                 ; %while.cond.0
                                        ; =>This Inner Loop Header: Depth=1
	ldr	w9, [sp, #28]
	str	w8, [sp, #8]
	cmp	w8, w9
	b.ge	LBB1_3
; %bb.2:                                ; %while.body.1
                                        ;   in Loop: Header=BB1_1 Depth=1
	ldr	x8, [sp, #16]
	add	x9, x8, #8
	str	x9, [sp, #16]
	ldp	w10, w9, [sp, #8]
	ldr	w8, [x8]
	add	w8, w9, w8
	str	w8, [sp, #12]
	add	w8, w10, #1
	b	LBB1_1
LBB1_3:                                 ; %while.end.2
	ldr	w0, [sp, #12]
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #30                         ; =0x1e
	mov	w9, #20                         ; =0x14
	mov	w0, #3                          ; =0x3
	stp	x9, x8, [sp, #8]
	mov	w8, #10                         ; =0xa
	str	x8, [sp]
	bl	_sum
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	str	w0, [sp, #28]
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_err_header:                           ; @err_header
	.asciz	"\nline %d: segmentation fault (array bounds)\n"

l_err_msg:                              ; @err_msg
	.asciz	"error in accessing %s[%d]\n"

l_err_pointer:                          ; @err_pointer
	.asciz	"                   ^^^^\n"

.subsections_via_symbols
