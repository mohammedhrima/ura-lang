; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic/001.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Player = type { i8*, i32, i32 }
%struct.Enemy = type { i8*, i32, i32 }

@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [10 x i8] c"Orc Grunt\00", align 1
@STR2 = private unnamed_addr constant [8 x i8] c" faces \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [13 x i8] c"%s faces %s\0A\00", align 1
@STR4 = private unnamed_addr constant [10 x i8] c"Hero HP: \00", align 1
@STR5 = private unnamed_addr constant [12 x i8] c" | Orc HP: \00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [26 x i8] c"Hero HP: %d | Orc HP: %d\0A\00", align 1

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

define i32 @main() !dbg !4 {
entry:
  %hero = alloca %struct.Player, align 8, !dbg !7
  %orc = alloca %struct.Enemy, align 8, !dbg !7
  store %struct.Player zeroinitializer, %struct.Player* %hero, align 8, !dbg !7
  call void @Player.init(%struct.Player* %hero), !dbg !7
  store %struct.Enemy zeroinitializer, %struct.Enemy* %orc, align 8, !dbg !7
  call void @Enemy.init(%struct.Enemy* %orc), !dbg !7
  %name = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  %hp = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 1, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  %mp = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 2, !dbg !7
  store i32 50, i32* %mp, align 4, !dbg !7
  %name1 = getelementptr %struct.Enemy, %struct.Enemy* %orc, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR1, i32 0, i32 0), i8** %name1, align 8, !dbg !7
  %hp2 = getelementptr %struct.Enemy, %struct.Enemy* %orc, i32 0, i32 1, !dbg !7
  store i32 60, i32* %hp2, align 4, !dbg !7
  %atk = getelementptr %struct.Enemy, %struct.Enemy* %orc, i32 0, i32 2, !dbg !7
  store i32 14, i32* %atk, align 4, !dbg !7
  %name3 = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 0, !dbg !7
  %DOT = load i8*, i8** %name3, align 8, !dbg !7
  %name4 = getelementptr %struct.Enemy, %struct.Enemy* %orc, i32 0, i32 0, !dbg !7
  %DOT5 = load i8*, i8** %name4, align 8, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @output_fmt, i32 0, i32 0), i32 2, i8* %DOT, i8* %DOT5), !dbg !7
  %hp6 = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 1, !dbg !7
  %DOT7 = load i32, i32* %hp6, align 4, !dbg !7
  %hp8 = getelementptr %struct.Enemy, %struct.Enemy* %orc, i32 0, i32 1, !dbg !7
  %DOT9 = load i32, i32* %hp8, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @output_fmt.1, i32 0, i32 0), i32 2, i32 %DOT7, i32 %DOT9), !dbg !7
  call void @Player.clean(%struct.Player* %hero), !dbg !7
  call void @Enemy.clean(%struct.Enemy* %orc), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 13, scope: !4)
