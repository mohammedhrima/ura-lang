; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.String = type { i8*, i32, i32 }

@STR0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@STR1 = private unnamed_addr constant [6 x i8] c"12345\00", align 1
@STR2 = private unnamed_addr constant [5 x i8] c"6789\00", align 1
@STR3 = private unnamed_addr constant [8 x i8] c"value: \00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [11 x i8] c"value: %s\0A\00", align 1

declare i8* @calloc(i64, i64)

declare i8* @realloc(i8*, i32)

declare void @free(i8*)

declare i32 @strlen(i8*)

declare i8* @strcpy(i8*, i8*)

declare void @bzero(i8*, i64)

define i8* @strjoin(i8* %left, i8* %right) !dbg !4 {
entry:
  %left1 = alloca i8*, align 8, !dbg !7
  %right2 = alloca i8*, align 8, !dbg !7
  %len1 = alloca i32, align 4, !dbg !7
  %len2 = alloca i32, align 4, !dbg !7
  %str = alloca i8*, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  %j = alloca i32, align 4, !dbg !7
  store i8* %left, i8** %left1, align 8, !dbg !7
  store i8* %right, i8** %right2, align 8, !dbg !7
  store i32 0, i32* %len1, align 4, !dbg !7
  store i32 0, i32* %len1, align 4, !dbg !7
  store i32 0, i32* %len2, align 4, !dbg !7
  store i32 0, i32* %len2, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %left3 = load i8*, i8** %left1, align 8, !dbg !7
  %NEQ = icmp ne i8* %left3, null, !dbg !7
  br i1 %NEQ, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start5, !dbg !8

if.then:                                          ; preds = %if.start
  %left4 = load i8*, i8** %left1, align 8, !dbg !8
  %strlen = call i32 @strlen(i8* %left4), !dbg !8
  store i32 %strlen, i32* %len1, align 4, !dbg !8
  br label %if.end, !dbg !8

if.start5:                                        ; preds = %if.end
  %right8 = load i8*, i8** %right2, align 8, !dbg !8
  %NEQ9 = icmp ne i8* %right8, null, !dbg !8
  br i1 %NEQ9, label %if.then7, label %if.end6, !dbg !8

if.end6:                                          ; preds = %if.then7, %if.start5
  store i8* null, i8** %str, align 8, !dbg !7
  %len112 = load i32, i32* %len1, align 4, !dbg !9
  %len213 = load i32, i32* %len2, align 4, !dbg !9
  %ADD = add i32 %len112, %len213, !dbg !9
  %ADD14 = add i32 %ADD, 1, !dbg !9
  %as = sext i32 %ADD14 to i64, !dbg !9
  %calloc = call i8* @calloc(i64 %as, i64 1), !dbg !9
  store i8* %calloc, i8** %str, align 8, !dbg !9
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

if.then7:                                         ; preds = %if.start5
  %right10 = load i8*, i8** %right2, align 8, !dbg !10
  %strlen11 = call i32 @strlen(i8* %right10), !dbg !10
  store i32 %strlen11, i32* %len2, align 4, !dbg !10
  br label %if.end6, !dbg !10

while.start:                                      ; preds = %while.then, %if.end6
  %i15 = load i32, i32* %i, align 4, !dbg !7
  %len116 = load i32, i32* %len1, align 4, !dbg !7
  %LT = icmp slt i32 %i15, %len116, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %str17 = load i8*, i8** %str, align 8, !dbg !11
  %i18 = load i32, i32* %i, align 4, !dbg !11
  %ACCESS = getelementptr i8, i8* %str17, i32 %i18, !dbg !11
  %left19 = load i8*, i8** %left1, align 8, !dbg !11
  %i20 = load i32, i32* %i, align 4, !dbg !11
  %ACCESS21 = getelementptr i8, i8* %left19, i32 %i20, !dbg !11
  %ACC = load i8, i8* %ACCESS21, align 1, !dbg !11
  store i8 %ACC, i8* %ACCESS, align 1, !dbg !11
  %i22 = load i32, i32* %i, align 4, !dbg !11
  %ADD23 = add i32 %i22, 1, !dbg !11
  store i32 %ADD23, i32* %i, align 4, !dbg !11
  br label %while.start, !dbg !11

while.end:                                        ; preds = %while.start
  store i32 0, i32* %j, align 4, !dbg !7
  store i32 0, i32* %j, align 4, !dbg !7
  br label %while.start24, !dbg !7

while.start24:                                    ; preds = %while.then25, %while.end
  %j27 = load i32, i32* %j, align 4, !dbg !7
  %len228 = load i32, i32* %len2, align 4, !dbg !7
  %LT29 = icmp slt i32 %j27, %len228, !dbg !7
  br i1 %LT29, label %while.then25, label %while.end26, !dbg !7

while.then25:                                     ; preds = %while.start24
  %i30 = load i32, i32* %i, align 4, !dbg !12
  %j31 = load i32, i32* %j, align 4, !dbg !12
  %ADD32 = add i32 %i30, %j31, !dbg !12
  %str33 = load i8*, i8** %str, align 8, !dbg !12
  %ACCESS34 = getelementptr i8, i8* %str33, i32 %ADD32, !dbg !12
  %right35 = load i8*, i8** %right2, align 8, !dbg !12
  %j36 = load i32, i32* %j, align 4, !dbg !12
  %ACCESS37 = getelementptr i8, i8* %right35, i32 %j36, !dbg !12
  %ACC38 = load i8, i8* %ACCESS37, align 1, !dbg !12
  store i8 %ACC38, i8* %ACCESS34, align 1, !dbg !12
  %j39 = load i32, i32* %j, align 4, !dbg !12
  %ADD40 = add i32 %j39, 1, !dbg !12
  store i32 %ADD40, i32* %j, align 4, !dbg !12
  br label %while.start24, !dbg !12

while.end26:                                      ; preds = %while.start24
  %str41 = load i8*, i8** %str, align 8, !dbg !12
  ret i8* %str41, !dbg !12
}

define void @String.init(%struct.String* %self) !dbg !13 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !14
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !14
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !14
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !14
  %size = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 2, !dbg !14
  store i32 10, i32* %size, align 4, !dbg !14
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !14
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !14
  %calloc = call i8* @calloc(i64 10, i64 1), !dbg !15
  store i8* %calloc, i8** %value, align 8, !dbg !15
  ret void, !dbg !15
}

define void @String.clean(%struct.String* %self) !dbg !16 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !17
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !17
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !18
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !18
  %DOT = load i8*, i8** %value, align 8, !dbg !18
  call void @free(i8* %DOT), !dbg !18
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !18
  ret void, !dbg !18
}

define void @String.assign(i8* %str, %struct.String* %self) !dbg !19 {
entry:
  %str1 = alloca i8*, align 8, !dbg !20
  %self2 = alloca %struct.String*, align 8, !dbg !20
  %len = alloca i32, align 4, !dbg !20
  store i8* %str, i8** %str1, align 8, !dbg !20
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !20
  br label %if.start, !dbg !20

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !20
  %EQ = icmp eq i8* %str3, null, !dbg !20
  br i1 %EQ, label %if.then, label %if.end, !dbg !20

if.end:                                           ; preds = %if.then, %if.start
  store i32 0, i32* %len, align 4, !dbg !20
  %str4 = load i8*, i8** %str1, align 8, !dbg !21
  %strlen = call i32 @strlen(i8* %str4), !dbg !21
  store i32 %strlen, i32* %len, align 4, !dbg !21
  br label %if.start5, !dbg !21

if.then:                                          ; preds = %if.start
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @STR0, i32 0, i32 0), i8** %str1, align 8, !dbg !20
  br label %if.end, !dbg !20

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !21
  %size = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 2, !dbg !21
  %DOT = load i32, i32* %size, align 4, !dbg !21
  %len9 = load i32, i32* %len, align 4, !dbg !21
  %LT = icmp slt i32 %DOT, %len9, !dbg !21
  br i1 %LT, label %if.then7, label %if.end6, !dbg !21

if.end6:                                          ; preds = %while.end, %if.start5
  %self25 = load %struct.String*, %struct.String** %self2, align 8, !dbg !22
  %value26 = getelementptr %struct.String, %struct.String* %self25, i32 0, i32 0, !dbg !22
  %DOT27 = load i8*, i8** %value26, align 8, !dbg !22
  %str28 = load i8*, i8** %str1, align 8, !dbg !22
  %strcpy = call i8* @strcpy(i8* %DOT27, i8* %str28), !dbg !22
  %self29 = load %struct.String*, %struct.String** %self2, align 8, !dbg !23
  %value30 = getelementptr %struct.String, %struct.String* %self29, i32 0, i32 0, !dbg !23
  %DOT31 = load i8*, i8** %value30, align 8, !dbg !23
  %len32 = load i32, i32* %len, align 4, !dbg !23
  %idx = sext i32 %len32 to i64, !dbg !23
  %ADD = getelementptr i8, i8* %DOT31, i64 %idx, !dbg !23
  %self33 = load %struct.String*, %struct.String** %self2, align 8, !dbg !23
  %size34 = getelementptr %struct.String, %struct.String* %self33, i32 0, i32 2, !dbg !23
  %DOT35 = load i32, i32* %size34, align 4, !dbg !23
  %len36 = load i32, i32* %len, align 4, !dbg !23
  %SUB = sub i32 %DOT35, %len36, !dbg !23
  %as = sext i32 %SUB to i64, !dbg !23
  call void @bzero(i8* %ADD, i64 %as), !dbg !23
  %self37 = load %struct.String*, %struct.String** %self2, align 8, !dbg !23
  %count = getelementptr %struct.String, %struct.String* %self37, i32 0, i32 1, !dbg !23
  %len38 = load i32, i32* %len, align 4, !dbg !23
  store i32 %len38, i32* %count, align 4, !dbg !23
  ret void, !dbg !23

if.then7:                                         ; preds = %if.start5
  br label %while.start, !dbg !21

while.start:                                      ; preds = %while.then, %if.then7
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !21
  %size11 = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 2, !dbg !21
  %DOT12 = load i32, i32* %size11, align 4, !dbg !21
  %len13 = load i32, i32* %len, align 4, !dbg !21
  %LT14 = icmp slt i32 %DOT12, %len13, !dbg !21
  br i1 %LT14, label %while.then, label %while.end, !dbg !21

while.then:                                       ; preds = %while.start
  %self15 = load %struct.String*, %struct.String** %self2, align 8, !dbg !21
  %size16 = getelementptr %struct.String, %struct.String* %self15, i32 0, i32 2, !dbg !21
  %DOT17 = load i32, i32* %size16, align 4, !dbg !21
  %MUL = mul i32 %DOT17, 2, !dbg !21
  store i32 %MUL, i32* %size16, align 4, !dbg !21
  br label %while.start, !dbg !21

while.end:                                        ; preds = %while.start
  %self18 = load %struct.String*, %struct.String** %self2, align 8, !dbg !21
  %value = getelementptr %struct.String, %struct.String* %self18, i32 0, i32 0, !dbg !21
  %self19 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %value20 = getelementptr %struct.String, %struct.String* %self19, i32 0, i32 0, !dbg !24
  %DOT21 = load i8*, i8** %value20, align 8, !dbg !24
  %self22 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %size23 = getelementptr %struct.String, %struct.String* %self22, i32 0, i32 2, !dbg !24
  %DOT24 = load i32, i32* %size23, align 4, !dbg !24
  %realloc = call i8* @realloc(i8* %DOT21, i32 %DOT24), !dbg !24
  store i8* %realloc, i8** %value, align 8, !dbg !24
  br label %if.end6, !dbg !24
}

define void @String.join(i8* %str, %struct.String* %self) !dbg !25 {
entry:
  %str1 = alloca i8*, align 8, !dbg !26
  %self2 = alloca %struct.String*, align 8, !dbg !26
  %res = alloca i8*, align 8, !dbg !26
  store i8* %str, i8** %str1, align 8, !dbg !26
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !26
  br label %if.start, !dbg !26

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !26
  %EQ = icmp eq i8* %str3, null, !dbg !26
  br i1 %EQ, label %if.then, label %if.end, !dbg !26

if.end:                                           ; preds = %if.start
  store i8* null, i8** %res, align 8, !dbg !26
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !27
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !27
  %DOT = load i8*, i8** %value, align 8, !dbg !27
  %str5 = load i8*, i8** %str1, align 8, !dbg !27
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str5), !dbg !27
  store i8* %strjoin, i8** %res, align 8, !dbg !27
  %res6 = load i8*, i8** %res, align 8, !dbg !28
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !28
  call void @String.assign(i8* %res6, %struct.String* %ref_arg), !dbg !28
  %res7 = load i8*, i8** %res, align 8, !dbg !29
  call void @free(i8* %res7), !dbg !29
  ret void, !dbg !29

if.then:                                          ; preds = %if.start
  ret void, !dbg !26
}

define void @String.operator.ASSIGN.CHARS(i8* %str, %struct.String* %self) !dbg !30 {
entry:
  %str1 = alloca i8*, align 8, !dbg !31
  %self2 = alloca %struct.String*, align 8, !dbg !31
  store i8* %str, i8** %str1, align 8, !dbg !31
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !31
  %str3 = load i8*, i8** %str1, align 8, !dbg !32
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !32
  call void @String.assign(i8* %str3, %struct.String* %ref_arg), !dbg !32
  ret void, !dbg !32
}

define void @String.operator.ASSIGN.String(%struct.String* %v, %struct.String* %self) !dbg !33 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !34
  %self2 = alloca %struct.String*, align 8, !dbg !34
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !34
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !34
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !35
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !35
  %DOT = load i8*, i8** %value, align 8, !dbg !35
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !35
  call void @String.assign(i8* %DOT, %struct.String* %ref_arg), !dbg !35
  ret void, !dbg !35
}

define %struct.String @String.operator.ADD.String(%struct.String* %v, %struct.String* %self) !dbg !36 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !37
  %self2 = alloca %struct.String*, align 8, !dbg !37
  %res = alloca %struct.String, align 8, !dbg !37
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !37
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !37
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !37
  call void @String.init(%struct.String* %res), !dbg !37
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !38
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !38
  %DOT = load i8*, i8** %value, align 8, !dbg !38
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !38
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !38
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !38
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %DOT6), !dbg !38
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !38
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !38
  ret %struct.String %res7, !dbg !38
}

define i32 @main() !dbg !39 {
entry:
  %s0 = alloca %struct.String, align 8, !dbg !40
  %s1 = alloca %struct.String, align 8, !dbg !40
  %s2 = alloca %struct.String, align 8, !dbg !40
  store %struct.String zeroinitializer, %struct.String* %s0, align 8, !dbg !40
  call void @String.init(%struct.String* %s0), !dbg !40
  call void @String.operator.ASSIGN.CHARS(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR1, i32 0, i32 0), %struct.String* %s0), !dbg !40
  store %struct.String zeroinitializer, %struct.String* %s1, align 8, !dbg !40
  call void @String.init(%struct.String* %s1), !dbg !40
  call void @String.operator.ASSIGN.CHARS(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR2, i32 0, i32 0), %struct.String* %s1), !dbg !40
  store %struct.String zeroinitializer, %struct.String* %s2, align 8, !dbg !40
  call void @String.init(%struct.String* %s2), !dbg !40
  %0 = call %struct.String @String.operator.ADD.String(%struct.String* %s1, %struct.String* %s0), !dbg !40
  %tmp_op = alloca %struct.String, align 8, !dbg !40
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !40
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s2), !dbg !40
  %value = getelementptr %struct.String, %struct.String* %s2, i32 0, i32 0, !dbg !40
  %DOT = load i8*, i8** %value, align 8, !dbg !40
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %DOT), !dbg !40
  call void @String.clean(%struct.String* %s0), !dbg !40
  call void @String.clean(%struct.String* %s1), !dbg !40
  call void @String.clean(%struct.String* %s2), !dbg !40
  ret i32 0, !dbg !40
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "strjoin", linkageName: "strjoin", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
!9 = !DILocation(line: 9, scope: !4)
!10 = !DILocation(line: 8, scope: !4)
!11 = !DILocation(line: 12, scope: !4)
!12 = !DILocation(line: 16, scope: !4)
!13 = distinct !DISubprogram(name: "String.init", linkageName: "String.init", scope: null, file: !3, line: 25, type: !5, scopeLine: 25, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 25, scope: !13)
!15 = !DILocation(line: 27, scope: !13)
!16 = distinct !DISubprogram(name: "String.clean", linkageName: "String.clean", scope: null, file: !3, line: 29, type: !5, scopeLine: 29, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 29, scope: !16)
!18 = !DILocation(line: 30, scope: !16)
!19 = distinct !DISubprogram(name: "String.assign", linkageName: "String.assign", scope: null, file: !3, line: 32, type: !5, scopeLine: 32, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!20 = !DILocation(line: 32, scope: !19)
!21 = !DILocation(line: 34, scope: !19)
!22 = !DILocation(line: 39, scope: !19)
!23 = !DILocation(line: 40, scope: !19)
!24 = !DILocation(line: 38, scope: !19)
!25 = distinct !DISubprogram(name: "String.join", linkageName: "String.join", scope: null, file: !3, line: 43, type: !5, scopeLine: 43, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!26 = !DILocation(line: 43, scope: !25)
!27 = !DILocation(line: 45, scope: !25)
!28 = !DILocation(line: 46, scope: !25)
!29 = !DILocation(line: 47, scope: !25)
!30 = distinct !DISubprogram(name: "String.operator.ASSIGN.CHARS", linkageName: "String.operator.ASSIGN.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!31 = !DILocation(line: 0, scope: !30)
!32 = !DILocation(line: 50, scope: !30)
!33 = distinct !DISubprogram(name: "String.operator.ASSIGN.String", linkageName: "String.operator.ASSIGN.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!34 = !DILocation(line: 0, scope: !33)
!35 = !DILocation(line: 53, scope: !33)
!36 = distinct !DISubprogram(name: "String.operator.ADD.String", linkageName: "String.operator.ADD.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!37 = !DILocation(line: 0, scope: !36)
!38 = !DILocation(line: 56, scope: !36)
!39 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 60, type: !5, scopeLine: 60, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!40 = !DILocation(line: 60, scope: !39)
