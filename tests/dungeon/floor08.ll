; ModuleID = 'tests/dungeon/floor08.ura'
source_filename = "tests/dungeon/floor08.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Hero = type { i8*, i8*, i32 }

@STR0 = private unnamed_addr constant [2 x i8] c" \00", align 1
@STR1 = private unnamed_addr constant [10 x i8] c" \E2\80\94 HP: \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [18 x i8] c"%s %s \E2\80\94 HP: %d\0A\00", align 1
@STR3 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR4 = private unnamed_addr constant [9 x i8] c"The Bold\00", align 1

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
  %self6 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !7
  %hp = getelementptr %struct.Hero, %struct.Hero* %self6, i32 0, i32 2, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
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
  %self5 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !13
  %hp = getelementptr %struct.Hero, %struct.Hero* %self5, i32 0, i32 2, !dbg !13
  %DOT6 = load i32, i32* %hp, align 4, !dbg !13
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt, i32 0, i32 0), i32 3, i8* %DOT, i8* %DOT4, i32 %DOT6), !dbg !13
  ret void, !dbg !13
}

declare i32 @printf(i8*, i32, ...)

define void @Hero.heal(i32 %amount, %struct.Hero* %self) !dbg !14 {
entry:
  %amount1 = alloca i32, align 4, !dbg !15
  %self2 = alloca %struct.Hero*, align 8, !dbg !15
  store i32 %amount, i32* %amount1, align 4, !dbg !15
  store %struct.Hero* %self, %struct.Hero** %self2, align 8, !dbg !15
  %self3 = load %struct.Hero*, %struct.Hero** %self2, align 8, !dbg !15
  %hp = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 2, !dbg !15
  %self4 = load %struct.Hero*, %struct.Hero** %self2, align 8, !dbg !15
  %hp5 = getelementptr %struct.Hero, %struct.Hero* %self4, i32 0, i32 2, !dbg !15
  %DOT = load i32, i32* %hp5, align 4, !dbg !15
  %amount6 = load i32, i32* %amount1, align 4, !dbg !15
  %ADD = add i32 %DOT, %amount6, !dbg !15
  store i32 %ADD, i32* %hp, align 4, !dbg !15
  ret void, !dbg !15
}

define void @Hero.clean(%struct.Hero* %self) !dbg !16 {
entry:
  %self1 = alloca %struct.Hero*, align 8, !dbg !17
  store %struct.Hero* %self, %struct.Hero** %self1, align 8, !dbg !17
  %self2 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !18
  %name = getelementptr %struct.Hero, %struct.Hero* %self2, i32 0, i32 0, !dbg !18
  %DOT = load i8*, i8** %name, align 8, !dbg !18
  call void @free(i8* %DOT), !dbg !18
  %self3 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !19
  %title = getelementptr %struct.Hero, %struct.Hero* %self3, i32 0, i32 1, !dbg !19
  %DOT4 = load i8*, i8** %title, align 8, !dbg !19
  call void @free(i8* %DOT4), !dbg !19
  %self5 = load %struct.Hero*, %struct.Hero** %self1, align 8, !dbg !19
  ret void, !dbg !19
}

define i32 @main() !dbg !20 {
entry:
  %hero = alloca %struct.Hero, align 8, !dbg !21
  store %struct.Hero zeroinitializer, %struct.Hero* %hero, align 8, !dbg !21
  call void @Hero.init(%struct.Hero* %hero), !dbg !21
  call void @Hero.setup(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR3, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR4, i32 0, i32 0), %struct.Hero* %hero), !dbg !22
  call void @Hero.announce(%struct.Hero* %hero), !dbg !23
  call void @Hero.heal(i32 20, %struct.Hero* %hero), !dbg !24
  call void @Hero.announce(%struct.Hero* %hero), !dbg !25
  call void @Hero.clean(%struct.Hero* %hero), !dbg !25
  ret i32 0, !dbg !25
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor08.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "Hero.init", linkageName: "Hero.init", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 9, scope: !4)
!8 = distinct !DISubprogram(name: "Hero.setup", linkageName: "Hero.setup", scope: null, file: !3, line: 14, type: !5, scopeLine: 14, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 14, scope: !8)
!10 = !DILocation(line: 15, scope: !8)
!11 = !DILocation(line: 16, scope: !8)
!12 = distinct !DISubprogram(name: "Hero.announce", linkageName: "Hero.announce", scope: null, file: !3, line: 18, type: !5, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 18, scope: !12)
!14 = distinct !DISubprogram(name: "Hero.heal", linkageName: "Hero.heal", scope: null, file: !3, line: 21, type: !5, scopeLine: 21, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 21, scope: !14)
!16 = distinct !DISubprogram(name: "Hero.clean", linkageName: "Hero.clean", scope: null, file: !3, line: 24, type: !5, scopeLine: 24, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 24, scope: !16)
!18 = !DILocation(line: 25, scope: !16)
!19 = !DILocation(line: 26, scope: !16)
!20 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 28, type: !5, scopeLine: 28, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 28, scope: !20)
!22 = !DILocation(line: 30, scope: !20)
!23 = !DILocation(line: 31, scope: !20)
!24 = !DILocation(line: 32, scope: !20)
!25 = !DILocation(line: 33, scope: !20)
