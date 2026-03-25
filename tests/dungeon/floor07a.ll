; ModuleID = 'tests/dungeon/floor07a.ura'
source_filename = "tests/dungeon/floor07a.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Player = type { i8*, i32, i32 }
%struct.Enemy = type { i8*, i32, i32 }

@STR0 = private unnamed_addr constant [3 x i8] c" (\00", align 1
@STR1 = private unnamed_addr constant [12 x i8] c" HP) faces \00", align 1
@STR2 = private unnamed_addr constant [3 x i8] c" (\00", align 1
@STR3 = private unnamed_addr constant [6 x i8] c" HP)\0A\00", align 1
@output_fmt = private unnamed_addr constant [29 x i8] c"%s (%d HP) faces %s (%d HP)\0A\00", align 1
@STR4 = private unnamed_addr constant [8 x i8] c" wins!\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [10 x i8] c"%s wins!\0A\00", align 1
@STR5 = private unnamed_addr constant [8 x i8] c" wins!\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [10 x i8] c"%s wins!\0A\00", align 1
@STR6 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR7 = private unnamed_addr constant [10 x i8] c"Orc Grunt\00", align 1

define void @Player.init(%struct.Player* %0) {
entry:
  store %struct.Player zeroinitializer, %struct.Player* %0, align 8
  ret void
}

define void @Player.clean(%struct.Player* %0) {
entry:
  ret void
}

define void @Enemy.init(%struct.Enemy* %0) {
entry:
  store %struct.Enemy zeroinitializer, %struct.Enemy* %0, align 8
  ret void
}

define void @Enemy.clean(%struct.Enemy* %0) {
entry:
  ret void
}

define void @fight(%struct.Player %p, %struct.Enemy %e) !dbg !4 {
entry:
  %p1 = alloca %struct.Player, align 8, !dbg !7
  %e2 = alloca %struct.Enemy, align 8, !dbg !7
  store %struct.Player %p, %struct.Player* %p1, align 8, !dbg !7
  store %struct.Enemy %e, %struct.Enemy* %e2, align 8, !dbg !7
  %name = getelementptr %struct.Player, %struct.Player* %p1, i32 0, i32 0, !dbg !7
  %DOT = load i8*, i8** %name, align 8, !dbg !7
  %hp = getelementptr %struct.Player, %struct.Player* %p1, i32 0, i32 1, !dbg !7
  %DOT3 = load i32, i32* %hp, align 4, !dbg !7
  %name4 = getelementptr %struct.Enemy, %struct.Enemy* %e2, i32 0, i32 0, !dbg !7
  %DOT5 = load i8*, i8** %name4, align 8, !dbg !7
  %hp6 = getelementptr %struct.Enemy, %struct.Enemy* %e2, i32 0, i32 1, !dbg !7
  %DOT7 = load i32, i32* %hp6, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @output_fmt, i32 0, i32 0), i32 4, i8* %DOT, i32 %DOT3, i8* %DOT5, i32 %DOT7), !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %hp8 = getelementptr %struct.Player, %struct.Player* %p1, i32 0, i32 1, !dbg !7
  %hp9 = getelementptr %struct.Enemy, %struct.Enemy* %e2, i32 0, i32 1, !dbg !7
  %DOT10 = load i32, i32* %hp8, align 4, !dbg !7
  %DOT11 = load i32, i32* %hp9, align 4, !dbg !7
  %GT = icmp sgt i32 %DOT10, %DOT11, !dbg !7
  br i1 %GT, label %if.then, label %if.else, !dbg !7

if.end:                                           ; preds = %if.else, %if.then
  call void @Player.clean(%struct.Player* %p1), !dbg !7
  call void @Enemy.clean(%struct.Enemy* %e2), !dbg !7
  ret void, !dbg !7

if.then:                                          ; preds = %if.start
  %name12 = getelementptr %struct.Player, %struct.Player* %p1, i32 0, i32 0, !dbg !7
  %DOT13 = load i8*, i8** %name12, align 8, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %DOT13), !dbg !7
  br label %if.end, !dbg !7

if.else:                                          ; preds = %if.start
  %name14 = getelementptr %struct.Enemy, %struct.Enemy* %e2, i32 0, i32 0, !dbg !7
  %DOT15 = load i8*, i8** %name14, align 8, !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i8* %DOT15), !dbg !7
  br label %if.end, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !8 {
entry:
  %hero = alloca %struct.Player, align 8, !dbg !9
  %orc = alloca %struct.Enemy, align 8, !dbg !9
  store %struct.Player zeroinitializer, %struct.Player* %hero, align 8, !dbg !9
  call void @Player.init(%struct.Player* %hero), !dbg !9
  %name = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 0, !dbg !9
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR6, i32 0, i32 0), i8** %name, align 8, !dbg !9
  %hp = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 1, !dbg !9
  store i32 100, i32* %hp, align 4, !dbg !9
  %mp = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 2, !dbg !9
  store i32 50, i32* %mp, align 4, !dbg !9
  store %struct.Enemy zeroinitializer, %struct.Enemy* %orc, align 8, !dbg !9
  call void @Enemy.init(%struct.Enemy* %orc), !dbg !9
  %name1 = getelementptr %struct.Enemy, %struct.Enemy* %orc, i32 0, i32 0, !dbg !9
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR7, i32 0, i32 0), i8** %name1, align 8, !dbg !9
  %hp2 = getelementptr %struct.Enemy, %struct.Enemy* %orc, i32 0, i32 1, !dbg !9
  store i32 60, i32* %hp2, align 4, !dbg !9
  %atk = getelementptr %struct.Enemy, %struct.Enemy* %orc, i32 0, i32 2, !dbg !9
  store i32 14, i32* %atk, align 4, !dbg !9
  %hero3 = load %struct.Player, %struct.Player* %hero, align 8, !dbg !10
  %orc4 = load %struct.Enemy, %struct.Enemy* %orc, align 8, !dbg !10
  call void @fight(%struct.Player %hero3, %struct.Enemy %orc4), !dbg !10
  call void @Player.clean(%struct.Player* %hero), !dbg !10
  call void @Enemy.clean(%struct.Enemy* %orc), !dbg !10
  ret i32 0, !dbg !10
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor07a.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "fight", linkageName: "fight", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 12, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 19, scope: !8)
!10 = !DILocation(line: 30, scope: !8)
