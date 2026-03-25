; ModuleID = 'tests/dungeon/floor13b.ura'
source_filename = "tests/dungeon/floor13b.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Label = type { i8*, i32 }

@STR0 = private unnamed_addr constant [3 x i8] c" (\00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c" chars)\0A\00", align 1
@output_fmt = private unnamed_addr constant [15 x i8] c"%s (%d chars)\0A\00", align 1
@STR2 = private unnamed_addr constant [7 x i8] c"Shadow\00", align 1
@STR3 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1

declare void @free(i8*)

declare i32 @strlen(i8*)

declare i8* @strcpy(i8*, i8*)

declare i8* @strcat(i8*, i8*)

define void @Label.init(%struct.Label* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Label*, align 8, !dbg !7
  store %struct.Label* %self, %struct.Label** %self1, align 8, !dbg !7
  %self2 = load %struct.Label*, %struct.Label** %self1, align 8, !dbg !7
  %self3 = load %struct.Label*, %struct.Label** %self1, align 8, !dbg !7
  %value = getelementptr %struct.Label, %struct.Label* %self3, i32 0, i32 0, !dbg !7
  %heap = call i8* @calloc(i64 64, i64 1), !dbg !7
  store i8* %heap, i8** %value, align 8, !dbg !7
  %self4 = load %struct.Label*, %struct.Label** %self1, align 8, !dbg !7
  %len = getelementptr %struct.Label, %struct.Label* %self4, i32 0, i32 1, !dbg !7
  store i32 0, i32* %len, align 4, !dbg !7
  ret void, !dbg !7
}

declare i8* @calloc(i64, i64)

define void @Label.clean(%struct.Label* %self) !dbg !8 {
entry:
  %self1 = alloca %struct.Label*, align 8, !dbg !9
  store %struct.Label* %self, %struct.Label** %self1, align 8, !dbg !9
  %self2 = load %struct.Label*, %struct.Label** %self1, align 8, !dbg !10
  %value = getelementptr %struct.Label, %struct.Label* %self2, i32 0, i32 0, !dbg !10
  %DOT = load i8*, i8** %value, align 8, !dbg !10
  call void @free(i8* %DOT), !dbg !10
  %self3 = load %struct.Label*, %struct.Label** %self1, align 8, !dbg !10
  ret void, !dbg !10
}

define void @Label.set(i8* %s, %struct.Label* %self) !dbg !11 {
entry:
  %s1 = alloca i8*, align 8, !dbg !12
  %self2 = alloca %struct.Label*, align 8, !dbg !12
  store i8* %s, i8** %s1, align 8, !dbg !12
  store %struct.Label* %self, %struct.Label** %self2, align 8, !dbg !12
  %self3 = load %struct.Label*, %struct.Label** %self2, align 8, !dbg !13
  %value = getelementptr %struct.Label, %struct.Label* %self3, i32 0, i32 0, !dbg !13
  %DOT = load i8*, i8** %value, align 8, !dbg !13
  %s4 = load i8*, i8** %s1, align 8, !dbg !13
  %strcpy = call i8* @strcpy(i8* %DOT, i8* %s4), !dbg !13
  %self5 = load %struct.Label*, %struct.Label** %self2, align 8, !dbg !13
  %len = getelementptr %struct.Label, %struct.Label* %self5, i32 0, i32 1, !dbg !13
  %s6 = load i8*, i8** %s1, align 8, !dbg !14
  %strlen = call i32 @strlen(i8* %s6), !dbg !14
  store i32 %strlen, i32* %len, align 4, !dbg !14
  ret void, !dbg !14
}

define void @Label.operator.ASSIGN.CHARS(i8* %s, %struct.Label* %self) !dbg !15 {
entry:
  %s1 = alloca i8*, align 8, !dbg !16
  %self2 = alloca %struct.Label*, align 8, !dbg !16
  store i8* %s, i8** %s1, align 8, !dbg !16
  store %struct.Label* %self, %struct.Label** %self2, align 8, !dbg !16
  %s3 = load i8*, i8** %s1, align 8, !dbg !17
  %ref_arg = load %struct.Label*, %struct.Label** %self2, align 8, !dbg !17
  call void @Label.set(i8* %s3, %struct.Label* %ref_arg), !dbg !17
  ret void, !dbg !17
}

define void @Label.operator.ASSIGN.Label(%struct.Label* %v, %struct.Label* %self) !dbg !18 {
entry:
  %v1 = alloca %struct.Label*, align 8, !dbg !19
  %self2 = alloca %struct.Label*, align 8, !dbg !19
  store %struct.Label* %v, %struct.Label** %v1, align 8, !dbg !19
  store %struct.Label* %self, %struct.Label** %self2, align 8, !dbg !19
  %v3 = load %struct.Label*, %struct.Label** %v1, align 8, !dbg !20
  %value = getelementptr %struct.Label, %struct.Label* %v3, i32 0, i32 0, !dbg !20
  %DOT = load i8*, i8** %value, align 8, !dbg !20
  %ref_arg = load %struct.Label*, %struct.Label** %self2, align 8, !dbg !20
  call void @Label.set(i8* %DOT, %struct.Label* %ref_arg), !dbg !20
  ret void, !dbg !20
}

define %struct.Label @Label.operator.ADD.Label(%struct.Label* %v, %struct.Label* %self) !dbg !21 {
entry:
  %v1 = alloca %struct.Label*, align 8, !dbg !22
  %self2 = alloca %struct.Label*, align 8, !dbg !22
  %res = alloca %struct.Label, align 8, !dbg !22
  store %struct.Label* %v, %struct.Label** %v1, align 8, !dbg !22
  store %struct.Label* %self, %struct.Label** %self2, align 8, !dbg !22
  store %struct.Label zeroinitializer, %struct.Label* %res, align 8, !dbg !22
  call void @Label.init(%struct.Label* %res), !dbg !22
  %value = getelementptr %struct.Label, %struct.Label* %res, i32 0, i32 0, !dbg !23
  %DOT = load i8*, i8** %value, align 8, !dbg !23
  %self3 = load %struct.Label*, %struct.Label** %self2, align 8, !dbg !23
  %value4 = getelementptr %struct.Label, %struct.Label* %self3, i32 0, i32 0, !dbg !23
  %DOT5 = load i8*, i8** %value4, align 8, !dbg !23
  %strcpy = call i8* @strcpy(i8* %DOT, i8* %DOT5), !dbg !23
  %value6 = getelementptr %struct.Label, %struct.Label* %res, i32 0, i32 0, !dbg !24
  %DOT7 = load i8*, i8** %value6, align 8, !dbg !24
  %v8 = load %struct.Label*, %struct.Label** %v1, align 8, !dbg !24
  %value9 = getelementptr %struct.Label, %struct.Label* %v8, i32 0, i32 0, !dbg !24
  %DOT10 = load i8*, i8** %value9, align 8, !dbg !24
  %strcat = call i8* @strcat(i8* %DOT7, i8* %DOT10), !dbg !24
  %len = getelementptr %struct.Label, %struct.Label* %res, i32 0, i32 1, !dbg !24
  %self11 = load %struct.Label*, %struct.Label** %self2, align 8, !dbg !24
  %len12 = getelementptr %struct.Label, %struct.Label* %self11, i32 0, i32 1, !dbg !24
  %v13 = load %struct.Label*, %struct.Label** %v1, align 8, !dbg !24
  %len14 = getelementptr %struct.Label, %struct.Label* %v13, i32 0, i32 1, !dbg !24
  %DOT15 = load i32, i32* %len12, align 4, !dbg !24
  %DOT16 = load i32, i32* %len14, align 4, !dbg !24
  %ADD = add i32 %DOT15, %DOT16, !dbg !24
  store i32 %ADD, i32* %len, align 4, !dbg !24
  %res17 = load %struct.Label, %struct.Label* %res, align 8, !dbg !24
  ret %struct.Label %res17, !dbg !24
}

define void @Label.print(%struct.Label* %self) !dbg !25 {
entry:
  %self1 = alloca %struct.Label*, align 8, !dbg !26
  store %struct.Label* %self, %struct.Label** %self1, align 8, !dbg !26
  %self2 = load %struct.Label*, %struct.Label** %self1, align 8, !dbg !26
  %value = getelementptr %struct.Label, %struct.Label* %self2, i32 0, i32 0, !dbg !26
  %DOT = load i8*, i8** %value, align 8, !dbg !26
  %self3 = load %struct.Label*, %struct.Label** %self1, align 8, !dbg !26
  %len = getelementptr %struct.Label, %struct.Label* %self3, i32 0, i32 1, !dbg !26
  %DOT4 = load i32, i32* %len, align 4, !dbg !26
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt, i32 0, i32 0), i32 2, i8* %DOT, i32 %DOT4), !dbg !26
  ret void, !dbg !26
}

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !27 {
entry:
  %title = alloca %struct.Label, align 8, !dbg !28
  %name = alloca %struct.Label, align 8, !dbg !28
  %full = alloca %struct.Label, align 8, !dbg !28
  store %struct.Label zeroinitializer, %struct.Label* %title, align 8, !dbg !28
  call void @Label.init(%struct.Label* %title), !dbg !28
  call void @Label.operator.ASSIGN.CHARS(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR2, i32 0, i32 0), %struct.Label* %title), !dbg !28
  store %struct.Label zeroinitializer, %struct.Label* %name, align 8, !dbg !28
  call void @Label.init(%struct.Label* %name), !dbg !28
  call void @Label.operator.ASSIGN.CHARS(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR3, i32 0, i32 0), %struct.Label* %name), !dbg !28
  store %struct.Label zeroinitializer, %struct.Label* %full, align 8, !dbg !28
  call void @Label.init(%struct.Label* %full), !dbg !28
  %0 = call %struct.Label @Label.operator.ADD.Label(%struct.Label* %name, %struct.Label* %title), !dbg !28
  %tmp_op = alloca %struct.Label, align 8, !dbg !28
  store %struct.Label %0, %struct.Label* %tmp_op, align 8, !dbg !28
  call void @Label.operator.ASSIGN.Label(%struct.Label* %tmp_op, %struct.Label* %full), !dbg !28
  call void @Label.print(%struct.Label* %full), !dbg !29
  call void @Label.clean(%struct.Label* %title), !dbg !29
  call void @Label.clean(%struct.Label* %name), !dbg !29
  call void @Label.clean(%struct.Label* %full), !dbg !29
  ret i32 0, !dbg !29
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor13b.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "Label.init", linkageName: "Label.init", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
!8 = distinct !DISubprogram(name: "Label.clean", linkageName: "Label.clean", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 12, scope: !8)
!10 = !DILocation(line: 13, scope: !8)
!11 = distinct !DISubprogram(name: "Label.set", linkageName: "Label.set", scope: null, file: !3, line: 15, type: !5, scopeLine: 15, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!12 = !DILocation(line: 15, scope: !11)
!13 = !DILocation(line: 16, scope: !11)
!14 = !DILocation(line: 17, scope: !11)
!15 = distinct !DISubprogram(name: "Label.operator.ASSIGN.CHARS", linkageName: "Label.operator.ASSIGN.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!16 = !DILocation(line: 0, scope: !15)
!17 = !DILocation(line: 20, scope: !15)
!18 = distinct !DISubprogram(name: "Label.operator.ASSIGN.Label", linkageName: "Label.operator.ASSIGN.Label", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 0, scope: !18)
!20 = !DILocation(line: 23, scope: !18)
!21 = distinct !DISubprogram(name: "Label.operator.ADD.Label", linkageName: "Label.operator.ADD.Label", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!22 = !DILocation(line: 0, scope: !21)
!23 = !DILocation(line: 27, scope: !21)
!24 = !DILocation(line: 28, scope: !21)
!25 = distinct !DISubprogram(name: "Label.print", linkageName: "Label.print", scope: null, file: !3, line: 32, type: !5, scopeLine: 32, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!26 = !DILocation(line: 32, scope: !25)
!27 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 35, type: !5, scopeLine: 35, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!28 = !DILocation(line: 35, scope: !27)
!29 = !DILocation(line: 43, scope: !27)
