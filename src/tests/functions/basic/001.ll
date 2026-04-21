; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [20 x i8] c"=== Entering floor \00", align 1
@STR1 = private unnamed_addr constant [6 x i8] c" ===\0A\00", align 1
@output_fmt = private unnamed_addr constant [27 x i8] c"=== Entering floor %d ===\0A\00", align 1
@STR2 = private unnamed_addr constant [7 x i8] c"BOSS: \00", align 1
@STR3 = private unnamed_addr constant [11 x i8] c" appears!\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [19 x i8] c"BOSS: %s appears!\0A\00", align 1
@STR4 = private unnamed_addr constant [14 x i8] c"The Lich King\00", align 1

define void @announce_floor(i32 %floor) !dbg !4 {
entry:
  %floor1 = alloca i32, align 4, !dbg !7
  store i32 %floor, i32* %floor1, align 4, !dbg !7
  %floor2 = load i32, i32* %floor1, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %floor2), !dbg !7
  ret void, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

define void @announce_boss(i8* %name) !dbg !8 {
entry:
  %name1 = alloca i8*, align 8, !dbg !9
  store i8* %name, i8** %name1, align 8, !dbg !9
  %name2 = load i8*, i8** %name1, align 8, !dbg !9
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %name2), !dbg !9
  ret void, !dbg !9
}

define i32 @main() !dbg !10 {
entry:
  call void @announce_floor(i32 1), !dbg !11
  call void @announce_floor(i32 5), !dbg !12
  call void @announce_boss(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR4, i32 0, i32 0)), !dbg !13
  ret i32 0, !dbg !13
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/basic")
!4 = distinct !DISubprogram(name: "announce_floor", linkageName: "announce_floor", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "announce_boss", linkageName: "announce_boss", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 10, scope: !10)
!12 = !DILocation(line: 11, scope: !10)
!13 = !DILocation(line: 12, scope: !10)
