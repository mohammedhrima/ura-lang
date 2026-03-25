; ModuleID = 'tests/dungeon/floor14b.ura'
source_filename = "tests/dungeon/floor14b.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [13 x i8] c"Giant Spider\00", align 1
@STR1 = private unnamed_addr constant [11 x i8] c"Orc Shaman\00", align 1
@STR2 = private unnamed_addr constant [12 x i8] c"Stone Golem\00", align 1
@STR3 = private unnamed_addr constant [14 x i8] c"Shadow Wraith\00", align 1
@STR4 = private unnamed_addr constant [14 x i8] c"Encounter 1: \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [17 x i8] c"Encounter 1: %s\0A\00", align 1
@STR6 = private unnamed_addr constant [14 x i8] c"Encounter 2: \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [17 x i8] c"Encounter 2: %s\0A\00", align 1
@STR8 = private unnamed_addr constant [14 x i8] c"Encounter 3: \00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [17 x i8] c"Encounter 3: %s\0A\00", align 1

declare i32 @rand()

declare void @srand(i32)

define i8* @random_encounter() !dbg !4 {
entry:
  %r = alloca i32, align 4, !dbg !7
  store i32 0, i32* %r, align 4, !dbg !7
  %rand = call i32 @rand(), !dbg !8
  %MOD = srem i32 %rand, 4, !dbg !8
  store i32 %MOD, i32* %r, align 4, !dbg !8
  br label %if.start, !dbg !8

if.start:                                         ; preds = %entry
  %r1 = load i32, i32* %r, align 4, !dbg !8
  %EQ = icmp eq i32 %r1, 0, !dbg !8
  br i1 %EQ, label %if.then, label %if.end, !dbg !8

if.end:                                           ; preds = %if.start
  br label %if.start2, !dbg !8

if.then:                                          ; preds = %if.start
  ret i8* getelementptr inbounds ([13 x i8], [13 x i8]* @STR0, i32 0, i32 0), !dbg !8

if.start2:                                        ; preds = %if.end
  %r5 = load i32, i32* %r, align 4, !dbg !8
  %EQ6 = icmp eq i32 %r5, 1, !dbg !8
  br i1 %EQ6, label %if.then4, label %if.end3, !dbg !8

if.end3:                                          ; preds = %if.start2
  br label %if.start7, !dbg !8

if.then4:                                         ; preds = %if.start2
  ret i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR1, i32 0, i32 0), !dbg !8

if.start7:                                        ; preds = %if.end3
  %r10 = load i32, i32* %r, align 4, !dbg !8
  %EQ11 = icmp eq i32 %r10, 2, !dbg !8
  br i1 %EQ11, label %if.then9, label %if.end8, !dbg !8

if.end8:                                          ; preds = %if.start7
  ret i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR3, i32 0, i32 0), !dbg !8

if.then9:                                         ; preds = %if.start7
  ret i8* getelementptr inbounds ([12 x i8], [12 x i8]* @STR2, i32 0, i32 0), !dbg !8
}

define i32 @main() !dbg !9 {
entry:
  call void @srand(i32 42), !dbg !10
  %random_encounter = call i8* @random_encounter(), !dbg !11
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %random_encounter), !dbg !11
  %random_encounter1 = call i8* @random_encounter(), !dbg !12
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %random_encounter1), !dbg !12
  %random_encounter2 = call i8* @random_encounter(), !dbg !13
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i8* %random_encounter2), !dbg !13
  ret i32 0, !dbg !13
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor14b.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "random_encounter", linkageName: "random_encounter", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 5, scope: !4)
!9 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 12, scope: !9)
!11 = !DILocation(line: 13, scope: !9)
!12 = !DILocation(line: 14, scope: !9)
!13 = !DILocation(line: 15, scope: !9)
