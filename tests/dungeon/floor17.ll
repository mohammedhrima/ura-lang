; ModuleID = 'tests/dungeon/floor17.ura'
source_filename = "tests/dungeon/floor17.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [15 x i8] c"socket failed\0A\00", align 1
@output_fmt = private unnamed_addr constant [15 x i8] c"socket failed\0A\00", align 1
@STR1 = private unnamed_addr constant [20 x i8] c"listening on :8080\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [20 x i8] c"listening on :8080\0A\00", align 1
@STR2 = private unnamed_addr constant [6 x i8] c"got: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [9 x i8] c"got: %s\0A\00", align 1
@STR4 = private unnamed_addr constant [6 x i8] c"pong\0A\00", align 1

declare i32 @write(i32, i8*, i32)

declare i32 @read(i32, i8*, i32)

declare void @free(i8*)

declare i32 @socket(i32, i32, i32)

declare i32 @bind(i32, i8*, i32)

declare i32 @listen(i32, i32)

declare i32 @accept(i32, i8*, i8*)

declare i32 @close(i32)

define void @run() !dbg !4 {
entry:
  %fd = alloca i32, align 4, !dbg !7
  %port = alloca i32, align 4, !dbg !7
  %addr = alloca i8*, align 8, !dbg !7
  %hi = alloca i32, align 4, !dbg !7
  %lo = alloca i32, align 4, !dbg !7
  %buf = alloca i8*, align 8, !dbg !7
  %client = alloca i32, align 4, !dbg !7
  %n = alloca i32, align 4, !dbg !7
  store i32 0, i32* %fd, align 4, !dbg !7
  %socket = call i32 @socket(i32 2, i32 1, i32 0), !dbg !8
  store i32 %socket, i32* %fd, align 4, !dbg !8
  br label %if.start, !dbg !8

if.start:                                         ; preds = %entry
  %fd1 = load i32, i32* %fd, align 4, !dbg !8
  %LT = icmp slt i32 %fd1, 0, !dbg !8
  br i1 %LT, label %if.then, label %if.end, !dbg !8

if.end:                                           ; preds = %if.start
  store i32 0, i32* %port, align 4, !dbg !7
  store i32 8080, i32* %port, align 4, !dbg !7
  store i8* null, i8** %addr, align 8, !dbg !7
  %heap = call i8* @calloc(i64 16, i64 1), !dbg !7
  store i8* %heap, i8** %addr, align 8, !dbg !7
  %addr2 = load i8*, i8** %addr, align 8, !dbg !9
  %ACCESS = getelementptr i8, i8* %addr2, i32 0, !dbg !9
  store i8 16, i8* %ACCESS, align 1, !dbg !9
  %addr3 = load i8*, i8** %addr, align 8, !dbg !10
  %ACCESS4 = getelementptr i8, i8* %addr3, i32 1, !dbg !10
  store i8 2, i8* %ACCESS4, align 1, !dbg !10
  store i32 0, i32* %hi, align 4, !dbg !7
  %port5 = load i32, i32* %port, align 4, !dbg !7
  %RSHIFT = ashr i32 %port5, 8, !dbg !7
  store i32 %RSHIFT, i32* %hi, align 4, !dbg !7
  store i32 0, i32* %lo, align 4, !dbg !7
  %port6 = load i32, i32* %port, align 4, !dbg !7
  %BAND = and i32 %port6, 255, !dbg !7
  store i32 %BAND, i32* %lo, align 4, !dbg !7
  %addr7 = load i8*, i8** %addr, align 8, !dbg !11
  %ACCESS8 = getelementptr i8, i8* %addr7, i32 2, !dbg !11
  %hi9 = load i32, i32* %hi, align 4, !dbg !11
  %as = trunc i32 %hi9 to i8, !dbg !11
  store i8 %as, i8* %ACCESS8, align 1, !dbg !11
  %addr10 = load i8*, i8** %addr, align 8, !dbg !12
  %ACCESS11 = getelementptr i8, i8* %addr10, i32 3, !dbg !12
  %lo12 = load i32, i32* %lo, align 4, !dbg !12
  %as13 = trunc i32 %lo12 to i8, !dbg !12
  store i8 %as13, i8* %ACCESS11, align 1, !dbg !12
  %fd14 = load i32, i32* %fd, align 4, !dbg !13
  %addr15 = load i8*, i8** %addr, align 8, !dbg !13
  %bind = call i32 @bind(i32 %fd14, i8* %addr15, i32 16), !dbg !13
  %fd16 = load i32, i32* %fd, align 4, !dbg !14
  %listen = call i32 @listen(i32 %fd16, i32 5), !dbg !14
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !14
  store i8* null, i8** %buf, align 8, !dbg !7
  %heap17 = call i8* @calloc(i64 1024, i64 1), !dbg !7
  store i8* %heap17, i8** %buf, align 8, !dbg !7
  store i32 0, i32* %client, align 4, !dbg !7
  %fd18 = load i32, i32* %fd, align 4, !dbg !15
  %accept = call i32 @accept(i32 %fd18, i8* null, i8* null), !dbg !15
  store i32 %accept, i32* %client, align 4, !dbg !15
  br label %if.start19, !dbg !15

if.then:                                          ; preds = %if.start
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !8
  ret void, !dbg !8

if.start19:                                       ; preds = %if.end
  %client22 = load i32, i32* %client, align 4, !dbg !15
  %GE = icmp sge i32 %client22, 0, !dbg !15
  br i1 %GE, label %if.then21, label %if.end20, !dbg !15

if.end20:                                         ; preds = %if.end26, %if.start19
  %buf32 = load i8*, i8** %buf, align 8, !dbg !16
  call void @free(i8* %buf32), !dbg !16
  %addr33 = load i8*, i8** %addr, align 8, !dbg !17
  call void @free(i8* %addr33), !dbg !17
  ret void, !dbg !17

if.then21:                                        ; preds = %if.start19
  store i32 0, i32* %n, align 4, !dbg !7
  %client23 = load i32, i32* %client, align 4, !dbg !18
  %buf24 = load i8*, i8** %buf, align 8, !dbg !18
  %read = call i32 @read(i32 %client23, i8* %buf24, i32 1023), !dbg !18
  store i32 %read, i32* %n, align 4, !dbg !18
  br label %if.start25, !dbg !18

if.start25:                                       ; preds = %if.then21
  %n28 = load i32, i32* %n, align 4, !dbg !18
  %GT = icmp sgt i32 %n28, 0, !dbg !18
  br i1 %GT, label %if.then27, label %if.end26, !dbg !18

if.end26:                                         ; preds = %if.then27, %if.start25
  %client31 = load i32, i32* %client, align 4, !dbg !19
  %close = call i32 @close(i32 %client31), !dbg !19
  br label %if.end20, !dbg !19

if.then27:                                        ; preds = %if.start25
  %buf29 = load i8*, i8** %buf, align 8, !dbg !18
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i8* %buf29), !dbg !18
  %client30 = load i32, i32* %client, align 4, !dbg !20
  %write = call i32 @write(i32 %client30, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR4, i32 0, i32 0), i32 5), !dbg !20
  br label %if.end26, !dbg !20
}

declare i32 @printf(i8*, i32, ...)

declare i8* @calloc(i64, i64)

define i32 @main() !dbg !21 {
entry:
  call void @run(), !dbg !22
  ret i32 0, !dbg !22
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor17.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "run", linkageName: "run", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 5, scope: !4)
!9 = !DILocation(line: 12, scope: !4)
!10 = !DILocation(line: 13, scope: !4)
!11 = !DILocation(line: 16, scope: !4)
!12 = !DILocation(line: 17, scope: !4)
!13 = !DILocation(line: 19, scope: !4)
!14 = !DILocation(line: 20, scope: !4)
!15 = !DILocation(line: 24, scope: !4)
!16 = !DILocation(line: 31, scope: !4)
!17 = !DILocation(line: 32, scope: !4)
!18 = !DILocation(line: 26, scope: !4)
!19 = !DILocation(line: 30, scope: !4)
!20 = !DILocation(line: 29, scope: !4)
!21 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 34, type: !5, scopeLine: 34, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!22 = !DILocation(line: 35, scope: !21)
