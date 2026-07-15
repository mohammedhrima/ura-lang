; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/list/006.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/list/006.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.__list_int = type { i32*, i32, i32 }
%struct.String = type { i8*, i32, i32 }

@STR0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@STR1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define void @__list_int.delete(%struct.__list_int* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.__list_int*, align 8, !dbg !7
  store %struct.__list_int* %self, %struct.__list_int** %self1, align 8, !dbg !7
  %self2 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !8
  %data = getelementptr %struct.__list_int, %struct.__list_int* %self2, i32 0, i32 0, !dbg !8
  %DOT = load i32*, i32** %data, align 8, !dbg !8
  %as = bitcast i32* %DOT to i8*, !dbg !8
  call void @free(i8* %as), !dbg !8
  %self3 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !8
  ret void, !dbg !8
}

define void @__list_int.push(i32 %e, %struct.__list_int* %self) !dbg !9 {
entry:
  %e1 = alloca i32, align 4, !dbg !10
  %self2 = alloca %struct.__list_int*, align 8, !dbg !10
  store i32 %e, i32* %e1, align 4, !dbg !10
  store %struct.__list_int* %self, %struct.__list_int** %self2, align 8, !dbg !10
  br label %if.start, !dbg !10

if.start:                                         ; preds = %entry
  %self3 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__len = getelementptr %struct.__list_int, %struct.__list_int* %self3, i32 0, i32 1, !dbg !10
  %self4 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__cap = getelementptr %struct.__list_int, %struct.__list_int* %self4, i32 0, i32 2, !dbg !10
  %DOT = load i32, i32* %__len, align 4, !dbg !10
  %DOT5 = load i32, i32* %__cap, align 4, !dbg !10
  %GE = icmp sge i32 %DOT, %DOT5, !dbg !10
  br i1 %GE, label %if.then, label %if.end, !dbg !10

if.end:                                           ; preds = %if.end7, %if.start
  %self25 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !11
  %data26 = getelementptr %struct.__list_int, %struct.__list_int* %self25, i32 0, i32 0, !dbg !11
  %self27 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !11
  %__len28 = getelementptr %struct.__list_int, %struct.__list_int* %self27, i32 0, i32 1, !dbg !11
  %DOT29 = load i32*, i32** %data26, align 8, !dbg !11
  %DOT30 = load i32, i32* %__len28, align 4, !dbg !11
  %ACCESS = getelementptr i32, i32* %DOT29, i32 %DOT30, !dbg !11
  %e31 = load i32, i32* %e1, align 4, !dbg !11
  store i32 %e31, i32* %ACCESS, align 4, !dbg !11
  %self32 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !11
  %__len33 = getelementptr %struct.__list_int, %struct.__list_int* %self32, i32 0, i32 1, !dbg !11
  %DOT34 = load i32, i32* %__len33, align 4, !dbg !11
  %ADD = add i32 %DOT34, 1, !dbg !11
  store i32 %ADD, i32* %__len33, align 4, !dbg !11
  ret void, !dbg !11

if.then:                                          ; preds = %if.start
  br label %if.start6, !dbg !10

if.start6:                                        ; preds = %if.then
  %self9 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__cap10 = getelementptr %struct.__list_int, %struct.__list_int* %self9, i32 0, i32 2, !dbg !10
  %DOT11 = load i32, i32* %__cap10, align 4, !dbg !10
  %EQ = icmp eq i32 %DOT11, 0, !dbg !10
  br i1 %EQ, label %if.then8, label %if.else, !dbg !10

if.end7:                                          ; preds = %if.else, %if.then8
  %self17 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %data = getelementptr %struct.__list_int, %struct.__list_int* %self17, i32 0, i32 0, !dbg !10
  %self18 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !12
  %data19 = getelementptr %struct.__list_int, %struct.__list_int* %self18, i32 0, i32 0, !dbg !12
  %DOT20 = load i32*, i32** %data19, align 8, !dbg !12
  %as = bitcast i32* %DOT20 to i8*, !dbg !12
  %self21 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !12
  %__cap22 = getelementptr %struct.__list_int, %struct.__list_int* %self21, i32 0, i32 2, !dbg !12
  %DOT23 = load i32, i32* %__cap22, align 4, !dbg !12
  %MUL24 = mul i32 %DOT23, 4, !dbg !12
  %realloc = call i8* @realloc(i8* %as, i32 %MUL24), !dbg !12
  %ptr_cast = bitcast i8* %realloc to i32*, !dbg !12
  store i32* %ptr_cast, i32** %data, align 8, !dbg !12
  br label %if.end, !dbg !12

if.then8:                                         ; preds = %if.start6
  %self12 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__cap13 = getelementptr %struct.__list_int, %struct.__list_int* %self12, i32 0, i32 2, !dbg !10
  store i32 8, i32* %__cap13, align 4, !dbg !10
  br label %if.end7, !dbg !10

if.else:                                          ; preds = %if.start6
  %self14 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !10
  %__cap15 = getelementptr %struct.__list_int, %struct.__list_int* %self14, i32 0, i32 2, !dbg !10
  %DOT16 = load i32, i32* %__cap15, align 4, !dbg !10
  %MUL = mul i32 %DOT16, 2, !dbg !10
  store i32 %MUL, i32* %__cap15, align 4, !dbg !10
  br label %if.end7, !dbg !10
}

define i32 @__list_int.pop(%struct.__list_int* %self) !dbg !13 {
entry:
  %self1 = alloca %struct.__list_int*, align 8, !dbg !14
  store %struct.__list_int* %self, %struct.__list_int** %self1, align 8, !dbg !14
  %self2 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !14
  %__len = getelementptr %struct.__list_int, %struct.__list_int* %self2, i32 0, i32 1, !dbg !14
  %DOT = load i32, i32* %__len, align 4, !dbg !14
  %SUB = sub i32 %DOT, 1, !dbg !14
  store i32 %SUB, i32* %__len, align 4, !dbg !14
  %self3 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !15
  %data = getelementptr %struct.__list_int, %struct.__list_int* %self3, i32 0, i32 0, !dbg !15
  %self4 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !15
  %__len5 = getelementptr %struct.__list_int, %struct.__list_int* %self4, i32 0, i32 1, !dbg !15
  %DOT6 = load i32*, i32** %data, align 8, !dbg !15
  %DOT7 = load i32, i32* %__len5, align 4, !dbg !15
  %ACCESS = getelementptr i32, i32* %DOT6, i32 %DOT7, !dbg !15
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !15
  ret i32 %ACC, !dbg !15
}

define i32 @__list_int.len(%struct.__list_int* %self) !dbg !16 {
entry:
  %self1 = alloca %struct.__list_int*, align 8, !dbg !17
  store %struct.__list_int* %self, %struct.__list_int** %self1, align 8, !dbg !17
  %self2 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !17
  %__len = getelementptr %struct.__list_int, %struct.__list_int* %self2, i32 0, i32 1, !dbg !17
  %DOT = load i32, i32* %__len, align 4, !dbg !17
  ret i32 %DOT, !dbg !17
}

define i32 @__list_int.cap(%struct.__list_int* %self) !dbg !18 {
entry:
  %self1 = alloca %struct.__list_int*, align 8, !dbg !19
  store %struct.__list_int* %self, %struct.__list_int** %self1, align 8, !dbg !19
  %self2 = load %struct.__list_int*, %struct.__list_int** %self1, align 8, !dbg !19
  %__cap = getelementptr %struct.__list_int, %struct.__list_int* %self2, i32 0, i32 2, !dbg !19
  %DOT = load i32, i32* %__cap, align 4, !dbg !19
  ret i32 %DOT, !dbg !19
}

define void @__list_int.foreach(void (i32)* %cb, %struct.__list_int* %self) !dbg !20 {
entry:
  %cb1 = alloca void (i32)*, align 8, !dbg !21
  %self2 = alloca %struct.__list_int*, align 8, !dbg !21
  %i = alloca i32, align 4, !dbg !21
  store void (i32)* %cb, void (i32)** %cb1, align 8, !dbg !21
  store %struct.__list_int* %self, %struct.__list_int** %self2, align 8, !dbg !21
  store i32 0, i32* %i, align 4, !dbg !21
  store i32 0, i32* %i, align 4, !dbg !21
  br label %while.start, !dbg !21

while.start:                                      ; preds = %while.then, %entry
  %self3 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !21
  %__len = getelementptr %struct.__list_int, %struct.__list_int* %self3, i32 0, i32 1, !dbg !21
  %i4 = load i32, i32* %i, align 4, !dbg !21
  %DOT = load i32, i32* %__len, align 4, !dbg !21
  %LT = icmp slt i32 %i4, %DOT, !dbg !21
  br i1 %LT, label %while.then, label %while.end, !dbg !21

while.then:                                       ; preds = %while.start
  %fn_ptr = load void (i32)*, void (i32)** %cb1, align 8, !dbg !22
  %self5 = load %struct.__list_int*, %struct.__list_int** %self2, align 8, !dbg !22
  %data = getelementptr %struct.__list_int, %struct.__list_int* %self5, i32 0, i32 0, !dbg !22
  %DOT6 = load i32*, i32** %data, align 8, !dbg !22
  %i7 = load i32, i32* %i, align 4, !dbg !22
  %ACCESS = getelementptr i32, i32* %DOT6, i32 %i7, !dbg !22
  %ACC = load i32, i32* %ACCESS, align 4, !dbg !22
  call void %fn_ptr(i32 %ACC), !dbg !22
  %i8 = load i32, i32* %i, align 4, !dbg !22
  %ADD = add i32 %i8, 1, !dbg !22
  store i32 %ADD, i32* %i, align 4, !dbg !22
  br label %while.start, !dbg !22

while.end:                                        ; preds = %while.start
  ret void, !dbg !22
}

declare void @free(i8*)

declare i8* @realloc(i8*, i32)

define void @String.delete(%struct.String* %self) !dbg !23 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !24
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !24
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !25
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !25
  %DOT = load i8*, i8** %value, align 8, !dbg !25
  call void @free(i8* %DOT), !dbg !25
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !25
  ret void, !dbg !25
}

define void @String.assign(i8* %str, %struct.String* %self) !dbg !26 {
entry:
  %str1 = alloca i8*, align 8, !dbg !27
  %self2 = alloca %struct.String*, align 8, !dbg !27
  %len = alloca i32, align 4, !dbg !27
  store i8* %str, i8** %str1, align 8, !dbg !27
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !27
  br label %if.start, !dbg !27

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !27
  %EQ = icmp eq i8* %str3, null, !dbg !27
  br i1 %EQ, label %if.then, label %if.end, !dbg !27

if.end:                                           ; preds = %if.then, %if.start
  store i32 0, i32* %len, align 4, !dbg !27
  %str4 = load i8*, i8** %str1, align 8, !dbg !28
  %strlen = call i32 @strlen(i8* %str4), !dbg !28
  store i32 %strlen, i32* %len, align 4, !dbg !28
  br label %if.start5, !dbg !28

if.then:                                          ; preds = %if.start
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @STR0, i32 0, i32 0), i8** %str1, align 8, !dbg !27
  br label %if.end, !dbg !27

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !28
  %value = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 0, !dbg !28
  %DOT = load i8*, i8** %value, align 8, !dbg !28
  %EQ9 = icmp eq i8* %DOT, null, !dbg !28
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !28
  %size = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 2, !dbg !28
  %DOT11 = load i32, i32* %size, align 4, !dbg !28
  %len12 = load i32, i32* %len, align 4, !dbg !28
  %LE = icmp sle i32 %DOT11, %len12, !dbg !28
  %OR = or i1 %EQ9, %LE, !dbg !28
  br i1 %OR, label %if.then7, label %if.end6, !dbg !28

if.end6:                                          ; preds = %while.end, %if.start5
  %self38 = load %struct.String*, %struct.String** %self2, align 8, !dbg !29
  %value39 = getelementptr %struct.String, %struct.String* %self38, i32 0, i32 0, !dbg !29
  %DOT40 = load i8*, i8** %value39, align 8, !dbg !29
  %str41 = load i8*, i8** %str1, align 8, !dbg !29
  %strcpy = call i8* @strcpy(i8* %DOT40, i8* %str41), !dbg !29
  %self42 = load %struct.String*, %struct.String** %self2, align 8, !dbg !30
  %value43 = getelementptr %struct.String, %struct.String* %self42, i32 0, i32 0, !dbg !30
  %DOT44 = load i8*, i8** %value43, align 8, !dbg !30
  %len45 = load i32, i32* %len, align 4, !dbg !30
  %idx = sext i32 %len45 to i64, !dbg !30
  %ADD = getelementptr i8, i8* %DOT44, i64 %idx, !dbg !30
  %self46 = load %struct.String*, %struct.String** %self2, align 8, !dbg !30
  %size47 = getelementptr %struct.String, %struct.String* %self46, i32 0, i32 2, !dbg !30
  %DOT48 = load i32, i32* %size47, align 4, !dbg !30
  %len49 = load i32, i32* %len, align 4, !dbg !30
  %SUB = sub i32 %DOT48, %len49, !dbg !30
  %as = sext i32 %SUB to i64, !dbg !30
  call void @bzero(i8* %ADD, i64 %as), !dbg !30
  %self50 = load %struct.String*, %struct.String** %self2, align 8, !dbg !30
  %count = getelementptr %struct.String, %struct.String* %self50, i32 0, i32 1, !dbg !30
  %len51 = load i32, i32* %len, align 4, !dbg !30
  store i32 %len51, i32* %count, align 4, !dbg !30
  ret void, !dbg !30

if.then7:                                         ; preds = %if.start5
  br label %if.start13, !dbg !28

if.start13:                                       ; preds = %if.then7
  %self16 = load %struct.String*, %struct.String** %self2, align 8, !dbg !28
  %size17 = getelementptr %struct.String, %struct.String* %self16, i32 0, i32 2, !dbg !28
  %DOT18 = load i32, i32* %size17, align 4, !dbg !28
  %EQ19 = icmp eq i32 %DOT18, 0, !dbg !28
  br i1 %EQ19, label %if.then15, label %if.end14, !dbg !28

if.end14:                                         ; preds = %if.then15, %if.start13
  br label %while.start, !dbg !28

if.then15:                                        ; preds = %if.start13
  %self20 = load %struct.String*, %struct.String** %self2, align 8, !dbg !28
  %size21 = getelementptr %struct.String, %struct.String* %self20, i32 0, i32 2, !dbg !28
  store i32 10, i32* %size21, align 4, !dbg !28
  br label %if.end14, !dbg !28

while.start:                                      ; preds = %while.then, %if.end14
  %self22 = load %struct.String*, %struct.String** %self2, align 8, !dbg !28
  %size23 = getelementptr %struct.String, %struct.String* %self22, i32 0, i32 2, !dbg !28
  %DOT24 = load i32, i32* %size23, align 4, !dbg !28
  %len25 = load i32, i32* %len, align 4, !dbg !28
  %LE26 = icmp sle i32 %DOT24, %len25, !dbg !28
  br i1 %LE26, label %while.then, label %while.end, !dbg !28

while.then:                                       ; preds = %while.start
  %self27 = load %struct.String*, %struct.String** %self2, align 8, !dbg !28
  %size28 = getelementptr %struct.String, %struct.String* %self27, i32 0, i32 2, !dbg !28
  %DOT29 = load i32, i32* %size28, align 4, !dbg !28
  %MUL = mul i32 %DOT29, 2, !dbg !28
  store i32 %MUL, i32* %size28, align 4, !dbg !28
  br label %while.start, !dbg !28

while.end:                                        ; preds = %while.start
  %self30 = load %struct.String*, %struct.String** %self2, align 8, !dbg !28
  %value31 = getelementptr %struct.String, %struct.String* %self30, i32 0, i32 0, !dbg !28
  %self32 = load %struct.String*, %struct.String** %self2, align 8, !dbg !31
  %value33 = getelementptr %struct.String, %struct.String* %self32, i32 0, i32 0, !dbg !31
  %DOT34 = load i8*, i8** %value33, align 8, !dbg !31
  %self35 = load %struct.String*, %struct.String** %self2, align 8, !dbg !31
  %size36 = getelementptr %struct.String, %struct.String* %self35, i32 0, i32 2, !dbg !31
  %DOT37 = load i32, i32* %size36, align 4, !dbg !31
  %realloc = call i8* @realloc(i8* %DOT34, i32 %DOT37), !dbg !31
  store i8* %realloc, i8** %value31, align 8, !dbg !31
  br label %if.end6, !dbg !31
}

define void @String.join(i8* %str, %struct.String* %self) !dbg !32 {
entry:
  %str1 = alloca i8*, align 8, !dbg !33
  %self2 = alloca %struct.String*, align 8, !dbg !33
  %res = alloca i8*, align 8, !dbg !33
  store i8* %str, i8** %str1, align 8, !dbg !33
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !33
  br label %if.start, !dbg !33

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !33
  %EQ = icmp eq i8* %str3, null, !dbg !33
  br i1 %EQ, label %if.then, label %if.end, !dbg !33

if.end:                                           ; preds = %if.start
  store i8* null, i8** %res, align 8, !dbg !33
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !34
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !34
  %DOT = load i8*, i8** %value, align 8, !dbg !34
  %str5 = load i8*, i8** %str1, align 8, !dbg !34
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str5), !dbg !34
  store i8* %strjoin, i8** %res, align 8, !dbg !34
  %res6 = load i8*, i8** %res, align 8, !dbg !35
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !35
  call void @String.assign(i8* %res6, %struct.String* %ref_arg), !dbg !35
  %res7 = load i8*, i8** %res, align 8, !dbg !36
  call void @free(i8* %res7), !dbg !36
  ret void, !dbg !36

if.then:                                          ; preds = %if.start
  ret void, !dbg !33
}

define void @String.clear(%struct.String* %self) !dbg !37 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !38
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !38
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !39
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !39
  %DOT = load i8*, i8** %value, align 8, !dbg !39
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !39
  %size = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 2, !dbg !39
  %DOT4 = load i32, i32* %size, align 4, !dbg !39
  %as = sext i32 %DOT4 to i64, !dbg !39
  call void @bzero(i8* %DOT, i64 %as), !dbg !39
  %self5 = load %struct.String*, %struct.String** %self1, align 8, !dbg !39
  %count = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 1, !dbg !39
  store i32 0, i32* %count, align 4, !dbg !39
  ret void, !dbg !39
}

define void @String.push(i8 %c, %struct.String* %self) !dbg !40 {
entry:
  %c1 = alloca i8, align 1, !dbg !41
  %self2 = alloca %struct.String*, align 8, !dbg !41
  store i8 %c, i8* %c1, align 1, !dbg !41
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !41
  br label %if.start, !dbg !41

if.start:                                         ; preds = %entry
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !41
  %count = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 1, !dbg !41
  %DOT = load i32, i32* %count, align 4, !dbg !41
  %ADD = add i32 %DOT, 1, !dbg !41
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !41
  %size = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 2, !dbg !41
  %DOT5 = load i32, i32* %size, align 4, !dbg !41
  %GE = icmp sge i32 %ADD, %DOT5, !dbg !41
  br i1 %GE, label %if.then, label %if.end, !dbg !41

if.end:                                           ; preds = %if.end7, %if.start
  %self24 = load %struct.String*, %struct.String** %self2, align 8, !dbg !42
  %value25 = getelementptr %struct.String, %struct.String* %self24, i32 0, i32 0, !dbg !42
  %self26 = load %struct.String*, %struct.String** %self2, align 8, !dbg !42
  %count27 = getelementptr %struct.String, %struct.String* %self26, i32 0, i32 1, !dbg !42
  %DOT28 = load i8*, i8** %value25, align 8, !dbg !42
  %DOT29 = load i32, i32* %count27, align 4, !dbg !42
  %ACCESS = getelementptr i8, i8* %DOT28, i32 %DOT29, !dbg !42
  %c30 = load i8, i8* %c1, align 1, !dbg !42
  store i8 %c30, i8* %ACCESS, align 1, !dbg !42
  %self31 = load %struct.String*, %struct.String** %self2, align 8, !dbg !42
  %count32 = getelementptr %struct.String, %struct.String* %self31, i32 0, i32 1, !dbg !42
  %DOT33 = load i32, i32* %count32, align 4, !dbg !42
  %ADD34 = add i32 %DOT33, 1, !dbg !42
  store i32 %ADD34, i32* %count32, align 4, !dbg !42
  %self35 = load %struct.String*, %struct.String** %self2, align 8, !dbg !43
  %value36 = getelementptr %struct.String, %struct.String* %self35, i32 0, i32 0, !dbg !43
  %self37 = load %struct.String*, %struct.String** %self2, align 8, !dbg !43
  %count38 = getelementptr %struct.String, %struct.String* %self37, i32 0, i32 1, !dbg !43
  %DOT39 = load i8*, i8** %value36, align 8, !dbg !43
  %DOT40 = load i32, i32* %count38, align 4, !dbg !43
  %ACCESS41 = getelementptr i8, i8* %DOT39, i32 %DOT40, !dbg !43
  store i8 0, i8* %ACCESS41, align 1, !dbg !43
  ret void, !dbg !43

if.then:                                          ; preds = %if.start
  br label %if.start6, !dbg !41

if.start6:                                        ; preds = %if.then
  %self9 = load %struct.String*, %struct.String** %self2, align 8, !dbg !41
  %size10 = getelementptr %struct.String, %struct.String* %self9, i32 0, i32 2, !dbg !41
  %DOT11 = load i32, i32* %size10, align 4, !dbg !41
  %EQ = icmp eq i32 %DOT11, 0, !dbg !41
  br i1 %EQ, label %if.then8, label %if.else, !dbg !41

if.end7:                                          ; preds = %if.else, %if.then8
  %self17 = load %struct.String*, %struct.String** %self2, align 8, !dbg !41
  %value = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 0, !dbg !41
  %self18 = load %struct.String*, %struct.String** %self2, align 8, !dbg !44
  %value19 = getelementptr %struct.String, %struct.String* %self18, i32 0, i32 0, !dbg !44
  %DOT20 = load i8*, i8** %value19, align 8, !dbg !44
  %self21 = load %struct.String*, %struct.String** %self2, align 8, !dbg !44
  %size22 = getelementptr %struct.String, %struct.String* %self21, i32 0, i32 2, !dbg !44
  %DOT23 = load i32, i32* %size22, align 4, !dbg !44
  %realloc = call i8* @realloc(i8* %DOT20, i32 %DOT23), !dbg !44
  store i8* %realloc, i8** %value, align 8, !dbg !44
  br label %if.end, !dbg !44

if.then8:                                         ; preds = %if.start6
  %self12 = load %struct.String*, %struct.String** %self2, align 8, !dbg !41
  %size13 = getelementptr %struct.String, %struct.String* %self12, i32 0, i32 2, !dbg !41
  store i32 10, i32* %size13, align 4, !dbg !41
  br label %if.end7, !dbg !41

if.else:                                          ; preds = %if.start6
  %self14 = load %struct.String*, %struct.String** %self2, align 8, !dbg !41
  %size15 = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 2, !dbg !41
  %DOT16 = load i32, i32* %size15, align 4, !dbg !41
  %MUL = mul i32 %DOT16, 2, !dbg !41
  store i32 %MUL, i32* %size15, align 4, !dbg !41
  br label %if.end7, !dbg !41
}

define i8 @String.pop(%struct.String* %self) !dbg !45 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !46
  %c = alloca i8, align 1, !dbg !46
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !46
  br label %if.start, !dbg !46

if.start:                                         ; preds = %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !46
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !46
  %DOT = load i32, i32* %count, align 4, !dbg !46
  %EQ = icmp eq i32 %DOT, 0, !dbg !46
  br i1 %EQ, label %if.then, label %if.end, !dbg !46

if.end:                                           ; preds = %if.start
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !46
  %count4 = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 1, !dbg !46
  %DOT5 = load i32, i32* %count4, align 4, !dbg !46
  %SUB = sub i32 %DOT5, 1, !dbg !46
  store i32 %SUB, i32* %count4, align 4, !dbg !46
  store i8 0, i8* %c, align 1, !dbg !46
  %self6 = load %struct.String*, %struct.String** %self1, align 8, !dbg !47
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !47
  %self7 = load %struct.String*, %struct.String** %self1, align 8, !dbg !47
  %count8 = getelementptr %struct.String, %struct.String* %self7, i32 0, i32 1, !dbg !47
  %DOT9 = load i8*, i8** %value, align 8, !dbg !47
  %DOT10 = load i32, i32* %count8, align 4, !dbg !47
  %ACCESS = getelementptr i8, i8* %DOT9, i32 %DOT10, !dbg !47
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !47
  store i8 %ACC, i8* %c, align 1, !dbg !47
  %self11 = load %struct.String*, %struct.String** %self1, align 8, !dbg !48
  %value12 = getelementptr %struct.String, %struct.String* %self11, i32 0, i32 0, !dbg !48
  %self13 = load %struct.String*, %struct.String** %self1, align 8, !dbg !48
  %count14 = getelementptr %struct.String, %struct.String* %self13, i32 0, i32 1, !dbg !48
  %DOT15 = load i8*, i8** %value12, align 8, !dbg !48
  %DOT16 = load i32, i32* %count14, align 4, !dbg !48
  %ACCESS17 = getelementptr i8, i8* %DOT15, i32 %DOT16, !dbg !48
  store i8 0, i8* %ACCESS17, align 1, !dbg !48
  %c18 = load i8, i8* %c, align 1, !dbg !48
  ret i8 %c18, !dbg !48

if.then:                                          ; preds = %if.start
  ret i8 0, !dbg !46
}

define %struct.String @String.new() !dbg !49 {
entry:
  %s = alloca %struct.String, align 8, !dbg !50
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !50
  %size = getelementptr %struct.String, %struct.String* %s, i32 0, i32 2, !dbg !50
  store i32 10, i32* %size, align 4, !dbg !50
  %value = getelementptr %struct.String, %struct.String* %s, i32 0, i32 0, !dbg !50
  %calloc = call i8* @calloc(i64 10, i64 1), !dbg !51
  store i8* %calloc, i8** %value, align 8, !dbg !51
  %s1 = load %struct.String, %struct.String* %s, align 8, !dbg !51
  ret %struct.String %s1, !dbg !51
}

define %struct.String @String.from(i8* %str) !dbg !52 {
entry:
  %str1 = alloca i8*, align 8, !dbg !53
  %s = alloca %struct.String, align 8, !dbg !53
  store i8* %str, i8** %str1, align 8, !dbg !53
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !53
  %0 = call %struct.String @String.new(), !dbg !54
  %tmp_struct = alloca %struct.String, align 8, !dbg !54
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !54
  %tmp_op = alloca %struct.String, align 8, !dbg !54
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !54
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s), !dbg !54
  %str2 = load i8*, i8** %str1, align 8, !dbg !55
  call void @String.assign(i8* %str2, %struct.String* %s), !dbg !55
  call void @String.delete(%struct.String* %tmp_struct), !dbg !55
  %s3 = load %struct.String, %struct.String* %s, align 8, !dbg !55
  ret %struct.String %s3, !dbg !55
}

define %struct.String @String.from_int(i32 %n) !dbg !56 {
entry:
  %n1 = alloca i32, align 4, !dbg !57
  %s = alloca %struct.String, align 8, !dbg !57
  %neg = alloca i1, align 1, !dbg !57
  %num = alloca i32, align 4, !dbg !57
  %buf = alloca i8*, align 8, !dbg !57
  %i = alloca i32, align 4, !dbg !57
  %j = alloca i32, align 4, !dbg !57
  store i32 %n, i32* %n1, align 4, !dbg !57
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !57
  %0 = call %struct.String @String.new(), !dbg !58
  %tmp_struct = alloca %struct.String, align 8, !dbg !58
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !58
  %tmp_op = alloca %struct.String, align 8, !dbg !58
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !58
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s), !dbg !58
  br label %if.start, !dbg !58

if.start:                                         ; preds = %entry
  %n2 = load i32, i32* %n1, align 4, !dbg !58
  %EQ = icmp eq i32 %n2, 0, !dbg !58
  br i1 %EQ, label %if.then, label %if.end, !dbg !58

if.end:                                           ; preds = %if.start
  store i1 false, i1* %neg, align 1, !dbg !57
  store i1 false, i1* %neg, align 1, !dbg !57
  store i32 0, i32* %num, align 4, !dbg !57
  %n4 = load i32, i32* %n1, align 4, !dbg !57
  store i32 %n4, i32* %num, align 4, !dbg !57
  br label %if.start5, !dbg !57

if.then:                                          ; preds = %if.start
  call void @String.push(i8 48, %struct.String* %s), !dbg !59
  %s3 = load %struct.String, %struct.String* %s, align 8, !dbg !59
  ret %struct.String %s3, !dbg !59

if.start5:                                        ; preds = %if.end
  %num8 = load i32, i32* %num, align 4, !dbg !57
  %LT = icmp slt i32 %num8, 0, !dbg !57
  br i1 %LT, label %if.then7, label %if.end6, !dbg !57

if.end6:                                          ; preds = %if.then7, %if.start5
  store i8* null, i8** %buf, align 8, !dbg !57
  %stack = alloca [20 x i8], align 1, !dbg !57
  %stack10 = getelementptr [20 x i8], [20 x i8]* %stack, i32 0, i32 0, !dbg !57
  store i8* %stack10, i8** %buf, align 8, !dbg !57
  store i32 0, i32* %i, align 4, !dbg !57
  store i32 0, i32* %i, align 4, !dbg !57
  br label %while.start, !dbg !57

if.then7:                                         ; preds = %if.start5
  store i1 true, i1* %neg, align 1, !dbg !57
  %num9 = load i32, i32* %num, align 4, !dbg !57
  %SUB = sub i32 0, %num9, !dbg !57
  store i32 %SUB, i32* %num, align 4, !dbg !57
  br label %if.end6, !dbg !57

while.start:                                      ; preds = %while.then, %if.end6
  %num11 = load i32, i32* %num, align 4, !dbg !57
  %GT = icmp sgt i32 %num11, 0, !dbg !57
  br i1 %GT, label %while.then, label %while.end, !dbg !57

while.then:                                       ; preds = %while.start
  %buf12 = load i8*, i8** %buf, align 8, !dbg !60
  %i13 = load i32, i32* %i, align 4, !dbg !60
  %ACCESS = getelementptr i8, i8* %buf12, i32 %i13, !dbg !60
  %num14 = load i32, i32* %num, align 4, !dbg !60
  %MOD = srem i32 %num14, 10, !dbg !60
  %ADD = add i32 %MOD, 48, !dbg !60
  %as = trunc i32 %ADD to i8, !dbg !60
  store i8 %as, i8* %ACCESS, align 1, !dbg !60
  %num15 = load i32, i32* %num, align 4, !dbg !60
  %DIV = sdiv i32 %num15, 10, !dbg !60
  store i32 %DIV, i32* %num, align 4, !dbg !60
  %i16 = load i32, i32* %i, align 4, !dbg !60
  %ADD17 = add i32 %i16, 1, !dbg !60
  store i32 %ADD17, i32* %i, align 4, !dbg !60
  br label %while.start, !dbg !60

while.end:                                        ; preds = %while.start
  br label %if.start18, !dbg !60

if.start18:                                       ; preds = %while.end
  %neg21 = load i1, i1* %neg, align 1, !dbg !60
  br i1 %neg21, label %if.then20, label %if.end19, !dbg !60

if.end19:                                         ; preds = %if.then20, %if.start18
  store i32 0, i32* %j, align 4, !dbg !57
  %i22 = load i32, i32* %i, align 4, !dbg !57
  %SUB23 = sub i32 %i22, 1, !dbg !57
  store i32 %SUB23, i32* %j, align 4, !dbg !57
  br label %while.start24, !dbg !57

if.then20:                                        ; preds = %if.start18
  call void @String.push(i8 45, %struct.String* %s), !dbg !61
  br label %if.end19, !dbg !61

while.start24:                                    ; preds = %while.then25, %if.end19
  %j27 = load i32, i32* %j, align 4, !dbg !57
  %GE = icmp sge i32 %j27, 0, !dbg !57
  br i1 %GE, label %while.then25, label %while.end26, !dbg !57

while.then25:                                     ; preds = %while.start24
  %buf28 = load i8*, i8** %buf, align 8, !dbg !62
  %j29 = load i32, i32* %j, align 4, !dbg !62
  %ACCESS30 = getelementptr i8, i8* %buf28, i32 %j29, !dbg !62
  %ACC = load i8, i8* %ACCESS30, align 1, !dbg !62
  call void @String.push(i8 %ACC, %struct.String* %s), !dbg !62
  %j31 = load i32, i32* %j, align 4, !dbg !62
  %SUB32 = sub i32 %j31, 1, !dbg !62
  store i32 %SUB32, i32* %j, align 4, !dbg !62
  br label %while.start24, !dbg !62

while.end26:                                      ; preds = %while.start24
  call void @String.delete(%struct.String* %tmp_struct), !dbg !62
  %s33 = load %struct.String, %struct.String* %s, align 8, !dbg !62
  ret %struct.String %s33, !dbg !62
}

define i32 @String.len(%struct.String* %self) !dbg !63 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !64
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !64
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !64
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !64
  %DOT = load i32, i32* %count, align 4, !dbg !64
  ret i32 %DOT, !dbg !64
}

define i1 @String.empty(%struct.String* %self) !dbg !65 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !66
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !66
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !66
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !66
  %DOT = load i32, i32* %count, align 4, !dbg !66
  %EQ = icmp eq i32 %DOT, 0, !dbg !66
  ret i1 %EQ, !dbg !66
}

define i8* @String.c_str(%struct.String* %self) !dbg !67 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !68
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !68
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !68
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !68
  %DOT = load i8*, i8** %value, align 8, !dbg !68
  ret i8* %DOT, !dbg !68
}

define i8 @String.at(i32 %i, %struct.String* %self) !dbg !69 {
entry:
  %i1 = alloca i32, align 4, !dbg !70
  %self2 = alloca %struct.String*, align 8, !dbg !70
  store i32 %i, i32* %i1, align 4, !dbg !70
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !70
  br label %if.start, !dbg !70

if.start:                                         ; preds = %entry
  %i3 = load i32, i32* %i1, align 4, !dbg !70
  %LT = icmp slt i32 %i3, 0, !dbg !70
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !70
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !70
  %i5 = load i32, i32* %i1, align 4, !dbg !70
  %DOT = load i32, i32* %count, align 4, !dbg !70
  %GE = icmp sge i32 %i5, %DOT, !dbg !70
  %OR = or i1 %LT, %GE, !dbg !70
  br i1 %OR, label %if.then, label %if.end, !dbg !70

if.end:                                           ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self2, align 8, !dbg !71
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !71
  %DOT7 = load i8*, i8** %value, align 8, !dbg !71
  %i8 = load i32, i32* %i1, align 4, !dbg !71
  %ACCESS = getelementptr i8, i8* %DOT7, i32 %i8, !dbg !71
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !71
  ret i8 %ACC, !dbg !71

if.then:                                          ; preds = %if.start
  ret i8 0, !dbg !70
}

define i32 @String.find(i8* %needle, %struct.String* %self) !dbg !72 {
entry:
  %needle1 = alloca i8*, align 8, !dbg !73
  %self2 = alloca %struct.String*, align 8, !dbg !73
  %i = alloca i32, align 4, !dbg !73
  %nlen = alloca i32, align 4, !dbg !73
  store i8* %needle, i8** %needle1, align 8, !dbg !73
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !73
  br label %if.start, !dbg !73

if.start:                                         ; preds = %entry
  %needle3 = load i8*, i8** %needle1, align 8, !dbg !73
  %EQ = icmp eq i8* %needle3, null, !dbg !73
  br i1 %EQ, label %if.then, label %if.end, !dbg !73

if.end:                                           ; preds = %if.start
  store i32 0, i32* %i, align 4, !dbg !73
  store i32 0, i32* %i, align 4, !dbg !73
  store i32 0, i32* %nlen, align 4, !dbg !73
  %needle4 = load i8*, i8** %needle1, align 8, !dbg !74
  %strlen = call i32 @strlen(i8* %needle4), !dbg !74
  store i32 %strlen, i32* %nlen, align 4, !dbg !74
  br label %if.start5, !dbg !74

if.then:                                          ; preds = %if.start
  ret i32 -1, !dbg !73

if.start5:                                        ; preds = %if.end
  %nlen8 = load i32, i32* %nlen, align 4, !dbg !74
  %EQ9 = icmp eq i32 %nlen8, 0, !dbg !74
  br i1 %EQ9, label %if.then7, label %if.end6, !dbg !74

if.end6:                                          ; preds = %if.start5
  br label %while.start, !dbg !74

if.then7:                                         ; preds = %if.start5
  ret i32 0, !dbg !74

while.start:                                      ; preds = %if.end14, %if.end6
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !74
  %count = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 1, !dbg !74
  %DOT = load i32, i32* %count, align 4, !dbg !74
  %nlen11 = load i32, i32* %nlen, align 4, !dbg !74
  %SUB = sub i32 %DOT, %nlen11, !dbg !74
  %i12 = load i32, i32* %i, align 4, !dbg !74
  %LE = icmp sle i32 %i12, %SUB, !dbg !74
  br i1 %LE, label %while.then, label %while.end, !dbg !74

while.then:                                       ; preds = %while.start
  br label %if.start13, !dbg !74

while.end:                                        ; preds = %while.start
  ret i32 -1, !dbg !75

if.start13:                                       ; preds = %while.then
  %self16 = load %struct.String*, %struct.String** %self2, align 8, !dbg !75
  %value = getelementptr %struct.String, %struct.String* %self16, i32 0, i32 0, !dbg !75
  %DOT17 = load i8*, i8** %value, align 8, !dbg !75
  %i18 = load i32, i32* %i, align 4, !dbg !75
  %idx = sext i32 %i18 to i64, !dbg !75
  %ADD = getelementptr i8, i8* %DOT17, i64 %idx, !dbg !75
  %needle19 = load i8*, i8** %needle1, align 8, !dbg !75
  %nlen20 = load i32, i32* %nlen, align 4, !dbg !75
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %needle19, i32 %nlen20), !dbg !75
  %EQ21 = icmp eq i32 %strncmp, 0, !dbg !75
  br i1 %EQ21, label %if.then15, label %if.end14, !dbg !75

if.end14:                                         ; preds = %if.start13
  %i23 = load i32, i32* %i, align 4, !dbg !75
  %ADD24 = add i32 %i23, 1, !dbg !75
  store i32 %ADD24, i32* %i, align 4, !dbg !75
  br label %while.start, !dbg !75

if.then15:                                        ; preds = %if.start13
  %i22 = load i32, i32* %i, align 4, !dbg !75
  ret i32 %i22, !dbg !75
}

define i1 @String.contains(i8* %needle, %struct.String* %self) !dbg !76 {
entry:
  %needle1 = alloca i8*, align 8, !dbg !77
  %self2 = alloca %struct.String*, align 8, !dbg !77
  store i8* %needle, i8** %needle1, align 8, !dbg !77
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !77
  %needle3 = load i8*, i8** %needle1, align 8, !dbg !78
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !78
  %String.find = call i32 @String.find(i8* %needle3, %struct.String* %ref_arg), !dbg !78
  %NEQ = icmp ne i32 %String.find, -1, !dbg !78
  ret i1 %NEQ, !dbg !78
}

define i1 @String.starts_with(i8* %prefix, %struct.String* %self) !dbg !79 {
entry:
  %prefix1 = alloca i8*, align 8, !dbg !80
  %self2 = alloca %struct.String*, align 8, !dbg !80
  %plen = alloca i32, align 4, !dbg !80
  store i8* %prefix, i8** %prefix1, align 8, !dbg !80
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !80
  br label %if.start, !dbg !80

if.start:                                         ; preds = %entry
  %prefix3 = load i8*, i8** %prefix1, align 8, !dbg !80
  %EQ = icmp eq i8* %prefix3, null, !dbg !80
  br i1 %EQ, label %if.then, label %if.end, !dbg !80

if.end:                                           ; preds = %if.start
  store i32 0, i32* %plen, align 4, !dbg !80
  %prefix4 = load i8*, i8** %prefix1, align 8, !dbg !81
  %strlen = call i32 @strlen(i8* %prefix4), !dbg !81
  store i32 %strlen, i32* %plen, align 4, !dbg !81
  br label %if.start5, !dbg !81

if.then:                                          ; preds = %if.start
  ret i1 true, !dbg !80

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !81
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !81
  %plen9 = load i32, i32* %plen, align 4, !dbg !81
  %DOT = load i32, i32* %count, align 4, !dbg !81
  %GT = icmp sgt i32 %plen9, %DOT, !dbg !81
  br i1 %GT, label %if.then7, label %if.end6, !dbg !81

if.end6:                                          ; preds = %if.start5
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !82
  %value = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 0, !dbg !82
  %DOT11 = load i8*, i8** %value, align 8, !dbg !82
  %prefix12 = load i8*, i8** %prefix1, align 8, !dbg !82
  %plen13 = load i32, i32* %plen, align 4, !dbg !82
  %strncmp = call i32 @strncmp(i8* %DOT11, i8* %prefix12, i32 %plen13), !dbg !82
  %EQ14 = icmp eq i32 %strncmp, 0, !dbg !82
  ret i1 %EQ14, !dbg !82

if.then7:                                         ; preds = %if.start5
  ret i1 false, !dbg !81
}

define i1 @String.ends_with(i8* %suffix, %struct.String* %self) !dbg !83 {
entry:
  %suffix1 = alloca i8*, align 8, !dbg !84
  %self2 = alloca %struct.String*, align 8, !dbg !84
  %slen = alloca i32, align 4, !dbg !84
  %offset = alloca i32, align 4, !dbg !84
  store i8* %suffix, i8** %suffix1, align 8, !dbg !84
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !84
  br label %if.start, !dbg !84

if.start:                                         ; preds = %entry
  %suffix3 = load i8*, i8** %suffix1, align 8, !dbg !84
  %EQ = icmp eq i8* %suffix3, null, !dbg !84
  br i1 %EQ, label %if.then, label %if.end, !dbg !84

if.end:                                           ; preds = %if.start
  store i32 0, i32* %slen, align 4, !dbg !84
  %suffix4 = load i8*, i8** %suffix1, align 8, !dbg !85
  %strlen = call i32 @strlen(i8* %suffix4), !dbg !85
  store i32 %strlen, i32* %slen, align 4, !dbg !85
  br label %if.start5, !dbg !85

if.then:                                          ; preds = %if.start
  ret i1 true, !dbg !84

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !85
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !85
  %slen9 = load i32, i32* %slen, align 4, !dbg !85
  %DOT = load i32, i32* %count, align 4, !dbg !85
  %GT = icmp sgt i32 %slen9, %DOT, !dbg !85
  br i1 %GT, label %if.then7, label %if.end6, !dbg !85

if.end6:                                          ; preds = %if.start5
  store i32 0, i32* %offset, align 4, !dbg !84
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !84
  %count11 = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 1, !dbg !84
  %DOT12 = load i32, i32* %count11, align 4, !dbg !84
  %slen13 = load i32, i32* %slen, align 4, !dbg !84
  %SUB = sub i32 %DOT12, %slen13, !dbg !84
  store i32 %SUB, i32* %offset, align 4, !dbg !84
  %self14 = load %struct.String*, %struct.String** %self2, align 8, !dbg !86
  %value = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 0, !dbg !86
  %DOT15 = load i8*, i8** %value, align 8, !dbg !86
  %offset16 = load i32, i32* %offset, align 4, !dbg !86
  %idx = sext i32 %offset16 to i64, !dbg !86
  %ADD = getelementptr i8, i8* %DOT15, i64 %idx, !dbg !86
  %suffix17 = load i8*, i8** %suffix1, align 8, !dbg !86
  %slen18 = load i32, i32* %slen, align 4, !dbg !86
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %suffix17, i32 %slen18), !dbg !86
  %EQ19 = icmp eq i32 %strncmp, 0, !dbg !86
  ret i1 %EQ19, !dbg !86

if.then7:                                         ; preds = %if.start5
  ret i1 false, !dbg !85
}

define %struct.String @String.substr(i32 %start, i32 %length, %struct.String* %self) !dbg !87 {
entry:
  %start1 = alloca i32, align 4, !dbg !88
  %length2 = alloca i32, align 4, !dbg !88
  %self3 = alloca %struct.String*, align 8, !dbg !88
  %res = alloca %struct.String, align 8, !dbg !88
  %end = alloca i32, align 4, !dbg !88
  %i = alloca i32, align 4, !dbg !88
  store i32 %start, i32* %start1, align 4, !dbg !88
  store i32 %length, i32* %length2, align 4, !dbg !88
  store %struct.String* %self, %struct.String** %self3, align 8, !dbg !88
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !88
  %0 = call %struct.String @String.new(), !dbg !89
  %tmp_struct = alloca %struct.String, align 8, !dbg !89
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !89
  %tmp_op = alloca %struct.String, align 8, !dbg !89
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !89
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !89
  br label %if.start, !dbg !89

if.start:                                         ; preds = %entry
  %start4 = load i32, i32* %start1, align 4, !dbg !89
  %LT = icmp slt i32 %start4, 0, !dbg !89
  %self5 = load %struct.String*, %struct.String** %self3, align 8, !dbg !89
  %count = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 1, !dbg !89
  %start6 = load i32, i32* %start1, align 4, !dbg !89
  %DOT = load i32, i32* %count, align 4, !dbg !89
  %GE = icmp sge i32 %start6, %DOT, !dbg !89
  %OR = or i1 %LT, %GE, !dbg !89
  br i1 %OR, label %if.then, label %if.end, !dbg !89

if.end:                                           ; preds = %if.start
  store i32 0, i32* %end, align 4, !dbg !88
  %start8 = load i32, i32* %start1, align 4, !dbg !88
  %length9 = load i32, i32* %length2, align 4, !dbg !88
  %ADD = add i32 %start8, %length9, !dbg !88
  store i32 %ADD, i32* %end, align 4, !dbg !88
  br label %if.start10, !dbg !88

if.then:                                          ; preds = %if.start
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !89
  ret %struct.String %res7, !dbg !89

if.start10:                                       ; preds = %if.end
  %self13 = load %struct.String*, %struct.String** %self3, align 8, !dbg !88
  %count14 = getelementptr %struct.String, %struct.String* %self13, i32 0, i32 1, !dbg !88
  %end15 = load i32, i32* %end, align 4, !dbg !88
  %DOT16 = load i32, i32* %count14, align 4, !dbg !88
  %GT = icmp sgt i32 %end15, %DOT16, !dbg !88
  br i1 %GT, label %if.then12, label %if.end11, !dbg !88

if.end11:                                         ; preds = %if.then12, %if.start10
  store i32 0, i32* %i, align 4, !dbg !88
  %start20 = load i32, i32* %start1, align 4, !dbg !88
  store i32 %start20, i32* %i, align 4, !dbg !88
  br label %while.start, !dbg !88

if.then12:                                        ; preds = %if.start10
  %self17 = load %struct.String*, %struct.String** %self3, align 8, !dbg !88
  %count18 = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 1, !dbg !88
  %DOT19 = load i32, i32* %count18, align 4, !dbg !88
  store i32 %DOT19, i32* %end, align 4, !dbg !88
  br label %if.end11, !dbg !88

while.start:                                      ; preds = %while.then, %if.end11
  %i21 = load i32, i32* %i, align 4, !dbg !88
  %end22 = load i32, i32* %end, align 4, !dbg !88
  %LT23 = icmp slt i32 %i21, %end22, !dbg !88
  br i1 %LT23, label %while.then, label %while.end, !dbg !88

while.then:                                       ; preds = %while.start
  %self24 = load %struct.String*, %struct.String** %self3, align 8, !dbg !90
  %value = getelementptr %struct.String, %struct.String* %self24, i32 0, i32 0, !dbg !90
  %DOT25 = load i8*, i8** %value, align 8, !dbg !90
  %i26 = load i32, i32* %i, align 4, !dbg !90
  %ACCESS = getelementptr i8, i8* %DOT25, i32 %i26, !dbg !90
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !90
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !90
  %i27 = load i32, i32* %i, align 4, !dbg !90
  %ADD28 = add i32 %i27, 1, !dbg !90
  store i32 %ADD28, i32* %i, align 4, !dbg !90
  br label %while.start, !dbg !90

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !90
  %res29 = load %struct.String, %struct.String* %res, align 8, !dbg !90
  ret %struct.String %res29, !dbg !90
}

define %struct.String @String.upper(%struct.String* %self) !dbg !91 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !92
  %res = alloca %struct.String, align 8, !dbg !92
  %i = alloca i32, align 4, !dbg !92
  %c = alloca i8, align 1, !dbg !92
  %v = alloca i32, align 4, !dbg !92
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !92
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !92
  %0 = call %struct.String @String.new(), !dbg !93
  %tmp_struct = alloca %struct.String, align 8, !dbg !93
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !93
  %tmp_op = alloca %struct.String, align 8, !dbg !93
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !93
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !93
  store i32 0, i32* %i, align 4, !dbg !92
  store i32 0, i32* %i, align 4, !dbg !92
  br label %while.start, !dbg !92

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !92
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !92
  %i3 = load i32, i32* %i, align 4, !dbg !92
  %DOT = load i32, i32* %count, align 4, !dbg !92
  %LT = icmp slt i32 %i3, %DOT, !dbg !92
  br i1 %LT, label %while.then, label %while.end, !dbg !92

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !92
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !94
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !94
  %DOT5 = load i8*, i8** %value, align 8, !dbg !94
  %i6 = load i32, i32* %i, align 4, !dbg !94
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !94
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !94
  store i8 %ACC, i8* %c, align 1, !dbg !94
  br label %if.start, !dbg !94

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !95
  %res14 = load %struct.String, %struct.String* %res, align 8, !dbg !95
  ret %struct.String %res14, !dbg !95

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !94
  %GE = icmp sge i8 %c7, 97, !dbg !94
  %c8 = load i8, i8* %c, align 1, !dbg !94
  %LE = icmp sle i8 %c8, 122, !dbg !94
  %AND = and i1 %GE, %LE, !dbg !94
  br i1 %AND, label %if.then, label %if.else, !dbg !94

if.end:                                           ; preds = %if.else, %if.then
  %i13 = load i32, i32* %i, align 4, !dbg !95
  %ADD = add i32 %i13, 1, !dbg !95
  store i32 %ADD, i32* %i, align 4, !dbg !95
  br label %while.start, !dbg !95

if.then:                                          ; preds = %if.start
  store i32 0, i32* %v, align 4, !dbg !92
  %c9 = load i8, i8* %c, align 1, !dbg !92
  %as = sext i8 %c9 to i32, !dbg !92
  %SUB = sub i32 %as, 32, !dbg !92
  store i32 %SUB, i32* %v, align 4, !dbg !92
  %v10 = load i32, i32* %v, align 4, !dbg !96
  %as11 = trunc i32 %v10 to i8, !dbg !96
  call void @String.push(i8 %as11, %struct.String* %res), !dbg !96
  br label %if.end, !dbg !96

if.else:                                          ; preds = %if.start
  %c12 = load i8, i8* %c, align 1, !dbg !95
  call void @String.push(i8 %c12, %struct.String* %res), !dbg !95
  br label %if.end, !dbg !95
}

define %struct.String @String.lower(%struct.String* %self) !dbg !97 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !98
  %res = alloca %struct.String, align 8, !dbg !98
  %i = alloca i32, align 4, !dbg !98
  %c = alloca i8, align 1, !dbg !98
  %v = alloca i32, align 4, !dbg !98
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !98
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !98
  %0 = call %struct.String @String.new(), !dbg !99
  %tmp_struct = alloca %struct.String, align 8, !dbg !99
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !99
  %tmp_op = alloca %struct.String, align 8, !dbg !99
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !99
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !99
  store i32 0, i32* %i, align 4, !dbg !98
  store i32 0, i32* %i, align 4, !dbg !98
  br label %while.start, !dbg !98

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !98
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !98
  %i3 = load i32, i32* %i, align 4, !dbg !98
  %DOT = load i32, i32* %count, align 4, !dbg !98
  %LT = icmp slt i32 %i3, %DOT, !dbg !98
  br i1 %LT, label %while.then, label %while.end, !dbg !98

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !98
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !100
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !100
  %DOT5 = load i8*, i8** %value, align 8, !dbg !100
  %i6 = load i32, i32* %i, align 4, !dbg !100
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !100
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !100
  store i8 %ACC, i8* %c, align 1, !dbg !100
  br label %if.start, !dbg !100

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !101
  %res15 = load %struct.String, %struct.String* %res, align 8, !dbg !101
  ret %struct.String %res15, !dbg !101

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !100
  %GE = icmp sge i8 %c7, 65, !dbg !100
  %c8 = load i8, i8* %c, align 1, !dbg !100
  %LE = icmp sle i8 %c8, 90, !dbg !100
  %AND = and i1 %GE, %LE, !dbg !100
  br i1 %AND, label %if.then, label %if.else, !dbg !100

if.end:                                           ; preds = %if.else, %if.then
  %i13 = load i32, i32* %i, align 4, !dbg !101
  %ADD14 = add i32 %i13, 1, !dbg !101
  store i32 %ADD14, i32* %i, align 4, !dbg !101
  br label %while.start, !dbg !101

if.then:                                          ; preds = %if.start
  store i32 0, i32* %v, align 4, !dbg !98
  %c9 = load i8, i8* %c, align 1, !dbg !98
  %as = sext i8 %c9 to i32, !dbg !98
  %ADD = add i32 %as, 32, !dbg !98
  store i32 %ADD, i32* %v, align 4, !dbg !98
  %v10 = load i32, i32* %v, align 4, !dbg !102
  %as11 = trunc i32 %v10 to i8, !dbg !102
  call void @String.push(i8 %as11, %struct.String* %res), !dbg !102
  br label %if.end, !dbg !102

if.else:                                          ; preds = %if.start
  %c12 = load i8, i8* %c, align 1, !dbg !101
  call void @String.push(i8 %c12, %struct.String* %res), !dbg !101
  br label %if.end, !dbg !101
}

define %struct.String @String.trim(%struct.String* %self) !dbg !103 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !104
  %start = alloca i32, align 4, !dbg !104
  %c = alloca i8, align 1, !dbg !104
  %end = alloca i32, align 4, !dbg !104
  %c25 = alloca i8, align 1, !dbg !104
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !104
  store i32 0, i32* %start, align 4, !dbg !104
  store i32 0, i32* %start, align 4, !dbg !104
  br label %while.start, !dbg !104

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !104
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !104
  %start3 = load i32, i32* %start, align 4, !dbg !104
  %DOT = load i32, i32* %count, align 4, !dbg !104
  %LT = icmp slt i32 %start3, %DOT, !dbg !104
  br i1 %LT, label %while.then, label %while.end, !dbg !104

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !104
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !105
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !105
  %DOT5 = load i8*, i8** %value, align 8, !dbg !105
  %start6 = load i32, i32* %start, align 4, !dbg !105
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %start6, !dbg !105
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !105
  store i8 %ACC, i8* %c, align 1, !dbg !105
  br label %if.start, !dbg !105

while.end:                                        ; preds = %if.else, %while.start
  store i32 0, i32* %end, align 4, !dbg !104
  %self17 = load %struct.String*, %struct.String** %self1, align 8, !dbg !104
  %count18 = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 1, !dbg !104
  %DOT19 = load i32, i32* %count18, align 4, !dbg !104
  store i32 %DOT19, i32* %end, align 4, !dbg !104
  br label %while.start20, !dbg !104

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !105
  %EQ = icmp eq i8 %c7, 32, !dbg !105
  %c8 = load i8, i8* %c, align 1, !dbg !105
  %EQ9 = icmp eq i8 %c8, 9, !dbg !105
  %OR = or i1 %EQ, %EQ9, !dbg !105
  %c10 = load i8, i8* %c, align 1, !dbg !105
  %EQ11 = icmp eq i8 %c10, 10, !dbg !105
  %OR12 = or i1 %OR, %EQ11, !dbg !105
  %c13 = load i8, i8* %c, align 1, !dbg !105
  %EQ14 = icmp eq i8 %c13, 13, !dbg !105
  %OR15 = or i1 %OR12, %EQ14, !dbg !105
  br i1 %OR15, label %if.then, label %if.else, !dbg !105

if.end:                                           ; preds = %if.then
  br label %while.start, !dbg !105

if.then:                                          ; preds = %if.start
  %start16 = load i32, i32* %start, align 4, !dbg !105
  %ADD = add i32 %start16, 1, !dbg !105
  store i32 %ADD, i32* %start, align 4, !dbg !105
  br label %if.end, !dbg !105

if.else:                                          ; preds = %if.start
  br label %while.end, !dbg !105

while.start20:                                    ; preds = %if.end33, %while.end
  %end23 = load i32, i32* %end, align 4, !dbg !104
  %start24 = load i32, i32* %start, align 4, !dbg !104
  %GT = icmp sgt i32 %end23, %start24, !dbg !104
  br i1 %GT, label %while.then21, label %while.end22, !dbg !104

while.then21:                                     ; preds = %while.start20
  store i8 0, i8* %c25, align 1, !dbg !104
  %self26 = load %struct.String*, %struct.String** %self1, align 8, !dbg !106
  %value27 = getelementptr %struct.String, %struct.String* %self26, i32 0, i32 0, !dbg !106
  %end28 = load i32, i32* %end, align 4, !dbg !106
  %SUB = sub i32 %end28, 1, !dbg !106
  %DOT29 = load i8*, i8** %value27, align 8, !dbg !106
  %ACCESS30 = getelementptr i8, i8* %DOT29, i32 %SUB, !dbg !106
  %ACC31 = load i8, i8* %ACCESS30, align 1, !dbg !106
  store i8 %ACC31, i8* %c25, align 1, !dbg !106
  br label %if.start32, !dbg !106

while.end22:                                      ; preds = %if.else35, %while.start20
  %start49 = load i32, i32* %start, align 4, !dbg !107
  %end50 = load i32, i32* %end, align 4, !dbg !107
  %start51 = load i32, i32* %start, align 4, !dbg !107
  %SUB52 = sub i32 %end50, %start51, !dbg !107
  %ref_arg = load %struct.String*, %struct.String** %self1, align 8, !dbg !107
  %String.substr = call %struct.String @String.substr(i32 %start49, i32 %SUB52, %struct.String* %ref_arg), !dbg !107
  %tmp_struct = alloca %struct.String, align 8, !dbg !107
  store %struct.String %String.substr, %struct.String* %tmp_struct, align 8, !dbg !107
  ret %struct.String %String.substr, !dbg !107

if.start32:                                       ; preds = %while.then21
  %c36 = load i8, i8* %c25, align 1, !dbg !106
  %EQ37 = icmp eq i8 %c36, 32, !dbg !106
  %c38 = load i8, i8* %c25, align 1, !dbg !106
  %EQ39 = icmp eq i8 %c38, 9, !dbg !106
  %OR40 = or i1 %EQ37, %EQ39, !dbg !106
  %c41 = load i8, i8* %c25, align 1, !dbg !106
  %EQ42 = icmp eq i8 %c41, 10, !dbg !106
  %OR43 = or i1 %OR40, %EQ42, !dbg !106
  %c44 = load i8, i8* %c25, align 1, !dbg !106
  %EQ45 = icmp eq i8 %c44, 13, !dbg !106
  %OR46 = or i1 %OR43, %EQ45, !dbg !106
  br i1 %OR46, label %if.then34, label %if.else35, !dbg !106

if.end33:                                         ; preds = %if.then34
  br label %while.start20, !dbg !106

if.then34:                                        ; preds = %if.start32
  %end47 = load i32, i32* %end, align 4, !dbg !106
  %SUB48 = sub i32 %end47, 1, !dbg !106
  store i32 %SUB48, i32* %end, align 4, !dbg !106
  br label %if.end33, !dbg !106

if.else35:                                        ; preds = %if.start32
  br label %while.end22, !dbg !106
}

define %struct.String @String.replace(i8* %old, i8* %replacement, %struct.String* %self) !dbg !108 {
entry:
  %old1 = alloca i8*, align 8, !dbg !109
  %replacement2 = alloca i8*, align 8, !dbg !109
  %self3 = alloca %struct.String*, align 8, !dbg !109
  %res = alloca %struct.String, align 8, !dbg !109
  %olen = alloca i32, align 4, !dbg !109
  %i = alloca i32, align 4, !dbg !109
  store i8* %old, i8** %old1, align 8, !dbg !109
  store i8* %replacement, i8** %replacement2, align 8, !dbg !109
  store %struct.String* %self, %struct.String** %self3, align 8, !dbg !109
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !109
  %0 = call %struct.String @String.new(), !dbg !110
  %tmp_struct = alloca %struct.String, align 8, !dbg !110
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !110
  %tmp_op = alloca %struct.String, align 8, !dbg !110
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !110
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !110
  store i32 0, i32* %olen, align 4, !dbg !109
  %old4 = load i8*, i8** %old1, align 8, !dbg !111
  %strlen = call i32 @strlen(i8* %old4), !dbg !111
  store i32 %strlen, i32* %olen, align 4, !dbg !111
  br label %if.start, !dbg !111

if.start:                                         ; preds = %entry
  %olen5 = load i32, i32* %olen, align 4, !dbg !111
  %EQ = icmp eq i32 %olen5, 0, !dbg !111
  br i1 %EQ, label %if.then, label %if.end, !dbg !111

if.end:                                           ; preds = %if.start
  store i32 0, i32* %i, align 4, !dbg !109
  store i32 0, i32* %i, align 4, !dbg !109
  br label %while.start, !dbg !109

if.then:                                          ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self3, align 8, !dbg !112
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !112
  %DOT = load i8*, i8** %value, align 8, !dbg !112
  call void @String.assign(i8* %DOT, %struct.String* %res), !dbg !112
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !112
  ret %struct.String %res7, !dbg !112

while.start:                                      ; preds = %if.end12, %if.end
  %self8 = load %struct.String*, %struct.String** %self3, align 8, !dbg !109
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !109
  %i9 = load i32, i32* %i, align 4, !dbg !109
  %DOT10 = load i32, i32* %count, align 4, !dbg !109
  %LT = icmp slt i32 %i9, %DOT10, !dbg !109
  br i1 %LT, label %while.then, label %while.end, !dbg !109

while.then:                                       ; preds = %while.start
  br label %if.start11, !dbg !109

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !113
  %res36 = load %struct.String, %struct.String* %res, align 8, !dbg !113
  ret %struct.String %res36, !dbg !113

if.start11:                                       ; preds = %while.then
  %self14 = load %struct.String*, %struct.String** %self3, align 8, !dbg !109
  %count15 = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 1, !dbg !109
  %DOT16 = load i32, i32* %count15, align 4, !dbg !109
  %olen17 = load i32, i32* %olen, align 4, !dbg !109
  %SUB = sub i32 %DOT16, %olen17, !dbg !109
  %i18 = load i32, i32* %i, align 4, !dbg !109
  %LE = icmp sle i32 %i18, %SUB, !dbg !109
  %self19 = load %struct.String*, %struct.String** %self3, align 8, !dbg !114
  %value20 = getelementptr %struct.String, %struct.String* %self19, i32 0, i32 0, !dbg !114
  %DOT21 = load i8*, i8** %value20, align 8, !dbg !114
  %i22 = load i32, i32* %i, align 4, !dbg !114
  %idx = sext i32 %i22 to i64, !dbg !114
  %ADD = getelementptr i8, i8* %DOT21, i64 %idx, !dbg !114
  %old23 = load i8*, i8** %old1, align 8, !dbg !114
  %olen24 = load i32, i32* %olen, align 4, !dbg !114
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %old23, i32 %olen24), !dbg !114
  %EQ25 = icmp eq i32 %strncmp, 0, !dbg !114
  %AND = and i1 %LE, %EQ25, !dbg !114
  br i1 %AND, label %if.then13, label %if.else, !dbg !114

if.end12:                                         ; preds = %if.else, %if.then13
  br label %while.start, !dbg !113

if.then13:                                        ; preds = %if.start11
  %replacement26 = load i8*, i8** %replacement2, align 8, !dbg !115
  call void @String.join(i8* %replacement26, %struct.String* %res), !dbg !115
  %i27 = load i32, i32* %i, align 4, !dbg !115
  %olen28 = load i32, i32* %olen, align 4, !dbg !115
  %ADD29 = add i32 %i27, %olen28, !dbg !115
  store i32 %ADD29, i32* %i, align 4, !dbg !115
  br label %if.end12, !dbg !115

if.else:                                          ; preds = %if.start11
  %self30 = load %struct.String*, %struct.String** %self3, align 8, !dbg !113
  %value31 = getelementptr %struct.String, %struct.String* %self30, i32 0, i32 0, !dbg !113
  %DOT32 = load i8*, i8** %value31, align 8, !dbg !113
  %i33 = load i32, i32* %i, align 4, !dbg !113
  %ACCESS = getelementptr i8, i8* %DOT32, i32 %i33, !dbg !113
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !113
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !113
  %i34 = load i32, i32* %i, align 4, !dbg !113
  %ADD35 = add i32 %i34, 1, !dbg !113
  store i32 %ADD35, i32* %i, align 4, !dbg !113
  br label %if.end12, !dbg !113
}

define %struct.String @String.repeat(i32 %n, %struct.String* %self) !dbg !116 {
entry:
  %n1 = alloca i32, align 4, !dbg !117
  %self2 = alloca %struct.String*, align 8, !dbg !117
  %res = alloca %struct.String, align 8, !dbg !117
  %i = alloca i32, align 4, !dbg !117
  store i32 %n, i32* %n1, align 4, !dbg !117
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !117
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !117
  %0 = call %struct.String @String.new(), !dbg !118
  %tmp_struct = alloca %struct.String, align 8, !dbg !118
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !118
  %tmp_op = alloca %struct.String, align 8, !dbg !118
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !118
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !118
  store i32 0, i32* %i, align 4, !dbg !117
  store i32 0, i32* %i, align 4, !dbg !117
  br label %while.start, !dbg !117

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !117
  %n4 = load i32, i32* %n1, align 4, !dbg !117
  %LT = icmp slt i32 %i3, %n4, !dbg !117
  br i1 %LT, label %while.then, label %while.end, !dbg !117

while.then:                                       ; preds = %while.start
  %self5 = load %struct.String*, %struct.String** %self2, align 8, !dbg !119
  %value = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 0, !dbg !119
  %DOT = load i8*, i8** %value, align 8, !dbg !119
  call void @String.join(i8* %DOT, %struct.String* %res), !dbg !119
  %i6 = load i32, i32* %i, align 4, !dbg !119
  %ADD = add i32 %i6, 1, !dbg !119
  store i32 %ADD, i32* %i, align 4, !dbg !119
  br label %while.start, !dbg !119

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !119
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !119
  ret %struct.String %res7, !dbg !119
}

define %struct.String @String.reverse(%struct.String* %self) !dbg !120 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !121
  %res = alloca %struct.String, align 8, !dbg !121
  %i = alloca i32, align 4, !dbg !121
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !121
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !121
  %0 = call %struct.String @String.new(), !dbg !122
  %tmp_struct = alloca %struct.String, align 8, !dbg !122
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !122
  %tmp_op = alloca %struct.String, align 8, !dbg !122
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !122
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !122
  store i32 0, i32* %i, align 4, !dbg !121
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !121
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !121
  %DOT = load i32, i32* %count, align 4, !dbg !121
  %SUB = sub i32 %DOT, 1, !dbg !121
  store i32 %SUB, i32* %i, align 4, !dbg !121
  br label %while.start, !dbg !121

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !121
  %GE = icmp sge i32 %i3, 0, !dbg !121
  br i1 %GE, label %while.then, label %while.end, !dbg !121

while.then:                                       ; preds = %while.start
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !123
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !123
  %DOT5 = load i8*, i8** %value, align 8, !dbg !123
  %i6 = load i32, i32* %i, align 4, !dbg !123
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !123
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !123
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !123
  %i7 = load i32, i32* %i, align 4, !dbg !123
  %SUB8 = sub i32 %i7, 1, !dbg !123
  store i32 %SUB8, i32* %i, align 4, !dbg !123
  br label %while.start, !dbg !123

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !123
  %res9 = load %struct.String, %struct.String* %res, align 8, !dbg !123
  ret %struct.String %res9, !dbg !123
}

define i32 @String.compare(%struct.String* %other, %struct.String* %self) !dbg !124 {
entry:
  %other1 = alloca %struct.String*, align 8, !dbg !125
  %self2 = alloca %struct.String*, align 8, !dbg !125
  store %struct.String* %other, %struct.String** %other1, align 8, !dbg !125
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !125
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !126
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !126
  %DOT = load i8*, i8** %value, align 8, !dbg !126
  %other4 = load %struct.String*, %struct.String** %other1, align 8, !dbg !126
  %value5 = getelementptr %struct.String, %struct.String* %other4, i32 0, i32 0, !dbg !126
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !126
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !126
  ret i32 %strcmp, !dbg !126
}

define i32 @String.to_int(%struct.String* %self) !dbg !127 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !128
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !128
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !129
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !129
  %DOT = load i8*, i8** %value, align 8, !dbg !129
  %atoi = call i32 @atoi(i8* %DOT), !dbg !129
  ret i32 %atoi, !dbg !129
}

define void @String.operator.ASSIGN.CHARS(i8* %str, %struct.String* %self) !dbg !130 {
entry:
  %str1 = alloca i8*, align 8, !dbg !131
  %self2 = alloca %struct.String*, align 8, !dbg !131
  store i8* %str, i8** %str1, align 8, !dbg !131
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !131
  %str3 = load i8*, i8** %str1, align 8, !dbg !132
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !132
  call void @String.assign(i8* %str3, %struct.String* %ref_arg), !dbg !132
  ret void, !dbg !132
}

define void @String.operator.ASSIGN.String(%struct.String* %v, %struct.String* %self) !dbg !133 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !134
  %self2 = alloca %struct.String*, align 8, !dbg !134
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !134
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !134
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !135
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !135
  %DOT = load i8*, i8** %value, align 8, !dbg !135
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !135
  call void @String.assign(i8* %DOT, %struct.String* %ref_arg), !dbg !135
  ret void, !dbg !135
}

define %struct.String @String.operator.ADD.String(%struct.String* %v, %struct.String* %self) !dbg !136 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !137
  %self2 = alloca %struct.String*, align 8, !dbg !137
  %res = alloca %struct.String, align 8, !dbg !137
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !137
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !137
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !137
  %0 = call %struct.String @String.new(), !dbg !138
  %tmp_struct = alloca %struct.String, align 8, !dbg !138
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !138
  %tmp_op = alloca %struct.String, align 8, !dbg !138
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !138
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !138
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !139
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !139
  %DOT = load i8*, i8** %value, align 8, !dbg !139
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !139
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !139
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !139
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %DOT6), !dbg !139
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !139
  call void @String.delete(%struct.String* %tmp_struct), !dbg !139
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !139
  ret %struct.String %res7, !dbg !139
}

define %struct.String @String.operator.ADD.CHARS(i8* %str, %struct.String* %self) !dbg !140 {
entry:
  %str1 = alloca i8*, align 8, !dbg !141
  %self2 = alloca %struct.String*, align 8, !dbg !141
  %res = alloca %struct.String, align 8, !dbg !141
  store i8* %str, i8** %str1, align 8, !dbg !141
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !141
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !141
  %0 = call %struct.String @String.new(), !dbg !142
  %tmp_struct = alloca %struct.String, align 8, !dbg !142
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !142
  %tmp_op = alloca %struct.String, align 8, !dbg !142
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !142
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !142
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !143
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !143
  %DOT = load i8*, i8** %value, align 8, !dbg !143
  %str4 = load i8*, i8** %str1, align 8, !dbg !143
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str4), !dbg !143
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !143
  call void @String.delete(%struct.String* %tmp_struct), !dbg !143
  %res5 = load %struct.String, %struct.String* %res, align 8, !dbg !143
  ret %struct.String %res5, !dbg !143
}

define void @String.operator.ADD_ASS.String(%struct.String* %v, %struct.String* %self) !dbg !144 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !145
  %self2 = alloca %struct.String*, align 8, !dbg !145
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !145
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !145
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !146
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !146
  %DOT = load i8*, i8** %value, align 8, !dbg !146
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !146
  call void @String.join(i8* %DOT, %struct.String* %ref_arg), !dbg !146
  ret void, !dbg !146
}

define void @String.operator.ADD_ASS.CHARS(i8* %str, %struct.String* %self) !dbg !147 {
entry:
  %str1 = alloca i8*, align 8, !dbg !148
  %self2 = alloca %struct.String*, align 8, !dbg !148
  store i8* %str, i8** %str1, align 8, !dbg !148
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !148
  %str3 = load i8*, i8** %str1, align 8, !dbg !149
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !149
  call void @String.join(i8* %str3, %struct.String* %ref_arg), !dbg !149
  ret void, !dbg !149
}

define i1 @String.operator.EQ.String(%struct.String* %v, %struct.String* %self) !dbg !150 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !151
  %self2 = alloca %struct.String*, align 8, !dbg !151
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !151
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !151
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !152
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !152
  %DOT = load i8*, i8** %value, align 8, !dbg !152
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !152
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !152
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !152
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !152
  %EQ = icmp eq i32 %strcmp, 0, !dbg !152
  ret i1 %EQ, !dbg !152
}

define i1 @String.operator.EQ.CHARS(i8* %str, %struct.String* %self) !dbg !153 {
entry:
  %str1 = alloca i8*, align 8, !dbg !154
  %self2 = alloca %struct.String*, align 8, !dbg !154
  store i8* %str, i8** %str1, align 8, !dbg !154
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !154
  br label %if.start, !dbg !154

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !154
  %EQ = icmp eq i8* %str3, null, !dbg !154
  br i1 %EQ, label %if.then, label %if.end, !dbg !154

if.end:                                           ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self2, align 8, !dbg !155
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !155
  %DOT7 = load i8*, i8** %value, align 8, !dbg !155
  %str8 = load i8*, i8** %str1, align 8, !dbg !155
  %strcmp = call i32 @strcmp(i8* %DOT7, i8* %str8), !dbg !155
  %EQ9 = icmp eq i32 %strcmp, 0, !dbg !155
  ret i1 %EQ9, !dbg !155

if.then:                                          ; preds = %if.start
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !154
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !154
  %DOT = load i32, i32* %count, align 4, !dbg !154
  %EQ5 = icmp eq i32 %DOT, 0, !dbg !154
  ret i1 %EQ5, !dbg !154
}

define i1 @String.operator.NEQ.String(%struct.String* %v, %struct.String* %self) !dbg !156 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !157
  %self2 = alloca %struct.String*, align 8, !dbg !157
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !157
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !157
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !158
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !158
  %DOT = load i8*, i8** %value, align 8, !dbg !158
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !158
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !158
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !158
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !158
  %NEQ = icmp ne i32 %strcmp, 0, !dbg !158
  ret i1 %NEQ, !dbg !158
}

define i1 @String.operator.NEQ.CHARS(i8* %str, %struct.String* %self) !dbg !159 {
entry:
  %str1 = alloca i8*, align 8, !dbg !160
  %self2 = alloca %struct.String*, align 8, !dbg !160
  store i8* %str, i8** %str1, align 8, !dbg !160
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !160
  br label %if.start, !dbg !160

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !160
  %EQ = icmp eq i8* %str3, null, !dbg !160
  br i1 %EQ, label %if.then, label %if.end, !dbg !160

if.end:                                           ; preds = %if.start
  %self5 = load %struct.String*, %struct.String** %self2, align 8, !dbg !161
  %value = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 0, !dbg !161
  %DOT6 = load i8*, i8** %value, align 8, !dbg !161
  %str7 = load i8*, i8** %str1, align 8, !dbg !161
  %strcmp = call i32 @strcmp(i8* %DOT6, i8* %str7), !dbg !161
  %NEQ8 = icmp ne i32 %strcmp, 0, !dbg !161
  ret i1 %NEQ8, !dbg !161

if.then:                                          ; preds = %if.start
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !160
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !160
  %DOT = load i32, i32* %count, align 4, !dbg !160
  %NEQ = icmp ne i32 %DOT, 0, !dbg !160
  ret i1 %NEQ, !dbg !160
}

define i1 @String.operator.LT.String(%struct.String* %v, %struct.String* %self) !dbg !162 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !163
  %self2 = alloca %struct.String*, align 8, !dbg !163
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !163
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !163
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !164
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !164
  %DOT = load i8*, i8** %value, align 8, !dbg !164
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !164
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !164
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !164
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !164
  %LT = icmp slt i32 %strcmp, 0, !dbg !164
  ret i1 %LT, !dbg !164
}

define i1 @String.operator.GT.String(%struct.String* %v, %struct.String* %self) !dbg !165 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !166
  %self2 = alloca %struct.String*, align 8, !dbg !166
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !166
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !166
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !167
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !167
  %DOT = load i8*, i8** %value, align 8, !dbg !167
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !167
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !167
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !167
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !167
  %GT = icmp sgt i32 %strcmp, 0, !dbg !167
  ret i1 %GT, !dbg !167
}

define i8* @String.operator.output(%struct.String* %self) !dbg !168 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !169
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !169
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !169
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !169
  %DOT = load i8*, i8** %value, align 8, !dbg !169
  ret i8* %DOT, !dbg !169
}

declare i32 @strlen(i8*)

declare i8* @strcpy(i8*, i8*)

declare void @bzero(i8*, i64)

define i8* @strjoin(i8* %left, i8* %right) !dbg !170 {
entry:
  %left1 = alloca i8*, align 8, !dbg !171
  %right2 = alloca i8*, align 8, !dbg !171
  %len1 = alloca i32, align 4, !dbg !171
  %len2 = alloca i32, align 4, !dbg !171
  %str = alloca i8*, align 8, !dbg !171
  %i = alloca i32, align 4, !dbg !171
  %j = alloca i32, align 4, !dbg !171
  store i8* %left, i8** %left1, align 8, !dbg !171
  store i8* %right, i8** %right2, align 8, !dbg !171
  store i32 0, i32* %len1, align 4, !dbg !171
  store i32 0, i32* %len1, align 4, !dbg !171
  store i32 0, i32* %len2, align 4, !dbg !171
  store i32 0, i32* %len2, align 4, !dbg !171
  br label %if.start, !dbg !171

if.start:                                         ; preds = %entry
  %left3 = load i8*, i8** %left1, align 8, !dbg !171
  %NEQ = icmp ne i8* %left3, null, !dbg !171
  br i1 %NEQ, label %if.then, label %if.end, !dbg !171

if.end:                                           ; preds = %if.then, %if.start
  br label %if.start5, !dbg !172

if.then:                                          ; preds = %if.start
  %left4 = load i8*, i8** %left1, align 8, !dbg !172
  %strlen = call i32 @strlen(i8* %left4), !dbg !172
  store i32 %strlen, i32* %len1, align 4, !dbg !172
  br label %if.end, !dbg !172

if.start5:                                        ; preds = %if.end
  %right8 = load i8*, i8** %right2, align 8, !dbg !172
  %NEQ9 = icmp ne i8* %right8, null, !dbg !172
  br i1 %NEQ9, label %if.then7, label %if.end6, !dbg !172

if.end6:                                          ; preds = %if.then7, %if.start5
  store i8* null, i8** %str, align 8, !dbg !171
  %len112 = load i32, i32* %len1, align 4, !dbg !173
  %len213 = load i32, i32* %len2, align 4, !dbg !173
  %ADD = add i32 %len112, %len213, !dbg !173
  %ADD14 = add i32 %ADD, 1, !dbg !173
  %as = sext i32 %ADD14 to i64, !dbg !173
  %calloc = call i8* @calloc(i64 %as, i64 1), !dbg !173
  store i8* %calloc, i8** %str, align 8, !dbg !173
  store i32 0, i32* %i, align 4, !dbg !171
  store i32 0, i32* %i, align 4, !dbg !171
  br label %while.start, !dbg !171

if.then7:                                         ; preds = %if.start5
  %right10 = load i8*, i8** %right2, align 8, !dbg !174
  %strlen11 = call i32 @strlen(i8* %right10), !dbg !174
  store i32 %strlen11, i32* %len2, align 4, !dbg !174
  br label %if.end6, !dbg !174

while.start:                                      ; preds = %while.then, %if.end6
  %i15 = load i32, i32* %i, align 4, !dbg !171
  %len116 = load i32, i32* %len1, align 4, !dbg !171
  %LT = icmp slt i32 %i15, %len116, !dbg !171
  br i1 %LT, label %while.then, label %while.end, !dbg !171

while.then:                                       ; preds = %while.start
  %str17 = load i8*, i8** %str, align 8, !dbg !175
  %i18 = load i32, i32* %i, align 4, !dbg !175
  %ACCESS = getelementptr i8, i8* %str17, i32 %i18, !dbg !175
  %left19 = load i8*, i8** %left1, align 8, !dbg !175
  %i20 = load i32, i32* %i, align 4, !dbg !175
  %ACCESS21 = getelementptr i8, i8* %left19, i32 %i20, !dbg !175
  %ACC = load i8, i8* %ACCESS21, align 1, !dbg !175
  store i8 %ACC, i8* %ACCESS, align 1, !dbg !175
  %i22 = load i32, i32* %i, align 4, !dbg !175
  %ADD23 = add i32 %i22, 1, !dbg !175
  store i32 %ADD23, i32* %i, align 4, !dbg !175
  br label %while.start, !dbg !175

while.end:                                        ; preds = %while.start
  store i32 0, i32* %j, align 4, !dbg !171
  store i32 0, i32* %j, align 4, !dbg !171
  br label %while.start24, !dbg !171

while.start24:                                    ; preds = %while.then25, %while.end
  %j27 = load i32, i32* %j, align 4, !dbg !171
  %len228 = load i32, i32* %len2, align 4, !dbg !171
  %LT29 = icmp slt i32 %j27, %len228, !dbg !171
  br i1 %LT29, label %while.then25, label %while.end26, !dbg !171

while.then25:                                     ; preds = %while.start24
  %i30 = load i32, i32* %i, align 4, !dbg !176
  %j31 = load i32, i32* %j, align 4, !dbg !176
  %ADD32 = add i32 %i30, %j31, !dbg !176
  %str33 = load i8*, i8** %str, align 8, !dbg !176
  %ACCESS34 = getelementptr i8, i8* %str33, i32 %ADD32, !dbg !176
  %right35 = load i8*, i8** %right2, align 8, !dbg !176
  %j36 = load i32, i32* %j, align 4, !dbg !176
  %ACCESS37 = getelementptr i8, i8* %right35, i32 %j36, !dbg !176
  %ACC38 = load i8, i8* %ACCESS37, align 1, !dbg !176
  store i8 %ACC38, i8* %ACCESS34, align 1, !dbg !176
  %j39 = load i32, i32* %j, align 4, !dbg !176
  %ADD40 = add i32 %j39, 1, !dbg !176
  store i32 %ADD40, i32* %j, align 4, !dbg !176
  br label %while.start24, !dbg !176

while.end26:                                      ; preds = %while.start24
  %str41 = load i8*, i8** %str, align 8, !dbg !176
  ret i8* %str41, !dbg !176
}

declare i8* @calloc(i64, i64)

declare i32 @strncmp(i8*, i8*, i32)

declare i32 @strcmp(i8*, i8*)

declare i32 @atoi(i8*)

define void @print_doubled(i32 %n) !dbg !177 {
entry:
  %n1 = alloca i32, align 4, !dbg !178
  store i32 %n, i32* %n1, align 4, !dbg !178
  %n2 = load i32, i32* %n1, align 4, !dbg !178
  %MUL = mul i32 %n2, 2, !dbg !178
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 %MUL), !dbg !178
  ret void, !dbg !178
}

define i32 @main() !dbg !179 {
entry:
  %items = alloca %struct.__list_int, align 8, !dbg !180
  store %struct.__list_int zeroinitializer, %struct.__list_int* %items, align 8, !dbg !180
  call void @__list_int.push(i32 1, %struct.__list_int* %items), !dbg !181
  call void @__list_int.push(i32 2, %struct.__list_int* %items), !dbg !182
  call void @__list_int.push(i32 3, %struct.__list_int* %items), !dbg !183
  call void @__list_int.foreach(void (i32)* @print_doubled, %struct.__list_int* %items), !dbg !184
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @output_fmt.1, i32 0, i32 0)), !dbg !184
  call void @__list_int.delete(%struct.__list_int* %items), !dbg !184
  ret i32 0, !dbg !184
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "006.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/list")
!4 = distinct !DISubprogram(name: "__list_int.delete", linkageName: "__list_int.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 0, scope: !4)
!8 = !DILocation(line: 9, scope: !4)
!9 = distinct !DISubprogram(name: "__list_int.push", linkageName: "__list_int.push", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 11, scope: !9)
!11 = !DILocation(line: 16, scope: !9)
!12 = !DILocation(line: 15, scope: !9)
!13 = distinct !DISubprogram(name: "__list_int.pop", linkageName: "__list_int.pop", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 19, scope: !13)
!15 = !DILocation(line: 21, scope: !13)
!16 = distinct !DISubprogram(name: "__list_int.len", linkageName: "__list_int.len", scope: null, file: !3, line: 23, type: !5, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 23, scope: !16)
!18 = distinct !DISubprogram(name: "__list_int.cap", linkageName: "__list_int.cap", scope: null, file: !3, line: 26, type: !5, scopeLine: 26, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 26, scope: !18)
!20 = distinct !DISubprogram(name: "__list_int.foreach", linkageName: "__list_int.foreach", scope: null, file: !3, line: 29, type: !5, scopeLine: 29, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 29, scope: !20)
!22 = !DILocation(line: 32, scope: !20)
!23 = distinct !DISubprogram(name: "String.delete", linkageName: "String.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!24 = !DILocation(line: 0, scope: !23)
!25 = !DILocation(line: 82, scope: !23)
!26 = distinct !DISubprogram(name: "String.assign", linkageName: "String.assign", scope: null, file: !3, line: 86, type: !5, scopeLine: 86, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!27 = !DILocation(line: 86, scope: !26)
!28 = !DILocation(line: 88, scope: !26)
!29 = !DILocation(line: 95, scope: !26)
!30 = !DILocation(line: 96, scope: !26)
!31 = !DILocation(line: 94, scope: !26)
!32 = distinct !DISubprogram(name: "String.join", linkageName: "String.join", scope: null, file: !3, line: 99, type: !5, scopeLine: 99, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!33 = !DILocation(line: 99, scope: !32)
!34 = !DILocation(line: 101, scope: !32)
!35 = !DILocation(line: 102, scope: !32)
!36 = !DILocation(line: 103, scope: !32)
!37 = distinct !DISubprogram(name: "String.clear", linkageName: "String.clear", scope: null, file: !3, line: 105, type: !5, scopeLine: 105, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!38 = !DILocation(line: 105, scope: !37)
!39 = !DILocation(line: 106, scope: !37)
!40 = distinct !DISubprogram(name: "String.push", linkageName: "String.push", scope: null, file: !3, line: 109, type: !5, scopeLine: 109, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!41 = !DILocation(line: 109, scope: !40)
!42 = !DILocation(line: 116, scope: !40)
!43 = !DILocation(line: 118, scope: !40)
!44 = !DILocation(line: 115, scope: !40)
!45 = distinct !DISubprogram(name: "String.pop", linkageName: "String.pop", scope: null, file: !3, line: 120, type: !5, scopeLine: 120, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!46 = !DILocation(line: 120, scope: !45)
!47 = !DILocation(line: 124, scope: !45)
!48 = !DILocation(line: 125, scope: !45)
!49 = distinct !DISubprogram(name: "String.new", linkageName: "String.new", scope: null, file: !3, line: 130, type: !5, scopeLine: 130, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!50 = !DILocation(line: 130, scope: !49)
!51 = !DILocation(line: 133, scope: !49)
!52 = distinct !DISubprogram(name: "String.from", linkageName: "String.from", scope: null, file: !3, line: 136, type: !5, scopeLine: 136, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!53 = !DILocation(line: 136, scope: !52)
!54 = !DILocation(line: 137, scope: !52)
!55 = !DILocation(line: 138, scope: !52)
!56 = distinct !DISubprogram(name: "String.from_int", linkageName: "String.from_int", scope: null, file: !3, line: 141, type: !5, scopeLine: 141, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!57 = !DILocation(line: 141, scope: !56)
!58 = !DILocation(line: 142, scope: !56)
!59 = !DILocation(line: 144, scope: !56)
!60 = !DILocation(line: 154, scope: !56)
!61 = !DILocation(line: 158, scope: !56)
!62 = !DILocation(line: 161, scope: !56)
!63 = distinct !DISubprogram(name: "String.len", linkageName: "String.len", scope: null, file: !3, line: 167, type: !5, scopeLine: 167, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!64 = !DILocation(line: 167, scope: !63)
!65 = distinct !DISubprogram(name: "String.empty", linkageName: "String.empty", scope: null, file: !3, line: 170, type: !5, scopeLine: 170, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!66 = !DILocation(line: 170, scope: !65)
!67 = distinct !DISubprogram(name: "String.c_str", linkageName: "String.c_str", scope: null, file: !3, line: 173, type: !5, scopeLine: 173, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!68 = !DILocation(line: 173, scope: !67)
!69 = distinct !DISubprogram(name: "String.at", linkageName: "String.at", scope: null, file: !3, line: 176, type: !5, scopeLine: 176, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!70 = !DILocation(line: 176, scope: !69)
!71 = !DILocation(line: 179, scope: !69)
!72 = distinct !DISubprogram(name: "String.find", linkageName: "String.find", scope: null, file: !3, line: 183, type: !5, scopeLine: 183, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!73 = !DILocation(line: 183, scope: !72)
!74 = !DILocation(line: 186, scope: !72)
!75 = !DILocation(line: 189, scope: !72)
!76 = distinct !DISubprogram(name: "String.contains", linkageName: "String.contains", scope: null, file: !3, line: 194, type: !5, scopeLine: 194, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!77 = !DILocation(line: 194, scope: !76)
!78 = !DILocation(line: 195, scope: !76)
!79 = distinct !DISubprogram(name: "String.starts_with", linkageName: "String.starts_with", scope: null, file: !3, line: 197, type: !5, scopeLine: 197, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!80 = !DILocation(line: 197, scope: !79)
!81 = !DILocation(line: 199, scope: !79)
!82 = !DILocation(line: 201, scope: !79)
!83 = distinct !DISubprogram(name: "String.ends_with", linkageName: "String.ends_with", scope: null, file: !3, line: 203, type: !5, scopeLine: 203, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!84 = !DILocation(line: 203, scope: !83)
!85 = !DILocation(line: 205, scope: !83)
!86 = !DILocation(line: 208, scope: !83)
!87 = distinct !DISubprogram(name: "String.substr", linkageName: "String.substr", scope: null, file: !3, line: 212, type: !5, scopeLine: 212, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!88 = !DILocation(line: 212, scope: !87)
!89 = !DILocation(line: 213, scope: !87)
!90 = !DILocation(line: 221, scope: !87)
!91 = distinct !DISubprogram(name: "String.upper", linkageName: "String.upper", scope: null, file: !3, line: 225, type: !5, scopeLine: 225, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!92 = !DILocation(line: 225, scope: !91)
!93 = !DILocation(line: 226, scope: !91)
!94 = !DILocation(line: 229, scope: !91)
!95 = !DILocation(line: 234, scope: !91)
!96 = !DILocation(line: 232, scope: !91)
!97 = distinct !DISubprogram(name: "String.lower", linkageName: "String.lower", scope: null, file: !3, line: 238, type: !5, scopeLine: 238, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!98 = !DILocation(line: 238, scope: !97)
!99 = !DILocation(line: 239, scope: !97)
!100 = !DILocation(line: 242, scope: !97)
!101 = !DILocation(line: 247, scope: !97)
!102 = !DILocation(line: 245, scope: !97)
!103 = distinct !DISubprogram(name: "String.trim", linkageName: "String.trim", scope: null, file: !3, line: 251, type: !5, scopeLine: 251, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!104 = !DILocation(line: 251, scope: !103)
!105 = !DILocation(line: 254, scope: !103)
!106 = !DILocation(line: 261, scope: !103)
!107 = !DILocation(line: 266, scope: !103)
!108 = distinct !DISubprogram(name: "String.replace", linkageName: "String.replace", scope: null, file: !3, line: 268, type: !5, scopeLine: 268, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!109 = !DILocation(line: 268, scope: !108)
!110 = !DILocation(line: 269, scope: !108)
!111 = !DILocation(line: 270, scope: !108)
!112 = !DILocation(line: 272, scope: !108)
!113 = !DILocation(line: 280, scope: !108)
!114 = !DILocation(line: 276, scope: !108)
!115 = !DILocation(line: 277, scope: !108)
!116 = distinct !DISubprogram(name: "String.repeat", linkageName: "String.repeat", scope: null, file: !3, line: 284, type: !5, scopeLine: 284, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!117 = !DILocation(line: 284, scope: !116)
!118 = !DILocation(line: 285, scope: !116)
!119 = !DILocation(line: 288, scope: !116)
!120 = distinct !DISubprogram(name: "String.reverse", linkageName: "String.reverse", scope: null, file: !3, line: 292, type: !5, scopeLine: 292, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!121 = !DILocation(line: 292, scope: !120)
!122 = !DILocation(line: 293, scope: !120)
!123 = !DILocation(line: 296, scope: !120)
!124 = distinct !DISubprogram(name: "String.compare", linkageName: "String.compare", scope: null, file: !3, line: 302, type: !5, scopeLine: 302, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!125 = !DILocation(line: 302, scope: !124)
!126 = !DILocation(line: 303, scope: !124)
!127 = distinct !DISubprogram(name: "String.to_int", linkageName: "String.to_int", scope: null, file: !3, line: 307, type: !5, scopeLine: 307, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!128 = !DILocation(line: 307, scope: !127)
!129 = !DILocation(line: 308, scope: !127)
!130 = distinct !DISubprogram(name: "String.operator.ASSIGN.CHARS", linkageName: "String.operator.ASSIGN.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!131 = !DILocation(line: 0, scope: !130)
!132 = !DILocation(line: 313, scope: !130)
!133 = distinct !DISubprogram(name: "String.operator.ASSIGN.String", linkageName: "String.operator.ASSIGN.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!134 = !DILocation(line: 0, scope: !133)
!135 = !DILocation(line: 316, scope: !133)
!136 = distinct !DISubprogram(name: "String.operator.ADD.String", linkageName: "String.operator.ADD.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!137 = !DILocation(line: 0, scope: !136)
!138 = !DILocation(line: 321, scope: !136)
!139 = !DILocation(line: 322, scope: !136)
!140 = distinct !DISubprogram(name: "String.operator.ADD.CHARS", linkageName: "String.operator.ADD.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!141 = !DILocation(line: 0, scope: !140)
!142 = !DILocation(line: 326, scope: !140)
!143 = !DILocation(line: 327, scope: !140)
!144 = distinct !DISubprogram(name: "String.operator.ADD_ASS.String", linkageName: "String.operator.ADD_ASS.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!145 = !DILocation(line: 0, scope: !144)
!146 = !DILocation(line: 331, scope: !144)
!147 = distinct !DISubprogram(name: "String.operator.ADD_ASS.CHARS", linkageName: "String.operator.ADD_ASS.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!148 = !DILocation(line: 0, scope: !147)
!149 = !DILocation(line: 334, scope: !147)
!150 = distinct !DISubprogram(name: "String.operator.EQ.String", linkageName: "String.operator.EQ.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!151 = !DILocation(line: 0, scope: !150)
!152 = !DILocation(line: 339, scope: !150)
!153 = distinct !DISubprogram(name: "String.operator.EQ.CHARS", linkageName: "String.operator.EQ.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!154 = !DILocation(line: 0, scope: !153)
!155 = !DILocation(line: 343, scope: !153)
!156 = distinct !DISubprogram(name: "String.operator.NEQ.String", linkageName: "String.operator.NEQ.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!157 = !DILocation(line: 0, scope: !156)
!158 = !DILocation(line: 346, scope: !156)
!159 = distinct !DISubprogram(name: "String.operator.NEQ.CHARS", linkageName: "String.operator.NEQ.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!160 = !DILocation(line: 0, scope: !159)
!161 = !DILocation(line: 350, scope: !159)
!162 = distinct !DISubprogram(name: "String.operator.LT.String", linkageName: "String.operator.LT.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!163 = !DILocation(line: 0, scope: !162)
!164 = !DILocation(line: 353, scope: !162)
!165 = distinct !DISubprogram(name: "String.operator.GT.String", linkageName: "String.operator.GT.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!166 = !DILocation(line: 0, scope: !165)
!167 = !DILocation(line: 356, scope: !165)
!168 = distinct !DISubprogram(name: "String.operator.output", linkageName: "String.operator.output", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!169 = !DILocation(line: 0, scope: !168)
!170 = distinct !DISubprogram(name: "strjoin", linkageName: "strjoin", scope: null, file: !3, line: 58, type: !5, scopeLine: 58, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!171 = !DILocation(line: 58, scope: !170)
!172 = !DILocation(line: 61, scope: !170)
!173 = !DILocation(line: 63, scope: !170)
!174 = !DILocation(line: 62, scope: !170)
!175 = !DILocation(line: 66, scope: !170)
!176 = !DILocation(line: 70, scope: !170)
!177 = distinct !DISubprogram(name: "print_doubled", linkageName: "print_doubled", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!178 = !DILocation(line: 5, scope: !177)
!179 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!180 = !DILocation(line: 8, scope: !179)
!181 = !DILocation(line: 10, scope: !179)
!182 = !DILocation(line: 11, scope: !179)
!183 = !DILocation(line: 12, scope: !179)
!184 = !DILocation(line: 13, scope: !179)
