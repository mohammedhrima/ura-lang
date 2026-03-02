; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net/tcp-server.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net/tcp-server.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [15 x i8] c"socket failed\0A\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"bind failed\0A\00", align 1
@STR2 = private unnamed_addr constant [27 x i8] c"listening on port 8080...\0A\00", align 1
@STR3 = private unnamed_addr constant [12 x i8] c"new client\0A\00", align 1
@STR4 = private unnamed_addr constant [14 x i8] c"received: %s\0A\00", align 1
@STR5 = private unnamed_addr constant [19 x i8] c"hello from server\0A\00", align 1
@STR6 = private unnamed_addr constant [13 x i8] c"read failed\0A\00", align 1

declare i32 @write(i32, ptr, i32)

declare i32 @read(i32, ptr, i32)

declare i32 @printf(ptr, i32, ...)

declare ptr @calloc(i32, i32)

declare void @free(ptr)

declare i32 @socket(i32, i32, i32)

declare i32 @bind(i32, ptr, i32)

declare i32 @listen(i32, i32)

declare i32 @accept(i32, ptr, i32)

declare i32 @close(i32)

define i32 @main() !dbg !4 {
entry:
  %server_fd = alloca i32, align 4, !dbg !7
  %port = alloca i32, align 4, !dbg !7
  %addr = alloca ptr, align 8, !dbg !7
  %result = alloca i32, align 4, !dbg !7
  %buf = alloca ptr, align 8, !dbg !7
  %client_fd = alloca i32, align 4, !dbg !7
  %r = alloca i32, align 4, !dbg !7
  store i32 0, ptr %r, align 4, !dbg !7
  store i32 0, ptr %client_fd, align 4, !dbg !7
  store ptr null, ptr %buf, align 8, !dbg !7
  store i32 0, ptr %result, align 4, !dbg !7
  store ptr null, ptr %addr, align 8, !dbg !7
  store i32 0, ptr %port, align 4, !dbg !7
  store i32 0, ptr %server_fd, align 4, !dbg !7
  %socket = call i32 @socket(i32 2, i32 1, i32 0), !dbg !8
  store i32 %socket, ptr %server_fd, align 4, !dbg !8
  br label %if.start, !dbg !8

if.start:                                         ; preds = %entry
  %server_fd1 = load i32, ptr %server_fd, align 4, !dbg !8
  %LT = icmp slt i32 %server_fd1, 0, !dbg !8
  br i1 %LT, label %if.then, label %if.end, !dbg !8

if.end:                                           ; preds = %if.start
  store i32 8080, ptr %port, align 4, !dbg !7
  %calloc = call ptr @calloc(i32 16, i32 1), !dbg !9
  store ptr %calloc, ptr %addr, align 8, !dbg !9
  %addr2 = load ptr, ptr %addr, align 8, !dbg !10
  %ACCESS = getelementptr i8, ptr %addr2, i32 0, !dbg !10
  store i8 2, ptr %ACCESS, align 1, !dbg !10
  %addr3 = load ptr, ptr %addr, align 8, !dbg !11
  %ACCESS4 = getelementptr i8, ptr %addr3, i32 1, !dbg !11
  store i8 0, ptr %ACCESS4, align 1, !dbg !11
  %addr5 = load ptr, ptr %addr, align 8, !dbg !12
  %ACCESS6 = getelementptr i8, ptr %addr5, i32 2, !dbg !12
  store i8 31, ptr %ACCESS6, align 1, !dbg !12
  %addr7 = load ptr, ptr %addr, align 8, !dbg !13
  %ACCESS8 = getelementptr i8, ptr %addr7, i32 3, !dbg !13
  store i8 -112, ptr %ACCESS8, align 1, !dbg !13
  %server_fd9 = load i32, ptr %server_fd, align 4, !dbg !14
  %addr10 = load ptr, ptr %addr, align 8, !dbg !14
  %bind = call i32 @bind(i32 %server_fd9, ptr %addr10, i32 16), !dbg !14
  store i32 %bind, ptr %result, align 4, !dbg !14
  br label %if.start11, !dbg !14

if.then:                                          ; preds = %if.start
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 0), !dbg !15
  ret i32 1, !dbg !15

if.start11:                                       ; preds = %if.end
  %result14 = load i32, ptr %result, align 4, !dbg !14
  %LT15 = icmp slt i32 %result14, 0, !dbg !14
  br i1 %LT15, label %if.then13, label %if.end12, !dbg !14

if.end12:                                         ; preds = %if.start11
  %server_fd17 = load i32, ptr %server_fd, align 4, !dbg !16
  %listen = call i32 @listen(i32 %server_fd17, i32 5), !dbg !16
  %printf18 = call i32 (ptr, i32, ...) @printf(ptr @STR2, i32 0), !dbg !17
  %calloc19 = call ptr @calloc(i32 1024, i32 1), !dbg !18
  store ptr %calloc19, ptr %buf, align 8, !dbg !18
  store i32 0, ptr %client_fd, align 4, !dbg !7
  store i32 0, ptr %r, align 4, !dbg !7
  br label %while.start, !dbg !7

if.then13:                                        ; preds = %if.start11
  %printf16 = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 0), !dbg !19
  ret i32 1, !dbg !19

while.start:                                      ; preds = %if.end30, %if.then23, %if.end12
  br i1 true, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %server_fd20 = load i32, ptr %server_fd, align 4, !dbg !20
  %accept = call i32 @accept(i32 %server_fd20, ptr null, i32 0), !dbg !20
  store i32 %accept, ptr %client_fd, align 4, !dbg !20
  br label %if.start21, !dbg !20

while.end:                                        ; preds = %while.start
  %buf40 = load ptr, ptr %buf, align 8, !dbg !21
  call void @free(ptr %buf40), !dbg !21
  %addr41 = load ptr, ptr %addr, align 8, !dbg !22
  call void @free(ptr %addr41), !dbg !22
  ret i32 0, !dbg !22

if.start21:                                       ; preds = %while.then
  %client_fd24 = load i32, ptr %client_fd, align 4, !dbg !20
  %LT25 = icmp slt i32 %client_fd24, 0, !dbg !20
  br i1 %LT25, label %if.then23, label %if.end22, !dbg !20

if.end22:                                         ; preds = %if.start21
  %printf26 = call i32 (ptr, i32, ...) @printf(ptr @STR3, i32 0), !dbg !23
  %client_fd27 = load i32, ptr %client_fd, align 4, !dbg !24
  %buf28 = load ptr, ptr %buf, align 8, !dbg !24
  %read = call i32 @read(i32 %client_fd27, ptr %buf28, i32 1023), !dbg !24
  store i32 %read, ptr %r, align 4, !dbg !24
  br label %if.start29, !dbg !24

if.then23:                                        ; preds = %if.start21
  br label %while.start, !dbg !20

if.start29:                                       ; preds = %if.end22
  %r32 = load i32, ptr %r, align 4, !dbg !24
  %GT = icmp sgt i32 %r32, 0, !dbg !24
  br i1 %GT, label %if.then31, label %elif.start, !dbg !24

if.end30:                                         ; preds = %elif.then, %elif.start, %if.then31
  %client_fd39 = load i32, ptr %client_fd, align 4, !dbg !25
  %close = call i32 @close(i32 %client_fd39), !dbg !25
  br label %while.start, !dbg !25

if.then31:                                        ; preds = %if.start29
  %buf33 = load ptr, ptr %buf, align 8, !dbg !26
  %printf34 = call i32 (ptr, i32, ...) @printf(ptr @STR4, i32 1, ptr %buf33), !dbg !26
  %client_fd35 = load i32, ptr %client_fd, align 4, !dbg !27
  %write = call i32 @write(i32 %client_fd35, ptr @STR5, i32 18), !dbg !27
  br label %if.end30, !dbg !27

elif.start:                                       ; preds = %if.start29
  %r36 = load i32, ptr %r, align 4, !dbg !27
  %LT37 = icmp slt i32 %r36, 0, !dbg !27
  br i1 %LT37, label %elif.then, label %if.end30, !dbg !27

elif.then:                                        ; preds = %elif.start
  %printf38 = call i32 (ptr, i32, ...) @printf(ptr @STR6, i32 0), !dbg !28
  br label %if.end30, !dbg !28
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "tcp-server.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 4, scope: !4)
!9 = !DILocation(line: 10, scope: !4)
!10 = !DILocation(line: 11, scope: !4)
!11 = !DILocation(line: 12, scope: !4)
!12 = !DILocation(line: 13, scope: !4)
!13 = !DILocation(line: 14, scope: !4)
!14 = !DILocation(line: 17, scope: !4)
!15 = !DILocation(line: 6, scope: !4)
!16 = !DILocation(line: 22, scope: !4)
!17 = !DILocation(line: 23, scope: !4)
!18 = !DILocation(line: 25, scope: !4)
!19 = !DILocation(line: 19, scope: !4)
!20 = !DILocation(line: 29, scope: !4)
!21 = !DILocation(line: 41, scope: !4)
!22 = !DILocation(line: 42, scope: !4)
!23 = !DILocation(line: 32, scope: !4)
!24 = !DILocation(line: 33, scope: !4)
!25 = !DILocation(line: 39, scope: !4)
!26 = !DILocation(line: 35, scope: !4)
!27 = !DILocation(line: 36, scope: !4)
!28 = !DILocation(line: 38, scope: !4)
