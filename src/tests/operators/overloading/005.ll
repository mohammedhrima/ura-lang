; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading/005.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading/005.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.HP = type { i32 }

@STR0 = private unnamed_addr constant [15 x i8] c"hero + bonus: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [18 x i8] c"hero + bonus: %d\0A\00", align 1
@STR2 = private unnamed_addr constant [14 x i8] c"hero + 25:   \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [17 x i8] c"hero + 25:   %d\0A\00", align 1
@STR4 = private unnamed_addr constant [15 x i8] c"hero - bonus: \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [18 x i8] c"hero - bonus: %d\0A\00", align 1
@STR6 = private unnamed_addr constant [14 x i8] c"hero - 10:   \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [17 x i8] c"hero - 10:   %d\0A\00", align 1
@STR8 = private unnamed_addr constant [16 x i8] c"hero == bonus: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [19 x i8] c"hero == bonus: %s\0A\00", align 1
@STR10 = private unnamed_addr constant [16 x i8] c"hero == 100:   \00", align 1
@true_str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.7 = private unnamed_addr constant [19 x i8] c"hero == 100:   %s\0A\00", align 1

define void @HP.delete(%struct.HP* %0) {
entry:
  ret void
}

define void @HP.operator.ASSIGN.INT(i32 %n, %struct.HP* %self) !dbg !4 {
entry:
  %n1 = alloca i32, align 4, !dbg !7
  %self2 = alloca %struct.HP*, align 8, !dbg !7
  store i32 %n, i32* %n1, align 4, !dbg !7
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !7
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !7
  %val = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !7
  %n4 = load i32, i32* %n1, align 4, !dbg !7
  store i32 %n4, i32* %val, align 4, !dbg !7
  ret void, !dbg !7
}

define void @HP.operator.ASSIGN.HP(%struct.HP* %h, %struct.HP* %self) !dbg !8 {
entry:
  %h1 = alloca %struct.HP*, align 8, !dbg !9
  %self2 = alloca %struct.HP*, align 8, !dbg !9
  store %struct.HP* %h, %struct.HP** %h1, align 8, !dbg !9
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !9
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !9
  %val = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !9
  %h4 = load %struct.HP*, %struct.HP** %h1, align 8, !dbg !9
  %val5 = getelementptr %struct.HP, %struct.HP* %h4, i32 0, i32 0, !dbg !9
  %DOT = load i32, i32* %val5, align 4, !dbg !9
  store i32 %DOT, i32* %val, align 4, !dbg !9
  ret void, !dbg !9
}

define %struct.HP @HP.operator.ADD.HP(%struct.HP* %h, %struct.HP* %self) !dbg !10 {
entry:
  %h1 = alloca %struct.HP*, align 8, !dbg !11
  %self2 = alloca %struct.HP*, align 8, !dbg !11
  %res = alloca %struct.HP, align 8, !dbg !11
  store %struct.HP* %h, %struct.HP** %h1, align 8, !dbg !11
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !11
  store %struct.HP zeroinitializer, %struct.HP* %res, align 4, !dbg !11
  %val = getelementptr %struct.HP, %struct.HP* %res, i32 0, i32 0, !dbg !11
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !11
  %val4 = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !11
  %h5 = load %struct.HP*, %struct.HP** %h1, align 8, !dbg !11
  %val6 = getelementptr %struct.HP, %struct.HP* %h5, i32 0, i32 0, !dbg !11
  %DOT = load i32, i32* %val4, align 4, !dbg !11
  %DOT7 = load i32, i32* %val6, align 4, !dbg !11
  %ADD = add i32 %DOT, %DOT7, !dbg !11
  store i32 %ADD, i32* %val, align 4, !dbg !11
  %res8 = load %struct.HP, %struct.HP* %res, align 4, !dbg !11
  ret %struct.HP %res8, !dbg !11
}

define %struct.HP @HP.operator.ADD.INT(i32 %n, %struct.HP* %self) !dbg !12 {
entry:
  %n1 = alloca i32, align 4, !dbg !13
  %self2 = alloca %struct.HP*, align 8, !dbg !13
  %res = alloca %struct.HP, align 8, !dbg !13
  store i32 %n, i32* %n1, align 4, !dbg !13
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !13
  store %struct.HP zeroinitializer, %struct.HP* %res, align 4, !dbg !13
  %val = getelementptr %struct.HP, %struct.HP* %res, i32 0, i32 0, !dbg !13
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !13
  %val4 = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !13
  %DOT = load i32, i32* %val4, align 4, !dbg !13
  %n5 = load i32, i32* %n1, align 4, !dbg !13
  %ADD = add i32 %DOT, %n5, !dbg !13
  store i32 %ADD, i32* %val, align 4, !dbg !13
  %res6 = load %struct.HP, %struct.HP* %res, align 4, !dbg !13
  ret %struct.HP %res6, !dbg !13
}

define %struct.HP @HP.operator.SUB.HP(%struct.HP* %h, %struct.HP* %self) !dbg !14 {
entry:
  %h1 = alloca %struct.HP*, align 8, !dbg !15
  %self2 = alloca %struct.HP*, align 8, !dbg !15
  %res = alloca %struct.HP, align 8, !dbg !15
  store %struct.HP* %h, %struct.HP** %h1, align 8, !dbg !15
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !15
  store %struct.HP zeroinitializer, %struct.HP* %res, align 4, !dbg !15
  %val = getelementptr %struct.HP, %struct.HP* %res, i32 0, i32 0, !dbg !15
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !15
  %val4 = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !15
  %h5 = load %struct.HP*, %struct.HP** %h1, align 8, !dbg !15
  %val6 = getelementptr %struct.HP, %struct.HP* %h5, i32 0, i32 0, !dbg !15
  %DOT = load i32, i32* %val4, align 4, !dbg !15
  %DOT7 = load i32, i32* %val6, align 4, !dbg !15
  %SUB = sub i32 %DOT, %DOT7, !dbg !15
  store i32 %SUB, i32* %val, align 4, !dbg !15
  %res8 = load %struct.HP, %struct.HP* %res, align 4, !dbg !15
  ret %struct.HP %res8, !dbg !15
}

define %struct.HP @HP.operator.SUB.INT(i32 %n, %struct.HP* %self) !dbg !16 {
entry:
  %n1 = alloca i32, align 4, !dbg !17
  %self2 = alloca %struct.HP*, align 8, !dbg !17
  %res = alloca %struct.HP, align 8, !dbg !17
  store i32 %n, i32* %n1, align 4, !dbg !17
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !17
  store %struct.HP zeroinitializer, %struct.HP* %res, align 4, !dbg !17
  %val = getelementptr %struct.HP, %struct.HP* %res, i32 0, i32 0, !dbg !17
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !17
  %val4 = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !17
  %DOT = load i32, i32* %val4, align 4, !dbg !17
  %n5 = load i32, i32* %n1, align 4, !dbg !17
  %SUB = sub i32 %DOT, %n5, !dbg !17
  store i32 %SUB, i32* %val, align 4, !dbg !17
  %res6 = load %struct.HP, %struct.HP* %res, align 4, !dbg !17
  ret %struct.HP %res6, !dbg !17
}

define i1 @HP.operator.EQ.HP(%struct.HP* %h, %struct.HP* %self) !dbg !18 {
entry:
  %h1 = alloca %struct.HP*, align 8, !dbg !19
  %self2 = alloca %struct.HP*, align 8, !dbg !19
  store %struct.HP* %h, %struct.HP** %h1, align 8, !dbg !19
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !19
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !19
  %val = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !19
  %h4 = load %struct.HP*, %struct.HP** %h1, align 8, !dbg !19
  %val5 = getelementptr %struct.HP, %struct.HP* %h4, i32 0, i32 0, !dbg !19
  %DOT = load i32, i32* %val, align 4, !dbg !19
  %DOT6 = load i32, i32* %val5, align 4, !dbg !19
  %EQ = icmp eq i32 %DOT, %DOT6, !dbg !19
  ret i1 %EQ, !dbg !19
}

define i1 @HP.operator.EQ.INT(i32 %n, %struct.HP* %self) !dbg !20 {
entry:
  %n1 = alloca i32, align 4, !dbg !21
  %self2 = alloca %struct.HP*, align 8, !dbg !21
  store i32 %n, i32* %n1, align 4, !dbg !21
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !21
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !21
  %val = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !21
  %DOT = load i32, i32* %val, align 4, !dbg !21
  %n4 = load i32, i32* %n1, align 4, !dbg !21
  %EQ = icmp eq i32 %DOT, %n4, !dbg !21
  ret i1 %EQ, !dbg !21
}

define i32 @main() !dbg !22 {
entry:
  %hero = alloca %struct.HP, align 8, !dbg !23
  %bonus = alloca %struct.HP, align 8, !dbg !23
  %combined = alloca %struct.HP, align 8, !dbg !23
  %boosted = alloca %struct.HP, align 8, !dbg !23
  %reduced = alloca %struct.HP, align 8, !dbg !23
  %trimmed = alloca %struct.HP, align 8, !dbg !23
  store %struct.HP zeroinitializer, %struct.HP* %hero, align 4, !dbg !23
  call void @HP.operator.ASSIGN.INT(i32 100, %struct.HP* %hero), !dbg !23
  store %struct.HP zeroinitializer, %struct.HP* %bonus, align 4, !dbg !23
  call void @HP.operator.ASSIGN.INT(i32 50, %struct.HP* %bonus), !dbg !23
  store %struct.HP zeroinitializer, %struct.HP* %combined, align 4, !dbg !23
  %0 = call %struct.HP @HP.operator.ADD.HP(%struct.HP* %bonus, %struct.HP* %hero), !dbg !23
  %tmp_op = alloca %struct.HP, align 8, !dbg !23
  store %struct.HP %0, %struct.HP* %tmp_op, align 4, !dbg !23
  call void @HP.operator.ASSIGN.HP(%struct.HP* %tmp_op, %struct.HP* %combined), !dbg !23
  %val = getelementptr %struct.HP, %struct.HP* %combined, i32 0, i32 0, !dbg !23
  %DOT = load i32, i32* %val, align 4, !dbg !23
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt, i32 0, i32 0), i32 %DOT), !dbg !23
  store %struct.HP zeroinitializer, %struct.HP* %boosted, align 4, !dbg !23
  %2 = call %struct.HP @HP.operator.ADD.INT(i32 25, %struct.HP* %hero), !dbg !23
  %tmp_op1 = alloca %struct.HP, align 8, !dbg !23
  store %struct.HP %2, %struct.HP* %tmp_op1, align 4, !dbg !23
  call void @HP.operator.ASSIGN.HP(%struct.HP* %tmp_op1, %struct.HP* %boosted), !dbg !23
  %val2 = getelementptr %struct.HP, %struct.HP* %boosted, i32 0, i32 0, !dbg !23
  %DOT3 = load i32, i32* %val2, align 4, !dbg !23
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.1, i32 0, i32 0), i32 %DOT3), !dbg !23
  store %struct.HP zeroinitializer, %struct.HP* %reduced, align 4, !dbg !23
  %4 = call %struct.HP @HP.operator.SUB.HP(%struct.HP* %bonus, %struct.HP* %hero), !dbg !23
  %tmp_op4 = alloca %struct.HP, align 8, !dbg !23
  store %struct.HP %4, %struct.HP* %tmp_op4, align 4, !dbg !23
  call void @HP.operator.ASSIGN.HP(%struct.HP* %tmp_op4, %struct.HP* %reduced), !dbg !23
  %val5 = getelementptr %struct.HP, %struct.HP* %reduced, i32 0, i32 0, !dbg !23
  %DOT6 = load i32, i32* %val5, align 4, !dbg !23
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.2, i32 0, i32 0), i32 %DOT6), !dbg !23
  store %struct.HP zeroinitializer, %struct.HP* %trimmed, align 4, !dbg !23
  %6 = call %struct.HP @HP.operator.SUB.INT(i32 10, %struct.HP* %hero), !dbg !23
  %tmp_op7 = alloca %struct.HP, align 8, !dbg !23
  store %struct.HP %6, %struct.HP* %tmp_op7, align 4, !dbg !23
  call void @HP.operator.ASSIGN.HP(%struct.HP* %tmp_op7, %struct.HP* %trimmed), !dbg !23
  %val8 = getelementptr %struct.HP, %struct.HP* %trimmed, i32 0, i32 0, !dbg !23
  %DOT9 = load i32, i32* %val8, align 4, !dbg !23
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.3, i32 0, i32 0), i32 %DOT9), !dbg !23
  %8 = call i1 @HP.operator.EQ.HP(%struct.HP* %bonus, %struct.HP* %hero), !dbg !23
  %bool_str = select i1 %8, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !23
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt.4, i32 0, i32 0), i8* %bool_str), !dbg !23
  %10 = call i1 @HP.operator.EQ.INT(i32 100, %struct.HP* %hero), !dbg !23
  %bool_str10 = select i1 %10, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.5, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.6, i32 0, i32 0), !dbg !23
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt.7, i32 0, i32 0), i8* %bool_str10), !dbg !23
  call void @HP.delete(%struct.HP* %hero), !dbg !23
  call void @HP.delete(%struct.HP* %bonus), !dbg !23
  call void @HP.delete(%struct.HP* %combined), !dbg !23
  call void @HP.delete(%struct.HP* %boosted), !dbg !23
  call void @HP.delete(%struct.HP* %reduced), !dbg !23
  call void @HP.delete(%struct.HP* %trimmed), !dbg !23
  ret i32 0, !dbg !23
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "005.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading")
!4 = distinct !DISubprogram(name: "HP.operator.ASSIGN.INT", linkageName: "HP.operator.ASSIGN.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = distinct !DISubprogram(name: "HP.operator.ASSIGN.HP", linkageName: "HP.operator.ASSIGN.HP", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(name: "HP.operator.ADD.HP", linkageName: "HP.operator.ADD.HP", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 0, scope: !10)
!12 = distinct !DISubprogram(name: "HP.operator.ADD.INT", linkageName: "HP.operator.ADD.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 0, scope: !12)
!14 = distinct !DISubprogram(name: "HP.operator.SUB.HP", linkageName: "HP.operator.SUB.HP", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 0, scope: !14)
!16 = distinct !DISubprogram(name: "HP.operator.SUB.INT", linkageName: "HP.operator.SUB.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 0, scope: !16)
!18 = distinct !DISubprogram(name: "HP.operator.EQ.HP", linkageName: "HP.operator.EQ.HP", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 0, scope: !18)
!20 = distinct !DISubprogram(name: "HP.operator.EQ.INT", linkageName: "HP.operator.EQ.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 0, scope: !20)
!22 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 38, type: !5, scopeLine: 38, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!23 = !DILocation(line: 38, scope: !22)
