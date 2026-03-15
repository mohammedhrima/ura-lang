; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c" \00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %cube = alloca i32*, align 8, !dbg !7
  %x = alloca i32, align 4, !dbg !7
  %y = alloca i32, align 4, !dbg !7
  %z = alloca i32, align 4, !dbg !7
  %y35 = alloca i32, align 4, !dbg !7
  %z41 = alloca i32, align 4, !dbg !7
  store i32* null, i32** %cube, align 8, !dbg !7
  %stack = alloca [96 x i32], align 4, !dbg !7
  %stack1 = getelementptr [96 x i32], [96 x i32]* %stack, i32 0, i32 0, !dbg !7
  store i32* %stack1, i32** %cube, align 8, !dbg !7
  store i32 0, i32* %x, align 4, !dbg !7
  store i32 0, i32* %x, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.end5, %entry
  %x2 = load i32, i32* %x, align 4, !dbg !7
  %LT = icmp slt i32 %x2, 2, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  store i32 0, i32* %y, align 4, !dbg !7
  store i32 0, i32* %y, align 4, !dbg !7
  br label %while.start3, !dbg !7

while.end:                                        ; preds = %while.start
  store i32 0, i32* %x, align 4, !dbg !8
  br label %while.start30, !dbg !8

while.start3:                                     ; preds = %while.end10, %while.then
  %y6 = load i32, i32* %y, align 4, !dbg !7
  %LT7 = icmp slt i32 %y6, 3, !dbg !7
  br i1 %LT7, label %while.then4, label %while.end5, !dbg !7

while.then4:                                      ; preds = %while.start3
  store i32 0, i32* %z, align 4, !dbg !7
  store i32 0, i32* %z, align 4, !dbg !7
  br label %while.start8, !dbg !7

while.end5:                                       ; preds = %while.start3
  %x28 = load i32, i32* %x, align 4, !dbg !8
  %ADD29 = add i32 %x28, 1, !dbg !8
  store i32 %ADD29, i32* %x, align 4, !dbg !8
  br label %while.start, !dbg !8

while.start8:                                     ; preds = %while.then9, %while.then4
  %z11 = load i32, i32* %z, align 4, !dbg !7
  %LT12 = icmp slt i32 %z11, 4, !dbg !7
  br i1 %LT12, label %while.then9, label %while.end10, !dbg !7

while.then9:                                      ; preds = %while.start8
  %cube13 = load i32*, i32** %cube, align 8, !dbg !8
  %x14 = load i32, i32* %x, align 4, !dbg !8
  %flat_idx = mul i32 %x14, 12, !dbg !8
  %row = getelementptr i32, i32* %cube13, i32 %flat_idx, !dbg !8
  %y15 = load i32, i32* %y, align 4, !dbg !8
  %flat_idx16 = mul i32 %y15, 4, !dbg !8
  %row17 = getelementptr i32, i32* %row, i32 %flat_idx16, !dbg !8
  %z18 = load i32, i32* %z, align 4, !dbg !8
  %ACCESS = getelementptr i32, i32* %row17, i32 %z18, !dbg !8
  %x19 = load i32, i32* %x, align 4, !dbg !8
  %MUL = mul i32 %x19, 12, !dbg !8
  %y20 = load i32, i32* %y, align 4, !dbg !8
  %MUL21 = mul i32 %y20, 4, !dbg !8
  %ADD = add i32 %MUL, %MUL21, !dbg !8
  %z22 = load i32, i32* %z, align 4, !dbg !8
  %ADD23 = add i32 %ADD, %z22, !dbg !8
  store i32 %ADD23, i32* %ACCESS, align 4, !dbg !8
  %z24 = load i32, i32* %z, align 4, !dbg !8
  %ADD25 = add i32 %z24, 1, !dbg !8
  store i32 %ADD25, i32* %z, align 4, !dbg !8
  br label %while.start8, !dbg !8

while.end10:                                      ; preds = %while.start8
  %y26 = load i32, i32* %y, align 4, !dbg !8
  %ADD27 = add i32 %y26, 1, !dbg !8
  store i32 %ADD27, i32* %y, align 4, !dbg !8
  br label %while.start3, !dbg !8

while.start30:                                    ; preds = %while.end38, %while.end
  %x33 = load i32, i32* %x, align 4, !dbg !8
  %LT34 = icmp slt i32 %x33, 2, !dbg !8
  br i1 %LT34, label %while.then31, label %while.end32, !dbg !8

while.then31:                                     ; preds = %while.start30
  store i32 0, i32* %y35, align 4, !dbg !7
  store i32 0, i32* %y35, align 4, !dbg !7
  br label %while.start36, !dbg !7

while.end32:                                      ; preds = %while.start30
  ret i32 0, !dbg !9

while.start36:                                    ; preds = %while.end44, %while.then31
  %y39 = load i32, i32* %y35, align 4, !dbg !7
  %LT40 = icmp slt i32 %y39, 3, !dbg !7
  br i1 %LT40, label %while.then37, label %while.end38, !dbg !7

while.then37:                                     ; preds = %while.start36
  store i32 0, i32* %z41, align 4, !dbg !7
  store i32 0, i32* %z41, align 4, !dbg !7
  br label %while.start42, !dbg !7

while.end38:                                      ; preds = %while.start36
  %x60 = load i32, i32* %x, align 4, !dbg !9
  %ADD61 = add i32 %x60, 1, !dbg !9
  store i32 %ADD61, i32* %x, align 4, !dbg !9
  br label %while.start30, !dbg !9

while.start42:                                    ; preds = %while.then43, %while.then37
  %z45 = load i32, i32* %z41, align 4, !dbg !7
  %LT46 = icmp slt i32 %z45, 4, !dbg !7
  br i1 %LT46, label %while.then43, label %while.end44, !dbg !7

while.then43:                                     ; preds = %while.start42
  %cube47 = load i32*, i32** %cube, align 8, !dbg !9
  %x48 = load i32, i32* %x, align 4, !dbg !9
  %flat_idx49 = mul i32 %x48, 12, !dbg !9
  %row50 = getelementptr i32, i32* %cube47, i32 %flat_idx49, !dbg !9
  %y51 = load i32, i32* %y35, align 4, !dbg !9
  %flat_idx52 = mul i32 %y51, 4, !dbg !9
  %row53 = getelementptr i32, i32* %row50, i32 %flat_idx52, !dbg !9
  %z54 = load i32, i32* %z41, align 4, !dbg !9
  %ACCESS55 = getelementptr i32, i32* %row53, i32 %z54, !dbg !9
  %ACC = load i32, i32* %ACCESS55, align 4, !dbg !9
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %ACC), !dbg !9
  %z56 = load i32, i32* %z41, align 4, !dbg !9
  %ADD57 = add i32 %z56, 1, !dbg !9
  store i32 %ADD57, i32* %z41, align 4, !dbg !9
  br label %while.start42, !dbg !9

while.end44:                                      ; preds = %while.start42
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !9
  %y58 = load i32, i32* %y35, align 4, !dbg !9
  %ADD59 = add i32 %y58, 1, !dbg !9
  store i32 %ADD59, i32* %y35, align 4, !dbg !9
  br label %while.start36, !dbg !9
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
!8 = !DILocation(line: 12, scope: !4)
!9 = !DILocation(line: 23, scope: !4)
