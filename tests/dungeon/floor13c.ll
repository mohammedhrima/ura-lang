; ModuleID = 'tests/dungeon/floor13c.ura'
source_filename = "tests/dungeon/floor13c.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Label = type { i8*, i32 }

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c"inside: \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [12 x i8] c"inside: %s\0A\00", align 1
@STR3 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c" the Brave\00", align 1
@STR5 = private unnamed_addr constant [9 x i8] c"after:  \00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [12 x i8] c"after:  %s\0A\00", align 1

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

define void @Label.print(%struct.Label* %self) !dbg !21 {
entry:
  %self1 = alloca %struct.Label*, align 8, !dbg !22
  store %struct.Label* %self, %struct.Label** %self1, align 8, !dbg !22
  %self2 = load %struct.Label*, %struct.Label** %self1, align 8, !dbg !22
  %value = getelementptr %struct.Label, %struct.Label* %self2, i32 0, i32 0, !dbg !22
  %DOT = load i8*, i8** %value, align 8, !dbg !22
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %DOT), !dbg !22
  ret void, !dbg !22
}

declare i32 @printf(i8*, i32, ...)

define void @hero_rename(%struct.Label %hero, i8* %suffix) !dbg !23 {
entry:
  %hero1 = alloca %struct.Label, align 8, !dbg !24
  %suffix2 = alloca i8*, align 8, !dbg !24
  store %struct.Label %hero, %struct.Label* %hero1, align 8, !dbg !24
  store i8* %suffix, i8** %suffix2, align 8, !dbg !24
  %value = getelementptr %struct.Label, %struct.Label* %hero1, i32 0, i32 0, !dbg !25
  %DOT = load i8*, i8** %value, align 8, !dbg !25
  %suffix3 = load i8*, i8** %suffix2, align 8, !dbg !25
  %strcat = call i8* @strcat(i8* %DOT, i8* %suffix3), !dbg !25
  %value4 = getelementptr %struct.Label, %struct.Label* %hero1, i32 0, i32 0, !dbg !25
  %DOT5 = load i8*, i8** %value4, align 8, !dbg !25
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %DOT5), !dbg !25
  call void @Label.clean(%struct.Label* %hero1), !dbg !25
  ret void, !dbg !25
}

define i32 @main() !dbg !26 {
entry:
  %hero = alloca %struct.Label, align 8, !dbg !27
  store %struct.Label zeroinitializer, %struct.Label* %hero, align 8, !dbg !27
  call void @Label.init(%struct.Label* %hero), !dbg !27
  call void @Label.operator.ASSIGN.CHARS(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR3, i32 0, i32 0), %struct.Label* %hero), !dbg !27
  %copy = alloca %struct.Label, align 8, !dbg !28
  store %struct.Label zeroinitializer, %struct.Label* %copy, align 8, !dbg !28
  call void @Label.init(%struct.Label* %copy), !dbg !28
  call void @Label.operator.ASSIGN.Label(%struct.Label* %hero, %struct.Label* %copy), !dbg !28
  %copy_val = load %struct.Label, %struct.Label* %copy, align 8, !dbg !28
  call void @hero_rename(%struct.Label %copy_val, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR4, i32 0, i32 0)), !dbg !28
  %value = getelementptr %struct.Label, %struct.Label* %hero, i32 0, i32 0, !dbg !28
  %DOT = load i8*, i8** %value, align 8, !dbg !28
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i8* %DOT), !dbg !28
  call void @Label.clean(%struct.Label* %hero), !dbg !28
  ret i32 0, !dbg !28
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor13c.ura", directory: "tests/dungeon")
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
!21 = distinct !DISubprogram(name: "Label.print", linkageName: "Label.print", scope: null, file: !3, line: 25, type: !5, scopeLine: 25, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!22 = !DILocation(line: 25, scope: !21)
!23 = distinct !DISubprogram(name: "hero_rename", linkageName: "hero_rename", scope: null, file: !3, line: 28, type: !5, scopeLine: 28, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!24 = !DILocation(line: 28, scope: !23)
!25 = !DILocation(line: 29, scope: !23)
!26 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 32, type: !5, scopeLine: 32, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!27 = !DILocation(line: 32, scope: !26)
!28 = !DILocation(line: 35, scope: !26)
