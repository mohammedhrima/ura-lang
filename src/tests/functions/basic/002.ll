; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic/002.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [16 x i8] c"Floor 3 bonus: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [19 x i8] c"Floor 3 bonus: %d\0A\00", align 1
@STR2 = private unnamed_addr constant [16 x i8] c"Hero alive:    \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [19 x i8] c"Hero alive:    %s\0A\00", align 1
@STR4 = private unnamed_addr constant [16 x i8] c"Boss floor:    \00", align 1
@true_str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [19 x i8] c"Boss floor:    %s\0A\00", align 1

define i32 @victory_bonus(i32 %floor) !dbg !4 {
entry:
  %floor1 = alloca i32, align 4, !dbg !7
  store i32 %floor, i32* %floor1, align 4, !dbg !7
  %floor2 = load i32, i32* %floor1, align 4, !dbg !7
  %MUL = mul i32 %floor2, 50, !dbg !7
  ret i32 %MUL, !dbg !7
}

define i1 @is_alive(i32 %hp) !dbg !8 {
entry:
  %hp1 = alloca i32, align 4, !dbg !9
  store i32 %hp, i32* %hp1, align 4, !dbg !9
  %hp2 = load i32, i32* %hp1, align 4, !dbg !9
  %GT = icmp sgt i32 %hp2, 0, !dbg !9
  ret i1 %GT, !dbg !9
}

define i1 @is_boss_floor(i32 %floor) !dbg !10 {
entry:
  %floor1 = alloca i32, align 4, !dbg !11
  store i32 %floor, i32* %floor1, align 4, !dbg !11
  %floor2 = load i32, i32* %floor1, align 4, !dbg !11
  %EQ = icmp eq i32 %floor2, 10, !dbg !11
  ret i1 %EQ, !dbg !11
}

define i32 @main() !dbg !12 {
entry:
  %bonus = alloca i32, align 4, !dbg !13
  store i32 0, i32* %bonus, align 4, !dbg !13
  %victory_bonus = call i32 @victory_bonus(i32 3), !dbg !14
  store i32 %victory_bonus, i32* %bonus, align 4, !dbg !14
  %bonus1 = load i32, i32* %bonus, align 4, !dbg !14
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %bonus1), !dbg !14
  %is_alive = call i1 @is_alive(i32 80), !dbg !15
  %bool_str = select i1 %is_alive, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !15
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %bool_str), !dbg !15
  %is_boss_floor = call i1 @is_boss_floor(i32 10), !dbg !16
  %bool_str2 = select i1 %is_boss_floor, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.2, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.3, i32 0, i32 0), !dbg !16
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt.4, i32 0, i32 0), i32 1, i8* %bool_str2), !dbg !16
  ret i32 0, !dbg !16
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic")
!4 = distinct !DISubprogram(name: "victory_bonus", linkageName: "victory_bonus", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "is_alive", linkageName: "is_alive", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = distinct !DISubprogram(name: "is_boss_floor", linkageName: "is_boss_floor", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 7, scope: !10)
!12 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 9, scope: !12)
!14 = !DILocation(line: 10, scope: !12)
!15 = !DILocation(line: 12, scope: !12)
!16 = !DILocation(line: 13, scope: !12)
