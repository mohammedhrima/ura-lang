; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/comparison/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/comparison/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [20 x i8] c"hero_hp == min_hp: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [23 x i8] c"hero_hp == min_hp: %d\0A\00", align 1
@STR2 = private unnamed_addr constant [20 x i8] c"hero_hp != orc_hp: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [23 x i8] c"hero_hp != orc_hp: %d\0A\00", align 1
@STR4 = private unnamed_addr constant [20 x i8] c"orc_hp < hero_hp:  \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [23 x i8] c"orc_hp < hero_hp:  %d\0A\00", align 1
@STR6 = private unnamed_addr constant [20 x i8] c"orc_hp > hero_hp:  \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [23 x i8] c"orc_hp > hero_hp:  %d\0A\00", align 1
@STR8 = private unnamed_addr constant [20 x i8] c"orc_hp <= 60:      \00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [23 x i8] c"orc_hp <= 60:      %d\0A\00", align 1
@STR10 = private unnamed_addr constant [20 x i8] c"hero_hp >= 100:    \00", align 1
@STR11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [23 x i8] c"hero_hp >= 100:    %d\0A\00", align 1
@STR12 = private unnamed_addr constant [14 x i8] c"strong enemy\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [14 x i8] c"strong enemy\0A\00", align 1
@STR13 = private unnamed_addr constant [12 x i8] c"weak enemy\0A\00", align 1
@output_fmt.7 = private unnamed_addr constant [12 x i8] c"weak enemy\0A\00", align 1
@STR14 = private unnamed_addr constant [36 x i8] c"hero is at full health and winning\0A\00", align 1
@output_fmt.8 = private unnamed_addr constant [36 x i8] c"hero is at full health and winning\0A\00", align 1

define i32 @enemy_atk() !dbg !4 {
entry:
  ret i32 18, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %hero_hp = alloca i32, align 4, !dbg !9
  %orc_hp = alloca i32, align 4, !dbg !9
  %min_hp = alloca i32, align 4, !dbg !9
  store i32 0, i32* %hero_hp, align 4, !dbg !9
  store i32 100, i32* %hero_hp, align 4, !dbg !9
  store i32 0, i32* %orc_hp, align 4, !dbg !9
  store i32 60, i32* %orc_hp, align 4, !dbg !9
  store i32 0, i32* %min_hp, align 4, !dbg !9
  store i32 100, i32* %min_hp, align 4, !dbg !9
  %hero_hp1 = load i32, i32* %hero_hp, align 4, !dbg !9
  %min_hp2 = load i32, i32* %min_hp, align 4, !dbg !9
  %EQ = icmp eq i32 %hero_hp1, %min_hp2, !dbg !9
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt, i32 0, i32 0), i32 1, i1 %EQ), !dbg !9
  %hero_hp3 = load i32, i32* %hero_hp, align 4, !dbg !9
  %orc_hp4 = load i32, i32* %orc_hp, align 4, !dbg !9
  %NEQ = icmp ne i32 %hero_hp3, %orc_hp4, !dbg !9
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i1 %NEQ), !dbg !9
  %orc_hp5 = load i32, i32* %orc_hp, align 4, !dbg !9
  %hero_hp6 = load i32, i32* %hero_hp, align 4, !dbg !9
  %LT = icmp slt i32 %orc_hp5, %hero_hp6, !dbg !9
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i1 %LT), !dbg !9
  %orc_hp7 = load i32, i32* %orc_hp, align 4, !dbg !9
  %hero_hp8 = load i32, i32* %hero_hp, align 4, !dbg !9
  %GT = icmp sgt i32 %orc_hp7, %hero_hp8, !dbg !9
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i1 %GT), !dbg !9
  %orc_hp9 = load i32, i32* %orc_hp, align 4, !dbg !9
  %LE = icmp sle i32 %orc_hp9, 60, !dbg !9
  %4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.4, i32 0, i32 0), i32 1, i1 %LE), !dbg !9
  %hero_hp10 = load i32, i32* %hero_hp, align 4, !dbg !9
  %GE = icmp sge i32 %hero_hp10, 100, !dbg !9
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.5, i32 0, i32 0), i32 1, i1 %GE), !dbg !9
  br label %if.start, !dbg !9

if.start:                                         ; preds = %entry
  %enemy_atk = call i32 @enemy_atk(), !dbg !10
  %GT11 = icmp sgt i32 %enemy_atk, 15, !dbg !10
  br i1 %GT11, label %if.then, label %if.else, !dbg !10

if.end:                                           ; preds = %if.else, %if.then
  br label %if.start12, !dbg !10

if.then:                                          ; preds = %if.start
  %6 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.6, i32 0, i32 0), i32 0), !dbg !10
  br label %if.end, !dbg !10

if.else:                                          ; preds = %if.start
  %7 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.7, i32 0, i32 0), i32 0), !dbg !10
  br label %if.end, !dbg !10

if.start12:                                       ; preds = %if.end
  %hero_hp15 = load i32, i32* %hero_hp, align 4, !dbg !10
  %EQ16 = icmp eq i32 %hero_hp15, 100, !dbg !10
  %orc_hp17 = load i32, i32* %orc_hp, align 4, !dbg !10
  %hero_hp18 = load i32, i32* %hero_hp, align 4, !dbg !10
  %LT19 = icmp slt i32 %orc_hp17, %hero_hp18, !dbg !10
  %AND = and i1 %EQ16, %LT19, !dbg !10
  br i1 %AND, label %if.then14, label %if.end13, !dbg !10

if.end13:                                         ; preds = %if.then14, %if.start12
  ret i32 0, !dbg !10

if.then14:                                        ; preds = %if.start12
  %8 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @output_fmt.8, i32 0, i32 0), i32 0), !dbg !10
  br label %if.end13, !dbg !10
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/comparison")
!4 = distinct !DISubprogram(name: "enemy_atk", linkageName: "enemy_atk", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 19, scope: !8)
