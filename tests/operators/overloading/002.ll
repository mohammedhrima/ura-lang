; ModuleID = 'tests/operators/002.ura'
source_filename = "tests/operators/002.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Score = type { i32 }

@STR0 = private unnamed_addr constant [9 x i8] c"a == b: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [12 x i8] c"a == b: %s\0A\00", align 1
@STR2 = private unnamed_addr constant [9 x i8] c"a != b: \00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [12 x i8] c"a != b: %s\0A\00", align 1
@STR4 = private unnamed_addr constant [9 x i8] c"a <  b: \00", align 1
@true_str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [12 x i8] c"a <  b: %s\0A\00", align 1
@STR6 = private unnamed_addr constant [9 x i8] c"a >  b: \00", align 1
@true_str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.9 = private unnamed_addr constant [12 x i8] c"a >  b: %s\0A\00", align 1
@STR8 = private unnamed_addr constant [9 x i8] c"a <= b: \00", align 1
@true_str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.12 = private unnamed_addr constant [12 x i8] c"a <= b: %s\0A\00", align 1
@STR10 = private unnamed_addr constant [9 x i8] c"a >= b: \00", align 1
@true_str.13 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.14 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.15 = private unnamed_addr constant [12 x i8] c"a >= b: %s\0A\00", align 1

define void @Score.clean(%struct.Score* %0) {
entry:
  ret void
}

define void @Score.init(%struct.Score* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Score*, align 8, !dbg !7
  store %struct.Score* %self, %struct.Score** %self1, align 8, !dbg !7
  %self2 = load %struct.Score*, %struct.Score** %self1, align 8, !dbg !7
  %self3 = load %struct.Score*, %struct.Score** %self1, align 8, !dbg !7
  %val = getelementptr %struct.Score, %struct.Score* %self3, i32 0, i32 0, !dbg !7
  store i32 0, i32* %val, align 4, !dbg !7
  ret void, !dbg !7
}

define void @Score.operator.ASSIGN.INT(i32 %n, %struct.Score* %self) !dbg !8 {
entry:
  %n1 = alloca i32, align 4, !dbg !9
  %self2 = alloca %struct.Score*, align 8, !dbg !9
  store i32 %n, i32* %n1, align 4, !dbg !9
  store %struct.Score* %self, %struct.Score** %self2, align 8, !dbg !9
  %self3 = load %struct.Score*, %struct.Score** %self2, align 8, !dbg !9
  %val = getelementptr %struct.Score, %struct.Score* %self3, i32 0, i32 0, !dbg !9
  %n4 = load i32, i32* %n1, align 4, !dbg !9
  store i32 %n4, i32* %val, align 4, !dbg !9
  ret void, !dbg !9
}

define i1 @Score.operator.EQ.Score(%struct.Score* %s, %struct.Score* %self) !dbg !10 {
entry:
  %s1 = alloca %struct.Score*, align 8, !dbg !11
  %self2 = alloca %struct.Score*, align 8, !dbg !11
  store %struct.Score* %s, %struct.Score** %s1, align 8, !dbg !11
  store %struct.Score* %self, %struct.Score** %self2, align 8, !dbg !11
  %self3 = load %struct.Score*, %struct.Score** %self2, align 8, !dbg !11
  %val = getelementptr %struct.Score, %struct.Score* %self3, i32 0, i32 0, !dbg !11
  %s4 = load %struct.Score*, %struct.Score** %s1, align 8, !dbg !11
  %val5 = getelementptr %struct.Score, %struct.Score* %s4, i32 0, i32 0, !dbg !11
  %DOT = load i32, i32* %val, align 4, !dbg !11
  %DOT6 = load i32, i32* %val5, align 4, !dbg !11
  %EQ = icmp eq i32 %DOT, %DOT6, !dbg !11
  ret i1 %EQ, !dbg !11
}

define i1 @Score.operator.NEQ.Score(%struct.Score* %s, %struct.Score* %self) !dbg !12 {
entry:
  %s1 = alloca %struct.Score*, align 8, !dbg !13
  %self2 = alloca %struct.Score*, align 8, !dbg !13
  store %struct.Score* %s, %struct.Score** %s1, align 8, !dbg !13
  store %struct.Score* %self, %struct.Score** %self2, align 8, !dbg !13
  %self3 = load %struct.Score*, %struct.Score** %self2, align 8, !dbg !13
  %val = getelementptr %struct.Score, %struct.Score* %self3, i32 0, i32 0, !dbg !13
  %s4 = load %struct.Score*, %struct.Score** %s1, align 8, !dbg !13
  %val5 = getelementptr %struct.Score, %struct.Score* %s4, i32 0, i32 0, !dbg !13
  %DOT = load i32, i32* %val, align 4, !dbg !13
  %DOT6 = load i32, i32* %val5, align 4, !dbg !13
  %NEQ = icmp ne i32 %DOT, %DOT6, !dbg !13
  ret i1 %NEQ, !dbg !13
}

define i1 @Score.operator.LT.Score(%struct.Score* %s, %struct.Score* %self) !dbg !14 {
entry:
  %s1 = alloca %struct.Score*, align 8, !dbg !15
  %self2 = alloca %struct.Score*, align 8, !dbg !15
  store %struct.Score* %s, %struct.Score** %s1, align 8, !dbg !15
  store %struct.Score* %self, %struct.Score** %self2, align 8, !dbg !15
  %self3 = load %struct.Score*, %struct.Score** %self2, align 8, !dbg !15
  %val = getelementptr %struct.Score, %struct.Score* %self3, i32 0, i32 0, !dbg !15
  %s4 = load %struct.Score*, %struct.Score** %s1, align 8, !dbg !15
  %val5 = getelementptr %struct.Score, %struct.Score* %s4, i32 0, i32 0, !dbg !15
  %DOT = load i32, i32* %val, align 4, !dbg !15
  %DOT6 = load i32, i32* %val5, align 4, !dbg !15
  %LT = icmp slt i32 %DOT, %DOT6, !dbg !15
  ret i1 %LT, !dbg !15
}

define i1 @Score.operator.GT.Score(%struct.Score* %s, %struct.Score* %self) !dbg !16 {
entry:
  %s1 = alloca %struct.Score*, align 8, !dbg !17
  %self2 = alloca %struct.Score*, align 8, !dbg !17
  store %struct.Score* %s, %struct.Score** %s1, align 8, !dbg !17
  store %struct.Score* %self, %struct.Score** %self2, align 8, !dbg !17
  %self3 = load %struct.Score*, %struct.Score** %self2, align 8, !dbg !17
  %val = getelementptr %struct.Score, %struct.Score* %self3, i32 0, i32 0, !dbg !17
  %s4 = load %struct.Score*, %struct.Score** %s1, align 8, !dbg !17
  %val5 = getelementptr %struct.Score, %struct.Score* %s4, i32 0, i32 0, !dbg !17
  %DOT = load i32, i32* %val, align 4, !dbg !17
  %DOT6 = load i32, i32* %val5, align 4, !dbg !17
  %GT = icmp sgt i32 %DOT, %DOT6, !dbg !17
  ret i1 %GT, !dbg !17
}

define i1 @Score.operator.LE.Score(%struct.Score* %s, %struct.Score* %self) !dbg !18 {
entry:
  %s1 = alloca %struct.Score*, align 8, !dbg !19
  %self2 = alloca %struct.Score*, align 8, !dbg !19
  store %struct.Score* %s, %struct.Score** %s1, align 8, !dbg !19
  store %struct.Score* %self, %struct.Score** %self2, align 8, !dbg !19
  %self3 = load %struct.Score*, %struct.Score** %self2, align 8, !dbg !19
  %val = getelementptr %struct.Score, %struct.Score* %self3, i32 0, i32 0, !dbg !19
  %s4 = load %struct.Score*, %struct.Score** %s1, align 8, !dbg !19
  %val5 = getelementptr %struct.Score, %struct.Score* %s4, i32 0, i32 0, !dbg !19
  %DOT = load i32, i32* %val, align 4, !dbg !19
  %DOT6 = load i32, i32* %val5, align 4, !dbg !19
  %LE = icmp sle i32 %DOT, %DOT6, !dbg !19
  ret i1 %LE, !dbg !19
}

define i1 @Score.operator.GE.Score(%struct.Score* %s, %struct.Score* %self) !dbg !20 {
entry:
  %s1 = alloca %struct.Score*, align 8, !dbg !21
  %self2 = alloca %struct.Score*, align 8, !dbg !21
  store %struct.Score* %s, %struct.Score** %s1, align 8, !dbg !21
  store %struct.Score* %self, %struct.Score** %self2, align 8, !dbg !21
  %self3 = load %struct.Score*, %struct.Score** %self2, align 8, !dbg !21
  %val = getelementptr %struct.Score, %struct.Score* %self3, i32 0, i32 0, !dbg !21
  %s4 = load %struct.Score*, %struct.Score** %s1, align 8, !dbg !21
  %val5 = getelementptr %struct.Score, %struct.Score* %s4, i32 0, i32 0, !dbg !21
  %DOT = load i32, i32* %val, align 4, !dbg !21
  %DOT6 = load i32, i32* %val5, align 4, !dbg !21
  %GE = icmp sge i32 %DOT, %DOT6, !dbg !21
  ret i1 %GE, !dbg !21
}

define i32 @main() !dbg !22 {
entry:
  %a = alloca %struct.Score, align 8, !dbg !23
  %b = alloca %struct.Score, align 8, !dbg !23
  store %struct.Score zeroinitializer, %struct.Score* %a, align 4, !dbg !23
  call void @Score.init(%struct.Score* %a), !dbg !23
  %val = getelementptr %struct.Score, %struct.Score* %a, i32 0, i32 0, !dbg !23
  store i32 50, i32* %val, align 4, !dbg !23
  store %struct.Score zeroinitializer, %struct.Score* %b, align 4, !dbg !23
  call void @Score.init(%struct.Score* %b), !dbg !23
  %val1 = getelementptr %struct.Score, %struct.Score* %b, i32 0, i32 0, !dbg !23
  store i32 80, i32* %val1, align 4, !dbg !23
  %0 = call i1 @Score.operator.EQ.Score(%struct.Score* %b, %struct.Score* %a), !dbg !23
  %bool_str = select i1 %0, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !23
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %bool_str), !dbg !23
  %2 = call i1 @Score.operator.NEQ.Score(%struct.Score* %b, %struct.Score* %a), !dbg !23
  %bool_str2 = select i1 %2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0), !dbg !23
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i8* %bool_str2), !dbg !23
  %4 = call i1 @Score.operator.LT.Score(%struct.Score* %b, %struct.Score* %a), !dbg !23
  %bool_str3 = select i1 %4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.4, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.5, i32 0, i32 0), !dbg !23
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.6, i32 0, i32 0), i32 1, i8* %bool_str3), !dbg !23
  %6 = call i1 @Score.operator.GT.Score(%struct.Score* %b, %struct.Score* %a), !dbg !23
  %bool_str4 = select i1 %6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.8, i32 0, i32 0), !dbg !23
  %7 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.9, i32 0, i32 0), i32 1, i8* %bool_str4), !dbg !23
  %8 = call i1 @Score.operator.LE.Score(%struct.Score* %b, %struct.Score* %a), !dbg !23
  %bool_str5 = select i1 %8, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.10, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.11, i32 0, i32 0), !dbg !23
  %9 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.12, i32 0, i32 0), i32 1, i8* %bool_str5), !dbg !23
  %10 = call i1 @Score.operator.GE.Score(%struct.Score* %b, %struct.Score* %a), !dbg !23
  %bool_str6 = select i1 %10, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.13, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.14, i32 0, i32 0), !dbg !23
  %11 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.15, i32 0, i32 0), i32 1, i8* %bool_str6), !dbg !23
  call void @Score.clean(%struct.Score* %a), !dbg !23
  call void @Score.clean(%struct.Score* %b), !dbg !23
  ret i32 0, !dbg !23
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "tests/operators")
!4 = distinct !DISubprogram(name: "Score.init", linkageName: "Score.init", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = distinct !DISubprogram(name: "Score.operator.ASSIGN.INT", linkageName: "Score.operator.ASSIGN.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(name: "Score.operator.EQ.Score", linkageName: "Score.operator.EQ.Score", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 0, scope: !10)
!12 = distinct !DISubprogram(name: "Score.operator.NEQ.Score", linkageName: "Score.operator.NEQ.Score", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 0, scope: !12)
!14 = distinct !DISubprogram(name: "Score.operator.LT.Score", linkageName: "Score.operator.LT.Score", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 0, scope: !14)
!16 = distinct !DISubprogram(name: "Score.operator.GT.Score", linkageName: "Score.operator.GT.Score", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 0, scope: !16)
!18 = distinct !DISubprogram(name: "Score.operator.LE.Score", linkageName: "Score.operator.LE.Score", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 0, scope: !18)
!20 = distinct !DISubprogram(name: "Score.operator.GE.Score", linkageName: "Score.operator.GE.Score", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 0, scope: !20)
!22 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 30, type: !5, scopeLine: 30, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!23 = !DILocation(line: 30, scope: !22)
