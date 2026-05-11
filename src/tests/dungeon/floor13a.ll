; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor13a.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor13a.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Vec2 = type { i32, i32 }

@STR0 = private unnamed_addr constant [2 x i8] c"(\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c", \00", align 1
@STR2 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@output_fmt = private unnamed_addr constant [10 x i8] c"(%d, %d)\0A\00", align 1
@STR3 = private unnamed_addr constant [12 x i8] c"at origin: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [15 x i8] c"at origin: %s\0A\00", align 1

define void @Vec2.delete(%struct.Vec2* %0) {
entry:
  ret void
}

define void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %v, %struct.Vec2* %self) !dbg !4 {
entry:
  %v1 = alloca %struct.Vec2*, align 8, !dbg !7
  %self2 = alloca %struct.Vec2*, align 8, !dbg !7
  store %struct.Vec2* %v, %struct.Vec2** %v1, align 8, !dbg !7
  store %struct.Vec2* %self, %struct.Vec2** %self2, align 8, !dbg !7
  %self3 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !7
  %x = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !7
  %v4 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !7
  %x5 = getelementptr %struct.Vec2, %struct.Vec2* %v4, i32 0, i32 0, !dbg !7
  %DOT = load i32, i32* %x5, align 4, !dbg !7
  store i32 %DOT, i32* %x, align 4, !dbg !7
  %self6 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !7
  %y = getelementptr %struct.Vec2, %struct.Vec2* %self6, i32 0, i32 1, !dbg !7
  %v7 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !7
  %y8 = getelementptr %struct.Vec2, %struct.Vec2* %v7, i32 0, i32 1, !dbg !7
  %DOT9 = load i32, i32* %y8, align 4, !dbg !7
  store i32 %DOT9, i32* %y, align 4, !dbg !7
  ret void, !dbg !7
}

define %struct.Vec2 @Vec2.operator.ADD.Vec2(%struct.Vec2* %v, %struct.Vec2* %self) !dbg !8 {
entry:
  %v1 = alloca %struct.Vec2*, align 8, !dbg !9
  %self2 = alloca %struct.Vec2*, align 8, !dbg !9
  %res = alloca %struct.Vec2, align 8, !dbg !9
  store %struct.Vec2* %v, %struct.Vec2** %v1, align 8, !dbg !9
  store %struct.Vec2* %self, %struct.Vec2** %self2, align 8, !dbg !9
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %res, align 4, !dbg !9
  %x = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 0, !dbg !9
  %self3 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !9
  %x4 = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !9
  %v5 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !9
  %x6 = getelementptr %struct.Vec2, %struct.Vec2* %v5, i32 0, i32 0, !dbg !9
  %DOT = load i32, i32* %x4, align 4, !dbg !9
  %DOT7 = load i32, i32* %x6, align 4, !dbg !9
  %ADD = add i32 %DOT, %DOT7, !dbg !9
  store i32 %ADD, i32* %x, align 4, !dbg !9
  %y = getelementptr %struct.Vec2, %struct.Vec2* %res, i32 0, i32 1, !dbg !9
  %self8 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !9
  %y9 = getelementptr %struct.Vec2, %struct.Vec2* %self8, i32 0, i32 1, !dbg !9
  %v10 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !9
  %y11 = getelementptr %struct.Vec2, %struct.Vec2* %v10, i32 0, i32 1, !dbg !9
  %DOT12 = load i32, i32* %y9, align 4, !dbg !9
  %DOT13 = load i32, i32* %y11, align 4, !dbg !9
  %ADD14 = add i32 %DOT12, %DOT13, !dbg !9
  store i32 %ADD14, i32* %y, align 4, !dbg !9
  %res15 = load %struct.Vec2, %struct.Vec2* %res, align 4, !dbg !9
  ret %struct.Vec2 %res15, !dbg !9
}

define i1 @Vec2.operator.EQ.Vec2(%struct.Vec2* %v, %struct.Vec2* %self) !dbg !10 {
entry:
  %v1 = alloca %struct.Vec2*, align 8, !dbg !11
  %self2 = alloca %struct.Vec2*, align 8, !dbg !11
  store %struct.Vec2* %v, %struct.Vec2** %v1, align 8, !dbg !11
  store %struct.Vec2* %self, %struct.Vec2** %self2, align 8, !dbg !11
  %self3 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !11
  %x = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 0, !dbg !11
  %v4 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !11
  %x5 = getelementptr %struct.Vec2, %struct.Vec2* %v4, i32 0, i32 0, !dbg !11
  %DOT = load i32, i32* %x, align 4, !dbg !11
  %DOT6 = load i32, i32* %x5, align 4, !dbg !11
  %EQ = icmp eq i32 %DOT, %DOT6, !dbg !11
  %self7 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !11
  %y = getelementptr %struct.Vec2, %struct.Vec2* %self7, i32 0, i32 1, !dbg !11
  %v8 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !11
  %y9 = getelementptr %struct.Vec2, %struct.Vec2* %v8, i32 0, i32 1, !dbg !11
  %DOT10 = load i32, i32* %y, align 4, !dbg !11
  %DOT11 = load i32, i32* %y9, align 4, !dbg !11
  %EQ12 = icmp eq i32 %DOT10, %DOT11, !dbg !11
  %AND = and i1 %EQ, %EQ12, !dbg !11
  ret i1 %AND, !dbg !11
}

define void @Vec2.print(%struct.Vec2* %self) !dbg !12 {
entry:
  %self1 = alloca %struct.Vec2*, align 8, !dbg !13
  store %struct.Vec2* %self, %struct.Vec2** %self1, align 8, !dbg !13
  %self2 = load %struct.Vec2*, %struct.Vec2** %self1, align 8, !dbg !13
  %x = getelementptr %struct.Vec2, %struct.Vec2* %self2, i32 0, i32 0, !dbg !13
  %DOT = load i32, i32* %x, align 4, !dbg !13
  %self3 = load %struct.Vec2*, %struct.Vec2** %self1, align 8, !dbg !13
  %y = getelementptr %struct.Vec2, %struct.Vec2* %self3, i32 0, i32 1, !dbg !13
  %DOT4 = load i32, i32* %y, align 4, !dbg !13
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @output_fmt, i32 0, i32 0), i32 %DOT, i32 %DOT4), !dbg !13
  ret void, !dbg !13
}

declare i32 @printf(i8*, ...)

define %struct.Vec2 @walk(i32 %steps, %struct.Vec2 %dir) !dbg !14 {
entry:
  %steps1 = alloca i32, align 4, !dbg !15
  %dir2 = alloca %struct.Vec2, align 8, !dbg !15
  %pos = alloca %struct.Vec2, align 8, !dbg !15
  %i = alloca i32, align 4, !dbg !15
  store i32 %steps, i32* %steps1, align 4, !dbg !15
  store %struct.Vec2 %dir, %struct.Vec2* %dir2, align 4, !dbg !15
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %pos, align 4, !dbg !15
  store i32 0, i32* %i, align 4, !dbg !15
  store i32 0, i32* %i, align 4, !dbg !15
  br label %while.start, !dbg !15

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !15
  %steps4 = load i32, i32* %steps1, align 4, !dbg !15
  %LT = icmp slt i32 %i3, %steps4, !dbg !15
  br i1 %LT, label %while.then, label %while.end, !dbg !15

while.then:                                       ; preds = %while.start
  %0 = call %struct.Vec2 @Vec2.operator.ADD.Vec2(%struct.Vec2* %dir2, %struct.Vec2* %pos), !dbg !15
  %tmp_op = alloca %struct.Vec2, align 8, !dbg !15
  store %struct.Vec2 %0, %struct.Vec2* %tmp_op, align 4, !dbg !15
  call void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %tmp_op, %struct.Vec2* %pos), !dbg !15
  %i5 = load i32, i32* %i, align 4, !dbg !15
  %ADD = add i32 %i5, 1, !dbg !15
  store i32 %ADD, i32* %i, align 4, !dbg !15
  br label %while.start, !dbg !15

while.end:                                        ; preds = %while.start
  call void @Vec2.delete(%struct.Vec2* %dir2), !dbg !15
  %pos6 = load %struct.Vec2, %struct.Vec2* %pos, align 4, !dbg !15
  ret %struct.Vec2 %pos6, !dbg !15
}

define i32 @main() !dbg !16 {
entry:
  %north = alloca %struct.Vec2, align 8, !dbg !17
  %east = alloca %struct.Vec2, align 8, !dbg !17
  %pos = alloca %struct.Vec2, align 8, !dbg !17
  %origin = alloca %struct.Vec2, align 8, !dbg !17
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %north, align 4, !dbg !17
  %x = getelementptr %struct.Vec2, %struct.Vec2* %north, i32 0, i32 0, !dbg !17
  store i32 0, i32* %x, align 4, !dbg !17
  %y = getelementptr %struct.Vec2, %struct.Vec2* %north, i32 0, i32 1, !dbg !17
  store i32 1, i32* %y, align 4, !dbg !17
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %east, align 4, !dbg !17
  %x1 = getelementptr %struct.Vec2, %struct.Vec2* %east, i32 0, i32 0, !dbg !17
  store i32 1, i32* %x1, align 4, !dbg !17
  %y2 = getelementptr %struct.Vec2, %struct.Vec2* %east, i32 0, i32 1, !dbg !17
  store i32 0, i32* %y2, align 4, !dbg !17
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %pos, align 4, !dbg !17
  %copy = alloca %struct.Vec2, align 8, !dbg !18
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %copy, align 4, !dbg !18
  call void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %north, %struct.Vec2* %copy), !dbg !18
  %copy_val = load %struct.Vec2, %struct.Vec2* %copy, align 4, !dbg !18
  %walk = call %struct.Vec2 @walk(i32 3, %struct.Vec2 %copy_val), !dbg !18
  %tmp_struct = alloca %struct.Vec2, align 8, !dbg !18
  store %struct.Vec2 %walk, %struct.Vec2* %tmp_struct, align 4, !dbg !18
  %tmp_op = alloca %struct.Vec2, align 8, !dbg !18
  store %struct.Vec2 %walk, %struct.Vec2* %tmp_op, align 4, !dbg !18
  call void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %tmp_op, %struct.Vec2* %pos), !dbg !18
  %0 = call %struct.Vec2 @Vec2.operator.ADD.Vec2(%struct.Vec2* %east, %struct.Vec2* %pos), !dbg !18
  %tmp_op3 = alloca %struct.Vec2, align 8, !dbg !18
  store %struct.Vec2 %0, %struct.Vec2* %tmp_op3, align 4, !dbg !18
  call void @Vec2.operator.ASSIGN.Vec2(%struct.Vec2* %tmp_op3, %struct.Vec2* %pos), !dbg !18
  call void @Vec2.print(%struct.Vec2* %pos), !dbg !19
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %origin, align 4, !dbg !17
  %1 = call i1 @Vec2.operator.EQ.Vec2(%struct.Vec2* %origin, %struct.Vec2* %pos), !dbg !17
  %bool_str = select i1 %1, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !17
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.1, i32 0, i32 0), i8* %bool_str), !dbg !17
  call void @Vec2.delete(%struct.Vec2* %north), !dbg !17
  call void @Vec2.delete(%struct.Vec2* %east), !dbg !17
  call void @Vec2.delete(%struct.Vec2* %pos), !dbg !17
  call void @Vec2.delete(%struct.Vec2* %origin), !dbg !17
  call void @Vec2.delete(%struct.Vec2* %tmp_struct), !dbg !17
  ret i32 0, !dbg !17
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor13a.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon")
!4 = distinct !DISubprogram(name: "Vec2.operator.ASSIGN.Vec2", linkageName: "Vec2.operator.ASSIGN.Vec2", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = distinct !DISubprogram(name: "Vec2.operator.ADD.Vec2", linkageName: "Vec2.operator.ADD.Vec2", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(name: "Vec2.operator.EQ.Vec2", linkageName: "Vec2.operator.EQ.Vec2", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 0, scope: !10)
!12 = distinct !DISubprogram(name: "Vec2.print", linkageName: "Vec2.print", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 19, scope: !12)
!14 = distinct !DISubprogram(name: "walk", linkageName: "walk", scope: null, file: !3, line: 22, type: !5, scopeLine: 22, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 22, scope: !14)
!16 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 30, type: !5, scopeLine: 30, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 30, scope: !16)
!18 = !DILocation(line: 39, scope: !16)
!19 = !DILocation(line: 41, scope: !16)
