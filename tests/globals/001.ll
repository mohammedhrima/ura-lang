; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@count = global i32 0
@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define void @increment() !dbg !4 {
entry:
  %current = load i32, i32* @count, align 4, !dbg !7
  %ADD = add i32 %current, 1, !dbg !7
  store i32 %ADD, i32* @count, align 4, !dbg !7
  ret void, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  call void @increment(), !dbg !9
  call void @increment(), !dbg !10
  %count = load i32, i32* @count, align 4, !dbg !10
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %count), !dbg !10
  ret i32 0, !dbg !10
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals")
!4 = distinct !DISubprogram(name: "increment", linkageName: "increment", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 9, scope: !8)
!10 = !DILocation(line: 10, scope: !8)
