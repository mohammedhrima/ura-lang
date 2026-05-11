; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading/007.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading/007.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.HP = type { i32 }

@STR0 = private unnamed_addr constant [14 x i8] c"hero is dead\0A\00", align 1
@output_fmt = private unnamed_addr constant [14 x i8] c"hero is dead\0A\00", align 1
@STR1 = private unnamed_addr constant [15 x i8] c"hero survived\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [15 x i8] c"hero survived\0A\00", align 1

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

define i1 @HP.operator.GT.HP(%struct.HP* %h, %struct.HP* %self) !dbg !8 {
entry:
  %h1 = alloca %struct.HP*, align 8, !dbg !9
  %self2 = alloca %struct.HP*, align 8, !dbg !9
  store %struct.HP* %h, %struct.HP** %h1, align 8, !dbg !9
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !9
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !9
  %val = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !9
  %h4 = load %struct.HP*, %struct.HP** %h1, align 8, !dbg !9
  %val5 = getelementptr %struct.HP, %struct.HP* %h4, i32 0, i32 0, !dbg !9
  %DOT = load i32, i32* %val, align 4, !dbg !9
  %DOT6 = load i32, i32* %val5, align 4, !dbg !9
  %GT = icmp sgt i32 %DOT, %DOT6, !dbg !9
  ret i1 %GT, !dbg !9
}

define i1 @HP.operator.EQ.HP(%struct.HP* %h, %struct.HP* %self) !dbg !10 {
entry:
  %h1 = alloca %struct.HP*, align 8, !dbg !11
  %self2 = alloca %struct.HP*, align 8, !dbg !11
  store %struct.HP* %h, %struct.HP** %h1, align 8, !dbg !11
  store %struct.HP* %self, %struct.HP** %self2, align 8, !dbg !11
  %self3 = load %struct.HP*, %struct.HP** %self2, align 8, !dbg !11
  %val = getelementptr %struct.HP, %struct.HP* %self3, i32 0, i32 0, !dbg !11
  %h4 = load %struct.HP*, %struct.HP** %h1, align 8, !dbg !11
  %val5 = getelementptr %struct.HP, %struct.HP* %h4, i32 0, i32 0, !dbg !11
  %DOT = load i32, i32* %val, align 4, !dbg !11
  %DOT6 = load i32, i32* %val5, align 4, !dbg !11
  %EQ = icmp eq i32 %DOT, %DOT6, !dbg !11
  ret i1 %EQ, !dbg !11
}

define i32 @main() !dbg !12 {
entry:
  %dead = alloca %struct.HP, align 8, !dbg !13
  %hero = alloca %struct.HP, align 8, !dbg !13
  store %struct.HP zeroinitializer, %struct.HP* %dead, align 4, !dbg !13
  %val = getelementptr %struct.HP, %struct.HP* %dead, i32 0, i32 0, !dbg !13
  store i32 0, i32* %val, align 4, !dbg !13
  store %struct.HP zeroinitializer, %struct.HP* %hero, align 4, !dbg !13
  call void @HP.operator.ASSIGN.INT(i32 30, %struct.HP* %hero), !dbg !13
  br label %while.start, !dbg !13

while.start:                                      ; preds = %while.then, %entry
  %0 = call i1 @HP.operator.GT.HP(%struct.HP* %dead, %struct.HP* %hero), !dbg !13
  br i1 %0, label %while.then, label %while.end, !dbg !13

while.then:                                       ; preds = %while.start
  %val1 = getelementptr %struct.HP, %struct.HP* %hero, i32 0, i32 0, !dbg !13
  %val2 = getelementptr %struct.HP, %struct.HP* %hero, i32 0, i32 0, !dbg !13
  %DOT = load i32, i32* %val2, align 4, !dbg !13
  %SUB = sub i32 %DOT, 10, !dbg !13
  store i32 %SUB, i32* %val1, align 4, !dbg !13
  br label %while.start, !dbg !13

while.end:                                        ; preds = %while.start
  br label %if.start, !dbg !13

if.start:                                         ; preds = %while.end
  %1 = call i1 @HP.operator.EQ.HP(%struct.HP* %dead, %struct.HP* %hero), !dbg !13
  br i1 %1, label %if.then, label %if.else, !dbg !13

if.end:                                           ; preds = %if.else, %if.then
  call void @HP.delete(%struct.HP* %dead), !dbg !13
  call void @HP.delete(%struct.HP* %hero), !dbg !13
  ret i32 0, !dbg !13

if.then:                                          ; preds = %if.start
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt, i32 0, i32 0)), !dbg !13
  br label %if.end, !dbg !13

if.else:                                          ; preds = %if.start
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.1, i32 0, i32 0)), !dbg !13
  br label %if.end, !dbg !13
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "007.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/operators/overloading")
!4 = distinct !DISubprogram(name: "HP.operator.ASSIGN.INT", linkageName: "HP.operator.ASSIGN.INT", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = distinct !DISubprogram(name: "HP.operator.GT.HP", linkageName: "HP.operator.GT.HP", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(name: "HP.operator.EQ.HP", linkageName: "HP.operator.EQ.HP", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 0, scope: !10)
!12 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 15, type: !5, scopeLine: 15, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 15, scope: !12)
