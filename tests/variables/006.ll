; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [11 x i8] c"Iron Sword\00", align 1
@STR1 = private unnamed_addr constant [12 x i8] c"Magic Staff\00", align 1
@STR2 = private unnamed_addr constant [8 x i8] c"Found: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [11 x i8] c"Found: %s\0A\00", align 1
@STR4 = private unnamed_addr constant [16 x i8] c"Chest is empty\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [16 x i8] c"Chest is empty\0A\00", align 1
@STR5 = private unnamed_addr constant [8 x i8] c"Found: \00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [11 x i8] c"Found: %s\0A\00", align 1
@STR7 = private unnamed_addr constant [16 x i8] c"Chest is empty\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [16 x i8] c"Chest is empty\0A\00", align 1

define i8* @find_weapon(i32 %chest) !dbg !4 {
entry:
  %chest1 = alloca i32, align 4, !dbg !7
  store i32 %chest, i32* %chest1, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %chest2 = load i32, i32* %chest1, align 4, !dbg !7
  %EQ = icmp eq i32 %chest2, 1, !dbg !7
  br i1 %EQ, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.start
  br label %if.start3, !dbg !7

if.then:                                          ; preds = %if.start
  ret i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR0, i32 0, i32 0), !dbg !7

if.start3:                                        ; preds = %if.end
  %chest6 = load i32, i32* %chest1, align 4, !dbg !7
  %EQ7 = icmp eq i32 %chest6, 2, !dbg !7
  br i1 %EQ7, label %if.then5, label %if.end4, !dbg !7

if.end4:                                          ; preds = %if.start3
  ret i8* null, !dbg !7

if.then5:                                         ; preds = %if.start3
  ret i8* getelementptr inbounds ([12 x i8], [12 x i8]* @STR1, i32 0, i32 0), !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %loot = alloca i8*, align 8, !dbg !9
  store i8* null, i8** %loot, align 8, !dbg !9
  %find_weapon = call i8* @find_weapon(i32 1), !dbg !10
  store i8* %find_weapon, i8** %loot, align 8, !dbg !10
  br label %if.start, !dbg !10

if.start:                                         ; preds = %entry
  %loot1 = load i8*, i8** %loot, align 8, !dbg !10
  %NEQ = icmp ne i8* %loot1, null, !dbg !10
  br i1 %NEQ, label %if.then, label %if.else, !dbg !10

if.end:                                           ; preds = %if.else, %if.then
  %find_weapon3 = call i8* @find_weapon(i32 99), !dbg !11
  store i8* %find_weapon3, i8** %loot, align 8, !dbg !11
  br label %if.start4, !dbg !11

if.then:                                          ; preds = %if.start
  %loot2 = load i8*, i8** %loot, align 8, !dbg !10
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %loot2), !dbg !10
  br label %if.end, !dbg !10

if.else:                                          ; preds = %if.start
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !10
  br label %if.end, !dbg !10

if.start4:                                        ; preds = %if.end
  %loot8 = load i8*, i8** %loot, align 8, !dbg !11
  %NEQ9 = icmp ne i8* %loot8, null, !dbg !11
  br i1 %NEQ9, label %if.then6, label %if.else7, !dbg !11

if.end5:                                          ; preds = %if.else7, %if.then6
  ret i32 0, !dbg !11

if.then6:                                         ; preds = %if.start4
  %loot10 = load i8*, i8** %loot, align 8, !dbg !11
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i8* %loot10), !dbg !11
  br label %if.end5, !dbg !11

if.else7:                                         ; preds = %if.start4
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.3, i32 0, i32 0), i32 0), !dbg !11
  br label %if.end5, !dbg !11
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "find_weapon", linkageName: "find_weapon", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 10, type: !5, scopeLine: 10, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 10, scope: !8)
!10 = !DILocation(line: 11, scope: !8)
!11 = !DILocation(line: 17, scope: !8)
