; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net/client.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net/client.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [35 x i8] c"\1B[0;31m[client] socket failed\1B[0m\0A\00", align 1
@STR1 = private unnamed_addr constant [36 x i8] c"\1B[0;31m[client] connect failed\1B[0m\0A\00", align 1
@STR2 = private unnamed_addr constant [54 x i8] c"\1B[0;36m[client]\1B[0m connected to server on port 8080\0A\00", align 1
@STR3 = private unnamed_addr constant [41 x i8] c"\1B[0;31m[client]\1B[0m server disconnected\0A\00", align 1
@STR4 = private unnamed_addr constant [6 x i8] c"\0D\1B[2K\00", align 1
@STR5 = private unnamed_addr constant [21 x i8] c"\1B[0;33m[server]\1B[0m \00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@STR7 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1
@STR8 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1

declare i32 @write(i32, ptr, i32)

declare i32 @read(i32, ptr, i32)

declare ptr @calloc(i32, i32)

declare void @free(ptr)

declare i32 @strlen(ptr)

declare i32 @socket(i32, i32, i32)

declare i32 @connect(i32, ptr, i32)

declare i32 @close(i32)

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
  %fd = alloca i32, align 4, !dbg !10
  %addr = alloca ptr, align 8, !dbg !10
  %result = alloca i32, align 4, !dbg !10
  %buf = alloca ptr, align 8, !dbg !10
  %rbuf = alloca ptr, align 8, !dbg !10
  %r = alloca i32, align 4, !dbg !10
  %pid = alloca i32, align 4, !dbg !10
  store i32 0, ptr %pid, align 4, !dbg !10
  store i32 0, ptr %r, align 4, !dbg !10
  store ptr null, ptr %rbuf, align 8, !dbg !10
  store ptr null, ptr %buf, align 8, !dbg !10
  store i32 0, ptr %result, align 4, !dbg !10
  store ptr null, ptr %addr, align 8, !dbg !10
  store i32 0, ptr %fd, align 4, !dbg !10
  %socket = call i32 @socket(i32 2, i32 1, i32 0), !dbg !11
  store i32 %socket, ptr %fd, align 4, !dbg !11
  br label %if.start, !dbg !11

if.start:                                         ; preds = %entry
  %fd1 = load i32, ptr %fd, align 4, !dbg !11
  %LT = icmp slt i32 %fd1, 0, !dbg !11
  br i1 %LT, label %if.then, label %if.end, !dbg !11

if.end:                                           ; preds = %if.start
  %calloc = call ptr @calloc(i32 16, i32 1), !dbg !12
  store ptr %calloc, ptr %addr, align 8, !dbg !12
  %addr2 = load ptr, ptr %addr, align 8, !dbg !13
  %ACCESS = getelementptr i8, ptr %addr2, i32 0, !dbg !13
  store i8 16, ptr %ACCESS, align 1, !dbg !13
  %addr3 = load ptr, ptr %addr, align 8, !dbg !14
  %ACCESS4 = getelementptr i8, ptr %addr3, i32 1, !dbg !14
  store i8 2, ptr %ACCESS4, align 1, !dbg !14
  %addr5 = load ptr, ptr %addr, align 8, !dbg !15
  %ACCESS6 = getelementptr i8, ptr %addr5, i32 2, !dbg !15
  store i8 31, ptr %ACCESS6, align 1, !dbg !15
  %addr7 = load ptr, ptr %addr, align 8, !dbg !16
  %ACCESS8 = getelementptr i8, ptr %addr7, i32 3, !dbg !16
  store i8 -112, ptr %ACCESS8, align 1, !dbg !16
  %addr9 = load ptr, ptr %addr, align 8, !dbg !17
  %ACCESS10 = getelementptr i8, ptr %addr9, i32 4, !dbg !17
  store i8 127, ptr %ACCESS10, align 1, !dbg !17
  %addr11 = load ptr, ptr %addr, align 8, !dbg !18
  %ACCESS12 = getelementptr i8, ptr %addr11, i32 5, !dbg !18
  store i8 0, ptr %ACCESS12, align 1, !dbg !18
  %addr13 = load ptr, ptr %addr, align 8, !dbg !19
  %ACCESS14 = getelementptr i8, ptr %addr13, i32 6, !dbg !19
  store i8 0, ptr %ACCESS14, align 1, !dbg !19
  %addr15 = load ptr, ptr %addr, align 8, !dbg !20
  %ACCESS16 = getelementptr i8, ptr %addr15, i32 7, !dbg !20
  store i8 1, ptr %ACCESS16, align 1, !dbg !20
  %fd17 = load i32, ptr %fd, align 4, !dbg !21
  %addr18 = load ptr, ptr %addr, align 8, !dbg !21
  %connect = call i32 @connect(i32 %fd17, ptr %addr18, i32 16), !dbg !21
  store i32 %connect, ptr %result, align 4, !dbg !21
  br label %if.start19, !dbg !21

if.then:                                          ; preds = %if.start
  call void @log(ptr @STR0), !dbg !22
  ret i32 1, !dbg !22

if.start19:                                       ; preds = %if.end
  %result22 = load i32, ptr %result, align 4, !dbg !21
  %LT23 = icmp slt i32 %result22, 0, !dbg !21
  br i1 %LT23, label %if.then21, label %if.end20, !dbg !21

if.end20:                                         ; preds = %if.start19
  call void @log(ptr @STR2), !dbg !23
  %calloc24 = call ptr @calloc(i32 1024, i32 1), !dbg !24
  store ptr %calloc24, ptr %buf, align 8, !dbg !24
  %calloc25 = call ptr @calloc(i32 1024, i32 1), !dbg !25
  store ptr %calloc25, ptr %rbuf, align 8, !dbg !25
  store i32 0, ptr %r, align 4, !dbg !10
  %fork = call i32 @fork(), !dbg !26
  store i32 %fork, ptr %pid, align 4, !dbg !26
  br label %if.start26, !dbg !26

if.then21:                                        ; preds = %if.start19
  call void @log(ptr @STR1), !dbg !27
  ret i32 1, !dbg !27

if.start26:                                       ; preds = %if.end20
  %pid29 = load i32, ptr %pid, align 4, !dbg !26
  %EQ = icmp eq i32 %pid29, 0, !dbg !26
  br i1 %EQ, label %if.then28, label %if.end27, !dbg !26

if.end27:                                         ; preds = %if.start26
  br label %while.start40, !dbg !28

if.then28:                                        ; preds = %if.start26
  br label %while.start, !dbg !26

while.start:                                      ; preds = %if.end33, %if.then28
  br i1 true, label %while.then, label %while.end, !dbg !26

while.then:                                       ; preds = %while.start
  %fd30 = load i32, ptr %fd, align 4, !dbg !29
  %rbuf31 = load ptr, ptr %rbuf, align 8, !dbg !29
  %read = call i32 @read(i32 %fd30, ptr %rbuf31, i32 1023), !dbg !29
  store i32 %read, ptr %r, align 4, !dbg !29
  br label %if.start32, !dbg !29

while.end:                                        ; preds = %if.then34, %while.start
  ret i32 0, !dbg !28

if.start32:                                       ; preds = %while.then
  %r35 = load i32, ptr %r, align 4, !dbg !29
  %LE = icmp sle i32 %r35, 0, !dbg !29
  br i1 %LE, label %if.then34, label %if.end33, !dbg !29

if.end33:                                         ; preds = %if.start32
  %rbuf36 = load ptr, ptr %rbuf, align 8, !dbg !30
  %r37 = load i32, ptr %r, align 4, !dbg !30
  %ACCESS38 = getelementptr i8, ptr %rbuf36, i32 %r37, !dbg !30
  store i8 0, ptr %ACCESS38, align 1, !dbg !30
  call void @log(ptr @STR4), !dbg !31
  call void @log(ptr @STR5), !dbg !32
  %rbuf39 = load ptr, ptr %rbuf, align 8, !dbg !33
  call void @log(ptr %rbuf39), !dbg !33
  call void @log(ptr @STR6), !dbg !34
  call void @log(ptr @STR7), !dbg !28
  br label %while.start, !dbg !28

if.then34:                                        ; preds = %if.start32
  call void @log(ptr @STR3), !dbg !35
  br label %while.end, !dbg !35

while.start40:                                    ; preds = %if.end46, %if.end27
  br i1 true, label %while.then41, label %while.end42, !dbg !28

while.then41:                                     ; preds = %while.start40
  call void @log(ptr @STR8), !dbg !36
  %buf43 = load ptr, ptr %buf, align 8, !dbg !37
  %read44 = call i32 @read(i32 0, ptr %buf43, i32 1023), !dbg !37
  store i32 %read44, ptr %r, align 4, !dbg !37
  br label %if.start45, !dbg !37

while.end42:                                      ; preds = %if.then47, %while.start40
  %fd57 = load i32, ptr %fd, align 4, !dbg !38
  %close = call i32 @close(i32 %fd57), !dbg !38
  %buf58 = load ptr, ptr %buf, align 8, !dbg !39
  call void @free(ptr %buf58), !dbg !39
  %rbuf59 = load ptr, ptr %rbuf, align 8, !dbg !40
  call void @free(ptr %rbuf59), !dbg !40
  %addr60 = load ptr, ptr %addr, align 8, !dbg !41
  call void @free(ptr %addr60), !dbg !41
  ret i32 0, !dbg !41

if.start45:                                       ; preds = %while.then41
  %r48 = load i32, ptr %r, align 4, !dbg !37
  %LE49 = icmp sle i32 %r48, 0, !dbg !37
  br i1 %LE49, label %if.then47, label %if.end46, !dbg !37

if.end46:                                         ; preds = %if.start45
  %r50 = load i32, ptr %r, align 4, !dbg !42
  %SUB = sub i32 %r50, 1, !dbg !42
  %buf51 = load ptr, ptr %buf, align 8, !dbg !42
  %ACCESS52 = getelementptr i8, ptr %buf51, i32 %SUB, !dbg !42
  store i8 0, ptr %ACCESS52, align 1, !dbg !42
  %fd53 = load i32, ptr %fd, align 4, !dbg !43
  %buf54 = load ptr, ptr %buf, align 8, !dbg !43
  %r55 = load i32, ptr %r, align 4, !dbg !43
  %SUB56 = sub i32 %r55, 1, !dbg !43
  %write = call i32 @write(i32 %fd53, ptr %buf54, i32 %SUB56), !dbg !43
  br label %while.start40, !dbg !43

if.then47:                                        ; preds = %if.start45
  br label %while.end42, !dbg !37
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "client.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/net")
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
!17 = !DILocation(line: 17, scope: !9)
!18 = !DILocation(line: 18, scope: !9)
!19 = !DILocation(line: 19, scope: !9)
!20 = !DILocation(line: 20, scope: !9)
!21 = !DILocation(line: 22, scope: !9)
!22 = !DILocation(line: 9, scope: !9)
!23 = !DILocation(line: 27, scope: !9)
!24 = !DILocation(line: 29, scope: !9)
!25 = !DILocation(line: 30, scope: !9)
!26 = !DILocation(line: 33, scope: !9)
!27 = !DILocation(line: 24, scope: !9)
!28 = !DILocation(line: 45, scope: !9)
!29 = !DILocation(line: 36, scope: !9)
!30 = !DILocation(line: 40, scope: !9)
!31 = !DILocation(line: 41, scope: !9)
!32 = !DILocation(line: 42, scope: !9)
!33 = !DILocation(line: 43, scope: !9)
!34 = !DILocation(line: 44, scope: !9)
!35 = !DILocation(line: 38, scope: !9)
!36 = !DILocation(line: 49, scope: !9)
!37 = !DILocation(line: 50, scope: !9)
!38 = !DILocation(line: 56, scope: !9)
!39 = !DILocation(line: 57, scope: !9)
!40 = !DILocation(line: 58, scope: !9)
!41 = !DILocation(line: 59, scope: !9)
!42 = !DILocation(line: 53, scope: !9)
!43 = !DILocation(line: 54, scope: !9)
