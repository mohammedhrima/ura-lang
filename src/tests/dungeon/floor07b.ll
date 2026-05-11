; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor07b.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor07b.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Room = type { i8*, i32 }
%struct.Dungeon = type { i8*, %struct.Room }

@STR0 = private unnamed_addr constant [10 x i8] c"The Abyss\00", align 1
@STR1 = private unnamed_addr constant [10 x i8] c"Gate Hall\00", align 1
@STR2 = private unnamed_addr constant [12 x i8] c"Welcome to \00", align 1
@STR3 = private unnamed_addr constant [16 x i8] c" \E2\80\94 enter via \00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [32 x i8] c"Welcome to %s \E2\80\94 enter via %s\0A\00", align 1

define void @Room.delete(%struct.Room* %0) {
entry:
  ret void
}

define void @Dungeon.delete(%struct.Dungeon* %0) {
entry:
  %entry1 = getelementptr %struct.Dungeon, %struct.Dungeon* %0, i32 0, i32 1
  call void @Room.delete(%struct.Room* %entry1)
  ret void
}

define i32 @main() !dbg !4 {
entry:
  %d = alloca %struct.Dungeon, align 8, !dbg !7
  store %struct.Dungeon zeroinitializer, %struct.Dungeon* %d, align 8, !dbg !7
  %name = getelementptr %struct.Dungeon, %struct.Dungeon* %d, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  %entry1 = getelementptr %struct.Dungeon, %struct.Dungeon* %d, i32 0, i32 1, !dbg !7
  %name2 = getelementptr %struct.Room, %struct.Room* %entry1, i32 0, i32 0, !dbg !7
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR1, i32 0, i32 0), i8** %name2, align 8, !dbg !7
  %entry3 = getelementptr %struct.Dungeon, %struct.Dungeon* %d, i32 0, i32 1, !dbg !7
  %floor = getelementptr %struct.Room, %struct.Room* %entry3, i32 0, i32 1, !dbg !7
  store i32 1, i32* %floor, align 4, !dbg !7
  %name4 = getelementptr %struct.Dungeon, %struct.Dungeon* %d, i32 0, i32 0, !dbg !7
  %DOT = load i8*, i8** %name4, align 8, !dbg !7
  %entry5 = getelementptr %struct.Dungeon, %struct.Dungeon* %d, i32 0, i32 1, !dbg !7
  %name6 = getelementptr %struct.Room, %struct.Room* %entry5, i32 0, i32 0, !dbg !7
  %DOT7 = load i8*, i8** %name6, align 8, !dbg !7
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @output_fmt, i32 0, i32 0), i8* %DOT, i8* %DOT7), !dbg !7
  call void @Dungeon.delete(%struct.Dungeon* %d), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor07b.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 10, type: !5, scopeLine: 10, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 10, scope: !4)
