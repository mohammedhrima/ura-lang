; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/002.ura"
target triple = "arm64-apple-macosx16.0.0"

@floor = global i32 1
@lives = global i32 3
@max_floor = global i32 10
@hardcore = global i1 true
@STR0 = private unnamed_addr constant [12 x i8] c"Floor:     \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [15 x i8] c"Floor:     %d\0A\00", align 1
@STR2 = private unnamed_addr constant [12 x i8] c"Lives:     \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [15 x i8] c"Lives:     %d\0A\00", align 1
@STR4 = private unnamed_addr constant [12 x i8] c"Max floor: \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [15 x i8] c"Max floor: %d\0A\00", align 1
@STR6 = private unnamed_addr constant [12 x i8] c"Hardcore:  \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [15 x i8] c"Hardcore:  %s\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %floor = load i32, i32* @floor, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %floor), !dbg !7
  %lives = load i32, i32* @lives, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %lives), !dbg !7
  %max_floor = load i32, i32* @max_floor, align 4, !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %max_floor), !dbg !7
  %hardcore = load i1, i1* @hardcore, align 1, !dbg !7
  %bool_str = select i1 %hardcore, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !7
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i8* %bool_str), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
