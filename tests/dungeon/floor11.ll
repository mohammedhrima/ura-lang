; ModuleID = 'tests/dungeon/floor11.ura'
source_filename = "tests/dungeon/floor11.ura"
target triple = "arm64-apple-macosx16.0.0"

@POISONED = global i32 1
@FROZEN = global i32 2
@BURNING = global i32 4
@STR0 = private unnamed_addr constant [11 x i8] c"Poisoned: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [14 x i8] c"Poisoned: %s\0A\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"Frozen:   \00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [14 x i8] c"Frozen:   %s\0A\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"Burning:  \00", align 1
@true_str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [14 x i8] c"Burning:  %s\0A\00", align 1
@STR6 = private unnamed_addr constant [11 x i8] c"Cured:    \00", align 1
@true_str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.9 = private unnamed_addr constant [14 x i8] c"Cured:    %s\0A\00", align 1

define i1 @has(i32 %status, i32 %flag) !dbg !4 {
entry:
  %status1 = alloca i32, align 4, !dbg !7
  %flag2 = alloca i32, align 4, !dbg !7
  store i32 %status, i32* %status1, align 4, !dbg !7
  store i32 %flag, i32* %flag2, align 4, !dbg !7
  %status3 = load i32, i32* %status1, align 4, !dbg !7
  %flag4 = load i32, i32* %flag2, align 4, !dbg !7
  %BAND = and i32 %status3, %flag4, !dbg !7
  %NEQ = icmp ne i32 %BAND, 0, !dbg !7
  ret i1 %NEQ, !dbg !7
}

define i32 @add(i32 %status, i32 %flag) !dbg !8 {
entry:
  %status1 = alloca i32, align 4, !dbg !9
  %flag2 = alloca i32, align 4, !dbg !9
  store i32 %status, i32* %status1, align 4, !dbg !9
  store i32 %flag, i32* %flag2, align 4, !dbg !9
  %status3 = load i32, i32* %status1, align 4, !dbg !9
  %flag4 = load i32, i32* %flag2, align 4, !dbg !9
  %BOR = or i32 %status3, %flag4, !dbg !9
  ret i32 %BOR, !dbg !9
}

define i32 @rem(i32 %status, i32 %flag) !dbg !10 {
entry:
  %status1 = alloca i32, align 4, !dbg !11
  %flag2 = alloca i32, align 4, !dbg !11
  store i32 %status, i32* %status1, align 4, !dbg !11
  store i32 %flag, i32* %flag2, align 4, !dbg !11
  %flag3 = load i32, i32* %flag2, align 4, !dbg !11
  %BNOT = xor i32 %flag3, -1, !dbg !11
  %status4 = load i32, i32* %status1, align 4, !dbg !11
  %BAND = and i32 %status4, %BNOT, !dbg !11
  ret i32 %BAND, !dbg !11
}

define i32 @main() !dbg !12 {
entry:
  %s = alloca i32, align 4, !dbg !13
  store i32 0, i32* %s, align 4, !dbg !13
  store i32 0, i32* %s, align 4, !dbg !13
  %s1 = load i32, i32* %s, align 4, !dbg !14
  %POISONED = load i32, i32* @POISONED, align 4, !dbg !14
  %add = call i32 @add(i32 %s1, i32 %POISONED), !dbg !14
  store i32 %add, i32* %s, align 4, !dbg !14
  %s2 = load i32, i32* %s, align 4, !dbg !15
  %BURNING = load i32, i32* @BURNING, align 4, !dbg !15
  %add3 = call i32 @add(i32 %s2, i32 %BURNING), !dbg !15
  store i32 %add3, i32* %s, align 4, !dbg !15
  %s4 = load i32, i32* %s, align 4, !dbg !16
  %POISONED5 = load i32, i32* @POISONED, align 4, !dbg !16
  %has = call i1 @has(i32 %s4, i32 %POISONED5), !dbg !16
  %bool_str = select i1 %has, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !16
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %bool_str), !dbg !16
  %s6 = load i32, i32* %s, align 4, !dbg !17
  %FROZEN = load i32, i32* @FROZEN, align 4, !dbg !17
  %has7 = call i1 @has(i32 %s6, i32 %FROZEN), !dbg !17
  %bool_str8 = select i1 %has7, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0), !dbg !17
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i8* %bool_str8), !dbg !17
  %s9 = load i32, i32* %s, align 4, !dbg !18
  %BURNING10 = load i32, i32* @BURNING, align 4, !dbg !18
  %has11 = call i1 @has(i32 %s9, i32 %BURNING10), !dbg !18
  %bool_str12 = select i1 %has11, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.4, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.5, i32 0, i32 0), !dbg !18
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.6, i32 0, i32 0), i32 1, i8* %bool_str12), !dbg !18
  %s13 = load i32, i32* %s, align 4, !dbg !19
  %POISONED14 = load i32, i32* @POISONED, align 4, !dbg !19
  %rem = call i32 @rem(i32 %s13, i32 %POISONED14), !dbg !19
  store i32 %rem, i32* %s, align 4, !dbg !19
  %s15 = load i32, i32* %s, align 4, !dbg !20
  %POISONED16 = load i32, i32* @POISONED, align 4, !dbg !20
  %has17 = call i1 @has(i32 %s15, i32 %POISONED16), !dbg !20
  %bool_str18 = select i1 %has17, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.8, i32 0, i32 0), !dbg !20
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.9, i32 0, i32 0), i32 1, i8* %bool_str18), !dbg !20
  ret i32 0, !dbg !20
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor11.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "has", linkageName: "has", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 7, scope: !4)
!8 = distinct !DISubprogram(name: "add", linkageName: "add", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 8, scope: !8)
!10 = distinct !DISubprogram(name: "rem", linkageName: "rem", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 9, scope: !10)
!12 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 11, scope: !12)
!14 = !DILocation(line: 13, scope: !12)
!15 = !DILocation(line: 14, scope: !12)
!16 = !DILocation(line: 15, scope: !12)
!17 = !DILocation(line: 16, scope: !12)
!18 = !DILocation(line: 17, scope: !12)
!19 = !DILocation(line: 18, scope: !12)
!20 = !DILocation(line: 19, scope: !12)
