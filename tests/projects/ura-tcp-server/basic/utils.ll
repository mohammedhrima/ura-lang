; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/projects/ura-tcp-server/basic/utils.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/projects/ura-tcp-server/basic/utils.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.String = type { i8*, i32, i32 }
%struct.SockAddr = type { i8* }

@STR0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@STR1 = private unnamed_addr constant [9 x i8] c"%H:%M:%S\00", align 1

define void @String.delete(%struct.String* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !7
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !7
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !8
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !8
  %DOT = load i8*, i8** %value, align 8, !dbg !8
  call void @free(i8* %DOT), !dbg !8
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !8
  ret void, !dbg !8
}

define void @String.assign(i8* %str, %struct.String* %self) !dbg !9 {
entry:
  %str1 = alloca i8*, align 8, !dbg !10
  %self2 = alloca %struct.String*, align 8, !dbg !10
  %len = alloca i32, align 4, !dbg !10
  store i8* %str, i8** %str1, align 8, !dbg !10
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !10
  br label %if.start, !dbg !10

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !10
  %EQ = icmp eq i8* %str3, null, !dbg !10
  br i1 %EQ, label %if.then, label %if.end, !dbg !10

if.end:                                           ; preds = %if.then, %if.start
  store i32 0, i32* %len, align 4, !dbg !10
  %str4 = load i8*, i8** %str1, align 8, !dbg !11
  %strlen = call i32 @strlen(i8* %str4), !dbg !11
  store i32 %strlen, i32* %len, align 4, !dbg !11
  br label %if.start5, !dbg !11

if.then:                                          ; preds = %if.start
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @STR0, i32 0, i32 0), i8** %str1, align 8, !dbg !10
  br label %if.end, !dbg !10

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !11
  %value = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 0, !dbg !11
  %DOT = load i8*, i8** %value, align 8, !dbg !11
  %EQ9 = icmp eq i8* %DOT, null, !dbg !11
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !11
  %size = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 2, !dbg !11
  %DOT11 = load i32, i32* %size, align 4, !dbg !11
  %len12 = load i32, i32* %len, align 4, !dbg !11
  %LE = icmp sle i32 %DOT11, %len12, !dbg !11
  %OR = or i1 %EQ9, %LE, !dbg !11
  br i1 %OR, label %if.then7, label %if.end6, !dbg !11

if.end6:                                          ; preds = %while.end, %if.start5
  %self38 = load %struct.String*, %struct.String** %self2, align 8, !dbg !12
  %value39 = getelementptr %struct.String, %struct.String* %self38, i32 0, i32 0, !dbg !12
  %DOT40 = load i8*, i8** %value39, align 8, !dbg !12
  %str41 = load i8*, i8** %str1, align 8, !dbg !12
  %strcpy = call i8* @strcpy(i8* %DOT40, i8* %str41), !dbg !12
  %self42 = load %struct.String*, %struct.String** %self2, align 8, !dbg !13
  %value43 = getelementptr %struct.String, %struct.String* %self42, i32 0, i32 0, !dbg !13
  %DOT44 = load i8*, i8** %value43, align 8, !dbg !13
  %len45 = load i32, i32* %len, align 4, !dbg !13
  %idx = sext i32 %len45 to i64, !dbg !13
  %ADD = getelementptr i8, i8* %DOT44, i64 %idx, !dbg !13
  %self46 = load %struct.String*, %struct.String** %self2, align 8, !dbg !13
  %size47 = getelementptr %struct.String, %struct.String* %self46, i32 0, i32 2, !dbg !13
  %DOT48 = load i32, i32* %size47, align 4, !dbg !13
  %len49 = load i32, i32* %len, align 4, !dbg !13
  %SUB = sub i32 %DOT48, %len49, !dbg !13
  %as = sext i32 %SUB to i64, !dbg !13
  call void @bzero(i8* %ADD, i64 %as), !dbg !13
  %self50 = load %struct.String*, %struct.String** %self2, align 8, !dbg !13
  %count = getelementptr %struct.String, %struct.String* %self50, i32 0, i32 1, !dbg !13
  %len51 = load i32, i32* %len, align 4, !dbg !13
  store i32 %len51, i32* %count, align 4, !dbg !13
  ret void, !dbg !13

if.then7:                                         ; preds = %if.start5
  br label %if.start13, !dbg !11

if.start13:                                       ; preds = %if.then7
  %self16 = load %struct.String*, %struct.String** %self2, align 8, !dbg !11
  %size17 = getelementptr %struct.String, %struct.String* %self16, i32 0, i32 2, !dbg !11
  %DOT18 = load i32, i32* %size17, align 4, !dbg !11
  %EQ19 = icmp eq i32 %DOT18, 0, !dbg !11
  br i1 %EQ19, label %if.then15, label %if.end14, !dbg !11

if.end14:                                         ; preds = %if.then15, %if.start13
  br label %while.start, !dbg !11

if.then15:                                        ; preds = %if.start13
  %self20 = load %struct.String*, %struct.String** %self2, align 8, !dbg !11
  %size21 = getelementptr %struct.String, %struct.String* %self20, i32 0, i32 2, !dbg !11
  store i32 10, i32* %size21, align 4, !dbg !11
  br label %if.end14, !dbg !11

while.start:                                      ; preds = %while.then, %if.end14
  %self22 = load %struct.String*, %struct.String** %self2, align 8, !dbg !11
  %size23 = getelementptr %struct.String, %struct.String* %self22, i32 0, i32 2, !dbg !11
  %DOT24 = load i32, i32* %size23, align 4, !dbg !11
  %len25 = load i32, i32* %len, align 4, !dbg !11
  %LE26 = icmp sle i32 %DOT24, %len25, !dbg !11
  br i1 %LE26, label %while.then, label %while.end, !dbg !11

while.then:                                       ; preds = %while.start
  %self27 = load %struct.String*, %struct.String** %self2, align 8, !dbg !11
  %size28 = getelementptr %struct.String, %struct.String* %self27, i32 0, i32 2, !dbg !11
  %DOT29 = load i32, i32* %size28, align 4, !dbg !11
  %MUL = mul i32 %DOT29, 2, !dbg !11
  store i32 %MUL, i32* %size28, align 4, !dbg !11
  br label %while.start, !dbg !11

while.end:                                        ; preds = %while.start
  %self30 = load %struct.String*, %struct.String** %self2, align 8, !dbg !11
  %value31 = getelementptr %struct.String, %struct.String* %self30, i32 0, i32 0, !dbg !11
  %self32 = load %struct.String*, %struct.String** %self2, align 8, !dbg !14
  %value33 = getelementptr %struct.String, %struct.String* %self32, i32 0, i32 0, !dbg !14
  %DOT34 = load i8*, i8** %value33, align 8, !dbg !14
  %self35 = load %struct.String*, %struct.String** %self2, align 8, !dbg !14
  %size36 = getelementptr %struct.String, %struct.String* %self35, i32 0, i32 2, !dbg !14
  %DOT37 = load i32, i32* %size36, align 4, !dbg !14
  %realloc = call i8* @realloc(i8* %DOT34, i32 %DOT37), !dbg !14
  store i8* %realloc, i8** %value31, align 8, !dbg !14
  br label %if.end6, !dbg !14
}

define void @String.join(i8* %str, %struct.String* %self) !dbg !15 {
entry:
  %str1 = alloca i8*, align 8, !dbg !16
  %self2 = alloca %struct.String*, align 8, !dbg !16
  %res = alloca i8*, align 8, !dbg !16
  store i8* %str, i8** %str1, align 8, !dbg !16
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !16
  br label %if.start, !dbg !16

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !16
  %EQ = icmp eq i8* %str3, null, !dbg !16
  br i1 %EQ, label %if.then, label %if.end, !dbg !16

if.end:                                           ; preds = %if.start
  store i8* null, i8** %res, align 8, !dbg !16
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !17
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !17
  %DOT = load i8*, i8** %value, align 8, !dbg !17
  %str5 = load i8*, i8** %str1, align 8, !dbg !17
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str5), !dbg !17
  store i8* %strjoin, i8** %res, align 8, !dbg !17
  %res6 = load i8*, i8** %res, align 8, !dbg !18
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !18
  call void @String.assign(i8* %res6, %struct.String* %ref_arg), !dbg !18
  %res7 = load i8*, i8** %res, align 8, !dbg !19
  call void @free(i8* %res7), !dbg !19
  ret void, !dbg !19

if.then:                                          ; preds = %if.start
  ret void, !dbg !16
}

define void @String.clear(%struct.String* %self) !dbg !20 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !21
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !21
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !22
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !22
  %DOT = load i8*, i8** %value, align 8, !dbg !22
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !22
  %size = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 2, !dbg !22
  %DOT4 = load i32, i32* %size, align 4, !dbg !22
  %as = sext i32 %DOT4 to i64, !dbg !22
  call void @bzero(i8* %DOT, i64 %as), !dbg !22
  %self5 = load %struct.String*, %struct.String** %self1, align 8, !dbg !22
  %count = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 1, !dbg !22
  store i32 0, i32* %count, align 4, !dbg !22
  ret void, !dbg !22
}

define void @String.push(i8 %c, %struct.String* %self) !dbg !23 {
entry:
  %c1 = alloca i8, align 1, !dbg !24
  %self2 = alloca %struct.String*, align 8, !dbg !24
  store i8 %c, i8* %c1, align 1, !dbg !24
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !24
  br label %if.start, !dbg !24

if.start:                                         ; preds = %entry
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %count = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 1, !dbg !24
  %DOT = load i32, i32* %count, align 4, !dbg !24
  %ADD = add i32 %DOT, 1, !dbg !24
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %size = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 2, !dbg !24
  %DOT5 = load i32, i32* %size, align 4, !dbg !24
  %GE = icmp sge i32 %ADD, %DOT5, !dbg !24
  br i1 %GE, label %if.then, label %if.end, !dbg !24

if.end:                                           ; preds = %if.end7, %if.start
  %self24 = load %struct.String*, %struct.String** %self2, align 8, !dbg !25
  %value25 = getelementptr %struct.String, %struct.String* %self24, i32 0, i32 0, !dbg !25
  %self26 = load %struct.String*, %struct.String** %self2, align 8, !dbg !25
  %count27 = getelementptr %struct.String, %struct.String* %self26, i32 0, i32 1, !dbg !25
  %DOT28 = load i8*, i8** %value25, align 8, !dbg !25
  %DOT29 = load i32, i32* %count27, align 4, !dbg !25
  %ACCESS = getelementptr i8, i8* %DOT28, i32 %DOT29, !dbg !25
  %c30 = load i8, i8* %c1, align 1, !dbg !25
  store i8 %c30, i8* %ACCESS, align 1, !dbg !25
  %self31 = load %struct.String*, %struct.String** %self2, align 8, !dbg !25
  %count32 = getelementptr %struct.String, %struct.String* %self31, i32 0, i32 1, !dbg !25
  %DOT33 = load i32, i32* %count32, align 4, !dbg !25
  %ADD34 = add i32 %DOT33, 1, !dbg !25
  store i32 %ADD34, i32* %count32, align 4, !dbg !25
  %self35 = load %struct.String*, %struct.String** %self2, align 8, !dbg !26
  %value36 = getelementptr %struct.String, %struct.String* %self35, i32 0, i32 0, !dbg !26
  %self37 = load %struct.String*, %struct.String** %self2, align 8, !dbg !26
  %count38 = getelementptr %struct.String, %struct.String* %self37, i32 0, i32 1, !dbg !26
  %DOT39 = load i8*, i8** %value36, align 8, !dbg !26
  %DOT40 = load i32, i32* %count38, align 4, !dbg !26
  %ACCESS41 = getelementptr i8, i8* %DOT39, i32 %DOT40, !dbg !26
  store i8 0, i8* %ACCESS41, align 1, !dbg !26
  ret void, !dbg !26

if.then:                                          ; preds = %if.start
  br label %if.start6, !dbg !24

if.start6:                                        ; preds = %if.then
  %self9 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %size10 = getelementptr %struct.String, %struct.String* %self9, i32 0, i32 2, !dbg !24
  %DOT11 = load i32, i32* %size10, align 4, !dbg !24
  %EQ = icmp eq i32 %DOT11, 0, !dbg !24
  br i1 %EQ, label %if.then8, label %if.else, !dbg !24

if.end7:                                          ; preds = %if.else, %if.then8
  %self17 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %value = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 0, !dbg !24
  %self18 = load %struct.String*, %struct.String** %self2, align 8, !dbg !27
  %value19 = getelementptr %struct.String, %struct.String* %self18, i32 0, i32 0, !dbg !27
  %DOT20 = load i8*, i8** %value19, align 8, !dbg !27
  %self21 = load %struct.String*, %struct.String** %self2, align 8, !dbg !27
  %size22 = getelementptr %struct.String, %struct.String* %self21, i32 0, i32 2, !dbg !27
  %DOT23 = load i32, i32* %size22, align 4, !dbg !27
  %realloc = call i8* @realloc(i8* %DOT20, i32 %DOT23), !dbg !27
  store i8* %realloc, i8** %value, align 8, !dbg !27
  br label %if.end, !dbg !27

if.then8:                                         ; preds = %if.start6
  %self12 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %size13 = getelementptr %struct.String, %struct.String* %self12, i32 0, i32 2, !dbg !24
  store i32 10, i32* %size13, align 4, !dbg !24
  br label %if.end7, !dbg !24

if.else:                                          ; preds = %if.start6
  %self14 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %size15 = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 2, !dbg !24
  %DOT16 = load i32, i32* %size15, align 4, !dbg !24
  %MUL = mul i32 %DOT16, 2, !dbg !24
  store i32 %MUL, i32* %size15, align 4, !dbg !24
  br label %if.end7, !dbg !24
}

define i8 @String.pop(%struct.String* %self) !dbg !28 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !29
  %c = alloca i8, align 1, !dbg !29
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !29
  br label %if.start, !dbg !29

if.start:                                         ; preds = %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !29
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !29
  %DOT = load i32, i32* %count, align 4, !dbg !29
  %EQ = icmp eq i32 %DOT, 0, !dbg !29
  br i1 %EQ, label %if.then, label %if.end, !dbg !29

if.end:                                           ; preds = %if.start
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !29
  %count4 = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 1, !dbg !29
  %DOT5 = load i32, i32* %count4, align 4, !dbg !29
  %SUB = sub i32 %DOT5, 1, !dbg !29
  store i32 %SUB, i32* %count4, align 4, !dbg !29
  store i8 0, i8* %c, align 1, !dbg !29
  %self6 = load %struct.String*, %struct.String** %self1, align 8, !dbg !30
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !30
  %self7 = load %struct.String*, %struct.String** %self1, align 8, !dbg !30
  %count8 = getelementptr %struct.String, %struct.String* %self7, i32 0, i32 1, !dbg !30
  %DOT9 = load i8*, i8** %value, align 8, !dbg !30
  %DOT10 = load i32, i32* %count8, align 4, !dbg !30
  %ACCESS = getelementptr i8, i8* %DOT9, i32 %DOT10, !dbg !30
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !30
  store i8 %ACC, i8* %c, align 1, !dbg !30
  %self11 = load %struct.String*, %struct.String** %self1, align 8, !dbg !31
  %value12 = getelementptr %struct.String, %struct.String* %self11, i32 0, i32 0, !dbg !31
  %self13 = load %struct.String*, %struct.String** %self1, align 8, !dbg !31
  %count14 = getelementptr %struct.String, %struct.String* %self13, i32 0, i32 1, !dbg !31
  %DOT15 = load i8*, i8** %value12, align 8, !dbg !31
  %DOT16 = load i32, i32* %count14, align 4, !dbg !31
  %ACCESS17 = getelementptr i8, i8* %DOT15, i32 %DOT16, !dbg !31
  store i8 0, i8* %ACCESS17, align 1, !dbg !31
  %c18 = load i8, i8* %c, align 1, !dbg !31
  ret i8 %c18, !dbg !31

if.then:                                          ; preds = %if.start
  ret i8 0, !dbg !29
}

define %struct.String @String.new() !dbg !32 {
entry:
  %s = alloca %struct.String, align 8, !dbg !33
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !33
  %size = getelementptr %struct.String, %struct.String* %s, i32 0, i32 2, !dbg !33
  store i32 10, i32* %size, align 4, !dbg !33
  %value = getelementptr %struct.String, %struct.String* %s, i32 0, i32 0, !dbg !33
  %calloc = call i8* @calloc(i64 10, i64 1), !dbg !34
  store i8* %calloc, i8** %value, align 8, !dbg !34
  %s1 = load %struct.String, %struct.String* %s, align 8, !dbg !34
  ret %struct.String %s1, !dbg !34
}

define %struct.String @String.from(i8* %str) !dbg !35 {
entry:
  %str1 = alloca i8*, align 8, !dbg !36
  %s = alloca %struct.String, align 8, !dbg !36
  store i8* %str, i8** %str1, align 8, !dbg !36
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !36
  %0 = call %struct.String @String.new(), !dbg !37
  %tmp_struct = alloca %struct.String, align 8, !dbg !37
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !37
  %tmp_op = alloca %struct.String, align 8, !dbg !37
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !37
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s), !dbg !37
  %str2 = load i8*, i8** %str1, align 8, !dbg !38
  call void @String.assign(i8* %str2, %struct.String* %s), !dbg !38
  call void @String.delete(%struct.String* %tmp_struct), !dbg !38
  %s3 = load %struct.String, %struct.String* %s, align 8, !dbg !38
  ret %struct.String %s3, !dbg !38
}

define %struct.String @String.from_int(i32 %n) !dbg !39 {
entry:
  %n1 = alloca i32, align 4, !dbg !40
  %s = alloca %struct.String, align 8, !dbg !40
  %neg = alloca i1, align 1, !dbg !40
  %num = alloca i32, align 4, !dbg !40
  %buf = alloca i8*, align 8, !dbg !40
  %i = alloca i32, align 4, !dbg !40
  %j = alloca i32, align 4, !dbg !40
  store i32 %n, i32* %n1, align 4, !dbg !40
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !40
  %0 = call %struct.String @String.new(), !dbg !41
  %tmp_struct = alloca %struct.String, align 8, !dbg !41
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !41
  %tmp_op = alloca %struct.String, align 8, !dbg !41
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !41
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s), !dbg !41
  br label %if.start, !dbg !41

if.start:                                         ; preds = %entry
  %n2 = load i32, i32* %n1, align 4, !dbg !41
  %EQ = icmp eq i32 %n2, 0, !dbg !41
  br i1 %EQ, label %if.then, label %if.end, !dbg !41

if.end:                                           ; preds = %if.start
  store i1 false, i1* %neg, align 1, !dbg !40
  store i1 false, i1* %neg, align 1, !dbg !40
  store i32 0, i32* %num, align 4, !dbg !40
  %n4 = load i32, i32* %n1, align 4, !dbg !40
  store i32 %n4, i32* %num, align 4, !dbg !40
  br label %if.start5, !dbg !40

if.then:                                          ; preds = %if.start
  call void @String.push(i8 48, %struct.String* %s), !dbg !42
  %s3 = load %struct.String, %struct.String* %s, align 8, !dbg !42
  ret %struct.String %s3, !dbg !42

if.start5:                                        ; preds = %if.end
  %num8 = load i32, i32* %num, align 4, !dbg !40
  %LT = icmp slt i32 %num8, 0, !dbg !40
  br i1 %LT, label %if.then7, label %if.end6, !dbg !40

if.end6:                                          ; preds = %if.then7, %if.start5
  store i8* null, i8** %buf, align 8, !dbg !40
  %stack = alloca [20 x i8], align 1, !dbg !40
  %stack10 = getelementptr [20 x i8], [20 x i8]* %stack, i32 0, i32 0, !dbg !40
  store i8* %stack10, i8** %buf, align 8, !dbg !40
  store i32 0, i32* %i, align 4, !dbg !40
  store i32 0, i32* %i, align 4, !dbg !40
  br label %while.start, !dbg !40

if.then7:                                         ; preds = %if.start5
  store i1 true, i1* %neg, align 1, !dbg !40
  %num9 = load i32, i32* %num, align 4, !dbg !40
  %SUB = sub i32 0, %num9, !dbg !40
  store i32 %SUB, i32* %num, align 4, !dbg !40
  br label %if.end6, !dbg !40

while.start:                                      ; preds = %while.then, %if.end6
  %num11 = load i32, i32* %num, align 4, !dbg !40
  %GT = icmp sgt i32 %num11, 0, !dbg !40
  br i1 %GT, label %while.then, label %while.end, !dbg !40

while.then:                                       ; preds = %while.start
  %buf12 = load i8*, i8** %buf, align 8, !dbg !43
  %i13 = load i32, i32* %i, align 4, !dbg !43
  %ACCESS = getelementptr i8, i8* %buf12, i32 %i13, !dbg !43
  %num14 = load i32, i32* %num, align 4, !dbg !43
  %MOD = srem i32 %num14, 10, !dbg !43
  %ADD = add i32 %MOD, 48, !dbg !43
  %as = trunc i32 %ADD to i8, !dbg !43
  store i8 %as, i8* %ACCESS, align 1, !dbg !43
  %num15 = load i32, i32* %num, align 4, !dbg !43
  %DIV = sdiv i32 %num15, 10, !dbg !43
  store i32 %DIV, i32* %num, align 4, !dbg !43
  %i16 = load i32, i32* %i, align 4, !dbg !43
  %ADD17 = add i32 %i16, 1, !dbg !43
  store i32 %ADD17, i32* %i, align 4, !dbg !43
  br label %while.start, !dbg !43

while.end:                                        ; preds = %while.start
  br label %if.start18, !dbg !43

if.start18:                                       ; preds = %while.end
  %neg21 = load i1, i1* %neg, align 1, !dbg !43
  br i1 %neg21, label %if.then20, label %if.end19, !dbg !43

if.end19:                                         ; preds = %if.then20, %if.start18
  store i32 0, i32* %j, align 4, !dbg !40
  %i22 = load i32, i32* %i, align 4, !dbg !40
  %SUB23 = sub i32 %i22, 1, !dbg !40
  store i32 %SUB23, i32* %j, align 4, !dbg !40
  br label %while.start24, !dbg !40

if.then20:                                        ; preds = %if.start18
  call void @String.push(i8 45, %struct.String* %s), !dbg !44
  br label %if.end19, !dbg !44

while.start24:                                    ; preds = %while.then25, %if.end19
  %j27 = load i32, i32* %j, align 4, !dbg !40
  %GE = icmp sge i32 %j27, 0, !dbg !40
  br i1 %GE, label %while.then25, label %while.end26, !dbg !40

while.then25:                                     ; preds = %while.start24
  %buf28 = load i8*, i8** %buf, align 8, !dbg !45
  %j29 = load i32, i32* %j, align 4, !dbg !45
  %ACCESS30 = getelementptr i8, i8* %buf28, i32 %j29, !dbg !45
  %ACC = load i8, i8* %ACCESS30, align 1, !dbg !45
  call void @String.push(i8 %ACC, %struct.String* %s), !dbg !45
  %j31 = load i32, i32* %j, align 4, !dbg !45
  %SUB32 = sub i32 %j31, 1, !dbg !45
  store i32 %SUB32, i32* %j, align 4, !dbg !45
  br label %while.start24, !dbg !45

while.end26:                                      ; preds = %while.start24
  call void @String.delete(%struct.String* %tmp_struct), !dbg !45
  %s33 = load %struct.String, %struct.String* %s, align 8, !dbg !45
  ret %struct.String %s33, !dbg !45
}

define i32 @String.len(%struct.String* %self) !dbg !46 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !47
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !47
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !47
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !47
  %DOT = load i32, i32* %count, align 4, !dbg !47
  ret i32 %DOT, !dbg !47
}

define i1 @String.empty(%struct.String* %self) !dbg !48 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !49
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !49
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !49
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !49
  %DOT = load i32, i32* %count, align 4, !dbg !49
  %EQ = icmp eq i32 %DOT, 0, !dbg !49
  ret i1 %EQ, !dbg !49
}

define i8* @String.c_str(%struct.String* %self) !dbg !50 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !51
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !51
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !51
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !51
  %DOT = load i8*, i8** %value, align 8, !dbg !51
  ret i8* %DOT, !dbg !51
}

define i8 @String.at(i32 %i, %struct.String* %self) !dbg !52 {
entry:
  %i1 = alloca i32, align 4, !dbg !53
  %self2 = alloca %struct.String*, align 8, !dbg !53
  store i32 %i, i32* %i1, align 4, !dbg !53
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !53
  br label %if.start, !dbg !53

if.start:                                         ; preds = %entry
  %i3 = load i32, i32* %i1, align 4, !dbg !53
  %LT = icmp slt i32 %i3, 0, !dbg !53
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !53
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !53
  %i5 = load i32, i32* %i1, align 4, !dbg !53
  %DOT = load i32, i32* %count, align 4, !dbg !53
  %GE = icmp sge i32 %i5, %DOT, !dbg !53
  %OR = or i1 %LT, %GE, !dbg !53
  br i1 %OR, label %if.then, label %if.end, !dbg !53

if.end:                                           ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self2, align 8, !dbg !54
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !54
  %DOT7 = load i8*, i8** %value, align 8, !dbg !54
  %i8 = load i32, i32* %i1, align 4, !dbg !54
  %ACCESS = getelementptr i8, i8* %DOT7, i32 %i8, !dbg !54
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !54
  ret i8 %ACC, !dbg !54

if.then:                                          ; preds = %if.start
  ret i8 0, !dbg !53
}

define i32 @String.find(i8* %needle, %struct.String* %self) !dbg !55 {
entry:
  %needle1 = alloca i8*, align 8, !dbg !56
  %self2 = alloca %struct.String*, align 8, !dbg !56
  %i = alloca i32, align 4, !dbg !56
  %nlen = alloca i32, align 4, !dbg !56
  store i8* %needle, i8** %needle1, align 8, !dbg !56
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !56
  br label %if.start, !dbg !56

if.start:                                         ; preds = %entry
  %needle3 = load i8*, i8** %needle1, align 8, !dbg !56
  %EQ = icmp eq i8* %needle3, null, !dbg !56
  br i1 %EQ, label %if.then, label %if.end, !dbg !56

if.end:                                           ; preds = %if.start
  store i32 0, i32* %i, align 4, !dbg !56
  store i32 0, i32* %i, align 4, !dbg !56
  store i32 0, i32* %nlen, align 4, !dbg !56
  %needle4 = load i8*, i8** %needle1, align 8, !dbg !57
  %strlen = call i32 @strlen(i8* %needle4), !dbg !57
  store i32 %strlen, i32* %nlen, align 4, !dbg !57
  br label %if.start5, !dbg !57

if.then:                                          ; preds = %if.start
  ret i32 -1, !dbg !56

if.start5:                                        ; preds = %if.end
  %nlen8 = load i32, i32* %nlen, align 4, !dbg !57
  %EQ9 = icmp eq i32 %nlen8, 0, !dbg !57
  br i1 %EQ9, label %if.then7, label %if.end6, !dbg !57

if.end6:                                          ; preds = %if.start5
  br label %while.start, !dbg !57

if.then7:                                         ; preds = %if.start5
  ret i32 0, !dbg !57

while.start:                                      ; preds = %if.end14, %if.end6
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !57
  %count = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 1, !dbg !57
  %DOT = load i32, i32* %count, align 4, !dbg !57
  %nlen11 = load i32, i32* %nlen, align 4, !dbg !57
  %SUB = sub i32 %DOT, %nlen11, !dbg !57
  %i12 = load i32, i32* %i, align 4, !dbg !57
  %LE = icmp sle i32 %i12, %SUB, !dbg !57
  br i1 %LE, label %while.then, label %while.end, !dbg !57

while.then:                                       ; preds = %while.start
  br label %if.start13, !dbg !57

while.end:                                        ; preds = %while.start
  ret i32 -1, !dbg !58

if.start13:                                       ; preds = %while.then
  %self16 = load %struct.String*, %struct.String** %self2, align 8, !dbg !58
  %value = getelementptr %struct.String, %struct.String* %self16, i32 0, i32 0, !dbg !58
  %DOT17 = load i8*, i8** %value, align 8, !dbg !58
  %i18 = load i32, i32* %i, align 4, !dbg !58
  %idx = sext i32 %i18 to i64, !dbg !58
  %ADD = getelementptr i8, i8* %DOT17, i64 %idx, !dbg !58
  %needle19 = load i8*, i8** %needle1, align 8, !dbg !58
  %nlen20 = load i32, i32* %nlen, align 4, !dbg !58
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %needle19, i32 %nlen20), !dbg !58
  %EQ21 = icmp eq i32 %strncmp, 0, !dbg !58
  br i1 %EQ21, label %if.then15, label %if.end14, !dbg !58

if.end14:                                         ; preds = %if.start13
  %i23 = load i32, i32* %i, align 4, !dbg !58
  %ADD24 = add i32 %i23, 1, !dbg !58
  store i32 %ADD24, i32* %i, align 4, !dbg !58
  br label %while.start, !dbg !58

if.then15:                                        ; preds = %if.start13
  %i22 = load i32, i32* %i, align 4, !dbg !58
  ret i32 %i22, !dbg !58
}

define i1 @String.contains(i8* %needle, %struct.String* %self) !dbg !59 {
entry:
  %needle1 = alloca i8*, align 8, !dbg !60
  %self2 = alloca %struct.String*, align 8, !dbg !60
  store i8* %needle, i8** %needle1, align 8, !dbg !60
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !60
  %needle3 = load i8*, i8** %needle1, align 8, !dbg !61
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !61
  %String.find = call i32 @String.find(i8* %needle3, %struct.String* %ref_arg), !dbg !61
  %NEQ = icmp ne i32 %String.find, -1, !dbg !61
  ret i1 %NEQ, !dbg !61
}

define i1 @String.starts_with(i8* %prefix, %struct.String* %self) !dbg !62 {
entry:
  %prefix1 = alloca i8*, align 8, !dbg !63
  %self2 = alloca %struct.String*, align 8, !dbg !63
  %plen = alloca i32, align 4, !dbg !63
  store i8* %prefix, i8** %prefix1, align 8, !dbg !63
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !63
  br label %if.start, !dbg !63

if.start:                                         ; preds = %entry
  %prefix3 = load i8*, i8** %prefix1, align 8, !dbg !63
  %EQ = icmp eq i8* %prefix3, null, !dbg !63
  br i1 %EQ, label %if.then, label %if.end, !dbg !63

if.end:                                           ; preds = %if.start
  store i32 0, i32* %plen, align 4, !dbg !63
  %prefix4 = load i8*, i8** %prefix1, align 8, !dbg !64
  %strlen = call i32 @strlen(i8* %prefix4), !dbg !64
  store i32 %strlen, i32* %plen, align 4, !dbg !64
  br label %if.start5, !dbg !64

if.then:                                          ; preds = %if.start
  ret i1 true, !dbg !63

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !64
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !64
  %plen9 = load i32, i32* %plen, align 4, !dbg !64
  %DOT = load i32, i32* %count, align 4, !dbg !64
  %GT = icmp sgt i32 %plen9, %DOT, !dbg !64
  br i1 %GT, label %if.then7, label %if.end6, !dbg !64

if.end6:                                          ; preds = %if.start5
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !65
  %value = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 0, !dbg !65
  %DOT11 = load i8*, i8** %value, align 8, !dbg !65
  %prefix12 = load i8*, i8** %prefix1, align 8, !dbg !65
  %plen13 = load i32, i32* %plen, align 4, !dbg !65
  %strncmp = call i32 @strncmp(i8* %DOT11, i8* %prefix12, i32 %plen13), !dbg !65
  %EQ14 = icmp eq i32 %strncmp, 0, !dbg !65
  ret i1 %EQ14, !dbg !65

if.then7:                                         ; preds = %if.start5
  ret i1 false, !dbg !64
}

define i1 @String.ends_with(i8* %suffix, %struct.String* %self) !dbg !66 {
entry:
  %suffix1 = alloca i8*, align 8, !dbg !67
  %self2 = alloca %struct.String*, align 8, !dbg !67
  %slen = alloca i32, align 4, !dbg !67
  %offset = alloca i32, align 4, !dbg !67
  store i8* %suffix, i8** %suffix1, align 8, !dbg !67
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !67
  br label %if.start, !dbg !67

if.start:                                         ; preds = %entry
  %suffix3 = load i8*, i8** %suffix1, align 8, !dbg !67
  %EQ = icmp eq i8* %suffix3, null, !dbg !67
  br i1 %EQ, label %if.then, label %if.end, !dbg !67

if.end:                                           ; preds = %if.start
  store i32 0, i32* %slen, align 4, !dbg !67
  %suffix4 = load i8*, i8** %suffix1, align 8, !dbg !68
  %strlen = call i32 @strlen(i8* %suffix4), !dbg !68
  store i32 %strlen, i32* %slen, align 4, !dbg !68
  br label %if.start5, !dbg !68

if.then:                                          ; preds = %if.start
  ret i1 true, !dbg !67

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !68
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !68
  %slen9 = load i32, i32* %slen, align 4, !dbg !68
  %DOT = load i32, i32* %count, align 4, !dbg !68
  %GT = icmp sgt i32 %slen9, %DOT, !dbg !68
  br i1 %GT, label %if.then7, label %if.end6, !dbg !68

if.end6:                                          ; preds = %if.start5
  store i32 0, i32* %offset, align 4, !dbg !67
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !67
  %count11 = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 1, !dbg !67
  %DOT12 = load i32, i32* %count11, align 4, !dbg !67
  %slen13 = load i32, i32* %slen, align 4, !dbg !67
  %SUB = sub i32 %DOT12, %slen13, !dbg !67
  store i32 %SUB, i32* %offset, align 4, !dbg !67
  %self14 = load %struct.String*, %struct.String** %self2, align 8, !dbg !69
  %value = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 0, !dbg !69
  %DOT15 = load i8*, i8** %value, align 8, !dbg !69
  %offset16 = load i32, i32* %offset, align 4, !dbg !69
  %idx = sext i32 %offset16 to i64, !dbg !69
  %ADD = getelementptr i8, i8* %DOT15, i64 %idx, !dbg !69
  %suffix17 = load i8*, i8** %suffix1, align 8, !dbg !69
  %slen18 = load i32, i32* %slen, align 4, !dbg !69
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %suffix17, i32 %slen18), !dbg !69
  %EQ19 = icmp eq i32 %strncmp, 0, !dbg !69
  ret i1 %EQ19, !dbg !69

if.then7:                                         ; preds = %if.start5
  ret i1 false, !dbg !68
}

define %struct.String @String.substr(i32 %start, i32 %length, %struct.String* %self) !dbg !70 {
entry:
  %start1 = alloca i32, align 4, !dbg !71
  %length2 = alloca i32, align 4, !dbg !71
  %self3 = alloca %struct.String*, align 8, !dbg !71
  %res = alloca %struct.String, align 8, !dbg !71
  %end = alloca i32, align 4, !dbg !71
  %i = alloca i32, align 4, !dbg !71
  store i32 %start, i32* %start1, align 4, !dbg !71
  store i32 %length, i32* %length2, align 4, !dbg !71
  store %struct.String* %self, %struct.String** %self3, align 8, !dbg !71
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !71
  %0 = call %struct.String @String.new(), !dbg !72
  %tmp_struct = alloca %struct.String, align 8, !dbg !72
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !72
  %tmp_op = alloca %struct.String, align 8, !dbg !72
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !72
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !72
  br label %if.start, !dbg !72

if.start:                                         ; preds = %entry
  %start4 = load i32, i32* %start1, align 4, !dbg !72
  %LT = icmp slt i32 %start4, 0, !dbg !72
  %self5 = load %struct.String*, %struct.String** %self3, align 8, !dbg !72
  %count = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 1, !dbg !72
  %start6 = load i32, i32* %start1, align 4, !dbg !72
  %DOT = load i32, i32* %count, align 4, !dbg !72
  %GE = icmp sge i32 %start6, %DOT, !dbg !72
  %OR = or i1 %LT, %GE, !dbg !72
  br i1 %OR, label %if.then, label %if.end, !dbg !72

if.end:                                           ; preds = %if.start
  store i32 0, i32* %end, align 4, !dbg !71
  %start8 = load i32, i32* %start1, align 4, !dbg !71
  %length9 = load i32, i32* %length2, align 4, !dbg !71
  %ADD = add i32 %start8, %length9, !dbg !71
  store i32 %ADD, i32* %end, align 4, !dbg !71
  br label %if.start10, !dbg !71

if.then:                                          ; preds = %if.start
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !72
  ret %struct.String %res7, !dbg !72

if.start10:                                       ; preds = %if.end
  %self13 = load %struct.String*, %struct.String** %self3, align 8, !dbg !71
  %count14 = getelementptr %struct.String, %struct.String* %self13, i32 0, i32 1, !dbg !71
  %end15 = load i32, i32* %end, align 4, !dbg !71
  %DOT16 = load i32, i32* %count14, align 4, !dbg !71
  %GT = icmp sgt i32 %end15, %DOT16, !dbg !71
  br i1 %GT, label %if.then12, label %if.end11, !dbg !71

if.end11:                                         ; preds = %if.then12, %if.start10
  store i32 0, i32* %i, align 4, !dbg !71
  %start20 = load i32, i32* %start1, align 4, !dbg !71
  store i32 %start20, i32* %i, align 4, !dbg !71
  br label %while.start, !dbg !71

if.then12:                                        ; preds = %if.start10
  %self17 = load %struct.String*, %struct.String** %self3, align 8, !dbg !71
  %count18 = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 1, !dbg !71
  %DOT19 = load i32, i32* %count18, align 4, !dbg !71
  store i32 %DOT19, i32* %end, align 4, !dbg !71
  br label %if.end11, !dbg !71

while.start:                                      ; preds = %while.then, %if.end11
  %i21 = load i32, i32* %i, align 4, !dbg !71
  %end22 = load i32, i32* %end, align 4, !dbg !71
  %LT23 = icmp slt i32 %i21, %end22, !dbg !71
  br i1 %LT23, label %while.then, label %while.end, !dbg !71

while.then:                                       ; preds = %while.start
  %self24 = load %struct.String*, %struct.String** %self3, align 8, !dbg !73
  %value = getelementptr %struct.String, %struct.String* %self24, i32 0, i32 0, !dbg !73
  %DOT25 = load i8*, i8** %value, align 8, !dbg !73
  %i26 = load i32, i32* %i, align 4, !dbg !73
  %ACCESS = getelementptr i8, i8* %DOT25, i32 %i26, !dbg !73
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !73
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !73
  %i27 = load i32, i32* %i, align 4, !dbg !73
  %ADD28 = add i32 %i27, 1, !dbg !73
  store i32 %ADD28, i32* %i, align 4, !dbg !73
  br label %while.start, !dbg !73

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !73
  %res29 = load %struct.String, %struct.String* %res, align 8, !dbg !73
  ret %struct.String %res29, !dbg !73
}

define %struct.String @String.upper(%struct.String* %self) !dbg !74 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !75
  %res = alloca %struct.String, align 8, !dbg !75
  %i = alloca i32, align 4, !dbg !75
  %c = alloca i8, align 1, !dbg !75
  %v = alloca i32, align 4, !dbg !75
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !75
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !75
  %0 = call %struct.String @String.new(), !dbg !76
  %tmp_struct = alloca %struct.String, align 8, !dbg !76
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !76
  %tmp_op = alloca %struct.String, align 8, !dbg !76
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !76
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !76
  store i32 0, i32* %i, align 4, !dbg !75
  store i32 0, i32* %i, align 4, !dbg !75
  br label %while.start, !dbg !75

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !75
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !75
  %i3 = load i32, i32* %i, align 4, !dbg !75
  %DOT = load i32, i32* %count, align 4, !dbg !75
  %LT = icmp slt i32 %i3, %DOT, !dbg !75
  br i1 %LT, label %while.then, label %while.end, !dbg !75

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !75
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !77
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !77
  %DOT5 = load i8*, i8** %value, align 8, !dbg !77
  %i6 = load i32, i32* %i, align 4, !dbg !77
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !77
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !77
  store i8 %ACC, i8* %c, align 1, !dbg !77
  br label %if.start, !dbg !77

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !78
  %res14 = load %struct.String, %struct.String* %res, align 8, !dbg !78
  ret %struct.String %res14, !dbg !78

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !77
  %GE = icmp sge i8 %c7, 97, !dbg !77
  %c8 = load i8, i8* %c, align 1, !dbg !77
  %LE = icmp sle i8 %c8, 122, !dbg !77
  %AND = and i1 %GE, %LE, !dbg !77
  br i1 %AND, label %if.then, label %if.else, !dbg !77

if.end:                                           ; preds = %if.else, %if.then
  %i13 = load i32, i32* %i, align 4, !dbg !78
  %ADD = add i32 %i13, 1, !dbg !78
  store i32 %ADD, i32* %i, align 4, !dbg !78
  br label %while.start, !dbg !78

if.then:                                          ; preds = %if.start
  store i32 0, i32* %v, align 4, !dbg !75
  %c9 = load i8, i8* %c, align 1, !dbg !75
  %as = sext i8 %c9 to i32, !dbg !75
  %SUB = sub i32 %as, 32, !dbg !75
  store i32 %SUB, i32* %v, align 4, !dbg !75
  %v10 = load i32, i32* %v, align 4, !dbg !79
  %as11 = trunc i32 %v10 to i8, !dbg !79
  call void @String.push(i8 %as11, %struct.String* %res), !dbg !79
  br label %if.end, !dbg !79

if.else:                                          ; preds = %if.start
  %c12 = load i8, i8* %c, align 1, !dbg !78
  call void @String.push(i8 %c12, %struct.String* %res), !dbg !78
  br label %if.end, !dbg !78
}

define %struct.String @String.lower(%struct.String* %self) !dbg !80 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !81
  %res = alloca %struct.String, align 8, !dbg !81
  %i = alloca i32, align 4, !dbg !81
  %c = alloca i8, align 1, !dbg !81
  %v = alloca i32, align 4, !dbg !81
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !81
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !81
  %0 = call %struct.String @String.new(), !dbg !82
  %tmp_struct = alloca %struct.String, align 8, !dbg !82
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !82
  %tmp_op = alloca %struct.String, align 8, !dbg !82
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !82
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !82
  store i32 0, i32* %i, align 4, !dbg !81
  store i32 0, i32* %i, align 4, !dbg !81
  br label %while.start, !dbg !81

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !81
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !81
  %i3 = load i32, i32* %i, align 4, !dbg !81
  %DOT = load i32, i32* %count, align 4, !dbg !81
  %LT = icmp slt i32 %i3, %DOT, !dbg !81
  br i1 %LT, label %while.then, label %while.end, !dbg !81

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !81
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !83
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !83
  %DOT5 = load i8*, i8** %value, align 8, !dbg !83
  %i6 = load i32, i32* %i, align 4, !dbg !83
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !83
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !83
  store i8 %ACC, i8* %c, align 1, !dbg !83
  br label %if.start, !dbg !83

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !84
  %res15 = load %struct.String, %struct.String* %res, align 8, !dbg !84
  ret %struct.String %res15, !dbg !84

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !83
  %GE = icmp sge i8 %c7, 65, !dbg !83
  %c8 = load i8, i8* %c, align 1, !dbg !83
  %LE = icmp sle i8 %c8, 90, !dbg !83
  %AND = and i1 %GE, %LE, !dbg !83
  br i1 %AND, label %if.then, label %if.else, !dbg !83

if.end:                                           ; preds = %if.else, %if.then
  %i13 = load i32, i32* %i, align 4, !dbg !84
  %ADD14 = add i32 %i13, 1, !dbg !84
  store i32 %ADD14, i32* %i, align 4, !dbg !84
  br label %while.start, !dbg !84

if.then:                                          ; preds = %if.start
  store i32 0, i32* %v, align 4, !dbg !81
  %c9 = load i8, i8* %c, align 1, !dbg !81
  %as = sext i8 %c9 to i32, !dbg !81
  %ADD = add i32 %as, 32, !dbg !81
  store i32 %ADD, i32* %v, align 4, !dbg !81
  %v10 = load i32, i32* %v, align 4, !dbg !85
  %as11 = trunc i32 %v10 to i8, !dbg !85
  call void @String.push(i8 %as11, %struct.String* %res), !dbg !85
  br label %if.end, !dbg !85

if.else:                                          ; preds = %if.start
  %c12 = load i8, i8* %c, align 1, !dbg !84
  call void @String.push(i8 %c12, %struct.String* %res), !dbg !84
  br label %if.end, !dbg !84
}

define %struct.String @String.trim(%struct.String* %self) !dbg !86 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !87
  %start = alloca i32, align 4, !dbg !87
  %c = alloca i8, align 1, !dbg !87
  %end = alloca i32, align 4, !dbg !87
  %c25 = alloca i8, align 1, !dbg !87
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !87
  store i32 0, i32* %start, align 4, !dbg !87
  store i32 0, i32* %start, align 4, !dbg !87
  br label %while.start, !dbg !87

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !87
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !87
  %start3 = load i32, i32* %start, align 4, !dbg !87
  %DOT = load i32, i32* %count, align 4, !dbg !87
  %LT = icmp slt i32 %start3, %DOT, !dbg !87
  br i1 %LT, label %while.then, label %while.end, !dbg !87

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !87
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !88
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !88
  %DOT5 = load i8*, i8** %value, align 8, !dbg !88
  %start6 = load i32, i32* %start, align 4, !dbg !88
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %start6, !dbg !88
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !88
  store i8 %ACC, i8* %c, align 1, !dbg !88
  br label %if.start, !dbg !88

while.end:                                        ; preds = %if.else, %while.start
  store i32 0, i32* %end, align 4, !dbg !87
  %self17 = load %struct.String*, %struct.String** %self1, align 8, !dbg !87
  %count18 = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 1, !dbg !87
  %DOT19 = load i32, i32* %count18, align 4, !dbg !87
  store i32 %DOT19, i32* %end, align 4, !dbg !87
  br label %while.start20, !dbg !87

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !88
  %EQ = icmp eq i8 %c7, 32, !dbg !88
  %c8 = load i8, i8* %c, align 1, !dbg !88
  %EQ9 = icmp eq i8 %c8, 9, !dbg !88
  %OR = or i1 %EQ, %EQ9, !dbg !88
  %c10 = load i8, i8* %c, align 1, !dbg !88
  %EQ11 = icmp eq i8 %c10, 10, !dbg !88
  %OR12 = or i1 %OR, %EQ11, !dbg !88
  %c13 = load i8, i8* %c, align 1, !dbg !88
  %EQ14 = icmp eq i8 %c13, 13, !dbg !88
  %OR15 = or i1 %OR12, %EQ14, !dbg !88
  br i1 %OR15, label %if.then, label %if.else, !dbg !88

if.end:                                           ; preds = %if.then
  br label %while.start, !dbg !88

if.then:                                          ; preds = %if.start
  %start16 = load i32, i32* %start, align 4, !dbg !88
  %ADD = add i32 %start16, 1, !dbg !88
  store i32 %ADD, i32* %start, align 4, !dbg !88
  br label %if.end, !dbg !88

if.else:                                          ; preds = %if.start
  br label %while.end, !dbg !88

while.start20:                                    ; preds = %if.end33, %while.end
  %end23 = load i32, i32* %end, align 4, !dbg !87
  %start24 = load i32, i32* %start, align 4, !dbg !87
  %GT = icmp sgt i32 %end23, %start24, !dbg !87
  br i1 %GT, label %while.then21, label %while.end22, !dbg !87

while.then21:                                     ; preds = %while.start20
  store i8 0, i8* %c25, align 1, !dbg !87
  %self26 = load %struct.String*, %struct.String** %self1, align 8, !dbg !89
  %value27 = getelementptr %struct.String, %struct.String* %self26, i32 0, i32 0, !dbg !89
  %end28 = load i32, i32* %end, align 4, !dbg !89
  %SUB = sub i32 %end28, 1, !dbg !89
  %DOT29 = load i8*, i8** %value27, align 8, !dbg !89
  %ACCESS30 = getelementptr i8, i8* %DOT29, i32 %SUB, !dbg !89
  %ACC31 = load i8, i8* %ACCESS30, align 1, !dbg !89
  store i8 %ACC31, i8* %c25, align 1, !dbg !89
  br label %if.start32, !dbg !89

while.end22:                                      ; preds = %if.else35, %while.start20
  %start49 = load i32, i32* %start, align 4, !dbg !90
  %end50 = load i32, i32* %end, align 4, !dbg !90
  %start51 = load i32, i32* %start, align 4, !dbg !90
  %SUB52 = sub i32 %end50, %start51, !dbg !90
  %ref_arg = load %struct.String*, %struct.String** %self1, align 8, !dbg !90
  %String.substr = call %struct.String @String.substr(i32 %start49, i32 %SUB52, %struct.String* %ref_arg), !dbg !90
  %tmp_struct = alloca %struct.String, align 8, !dbg !90
  store %struct.String %String.substr, %struct.String* %tmp_struct, align 8, !dbg !90
  ret %struct.String %String.substr, !dbg !90

if.start32:                                       ; preds = %while.then21
  %c36 = load i8, i8* %c25, align 1, !dbg !89
  %EQ37 = icmp eq i8 %c36, 32, !dbg !89
  %c38 = load i8, i8* %c25, align 1, !dbg !89
  %EQ39 = icmp eq i8 %c38, 9, !dbg !89
  %OR40 = or i1 %EQ37, %EQ39, !dbg !89
  %c41 = load i8, i8* %c25, align 1, !dbg !89
  %EQ42 = icmp eq i8 %c41, 10, !dbg !89
  %OR43 = or i1 %OR40, %EQ42, !dbg !89
  %c44 = load i8, i8* %c25, align 1, !dbg !89
  %EQ45 = icmp eq i8 %c44, 13, !dbg !89
  %OR46 = or i1 %OR43, %EQ45, !dbg !89
  br i1 %OR46, label %if.then34, label %if.else35, !dbg !89

if.end33:                                         ; preds = %if.then34
  br label %while.start20, !dbg !89

if.then34:                                        ; preds = %if.start32
  %end47 = load i32, i32* %end, align 4, !dbg !89
  %SUB48 = sub i32 %end47, 1, !dbg !89
  store i32 %SUB48, i32* %end, align 4, !dbg !89
  br label %if.end33, !dbg !89

if.else35:                                        ; preds = %if.start32
  br label %while.end22, !dbg !89
}

define %struct.String @String.replace(i8* %old, i8* %replacement, %struct.String* %self) !dbg !91 {
entry:
  %old1 = alloca i8*, align 8, !dbg !92
  %replacement2 = alloca i8*, align 8, !dbg !92
  %self3 = alloca %struct.String*, align 8, !dbg !92
  %res = alloca %struct.String, align 8, !dbg !92
  %olen = alloca i32, align 4, !dbg !92
  %i = alloca i32, align 4, !dbg !92
  store i8* %old, i8** %old1, align 8, !dbg !92
  store i8* %replacement, i8** %replacement2, align 8, !dbg !92
  store %struct.String* %self, %struct.String** %self3, align 8, !dbg !92
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !92
  %0 = call %struct.String @String.new(), !dbg !93
  %tmp_struct = alloca %struct.String, align 8, !dbg !93
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !93
  %tmp_op = alloca %struct.String, align 8, !dbg !93
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !93
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !93
  store i32 0, i32* %olen, align 4, !dbg !92
  %old4 = load i8*, i8** %old1, align 8, !dbg !94
  %strlen = call i32 @strlen(i8* %old4), !dbg !94
  store i32 %strlen, i32* %olen, align 4, !dbg !94
  br label %if.start, !dbg !94

if.start:                                         ; preds = %entry
  %olen5 = load i32, i32* %olen, align 4, !dbg !94
  %EQ = icmp eq i32 %olen5, 0, !dbg !94
  br i1 %EQ, label %if.then, label %if.end, !dbg !94

if.end:                                           ; preds = %if.start
  store i32 0, i32* %i, align 4, !dbg !92
  store i32 0, i32* %i, align 4, !dbg !92
  br label %while.start, !dbg !92

if.then:                                          ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self3, align 8, !dbg !95
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !95
  %DOT = load i8*, i8** %value, align 8, !dbg !95
  call void @String.assign(i8* %DOT, %struct.String* %res), !dbg !95
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !95
  ret %struct.String %res7, !dbg !95

while.start:                                      ; preds = %if.end12, %if.end
  %self8 = load %struct.String*, %struct.String** %self3, align 8, !dbg !92
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !92
  %i9 = load i32, i32* %i, align 4, !dbg !92
  %DOT10 = load i32, i32* %count, align 4, !dbg !92
  %LT = icmp slt i32 %i9, %DOT10, !dbg !92
  br i1 %LT, label %while.then, label %while.end, !dbg !92

while.then:                                       ; preds = %while.start
  br label %if.start11, !dbg !92

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !96
  %res36 = load %struct.String, %struct.String* %res, align 8, !dbg !96
  ret %struct.String %res36, !dbg !96

if.start11:                                       ; preds = %while.then
  %self14 = load %struct.String*, %struct.String** %self3, align 8, !dbg !92
  %count15 = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 1, !dbg !92
  %DOT16 = load i32, i32* %count15, align 4, !dbg !92
  %olen17 = load i32, i32* %olen, align 4, !dbg !92
  %SUB = sub i32 %DOT16, %olen17, !dbg !92
  %i18 = load i32, i32* %i, align 4, !dbg !92
  %LE = icmp sle i32 %i18, %SUB, !dbg !92
  %self19 = load %struct.String*, %struct.String** %self3, align 8, !dbg !97
  %value20 = getelementptr %struct.String, %struct.String* %self19, i32 0, i32 0, !dbg !97
  %DOT21 = load i8*, i8** %value20, align 8, !dbg !97
  %i22 = load i32, i32* %i, align 4, !dbg !97
  %idx = sext i32 %i22 to i64, !dbg !97
  %ADD = getelementptr i8, i8* %DOT21, i64 %idx, !dbg !97
  %old23 = load i8*, i8** %old1, align 8, !dbg !97
  %olen24 = load i32, i32* %olen, align 4, !dbg !97
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %old23, i32 %olen24), !dbg !97
  %EQ25 = icmp eq i32 %strncmp, 0, !dbg !97
  %AND = and i1 %LE, %EQ25, !dbg !97
  br i1 %AND, label %if.then13, label %if.else, !dbg !97

if.end12:                                         ; preds = %if.else, %if.then13
  br label %while.start, !dbg !96

if.then13:                                        ; preds = %if.start11
  %replacement26 = load i8*, i8** %replacement2, align 8, !dbg !98
  call void @String.join(i8* %replacement26, %struct.String* %res), !dbg !98
  %i27 = load i32, i32* %i, align 4, !dbg !98
  %olen28 = load i32, i32* %olen, align 4, !dbg !98
  %ADD29 = add i32 %i27, %olen28, !dbg !98
  store i32 %ADD29, i32* %i, align 4, !dbg !98
  br label %if.end12, !dbg !98

if.else:                                          ; preds = %if.start11
  %self30 = load %struct.String*, %struct.String** %self3, align 8, !dbg !96
  %value31 = getelementptr %struct.String, %struct.String* %self30, i32 0, i32 0, !dbg !96
  %DOT32 = load i8*, i8** %value31, align 8, !dbg !96
  %i33 = load i32, i32* %i, align 4, !dbg !96
  %ACCESS = getelementptr i8, i8* %DOT32, i32 %i33, !dbg !96
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !96
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !96
  %i34 = load i32, i32* %i, align 4, !dbg !96
  %ADD35 = add i32 %i34, 1, !dbg !96
  store i32 %ADD35, i32* %i, align 4, !dbg !96
  br label %if.end12, !dbg !96
}

define %struct.String @String.repeat(i32 %n, %struct.String* %self) !dbg !99 {
entry:
  %n1 = alloca i32, align 4, !dbg !100
  %self2 = alloca %struct.String*, align 8, !dbg !100
  %res = alloca %struct.String, align 8, !dbg !100
  %i = alloca i32, align 4, !dbg !100
  store i32 %n, i32* %n1, align 4, !dbg !100
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !100
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !100
  %0 = call %struct.String @String.new(), !dbg !101
  %tmp_struct = alloca %struct.String, align 8, !dbg !101
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !101
  %tmp_op = alloca %struct.String, align 8, !dbg !101
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !101
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !101
  store i32 0, i32* %i, align 4, !dbg !100
  store i32 0, i32* %i, align 4, !dbg !100
  br label %while.start, !dbg !100

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !100
  %n4 = load i32, i32* %n1, align 4, !dbg !100
  %LT = icmp slt i32 %i3, %n4, !dbg !100
  br i1 %LT, label %while.then, label %while.end, !dbg !100

while.then:                                       ; preds = %while.start
  %self5 = load %struct.String*, %struct.String** %self2, align 8, !dbg !102
  %value = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 0, !dbg !102
  %DOT = load i8*, i8** %value, align 8, !dbg !102
  call void @String.join(i8* %DOT, %struct.String* %res), !dbg !102
  %i6 = load i32, i32* %i, align 4, !dbg !102
  %ADD = add i32 %i6, 1, !dbg !102
  store i32 %ADD, i32* %i, align 4, !dbg !102
  br label %while.start, !dbg !102

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !102
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !102
  ret %struct.String %res7, !dbg !102
}

define %struct.String @String.reverse(%struct.String* %self) !dbg !103 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !104
  %res = alloca %struct.String, align 8, !dbg !104
  %i = alloca i32, align 4, !dbg !104
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !104
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !104
  %0 = call %struct.String @String.new(), !dbg !105
  %tmp_struct = alloca %struct.String, align 8, !dbg !105
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !105
  %tmp_op = alloca %struct.String, align 8, !dbg !105
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !105
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !105
  store i32 0, i32* %i, align 4, !dbg !104
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !104
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !104
  %DOT = load i32, i32* %count, align 4, !dbg !104
  %SUB = sub i32 %DOT, 1, !dbg !104
  store i32 %SUB, i32* %i, align 4, !dbg !104
  br label %while.start, !dbg !104

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !104
  %GE = icmp sge i32 %i3, 0, !dbg !104
  br i1 %GE, label %while.then, label %while.end, !dbg !104

while.then:                                       ; preds = %while.start
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !106
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !106
  %DOT5 = load i8*, i8** %value, align 8, !dbg !106
  %i6 = load i32, i32* %i, align 4, !dbg !106
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !106
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !106
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !106
  %i7 = load i32, i32* %i, align 4, !dbg !106
  %SUB8 = sub i32 %i7, 1, !dbg !106
  store i32 %SUB8, i32* %i, align 4, !dbg !106
  br label %while.start, !dbg !106

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !106
  %res9 = load %struct.String, %struct.String* %res, align 8, !dbg !106
  ret %struct.String %res9, !dbg !106
}

define i32 @String.compare(%struct.String* %other, %struct.String* %self) !dbg !107 {
entry:
  %other1 = alloca %struct.String*, align 8, !dbg !108
  %self2 = alloca %struct.String*, align 8, !dbg !108
  store %struct.String* %other, %struct.String** %other1, align 8, !dbg !108
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !108
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !109
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !109
  %DOT = load i8*, i8** %value, align 8, !dbg !109
  %other4 = load %struct.String*, %struct.String** %other1, align 8, !dbg !109
  %value5 = getelementptr %struct.String, %struct.String* %other4, i32 0, i32 0, !dbg !109
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !109
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !109
  ret i32 %strcmp, !dbg !109
}

define i32 @String.to_int(%struct.String* %self) !dbg !110 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !111
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !111
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !112
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !112
  %DOT = load i8*, i8** %value, align 8, !dbg !112
  %atoi = call i32 @atoi(i8* %DOT), !dbg !112
  ret i32 %atoi, !dbg !112
}

define void @String.operator.ASSIGN.CHARS(i8* %str, %struct.String* %self) !dbg !113 {
entry:
  %str1 = alloca i8*, align 8, !dbg !114
  %self2 = alloca %struct.String*, align 8, !dbg !114
  store i8* %str, i8** %str1, align 8, !dbg !114
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !114
  %str3 = load i8*, i8** %str1, align 8, !dbg !115
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !115
  call void @String.assign(i8* %str3, %struct.String* %ref_arg), !dbg !115
  ret void, !dbg !115
}

define void @String.operator.ASSIGN.String(%struct.String* %v, %struct.String* %self) !dbg !116 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !117
  %self2 = alloca %struct.String*, align 8, !dbg !117
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !117
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !117
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !118
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !118
  %DOT = load i8*, i8** %value, align 8, !dbg !118
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !118
  call void @String.assign(i8* %DOT, %struct.String* %ref_arg), !dbg !118
  ret void, !dbg !118
}

define %struct.String @String.operator.ADD.String(%struct.String* %v, %struct.String* %self) !dbg !119 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !120
  %self2 = alloca %struct.String*, align 8, !dbg !120
  %res = alloca %struct.String, align 8, !dbg !120
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !120
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !120
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !120
  %0 = call %struct.String @String.new(), !dbg !121
  %tmp_struct = alloca %struct.String, align 8, !dbg !121
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !121
  %tmp_op = alloca %struct.String, align 8, !dbg !121
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !121
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !121
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !122
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !122
  %DOT = load i8*, i8** %value, align 8, !dbg !122
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !122
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !122
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !122
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %DOT6), !dbg !122
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !122
  call void @String.delete(%struct.String* %tmp_struct), !dbg !122
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !122
  ret %struct.String %res7, !dbg !122
}

define %struct.String @String.operator.ADD.CHARS(i8* %str, %struct.String* %self) !dbg !123 {
entry:
  %str1 = alloca i8*, align 8, !dbg !124
  %self2 = alloca %struct.String*, align 8, !dbg !124
  %res = alloca %struct.String, align 8, !dbg !124
  store i8* %str, i8** %str1, align 8, !dbg !124
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !124
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !124
  %0 = call %struct.String @String.new(), !dbg !125
  %tmp_struct = alloca %struct.String, align 8, !dbg !125
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !125
  %tmp_op = alloca %struct.String, align 8, !dbg !125
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !125
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !125
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !126
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !126
  %DOT = load i8*, i8** %value, align 8, !dbg !126
  %str4 = load i8*, i8** %str1, align 8, !dbg !126
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str4), !dbg !126
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !126
  call void @String.delete(%struct.String* %tmp_struct), !dbg !126
  %res5 = load %struct.String, %struct.String* %res, align 8, !dbg !126
  ret %struct.String %res5, !dbg !126
}

define void @String.operator.ADD_ASS.String(%struct.String* %v, %struct.String* %self) !dbg !127 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !128
  %self2 = alloca %struct.String*, align 8, !dbg !128
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !128
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !128
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !129
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !129
  %DOT = load i8*, i8** %value, align 8, !dbg !129
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !129
  call void @String.join(i8* %DOT, %struct.String* %ref_arg), !dbg !129
  ret void, !dbg !129
}

define void @String.operator.ADD_ASS.CHARS(i8* %str, %struct.String* %self) !dbg !130 {
entry:
  %str1 = alloca i8*, align 8, !dbg !131
  %self2 = alloca %struct.String*, align 8, !dbg !131
  store i8* %str, i8** %str1, align 8, !dbg !131
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !131
  %str3 = load i8*, i8** %str1, align 8, !dbg !132
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !132
  call void @String.join(i8* %str3, %struct.String* %ref_arg), !dbg !132
  ret void, !dbg !132
}

define i1 @String.operator.EQ.String(%struct.String* %v, %struct.String* %self) !dbg !133 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !134
  %self2 = alloca %struct.String*, align 8, !dbg !134
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !134
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !134
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !135
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !135
  %DOT = load i8*, i8** %value, align 8, !dbg !135
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !135
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !135
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !135
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !135
  %EQ = icmp eq i32 %strcmp, 0, !dbg !135
  ret i1 %EQ, !dbg !135
}

define i1 @String.operator.EQ.CHARS(i8* %str, %struct.String* %self) !dbg !136 {
entry:
  %str1 = alloca i8*, align 8, !dbg !137
  %self2 = alloca %struct.String*, align 8, !dbg !137
  store i8* %str, i8** %str1, align 8, !dbg !137
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !137
  br label %if.start, !dbg !137

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !137
  %EQ = icmp eq i8* %str3, null, !dbg !137
  br i1 %EQ, label %if.then, label %if.end, !dbg !137

if.end:                                           ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self2, align 8, !dbg !138
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !138
  %DOT7 = load i8*, i8** %value, align 8, !dbg !138
  %str8 = load i8*, i8** %str1, align 8, !dbg !138
  %strcmp = call i32 @strcmp(i8* %DOT7, i8* %str8), !dbg !138
  %EQ9 = icmp eq i32 %strcmp, 0, !dbg !138
  ret i1 %EQ9, !dbg !138

if.then:                                          ; preds = %if.start
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !137
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !137
  %DOT = load i32, i32* %count, align 4, !dbg !137
  %EQ5 = icmp eq i32 %DOT, 0, !dbg !137
  ret i1 %EQ5, !dbg !137
}

define i1 @String.operator.NEQ.String(%struct.String* %v, %struct.String* %self) !dbg !139 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !140
  %self2 = alloca %struct.String*, align 8, !dbg !140
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !140
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !140
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !141
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !141
  %DOT = load i8*, i8** %value, align 8, !dbg !141
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !141
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !141
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !141
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !141
  %NEQ = icmp ne i32 %strcmp, 0, !dbg !141
  ret i1 %NEQ, !dbg !141
}

define i1 @String.operator.NEQ.CHARS(i8* %str, %struct.String* %self) !dbg !142 {
entry:
  %str1 = alloca i8*, align 8, !dbg !143
  %self2 = alloca %struct.String*, align 8, !dbg !143
  store i8* %str, i8** %str1, align 8, !dbg !143
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !143
  br label %if.start, !dbg !143

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !143
  %EQ = icmp eq i8* %str3, null, !dbg !143
  br i1 %EQ, label %if.then, label %if.end, !dbg !143

if.end:                                           ; preds = %if.start
  %self5 = load %struct.String*, %struct.String** %self2, align 8, !dbg !144
  %value = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 0, !dbg !144
  %DOT6 = load i8*, i8** %value, align 8, !dbg !144
  %str7 = load i8*, i8** %str1, align 8, !dbg !144
  %strcmp = call i32 @strcmp(i8* %DOT6, i8* %str7), !dbg !144
  %NEQ8 = icmp ne i32 %strcmp, 0, !dbg !144
  ret i1 %NEQ8, !dbg !144

if.then:                                          ; preds = %if.start
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !143
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !143
  %DOT = load i32, i32* %count, align 4, !dbg !143
  %NEQ = icmp ne i32 %DOT, 0, !dbg !143
  ret i1 %NEQ, !dbg !143
}

define i1 @String.operator.LT.String(%struct.String* %v, %struct.String* %self) !dbg !145 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !146
  %self2 = alloca %struct.String*, align 8, !dbg !146
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !146
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !146
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !147
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !147
  %DOT = load i8*, i8** %value, align 8, !dbg !147
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !147
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !147
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !147
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !147
  %LT = icmp slt i32 %strcmp, 0, !dbg !147
  ret i1 %LT, !dbg !147
}

define i1 @String.operator.GT.String(%struct.String* %v, %struct.String* %self) !dbg !148 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !149
  %self2 = alloca %struct.String*, align 8, !dbg !149
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !149
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !149
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !150
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !150
  %DOT = load i8*, i8** %value, align 8, !dbg !150
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !150
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !150
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !150
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !150
  %GT = icmp sgt i32 %strcmp, 0, !dbg !150
  ret i1 %GT, !dbg !150
}

define i8* @String.operator.output(%struct.String* %self) !dbg !151 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !152
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !152
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !152
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !152
  %DOT = load i8*, i8** %value, align 8, !dbg !152
  ret i8* %DOT, !dbg !152
}

declare void @free(i8*)

declare i32 @strlen(i8*)

declare i8* @realloc(i8*, i32)

declare i8* @strcpy(i8*, i8*)

declare void @bzero(i8*, i64)

define i8* @strjoin(i8* %left, i8* %right) !dbg !153 {
entry:
  %left1 = alloca i8*, align 8, !dbg !154
  %right2 = alloca i8*, align 8, !dbg !154
  %len1 = alloca i32, align 4, !dbg !154
  %len2 = alloca i32, align 4, !dbg !154
  %str = alloca i8*, align 8, !dbg !154
  %i = alloca i32, align 4, !dbg !154
  %j = alloca i32, align 4, !dbg !154
  store i8* %left, i8** %left1, align 8, !dbg !154
  store i8* %right, i8** %right2, align 8, !dbg !154
  store i32 0, i32* %len1, align 4, !dbg !154
  store i32 0, i32* %len1, align 4, !dbg !154
  store i32 0, i32* %len2, align 4, !dbg !154
  store i32 0, i32* %len2, align 4, !dbg !154
  br label %if.start, !dbg !154

if.start:                                         ; preds = %entry
  %left3 = load i8*, i8** %left1, align 8, !dbg !154
  %NEQ = icmp ne i8* %left3, null, !dbg !154
  br i1 %NEQ, label %if.then, label %if.end, !dbg !154

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start5, !dbg !155

if.then:                                          ; preds = %if.start
  %left4 = load i8*, i8** %left1, align 8, !dbg !155
  %strlen = call i32 @strlen(i8* %left4), !dbg !155
  store i32 %strlen, i32* %len1, align 4, !dbg !155
  br label %if.end, !dbg !155

if.start5:                                        ; preds = %if.end
  %right8 = load i8*, i8** %right2, align 8, !dbg !155
  %NEQ9 = icmp ne i8* %right8, null, !dbg !155
  br i1 %NEQ9, label %if.then7, label %if.end6, !dbg !155

if.end6:                                          ; preds = %if.then7, %if.start5
  store i8* null, i8** %str, align 8, !dbg !154
  %len112 = load i32, i32* %len1, align 4, !dbg !156
  %len213 = load i32, i32* %len2, align 4, !dbg !156
  %ADD = add i32 %len112, %len213, !dbg !156
  %ADD14 = add i32 %ADD, 1, !dbg !156
  %as = sext i32 %ADD14 to i64, !dbg !156
  %calloc = call i8* @calloc(i64 %as, i64 1), !dbg !156
  store i8* %calloc, i8** %str, align 8, !dbg !156
  store i32 0, i32* %i, align 4, !dbg !154
  store i32 0, i32* %i, align 4, !dbg !154
  br label %while.start, !dbg !154

if.then7:                                         ; preds = %if.start5
  %right10 = load i8*, i8** %right2, align 8, !dbg !157
  %strlen11 = call i32 @strlen(i8* %right10), !dbg !157
  store i32 %strlen11, i32* %len2, align 4, !dbg !157
  br label %if.end6, !dbg !157

while.start:                                      ; preds = %while.then, %if.end6
  %i15 = load i32, i32* %i, align 4, !dbg !154
  %len116 = load i32, i32* %len1, align 4, !dbg !154
  %LT = icmp slt i32 %i15, %len116, !dbg !154
  br i1 %LT, label %while.then, label %while.end, !dbg !154

while.then:                                       ; preds = %while.start
  %str17 = load i8*, i8** %str, align 8, !dbg !158
  %i18 = load i32, i32* %i, align 4, !dbg !158
  %ACCESS = getelementptr i8, i8* %str17, i32 %i18, !dbg !158
  %left19 = load i8*, i8** %left1, align 8, !dbg !158
  %i20 = load i32, i32* %i, align 4, !dbg !158
  %ACCESS21 = getelementptr i8, i8* %left19, i32 %i20, !dbg !158
  %ACC = load i8, i8* %ACCESS21, align 1, !dbg !158
  store i8 %ACC, i8* %ACCESS, align 1, !dbg !158
  %i22 = load i32, i32* %i, align 4, !dbg !158
  %ADD23 = add i32 %i22, 1, !dbg !158
  store i32 %ADD23, i32* %i, align 4, !dbg !158
  br label %while.start, !dbg !158

while.end:                                        ; preds = %while.start
  store i32 0, i32* %j, align 4, !dbg !154
  store i32 0, i32* %j, align 4, !dbg !154
  br label %while.start24, !dbg !154

while.start24:                                    ; preds = %while.then25, %while.end
  %j27 = load i32, i32* %j, align 4, !dbg !154
  %len228 = load i32, i32* %len2, align 4, !dbg !154
  %LT29 = icmp slt i32 %j27, %len228, !dbg !154
  br i1 %LT29, label %while.then25, label %while.end26, !dbg !154

while.then25:                                     ; preds = %while.start24
  %i30 = load i32, i32* %i, align 4, !dbg !159
  %j31 = load i32, i32* %j, align 4, !dbg !159
  %ADD32 = add i32 %i30, %j31, !dbg !159
  %str33 = load i8*, i8** %str, align 8, !dbg !159
  %ACCESS34 = getelementptr i8, i8* %str33, i32 %ADD32, !dbg !159
  %right35 = load i8*, i8** %right2, align 8, !dbg !159
  %j36 = load i32, i32* %j, align 4, !dbg !159
  %ACCESS37 = getelementptr i8, i8* %right35, i32 %j36, !dbg !159
  %ACC38 = load i8, i8* %ACCESS37, align 1, !dbg !159
  store i8 %ACC38, i8* %ACCESS34, align 1, !dbg !159
  %j39 = load i32, i32* %j, align 4, !dbg !159
  %ADD40 = add i32 %j39, 1, !dbg !159
  store i32 %ADD40, i32* %j, align 4, !dbg !159
  br label %while.start24, !dbg !159

while.end26:                                      ; preds = %while.start24
  %str41 = load i8*, i8** %str, align 8, !dbg !159
  ret i8* %str41, !dbg !159
}

declare i8* @calloc(i64, i64)

declare i32 @strncmp(i8*, i8*, i32)

declare i32 @strcmp(i8*, i8*)

declare i32 @atoi(i8*)

define void @SockAddr.init(%struct.SockAddr* %self) !dbg !160 {
entry:
  %self1 = alloca %struct.SockAddr*, align 8, !dbg !161
  store %struct.SockAddr* %self, %struct.SockAddr** %self1, align 8, !dbg !161
  %self2 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !161
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self2, i32 0, i32 0, !dbg !161
  %heap = call i8* @calloc(i64 16, i64 1), !dbg !161
  store i8* %heap, i8** %data, align 8, !dbg !161
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !162
  %data4 = getelementptr %struct.SockAddr, %struct.SockAddr* %self3, i32 0, i32 0, !dbg !162
  %DOT = load i8*, i8** %data4, align 8, !dbg !162
  %ACCESS = getelementptr i8, i8* %DOT, i32 0, !dbg !162
  store i8 16, i8* %ACCESS, align 1, !dbg !162
  %self5 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !163
  %data6 = getelementptr %struct.SockAddr, %struct.SockAddr* %self5, i32 0, i32 0, !dbg !163
  %DOT7 = load i8*, i8** %data6, align 8, !dbg !163
  %ACCESS8 = getelementptr i8, i8* %DOT7, i32 1, !dbg !163
  store i8 2, i8* %ACCESS8, align 1, !dbg !163
  ret void, !dbg !163
}

define void @SockAddr.set_port(i32 %port, %struct.SockAddr* %self) !dbg !164 {
entry:
  %port1 = alloca i32, align 4, !dbg !165
  %self2 = alloca %struct.SockAddr*, align 8, !dbg !165
  store i32 %port, i32* %port1, align 4, !dbg !165
  store %struct.SockAddr* %self, %struct.SockAddr** %self2, align 8, !dbg !165
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self2, align 8, !dbg !166
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self3, i32 0, i32 0, !dbg !166
  %DOT = load i8*, i8** %data, align 8, !dbg !166
  %ACCESS = getelementptr i8, i8* %DOT, i32 2, !dbg !166
  %port4 = load i32, i32* %port1, align 4, !dbg !166
  %RSHIFT = ashr i32 %port4, 8, !dbg !166
  %as = trunc i32 %RSHIFT to i8, !dbg !166
  store i8 %as, i8* %ACCESS, align 1, !dbg !166
  %self5 = load %struct.SockAddr*, %struct.SockAddr** %self2, align 8, !dbg !167
  %data6 = getelementptr %struct.SockAddr, %struct.SockAddr* %self5, i32 0, i32 0, !dbg !167
  %DOT7 = load i8*, i8** %data6, align 8, !dbg !167
  %ACCESS8 = getelementptr i8, i8* %DOT7, i32 3, !dbg !167
  %port9 = load i32, i32* %port1, align 4, !dbg !167
  %BAND = and i32 %port9, 255, !dbg !167
  %as10 = trunc i32 %BAND to i8, !dbg !167
  store i8 %as10, i8* %ACCESS8, align 1, !dbg !167
  ret void, !dbg !167
}

define void @SockAddr.set_ip(i32 %a, i32 %b, i32 %c, i32 %d, %struct.SockAddr* %self) !dbg !168 {
entry:
  %a1 = alloca i32, align 4, !dbg !169
  %b2 = alloca i32, align 4, !dbg !169
  %c3 = alloca i32, align 4, !dbg !169
  %d4 = alloca i32, align 4, !dbg !169
  %self5 = alloca %struct.SockAddr*, align 8, !dbg !169
  store i32 %a, i32* %a1, align 4, !dbg !169
  store i32 %b, i32* %b2, align 4, !dbg !169
  store i32 %c, i32* %c3, align 4, !dbg !169
  store i32 %d, i32* %d4, align 4, !dbg !169
  store %struct.SockAddr* %self, %struct.SockAddr** %self5, align 8, !dbg !169
  %self6 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !170
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self6, i32 0, i32 0, !dbg !170
  %DOT = load i8*, i8** %data, align 8, !dbg !170
  %ACCESS = getelementptr i8, i8* %DOT, i32 4, !dbg !170
  %a7 = load i32, i32* %a1, align 4, !dbg !170
  %as = trunc i32 %a7 to i8, !dbg !170
  store i8 %as, i8* %ACCESS, align 1, !dbg !170
  %self8 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !171
  %data9 = getelementptr %struct.SockAddr, %struct.SockAddr* %self8, i32 0, i32 0, !dbg !171
  %DOT10 = load i8*, i8** %data9, align 8, !dbg !171
  %ACCESS11 = getelementptr i8, i8* %DOT10, i32 5, !dbg !171
  %b12 = load i32, i32* %b2, align 4, !dbg !171
  %as13 = trunc i32 %b12 to i8, !dbg !171
  store i8 %as13, i8* %ACCESS11, align 1, !dbg !171
  %self14 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !172
  %data15 = getelementptr %struct.SockAddr, %struct.SockAddr* %self14, i32 0, i32 0, !dbg !172
  %DOT16 = load i8*, i8** %data15, align 8, !dbg !172
  %ACCESS17 = getelementptr i8, i8* %DOT16, i32 6, !dbg !172
  %c18 = load i32, i32* %c3, align 4, !dbg !172
  %as19 = trunc i32 %c18 to i8, !dbg !172
  store i8 %as19, i8* %ACCESS17, align 1, !dbg !172
  %self20 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !173
  %data21 = getelementptr %struct.SockAddr, %struct.SockAddr* %self20, i32 0, i32 0, !dbg !173
  %DOT22 = load i8*, i8** %data21, align 8, !dbg !173
  %ACCESS23 = getelementptr i8, i8* %DOT22, i32 7, !dbg !173
  %d24 = load i32, i32* %d4, align 4, !dbg !173
  %as25 = trunc i32 %d24 to i8, !dbg !173
  store i8 %as25, i8* %ACCESS23, align 1, !dbg !173
  ret void, !dbg !173
}

define void @SockAddr.delete(%struct.SockAddr* %self) !dbg !174 {
entry:
  %self1 = alloca %struct.SockAddr*, align 8, !dbg !175
  store %struct.SockAddr* %self, %struct.SockAddr** %self1, align 8, !dbg !175
  %self2 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !176
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self2, i32 0, i32 0, !dbg !176
  %DOT = load i8*, i8** %data, align 8, !dbg !176
  call void @free(i8* %DOT), !dbg !176
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !176
  ret void, !dbg !176
}

declare i32 @write(i32, i8*, i32)

declare i64 @time(i8*)

declare i8* @localtime(i64*)

declare i64 @strftime(i8*, i64, i8*, i8*)

define void @log(i8* %msg) !dbg !177 {
entry:
  %msg1 = alloca i8*, align 8, !dbg !178
  store i8* %msg, i8** %msg1, align 8, !dbg !178
  %msg2 = load i8*, i8** %msg1, align 8, !dbg !179
  %msg3 = load i8*, i8** %msg1, align 8, !dbg !179
  %strlen = call i32 @strlen(i8* %msg3), !dbg !179
  %write = call i32 @write(i32 1, i8* %msg2, i32 %strlen), !dbg !179
  ret void, !dbg !179
}

define i8* @get_time() !dbg !180 {
entry:
  %buf = alloca i8*, align 8, !dbg !181
  %t = alloca i64, align 8, !dbg !181
  %tm = alloca i8*, align 8, !dbg !181
  store i8* null, i8** %buf, align 8, !dbg !181
  %heap = call i8* @calloc(i64 16, i64 1), !dbg !181
  store i8* %heap, i8** %buf, align 8, !dbg !181
  store i64 0, i64* %t, align 4, !dbg !181
  %time = call i64 @time(i8* null), !dbg !182
  store i64 %time, i64* %t, align 4, !dbg !182
  store i8* null, i8** %tm, align 8, !dbg !181
  %localtime = call i8* @localtime(i64* %t), !dbg !183
  store i8* %localtime, i8** %tm, align 8, !dbg !183
  %buf1 = load i8*, i8** %buf, align 8, !dbg !184
  %tm2 = load i8*, i8** %tm, align 8, !dbg !184
  %strftime = call i64 @strftime(i8* %buf1, i64 16, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR1, i32 0, i32 0), i8* %tm2), !dbg !184
  %buf3 = load i8*, i8** %buf, align 8, !dbg !184
  ret i8* %buf3, !dbg !184
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "utils.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/projects/ura-tcp-server/basic")
!4 = distinct !DISubprogram(name: "String.delete", linkageName: "String.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = !DILocation(line: 82, scope: !4)
!9 = distinct !DISubprogram(name: "String.assign", linkageName: "String.assign", scope: null, file: !3, line: 86, type: !5, scopeLine: 86, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 86, scope: !9)
!11 = !DILocation(line: 88, scope: !9)
!12 = !DILocation(line: 95, scope: !9)
!13 = !DILocation(line: 96, scope: !9)
!14 = !DILocation(line: 94, scope: !9)
!15 = distinct !DISubprogram(name: "String.join", linkageName: "String.join", scope: null, file: !3, line: 99, type: !5, scopeLine: 99, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!16 = !DILocation(line: 99, scope: !15)
!17 = !DILocation(line: 101, scope: !15)
!18 = !DILocation(line: 102, scope: !15)
!19 = !DILocation(line: 103, scope: !15)
!20 = distinct !DISubprogram(name: "String.clear", linkageName: "String.clear", scope: null, file: !3, line: 105, type: !5, scopeLine: 105, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 105, scope: !20)
!22 = !DILocation(line: 106, scope: !20)
!23 = distinct !DISubprogram(name: "String.push", linkageName: "String.push", scope: null, file: !3, line: 109, type: !5, scopeLine: 109, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!24 = !DILocation(line: 109, scope: !23)
!25 = !DILocation(line: 116, scope: !23)
!26 = !DILocation(line: 118, scope: !23)
!27 = !DILocation(line: 115, scope: !23)
!28 = distinct !DISubprogram(name: "String.pop", linkageName: "String.pop", scope: null, file: !3, line: 120, type: !5, scopeLine: 120, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!29 = !DILocation(line: 120, scope: !28)
!30 = !DILocation(line: 124, scope: !28)
!31 = !DILocation(line: 125, scope: !28)
!32 = distinct !DISubprogram(name: "String.new", linkageName: "String.new", scope: null, file: !3, line: 130, type: !5, scopeLine: 130, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!33 = !DILocation(line: 130, scope: !32)
!34 = !DILocation(line: 133, scope: !32)
!35 = distinct !DISubprogram(name: "String.from", linkageName: "String.from", scope: null, file: !3, line: 136, type: !5, scopeLine: 136, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!36 = !DILocation(line: 136, scope: !35)
!37 = !DILocation(line: 137, scope: !35)
!38 = !DILocation(line: 138, scope: !35)
!39 = distinct !DISubprogram(name: "String.from_int", linkageName: "String.from_int", scope: null, file: !3, line: 141, type: !5, scopeLine: 141, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!40 = !DILocation(line: 141, scope: !39)
!41 = !DILocation(line: 142, scope: !39)
!42 = !DILocation(line: 144, scope: !39)
!43 = !DILocation(line: 154, scope: !39)
!44 = !DILocation(line: 158, scope: !39)
!45 = !DILocation(line: 161, scope: !39)
!46 = distinct !DISubprogram(name: "String.len", linkageName: "String.len", scope: null, file: !3, line: 167, type: !5, scopeLine: 167, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!47 = !DILocation(line: 167, scope: !46)
!48 = distinct !DISubprogram(name: "String.empty", linkageName: "String.empty", scope: null, file: !3, line: 170, type: !5, scopeLine: 170, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!49 = !DILocation(line: 170, scope: !48)
!50 = distinct !DISubprogram(name: "String.c_str", linkageName: "String.c_str", scope: null, file: !3, line: 173, type: !5, scopeLine: 173, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!51 = !DILocation(line: 173, scope: !50)
!52 = distinct !DISubprogram(name: "String.at", linkageName: "String.at", scope: null, file: !3, line: 176, type: !5, scopeLine: 176, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!53 = !DILocation(line: 176, scope: !52)
!54 = !DILocation(line: 179, scope: !52)
!55 = distinct !DISubprogram(name: "String.find", linkageName: "String.find", scope: null, file: !3, line: 183, type: !5, scopeLine: 183, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!56 = !DILocation(line: 183, scope: !55)
!57 = !DILocation(line: 186, scope: !55)
!58 = !DILocation(line: 189, scope: !55)
!59 = distinct !DISubprogram(name: "String.contains", linkageName: "String.contains", scope: null, file: !3, line: 194, type: !5, scopeLine: 194, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!60 = !DILocation(line: 194, scope: !59)
!61 = !DILocation(line: 195, scope: !59)
!62 = distinct !DISubprogram(name: "String.starts_with", linkageName: "String.starts_with", scope: null, file: !3, line: 197, type: !5, scopeLine: 197, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!63 = !DILocation(line: 197, scope: !62)
!64 = !DILocation(line: 199, scope: !62)
!65 = !DILocation(line: 201, scope: !62)
!66 = distinct !DISubprogram(name: "String.ends_with", linkageName: "String.ends_with", scope: null, file: !3, line: 203, type: !5, scopeLine: 203, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!67 = !DILocation(line: 203, scope: !66)
!68 = !DILocation(line: 205, scope: !66)
!69 = !DILocation(line: 208, scope: !66)
!70 = distinct !DISubprogram(name: "String.substr", linkageName: "String.substr", scope: null, file: !3, line: 212, type: !5, scopeLine: 212, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!71 = !DILocation(line: 212, scope: !70)
!72 = !DILocation(line: 213, scope: !70)
!73 = !DILocation(line: 221, scope: !70)
!74 = distinct !DISubprogram(name: "String.upper", linkageName: "String.upper", scope: null, file: !3, line: 225, type: !5, scopeLine: 225, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!75 = !DILocation(line: 225, scope: !74)
!76 = !DILocation(line: 226, scope: !74)
!77 = !DILocation(line: 229, scope: !74)
!78 = !DILocation(line: 234, scope: !74)
!79 = !DILocation(line: 232, scope: !74)
!80 = distinct !DISubprogram(name: "String.lower", linkageName: "String.lower", scope: null, file: !3, line: 238, type: !5, scopeLine: 238, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!81 = !DILocation(line: 238, scope: !80)
!82 = !DILocation(line: 239, scope: !80)
!83 = !DILocation(line: 242, scope: !80)
!84 = !DILocation(line: 247, scope: !80)
!85 = !DILocation(line: 245, scope: !80)
!86 = distinct !DISubprogram(name: "String.trim", linkageName: "String.trim", scope: null, file: !3, line: 251, type: !5, scopeLine: 251, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!87 = !DILocation(line: 251, scope: !86)
!88 = !DILocation(line: 254, scope: !86)
!89 = !DILocation(line: 261, scope: !86)
!90 = !DILocation(line: 266, scope: !86)
!91 = distinct !DISubprogram(name: "String.replace", linkageName: "String.replace", scope: null, file: !3, line: 268, type: !5, scopeLine: 268, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!92 = !DILocation(line: 268, scope: !91)
!93 = !DILocation(line: 269, scope: !91)
!94 = !DILocation(line: 270, scope: !91)
!95 = !DILocation(line: 272, scope: !91)
!96 = !DILocation(line: 280, scope: !91)
!97 = !DILocation(line: 276, scope: !91)
!98 = !DILocation(line: 277, scope: !91)
!99 = distinct !DISubprogram(name: "String.repeat", linkageName: "String.repeat", scope: null, file: !3, line: 284, type: !5, scopeLine: 284, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!100 = !DILocation(line: 284, scope: !99)
!101 = !DILocation(line: 285, scope: !99)
!102 = !DILocation(line: 288, scope: !99)
!103 = distinct !DISubprogram(name: "String.reverse", linkageName: "String.reverse", scope: null, file: !3, line: 292, type: !5, scopeLine: 292, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!104 = !DILocation(line: 292, scope: !103)
!105 = !DILocation(line: 293, scope: !103)
!106 = !DILocation(line: 296, scope: !103)
!107 = distinct !DISubprogram(name: "String.compare", linkageName: "String.compare", scope: null, file: !3, line: 302, type: !5, scopeLine: 302, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!108 = !DILocation(line: 302, scope: !107)
!109 = !DILocation(line: 303, scope: !107)
!110 = distinct !DISubprogram(name: "String.to_int", linkageName: "String.to_int", scope: null, file: !3, line: 307, type: !5, scopeLine: 307, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!111 = !DILocation(line: 307, scope: !110)
!112 = !DILocation(line: 308, scope: !110)
!113 = distinct !DISubprogram(name: "String.operator.ASSIGN.CHARS", linkageName: "String.operator.ASSIGN.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!114 = !DILocation(line: 0, scope: !113)
!115 = !DILocation(line: 313, scope: !113)
!116 = distinct !DISubprogram(name: "String.operator.ASSIGN.String", linkageName: "String.operator.ASSIGN.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!117 = !DILocation(line: 0, scope: !116)
!118 = !DILocation(line: 316, scope: !116)
!119 = distinct !DISubprogram(name: "String.operator.ADD.String", linkageName: "String.operator.ADD.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!120 = !DILocation(line: 0, scope: !119)
!121 = !DILocation(line: 321, scope: !119)
!122 = !DILocation(line: 322, scope: !119)
!123 = distinct !DISubprogram(name: "String.operator.ADD.CHARS", linkageName: "String.operator.ADD.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!124 = !DILocation(line: 0, scope: !123)
!125 = !DILocation(line: 326, scope: !123)
!126 = !DILocation(line: 327, scope: !123)
!127 = distinct !DISubprogram(name: "String.operator.ADD_ASS.String", linkageName: "String.operator.ADD_ASS.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!128 = !DILocation(line: 0, scope: !127)
!129 = !DILocation(line: 331, scope: !127)
!130 = distinct !DISubprogram(name: "String.operator.ADD_ASS.CHARS", linkageName: "String.operator.ADD_ASS.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!131 = !DILocation(line: 0, scope: !130)
!132 = !DILocation(line: 334, scope: !130)
!133 = distinct !DISubprogram(name: "String.operator.EQ.String", linkageName: "String.operator.EQ.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!134 = !DILocation(line: 0, scope: !133)
!135 = !DILocation(line: 339, scope: !133)
!136 = distinct !DISubprogram(name: "String.operator.EQ.CHARS", linkageName: "String.operator.EQ.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!137 = !DILocation(line: 0, scope: !136)
!138 = !DILocation(line: 343, scope: !136)
!139 = distinct !DISubprogram(name: "String.operator.NEQ.String", linkageName: "String.operator.NEQ.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!140 = !DILocation(line: 0, scope: !139)
!141 = !DILocation(line: 346, scope: !139)
!142 = distinct !DISubprogram(name: "String.operator.NEQ.CHARS", linkageName: "String.operator.NEQ.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!143 = !DILocation(line: 0, scope: !142)
!144 = !DILocation(line: 350, scope: !142)
!145 = distinct !DISubprogram(name: "String.operator.LT.String", linkageName: "String.operator.LT.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!146 = !DILocation(line: 0, scope: !145)
!147 = !DILocation(line: 353, scope: !145)
!148 = distinct !DISubprogram(name: "String.operator.GT.String", linkageName: "String.operator.GT.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!149 = !DILocation(line: 0, scope: !148)
!150 = !DILocation(line: 356, scope: !148)
!151 = distinct !DISubprogram(name: "String.operator.output", linkageName: "String.operator.output", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!152 = !DILocation(line: 0, scope: !151)
!153 = distinct !DISubprogram(name: "strjoin", linkageName: "strjoin", scope: null, file: !3, line: 58, type: !5, scopeLine: 58, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!154 = !DILocation(line: 58, scope: !153)
!155 = !DILocation(line: 61, scope: !153)
!156 = !DILocation(line: 63, scope: !153)
!157 = !DILocation(line: 62, scope: !153)
!158 = !DILocation(line: 66, scope: !153)
!159 = !DILocation(line: 70, scope: !153)
!160 = distinct !DISubprogram(name: "SockAddr.init", linkageName: "SockAddr.init", scope: null, file: !3, line: 40, type: !5, scopeLine: 40, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!161 = !DILocation(line: 40, scope: !160)
!162 = !DILocation(line: 42, scope: !160)
!163 = !DILocation(line: 43, scope: !160)
!164 = distinct !DISubprogram(name: "SockAddr.set_port", linkageName: "SockAddr.set_port", scope: null, file: !3, line: 45, type: !5, scopeLine: 45, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!165 = !DILocation(line: 45, scope: !164)
!166 = !DILocation(line: 46, scope: !164)
!167 = !DILocation(line: 47, scope: !164)
!168 = distinct !DISubprogram(name: "SockAddr.set_ip", linkageName: "SockAddr.set_ip", scope: null, file: !3, line: 49, type: !5, scopeLine: 49, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!169 = !DILocation(line: 49, scope: !168)
!170 = !DILocation(line: 50, scope: !168)
!171 = !DILocation(line: 51, scope: !168)
!172 = !DILocation(line: 52, scope: !168)
!173 = !DILocation(line: 53, scope: !168)
!174 = distinct !DISubprogram(name: "SockAddr.delete", linkageName: "SockAddr.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!175 = !DILocation(line: 0, scope: !174)
!176 = !DILocation(line: 56, scope: !174)
!177 = distinct !DISubprogram(name: "log", linkageName: "log", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!178 = !DILocation(line: 3, scope: !177)
!179 = !DILocation(line: 4, scope: !177)
!180 = distinct !DISubprogram(name: "get_time", linkageName: "get_time", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!181 = !DILocation(line: 6, scope: !180)
!182 = !DILocation(line: 8, scope: !180)
!183 = !DILocation(line: 9, scope: !180)
!184 = !DILocation(line: 10, scope: !180)
