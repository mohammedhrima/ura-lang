; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading/004.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading/004.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Flags = type { i32 }

@STR0 = private unnamed_addr constant [9 x i8] c"a & b:  \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [12 x i8] c"a & b:  %d\0A\00", align 1
@STR2 = private unnamed_addr constant [9 x i8] c"a | b:  \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [12 x i8] c"a | b:  %d\0A\00", align 1
@STR4 = private unnamed_addr constant [9 x i8] c"a ^ b:  \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [12 x i8] c"a ^ b:  %d\0A\00", align 1
@STR6 = private unnamed_addr constant [9 x i8] c"a << 2: \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [12 x i8] c"a << 2: %d\0A\00", align 1
@STR8 = private unnamed_addr constant [9 x i8] c"a >> 1: \00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [12 x i8] c"a >> 1: %d\0A\00", align 1

define void @Flags.delete(%struct.Flags* %0) {
entry:
  ret void
}

define void @Flags.operator.ASSIGN.INT(i32 %n, %struct.Flags* %self) !dbg !4 {
entry:
  %n1 = alloca i32, align 4, !dbg !7
  %self2 = alloca %struct.Flags*, align 8, !dbg !7
  store i32 %n, i32* %n1, align 4, !dbg !7
  store %struct.Flags* %self, %struct.Flags** %self2, align 8, !dbg !7
  %self3 = load %struct.Flags*, %struct.Flags** %self2, align 8, !dbg !7
  %bits = getelementptr %struct.Flags, %struct.Flags* %self3, i32 0, i32 0, !dbg !7
  %n4 = load i32, i32* %n1, align 4, !dbg !7
  store i32 %n4, i32* %bits, align 4, !dbg !7
  ret void, !dbg !7
}

define void @Flags.operator.ASSIGN.Flags(%struct.Flags* %f, %struct.Flags* %self) !dbg !8 {
entry:
  %f1 = alloca %struct.Flags*, align 8, !dbg !9
  %self2 = alloca %struct.Flags*, align 8, !dbg !9
  store %struct.Flags* %f, %struct.Flags** %f1, align 8, !dbg !9
  store %struct.Flags* %self, %struct.Flags** %self2, align 8, !dbg !9
  %self3 = load %struct.Flags*, %struct.Flags** %self2, align 8, !dbg !9
  %bits = getelementptr %struct.Flags, %struct.Flags* %self3, i32 0, i32 0, !dbg !9
  %f4 = load %struct.Flags*, %struct.Flags** %f1, align 8, !dbg !9
  %bits5 = getelementptr %struct.Flags, %struct.Flags* %f4, i32 0, i32 0, !dbg !9
  %DOT = load i32, i32* %bits5, align 4, !dbg !9
  store i32 %DOT, i32* %bits, align 4, !dbg !9
  ret void, !dbg !9
}

define %struct.Flags @Flags.operator.BAND.Flags(%struct.Flags* %f, %struct.Flags* %self) !dbg !10 {
entry:
  %f1 = alloca %struct.Flags*, align 8, !dbg !11
  %self2 = alloca %struct.Flags*, align 8, !dbg !11
  %res = alloca %struct.Flags, align 8, !dbg !11
  store %struct.Flags* %f, %struct.Flags** %f1, align 8, !dbg !11
  store %struct.Flags* %self, %struct.Flags** %self2, align 8, !dbg !11
  store %struct.Flags zeroinitializer, %struct.Flags* %res, align 4, !dbg !11
  %bits = getelementptr %struct.Flags, %struct.Flags* %res, i32 0, i32 0, !dbg !11
  %self3 = load %struct.Flags*, %struct.Flags** %self2, align 8, !dbg !11
  %bits4 = getelementptr %struct.Flags, %struct.Flags* %self3, i32 0, i32 0, !dbg !11
  %f5 = load %struct.Flags*, %struct.Flags** %f1, align 8, !dbg !11
  %bits6 = getelementptr %struct.Flags, %struct.Flags* %f5, i32 0, i32 0, !dbg !11
  %DOT = load i32, i32* %bits4, align 4, !dbg !11
  %DOT7 = load i32, i32* %bits6, align 4, !dbg !11
  %BAND = and i32 %DOT, %DOT7, !dbg !11
  store i32 %BAND, i32* %bits, align 4, !dbg !11
  %res8 = load %struct.Flags, %struct.Flags* %res, align 4, !dbg !11
  ret %struct.Flags %res8, !dbg !11
}

define %struct.Flags @Flags.operator.BOR.Flags(%struct.Flags* %f, %struct.Flags* %self) !dbg !12 {
entry:
  %f1 = alloca %struct.Flags*, align 8, !dbg !13
  %self2 = alloca %struct.Flags*, align 8, !dbg !13
  %res = alloca %struct.Flags, align 8, !dbg !13
  store %struct.Flags* %f, %struct.Flags** %f1, align 8, !dbg !13
  store %struct.Flags* %self, %struct.Flags** %self2, align 8, !dbg !13
  store %struct.Flags zeroinitializer, %struct.Flags* %res, align 4, !dbg !13
  %bits = getelementptr %struct.Flags, %struct.Flags* %res, i32 0, i32 0, !dbg !13
  %self3 = load %struct.Flags*, %struct.Flags** %self2, align 8, !dbg !13
  %bits4 = getelementptr %struct.Flags, %struct.Flags* %self3, i32 0, i32 0, !dbg !13
  %f5 = load %struct.Flags*, %struct.Flags** %f1, align 8, !dbg !13
  %bits6 = getelementptr %struct.Flags, %struct.Flags* %f5, i32 0, i32 0, !dbg !13
  %DOT = load i32, i32* %bits4, align 4, !dbg !13
  %DOT7 = load i32, i32* %bits6, align 4, !dbg !13
  %BOR = or i32 %DOT, %DOT7, !dbg !13
  store i32 %BOR, i32* %bits, align 4, !dbg !13
  %res8 = load %struct.Flags, %struct.Flags* %res, align 4, !dbg !13
  ret %struct.Flags %res8, !dbg !13
}

define %struct.Flags @Flags.operator.BXOR.Flags(%struct.Flags* %f, %struct.Flags* %self) !dbg !14 {
entry:
  %f1 = alloca %struct.Flags*, align 8, !dbg !15
  %self2 = alloca %struct.Flags*, align 8, !dbg !15
  %res = alloca %struct.Flags, align 8, !dbg !15
  store %struct.Flags* %f, %struct.Flags** %f1, align 8, !dbg !15
  store %struct.Flags* %self, %struct.Flags** %self2, align 8, !dbg !15
  store %struct.Flags zeroinitializer, %struct.Flags* %res, align 4, !dbg !15
  %bits = getelementptr %struct.Flags, %struct.Flags* %res, i32 0, i32 0, !dbg !15
  %self3 = load %struct.Flags*, %struct.Flags** %self2, align 8, !dbg !15
  %bits4 = getelementptr %struct.Flags, %struct.Flags* %self3, i32 0, i32 0, !dbg !15
  %f5 = load %struct.Flags*, %struct.Flags** %f1, align 8, !dbg !15
  %bits6 = getelementptr %struct.Flags, %struct.Flags* %f5, i32 0, i32 0, !dbg !15
  %DOT = load i32, i32* %bits4, align 4, !dbg !15
  %DOT7 = load i32, i32* %bits6, align 4, !dbg !15
  %BXOR = xor i32 %DOT, %DOT7, !dbg !15
  store i32 %BXOR, i32* %bits, align 4, !dbg !15
  %res8 = load %struct.Flags, %struct.Flags* %res, align 4, !dbg !15
  ret %struct.Flags %res8, !dbg !15
}

define %struct.Flags @Flags.operator.LSHIFT.INT(i32 %n, %struct.Flags* %self) !dbg !16 {
entry:
  %n1 = alloca i32, align 4, !dbg !17
  %self2 = alloca %struct.Flags*, align 8, !dbg !17
  %res = alloca %struct.Flags, align 8, !dbg !17
  store i32 %n, i32* %n1, align 4, !dbg !17
  store %struct.Flags* %self, %struct.Flags** %self2, align 8, !dbg !17
  store %struct.Flags zeroinitializer, %struct.Flags* %res, align 4, !dbg !17
  %bits = getelementptr %struct.Flags, %struct.Flags* %res, i32 0, i32 0, !dbg !17
  %self3 = load %struct.Flags*, %struct.Flags** %self2, align 8, !dbg !17
  %bits4 = getelementptr %struct.Flags, %struct.Flags* %self3, i32 0, i32 0, !dbg !17
  %DOT = load i32, i32* %bits4, align 4, !dbg !17
  %n5 = load i32, i32* %n1, align 4, !dbg !17
  %LSHIFT = shl i32 %DOT, %n5, !dbg !17
  store i32 %LSHIFT, i32* %bits, align 4, !dbg !17
  %res6 = load %struct.Flags, %struct.Flags* %res, align 4, !dbg !17
  ret %struct.Flags %res6, !dbg !17
}

define %struct.Flags @Flags.operator.RSHIFT.INT(i32 %n, %struct.Flags* %self) !dbg !18 {
entry:
  %n1 = alloca i32, align 4, !dbg !19
  %self2 = alloca %struct.Flags*, align 8, !dbg !19
  %res = alloca %struct.Flags, align 8, !dbg !19
  store i32 %n, i32* %n1, align 4, !dbg !19
  store %struct.Flags* %self, %struct.Flags** %self2, align 8, !dbg !19
  store %struct.Flags zeroinitializer, %struct.Flags* %res, align 4, !dbg !19
  %bits = getelementptr %struct.Flags, %struct.Flags* %res, i32 0, i32 0, !dbg !19
  %self3 = load %struct.Flags*, %struct.Flags** %self2, align 8, !dbg !19
  %bits4 = getelementptr %struct.Flags, %struct.Flags* %self3, i32 0, i32 0, !dbg !19
  %DOT = load i32, i32* %bits4, align 4, !dbg !19
  %n5 = load i32, i32* %n1, align 4, !dbg !19
  %RSHIFT = ashr i32 %DOT, %n5, !dbg !19
  store i32 %RSHIFT, i32* %bits, align 4, !dbg !19
  %res6 = load %struct.Flags, %struct.Flags* %res, align 4, !dbg !19
  ret %struct.Flags %res6, !dbg !19
}

define i32 @main() !dbg !20 {
entry:
  %a = alloca %struct.Flags, align 8, !dbg !21
  %b = alloca %struct.Flags, align 8, !dbg !21
  %band = alloca %struct.Flags, align 8, !dbg !21
  %bor = alloca %struct.Flags, align 8, !dbg !21
  %bxor = alloca %struct.Flags, align 8, !dbg !21
  %lsh = alloca %struct.Flags, align 8, !dbg !21
  %rsh = alloca %struct.Flags, align 8, !dbg !21
  store %struct.Flags zeroinitializer, %struct.Flags* %a, align 4, !dbg !21
  call void @Flags.operator.ASSIGN.INT(i32 12, %struct.Flags* %a), !dbg !21
  store %struct.Flags zeroinitializer, %struct.Flags* %b, align 4, !dbg !21
  call void @Flags.operator.ASSIGN.INT(i32 10, %struct.Flags* %b), !dbg !21
  store %struct.Flags zeroinitializer, %struct.Flags* %band, align 4, !dbg !21
  %0 = call %struct.Flags @Flags.operator.BAND.Flags(%struct.Flags* %b, %struct.Flags* %a), !dbg !21
  %tmp_op = alloca %struct.Flags, align 8, !dbg !21
  store %struct.Flags %0, %struct.Flags* %tmp_op, align 4, !dbg !21
  call void @Flags.operator.ASSIGN.Flags(%struct.Flags* %tmp_op, %struct.Flags* %band), !dbg !21
  %bits = getelementptr %struct.Flags, %struct.Flags* %band, i32 0, i32 0, !dbg !21
  %DOT = load i32, i32* %bits, align 4, !dbg !21
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt, i32 0, i32 0), i32 %DOT), !dbg !21
  store %struct.Flags zeroinitializer, %struct.Flags* %bor, align 4, !dbg !21
  %2 = call %struct.Flags @Flags.operator.BOR.Flags(%struct.Flags* %b, %struct.Flags* %a), !dbg !21
  %tmp_op1 = alloca %struct.Flags, align 8, !dbg !21
  store %struct.Flags %2, %struct.Flags* %tmp_op1, align 4, !dbg !21
  call void @Flags.operator.ASSIGN.Flags(%struct.Flags* %tmp_op1, %struct.Flags* %bor), !dbg !21
  %bits2 = getelementptr %struct.Flags, %struct.Flags* %bor, i32 0, i32 0, !dbg !21
  %DOT3 = load i32, i32* %bits2, align 4, !dbg !21
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.1, i32 0, i32 0), i32 %DOT3), !dbg !21
  store %struct.Flags zeroinitializer, %struct.Flags* %bxor, align 4, !dbg !21
  %4 = call %struct.Flags @Flags.operator.BXOR.Flags(%struct.Flags* %b, %struct.Flags* %a), !dbg !21
  %tmp_op4 = alloca %struct.Flags, align 8, !dbg !21
  store %struct.Flags %4, %struct.Flags* %tmp_op4, align 4, !dbg !21
  call void @Flags.operator.ASSIGN.Flags(%struct.Flags* %tmp_op4, %struct.Flags* %bxor), !dbg !21
  %bits5 = getelementptr %struct.Flags, %struct.Flags* %bxor, i32 0, i32 0, !dbg !21
  %DOT6 = load i32, i32* %bits5, align 4, !dbg !21
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.2, i32 0, i32 0), i32 %DOT6), !dbg !21
  store %struct.Flags zeroinitializer, %struct.Flags* %lsh, align 4, !dbg !21
  %6 = call %struct.Flags @Flags.operator.LSHIFT.INT(i32 2, %struct.Flags* %a), !dbg !21
  %tmp_op7 = alloca %struct.Flags, align 8, !dbg !21
  store %struct.Flags %6, %struct.Flags* %tmp_op7, align 4, !dbg !21
  call void @Flags.operator.ASSIGN.Flags(%struct.Flags* %tmp_op7, %struct.Flags* %lsh), !dbg !21
  %bits8 = getelementptr %struct.Flags, %struct.Flags* %lsh, i32 0, i32 0, !dbg !21
  %DOT9 = load i32, i32* %bits8, align 4, !dbg !21
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.3, i32 0, i32 0), i32 %DOT9), !dbg !21
  store %struct.Flags zeroinitializer, %struct.Flags* %rsh, align 4, !dbg !21
  %8 = call %struct.Flags @Flags.operator.RSHIFT.INT(i32 1, %struct.Flags* %a), !dbg !21
  %tmp_op10 = alloca %struct.Flags, align 8, !dbg !21
  store %struct.Flags %8, %struct.Flags* %tmp_op10, align 4, !dbg !21
  call void @Flags.operator.ASSIGN.Flags(%struct.Flags* %tmp_op10, %struct.Flags* %rsh), !dbg !21
  %bits11 = getelementptr %struct.Flags, %struct.Flags* %rsh, i32 0, i32 0, !dbg !21
  %DOT12 = load i32, i32* %bits11, align 4, !dbg !21
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.4, i32 0, i32 0), i32 %DOT12), !dbg !21
  call void @Flags.delete(%struct.Flags* %a), !dbg !21
  call void @Flags.delete(%struct.Flags* %b), !dbg !21
  call void @Flags.delete(%struct.Flags* %band), !dbg !21
  call void @Flags.delete(%struct.Flags* %bor), !dbg !21
  call void @Flags.delete(%struct.Flags* %bxor), !dbg !21
  call void @Flags.delete(%struct.Flags* %lsh), !dbg !21
  call void @Flags.delete(%struct.Flags* %rsh), !dbg !21
  ret i32 0, !dbg !21
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "004.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading")
!4 = distinct !DISubprogram(name: "Flags.operator.ASSIGN.INT", linkageName: "Flags.operator.ASSIGN.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = distinct !DISubprogram(name: "Flags.operator.ASSIGN.Flags", linkageName: "Flags.operator.ASSIGN.Flags", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(name: "Flags.operator.BAND.Flags", linkageName: "Flags.operator.BAND.Flags", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 0, scope: !10)
!12 = distinct !DISubprogram(name: "Flags.operator.BOR.Flags", linkageName: "Flags.operator.BOR.Flags", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 0, scope: !12)
!14 = distinct !DISubprogram(name: "Flags.operator.BXOR.Flags", linkageName: "Flags.operator.BXOR.Flags", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 0, scope: !14)
!16 = distinct !DISubprogram(name: "Flags.operator.LSHIFT.INT", linkageName: "Flags.operator.LSHIFT.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 0, scope: !16)
!18 = distinct !DISubprogram(name: "Flags.operator.RSHIFT.INT", linkageName: "Flags.operator.RSHIFT.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 0, scope: !18)
!20 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 37, type: !5, scopeLine: 37, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 37, scope: !20)
