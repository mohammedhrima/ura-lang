; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor02.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor02.ura"
target triple = "arm64-apple-macosx16.0.0"

@score = global i32 0
@floor = global i32 1
@lives = global i32 3
@STR0 = private unnamed_addr constant [7 x i8] c"Floor \00", align 1
@STR1 = private unnamed_addr constant [11 x i8] c" | Score: \00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c" | Lives: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [34 x i8] c"Floor %d | Score: %d | Lives: %d\0A\00", align 1
@STR4 = private unnamed_addr constant [28 x i8] c"After two kills \E2\80\94 Score: \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [31 x i8] c"After two kills \E2\80\94 Score: %d\0A\00", align 1

define void @earn(i32 %pts) !dbg !4 {
entry:
  %pts1 = alloca i32, align 4, !dbg !7
  store i32 %pts, i32* %pts1, align 4, !dbg !7
  %score = load i32, i32* @score, align 4, !dbg !7
  %pts2 = load i32, i32* %pts1, align 4, !dbg !7
  %ADD = add i32 %score, %pts2, !dbg !7
  store i32 %ADD, i32* @score, align 4, !dbg !7
  ret void, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %floor = load i32, i32* @floor, align 4, !dbg !9
  %score = load i32, i32* @score, align 4, !dbg !9
  %lives = load i32, i32* @lives, align 4, !dbg !9
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @output_fmt, i32 0, i32 0), i32 %floor, i32 %score, i32 %lives), !dbg !9
  call void @earn(i32 100), !dbg !10
  call void @earn(i32 50), !dbg !11
  %score1 = load i32, i32* @score, align 4, !dbg !11
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @output_fmt.1, i32 0, i32 0), i32 %score1), !dbg !11
  ret i32 0, !dbg !11
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor02.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon")
!4 = distinct !DISubprogram(name: "earn", linkageName: "earn", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 9, scope: !8)
!10 = !DILocation(line: 11, scope: !8)
!11 = !DILocation(line: 12, scope: !8)
