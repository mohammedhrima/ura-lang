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
	mov	w0, #10                         ; =0xa
	mov	w1, #1                          ; =0x1
	bl	_calloc
Lloh0:
	adrp	x1, l_src_literal@PAGE
Lloh1:
	add	x1, x1, l_src_literal@PAGEOFF
	str	x0, [sp, #8]
	bl	_strcpy
	mov	w9, wzr
	mov	w8, #97                         ; =0x61
LBB0_1:                                 ; %while.cond
                                        ; =>This Inner Loop Header: Depth=1
	ldr	x0, [sp, #8]
	cmp	w9, #1
	str	w9, [sp, #4]
	b.gt	LBB0_3
; %bb.2:                                ; %while.body
                                        ;   in Loop: Header=BB0_1 Depth=1
	ldrsw	x9, [sp, #4]
	strb	w8, [x0, x9]
	add	w9, w9, #1
	b	LBB0_1
LBB0_3:                                 ; %while.end
	bl	_puts
	ldr	x0, [sp, #8]
	bl	_free
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	mov	w0, wzr
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_src_literal:                          ; @src_literal
	.asciz	"abcdef"

.subsections_via_symbols
