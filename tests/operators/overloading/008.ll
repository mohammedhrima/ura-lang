; ModuleID = 'tests/operators/008.ura'
source_filename = "tests/operators/008.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Vec2 = type { i32, i32 }

@STR0 = private unnamed_addr constant [14 x i8] c"dot product: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [17 x i8] c"dot product: %d\0A\00", align 1
@STR2 = private unnamed_addr constant [16 x i8] c"perpendicular: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [19 x i8] c"perpendicular: %d\0A\00", align 1

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

define i32 @Vec2.operator.MUL.Vec2(%struct.Vec2* %v, %struct.Vec2* %self) !dbg !10 {
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
  %MUL = mul i32 %DOT, %DOT6, !dbg !11
  %self7 = load %struct.Vec2*, %struct.Vec2** %self2, align 8, !dbg !11
  %y = getelementptr %struct.Vec2, %struct.Vec2* %self7, i32 0, i32 1, !dbg !11
  %v8 = load %struct.Vec2*, %struct.Vec2** %v1, align 8, !dbg !11
  %y9 = getelementptr %struct.Vec2, %struct.Vec2* %v8, i32 0, i32 1, !dbg !11
  %DOT10 = load i32, i32* %y, align 4, !dbg !11
  %DOT11 = load i32, i32* %y9, align 4, !dbg !11
  %MUL12 = mul i32 %DOT10, %DOT11, !dbg !11
  %ADD = add i32 %MUL, %MUL12, !dbg !11
  ret i32 %ADD, !dbg !11
}

define i32 @main() !dbg !12 {
entry:
  %a = alloca %struct.Vec2, align 8, !dbg !13
  %b = alloca %struct.Vec2, align 8, !dbg !13
  %dot = alloca i32, align 4, !dbg !13
  %c = alloca %struct.Vec2, align 8, !dbg !13
  %d = alloca %struct.Vec2, align 8, !dbg !13
  %perp = alloca i32, align 4, !dbg !13
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %a, align 4, !dbg !13
  call void @Vec2.init(%struct.Vec2* %a), !dbg !13
  %x = getelementptr %struct.Vec2, %struct.Vec2* %a, i32 0, i32 0, !dbg !13
  store i32 3, i32* %x, align 4, !dbg !13
  %y = getelementptr %struct.Vec2, %struct.Vec2* %a, i32 0, i32 1, !dbg !13
  store i32 4, i32* %y, align 4, !dbg !13
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %b, align 4, !dbg !13
  call void @Vec2.init(%struct.Vec2* %b), !dbg !13
  %x1 = getelementptr %struct.Vec2, %struct.Vec2* %b, i32 0, i32 0, !dbg !13
  store i32 2, i32* %x1, align 4, !dbg !13
  %y2 = getelementptr %struct.Vec2, %struct.Vec2* %b, i32 0, i32 1, !dbg !13
  store i32 5, i32* %y2, align 4, !dbg !13
  store i32 0, i32* %dot, align 4, !dbg !13
  %0 = call i32 @Vec2.operator.MUL.Vec2(%struct.Vec2* %b, %struct.Vec2* %a), !dbg !13
  store i32 %0, i32* %dot, align 4, !dbg !13
  %dot3 = load i32, i32* %dot, align 4, !dbg !13
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %dot3), !dbg !13
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %c, align 4, !dbg !13
  call void @Vec2.init(%struct.Vec2* %c), !dbg !13
  %x4 = getelementptr %struct.Vec2, %struct.Vec2* %c, i32 0, i32 0, !dbg !13
  store i32 1, i32* %x4, align 4, !dbg !13
  %y5 = getelementptr %struct.Vec2, %struct.Vec2* %c, i32 0, i32 1, !dbg !13
  store i32 0, i32* %y5, align 4, !dbg !13
  store %struct.Vec2 zeroinitializer, %struct.Vec2* %d, align 4, !dbg !13
  call void @Vec2.init(%struct.Vec2* %d), !dbg !13
  %x6 = getelementptr %struct.Vec2, %struct.Vec2* %d, i32 0, i32 0, !dbg !13
  store i32 0, i32* %x6, align 4, !dbg !13
  %y7 = getelementptr %struct.Vec2, %struct.Vec2* %d, i32 0, i32 1, !dbg !13
  store i32 1, i32* %y7, align 4, !dbg !13
  store i32 0, i32* %perp, align 4, !dbg !13
  %2 = call i32 @Vec2.operator.MUL.Vec2(%struct.Vec2* %d, %struct.Vec2* %c), !dbg !13
  store i32 %2, i32* %perp, align 4, !dbg !13
  %perp8 = load i32, i32* %perp, align 4, !dbg !13
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %perp8), !dbg !13
  call void @Vec2.clean(%struct.Vec2* %a), !dbg !13
  call void @Vec2.clean(%struct.Vec2* %b), !dbg !13
  call void @Vec2.clean(%struct.Vec2* %c), !dbg !13
  call void @Vec2.clean(%struct.Vec2* %d), !dbg !13
  ret i32 0, !dbg !13
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "008.ura", directory: "tests/operators")
!4 = distinct !DISubprogram(name: "Vec2.init", linkageName: "Vec2.init", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 7, scope: !4)
!8 = distinct !DISubprogram(name: "Vec2.operator.ASSIGN.Vec2", linkageName: "Vec2.operator.ASSIGN.Vec2", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(name: "Vec2.operator.MUL.Vec2", linkageName: "Vec2.operator.MUL.Vec2", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 0, scope: !10)
!12 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 18, type: !5, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 18, scope: !12)
