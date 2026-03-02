; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net/server.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net/server.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [35 x i8] c"\1B[0;31m[server] socket failed\1B[0m\0A\00", align 1
@STR1 = private unnamed_addr constant [33 x i8] c"\1B[0;31m[server] bind failed\1B[0m\0A\00", align 1
@STR2 = private unnamed_addr constant [47 x i8] c"\1B[0;32m[server]\1B[0m listening on port 8080...\0A\00", align 1
@STR3 = private unnamed_addr constant [38 x i8] c"\1B[0;32m[server]\1B[0m client connected\0A\00", align 1
@STR4 = private unnamed_addr constant [41 x i8] c"\1B[0;31m[server]\1B[0m client disconnected\0A\00", align 1
@STR5 = private unnamed_addr constant [6 x i8] c"\0D\1B[2K\00", align 1
@STR6 = private unnamed_addr constant [21 x i8] c"\1B[0;36m[client]\1B[0m \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@STR8 = private unnamed_addr constant [23 x i8] c"\1B[0;32m[server]\1B[0m > \00", align 1
@STR9 = private unnamed_addr constant [23 x i8] c"\1B[0;32m[server]\1B[0m > \00", align 1

declare i32 @write(i32, ptr, i32)

declare i32 @read(i32, ptr, i32)

declare ptr @calloc(i32, i32)

declare void @free(ptr)

declare i32 @strlen(ptr)

declare i32 @socket(i32, i32, i32)

declare i32 @bind(i32, ptr, i32)

declare i32 @listen(i32, i32)

declare i32 @accept(i32, ptr, i32)

declare i32 @close(i32)

declare i32 @setsockopt(i32, i32, i32, ptr, i32)

declare i32 @fork()

define void @log(ptr %msg) !dbg !4 {
entry:
  %msg1 = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %msg1, align 8, !dbg !7
  store ptr %msg, ptr %msg1, align 8, !dbg !7
  %msg2 = load ptr, ptr %msg1, align 8, !dbg !8
  %msg3 = load ptr, ptr %msg1, align 8, !dbg !8
  %strlen = call i32 @strlen(ptr %msg3), !dbg !8
  %write = call i32 @write(i32 1, ptr %msg2, i32 %strlen), !dbg !8
  ret void, !dbg !8
}

define i32 @main() !dbg !9 {
entry:
  %server_fd = alloca i32, align 4, !dbg !10
  %opt = alloca ptr, align 8, !dbg !10
  %addr = alloca ptr, align 8, !dbg !10
  %result = alloca i32, align 4, !dbg !10
  %buf = alloca ptr, align 8, !dbg !10
  %rbuf = alloca ptr, align 8, !dbg !10
  %client_fd = alloca i32, align 4, !dbg !10
  %r = alloca i32, align 4, !dbg !10
  %pid = alloca i32, align 4, !dbg !10
  %pid2 = alloca i32, align 4, !dbg !10
  store i32 0, ptr %pid2, align 4, !dbg !10
  store i32 0, ptr %pid, align 4, !dbg !10
  store i32 0, ptr %r, align 4, !dbg !10
  store i32 0, ptr %client_fd, align 4, !dbg !10
  store ptr null, ptr %rbuf, align 8, !dbg !10
  store ptr null, ptr %buf, align 8, !dbg !10
  store i32 0, ptr %result, align 4, !dbg !10
  store ptr null, ptr %addr, align 8, !dbg !10
  store ptr null, ptr %opt, align 8, !dbg !10
  store i32 0, ptr %server_fd, align 4, !dbg !10
  %socket = call i32 @socket(i32 2, i32 1, i32 0), !dbg !11
  store i32 %socket, ptr %server_fd, align 4, !dbg !11
  br label %if.start, !dbg !11

if.start:                                         ; preds = %entry
  %server_fd1 = load i32, ptr %server_fd, align 4, !dbg !11
  %LT = icmp slt i32 %server_fd1, 0, !dbg !11
  br i1 %LT, label %if.then, label %if.end, !dbg !11

if.end:                                           ; preds = %if.start
  %calloc = call ptr @calloc(i32 4, i32 1), !dbg !12
  store ptr %calloc, ptr %opt, align 8, !dbg !12
  %opt2 = load ptr, ptr %opt, align 8, !dbg !13
  %ACCESS = getelementptr i8, ptr %opt2, i32 0, !dbg !13
  store i8 1, ptr %ACCESS, align 1, !dbg !13
  %server_fd3 = load i32, ptr %server_fd, align 4, !dbg !14
  %opt4 = load ptr, ptr %opt, align 8, !dbg !14
  %setsockopt = call i32 @setsockopt(i32 %server_fd3, i32 65535, i32 4, ptr %opt4, i32 4), !dbg !14
  %server_fd5 = load i32, ptr %server_fd, align 4, !dbg !15
  %opt6 = load ptr, ptr %opt, align 8, !dbg !15
  %setsockopt7 = call i32 @setsockopt(i32 %server_fd5, i32 65535, i32 512, ptr %opt6, i32 4), !dbg !15
  %opt8 = load ptr, ptr %opt, align 8, !dbg !16
  call void @free(ptr %opt8), !dbg !16
  %calloc9 = call ptr @calloc(i32 16, i32 1), !dbg !17
  store ptr %calloc9, ptr %addr, align 8, !dbg !17
  %addr10 = load ptr, ptr %addr, align 8, !dbg !18
  %ACCESS11 = getelementptr i8, ptr %addr10, i32 0, !dbg !18
  store i8 16, ptr %ACCESS11, align 1, !dbg !18
  %addr12 = load ptr, ptr %addr, align 8, !dbg !19
  %ACCESS13 = getelementptr i8, ptr %addr12, i32 1, !dbg !19
  store i8 2, ptr %ACCESS13, align 1, !dbg !19
  %addr14 = load ptr, ptr %addr, align 8, !dbg !20
  %ACCESS15 = getelementptr i8, ptr %addr14, i32 2, !dbg !20
  store i8 31, ptr %ACCESS15, align 1, !dbg !20
  %addr16 = load ptr, ptr %addr, align 8, !dbg !21
  %ACCESS17 = getelementptr i8, ptr %addr16, i32 3, !dbg !21
  store i8 -112, ptr %ACCESS17, align 1, !dbg !21
  %server_fd18 = load i32, ptr %server_fd, align 4, !dbg !22
  %addr19 = load ptr, ptr %addr, align 8, !dbg !22
  %bind = call i32 @bind(i32 %server_fd18, ptr %addr19, i32 16), !dbg !22
  store i32 %bind, ptr %result, align 4, !dbg !22
  br label %if.start20, !dbg !22

if.then:                                          ; preds = %if.start
  call void @log(ptr @STR0), !dbg !23
  ret i32 1, !dbg !23

if.start20:                                       ; preds = %if.end
  %result23 = load i32, ptr %result, align 4, !dbg !22
  %LT24 = icmp slt i32 %result23, 0, !dbg !22
  br i1 %LT24, label %if.then22, label %if.end21, !dbg !22

if.end21:                                         ; preds = %if.start20
  %server_fd25 = load i32, ptr %server_fd, align 4, !dbg !24
  %listen = call i32 @listen(i32 %server_fd25, i32 5), !dbg !24
  call void @log(ptr @STR2), !dbg !25
  %calloc26 = call ptr @calloc(i32 1024, i32 1), !dbg !26
  store ptr %calloc26, ptr %buf, align 8, !dbg !26
  %calloc27 = call ptr @calloc(i32 1024, i32 1), !dbg !27
  store ptr %calloc27, ptr %rbuf, align 8, !dbg !27
  store i32 0, ptr %client_fd, align 4, !dbg !10
  store i32 0, ptr %r, align 4, !dbg !10
  br label %while.start, !dbg !10

if.then22:                                        ; preds = %if.start20
  call void @log(ptr @STR1), !dbg !28
  ret i32 1, !dbg !28

while.start:                                      ; preds = %if.end35, %if.then31, %if.end21
  br i1 true, label %while.then, label %while.end, !dbg !10

while.then:                                       ; preds = %while.start
  %server_fd28 = load i32, ptr %server_fd, align 4, !dbg !29
  %accept = call i32 @accept(i32 %server_fd28, ptr null, i32 0), !dbg !29
  store i32 %accept, ptr %client_fd, align 4, !dbg !29
  br label %if.start29, !dbg !29

while.end:                                        ; preds = %while.start
  %buf79 = load ptr, ptr %buf, align 8, !dbg !30
  call void @free(ptr %buf79), !dbg !30
  %rbuf80 = load ptr, ptr %rbuf, align 8, !dbg !31
  call void @free(ptr %rbuf80), !dbg !31
  %addr81 = load ptr, ptr %addr, align 8, !dbg !32
  call void @free(ptr %addr81), !dbg !32
  ret i32 0, !dbg !32

if.start29:                                       ; preds = %while.then
  %client_fd32 = load i32, ptr %client_fd, align 4, !dbg !29
  %LT33 = icmp slt i32 %client_fd32, 0, !dbg !29
  br i1 %LT33, label %if.then31, label %if.end30, !dbg !29

if.end30:                                         ; preds = %if.start29
  call void @log(ptr @STR3), !dbg !33
  %fork = call i32 @fork(), !dbg !34
  store i32 %fork, ptr %pid, align 4, !dbg !34
  br label %if.start34, !dbg !34

if.then31:                                        ; preds = %if.start29
  br label %while.start, !dbg !29

if.start34:                                       ; preds = %if.end30
  %pid37 = load i32, ptr %pid, align 4, !dbg !34
  %EQ = icmp eq i32 %pid37, 0, !dbg !34
  br i1 %EQ, label %if.then36, label %if.end35, !dbg !34

if.end35:                                         ; preds = %if.start34
  %client_fd77 = load i32, ptr %client_fd, align 4, !dbg !35
  %close78 = call i32 @close(i32 %client_fd77), !dbg !35
  br label %while.start, !dbg !35

if.then36:                                        ; preds = %if.start34
  %server_fd38 = load i32, ptr %server_fd, align 4, !dbg !36
  %close = call i32 @close(i32 %server_fd38), !dbg !36
  %fork39 = call i32 @fork(), !dbg !37
  store i32 %fork39, ptr %pid2, align 4, !dbg !37
  br label %if.start40, !dbg !37

if.start40:                                       ; preds = %if.then36
  %pid243 = load i32, ptr %pid2, align 4, !dbg !37
  %EQ44 = icmp eq i32 %pid243, 0, !dbg !37
  br i1 %EQ44, label %if.then42, label %if.end41, !dbg !37

if.end41:                                         ; preds = %if.start40
  br label %while.start58, !dbg !38

if.then42:                                        ; preds = %if.start40
  br label %while.start45, !dbg !37

while.start45:                                    ; preds = %if.end51, %if.then42
  br i1 true, label %while.then46, label %while.end47, !dbg !37

while.then46:                                     ; preds = %while.start45
  %client_fd48 = load i32, ptr %client_fd, align 4, !dbg !39
  %buf49 = load ptr, ptr %buf, align 8, !dbg !39
  %read = call i32 @read(i32 %client_fd48, ptr %buf49, i32 1023), !dbg !39
  store i32 %read, ptr %r, align 4, !dbg !39
  br label %if.start50, !dbg !39

while.end47:                                      ; preds = %if.then52, %while.start45
  ret i32 0, !dbg !38

if.start50:                                       ; preds = %while.then46
  %r53 = load i32, ptr %r, align 4, !dbg !39
  %LE = icmp sle i32 %r53, 0, !dbg !39
  br i1 %LE, label %if.then52, label %if.end51, !dbg !39

if.end51:                                         ; preds = %if.start50
  %buf54 = load ptr, ptr %buf, align 8, !dbg !40
  %r55 = load i32, ptr %r, align 4, !dbg !40
  %ACCESS56 = getelementptr i8, ptr %buf54, i32 %r55, !dbg !40
  store i8 0, ptr %ACCESS56, align 1, !dbg !40
  call void @log(ptr @STR5), !dbg !41
  call void @log(ptr @STR6), !dbg !42
  %buf57 = load ptr, ptr %buf, align 8, !dbg !43
  call void @log(ptr %buf57), !dbg !43
  call void @log(ptr @STR7), !dbg !44
  call void @log(ptr @STR8), !dbg !38
  br label %while.start45, !dbg !38

if.then52:                                        ; preds = %if.start50
  call void @log(ptr @STR4), !dbg !45
  br label %while.end47, !dbg !45

while.start58:                                    ; preds = %if.end64, %if.end41
  br i1 true, label %while.then59, label %while.end60, !dbg !38

while.then59:                                     ; preds = %while.start58
  call void @log(ptr @STR9), !dbg !46
  %rbuf61 = load ptr, ptr %rbuf, align 8, !dbg !47
  %read62 = call i32 @read(i32 0, ptr %rbuf61, i32 1023), !dbg !47
  store i32 %read62, ptr %r, align 4, !dbg !47
  br label %if.start63, !dbg !47

while.end60:                                      ; preds = %if.then65, %while.start58
  %client_fd75 = load i32, ptr %client_fd, align 4, !dbg !48
  %close76 = call i32 @close(i32 %client_fd75), !dbg !48
  ret i32 0, !dbg !48

if.start63:                                       ; preds = %while.then59
  %r66 = load i32, ptr %r, align 4, !dbg !47
  %LE67 = icmp sle i32 %r66, 0, !dbg !47
  br i1 %LE67, label %if.then65, label %if.end64, !dbg !47

if.end64:                                         ; preds = %if.start63
  %r68 = load i32, ptr %r, align 4, !dbg !49
  %SUB = sub i32 %r68, 1, !dbg !49
  %rbuf69 = load ptr, ptr %rbuf, align 8, !dbg !49
  %ACCESS70 = getelementptr i8, ptr %rbuf69, i32 %SUB, !dbg !49
  store i8 0, ptr %ACCESS70, align 1, !dbg !49
  %client_fd71 = load i32, ptr %client_fd, align 4, !dbg !50
  %rbuf72 = load ptr, ptr %rbuf, align 8, !dbg !50
  %r73 = load i32, ptr %r, align 4, !dbg !50
  %SUB74 = sub i32 %r73, 1, !dbg !50
  %write = call i32 @write(i32 %client_fd71, ptr %rbuf72, i32 %SUB74), !dbg !50
  br label %while.start58, !dbg !50

if.then65:                                        ; preds = %if.start63
  br label %while.end60, !dbg !47
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "server.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net")
!4 = distinct !DISubprogram(name: "log", linkageName: "log", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 4, scope: !4)
!9 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2)
!10 = !DILocation(line: 6, scope: !9)
!11 = !DILocation(line: 7, scope: !9)
!12 = !DILocation(line: 12, scope: !9)
!13 = !DILocation(line: 13, scope: !9)
!14 = !DILocation(line: 14, scope: !9)
!15 = !DILocation(line: 15, scope: !9)
!16 = !DILocation(line: 16, scope: !9)
!17 = !DILocation(line: 18, scope: !9)
!18 = !DILocation(line: 19, scope: !9)
!19 = !DILocation(line: 20, scope: !9)
!20 = !DILocation(line: 21, scope: !9)
!21 = !DILocation(line: 22, scope: !9)
!22 = !DILocation(line: 24, scope: !9)
!23 = !DILocation(line: 9, scope: !9)
!24 = !DILocation(line: 29, scope: !9)
!25 = !DILocation(line: 30, scope: !9)
!26 = !DILocation(line: 32, scope: !9)
!27 = !DILocation(line: 33, scope: !9)
!28 = !DILocation(line: 26, scope: !9)
!29 = !DILocation(line: 38, scope: !9)
!30 = !DILocation(line: 79, scope: !9)
!31 = !DILocation(line: 80, scope: !9)
!32 = !DILocation(line: 81, scope: !9)
!33 = !DILocation(line: 42, scope: !9)
!34 = !DILocation(line: 44, scope: !9)
!35 = !DILocation(line: 77, scope: !9)
!36 = !DILocation(line: 46, scope: !9)
!37 = !DILocation(line: 49, scope: !9)
!38 = !DILocation(line: 61, scope: !9)
!39 = !DILocation(line: 52, scope: !9)
!40 = !DILocation(line: 56, scope: !9)
!41 = !DILocation(line: 57, scope: !9)
!42 = !DILocation(line: 58, scope: !9)
!43 = !DILocation(line: 59, scope: !9)
!44 = !DILocation(line: 60, scope: !9)
!45 = !DILocation(line: 54, scope: !9)
!46 = !DILocation(line: 66, scope: !9)
!47 = !DILocation(line: 67, scope: !9)
!48 = !DILocation(line: 73, scope: !9)
!49 = !DILocation(line: 70, scope: !9)
!50 = !DILocation(line: 71, scope: !9)
