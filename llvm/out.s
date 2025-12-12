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
	add	x0, sp, #14
	mov	w1, #-1                         ; =0xffffffff
	mov	w2, #2                          ; =0x2
	mov	w3, #10                         ; =0xa
	mov	w4, #5                          ; =0x5
	bl	___ura_bounds_check
	mov	w8, #97                         ; =0x61
	add	x0, sp, #14
	mov	w1, wzr
	mov	w2, #2                          ; =0x2
	mov	w3, #15                         ; =0xf
	mov	w4, #5                          ; =0x5
	sturb	w8, [sp, #13]
	bl	___ura_bounds_check
	mov	w8, #72                         ; =0x48
	add	x0, sp, #14
	mov	w1, #1                          ; =0x1
	mov	w2, #2                          ; =0x2
	mov	w3, #20                         ; =0x14
	mov	w4, #5                          ; =0x5
	strb	w8, [sp, #14]
	bl	___ura_bounds_check
	mov	w8, #105                        ; =0x69
	add	x0, sp, #14
	mov	w1, #2                          ; =0x2
	mov	w2, #2                          ; =0x2
	mov	w3, #25                         ; =0x19
	mov	w4, #5                          ; =0x5
	strb	w8, [sp, #15]
	bl	___ura_bounds_check
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	mov	w8, #33                         ; =0x21
	mov	w0, wzr
	strb	w8, [sp, #16]
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	___ura_bounds_check             ; -- Begin function __ura_bounds_check
	.p2align	2
___ura_bounds_check:                    ; @__ura_bounds_check
	.cfi_startproc
; %bb.0:                                ; %entry
                                        ; kill: def $w4 killed $w4 def $x4
                                        ; kill: def $w3 killed $w3 def $x3
                                        ; kill: def $w1 killed $w1 def $x1
                                        ; kill: def $w2 killed $w2 def $x2
	tbnz	w1, #31, LBB1_3
; %bb.1:                                ; %entry
	cmp	w1, w2
	b.ge	LBB1_3
; %bb.2:                                ; %ok
	ret
LBB1_3:                                 ; %error
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh0:
	adrp	x0, l_STR0@PAGE
Lloh1:
	add	x0, x0, l_STR0@PAGEOFF
	stp	x1, x2, [sp, #16]
	stp	x3, x4, [sp]
	bl	_printf
	mov	w0, #1                          ; =0x1
	bl	_exit
	brk	#0x1
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_STR0:                                 ; @STR0
	.asciz	"\n\033[1;31m=================================================================\033[0m\n\033[1;31mRUNTIME ERROR: Array Index Out of Bounds\033[0m\n\033[1;31m=================================================================\033[0m\nLocation: \033[1;33mline %d, column %d\033[0m\nIndex:    \033[1;36m%d\033[0m\nMax size: \033[1;36m%d\033[0m\n\033[1;31m=================================================================\033[0m\n\n"

.subsections_via_symbols
