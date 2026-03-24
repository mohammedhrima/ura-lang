; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic/003.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Player = type { i8*, i32 }

@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [5 x i8] c"id: \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [8 x i8] c"id: %d\0A\00", align 1

define void @Player.init(%struct.Player* %0) {
entry:
  store %struct.Player zeroinitializer, %struct.Player* %0, align 8
  ret void
}

define void @Player.clean(%struct.Player* %0) {
entry:
  ret void
}

define void @buff_player(%struct.Player* %p) !dbg !4 {
entry:
  %p1 = alloca %struct.Player*, align 8, !dbg !7
  store %struct.Player* %p, %struct.Player** %p1, align 8, !dbg !7
  %p2 = load %struct.Player*, %struct.Player** %p1, align 8, !dbg !7
  %hp = getelementptr %struct.Player, %struct.Player* %p2, i32 0, i32 1, !dbg !7
  %DOT = load i32, i32* %hp, align 4, !dbg !7
  %ADD = add i32 %DOT, 20, !dbg !7
  store i32 %ADD, i32* %hp, align 4, !dbg !7
  ret void, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %hero = alloca %struct.Player, align 8, !dbg !9
  store %struct.Player zeroinitializer, %struct.Player* %hero, align 8, !dbg !9
  call void @Player.init(%struct.Player* %hero), !dbg !9
  %name = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 0, !dbg !9
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !9
  %hp = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 1, !dbg !9
  store i32 80, i32* %hp, align 4, !dbg !9
  call void @buff_player(%struct.Player* %hero), !dbg !10
  %hp1 = getelementptr %struct.Player, %struct.Player* %hero, i32 0, i32 1, !dbg !10
  %DOT = load i32, i32* %hp1, align 4, !dbg !10
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %DOT), !dbg !10
  call void @Player.clean(%struct.Player* %hero), !dbg !10
  ret i32 0, !dbg !10
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/basic")
!4 = distinct !DISubprogram(name: "buff_player", linkageName: "buff_player", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 7, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 10, type: !5, scopeLine: 10, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 10, scope: !8)
!10 = !DILocation(line: 14, scope: !8)
