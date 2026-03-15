; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs/002.ura"
target triple = "arm64-apple-macosx16.0.0"

declare i32 @write(i32, i8*, i32)

define i32 @putchar(i8 %c) !dbg !4 {
entry:
  %c1 = alloca i8, align 1, !dbg !7
  %buf = alloca i8*, align 8, !dbg !7
  store i8 %c, i8* %c1, align 1, !dbg !7
  store i8* null, i8** %buf, align 8, !dbg !7
  %stack = alloca [2 x i8], align 1, !dbg !7
  %stack2 = getelementptr [2 x i8], [2 x i8]* %stack, i32 0, i32 0, !dbg !7
  store i8* %stack2, i8** %buf, align 8, !dbg !7
  %buf3 = load i8*, i8** %buf, align 8, !dbg !8
  %ACCESS = getelementptr i8, i8* %buf3, i32 0, !dbg !8
  %c4 = load i8, i8* %c1, align 1, !dbg !8
  store i8 %c4, i8* %ACCESS, align 1, !dbg !8
  %buf5 = load i8*, i8** %buf, align 8, !dbg !9
  %write = call i32 @write(i32 1, i8* %buf5, i32 1), !dbg !9
  ret i32 0, !dbg !9
}

define i32 @main() !dbg !10 {
entry:
  %putchar = call i32 @putchar(i8 68), !dbg !11
  %putchar1 = call i32 @putchar(i8 85), !dbg !12
  %putchar2 = call i32 @putchar(i8 78), !dbg !13
  %putchar3 = call i32 @putchar(i8 71), !dbg !14
  %putchar4 = call i32 @putchar(i8 69), !dbg !15
  %putchar5 = call i32 @putchar(i8 79), !dbg !16
  %putchar6 = call i32 @putchar(i8 78), !dbg !17
  %putchar7 = call i32 @putchar(i8 10), !dbg !18
  ret i32 0, !dbg !18
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs")
!4 = distinct !DISubprogram(name: "putchar", linkageName: "putchar", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
!9 = !DILocation(line: 8, scope: !4)
!10 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 12, scope: !10)
!12 = !DILocation(line: 13, scope: !10)
!13 = !DILocation(line: 14, scope: !10)
!14 = !DILocation(line: 15, scope: !10)
!15 = !DILocation(line: 16, scope: !10)
!16 = !DILocation(line: 17, scope: !10)
!17 = !DILocation(line: 18, scope: !10)
!18 = !DILocation(line: 19, scope: !10)
