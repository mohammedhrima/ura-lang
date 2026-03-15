; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods/001.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Hero = type { i8*, i8* }

@STR0 = private unnamed_addr constant [2 x i8] c" \00", align 1
@STR1 = private unnamed_addr constant [10 x i8] c" enters!\0A\00", align 1
@output_fmt = private unnamed_addr constant [15 x i8] c"%s %s enters!\0A\00", align 1
@STR2 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR3 = private unnamed_addr constant [9 x i8] c"The Bold\00", align 1

declare void @free(i8*)

declare i8* @strcpy(i8*, i8*)

define void @Hero.init(%struct.Hero* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Hero*, align 8, !dbg !7
  store %struct.Hero* %self, %struct.Hero** %self1, align 8, !dbg !7
  %self2 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %self3 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %name = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 0, !dbg !7
  %heap = call i8* @calloc(i64 32, i64 1), !dbg !7
  store i8* %heap, i8** %name, align 8, !dbg !7
  %self4 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %title = getelementptr %struct.Hero, %struct.Hero* %self4, i32 0, i32 1, !dbg !7
  %heap5 = call i8* @calloc(i64 32, i64 1), !dbg !7
  store i8* %heap5, i8** %title, align 8, !dbg !7
  ret void, !dbg !7
}

declare i8* @calloc(i64, i64)

define void @Hero.setup(i8* %name, i8* %title, %struct.Hero* %self) !dbg !8 {
entry:
  %name1 = alloca i8*, align 8, !dbg !9
  %title2 = alloca i8*, align 8, !dbg !9
  %self3 = alloca %struct.Hero*, align 8, !dbg !9
  store i8* %name, i8** %name1, align 8, !dbg !9
  store i8* %title, i8** %title2, align 8, !dbg !9
  store %struct.Hero* %self, %struct.Hero** %self3, align 8, !dbg !9
  %self4 = load %struct.Hero*, %struct.Hero** %self3, align 8, !dbg !10
  %name5 = getelementptr %struct.Hero, %struct.Hero* %self4, i32 0, i32 0, !dbg !10
  %DOT = load i8*, i8** %name5, align 8, !dbg !10
  %name6 = load i8*, i8** %name1, align 8, !dbg !10
  %strcpy = call i8* @strcpy(i8* %DOT, i8* %name6), !dbg !10
  %self7 = load %struct.Hero*, %struct.Hero** %self3, align 8, !dbg !11
  %title8 = getelementptr %struct.Hero, %struct.Hero* %self7, i32 0, i32 1, !dbg !11
  %DOT9 = load i8*, i8** %title8, align 8, !dbg !11
  %title10 = load i8*, i8** %title2, align 8, !dbg !11
  %strcpy11 = call i8* @strcpy(i8* %DOT9, i8* %title10), !dbg !11
  ret void, !dbg !11
}

define void @Hero.announce(%struct.Hero* %self) !dbg !12 {
entry:
  %self1 = alloca %struct.Hero*, align 8, !dbg !13
  store %struct.Hero* %self, %struct.Hero** %self1, align 8, !dbg !13
  %self2 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %title = getelementptr %struct.Hero, %struct.Hero* %self2, i32 0, i32 1, !dbg !13
  %DOT = load i8*, i8** %title, align 8, !dbg !13
  %self3 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %name = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 0, !dbg !13
  %DOT4 = load i8*, i8** %name, align 8, !dbg !13
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt, i32 0, i32 0), i32 2, i8* %DOT, i8* %DOT4), !dbg !13
  ret void, !dbg !13
}

declare i32 @printf(i8*, i32, ...)

define void @Hero.clean(%struct.Hero* %self) !dbg !14 {
entry:
  %self1 = alloca %struct.Hero*, align 8, !dbg !15
  store %struct.Hero* %self, %struct.Hero** %self1, align 8, !dbg !15
  %self2 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !16
  %name = getelementptr %struct.Hero, %struct.Hero* %self2, i32 0, i32 0, !dbg !16
  %DOT = load i8*, i8** %name, align 8, !dbg !16
  call void @free(i8* %DOT), !dbg !16
  %self3 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !17
  %title = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 1, !dbg !17
  %DOT4 = load i8*, i8** %title, align 8, !dbg !17
  call void @free(i8* %DOT4), !dbg !17
  %self5 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !17
  ret void, !dbg !17
}

define i32 @main() !dbg !18 {
entry:
  %hero = alloca %struct.Hero, align 8, !dbg !19
  store %struct.Hero zeroinitializer, %struct.Hero* %hero, align 8, !dbg !19
  call void @Hero.init(%struct.Hero* %hero), !dbg !19
  call void @Hero.setup(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR2, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR3, i32 0, i32 0), %struct.Hero* %hero), !dbg !20
  call void @Hero.announce(%struct.Hero* %hero), !dbg !21
  call void @Hero.clean(%struct.Hero* %hero), !dbg !21
  ret i32 0, !dbg !21
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods")
!4 = distinct !DISubprogram(name: "Hero.init", linkageName: "Hero.init", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 9, scope: !4)
!8 = distinct !DISubprogram(name: "Hero.setup", linkageName: "Hero.setup", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 13, scope: !8)
!10 = !DILocation(line: 14, scope: !8)
!11 = !DILocation(line: 15, scope: !8)
!12 = distinct !DISubprogram(name: "Hero.announce", linkageName: "Hero.announce", scope: null, file: !3, line: 17, type: !5, scopeLine: 17, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 17, scope: !12)
!14 = distinct !DISubprogram(name: "Hero.clean", linkageName: "Hero.clean", scope: null, file: !3, line: 20, type: !5, scopeLine: 20, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 20, scope: !14)
!16 = !DILocation(line: 21, scope: !14)
!17 = !DILocation(line: 22, scope: !14)
!18 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 24, type: !5, scopeLine: 24, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 24, scope: !18)
!20 = !DILocation(line: 26, scope: !18)
!21 = !DILocation(line: 27, scope: !18)
