; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define void @print(i32 %num) !dbg !4 {
entry:
  %num1 = alloca i32, align 4, !dbg !7
  store i32 %num, i32* %num1, align 4, !dbg !7
  %num2 = load i32, i32* %num1, align 4, !dbg !7
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @STR0, i32 0, i32 0), i32 1, i32 %num2), !dbg !7
  ret void, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %val1 = alloca i32, align 4, !dbg !9
  %val2 = alloca i32, align 4, !dbg !9
  %val3 = alloca i32, align 4, !dbg !9
  %current = alloca i32*, align 8, !dbg !9
  %sum = alloca i32, align 4, !dbg !9
  store i32 10, i32* %val1, align 4, !dbg !9
  store i32 20, i32* %val2, align 4, !dbg !9
  store i32 30, i32* %val3, align 4, !dbg !9
  store i32* %val1, i32** %current, align 8, !dbg !9
  %ref_ptr = load i32*, i32** %current, align 8, !dbg !9
  store i32 15, i32* %ref_ptr, align 4, !dbg !9
  %val11 = load i32, i32* %val1, align 4, !dbg !10
  call void @print(i32 %val11), !dbg !10
  %ref_ptr2 = load i32*, i32** %current, align 8, !dbg !11
  %ref_val = load i32, i32* %ref_ptr2, align 4, !dbg !11
  call void @print(i32 %ref_val), !dbg !11
  %val23 = load i32, i32* %val2, align 4, !dbg !11
  %ref_ptr4 = load i32*, i32** %current, align 8, !dbg !11
  store i32 %val23, i32* %ref_ptr4, align 4, !dbg !11
  %val15 = load i32, i32* %val1, align 4, !dbg !12
  call void @print(i32 %val15), !dbg !12
  %ref_ptr6 = load i32*, i32** %current, align 8, !dbg !13
  %ref_val7 = load i32, i32* %ref_ptr6, align 4, !dbg !13
  call void @print(i32 %ref_val7), !dbg !13
  %ref_ptr8 = load i32*, i32** %current, align 8, !dbg !13
  store i32 25, i32* %ref_ptr8, align 4, !dbg !13
  %val19 = load i32, i32* %val1, align 4, !dbg !14
  call void @print(i32 %val19), !dbg !14
  %ref_ptr10 = load i32*, i32** %current, align 8, !dbg !15
  %ref_val11 = load i32, i32* %ref_ptr10, align 4, !dbg !15
  call void @print(i32 %ref_val11), !dbg !15
  %val312 = load i32, i32* %val3, align 4, !dbg !15
  %ref_ptr13 = load i32*, i32** %current, align 8, !dbg !15
  store i32 %val312, i32* %ref_ptr13, align 4, !dbg !15
  %ref_ptr14 = load i32*, i32** %current, align 8, !dbg !15
  %current15 = load i32, i32* %ref_ptr14, align 4, !dbg !15
  %ADD = add i32 %current15, 5, !dbg !15
  %ref_ptr16 = load i32*, i32** %current, align 8, !dbg !15
  store i32 %ADD, i32* %ref_ptr16, align 4, !dbg !15
  %val117 = load i32, i32* %val1, align 4, !dbg !16
  call void @print(i32 %val117), !dbg !16
  %ref_ptr18 = load i32*, i32** %current, align 8, !dbg !17
  %ref_val19 = load i32, i32* %ref_ptr18, align 4, !dbg !17
  call void @print(i32 %ref_val19), !dbg !17
  %val120 = load i32, i32* %val1, align 4, !dbg !9
  %val221 = load i32, i32* %val2, align 4, !dbg !9
  %ADD22 = add i32 %val120, %val221, !dbg !9
  %val323 = load i32, i32* %val3, align 4, !dbg !9
  %ADD24 = add i32 %ADD22, %val323, !dbg !9
  store i32 %ADD24, i32* %sum, align 4, !dbg !9
  %sum25 = load i32, i32* %sum, align 4, !dbg !18
  call void @print(i32 %sum25), !dbg !18
  ret i32 0, !dbg !18
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "print", linkageName: "print", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 7, scope: !8)
!10 = !DILocation(line: 14, scope: !8)
!11 = !DILocation(line: 15, scope: !8)
!12 = !DILocation(line: 18, scope: !8)
!13 = !DILocation(line: 19, scope: !8)
!14 = !DILocation(line: 21, scope: !8)
!15 = !DILocation(line: 22, scope: !8)
!16 = !DILocation(line: 26, scope: !8)
!17 = !DILocation(line: 27, scope: !8)
!18 = !DILocation(line: 30, scope: !8)
