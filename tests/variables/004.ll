; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c" \00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %board = alloca i32*, align 8, !dbg !7
  %r = alloca i32, align 4, !dbg !7
  %c = alloca i32, align 4, !dbg !7
  %c22 = alloca i32, align 4, !dbg !7
  store i32* null, i32** %board, align 8, !dbg !7
  %stack = alloca [48 x i32], align 4, !dbg !7
  %stack1 = getelementptr [48 x i32], [48 x i32]* %stack, i32 0, i32 0, !dbg !7
  store i32* %stack1, i32** %board, align 8, !dbg !7
  store i32 0, i32* %r, align 4, !dbg !7
  store i32 0, i32* %r, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.end5, %entry
  %r2 = load i32, i32* %r, align 4, !dbg !7
  %LT = icmp slt i32 %r2, 4, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  store i32 0, i32* %c, align 4, !dbg !7
  store i32 0, i32* %c, align 4, !dbg !7
  br label %while.start3, !dbg !7

while.end:                                        ; preds = %while.start
  store i32 0, i32* %r, align 4, !dbg !8
  br label %while.start17, !dbg !8

while.start3:                                     ; preds = %while.then4, %while.then
  %c6 = load i32, i32* %c, align 4, !dbg !7
  %LT7 = icmp slt i32 %c6, 3, !dbg !7
  br i1 %LT7, label %while.then4, label %while.end5, !dbg !7

while.then4:                                      ; preds = %while.start3
  %board8 = load i32*, i32** %board, align 8, !dbg !8
  %r9 = load i32, i32* %r, align 4, !dbg !8
  %flat_idx = mul i32 %r9, 3, !dbg !8
  %row = getelementptr i32, i32* %board8, i32 %flat_idx, !dbg !8
  %c10 = load i32, i32* %c, align 4, !dbg !8
  %ACCESS = getelementptr i32, i32* %row, i32 %c10, !dbg !8
  %r11 = load i32, i32* %r, align 4, !dbg !8
  %MUL = mul i32 %r11, 3, !dbg !8
  %c12 = load i32, i32* %c, align 4, !dbg !8
  %ADD = add i32 %MUL, %c12, !dbg !8
  store i32 %ADD, i32* %ACCESS, align 4, !dbg !8
  %c13 = load i32, i32* %c, align 4, !dbg !8
  %ADD14 = add i32 %c13, 1, !dbg !8
  store i32 %ADD14, i32* %c, align 4, !dbg !8
  br label %while.start3, !dbg !8

while.end5:                                       ; preds = %while.start3
  %r15 = load i32, i32* %r, align 4, !dbg !8
  %ADD16 = add i32 %r15, 1, !dbg !8
  store i32 %ADD16, i32* %r, align 4, !dbg !8
  br label %while.start, !dbg !8

while.start17:                                    ; preds = %while.end25, %while.end
  %r20 = load i32, i32* %r, align 4, !dbg !8
  %LT21 = icmp slt i32 %r20, 4, !dbg !8
  br i1 %LT21, label %while.then18, label %while.end19, !dbg !8

while.then18:                                     ; preds = %while.start17
  store i32 0, i32* %c22, align 4, !dbg !7
  store i32 0, i32* %c22, align 4, !dbg !7
  br label %while.start23, !dbg !7

while.end19:                                      ; preds = %while.start17
  ret i32 0, !dbg !9

while.start23:                                    ; preds = %while.then24, %while.then18
  %c26 = load i32, i32* %c22, align 4, !dbg !7
  %LT27 = icmp slt i32 %c26, 3, !dbg !7
  br i1 %LT27, label %while.then24, label %while.end25, !dbg !7

while.then24:                                     ; preds = %while.start23
  %board28 = load i32*, i32** %board, align 8, !dbg !9
  %r29 = load i32, i32* %r, align 4, !dbg !9
  %flat_idx30 = mul i32 %r29, 3, !dbg !9
  %row31 = getelementptr i32, i32* %board28, i32 %flat_idx30, !dbg !9
  %c32 = load i32, i32* %c22, align 4, !dbg !9
  %ACCESS33 = getelementptr i32, i32* %row31, i32 %c32, !dbg !9
  %ACC = load i32, i32* %ACCESS33, align 4, !dbg !9
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %ACC), !dbg !9
  %c34 = load i32, i32* %c22, align 4, !dbg !9
  %ADD35 = add i32 %c34, 1, !dbg !9
  store i32 %ADD35, i32* %c22, align 4, !dbg !9
  br label %while.start23, !dbg !9

while.end25:                                      ; preds = %while.start23
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !9
  %r36 = load i32, i32* %r, align 4, !dbg !9
  %ADD37 = add i32 %r36, 1, !dbg !9
  store i32 %ADD37, i32* %r, align 4, !dbg !9
  br label %while.start17, !dbg !9
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 9, scope: !4)
!9 = !DILocation(line: 17, scope: !4)
