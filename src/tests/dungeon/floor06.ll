; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/dungeon/floor06.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/dungeon/floor06.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [18 x i8] c"Victory in round \00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c"!\0A\00", align 1
@output_fmt = private unnamed_addr constant [22 x i8] c"Victory in round %d!\0A\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"Stalemate\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [11 x i8] c"Stalemate\0A\00", align 1
@STR3 = private unnamed_addr constant [20 x i8] c"Hero fell in round \00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [23 x i8] c"Hero fell in round %d\0A\00", align 1

define void @simulate(i32 %hero_hp, i32 %orc_hp) !dbg !4 {
entry:
  %hero_hp1 = alloca i32, align 4, !dbg !7
  %orc_hp2 = alloca i32, align 4, !dbg !7
  %round = alloca i32, align 4, !dbg !7
  store i32 %hero_hp, i32* %hero_hp1, align 4, !dbg !7
  store i32 %orc_hp, i32* %orc_hp2, align 4, !dbg !7
  store i32 0, i32* %round, align 4, !dbg !7
  store i32 0, i32* %round, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %if.end8, %entry
  %hero_hp3 = load i32, i32* %hero_hp1, align 4, !dbg !7
  %GT = icmp sgt i32 %hero_hp3, 0, !dbg !7
  br i1 %GT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %round4 = load i32, i32* %round, align 4, !dbg !7
  %ADD = add i32 %round4, 1, !dbg !7
  store i32 %ADD, i32* %round, align 4, !dbg !7
  br label %if.start, !dbg !7

while.end:                                        ; preds = %if.then9, %while.start
  br label %if.start16, !dbg !7

if.start:                                         ; preds = %while.then
  %orc_hp5 = load i32, i32* %orc_hp2, align 4, !dbg !7
  %LE = icmp sle i32 %orc_hp5, 0, !dbg !7
  br i1 %LE, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.start
  br label %if.start7, !dbg !7

if.then:                                          ; preds = %if.start
  %round6 = load i32, i32* %round, align 4, !dbg !7
  %SUB = sub i32 %round6, 1, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %SUB), !dbg !7
  ret void, !dbg !7

if.start7:                                        ; preds = %if.end
  %round10 = load i32, i32* %round, align 4, !dbg !7
  %GT11 = icmp sgt i32 %round10, 20, !dbg !7
  br i1 %GT11, label %if.then9, label %if.end8, !dbg !7

if.end8:                                          ; preds = %if.start7
  %hero_hp12 = load i32, i32* %hero_hp1, align 4, !dbg !7
  %SUB13 = sub i32 %hero_hp12, 10, !dbg !7
  store i32 %SUB13, i32* %hero_hp1, align 4, !dbg !7
  %orc_hp14 = load i32, i32* %orc_hp2, align 4, !dbg !7
  %SUB15 = sub i32 %orc_hp14, 18, !dbg !7
  store i32 %SUB15, i32* %orc_hp2, align 4, !dbg !7
  br label %while.start, !dbg !7

if.then9:                                         ; preds = %if.start7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !7
  br label %while.end, !dbg !7

if.start16:                                       ; preds = %while.end
  %hero_hp19 = load i32, i32* %hero_hp1, align 4, !dbg !7
  %LE20 = icmp sle i32 %hero_hp19, 0, !dbg !7
  br i1 %LE20, label %if.then18, label %if.end17, !dbg !7

if.end17:                                         ; preds = %if.then18, %if.start16
  ret void, !dbg !7

if.then18:                                        ; preds = %if.start16
  %round21 = load i32, i32* %round, align 4, !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %round21), !dbg !7
  br label %if.end17, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !8 {
entry:
  call void @simulate(i32 100, i32 60), !dbg !9
  call void @simulate(i32 30, i32 90), !dbg !10
  ret i32 0, !dbg !10
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor06.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/dungeon")
!4 = distinct !DISubprogram(name: "simulate", linkageName: "simulate", scope: null, file: !3, line: 2, type: !5, scopeLine: 2, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 2, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 18, type: !5, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 19, scope: !8)
!10 = !DILocation(line: 20, scope: !8)
