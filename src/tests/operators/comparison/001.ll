; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/comparison/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/comparison/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [20 x i8] c"hero_hp == min_hp: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [23 x i8] c"hero_hp == min_hp: %s\0A\00", align 1
@STR2 = private unnamed_addr constant [20 x i8] c"hero_hp != orc_hp: \00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [23 x i8] c"hero_hp != orc_hp: %s\0A\00", align 1
@STR4 = private unnamed_addr constant [20 x i8] c"orc_hp < hero_hp:  \00", align 1
@true_str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [23 x i8] c"orc_hp < hero_hp:  %s\0A\00", align 1
@STR6 = private unnamed_addr constant [20 x i8] c"orc_hp > hero_hp:  \00", align 1
@true_str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.9 = private unnamed_addr constant [23 x i8] c"orc_hp > hero_hp:  %s\0A\00", align 1
@STR8 = private unnamed_addr constant [20 x i8] c"orc_hp <= 60:      \00", align 1
@true_str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.12 = private unnamed_addr constant [23 x i8] c"orc_hp <= 60:      %s\0A\00", align 1
@STR10 = private unnamed_addr constant [20 x i8] c"hero_hp >= 100:    \00", align 1
@true_str.13 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.14 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.15 = private unnamed_addr constant [23 x i8] c"hero_hp >= 100:    %s\0A\00", align 1
@STR12 = private unnamed_addr constant [14 x i8] c"strong enemy\0A\00", align 1
@output_fmt.16 = private unnamed_addr constant [14 x i8] c"strong enemy\0A\00", align 1
@STR13 = private unnamed_addr constant [12 x i8] c"weak enemy\0A\00", align 1
@output_fmt.17 = private unnamed_addr constant [12 x i8] c"weak enemy\0A\00", align 1
@STR14 = private unnamed_addr constant [36 x i8] c"hero is at full health and winning\0A\00", align 1
@output_fmt.18 = private unnamed_addr constant [36 x i8] c"hero is at full health and winning\0A\00", align 1

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
  %bool_str = select i1 %EQ, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !9
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt, i32 0, i32 0), i8* %bool_str), !dbg !9
  %hero_hp3 = load i32, i32* %hero_hp, align 4, !dbg !9
  %orc_hp4 = load i32, i32* %orc_hp, align 4, !dbg !9
  %NEQ = icmp ne i32 %hero_hp3, %orc_hp4, !dbg !9
  %bool_str5 = select i1 %NEQ, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0), !dbg !9
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.3, i32 0, i32 0), i8* %bool_str5), !dbg !9
  %orc_hp6 = load i32, i32* %orc_hp, align 4, !dbg !9
  %hero_hp7 = load i32, i32* %hero_hp, align 4, !dbg !9
  %LT = icmp slt i32 %orc_hp6, %hero_hp7, !dbg !9
  %bool_str8 = select i1 %LT, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.4, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.5, i32 0, i32 0), !dbg !9
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.6, i32 0, i32 0), i8* %bool_str8), !dbg !9
  %orc_hp9 = load i32, i32* %orc_hp, align 4, !dbg !9
  %hero_hp10 = load i32, i32* %hero_hp, align 4, !dbg !9
  %GT = icmp sgt i32 %orc_hp9, %hero_hp10, !dbg !9
  %bool_str11 = select i1 %GT, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.8, i32 0, i32 0), !dbg !9
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.9, i32 0, i32 0), i8* %bool_str11), !dbg !9
  %orc_hp12 = load i32, i32* %orc_hp, align 4, !dbg !9
  %LE = icmp sle i32 %orc_hp12, 60, !dbg !9
  %bool_str13 = select i1 %LE, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.10, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.11, i32 0, i32 0), !dbg !9
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.12, i32 0, i32 0), i8* %bool_str13), !dbg !9
  %hero_hp14 = load i32, i32* %hero_hp, align 4, !dbg !9
  %GE = icmp sge i32 %hero_hp14, 100, !dbg !9
  %bool_str15 = select i1 %GE, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.13, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.14, i32 0, i32 0), !dbg !9
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.15, i32 0, i32 0), i8* %bool_str15), !dbg !9
  br label %if.start, !dbg !9

if.start:                                         ; preds = %entry
  %enemy_atk = call i32 @enemy_atk(), !dbg !10
  %GT16 = icmp sgt i32 %enemy_atk, 15, !dbg !10
  br i1 %GT16, label %if.then, label %if.else, !dbg !10

if.end:                                           ; preds = %if.else, %if.then
  br label %if.start17, !dbg !10

if.then:                                          ; preds = %if.start
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.16, i32 0, i32 0)), !dbg !10
  br label %if.end, !dbg !10

if.else:                                          ; preds = %if.start
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.17, i32 0, i32 0)), !dbg !10
  br label %if.end, !dbg !10

if.start17:                                       ; preds = %if.end
  %hero_hp20 = load i32, i32* %hero_hp, align 4, !dbg !10
  %EQ21 = icmp eq i32 %hero_hp20, 100, !dbg !10
  %orc_hp22 = load i32, i32* %orc_hp, align 4, !dbg !10
  %hero_hp23 = load i32, i32* %hero_hp, align 4, !dbg !10
  %LT24 = icmp slt i32 %orc_hp22, %hero_hp23, !dbg !10
  %AND = and i1 %EQ21, %LT24, !dbg !10
  br i1 %AND, label %if.then19, label %if.end18, !dbg !10

if.end18:                                         ; preds = %if.then19, %if.start17
  ret i32 0, !dbg !10

if.then19:                                        ; preds = %if.start17
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @output_fmt.18, i32 0, i32 0)), !dbg !10
  br label %if.end18, !dbg !10
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/comparison")
!4 = distinct !DISubprogram(name: "enemy_atk", linkageName: "enemy_atk", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 19, scope: !8)
