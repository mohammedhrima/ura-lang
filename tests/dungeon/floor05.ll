; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor05.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon/floor05.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [11 x i8] c"Iron Sword\00", align 1
@STR1 = private unnamed_addr constant [15 x i8] c"Floor 3 loot: \00", align 1
@STR2 = private unnamed_addr constant [8 x i8] c" worth \00", align 1
@STR3 = private unnamed_addr constant [7 x i8] c" gold\0A\00", align 1
@output_fmt = private unnamed_addr constant [32 x i8] c"Floor 3 loot: %s worth %d gold\0A\00", align 1
@STR4 = private unnamed_addr constant [13 x i8] c"Bonus gold: \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [16 x i8] c"Bonus gold: %d\0A\00", align 1

define { i8*, i32 } @roll_loot(i32 %f) !dbg !4 {
entry:
  %f1 = alloca i32, align 4, !dbg !7
  %item = alloca i8*, align 8, !dbg !7
  %value = alloca i32, align 4, !dbg !7
  store i32 %f, i32* %f1, align 4, !dbg !7
  store i8* null, i8** %item, align 8, !dbg !7
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR0, i32 0, i32 0), i8** %item, align 8, !dbg !7
  store i32 0, i32* %value, align 4, !dbg !7
  %f2 = load i32, i32* %f1, align 4, !dbg !7
  %MUL = mul i32 %f2, 15, !dbg !7
  store i32 %MUL, i32* %value, align 4, !dbg !7
  %item3 = load i8*, i8** %item, align 8, !dbg !7
  %0 = insertvalue { i8*, i32 } undef, i8* %item3, 0, !dbg !7
  %value4 = load i32, i32* %value, align 4, !dbg !7
  %1 = insertvalue { i8*, i32 } %0, i32 %value4, 1, !dbg !7
  ret { i8*, i32 } %1, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %item = alloca i8*, align 8, !dbg !9
  %gold = alloca i32, align 4, !dbg !9
  %item2 = alloca i8*, align 8, !dbg !9
  %bonus = alloca i32, align 4, !dbg !9
  %roll_loot = call { i8*, i32 } @roll_loot(i32 3), !dbg !9
  store i8* null, i8** %item, align 8, !dbg !9
  %item1 = extractvalue { i8*, i32 } %roll_loot, 0, !dbg !9
  store i8* %item1, i8** %item, align 8, !dbg !9
  store i32 0, i32* %gold, align 4, !dbg !9
  %gold2 = extractvalue { i8*, i32 } %roll_loot, 1, !dbg !9
  store i32 %gold2, i32* %gold, align 4, !dbg !9
  %item3 = load i8*, i8** %item, align 8, !dbg !9
  %gold4 = load i32, i32* %gold, align 4, !dbg !9
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @output_fmt, i32 0, i32 0), i8* %item3, i32 %gold4), !dbg !9
  %roll_loot5 = call { i8*, i32 } @roll_loot(i32 5), !dbg !10
  store i8* null, i8** %item2, align 8, !dbg !9
  %item26 = extractvalue { i8*, i32 } %roll_loot5, 0, !dbg !9
  store i8* %item26, i8** %item2, align 8, !dbg !9
  store i32 0, i32* %bonus, align 4, !dbg !9
  %bonus7 = extractvalue { i8*, i32 } %roll_loot5, 1, !dbg !9
  store i32 %bonus7, i32* %bonus, align 4, !dbg !9
  %bonus8 = load i32, i32* %bonus, align 4, !dbg !9
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.1, i32 0, i32 0), i32 %bonus8), !dbg !9
  ret i32 0, !dbg !9
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor05.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/dungeon")
!4 = distinct !DISubprogram(name: "roll_loot", linkageName: "roll_loot", scope: null, file: !3, line: 2, type: !5, scopeLine: 2, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 2, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 8, scope: !8)
!10 = !DILocation(line: 11, scope: !8)
