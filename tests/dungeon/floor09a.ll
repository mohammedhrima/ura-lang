; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor09a.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor09a.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [11 x i8] c"\E2\98\85 Level \00", align 1
@STR1 = private unnamed_addr constant [15 x i8] c"! HP+20 MP+10\0A\00", align 1
@output_fmt = private unnamed_addr constant [27 x i8] c"\E2\98\85 Level %d! HP+20 MP+10\0A\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"Stats: HP=\00", align 1
@STR3 = private unnamed_addr constant [5 x i8] c" MP=\00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [20 x i8] c"Stats: HP=%d MP=%d\0A\00", align 1
@STR5 = private unnamed_addr constant [3 x i8] c"x=\00", align 1
@STR6 = private unnamed_addr constant [4 x i8] c" y=\00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [11 x i8] c"x=%d y=%d\0A\00", align 1

define void @level_up(i32* %level, i32* %hp, i32* %mp) !dbg !4 {
entry:
  %level1 = alloca i32*, align 8, !dbg !7
  %hp2 = alloca i32*, align 8, !dbg !7
  %mp3 = alloca i32*, align 8, !dbg !7
  store i32* %level, i32** %level1, align 8, !dbg !7
  store i32* %hp, i32** %hp2, align 8, !dbg !7
  store i32* %mp, i32** %mp3, align 8, !dbg !7
  %ref_ptr = load i32*, i32** %level1, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !7
  %ADD = add i32 %ref_val, 1, !dbg !7
  %ref_ptr4 = load i32*, i32** %level1, align 8, !dbg !7
  store i32 %ADD, i32* %ref_ptr4, align 4, !dbg !7
  %ref_ptr5 = load i32*, i32** %hp2, align 8, !dbg !7
  %ref_val6 = load i32, i32* %ref_ptr5, align 4, !dbg !7
  %ADD7 = add i32 %ref_val6, 20, !dbg !7
  %ref_ptr8 = load i32*, i32** %hp2, align 8, !dbg !7
  store i32 %ADD7, i32* %ref_ptr8, align 4, !dbg !7
  %ref_ptr9 = load i32*, i32** %mp3, align 8, !dbg !7
  %ref_val10 = load i32, i32* %ref_ptr9, align 4, !dbg !7
  %ADD11 = add i32 %ref_val10, 10, !dbg !7
  %ref_ptr12 = load i32*, i32** %mp3, align 8, !dbg !7
  store i32 %ADD11, i32* %ref_ptr12, align 4, !dbg !7
  %ref_ptr13 = load i32*, i32** %level1, align 8, !dbg !7
  %ref_val14 = load i32, i32* %ref_ptr13, align 4, !dbg !7
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @output_fmt, i32 0, i32 0), i32 %ref_val14), !dbg !7
  ret void, !dbg !7
}

define void @swap(i32* %a, i32* %b) !dbg !8 {
entry:
  %a1 = alloca i32*, align 8, !dbg !9
  %b2 = alloca i32*, align 8, !dbg !9
  %tmp = alloca i32, align 4, !dbg !9
  store i32* %a, i32** %a1, align 8, !dbg !9
  store i32* %b, i32** %b2, align 8, !dbg !9
  store i32 0, i32* %tmp, align 4, !dbg !9
  %ref_ptr = load i32*, i32** %a1, align 8, !dbg !9
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !9
  store i32 %ref_val, i32* %tmp, align 4, !dbg !9
  %ref_ptr3 = load i32*, i32** %b2, align 8, !dbg !9
  %ref_val4 = load i32, i32* %ref_ptr3, align 4, !dbg !9
  %ref_ptr5 = load i32*, i32** %a1, align 8, !dbg !9
  store i32 %ref_val4, i32* %ref_ptr5, align 4, !dbg !9
  %tmp6 = load i32, i32* %tmp, align 4, !dbg !9
  %ref_ptr7 = load i32*, i32** %b2, align 8, !dbg !9
  store i32 %tmp6, i32* %ref_ptr7, align 4, !dbg !9
  ret void, !dbg !9
}

define i32 @main() !dbg !10 {
entry:
  %lv = alloca i32, align 4, !dbg !11
  %hp = alloca i32, align 4, !dbg !11
  %mp = alloca i32, align 4, !dbg !11
  %x = alloca i32, align 4, !dbg !11
  %y = alloca i32, align 4, !dbg !11
  store i32 0, i32* %lv, align 4, !dbg !11
  store i32 1, i32* %lv, align 4, !dbg !11
  store i32 0, i32* %hp, align 4, !dbg !11
  store i32 100, i32* %hp, align 4, !dbg !11
  store i32 0, i32* %mp, align 4, !dbg !11
  store i32 50, i32* %mp, align 4, !dbg !11
  call void @level_up(i32* %lv, i32* %hp, i32* %mp), !dbg !12
  %hp1 = load i32, i32* %hp, align 4, !dbg !12
  %mp2 = load i32, i32* %mp, align 4, !dbg !12
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @output_fmt.1, i32 0, i32 0), i32 %hp1, i32 %mp2), !dbg !12
  store i32 0, i32* %x, align 4, !dbg !11
  store i32 3, i32* %x, align 4, !dbg !11
  store i32 0, i32* %y, align 4, !dbg !11
  store i32 7, i32* %y, align 4, !dbg !11
  call void @swap(i32* %x, i32* %y), !dbg !13
  %x3 = load i32, i32* %x, align 4, !dbg !13
  %y4 = load i32, i32* %y, align 4, !dbg !13
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.2, i32 0, i32 0), i32 %x3, i32 %y4), !dbg !13
  ret i32 0, !dbg !13
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor09a.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon")
!4 = distinct !DISubprogram(name: "level_up", linkageName: "level_up", scope: null, file: !3, line: 2, type: !5, scopeLine: 2, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 2, scope: !4)
!8 = distinct !DISubprogram(name: "swap", linkageName: "swap", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 8, scope: !8)
!10 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 13, scope: !10)
!12 = !DILocation(line: 17, scope: !10)
!13 = !DILocation(line: 22, scope: !10)
