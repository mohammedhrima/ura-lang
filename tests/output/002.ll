; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/output/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/output/002.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [15 x i8] c"Damage dealt: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [18 x i8] c"Damage dealt: %d\0A\00", align 1
@STR2 = private unnamed_addr constant [15 x i8] c"XP gained:    \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [18 x i8] c"XP gained:    %d\0A\00", align 1
@STR4 = private unnamed_addr constant [15 x i8] c"Gold split:   \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [18 x i8] c"Gold split:   %d\0A\00", align 1
@STR6 = private unnamed_addr constant [15 x i8] c"Bonus:        \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [18 x i8] c"Bonus:        %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 17), !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 150), !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 30), !dbg !7
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i32 37), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/output")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
