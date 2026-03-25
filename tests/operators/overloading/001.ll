; ModuleID = 'tests/operators/001.ura'
source_filename = "tests/operators/001.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Vec2 = type { i32, i32 }

@STR0 = private unnamed_addr constant [7 x i8] c"add: (\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c", \00", align 1
@STR2 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@output_fmt = private unnamed_addr constant [15 x i8] c"add: (%d, %d)\0A\00", align 1
@STR3 = private unnamed_addr constant [7 x i8] c"sub: (\00", align 1
@STR4 = private unnamed_addr constant [3 x i8] c", \00", align 1
@STR5 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [15 x i8] c"sub: (%d, %d)\0A\00", align 1
@STR6 = private unnamed_addr constant [7 x i8] c"mul: (\00", align 1
@STR7 = private unnamed_addr constant [3 x i8] c", \00", align 1
@STR8 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [15 x i8] c"mul: (%d, %d)\0A\00", align 1
@STR9 = private unnamed_addr constant [7 x i8] c"div: (\00", align 1
@STR10 = private unnamed_addr constant [3 x i8] c", \00", align 1
@STR11 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [15 x i8] c"div: (%d, %d)\0A\00", align 1

define void @Vec2.clean(%struct.Vec2* %0) {
entry:
  ret void
}

define void @Vec2.init(%struct.Vec2* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Vec2*, align 8, !dbg !7
  store %struct.Vec2* %self, %struct.Vec2** %self1, align 8, !dbg !7
  %self2 = load %struct.Vec2*, %struct.Vec2** %self1, align 8, !dbg !7
  %self3 = load %struct.Vec2*, %struct.Vec2** %self1, align 8, !dbg !7
  %x = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !7
  store i32 0, i32* %x, align 4, !dbg !7
  %self4 = load %struct.Vec2*, %struct.Vec2** %self1, align 8, !dbg !7
  %y = getelementptr %struct.Vec2, %struct.Vec2* %self4, i32 0, i32 1, !dbg !7
  store i32 0, i32* %y, align 4, !dbg !7
  ret void, !dbg !7
}

define void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %v, %struct.Vec2* %self) !dbg !8 {
entry:
  %v1 = alloca %struct.Vec2*, align 8, !dbg !9
  %self2 = alloca %struct.Vec2*, align 8, !dbg !9
  store %struct.Vec2* %v, %struct.Vec2** %v1, align 8, !dbg !9
  store %struct.Vec2* %self, %struct.Vec2** %self2, align 8, !dbg !9
  %self3 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !9
  %x = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !9
  %v4 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !9
  %x5 = getelementptr %struct.Vec2, %struct.Vec2* %v4, i32 0, i32 0, !dbg !9
  %DOT = load i32, i32* %x5, align 4, !dbg !9
  store i32 %DOT, i32* %x, align 4, !dbg !9
  %self6 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !9
  %y = getelementptr %struct.Vec2, %struct.Vec2* %self6, i32 0, i32 1, !dbg !9
  %v7 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !9
  %y8 = getelementptr %struct.Vec2, %struct.Vec2* %v7, i32 0, i32 1, !dbg !9
  %DOT9 = load i32, i32* %y8, align 4, !dbg !9
  store i32 %DOT9, i32* %y, align 4, !dbg !9
  ret void, !dbg !9
}

define %struct.Vec2 @Vec2.operator.ADD.Vec2(%struct.Vec2* %v, %struct.Vec2* %self) !dbg !10 {
entry:
  %v1 = alloca %struct.Vec2*, align 8, !dbg !11
  %self2 = alloca %struct.Vec2*, align 8, !dbg !11
  %res = alloca %struct.Vec2, align 8, !dbg !11
  store %struct.Vec2* %v, %struct.Vec2** %v1, align 8, !dbg !11
  store %struct.Vec2* %self, %struct.Vec2** %self2, align 8, !dbg !11
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %res, align 4, !dbg !11
  call void @Vec2.init(%struct.Vec2* %res), !dbg !11
  %x = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 0, !dbg !11
  %self3 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !11
  %x4 = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !11
  %v5 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !11
  %x6 = getelementptr %struct.Vec2, %struct.Vec2* %v5, i32 0, i32 0, !dbg !11
  %DOT = load i32, i32* %x4, align 4, !dbg !11
  %DOT7 = load i32, i32* %x6, align 4, !dbg !11
  %ADD = add i32 %DOT, %DOT7, !dbg !11
  store i32 %ADD, i32* %x, align 4, !dbg !11
  %y = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 1, !dbg !11
  %self8 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !11
  %y9 = getelementptr %struct.Vec2, %struct.Vec2* %self8, i32 0, i32 1, !dbg !11
  %v10 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !11
  %y11 = getelementptr %struct.Vec2, %struct.Vec2* %v10, i32 0, i32 1, !dbg !11
  %DOT12 = load i32, i32* %y9, align 4, !dbg !11
  %DOT13 = load i32, i32* %y11, align 4, !dbg !11
  %ADD14 = add i32 %DOT12, %DOT13, !dbg !11
  store i32 %ADD14, i32* %y, align 4, !dbg !11
  %res15 = load %struct.Vec2, %struct.Vec2* %res, align 4, !dbg !11
  ret %struct.Vec2 %res15, !dbg !11
}

define %struct.Vec2 @Vec2.operator.SUB.Vec2(%struct.Vec2* %v, %struct.Vec2* %self) !dbg !12 {
entry:
  %v1 = alloca %struct.Vec2*, align 8, !dbg !13
  %self2 = alloca %struct.Vec2*, align 8, !dbg !13
  %res = alloca %struct.Vec2, align 8, !dbg !13
  store %struct.Vec2* %v, %struct.Vec2** %v1, align 8, !dbg !13
  store %struct.Vec2* %self, %struct.Vec2** %self2, align 8, !dbg !13
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %res, align 4, !dbg !13
  call void @Vec2.init(%struct.Vec2* %res), !dbg !13
  %x = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 0, !dbg !13
  %self3 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !13
  %x4 = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !13
  %v5 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !13
  %x6 = getelementptr %struct.Vec2, %struct.Vec2* %v5, i32 0, i32 0, !dbg !13
  %DOT = load i32, i32* %x4, align 4, !dbg !13
  %DOT7 = load i32, i32* %x6, align 4, !dbg !13
  %SUB = sub i32 %DOT, %DOT7, !dbg !13
  store i32 %SUB, i32* %x, align 4, !dbg !13
  %y = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 1, !dbg !13
  %self8 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !13
  %y9 = getelementptr %struct.Vec2, %struct.Vec2* %self8, i32 0, i32 1, !dbg !13
  %v10 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !13
  %y11 = getelementptr %struct.Vec2, %struct.Vec2* %v10, i32 0, i32 1, !dbg !13
  %DOT12 = load i32, i32* %y9, align 4, !dbg !13
  %DOT13 = load i32, i32* %y11, align 4, !dbg !13
  %SUB14 = sub i32 %DOT12, %DOT13, !dbg !13
  store i32 %SUB14, i32* %y, align 4, !dbg !13
  %res15 = load %struct.Vec2, %struct.Vec2* %res, align 4, !dbg !13
  ret %struct.Vec2 %res15, !dbg !13
}

define %struct.Vec2 @Vec2.operator.MUL.INT(i32 %n, %struct.Vec2* %self) !dbg !14 {
entry:
  %n1 = alloca i32, align 4, !dbg !15
  %self2 = alloca %struct.Vec2*, align 8, !dbg !15
  %res = alloca %struct.Vec2, align 8, !dbg !15
  store i32 %n, i32* %n1, align 4, !dbg !15
  store %struct.Vec2* %self, %struct.Vec2** %self2, align 8, !dbg !15
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %res, align 4, !dbg !15
  call void @Vec2.init(%struct.Vec2* %res), !dbg !15
  %x = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 0, !dbg !15
  %self3 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !15
  %x4 = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !15
  %DOT = load i32, i32* %x4, align 4, !dbg !15
  %n5 = load i32, i32* %n1, align 4, !dbg !15
  %MUL = mul i32 %DOT, %n5, !dbg !15
  store i32 %MUL, i32* %x, align 4, !dbg !15
  %y = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 1, !dbg !15
  %self6 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !15
  %y7 = getelementptr %struct.Vec2, %struct.Vec2* %self6, i32 0, i32 1, !dbg !15
  %DOT8 = load i32, i32* %y7, align 4, !dbg !15
  %n9 = load i32, i32* %n1, align 4, !dbg !15
  %MUL10 = mul i32 %DOT8, %n9, !dbg !15
  store i32 %MUL10, i32* %y, align 4, !dbg !15
  %res11 = load %struct.Vec2, %struct.Vec2* %res, align 4, !dbg !15
  ret %struct.Vec2 %res11, !dbg !15
}

define %struct.Vec2 @Vec2.operator.DIV.INT(i32 %n, %struct.Vec2* %self) !dbg !16 {
entry:
  %n1 = alloca i32, align 4, !dbg !17
  %self2 = alloca %struct.Vec2*, align 8, !dbg !17
  %res = alloca %struct.Vec2, align 8, !dbg !17
  store i32 %n, i32* %n1, align 4, !dbg !17
  store %struct.Vec2* %self, %struct.Vec2** %self2, align 8, !dbg !17
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %res, align 4, !dbg !17
  call void @Vec2.init(%struct.Vec2* %res), !dbg !17
  %x = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 0, !dbg !17
  %self3 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !17
  %x4 = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !17
  %DOT = load i32, i32* %x4, align 4, !dbg !17
  %n5 = load i32, i32* %n1, align 4, !dbg !17
  %DIV = sdiv i32 %DOT, %n5, !dbg !17
  store i32 %DIV, i32* %x, align 4, !dbg !17
  %y = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 1, !dbg !17
  %self6 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !17
  %y7 = getelementptr %struct.Vec2, %struct.Vec2* %self6, i32 0, i32 1, !dbg !17
  %DOT8 = load i32, i32* %y7, align 4, !dbg !17
  %n9 = load i32, i32* %n1, align 4, !dbg !17
  %DIV10 = sdiv i32 %DOT8, %n9, !dbg !17
  store i32 %DIV10, i32* %y, align 4, !dbg !17
  %res11 = load %struct.Vec2, %struct.Vec2* %res, align 4, !dbg !17
  ret %struct.Vec2 %res11, !dbg !17
}

define i32 @main() !dbg !18 {
entry:
  %a = alloca %struct.Vec2, align 8, !dbg !19
  %b = alloca %struct.Vec2, align 8, !dbg !19
  %add = alloca %struct.Vec2, align 8, !dbg !19
  %sub = alloca %struct.Vec2, align 8, !dbg !19
  %mul = alloca %struct.Vec2, align 8, !dbg !19
  %div = alloca %struct.Vec2, align 8, !dbg !19
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %a, align 4, !dbg !19
  call void @Vec2.init(%struct.Vec2* %a), !dbg !19
  %x = getelementptr %struct.Vec2, %struct.Vec2* %a, i32 0, i32 0, !dbg !19
  store i32 3, i32* %x, align 4, !dbg !19
  %y = getelementptr %struct.Vec2, %struct.Vec2* %a, i32 0, i32 1, !dbg !19
  store i32 3, i32* %y, align 4, !dbg !19
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %b, align 4, !dbg !19
  call void @Vec2.init(%struct.Vec2* %b), !dbg !19
  %x1 = getelementptr %struct.Vec2, %struct.Vec2* %b, i32 0, i32 0, !dbg !19
  store i32 2, i32* %x1, align 4, !dbg !19
  %y2 = getelementptr %struct.Vec2, %struct.Vec2* %b, i32 0, i32 1, !dbg !19
  store i32 4, i32* %y2, align 4, !dbg !19
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %add, align 4, !dbg !19
  call void @Vec2.init(%struct.Vec2* %add), !dbg !19
  %0 = call %struct.Vec2 @Vec2.operator.ADD.Vec2(%struct.Vec2* %b, %struct.Vec2* %a), !dbg !19
  %tmp_op = alloca %struct.Vec2, align 8, !dbg !19
  store %struct.Vec2 %0, %struct.Vec2* %tmp_op, align 4, !dbg !19
  call void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %tmp_op, %struct.Vec2* %add), !dbg !19
  %x3 = getelementptr %struct.Vec2, %struct.Vec2* %add, i32 0, i32 0, !dbg !19
  %DOT = load i32, i32* %x3, align 4, !dbg !19
  %y4 = getelementptr %struct.Vec2, %struct.Vec2* %add, i32 0, i32 1, !dbg !19
  %DOT5 = load i32, i32* %y4, align 4, !dbg !19
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt, i32 0, i32 0), i32 2, i32 %DOT, i32 %DOT5), !dbg !19
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %sub, align 4, !dbg !19
  call void @Vec2.init(%struct.Vec2* %sub), !dbg !19
  %2 = call %struct.Vec2 @Vec2.operator.SUB.Vec2(%struct.Vec2* %b, %struct.Vec2* %a), !dbg !19
  %tmp_op6 = alloca %struct.Vec2, align 8, !dbg !19
  store %struct.Vec2 %2, %struct.Vec2* %tmp_op6, align 4, !dbg !19
  call void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %tmp_op6, %struct.Vec2* %sub), !dbg !19
  %x7 = getelementptr %struct.Vec2, %struct.Vec2* %sub, i32 0, i32 0, !dbg !19
  %DOT8 = load i32, i32* %x7, align 4, !dbg !19
  %y9 = getelementptr %struct.Vec2, %struct.Vec2* %sub, i32 0, i32 1, !dbg !19
  %DOT10 = load i32, i32* %y9, align 4, !dbg !19
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.1, i32 0, i32 0), i32 2, i32 %DOT8, i32 %DOT10), !dbg !19
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %mul, align 4, !dbg !19
  call void @Vec2.init(%struct.Vec2* %mul), !dbg !19
  %4 = call %struct.Vec2 @Vec2.operator.MUL.INT(i32 2, %struct.Vec2* %a), !dbg !19
  %tmp_op11 = alloca %struct.Vec2, align 8, !dbg !19
  store %struct.Vec2 %4, %struct.Vec2* %tmp_op11, align 4, !dbg !19
  call void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %tmp_op11, %struct.Vec2* %mul), !dbg !19
  %x12 = getelementptr %struct.Vec2, %struct.Vec2* %mul, i32 0, i32 0, !dbg !19
  %DOT13 = load i32, i32* %x12, align 4, !dbg !19
  %y14 = getelementptr %struct.Vec2, %struct.Vec2* %mul, i32 0, i32 1, !dbg !19
  %DOT15 = load i32, i32* %y14, align 4, !dbg !19
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.2, i32 0, i32 0), i32 2, i32 %DOT13, i32 %DOT15), !dbg !19
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %div, align 4, !dbg !19
  call void @Vec2.init(%struct.Vec2* %div), !dbg !19
  %6 = call %struct.Vec2 @Vec2.operator.DIV.INT(i32 2, %struct.Vec2* %b), !dbg !19
  %tmp_op16 = alloca %struct.Vec2, align 8, !dbg !19
  store %struct.Vec2 %6, %struct.Vec2* %tmp_op16, align 4, !dbg !19
  call void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %tmp_op16, %struct.Vec2* %div), !dbg !19
  %x17 = getelementptr %struct.Vec2, %struct.Vec2* %div, i32 0, i32 0, !dbg !19
  %DOT18 = load i32, i32* %x17, align 4, !dbg !19
  %y19 = getelementptr %struct.Vec2, %struct.Vec2* %div, i32 0, i32 1, !dbg !19
  %DOT20 = load i32, i32* %y19, align 4, !dbg !19
  %7 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.3, i32 0, i32 0), i32 2, i32 %DOT18, i32 %DOT20), !dbg !19
  call void @Vec2.clean(%struct.Vec2* %a), !dbg !19
  call void @Vec2.clean(%struct.Vec2* %b), !dbg !19
  call void @Vec2.clean(%struct.Vec2* %add), !dbg !19
  call void @Vec2.clean(%struct.Vec2* %sub), !dbg !19
  call void @Vec2.clean(%struct.Vec2* %mul), !dbg !19
  call void @Vec2.clean(%struct.Vec2* %div), !dbg !19
  ret i32 0, !dbg !19
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "tests/operators")
!4 = distinct !DISubprogram(name: "Vec2.init", linkageName: "Vec2.init", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 7, scope: !4)
!8 = distinct !DISubprogram(name: "Vec2.operator.ASSIGN.Vec2", linkageName: "Vec2.operator.ASSIGN.Vec2", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(name: "Vec2.operator.ADD.Vec2", linkageName: "Vec2.operator.ADD.Vec2", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 0, scope: !10)
!12 = distinct !DISubprogram(name: "Vec2.operator.SUB.Vec2", linkageName: "Vec2.operator.SUB.Vec2", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 0, scope: !12)
!14 = distinct !DISubprogram(name: "Vec2.operator.MUL.INT", linkageName: "Vec2.operator.MUL.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 0, scope: !14)
!16 = distinct !DISubprogram(name: "Vec2.operator.DIV.INT", linkageName: "Vec2.operator.DIV.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 0, scope: !16)
!18 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 39, type: !5, scopeLine: 39, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 39, scope: !18)
