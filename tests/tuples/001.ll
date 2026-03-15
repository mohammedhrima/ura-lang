; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/tuples/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/tuples/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@STR2 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [9 x i8] c"<%d %d>\0A\00", align 1

define { i32, i32 } @swap(i32 %a, i32 %b) !dbg !4 {
entry:
  %a1 = alloca i32, align 4, !dbg !7
  %b2 = alloca i32, align 4, !dbg !7
  store i32 %a, i32* %a1, align 4, !dbg !7
  store i32 %b, i32* %b2, align 4, !dbg !7
  %b3 = load i32, i32* %b2, align 4, !dbg !7
  %0 = insertvalue { i32, i32 } undef, i32 %b3, 0, !dbg !7
  %a4 = load i32, i32* %a1, align 4, !dbg !7
  %1 = insertvalue { i32, i32 } %0, i32 %a4, 1, !dbg !7
  ret { i32, i32 } %1, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %x = alloca i32, align 4, !dbg !9
  %y = alloca i32, align 4, !dbg !9
  %swap = call { i32, i32 } @swap(i32 3, i32 7), !dbg !9
  store i32 0, i32* %x, align 4, !dbg !9
  %x1 = extractvalue { i32, i32 } %swap, 0, !dbg !9
  store i32 %x1, i32* %x, align 4, !dbg !9
  store i32 0, i32* %y, align 4, !dbg !9
  %y2 = extractvalue { i32, i32 } %swap, 1, !dbg !9
  store i32 %y2, i32* %y, align 4, !dbg !9
  %x3 = load i32, i32* %x, align 4, !dbg !9
  %y4 = load i32, i32* %y, align 4, !dbg !9
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @output_fmt, i32 0, i32 0), i32 2, i32 %x3, i32 %y4), !dbg !9
  ret i32 0, !dbg !9
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/tuples")
!4 = distinct !DISubprogram(name: "swap", linkageName: "swap", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 7, scope: !8)
