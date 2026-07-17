; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading/003.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Counter = type { i32 }

@STR0 = private unnamed_addr constant [14 x i8] c"after =  10: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [17 x i8] c"after =  10: %d\0A\00", align 1
@STR2 = private unnamed_addr constant [14 x i8] c"after += 5:  \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [17 x i8] c"after += 5:  %d\0A\00", align 1
@STR4 = private unnamed_addr constant [14 x i8] c"after -= 3:  \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [17 x i8] c"after -= 3:  %d\0A\00", align 1
@STR6 = private unnamed_addr constant [14 x i8] c"after *= 2:  \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [17 x i8] c"after *= 2:  %d\0A\00", align 1

define void @Counter.delete(%struct.Counter* %0) {
entry:
  ret void
}

define void @Counter.operator.ASSIGN.INT(i32 %n, %struct.Counter* %self) !dbg !4 {
entry:
  %n1 = alloca i32, align 4, !dbg !7
  %self2 = alloca %struct.Counter*, align 8, !dbg !7
  store i32 %n, i32* %n1, align 4, !dbg !7
  store %struct.Counter* %self, %struct.Counter** %self2, align 8, !dbg !7
  %self3 = load %struct.Counter*, %struct.Counter** %self2, align 8, !dbg !7
  %val = getelementptr %struct.Counter, %struct.Counter* %self3, i32 0, i32 0, !dbg !7
  %n4 = load i32, i32* %n1, align 4, !dbg !7
  store i32 %n4, i32* %val, align 4, !dbg !7
  ret void, !dbg !7
}

define void @Counter.operator.ADD_ASS.INT(i32 %n, %struct.Counter* %self) !dbg !8 {
entry:
  %n1 = alloca i32, align 4, !dbg !9
  %self2 = alloca %struct.Counter*, align 8, !dbg !9
  store i32 %n, i32* %n1, align 4, !dbg !9
  store %struct.Counter* %self, %struct.Counter** %self2, align 8, !dbg !9
  %self3 = load %struct.Counter*, %struct.Counter** %self2, align 8, !dbg !9
  %val = getelementptr %struct.Counter, %struct.Counter* %self3, i32 0, i32 0, !dbg !9
  %self4 = load %struct.Counter*, %struct.Counter** %self2, align 8, !dbg !9
  %val5 = getelementptr %struct.Counter, %struct.Counter* %self4, i32 0, i32 0, !dbg !9
  %DOT = load i32, i32* %val5, align 4, !dbg !9
  %n6 = load i32, i32* %n1, align 4, !dbg !9
  %ADD = add i32 %DOT, %n6, !dbg !9
  store i32 %ADD, i32* %val, align 4, !dbg !9
  ret void, !dbg !9
}

define void @Counter.operator.SUB_ASS.INT(i32 %n, %struct.Counter* %self) !dbg !10 {
entry:
  %n1 = alloca i32, align 4, !dbg !11
  %self2 = alloca %struct.Counter*, align 8, !dbg !11
  store i32 %n, i32* %n1, align 4, !dbg !11
  store %struct.Counter* %self, %struct.Counter** %self2, align 8, !dbg !11
  %self3 = load %struct.Counter*, %struct.Counter** %self2, align 8, !dbg !11
  %val = getelementptr %struct.Counter, %struct.Counter* %self3, i32 0, i32 0, !dbg !11
  %self4 = load %struct.Counter*, %struct.Counter** %self2, align 8, !dbg !11
  %val5 = getelementptr %struct.Counter, %struct.Counter* %self4, i32 0, i32 0, !dbg !11
  %DOT = load i32, i32* %val5, align 4, !dbg !11
  %n6 = load i32, i32* %n1, align 4, !dbg !11
  %SUB = sub i32 %DOT, %n6, !dbg !11
  store i32 %SUB, i32* %val, align 4, !dbg !11
  ret void, !dbg !11
}

define void @Counter.operator.MUL_ASS.INT(i32 %n, %struct.Counter* %self) !dbg !12 {
entry:
  %n1 = alloca i32, align 4, !dbg !13
  %self2 = alloca %struct.Counter*, align 8, !dbg !13
  store i32 %n, i32* %n1, align 4, !dbg !13
  store %struct.Counter* %self, %struct.Counter** %self2, align 8, !dbg !13
  %self3 = load %struct.Counter*, %struct.Counter** %self2, align 8, !dbg !13
  %val = getelementptr %struct.Counter, %struct.Counter* %self3, i32 0, i32 0, !dbg !13
  %self4 = load %struct.Counter*, %struct.Counter** %self2, align 8, !dbg !13
  %val5 = getelementptr %struct.Counter, %struct.Counter* %self4, i32 0, i32 0, !dbg !13
  %DOT = load i32, i32* %val5, align 4, !dbg !13
  %n6 = load i32, i32* %n1, align 4, !dbg !13
  %MUL = mul i32 %DOT, %n6, !dbg !13
  store i32 %MUL, i32* %val, align 4, !dbg !13
  ret void, !dbg !13
}

define i32 @main() !dbg !14 {
entry:
  %c = alloca %struct.Counter, align 8, !dbg !15
  store %struct.Counter zeroinitializer, %struct.Counter* %c, align 4, !dbg !15
  call void @Counter.operator.ASSIGN.INT(i32 10, %struct.Counter* %c), !dbg !15
  %val = getelementptr %struct.Counter, %struct.Counter* %c, i32 0, i32 0, !dbg !15
  %DOT = load i32, i32* %val, align 4, !dbg !15
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt, i32 0, i32 0), i32 %DOT), !dbg !15
  call void @Counter.operator.ADD_ASS.INT(i32 5, %struct.Counter* %c), !dbg !15
  %val1 = getelementptr %struct.Counter, %struct.Counter* %c, i32 0, i32 0, !dbg !15
  %DOT2 = load i32, i32* %val1, align 4, !dbg !15
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.1, i32 0, i32 0), i32 %DOT2), !dbg !15
  call void @Counter.operator.SUB_ASS.INT(i32 3, %struct.Counter* %c), !dbg !15
  %val3 = getelementptr %struct.Counter, %struct.Counter* %c, i32 0, i32 0, !dbg !15
  %DOT4 = load i32, i32* %val3, align 4, !dbg !15
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.2, i32 0, i32 0), i32 %DOT4), !dbg !15
  call void @Counter.operator.MUL_ASS.INT(i32 2, %struct.Counter* %c), !dbg !15
  %val5 = getelementptr %struct.Counter, %struct.Counter* %c, i32 0, i32 0, !dbg !15
  %DOT6 = load i32, i32* %val5, align 4, !dbg !15
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.3, i32 0, i32 0), i32 %DOT6), !dbg !15
  call void @Counter.delete(%struct.Counter* %c), !dbg !15
  ret i32 0, !dbg !15
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading")
!4 = distinct !DISubprogram(name: "Counter.operator.ASSIGN.INT", linkageName: "Counter.operator.ASSIGN.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = distinct !DISubprogram(name: "Counter.operator.ADD_ASS.INT", linkageName: "Counter.operator.ADD_ASS.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(name: "Counter.operator.SUB_ASS.INT", linkageName: "Counter.operator.SUB_ASS.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 0, scope: !10)
!12 = distinct !DISubprogram(name: "Counter.operator.MUL_ASS.INT", linkageName: "Counter.operator.MUL_ASS.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 0, scope: !12)
!14 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 17, type: !5, scopeLine: 17, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 17, scope: !14)
