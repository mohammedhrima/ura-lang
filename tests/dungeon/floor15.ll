; ModuleID = 'tests/dungeon/floor15.ura'
source_filename = "tests/dungeon/floor15.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Os = type { i32, i8** }

@os = global %struct.Os zeroinitializer
@STR0 = private unnamed_addr constant [28 x i8] c"Usage: dungeon <hero_name>\0A\00", align 1
@output_fmt = private unnamed_addr constant [28 x i8] c"Usage: dungeon <hero_name>\0A\00", align 1
@STR1 = private unnamed_addr constant [14 x i8] c"=== Welcome, \00", align 1
@STR2 = private unnamed_addr constant [7 x i8] c"! ===\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [22 x i8] c"=== Welcome, %s! ===\0A\00", align 1
@STR3 = private unnamed_addr constant [31 x i8] c"Your quest begins on floor 1.\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [31 x i8] c"Your quest begins on floor 1.\0A\00", align 1

define void @Os.init(%struct.Os* %0) {
entry:
  store %struct.Os zeroinitializer, %struct.Os* %0, align 8
  ret void
}

define void @Os.clean(%struct.Os* %0) {
entry:
  ret void
}

define void @run() !dbg !4 {
entry:
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %DOT = load i32, i32* getelementptr inbounds (%struct.Os, %struct.Os* @os, i32 0, i32 0), align 4, !dbg !7
  %LT = icmp slt i32 %DOT, 2, !dbg !7
  br i1 %LT, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.start
  %DOT1 = load i8**, i8*** getelementptr inbounds (%struct.Os, %struct.Os* @os, i32 0, i32 1), align 8, !dbg !8
  %ACCESS = getelementptr i8*, i8** %DOT1, i32 1, !dbg !8
  %ACC = load i8*, i8** %ACCESS, align 8, !dbg !8
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %ACC), !dbg !8
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @output_fmt.2, i32 0, i32 0), i32 0), !dbg !8
  ret void, !dbg !8

if.then:                                          ; preds = %if.start
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !7
  ret void, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

define i32 @main(i32 %0, i8** %1) !dbg !9 {
entry:
  call void @Os.init(%struct.Os* @os), !dbg !10
  store i32 %0, i32* getelementptr inbounds (%struct.Os, %struct.Os* @os, i32 0, i32 0), align 4, !dbg !10
  store i8** %1, i8*** getelementptr inbounds (%struct.Os, %struct.Os* @os, i32 0, i32 1), align 8, !dbg !10
  call void @run(), !dbg !11
  call void @Os.clean(%struct.Os* @os), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor15.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "run", linkageName: "run", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
!9 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 11, scope: !9)
!11 = !DILocation(line: 12, scope: !9)
