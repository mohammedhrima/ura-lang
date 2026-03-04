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
  %r1 = alloca i32*, align 8, !dbg !9
  %r2 = alloca i32*, align 8, !dbg !9
  %r3 = alloca i32*, align 8, !dbg !9
  %result = alloca i32, align 4, !dbg !9
  store i32 10, i32* %val1, align 4, !dbg !9
  store i32 20, i32* %val2, align 4, !dbg !9
  store i32 30, i32* %val3, align 4, !dbg !9
  store i32* %val1, i32** %r1, align 8, !dbg !9
  %ref_ptr = load i32*, i32** %r1, align 8, !dbg !9
  store i32 15, i32* %ref_ptr, align 4, !dbg !9
  %val11 = load i32, i32* %val1, align 4, !dbg !10
  call void @print(i32 %val11), !dbg !10
  %ref_ptr2 = load i32*, i32** %r1, align 8, !dbg !11
  %ref_val = load i32, i32* %ref_ptr2, align 4, !dbg !11
  call void @print(i32 %ref_val), !dbg !11
  store i32* %val2, i32** %r2, align 8, !dbg !9
  %ref_ptr3 = load i32*, i32** %r2, align 8, !dbg !9
  %current = load i32, i32* %ref_ptr3, align 4, !dbg !9
  %MUL = mul i32 %current, 2, !dbg !9
  %ref_ptr4 = load i32*, i32** %r2, align 8, !dbg !9
  store i32 %MUL, i32* %ref_ptr4, align 4, !dbg !9
  %val25 = load i32, i32* %val2, align 4, !dbg !12
  call void @print(i32 %val25), !dbg !12
  %ref_ptr6 = load i32*, i32** %r2, align 8, !dbg !13
  %ref_val7 = load i32, i32* %ref_ptr6, align 4, !dbg !13
  call void @print(i32 %ref_val7), !dbg !13
  store i32* %val3, i32** %r3, align 8, !dbg !9
  %ref_ptr8 = load i32*, i32** %r3, align 8, !dbg !9
  %current9 = load i32, i32* %ref_ptr8, align 4, !dbg !9
  %SUB = sub i32 %current9, 5, !dbg !9
  %ref_ptr10 = load i32*, i32** %r3, align 8, !dbg !9
  store i32 %SUB, i32* %ref_ptr10, align 4, !dbg !9
  %val311 = load i32, i32* %val3, align 4, !dbg !14
  call void @print(i32 %val311), !dbg !14
  %ref_ptr12 = load i32*, i32** %r3, align 8, !dbg !15
  %ref_val13 = load i32, i32* %ref_ptr12, align 4, !dbg !15
  call void @print(i32 %ref_val13), !dbg !15
  %ref_ptr14 = load i32*, i32** %r1, align 8, !dbg !9
  %ref_val15 = load i32, i32* %ref_ptr14, align 4, !dbg !9
  %ref_ptr16 = load i32*, i32** %r2, align 8, !dbg !9
  %ref_val17 = load i32, i32* %ref_ptr16, align 4, !dbg !9
  %ADD = add i32 %ref_val15, %ref_val17, !dbg !9
  %ref_ptr18 = load i32*, i32** %r3, align 8, !dbg !9
  %ref_val19 = load i32, i32* %ref_ptr18, align 4, !dbg !9
  %ADD20 = add i32 %ADD, %ref_val19, !dbg !9
  store i32 %ADD20, i32* %result, align 4, !dbg !9
  %result21 = load i32, i32* %result, align 4, !dbg !16
  call void @print(i32 %result21), !dbg !16
  ret i32 0, !dbg !16
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
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 13, scope: !8)
!11 = !DILocation(line: 14, scope: !8)
!12 = !DILocation(line: 18, scope: !8)
!13 = !DILocation(line: 19, scope: !8)
!14 = !DILocation(line: 23, scope: !8)
!15 = !DILocation(line: 24, scope: !8)
!16 = !DILocation(line: 27, scope: !8)
