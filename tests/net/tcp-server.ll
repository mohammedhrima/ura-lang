; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [15 x i8] c"socket failed\0A\00", align 1
@output_fmt = private unnamed_addr constant [15 x i8] c"socket failed\0A\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"bind failed\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [13 x i8] c"bind failed\0A\00", align 1
@STR2 = private unnamed_addr constant [27 x i8] c"listening on port 8080...\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [27 x i8] c"listening on port 8080...\0A\00", align 1
@STR3 = private unnamed_addr constant [12 x i8] c"new client\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [12 x i8] c"new client\0A\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"received: \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [14 x i8] c"received: %s\0A\00", align 1
@STR6 = private unnamed_addr constant [19 x i8] c"hello from server\0A\00", align 1
@STR7 = private unnamed_addr constant [13 x i8] c"read failed\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [13 x i8] c"read failed\0A\00", align 1

declare i32 @write(i32, i8*, i32)

declare i32 @read(i32, i8*, i32)

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @socket(i32, i32, i32)

declare i32 @bind(i32, i8*, i32)

declare i32 @listen(i32, i32)

declare i32 @accept(i32, i8*, i8*)

declare i32 @close(i32)

define i32 @main() !dbg !4 {
entry:
  %server_fd = alloca i32, align 4, !dbg !7
  %port = alloca i32, align 4, !dbg !7
  %addr = alloca i8*, align 8, !dbg !7
  %result = alloca i32, align 4, !dbg !7
  %buf = alloca i8*, align 8, !dbg !7
  %client_fd = alloca i32, align 4, !dbg !7
  %r = alloca i32, align 4, !dbg !7
  store i32 0, i32* %server_fd, align 4, !dbg !7
  %socket = call i32 @socket(i32 2, i32 1, i32 0), !dbg !8
  store i32 %socket, i32* %server_fd, align 4, !dbg !8
  br label %if.start, !dbg !8

if.start:                                         ; preds = %entry
  %server_fd1 = load i32, i32* %server_fd, align 4, !dbg !8
  %LT = icmp slt i32 %server_fd1, 0, !dbg !8
  br i1 %LT, label %if.then, label %if.end, !dbg !8

if.end:                                           ; preds = %if.start
  store i32 0, i32* %port, align 4, !dbg !7
  store i32 8080, i32* %port, align 4, !dbg !7
  store i8* null, i8** %addr, align 8, !dbg !7
  %calloc = call i8* @calloc(i64 16, i64 1), !dbg !9
  store i8* %calloc, i8** %addr, align 8, !dbg !9
  %addr2 = load i8*, i8** %addr, align 8, !dbg !10
  %ACCESS = getelementptr i8, i8* %addr2, i32 0, !dbg !10
  store i8 2, i8* %ACCESS, align 1, !dbg !10
  %addr3 = load i8*, i8** %addr, align 8, !dbg !11
  %ACCESS4 = getelementptr i8, i8* %addr3, i32 1, !dbg !11
  store i8 0, i8* %ACCESS4, align 1, !dbg !11
  %addr5 = load i8*, i8** %addr, align 8, !dbg !12
  %ACCESS6 = getelementptr i8, i8* %addr5, i32 2, !dbg !12
  store i8 31, i8* %ACCESS6, align 1, !dbg !12
  %addr7 = load i8*, i8** %addr, align 8, !dbg !13
  %ACCESS8 = getelementptr i8, i8* %addr7, i32 3, !dbg !13
  store i8 -112, i8* %ACCESS8, align 1, !dbg !13
  store i32 0, i32* %result, align 4, !dbg !7
  %server_fd9 = load i32, i32* %server_fd, align 4, !dbg !14
  %addr10 = load i8*, i8** %addr, align 8, !dbg !14
  %bind = call i32 @bind(i32 %server_fd9, i8* %addr10, i32 16), !dbg !14
  store i32 %bind, i32* %result, align 4, !dbg !14
  br label %if.start11, !dbg !14

if.then:                                          ; preds = %if.start
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !8
  ret i32 1, !dbg !8

if.start11:                                       ; preds = %if.end
  %result14 = load i32, i32* %result, align 4, !dbg !14
  %LT15 = icmp slt i32 %result14, 0, !dbg !14
  br i1 %LT15, label %if.then13, label %if.end12, !dbg !14

if.end12:                                         ; preds = %if.start11
  %server_fd16 = load i32, i32* %server_fd, align 4, !dbg !15
  %listen = call i32 @listen(i32 %server_fd16, i32 5), !dbg !15
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @output_fmt.2, i32 0, i32 0), i32 0), !dbg !15
  store i8* null, i8** %buf, align 8, !dbg !7
  %calloc17 = call i8* @calloc(i64 1024, i64 1), !dbg !16
  store i8* %calloc17, i8** %buf, align 8, !dbg !16
  store i32 0, i32* %client_fd, align 4, !dbg !7
  store i32 0, i32* %client_fd, align 4, !dbg !7
  store i32 0, i32* %r, align 4, !dbg !7
  store i32 0, i32* %r, align 4, !dbg !7
  br label %while.start, !dbg !7

if.then13:                                        ; preds = %if.start11
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !14
  ret i32 1, !dbg !14

while.start:                                      ; preds = %if.end27, %if.then21, %if.end12
  br i1 true, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %server_fd18 = load i32, i32* %server_fd, align 4, !dbg !17
  %accept = call i32 @accept(i32 %server_fd18, i8* null, i8* null), !dbg !17
  store i32 %accept, i32* %client_fd, align 4, !dbg !17
  br label %if.start19, !dbg !17

while.end:                                        ; preds = %while.start
  %buf35 = load i8*, i8** %buf, align 8, !dbg !18
  call void @free(i8* %buf35), !dbg !18
  %addr36 = load i8*, i8** %addr, align 8, !dbg !19
  call void @free(i8* %addr36), !dbg !19
  ret i32 0, !dbg !19

if.start19:                                       ; preds = %while.then
  %client_fd22 = load i32, i32* %client_fd, align 4, !dbg !17
  %LT23 = icmp slt i32 %client_fd22, 0, !dbg !17
  br i1 %LT23, label %if.then21, label %if.end20, !dbg !17

if.end20:                                         ; preds = %if.start19
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @output_fmt.3, i32 0, i32 0), i32 0), !dbg !17
  %client_fd24 = load i32, i32* %client_fd, align 4, !dbg !20
  %buf25 = load i8*, i8** %buf, align 8, !dbg !20
  %read = call i32 @read(i32 %client_fd24, i8* %buf25, i32 1023), !dbg !20
  store i32 %read, i32* %r, align 4, !dbg !20
  br label %if.start26, !dbg !20

if.then21:                                        ; preds = %if.start19
  br label %while.start, !dbg !17

if.start26:                                       ; preds = %if.end20
  %r29 = load i32, i32* %r, align 4, !dbg !20
  %GT = icmp sgt i32 %r29, 0, !dbg !20
  br i1 %GT, label %if.then28, label %elif.start, !dbg !20

if.end27:                                         ; preds = %elif.then, %elif.start, %if.then28
  %client_fd34 = load i32, i32* %client_fd, align 4, !dbg !21
  %close = call i32 @close(i32 %client_fd34), !dbg !21
  br label %while.start, !dbg !21

if.then28:                                        ; preds = %if.start26
  %buf30 = load i8*, i8** %buf, align 8, !dbg !20
  %4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.4, i32 0, i32 0), i32 1, i8* %buf30), !dbg !20
  %client_fd31 = load i32, i32* %client_fd, align 4, !dbg !22
  %write = call i32 @write(i32 %client_fd31, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @STR6, i32 0, i32 0), i32 18), !dbg !22
  br label %if.end27, !dbg !22

elif.start:                                       ; preds = %if.start26
  %r32 = load i32, i32* %r, align 4, !dbg !22
  %LT33 = icmp slt i32 %r32, 0, !dbg !22
  br i1 %LT33, label %elif.then, label %if.end27, !dbg !22

elif.then:                                        ; preds = %elif.start
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @output_fmt.5, i32 0, i32 0), i32 0), !dbg !22
  br label %if.end27, !dbg !22
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 5, scope: !4)
!9 = !DILocation(line: 11, scope: !4)
!10 = !DILocation(line: 12, scope: !4)
!11 = !DILocation(line: 13, scope: !4)
!12 = !DILocation(line: 14, scope: !4)
!13 = !DILocation(line: 15, scope: !4)
!14 = !DILocation(line: 18, scope: !4)
!15 = !DILocation(line: 23, scope: !4)
!16 = !DILocation(line: 26, scope: !4)
!17 = !DILocation(line: 30, scope: !4)
!18 = !DILocation(line: 42, scope: !4)
!19 = !DILocation(line: 43, scope: !4)
!20 = !DILocation(line: 34, scope: !4)
!21 = !DILocation(line: 40, scope: !4)
!22 = !DILocation(line: 37, scope: !4)
