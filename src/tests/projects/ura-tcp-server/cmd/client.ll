; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd/client.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd/client.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.String = type { i8*, i32, i32 }
%struct.SockAddr = type { i8* }
%struct.Client = type { i32, i8*, i8* }

@STR0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@STR1 = private unnamed_addr constant [9 x i8] c"%H:%M:%S\00", align 1
@STR2 = private unnamed_addr constant [9 x i8] c"\1B[0;90m[\00", align 1
@STR3 = private unnamed_addr constant [7 x i8] c"]\1B[0m \00", align 1
@STR4 = private unnamed_addr constant [6 x i8] c"\1B[0m \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@STR6 = private unnamed_addr constant [8 x i8] c"\1B[0;36m\00", align 1
@STR7 = private unnamed_addr constant [123 x i8] c"  \E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\97     \E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\96\88\E2\95\97   \E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\0A\00", align 1
@STR8 = private unnamed_addr constant [127 x i8] c" \E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97  \E2\96\88\E2\96\88\E2\95\91\E2\95\9A\E2\95\90\E2\95\90\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\9D\0A\00", align 1
@STR9 = private unnamed_addr constant [103 x i8] c" \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97  \E2\96\88\E2\96\88\E2\95\94\E2\96\88\E2\96\88\E2\95\97 \E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR10 = private unnamed_addr constant [105 x i8] c" \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\9D  \E2\96\88\E2\96\88\E2\95\91\E2\95\9A\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR11 = private unnamed_addr constant [127 x i8] c" \E2\95\9A\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\91 \E2\95\9A\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR12 = private unnamed_addr constant [123 x i8] c"  \E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\9D  \E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\9D   \E2\95\9A\E2\95\90\E2\95\9D   \0A\00", align 1
@STR13 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@STR14 = private unnamed_addr constant [148 x i8] c"\1B[0;90m\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\1B[0m\0A\00", align 1
@STR15 = private unnamed_addr constant [83 x i8] c"\1B[0;36m  URA Chat Client  \1B[0;90m\E2\94\82\1B[0m connecting to \1B[0;33m127.0.0.1:17000\1B[0m\0A\00", align 1
@STR16 = private unnamed_addr constant [149 x i8] c"\1B[0;90m\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\1B[0m\0A\0A\00", align 1
@STR17 = private unnamed_addr constant [32 x i8] c"failed to create client socket\0A\00", align 1
@STR18 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR19 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR20 = private unnamed_addr constant [15 x i8] c"connect failed\00", align 1
@STR21 = private unnamed_addr constant [8 x i8] c"\1B[0;36m\00", align 1
@STR22 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR23 = private unnamed_addr constant [20 x i8] c"connected to server\00", align 1
@STR24 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR25 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR26 = private unnamed_addr constant [20 x i8] c"server disconnected\00", align 1
@STR27 = private unnamed_addr constant [6 x i8] c"\0D\1B[2K\00", align 1
@STR28 = private unnamed_addr constant [8 x i8] c"\1B[0;33m\00", align 1
@STR29 = private unnamed_addr constant [9 x i8] c"[server]\00", align 1
@STR30 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1
@STR31 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1
@STR32 = private unnamed_addr constant [6 x i8] c"/exit\00", align 1
@STR33 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1

declare i32 @write(i32, i8*, i32)

declare i32 @read(i32, i8*, i32)

declare i8* @calloc(i64, i64)

declare i8* @realloc(i8*, i32)

declare void @free(i8*)

declare void @exit(i32)

declare i32 @atoi(i8*)

declare i32 @strlen(i8*)

declare i32 @strcmp(i8*, i8*)

declare i32 @strncmp(i8*, i8*, i32)

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

define void @String.delete(%struct.String* %self) !dbg !13 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !14
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !14
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !15
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !15
  %DOT = load i8*, i8** %value, align 8, !dbg !15
  call void @free(i8* %DOT), !dbg !15
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !15
  ret void, !dbg !15
}

define void @String.assign(i8* %str, %struct.String* %self) !dbg !16 {
entry:
  %str1 = alloca i8*, align 8, !dbg !17
  %self2 = alloca %struct.String*, align 8, !dbg !17
  %len = alloca i32, align 4, !dbg !17
  store i8* %str, i8** %str1, align 8, !dbg !17
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !17
  br label %if.start, !dbg !17

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !17
  %EQ = icmp eq i8* %str3, null, !dbg !17
  br i1 %EQ, label %if.then, label %if.end, !dbg !17

if.end:                                           ; preds = %if.then, %if.start
  store i32 0, i32* %len, align 4, !dbg !17
  %str4 = load i8*, i8** %str1, align 8, !dbg !18
  %strlen = call i32 @strlen(i8* %str4), !dbg !18
  store i32 %strlen, i32* %len, align 4, !dbg !18
  br label %if.start5, !dbg !18

if.then:                                          ; preds = %if.start
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @STR0, i32 0, i32 0), i8** %str1, align 8, !dbg !17
  br label %if.end, !dbg !17

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !18
  %value = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 0, !dbg !18
  %DOT = load i8*, i8** %value, align 8, !dbg !18
  %EQ9 = icmp eq i8* %DOT, null, !dbg !18
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !18
  %size = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 2, !dbg !18
  %DOT11 = load i32, i32* %size, align 4, !dbg !18
  %len12 = load i32, i32* %len, align 4, !dbg !18
  %LE = icmp sle i32 %DOT11, %len12, !dbg !18
  %OR = or i1 %EQ9, %LE, !dbg !18
  br i1 %OR, label %if.then7, label %if.end6, !dbg !18

if.end6:                                          ; preds = %while.end, %if.start5
  %self38 = load %struct.String*, %struct.String** %self2, align 8, !dbg !19
  %value39 = getelementptr %struct.String, %struct.String* %self38, i32 0, i32 0, !dbg !19
  %DOT40 = load i8*, i8** %value39, align 8, !dbg !19
  %str41 = load i8*, i8** %str1, align 8, !dbg !19
  %strcpy = call i8* @strcpy(i8* %DOT40, i8* %str41), !dbg !19
  %self42 = load %struct.String*, %struct.String** %self2, align 8, !dbg !20
  %value43 = getelementptr %struct.String, %struct.String* %self42, i32 0, i32 0, !dbg !20
  %DOT44 = load i8*, i8** %value43, align 8, !dbg !20
  %len45 = load i32, i32* %len, align 4, !dbg !20
  %idx = sext i32 %len45 to i64, !dbg !20
  %ADD = getelementptr i8, i8* %DOT44, i64 %idx, !dbg !20
  %self46 = load %struct.String*, %struct.String** %self2, align 8, !dbg !20
  %size47 = getelementptr %struct.String, %struct.String* %self46, i32 0, i32 2, !dbg !20
  %DOT48 = load i32, i32* %size47, align 4, !dbg !20
  %len49 = load i32, i32* %len, align 4, !dbg !20
  %SUB = sub i32 %DOT48, %len49, !dbg !20
  %as = sext i32 %SUB to i64, !dbg !20
  call void @bzero(i8* %ADD, i64 %as), !dbg !20
  %self50 = load %struct.String*, %struct.String** %self2, align 8, !dbg !20
  %count = getelementptr %struct.String, %struct.String* %self50, i32 0, i32 1, !dbg !20
  %len51 = load i32, i32* %len, align 4, !dbg !20
  store i32 %len51, i32* %count, align 4, !dbg !20
  ret void, !dbg !20

if.then7:                                         ; preds = %if.start5
  br label %if.start13, !dbg !18

if.start13:                                       ; preds = %if.then7
  %self16 = load %struct.String*, %struct.String** %self2, align 8, !dbg !18
  %size17 = getelementptr %struct.String, %struct.String* %self16, i32 0, i32 2, !dbg !18
  %DOT18 = load i32, i32* %size17, align 4, !dbg !18
  %EQ19 = icmp eq i32 %DOT18, 0, !dbg !18
  br i1 %EQ19, label %if.then15, label %if.end14, !dbg !18

if.end14:                                         ; preds = %if.then15, %if.start13
  br label %while.start, !dbg !18

if.then15:                                        ; preds = %if.start13
  %self20 = load %struct.String*, %struct.String** %self2, align 8, !dbg !18
  %size21 = getelementptr %struct.String, %struct.String* %self20, i32 0, i32 2, !dbg !18
  store i32 10, i32* %size21, align 4, !dbg !18
  br label %if.end14, !dbg !18

while.start:                                      ; preds = %while.then, %if.end14
  %self22 = load %struct.String*, %struct.String** %self2, align 8, !dbg !18
  %size23 = getelementptr %struct.String, %struct.String* %self22, i32 0, i32 2, !dbg !18
  %DOT24 = load i32, i32* %size23, align 4, !dbg !18
  %len25 = load i32, i32* %len, align 4, !dbg !18
  %LE26 = icmp sle i32 %DOT24, %len25, !dbg !18
  br i1 %LE26, label %while.then, label %while.end, !dbg !18

while.then:                                       ; preds = %while.start
  %self27 = load %struct.String*, %struct.String** %self2, align 8, !dbg !18
  %size28 = getelementptr %struct.String, %struct.String* %self27, i32 0, i32 2, !dbg !18
  %DOT29 = load i32, i32* %size28, align 4, !dbg !18
  %MUL = mul i32 %DOT29, 2, !dbg !18
  store i32 %MUL, i32* %size28, align 4, !dbg !18
  br label %while.start, !dbg !18

while.end:                                        ; preds = %while.start
  %self30 = load %struct.String*, %struct.String** %self2, align 8, !dbg !18
  %value31 = getelementptr %struct.String, %struct.String* %self30, i32 0, i32 0, !dbg !18
  %self32 = load %struct.String*, %struct.String** %self2, align 8, !dbg !21
  %value33 = getelementptr %struct.String, %struct.String* %self32, i32 0, i32 0, !dbg !21
  %DOT34 = load i8*, i8** %value33, align 8, !dbg !21
  %self35 = load %struct.String*, %struct.String** %self2, align 8, !dbg !21
  %size36 = getelementptr %struct.String, %struct.String* %self35, i32 0, i32 2, !dbg !21
  %DOT37 = load i32, i32* %size36, align 4, !dbg !21
  %realloc = call i8* @realloc(i8* %DOT34, i32 %DOT37), !dbg !21
  store i8* %realloc, i8** %value31, align 8, !dbg !21
  br label %if.end6, !dbg !21
}

define void @String.join(i8* %str, %struct.String* %self) !dbg !22 {
entry:
  %str1 = alloca i8*, align 8, !dbg !23
  %self2 = alloca %struct.String*, align 8, !dbg !23
  %res = alloca i8*, align 8, !dbg !23
  store i8* %str, i8** %str1, align 8, !dbg !23
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !23
  br label %if.start, !dbg !23

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !23
  %EQ = icmp eq i8* %str3, null, !dbg !23
  br i1 %EQ, label %if.then, label %if.end, !dbg !23

if.end:                                           ; preds = %if.start
  store i8* null, i8** %res, align 8, !dbg !23
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !24
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !24
  %DOT = load i8*, i8** %value, align 8, !dbg !24
  %str5 = load i8*, i8** %str1, align 8, !dbg !24
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str5), !dbg !24
  store i8* %strjoin, i8** %res, align 8, !dbg !24
  %res6 = load i8*, i8** %res, align 8, !dbg !25
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !25
  call void @String.assign(i8* %res6, %struct.String* %ref_arg), !dbg !25
  %res7 = load i8*, i8** %res, align 8, !dbg !26
  call void @free(i8* %res7), !dbg !26
  ret void, !dbg !26

if.then:                                          ; preds = %if.start
  ret void, !dbg !23
}

define void @String.clear(%struct.String* %self) !dbg !27 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !28
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !28
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !29
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !29
  %DOT = load i8*, i8** %value, align 8, !dbg !29
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !29
  %size = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 2, !dbg !29
  %DOT4 = load i32, i32* %size, align 4, !dbg !29
  %as = sext i32 %DOT4 to i64, !dbg !29
  call void @bzero(i8* %DOT, i64 %as), !dbg !29
  %self5 = load %struct.String*, %struct.String** %self1, align 8, !dbg !29
  %count = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 1, !dbg !29
  store i32 0, i32* %count, align 4, !dbg !29
  ret void, !dbg !29
}

define void @String.push(i8 %c, %struct.String* %self) !dbg !30 {
entry:
  %c1 = alloca i8, align 1, !dbg !31
  %self2 = alloca %struct.String*, align 8, !dbg !31
  store i8 %c, i8* %c1, align 1, !dbg !31
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !31
  br label %if.start, !dbg !31

if.start:                                         ; preds = %entry
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !31
  %count = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 1, !dbg !31
  %DOT = load i32, i32* %count, align 4, !dbg !31
  %ADD = add i32 %DOT, 1, !dbg !31
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !31
  %size = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 2, !dbg !31
  %DOT5 = load i32, i32* %size, align 4, !dbg !31
  %GE = icmp sge i32 %ADD, %DOT5, !dbg !31
  br i1 %GE, label %if.then, label %if.end, !dbg !31

if.end:                                           ; preds = %if.end7, %if.start
  %self24 = load %struct.String*, %struct.String** %self2, align 8, !dbg !32
  %value25 = getelementptr %struct.String, %struct.String* %self24, i32 0, i32 0, !dbg !32
  %self26 = load %struct.String*, %struct.String** %self2, align 8, !dbg !32
  %count27 = getelementptr %struct.String, %struct.String* %self26, i32 0, i32 1, !dbg !32
  %DOT28 = load i8*, i8** %value25, align 8, !dbg !32
  %DOT29 = load i32, i32* %count27, align 4, !dbg !32
  %ACCESS = getelementptr i8, i8* %DOT28, i32 %DOT29, !dbg !32
  %c30 = load i8, i8* %c1, align 1, !dbg !32
  store i8 %c30, i8* %ACCESS, align 1, !dbg !32
  %self31 = load %struct.String*, %struct.String** %self2, align 8, !dbg !32
  %count32 = getelementptr %struct.String, %struct.String* %self31, i32 0, i32 1, !dbg !32
  %DOT33 = load i32, i32* %count32, align 4, !dbg !32
  %ADD34 = add i32 %DOT33, 1, !dbg !32
  store i32 %ADD34, i32* %count32, align 4, !dbg !32
  %self35 = load %struct.String*, %struct.String** %self2, align 8, !dbg !33
  %value36 = getelementptr %struct.String, %struct.String* %self35, i32 0, i32 0, !dbg !33
  %self37 = load %struct.String*, %struct.String** %self2, align 8, !dbg !33
  %count38 = getelementptr %struct.String, %struct.String* %self37, i32 0, i32 1, !dbg !33
  %DOT39 = load i8*, i8** %value36, align 8, !dbg !33
  %DOT40 = load i32, i32* %count38, align 4, !dbg !33
  %ACCESS41 = getelementptr i8, i8* %DOT39, i32 %DOT40, !dbg !33
  store i8 0, i8* %ACCESS41, align 1, !dbg !33
  ret void, !dbg !33

if.then:                                          ; preds = %if.start
  br label %if.start6, !dbg !31

if.start6:                                        ; preds = %if.then
  %self9 = load %struct.String*, %struct.String** %self2, align 8, !dbg !31
  %size10 = getelementptr %struct.String, %struct.String* %self9, i32 0, i32 2, !dbg !31
  %DOT11 = load i32, i32* %size10, align 4, !dbg !31
  %EQ = icmp eq i32 %DOT11, 0, !dbg !31
  br i1 %EQ, label %if.then8, label %if.else, !dbg !31

if.end7:                                          ; preds = %if.else, %if.then8
  %self17 = load %struct.String*, %struct.String** %self2, align 8, !dbg !31
  %value = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 0, !dbg !31
  %self18 = load %struct.String*, %struct.String** %self2, align 8, !dbg !34
  %value19 = getelementptr %struct.String, %struct.String* %self18, i32 0, i32 0, !dbg !34
  %DOT20 = load i8*, i8** %value19, align 8, !dbg !34
  %self21 = load %struct.String*, %struct.String** %self2, align 8, !dbg !34
  %size22 = getelementptr %struct.String, %struct.String* %self21, i32 0, i32 2, !dbg !34
  %DOT23 = load i32, i32* %size22, align 4, !dbg !34
  %realloc = call i8* @realloc(i8* %DOT20, i32 %DOT23), !dbg !34
  store i8* %realloc, i8** %value, align 8, !dbg !34
  br label %if.end, !dbg !34

if.then8:                                         ; preds = %if.start6
  %self12 = load %struct.String*, %struct.String** %self2, align 8, !dbg !31
  %size13 = getelementptr %struct.String, %struct.String* %self12, i32 0, i32 2, !dbg !31
  store i32 10, i32* %size13, align 4, !dbg !31
  br label %if.end7, !dbg !31

if.else:                                          ; preds = %if.start6
  %self14 = load %struct.String*, %struct.String** %self2, align 8, !dbg !31
  %size15 = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 2, !dbg !31
  %DOT16 = load i32, i32* %size15, align 4, !dbg !31
  %MUL = mul i32 %DOT16, 2, !dbg !31
  store i32 %MUL, i32* %size15, align 4, !dbg !31
  br label %if.end7, !dbg !31
}

define i8 @String.pop(%struct.String* %self) !dbg !35 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !36
  %c = alloca i8, align 1, !dbg !36
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !36
  br label %if.start, !dbg !36

if.start:                                         ; preds = %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !36
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !36
  %DOT = load i32, i32* %count, align 4, !dbg !36
  %EQ = icmp eq i32 %DOT, 0, !dbg !36
  br i1 %EQ, label %if.then, label %if.end, !dbg !36

if.end:                                           ; preds = %if.start
  %self3 = load %struct.String*, %struct.String** %self1, align 8, !dbg !36
  %count4 = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 1, !dbg !36
  %DOT5 = load i32, i32* %count4, align 4, !dbg !36
  %SUB = sub i32 %DOT5, 1, !dbg !36
  store i32 %SUB, i32* %count4, align 4, !dbg !36
  store i8 0, i8* %c, align 1, !dbg !36
  %self6 = load %struct.String*, %struct.String** %self1, align 8, !dbg !37
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !37
  %self7 = load %struct.String*, %struct.String** %self1, align 8, !dbg !37
  %count8 = getelementptr %struct.String, %struct.String* %self7, i32 0, i32 1, !dbg !37
  %DOT9 = load i8*, i8** %value, align 8, !dbg !37
  %DOT10 = load i32, i32* %count8, align 4, !dbg !37
  %ACCESS = getelementptr i8, i8* %DOT9, i32 %DOT10, !dbg !37
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !37
  store i8 %ACC, i8* %c, align 1, !dbg !37
  %self11 = load %struct.String*, %struct.String** %self1, align 8, !dbg !38
  %value12 = getelementptr %struct.String, %struct.String* %self11, i32 0, i32 0, !dbg !38
  %self13 = load %struct.String*, %struct.String** %self1, align 8, !dbg !38
  %count14 = getelementptr %struct.String, %struct.String* %self13, i32 0, i32 1, !dbg !38
  %DOT15 = load i8*, i8** %value12, align 8, !dbg !38
  %DOT16 = load i32, i32* %count14, align 4, !dbg !38
  %ACCESS17 = getelementptr i8, i8* %DOT15, i32 %DOT16, !dbg !38
  store i8 0, i8* %ACCESS17, align 1, !dbg !38
  %c18 = load i8, i8* %c, align 1, !dbg !38
  ret i8 %c18, !dbg !38

if.then:                                          ; preds = %if.start
  ret i8 0, !dbg !36
}

define %struct.String @String.new() !dbg !39 {
entry:
  %s = alloca %struct.String, align 8, !dbg !40
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !40
  %size = getelementptr %struct.String, %struct.String* %s, i32 0, i32 2, !dbg !40
  store i32 10, i32* %size, align 4, !dbg !40
  %value = getelementptr %struct.String, %struct.String* %s, i32 0, i32 0, !dbg !40
  %calloc = call i8* @calloc(i64 10, i64 1), !dbg !41
  store i8* %calloc, i8** %value, align 8, !dbg !41
  %s1 = load %struct.String, %struct.String* %s, align 8, !dbg !41
  ret %struct.String %s1, !dbg !41
}

define %struct.String @String.from(i8* %str) !dbg !42 {
entry:
  %str1 = alloca i8*, align 8, !dbg !43
  %s = alloca %struct.String, align 8, !dbg !43
  store i8* %str, i8** %str1, align 8, !dbg !43
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !43
  %0 = call %struct.String @String.new(), !dbg !44
  %tmp_struct = alloca %struct.String, align 8, !dbg !44
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !44
  %tmp_op = alloca %struct.String, align 8, !dbg !44
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !44
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s), !dbg !44
  %str2 = load i8*, i8** %str1, align 8, !dbg !45
  call void @String.assign(i8* %str2, %struct.String* %s), !dbg !45
  call void @String.delete(%struct.String* %tmp_struct), !dbg !45
  %s3 = load %struct.String, %struct.String* %s, align 8, !dbg !45
  ret %struct.String %s3, !dbg !45
}

define %struct.String @String.from_int(i32 %n) !dbg !46 {
entry:
  %n1 = alloca i32, align 4, !dbg !47
  %s = alloca %struct.String, align 8, !dbg !47
  %neg = alloca i1, align 1, !dbg !47
  %num = alloca i32, align 4, !dbg !47
  %buf = alloca i8*, align 8, !dbg !47
  %i = alloca i32, align 4, !dbg !47
  %j = alloca i32, align 4, !dbg !47
  store i32 %n, i32* %n1, align 4, !dbg !47
  store %struct.String zeroinitializer, %struct.String* %s, align 8, !dbg !47
  %0 = call %struct.String @String.new(), !dbg !48
  %tmp_struct = alloca %struct.String, align 8, !dbg !48
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !48
  %tmp_op = alloca %struct.String, align 8, !dbg !48
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !48
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %s), !dbg !48
  br label %if.start, !dbg !48

if.start:                                         ; preds = %entry
  %n2 = load i32, i32* %n1, align 4, !dbg !48
  %EQ = icmp eq i32 %n2, 0, !dbg !48
  br i1 %EQ, label %if.then, label %if.end, !dbg !48

if.end:                                           ; preds = %if.start
  store i1 false, i1* %neg, align 1, !dbg !47
  store i1 false, i1* %neg, align 1, !dbg !47
  store i32 0, i32* %num, align 4, !dbg !47
  %n4 = load i32, i32* %n1, align 4, !dbg !47
  store i32 %n4, i32* %num, align 4, !dbg !47
  br label %if.start5, !dbg !47

if.then:                                          ; preds = %if.start
  call void @String.push(i8 48, %struct.String* %s), !dbg !49
  %s3 = load %struct.String, %struct.String* %s, align 8, !dbg !49
  ret %struct.String %s3, !dbg !49

if.start5:                                        ; preds = %if.end
  %num8 = load i32, i32* %num, align 4, !dbg !47
  %LT = icmp slt i32 %num8, 0, !dbg !47
  br i1 %LT, label %if.then7, label %if.end6, !dbg !47

if.end6:                                          ; preds = %if.then7, %if.start5
  store i8* null, i8** %buf, align 8, !dbg !47
  %stack = alloca [20 x i8], align 1, !dbg !47
  %stack10 = getelementptr [20 x i8], [20 x i8]* %stack, i32 0, i32 0, !dbg !47
  store i8* %stack10, i8** %buf, align 8, !dbg !47
  store i32 0, i32* %i, align 4, !dbg !47
  store i32 0, i32* %i, align 4, !dbg !47
  br label %while.start, !dbg !47

if.then7:                                         ; preds = %if.start5
  store i1 true, i1* %neg, align 1, !dbg !47
  %num9 = load i32, i32* %num, align 4, !dbg !47
  %SUB = sub i32 0, %num9, !dbg !47
  store i32 %SUB, i32* %num, align 4, !dbg !47
  br label %if.end6, !dbg !47

while.start:                                      ; preds = %while.then, %if.end6
  %num11 = load i32, i32* %num, align 4, !dbg !47
  %GT = icmp sgt i32 %num11, 0, !dbg !47
  br i1 %GT, label %while.then, label %while.end, !dbg !47

while.then:                                       ; preds = %while.start
  %buf12 = load i8*, i8** %buf, align 8, !dbg !50
  %i13 = load i32, i32* %i, align 4, !dbg !50
  %ACCESS = getelementptr i8, i8* %buf12, i32 %i13, !dbg !50
  %num14 = load i32, i32* %num, align 4, !dbg !50
  %MOD = srem i32 %num14, 10, !dbg !50
  %ADD = add i32 %MOD, 48, !dbg !50
  %as = trunc i32 %ADD to i8, !dbg !50
  store i8 %as, i8* %ACCESS, align 1, !dbg !50
  %num15 = load i32, i32* %num, align 4, !dbg !50
  %DIV = sdiv i32 %num15, 10, !dbg !50
  store i32 %DIV, i32* %num, align 4, !dbg !50
  %i16 = load i32, i32* %i, align 4, !dbg !50
  %ADD17 = add i32 %i16, 1, !dbg !50
  store i32 %ADD17, i32* %i, align 4, !dbg !50
  br label %while.start, !dbg !50

while.end:                                        ; preds = %while.start
  br label %if.start18, !dbg !50

if.start18:                                       ; preds = %while.end
  %neg21 = load i1, i1* %neg, align 1, !dbg !50
  br i1 %neg21, label %if.then20, label %if.end19, !dbg !50

if.end19:                                         ; preds = %if.then20, %if.start18
  store i32 0, i32* %j, align 4, !dbg !47
  %i22 = load i32, i32* %i, align 4, !dbg !47
  %SUB23 = sub i32 %i22, 1, !dbg !47
  store i32 %SUB23, i32* %j, align 4, !dbg !47
  br label %while.start24, !dbg !47

if.then20:                                        ; preds = %if.start18
  call void @String.push(i8 45, %struct.String* %s), !dbg !51
  br label %if.end19, !dbg !51

while.start24:                                    ; preds = %while.then25, %if.end19
  %j27 = load i32, i32* %j, align 4, !dbg !47
  %GE = icmp sge i32 %j27, 0, !dbg !47
  br i1 %GE, label %while.then25, label %while.end26, !dbg !47

while.then25:                                     ; preds = %while.start24
  %buf28 = load i8*, i8** %buf, align 8, !dbg !52
  %j29 = load i32, i32* %j, align 4, !dbg !52
  %ACCESS30 = getelementptr i8, i8* %buf28, i32 %j29, !dbg !52
  %ACC = load i8, i8* %ACCESS30, align 1, !dbg !52
  call void @String.push(i8 %ACC, %struct.String* %s), !dbg !52
  %j31 = load i32, i32* %j, align 4, !dbg !52
  %SUB32 = sub i32 %j31, 1, !dbg !52
  store i32 %SUB32, i32* %j, align 4, !dbg !52
  br label %while.start24, !dbg !52

while.end26:                                      ; preds = %while.start24
  call void @String.delete(%struct.String* %tmp_struct), !dbg !52
  %s33 = load %struct.String, %struct.String* %s, align 8, !dbg !52
  ret %struct.String %s33, !dbg !52
}

define i32 @String.len(%struct.String* %self) !dbg !53 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !54
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !54
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !54
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !54
  %DOT = load i32, i32* %count, align 4, !dbg !54
  ret i32 %DOT, !dbg !54
}

define i1 @String.empty(%struct.String* %self) !dbg !55 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !56
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !56
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !56
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !56
  %DOT = load i32, i32* %count, align 4, !dbg !56
  %EQ = icmp eq i32 %DOT, 0, !dbg !56
  ret i1 %EQ, !dbg !56
}

define i8* @String.c_str(%struct.String* %self) !dbg !57 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !58
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !58
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !58
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !58
  %DOT = load i8*, i8** %value, align 8, !dbg !58
  ret i8* %DOT, !dbg !58
}

define i8 @String.at(i32 %i, %struct.String* %self) !dbg !59 {
entry:
  %i1 = alloca i32, align 4, !dbg !60
  %self2 = alloca %struct.String*, align 8, !dbg !60
  store i32 %i, i32* %i1, align 4, !dbg !60
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !60
  br label %if.start, !dbg !60

if.start:                                         ; preds = %entry
  %i3 = load i32, i32* %i1, align 4, !dbg !60
  %LT = icmp slt i32 %i3, 0, !dbg !60
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !60
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !60
  %i5 = load i32, i32* %i1, align 4, !dbg !60
  %DOT = load i32, i32* %count, align 4, !dbg !60
  %GE = icmp sge i32 %i5, %DOT, !dbg !60
  %OR = or i1 %LT, %GE, !dbg !60
  br i1 %OR, label %if.then, label %if.end, !dbg !60

if.end:                                           ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self2, align 8, !dbg !61
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !61
  %DOT7 = load i8*, i8** %value, align 8, !dbg !61
  %i8 = load i32, i32* %i1, align 4, !dbg !61
  %ACCESS = getelementptr i8, i8* %DOT7, i32 %i8, !dbg !61
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !61
  ret i8 %ACC, !dbg !61

if.then:                                          ; preds = %if.start
  ret i8 0, !dbg !60
}

define i32 @String.find(i8* %needle, %struct.String* %self) !dbg !62 {
entry:
  %needle1 = alloca i8*, align 8, !dbg !63
  %self2 = alloca %struct.String*, align 8, !dbg !63
  %i = alloca i32, align 4, !dbg !63
  %nlen = alloca i32, align 4, !dbg !63
  store i8* %needle, i8** %needle1, align 8, !dbg !63
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !63
  br label %if.start, !dbg !63

if.start:                                         ; preds = %entry
  %needle3 = load i8*, i8** %needle1, align 8, !dbg !63
  %EQ = icmp eq i8* %needle3, null, !dbg !63
  br i1 %EQ, label %if.then, label %if.end, !dbg !63

if.end:                                           ; preds = %if.start
  store i32 0, i32* %i, align 4, !dbg !63
  store i32 0, i32* %i, align 4, !dbg !63
  store i32 0, i32* %nlen, align 4, !dbg !63
  %needle4 = load i8*, i8** %needle1, align 8, !dbg !64
  %strlen = call i32 @strlen(i8* %needle4), !dbg !64
  store i32 %strlen, i32* %nlen, align 4, !dbg !64
  br label %if.start5, !dbg !64

if.then:                                          ; preds = %if.start
  ret i32 -1, !dbg !63

if.start5:                                        ; preds = %if.end
  %nlen8 = load i32, i32* %nlen, align 4, !dbg !64
  %EQ9 = icmp eq i32 %nlen8, 0, !dbg !64
  br i1 %EQ9, label %if.then7, label %if.end6, !dbg !64

if.end6:                                          ; preds = %if.start5
  br label %while.start, !dbg !64

if.then7:                                         ; preds = %if.start5
  ret i32 0, !dbg !64

while.start:                                      ; preds = %if.end14, %if.end6
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !64
  %count = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 1, !dbg !64
  %DOT = load i32, i32* %count, align 4, !dbg !64
  %nlen11 = load i32, i32* %nlen, align 4, !dbg !64
  %SUB = sub i32 %DOT, %nlen11, !dbg !64
  %i12 = load i32, i32* %i, align 4, !dbg !64
  %LE = icmp sle i32 %i12, %SUB, !dbg !64
  br i1 %LE, label %while.then, label %while.end, !dbg !64

while.then:                                       ; preds = %while.start
  br label %if.start13, !dbg !64

while.end:                                        ; preds = %while.start
  ret i32 -1, !dbg !65

if.start13:                                       ; preds = %while.then
  %self16 = load %struct.String*, %struct.String** %self2, align 8, !dbg !65
  %value = getelementptr %struct.String, %struct.String* %self16, i32 0, i32 0, !dbg !65
  %DOT17 = load i8*, i8** %value, align 8, !dbg !65
  %i18 = load i32, i32* %i, align 4, !dbg !65
  %idx = sext i32 %i18 to i64, !dbg !65
  %ADD = getelementptr i8, i8* %DOT17, i64 %idx, !dbg !65
  %needle19 = load i8*, i8** %needle1, align 8, !dbg !65
  %nlen20 = load i32, i32* %nlen, align 4, !dbg !65
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %needle19, i32 %nlen20), !dbg !65
  %EQ21 = icmp eq i32 %strncmp, 0, !dbg !65
  br i1 %EQ21, label %if.then15, label %if.end14, !dbg !65

if.end14:                                         ; preds = %if.start13
  %i23 = load i32, i32* %i, align 4, !dbg !65
  %ADD24 = add i32 %i23, 1, !dbg !65
  store i32 %ADD24, i32* %i, align 4, !dbg !65
  br label %while.start, !dbg !65

if.then15:                                        ; preds = %if.start13
  %i22 = load i32, i32* %i, align 4, !dbg !65
  ret i32 %i22, !dbg !65
}

define i1 @String.contains(i8* %needle, %struct.String* %self) !dbg !66 {
entry:
  %needle1 = alloca i8*, align 8, !dbg !67
  %self2 = alloca %struct.String*, align 8, !dbg !67
  store i8* %needle, i8** %needle1, align 8, !dbg !67
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !67
  %needle3 = load i8*, i8** %needle1, align 8, !dbg !68
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !68
  %String.find = call i32 @String.find(i8* %needle3, %struct.String* %ref_arg), !dbg !68
  %NEQ = icmp ne i32 %String.find, -1, !dbg !68
  ret i1 %NEQ, !dbg !68
}

define i1 @String.starts_with(i8* %prefix, %struct.String* %self) !dbg !69 {
entry:
  %prefix1 = alloca i8*, align 8, !dbg !70
  %self2 = alloca %struct.String*, align 8, !dbg !70
  %plen = alloca i32, align 4, !dbg !70
  store i8* %prefix, i8** %prefix1, align 8, !dbg !70
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !70
  br label %if.start, !dbg !70

if.start:                                         ; preds = %entry
  %prefix3 = load i8*, i8** %prefix1, align 8, !dbg !70
  %EQ = icmp eq i8* %prefix3, null, !dbg !70
  br i1 %EQ, label %if.then, label %if.end, !dbg !70

if.end:                                           ; preds = %if.start
  store i32 0, i32* %plen, align 4, !dbg !70
  %prefix4 = load i8*, i8** %prefix1, align 8, !dbg !71
  %strlen = call i32 @strlen(i8* %prefix4), !dbg !71
  store i32 %strlen, i32* %plen, align 4, !dbg !71
  br label %if.start5, !dbg !71

if.then:                                          ; preds = %if.start
  ret i1 true, !dbg !70

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !71
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !71
  %plen9 = load i32, i32* %plen, align 4, !dbg !71
  %DOT = load i32, i32* %count, align 4, !dbg !71
  %GT = icmp sgt i32 %plen9, %DOT, !dbg !71
  br i1 %GT, label %if.then7, label %if.end6, !dbg !71

if.end6:                                          ; preds = %if.start5
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !72
  %value = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 0, !dbg !72
  %DOT11 = load i8*, i8** %value, align 8, !dbg !72
  %prefix12 = load i8*, i8** %prefix1, align 8, !dbg !72
  %plen13 = load i32, i32* %plen, align 4, !dbg !72
  %strncmp = call i32 @strncmp(i8* %DOT11, i8* %prefix12, i32 %plen13), !dbg !72
  %EQ14 = icmp eq i32 %strncmp, 0, !dbg !72
  ret i1 %EQ14, !dbg !72

if.then7:                                         ; preds = %if.start5
  ret i1 false, !dbg !71
}

define i1 @String.ends_with(i8* %suffix, %struct.String* %self) !dbg !73 {
entry:
  %suffix1 = alloca i8*, align 8, !dbg !74
  %self2 = alloca %struct.String*, align 8, !dbg !74
  %slen = alloca i32, align 4, !dbg !74
  %offset = alloca i32, align 4, !dbg !74
  store i8* %suffix, i8** %suffix1, align 8, !dbg !74
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !74
  br label %if.start, !dbg !74

if.start:                                         ; preds = %entry
  %suffix3 = load i8*, i8** %suffix1, align 8, !dbg !74
  %EQ = icmp eq i8* %suffix3, null, !dbg !74
  br i1 %EQ, label %if.then, label %if.end, !dbg !74

if.end:                                           ; preds = %if.start
  store i32 0, i32* %slen, align 4, !dbg !74
  %suffix4 = load i8*, i8** %suffix1, align 8, !dbg !75
  %strlen = call i32 @strlen(i8* %suffix4), !dbg !75
  store i32 %strlen, i32* %slen, align 4, !dbg !75
  br label %if.start5, !dbg !75

if.then:                                          ; preds = %if.start
  ret i1 true, !dbg !74

if.start5:                                        ; preds = %if.end
  %self8 = load %struct.String*, %struct.String** %self2, align 8, !dbg !75
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !75
  %slen9 = load i32, i32* %slen, align 4, !dbg !75
  %DOT = load i32, i32* %count, align 4, !dbg !75
  %GT = icmp sgt i32 %slen9, %DOT, !dbg !75
  br i1 %GT, label %if.then7, label %if.end6, !dbg !75

if.end6:                                          ; preds = %if.start5
  store i32 0, i32* %offset, align 4, !dbg !74
  %self10 = load %struct.String*, %struct.String** %self2, align 8, !dbg !74
  %count11 = getelementptr %struct.String, %struct.String* %self10, i32 0, i32 1, !dbg !74
  %DOT12 = load i32, i32* %count11, align 4, !dbg !74
  %slen13 = load i32, i32* %slen, align 4, !dbg !74
  %SUB = sub i32 %DOT12, %slen13, !dbg !74
  store i32 %SUB, i32* %offset, align 4, !dbg !74
  %self14 = load %struct.String*, %struct.String** %self2, align 8, !dbg !76
  %value = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 0, !dbg !76
  %DOT15 = load i8*, i8** %value, align 8, !dbg !76
  %offset16 = load i32, i32* %offset, align 4, !dbg !76
  %idx = sext i32 %offset16 to i64, !dbg !76
  %ADD = getelementptr i8, i8* %DOT15, i64 %idx, !dbg !76
  %suffix17 = load i8*, i8** %suffix1, align 8, !dbg !76
  %slen18 = load i32, i32* %slen, align 4, !dbg !76
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %suffix17, i32 %slen18), !dbg !76
  %EQ19 = icmp eq i32 %strncmp, 0, !dbg !76
  ret i1 %EQ19, !dbg !76

if.then7:                                         ; preds = %if.start5
  ret i1 false, !dbg !75
}

define %struct.String @String.substr(i32 %start, i32 %length, %struct.String* %self) !dbg !77 {
entry:
  %start1 = alloca i32, align 4, !dbg !78
  %length2 = alloca i32, align 4, !dbg !78
  %self3 = alloca %struct.String*, align 8, !dbg !78
  %res = alloca %struct.String, align 8, !dbg !78
  %end = alloca i32, align 4, !dbg !78
  %i = alloca i32, align 4, !dbg !78
  store i32 %start, i32* %start1, align 4, !dbg !78
  store i32 %length, i32* %length2, align 4, !dbg !78
  store %struct.String* %self, %struct.String** %self3, align 8, !dbg !78
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !78
  %0 = call %struct.String @String.new(), !dbg !79
  %tmp_struct = alloca %struct.String, align 8, !dbg !79
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !79
  %tmp_op = alloca %struct.String, align 8, !dbg !79
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !79
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !79
  br label %if.start, !dbg !79

if.start:                                         ; preds = %entry
  %start4 = load i32, i32* %start1, align 4, !dbg !79
  %LT = icmp slt i32 %start4, 0, !dbg !79
  %self5 = load %struct.String*, %struct.String** %self3, align 8, !dbg !79
  %count = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 1, !dbg !79
  %start6 = load i32, i32* %start1, align 4, !dbg !79
  %DOT = load i32, i32* %count, align 4, !dbg !79
  %GE = icmp sge i32 %start6, %DOT, !dbg !79
  %OR = or i1 %LT, %GE, !dbg !79
  br i1 %OR, label %if.then, label %if.end, !dbg !79

if.end:                                           ; preds = %if.start
  store i32 0, i32* %end, align 4, !dbg !78
  %start8 = load i32, i32* %start1, align 4, !dbg !78
  %length9 = load i32, i32* %length2, align 4, !dbg !78
  %ADD = add i32 %start8, %length9, !dbg !78
  store i32 %ADD, i32* %end, align 4, !dbg !78
  br label %if.start10, !dbg !78

if.then:                                          ; preds = %if.start
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !79
  ret %struct.String %res7, !dbg !79

if.start10:                                       ; preds = %if.end
  %self13 = load %struct.String*, %struct.String** %self3, align 8, !dbg !78
  %count14 = getelementptr %struct.String, %struct.String* %self13, i32 0, i32 1, !dbg !78
  %end15 = load i32, i32* %end, align 4, !dbg !78
  %DOT16 = load i32, i32* %count14, align 4, !dbg !78
  %GT = icmp sgt i32 %end15, %DOT16, !dbg !78
  br i1 %GT, label %if.then12, label %if.end11, !dbg !78

if.end11:                                         ; preds = %if.then12, %if.start10
  store i32 0, i32* %i, align 4, !dbg !78
  %start20 = load i32, i32* %start1, align 4, !dbg !78
  store i32 %start20, i32* %i, align 4, !dbg !78
  br label %while.start, !dbg !78

if.then12:                                        ; preds = %if.start10
  %self17 = load %struct.String*, %struct.String** %self3, align 8, !dbg !78
  %count18 = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 1, !dbg !78
  %DOT19 = load i32, i32* %count18, align 4, !dbg !78
  store i32 %DOT19, i32* %end, align 4, !dbg !78
  br label %if.end11, !dbg !78

while.start:                                      ; preds = %while.then, %if.end11
  %i21 = load i32, i32* %i, align 4, !dbg !78
  %end22 = load i32, i32* %end, align 4, !dbg !78
  %LT23 = icmp slt i32 %i21, %end22, !dbg !78
  br i1 %LT23, label %while.then, label %while.end, !dbg !78

while.then:                                       ; preds = %while.start
  %self24 = load %struct.String*, %struct.String** %self3, align 8, !dbg !80
  %value = getelementptr %struct.String, %struct.String* %self24, i32 0, i32 0, !dbg !80
  %DOT25 = load i8*, i8** %value, align 8, !dbg !80
  %i26 = load i32, i32* %i, align 4, !dbg !80
  %ACCESS = getelementptr i8, i8* %DOT25, i32 %i26, !dbg !80
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !80
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !80
  %i27 = load i32, i32* %i, align 4, !dbg !80
  %ADD28 = add i32 %i27, 1, !dbg !80
  store i32 %ADD28, i32* %i, align 4, !dbg !80
  br label %while.start, !dbg !80

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !80
  %res29 = load %struct.String, %struct.String* %res, align 8, !dbg !80
  ret %struct.String %res29, !dbg !80
}

define %struct.String @String.upper(%struct.String* %self) !dbg !81 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !82
  %res = alloca %struct.String, align 8, !dbg !82
  %i = alloca i32, align 4, !dbg !82
  %c = alloca i8, align 1, !dbg !82
  %v = alloca i32, align 4, !dbg !82
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !82
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !82
  %0 = call %struct.String @String.new(), !dbg !83
  %tmp_struct = alloca %struct.String, align 8, !dbg !83
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !83
  %tmp_op = alloca %struct.String, align 8, !dbg !83
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !83
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !83
  store i32 0, i32* %i, align 4, !dbg !82
  store i32 0, i32* %i, align 4, !dbg !82
  br label %while.start, !dbg !82

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !82
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !82
  %i3 = load i32, i32* %i, align 4, !dbg !82
  %DOT = load i32, i32* %count, align 4, !dbg !82
  %LT = icmp slt i32 %i3, %DOT, !dbg !82
  br i1 %LT, label %while.then, label %while.end, !dbg !82

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !82
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !84
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !84
  %DOT5 = load i8*, i8** %value, align 8, !dbg !84
  %i6 = load i32, i32* %i, align 4, !dbg !84
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !84
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !84
  store i8 %ACC, i8* %c, align 1, !dbg !84
  br label %if.start, !dbg !84

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !85
  %res14 = load %struct.String, %struct.String* %res, align 8, !dbg !85
  ret %struct.String %res14, !dbg !85

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !84
  %GE = icmp sge i8 %c7, 97, !dbg !84
  %c8 = load i8, i8* %c, align 1, !dbg !84
  %LE = icmp sle i8 %c8, 122, !dbg !84
  %AND = and i1 %GE, %LE, !dbg !84
  br i1 %AND, label %if.then, label %if.else, !dbg !84

if.end:                                           ; preds = %if.else, %if.then
  %i13 = load i32, i32* %i, align 4, !dbg !85
  %ADD = add i32 %i13, 1, !dbg !85
  store i32 %ADD, i32* %i, align 4, !dbg !85
  br label %while.start, !dbg !85

if.then:                                          ; preds = %if.start
  store i32 0, i32* %v, align 4, !dbg !82
  %c9 = load i8, i8* %c, align 1, !dbg !82
  %as = sext i8 %c9 to i32, !dbg !82
  %SUB = sub i32 %as, 32, !dbg !82
  store i32 %SUB, i32* %v, align 4, !dbg !82
  %v10 = load i32, i32* %v, align 4, !dbg !86
  %as11 = trunc i32 %v10 to i8, !dbg !86
  call void @String.push(i8 %as11, %struct.String* %res), !dbg !86
  br label %if.end, !dbg !86

if.else:                                          ; preds = %if.start
  %c12 = load i8, i8* %c, align 1, !dbg !85
  call void @String.push(i8 %c12, %struct.String* %res), !dbg !85
  br label %if.end, !dbg !85
}

define %struct.String @String.lower(%struct.String* %self) !dbg !87 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !88
  %res = alloca %struct.String, align 8, !dbg !88
  %i = alloca i32, align 4, !dbg !88
  %c = alloca i8, align 1, !dbg !88
  %v = alloca i32, align 4, !dbg !88
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !88
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !88
  %0 = call %struct.String @String.new(), !dbg !89
  %tmp_struct = alloca %struct.String, align 8, !dbg !89
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !89
  %tmp_op = alloca %struct.String, align 8, !dbg !89
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !89
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !89
  store i32 0, i32* %i, align 4, !dbg !88
  store i32 0, i32* %i, align 4, !dbg !88
  br label %while.start, !dbg !88

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !88
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !88
  %i3 = load i32, i32* %i, align 4, !dbg !88
  %DOT = load i32, i32* %count, align 4, !dbg !88
  %LT = icmp slt i32 %i3, %DOT, !dbg !88
  br i1 %LT, label %while.then, label %while.end, !dbg !88

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !88
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !90
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !90
  %DOT5 = load i8*, i8** %value, align 8, !dbg !90
  %i6 = load i32, i32* %i, align 4, !dbg !90
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !90
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !90
  store i8 %ACC, i8* %c, align 1, !dbg !90
  br label %if.start, !dbg !90

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !91
  %res15 = load %struct.String, %struct.String* %res, align 8, !dbg !91
  ret %struct.String %res15, !dbg !91

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !90
  %GE = icmp sge i8 %c7, 65, !dbg !90
  %c8 = load i8, i8* %c, align 1, !dbg !90
  %LE = icmp sle i8 %c8, 90, !dbg !90
  %AND = and i1 %GE, %LE, !dbg !90
  br i1 %AND, label %if.then, label %if.else, !dbg !90

if.end:                                           ; preds = %if.else, %if.then
  %i13 = load i32, i32* %i, align 4, !dbg !91
  %ADD14 = add i32 %i13, 1, !dbg !91
  store i32 %ADD14, i32* %i, align 4, !dbg !91
  br label %while.start, !dbg !91

if.then:                                          ; preds = %if.start
  store i32 0, i32* %v, align 4, !dbg !88
  %c9 = load i8, i8* %c, align 1, !dbg !88
  %as = sext i8 %c9 to i32, !dbg !88
  %ADD = add i32 %as, 32, !dbg !88
  store i32 %ADD, i32* %v, align 4, !dbg !88
  %v10 = load i32, i32* %v, align 4, !dbg !92
  %as11 = trunc i32 %v10 to i8, !dbg !92
  call void @String.push(i8 %as11, %struct.String* %res), !dbg !92
  br label %if.end, !dbg !92

if.else:                                          ; preds = %if.start
  %c12 = load i8, i8* %c, align 1, !dbg !91
  call void @String.push(i8 %c12, %struct.String* %res), !dbg !91
  br label %if.end, !dbg !91
}

define %struct.String @String.trim(%struct.String* %self) !dbg !93 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !94
  %start = alloca i32, align 4, !dbg !94
  %c = alloca i8, align 1, !dbg !94
  %end = alloca i32, align 4, !dbg !94
  %c25 = alloca i8, align 1, !dbg !94
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !94
  store i32 0, i32* %start, align 4, !dbg !94
  store i32 0, i32* %start, align 4, !dbg !94
  br label %while.start, !dbg !94

while.start:                                      ; preds = %if.end, %entry
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !94
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !94
  %start3 = load i32, i32* %start, align 4, !dbg !94
  %DOT = load i32, i32* %count, align 4, !dbg !94
  %LT = icmp slt i32 %start3, %DOT, !dbg !94
  br i1 %LT, label %while.then, label %while.end, !dbg !94

while.then:                                       ; preds = %while.start
  store i8 0, i8* %c, align 1, !dbg !94
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !95
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !95
  %DOT5 = load i8*, i8** %value, align 8, !dbg !95
  %start6 = load i32, i32* %start, align 4, !dbg !95
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %start6, !dbg !95
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !95
  store i8 %ACC, i8* %c, align 1, !dbg !95
  br label %if.start, !dbg !95

while.end:                                        ; preds = %if.else, %while.start
  store i32 0, i32* %end, align 4, !dbg !94
  %self17 = load %struct.String*, %struct.String** %self1, align 8, !dbg !94
  %count18 = getelementptr %struct.String, %struct.String* %self17, i32 0, i32 1, !dbg !94
  %DOT19 = load i32, i32* %count18, align 4, !dbg !94
  store i32 %DOT19, i32* %end, align 4, !dbg !94
  br label %while.start20, !dbg !94

if.start:                                         ; preds = %while.then
  %c7 = load i8, i8* %c, align 1, !dbg !95
  %EQ = icmp eq i8 %c7, 32, !dbg !95
  %c8 = load i8, i8* %c, align 1, !dbg !95
  %EQ9 = icmp eq i8 %c8, 9, !dbg !95
  %OR = or i1 %EQ, %EQ9, !dbg !95
  %c10 = load i8, i8* %c, align 1, !dbg !95
  %EQ11 = icmp eq i8 %c10, 10, !dbg !95
  %OR12 = or i1 %OR, %EQ11, !dbg !95
  %c13 = load i8, i8* %c, align 1, !dbg !95
  %EQ14 = icmp eq i8 %c13, 13, !dbg !95
  %OR15 = or i1 %OR12, %EQ14, !dbg !95
  br i1 %OR15, label %if.then, label %if.else, !dbg !95

if.end:                                           ; preds = %if.then
  br label %while.start, !dbg !95

if.then:                                          ; preds = %if.start
  %start16 = load i32, i32* %start, align 4, !dbg !95
  %ADD = add i32 %start16, 1, !dbg !95
  store i32 %ADD, i32* %start, align 4, !dbg !95
  br label %if.end, !dbg !95

if.else:                                          ; preds = %if.start
  br label %while.end, !dbg !95

while.start20:                                    ; preds = %if.end33, %while.end
  %end23 = load i32, i32* %end, align 4, !dbg !94
  %start24 = load i32, i32* %start, align 4, !dbg !94
  %GT = icmp sgt i32 %end23, %start24, !dbg !94
  br i1 %GT, label %while.then21, label %while.end22, !dbg !94

while.then21:                                     ; preds = %while.start20
  store i8 0, i8* %c25, align 1, !dbg !94
  %self26 = load %struct.String*, %struct.String** %self1, align 8, !dbg !96
  %value27 = getelementptr %struct.String, %struct.String* %self26, i32 0, i32 0, !dbg !96
  %end28 = load i32, i32* %end, align 4, !dbg !96
  %SUB = sub i32 %end28, 1, !dbg !96
  %DOT29 = load i8*, i8** %value27, align 8, !dbg !96
  %ACCESS30 = getelementptr i8, i8* %DOT29, i32 %SUB, !dbg !96
  %ACC31 = load i8, i8* %ACCESS30, align 1, !dbg !96
  store i8 %ACC31, i8* %c25, align 1, !dbg !96
  br label %if.start32, !dbg !96

while.end22:                                      ; preds = %if.else35, %while.start20
  %start49 = load i32, i32* %start, align 4, !dbg !97
  %end50 = load i32, i32* %end, align 4, !dbg !97
  %start51 = load i32, i32* %start, align 4, !dbg !97
  %SUB52 = sub i32 %end50, %start51, !dbg !97
  %ref_arg = load %struct.String*, %struct.String** %self1, align 8, !dbg !97
  %String.substr = call %struct.String @String.substr(i32 %start49, i32 %SUB52, %struct.String* %ref_arg), !dbg !97
  %tmp_struct = alloca %struct.String, align 8, !dbg !97
  store %struct.String %String.substr, %struct.String* %tmp_struct, align 8, !dbg !97
  ret %struct.String %String.substr, !dbg !97

if.start32:                                       ; preds = %while.then21
  %c36 = load i8, i8* %c25, align 1, !dbg !96
  %EQ37 = icmp eq i8 %c36, 32, !dbg !96
  %c38 = load i8, i8* %c25, align 1, !dbg !96
  %EQ39 = icmp eq i8 %c38, 9, !dbg !96
  %OR40 = or i1 %EQ37, %EQ39, !dbg !96
  %c41 = load i8, i8* %c25, align 1, !dbg !96
  %EQ42 = icmp eq i8 %c41, 10, !dbg !96
  %OR43 = or i1 %OR40, %EQ42, !dbg !96
  %c44 = load i8, i8* %c25, align 1, !dbg !96
  %EQ45 = icmp eq i8 %c44, 13, !dbg !96
  %OR46 = or i1 %OR43, %EQ45, !dbg !96
  br i1 %OR46, label %if.then34, label %if.else35, !dbg !96

if.end33:                                         ; preds = %if.then34
  br label %while.start20, !dbg !96

if.then34:                                        ; preds = %if.start32
  %end47 = load i32, i32* %end, align 4, !dbg !96
  %SUB48 = sub i32 %end47, 1, !dbg !96
  store i32 %SUB48, i32* %end, align 4, !dbg !96
  br label %if.end33, !dbg !96

if.else35:                                        ; preds = %if.start32
  br label %while.end22, !dbg !96
}

define %struct.String @String.replace(i8* %old, i8* %replacement, %struct.String* %self) !dbg !98 {
entry:
  %old1 = alloca i8*, align 8, !dbg !99
  %replacement2 = alloca i8*, align 8, !dbg !99
  %self3 = alloca %struct.String*, align 8, !dbg !99
  %res = alloca %struct.String, align 8, !dbg !99
  %olen = alloca i32, align 4, !dbg !99
  %i = alloca i32, align 4, !dbg !99
  store i8* %old, i8** %old1, align 8, !dbg !99
  store i8* %replacement, i8** %replacement2, align 8, !dbg !99
  store %struct.String* %self, %struct.String** %self3, align 8, !dbg !99
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !99
  %0 = call %struct.String @String.new(), !dbg !100
  %tmp_struct = alloca %struct.String, align 8, !dbg !100
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !100
  %tmp_op = alloca %struct.String, align 8, !dbg !100
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !100
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !100
  store i32 0, i32* %olen, align 4, !dbg !99
  %old4 = load i8*, i8** %old1, align 8, !dbg !101
  %strlen = call i32 @strlen(i8* %old4), !dbg !101
  store i32 %strlen, i32* %olen, align 4, !dbg !101
  br label %if.start, !dbg !101

if.start:                                         ; preds = %entry
  %olen5 = load i32, i32* %olen, align 4, !dbg !101
  %EQ = icmp eq i32 %olen5, 0, !dbg !101
  br i1 %EQ, label %if.then, label %if.end, !dbg !101

if.end:                                           ; preds = %if.start
  store i32 0, i32* %i, align 4, !dbg !99
  store i32 0, i32* %i, align 4, !dbg !99
  br label %while.start, !dbg !99

if.then:                                          ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self3, align 8, !dbg !102
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !102
  %DOT = load i8*, i8** %value, align 8, !dbg !102
  call void @String.assign(i8* %DOT, %struct.String* %res), !dbg !102
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !102
  ret %struct.String %res7, !dbg !102

while.start:                                      ; preds = %if.end12, %if.end
  %self8 = load %struct.String*, %struct.String** %self3, align 8, !dbg !99
  %count = getelementptr %struct.String, %struct.String* %self8, i32 0, i32 1, !dbg !99
  %i9 = load i32, i32* %i, align 4, !dbg !99
  %DOT10 = load i32, i32* %count, align 4, !dbg !99
  %LT = icmp slt i32 %i9, %DOT10, !dbg !99
  br i1 %LT, label %while.then, label %while.end, !dbg !99

while.then:                                       ; preds = %while.start
  br label %if.start11, !dbg !99

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !103
  %res36 = load %struct.String, %struct.String* %res, align 8, !dbg !103
  ret %struct.String %res36, !dbg !103

if.start11:                                       ; preds = %while.then
  %self14 = load %struct.String*, %struct.String** %self3, align 8, !dbg !99
  %count15 = getelementptr %struct.String, %struct.String* %self14, i32 0, i32 1, !dbg !99
  %DOT16 = load i32, i32* %count15, align 4, !dbg !99
  %olen17 = load i32, i32* %olen, align 4, !dbg !99
  %SUB = sub i32 %DOT16, %olen17, !dbg !99
  %i18 = load i32, i32* %i, align 4, !dbg !99
  %LE = icmp sle i32 %i18, %SUB, !dbg !99
  %self19 = load %struct.String*, %struct.String** %self3, align 8, !dbg !104
  %value20 = getelementptr %struct.String, %struct.String* %self19, i32 0, i32 0, !dbg !104
  %DOT21 = load i8*, i8** %value20, align 8, !dbg !104
  %i22 = load i32, i32* %i, align 4, !dbg !104
  %idx = sext i32 %i22 to i64, !dbg !104
  %ADD = getelementptr i8, i8* %DOT21, i64 %idx, !dbg !104
  %old23 = load i8*, i8** %old1, align 8, !dbg !104
  %olen24 = load i32, i32* %olen, align 4, !dbg !104
  %strncmp = call i32 @strncmp(i8* %ADD, i8* %old23, i32 %olen24), !dbg !104
  %EQ25 = icmp eq i32 %strncmp, 0, !dbg !104
  %AND = and i1 %LE, %EQ25, !dbg !104
  br i1 %AND, label %if.then13, label %if.else, !dbg !104

if.end12:                                         ; preds = %if.else, %if.then13
  br label %while.start, !dbg !103

if.then13:                                        ; preds = %if.start11
  %replacement26 = load i8*, i8** %replacement2, align 8, !dbg !105
  call void @String.join(i8* %replacement26, %struct.String* %res), !dbg !105
  %i27 = load i32, i32* %i, align 4, !dbg !105
  %olen28 = load i32, i32* %olen, align 4, !dbg !105
  %ADD29 = add i32 %i27, %olen28, !dbg !105
  store i32 %ADD29, i32* %i, align 4, !dbg !105
  br label %if.end12, !dbg !105

if.else:                                          ; preds = %if.start11
  %self30 = load %struct.String*, %struct.String** %self3, align 8, !dbg !103
  %value31 = getelementptr %struct.String, %struct.String* %self30, i32 0, i32 0, !dbg !103
  %DOT32 = load i8*, i8** %value31, align 8, !dbg !103
  %i33 = load i32, i32* %i, align 4, !dbg !103
  %ACCESS = getelementptr i8, i8* %DOT32, i32 %i33, !dbg !103
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !103
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !103
  %i34 = load i32, i32* %i, align 4, !dbg !103
  %ADD35 = add i32 %i34, 1, !dbg !103
  store i32 %ADD35, i32* %i, align 4, !dbg !103
  br label %if.end12, !dbg !103
}

define %struct.String @String.repeat(i32 %n, %struct.String* %self) !dbg !106 {
entry:
  %n1 = alloca i32, align 4, !dbg !107
  %self2 = alloca %struct.String*, align 8, !dbg !107
  %res = alloca %struct.String, align 8, !dbg !107
  %i = alloca i32, align 4, !dbg !107
  store i32 %n, i32* %n1, align 4, !dbg !107
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !107
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !107
  %0 = call %struct.String @String.new(), !dbg !108
  %tmp_struct = alloca %struct.String, align 8, !dbg !108
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !108
  %tmp_op = alloca %struct.String, align 8, !dbg !108
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !108
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !108
  store i32 0, i32* %i, align 4, !dbg !107
  store i32 0, i32* %i, align 4, !dbg !107
  br label %while.start, !dbg !107

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !107
  %n4 = load i32, i32* %n1, align 4, !dbg !107
  %LT = icmp slt i32 %i3, %n4, !dbg !107
  br i1 %LT, label %while.then, label %while.end, !dbg !107

while.then:                                       ; preds = %while.start
  %self5 = load %struct.String*, %struct.String** %self2, align 8, !dbg !109
  %value = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 0, !dbg !109
  %DOT = load i8*, i8** %value, align 8, !dbg !109
  call void @String.join(i8* %DOT, %struct.String* %res), !dbg !109
  %i6 = load i32, i32* %i, align 4, !dbg !109
  %ADD = add i32 %i6, 1, !dbg !109
  store i32 %ADD, i32* %i, align 4, !dbg !109
  br label %while.start, !dbg !109

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !109
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !109
  ret %struct.String %res7, !dbg !109
}

define %struct.String @String.reverse(%struct.String* %self) !dbg !110 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !111
  %res = alloca %struct.String, align 8, !dbg !111
  %i = alloca i32, align 4, !dbg !111
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !111
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !111
  %0 = call %struct.String @String.new(), !dbg !112
  %tmp_struct = alloca %struct.String, align 8, !dbg !112
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !112
  %tmp_op = alloca %struct.String, align 8, !dbg !112
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !112
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !112
  store i32 0, i32* %i, align 4, !dbg !111
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !111
  %count = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 1, !dbg !111
  %DOT = load i32, i32* %count, align 4, !dbg !111
  %SUB = sub i32 %DOT, 1, !dbg !111
  store i32 %SUB, i32* %i, align 4, !dbg !111
  br label %while.start, !dbg !111

while.start:                                      ; preds = %while.then, %entry
  %i3 = load i32, i32* %i, align 4, !dbg !111
  %GE = icmp sge i32 %i3, 0, !dbg !111
  br i1 %GE, label %while.then, label %while.end, !dbg !111

while.then:                                       ; preds = %while.start
  %self4 = load %struct.String*, %struct.String** %self1, align 8, !dbg !113
  %value = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 0, !dbg !113
  %DOT5 = load i8*, i8** %value, align 8, !dbg !113
  %i6 = load i32, i32* %i, align 4, !dbg !113
  %ACCESS = getelementptr i8, i8* %DOT5, i32 %i6, !dbg !113
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !113
  call void @String.push(i8 %ACC, %struct.String* %res), !dbg !113
  %i7 = load i32, i32* %i, align 4, !dbg !113
  %SUB8 = sub i32 %i7, 1, !dbg !113
  store i32 %SUB8, i32* %i, align 4, !dbg !113
  br label %while.start, !dbg !113

while.end:                                        ; preds = %while.start
  call void @String.delete(%struct.String* %tmp_struct), !dbg !113
  %res9 = load %struct.String, %struct.String* %res, align 8, !dbg !113
  ret %struct.String %res9, !dbg !113
}

define i32 @String.compare(%struct.String* %other, %struct.String* %self) !dbg !114 {
entry:
  %other1 = alloca %struct.String*, align 8, !dbg !115
  %self2 = alloca %struct.String*, align 8, !dbg !115
  store %struct.String* %other, %struct.String** %other1, align 8, !dbg !115
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !115
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !116
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !116
  %DOT = load i8*, i8** %value, align 8, !dbg !116
  %other4 = load %struct.String*, %struct.String** %other1, align 8, !dbg !116
  %value5 = getelementptr %struct.String, %struct.String* %other4, i32 0, i32 0, !dbg !116
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !116
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !116
  ret i32 %strcmp, !dbg !116
}

define i32 @String.to_int(%struct.String* %self) !dbg !117 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !118
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !118
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !119
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !119
  %DOT = load i8*, i8** %value, align 8, !dbg !119
  %atoi = call i32 @atoi(i8* %DOT), !dbg !119
  ret i32 %atoi, !dbg !119
}

define void @String.operator.ASSIGN.CHARS(i8* %str, %struct.String* %self) !dbg !120 {
entry:
  %str1 = alloca i8*, align 8, !dbg !121
  %self2 = alloca %struct.String*, align 8, !dbg !121
  store i8* %str, i8** %str1, align 8, !dbg !121
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !121
  %str3 = load i8*, i8** %str1, align 8, !dbg !122
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !122
  call void @String.assign(i8* %str3, %struct.String* %ref_arg), !dbg !122
  ret void, !dbg !122
}

define void @String.operator.ASSIGN.String(%struct.String* %v, %struct.String* %self) !dbg !123 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !124
  %self2 = alloca %struct.String*, align 8, !dbg !124
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !124
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !124
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !125
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !125
  %DOT = load i8*, i8** %value, align 8, !dbg !125
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !125
  call void @String.assign(i8* %DOT, %struct.String* %ref_arg), !dbg !125
  ret void, !dbg !125
}

define %struct.String @String.operator.ADD.String(%struct.String* %v, %struct.String* %self) !dbg !126 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !127
  %self2 = alloca %struct.String*, align 8, !dbg !127
  %res = alloca %struct.String, align 8, !dbg !127
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !127
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !127
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !127
  %0 = call %struct.String @String.new(), !dbg !128
  %tmp_struct = alloca %struct.String, align 8, !dbg !128
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !128
  %tmp_op = alloca %struct.String, align 8, !dbg !128
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !128
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !128
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !129
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !129
  %DOT = load i8*, i8** %value, align 8, !dbg !129
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !129
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !129
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !129
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %DOT6), !dbg !129
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !129
  call void @String.delete(%struct.String* %tmp_struct), !dbg !129
  %res7 = load %struct.String, %struct.String* %res, align 8, !dbg !129
  ret %struct.String %res7, !dbg !129
}

define %struct.String @String.operator.ADD.CHARS(i8* %str, %struct.String* %self) !dbg !130 {
entry:
  %str1 = alloca i8*, align 8, !dbg !131
  %self2 = alloca %struct.String*, align 8, !dbg !131
  %res = alloca %struct.String, align 8, !dbg !131
  store i8* %str, i8** %str1, align 8, !dbg !131
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !131
  store %struct.String zeroinitializer, %struct.String* %res, align 8, !dbg !131
  %0 = call %struct.String @String.new(), !dbg !132
  %tmp_struct = alloca %struct.String, align 8, !dbg !132
  store %struct.String %0, %struct.String* %tmp_struct, align 8, !dbg !132
  %tmp_op = alloca %struct.String, align 8, !dbg !132
  store %struct.String %0, %struct.String* %tmp_op, align 8, !dbg !132
  call void @String.operator.ASSIGN.String(%struct.String* %tmp_op, %struct.String* %res), !dbg !132
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !133
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !133
  %DOT = load i8*, i8** %value, align 8, !dbg !133
  %str4 = load i8*, i8** %str1, align 8, !dbg !133
  %strjoin = call i8* @strjoin(i8* %DOT, i8* %str4), !dbg !133
  call void @String.operator.ASSIGN.CHARS(i8* %strjoin, %struct.String* %res), !dbg !133
  call void @String.delete(%struct.String* %tmp_struct), !dbg !133
  %res5 = load %struct.String, %struct.String* %res, align 8, !dbg !133
  ret %struct.String %res5, !dbg !133
}

define void @String.operator.ADD_ASS.String(%struct.String* %v, %struct.String* %self) !dbg !134 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !135
  %self2 = alloca %struct.String*, align 8, !dbg !135
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !135
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !135
  %v3 = load %struct.String*, %struct.String** %v1, align 8, !dbg !136
  %value = getelementptr %struct.String, %struct.String* %v3, i32 0, i32 0, !dbg !136
  %DOT = load i8*, i8** %value, align 8, !dbg !136
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !136
  call void @String.join(i8* %DOT, %struct.String* %ref_arg), !dbg !136
  ret void, !dbg !136
}

define void @String.operator.ADD_ASS.CHARS(i8* %str, %struct.String* %self) !dbg !137 {
entry:
  %str1 = alloca i8*, align 8, !dbg !138
  %self2 = alloca %struct.String*, align 8, !dbg !138
  store i8* %str, i8** %str1, align 8, !dbg !138
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !138
  %str3 = load i8*, i8** %str1, align 8, !dbg !139
  %ref_arg = load %struct.String*, %struct.String** %self2, align 8, !dbg !139
  call void @String.join(i8* %str3, %struct.String* %ref_arg), !dbg !139
  ret void, !dbg !139
}

define i1 @String.operator.EQ.String(%struct.String* %v, %struct.String* %self) !dbg !140 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !141
  %self2 = alloca %struct.String*, align 8, !dbg !141
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !141
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !141
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !142
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !142
  %DOT = load i8*, i8** %value, align 8, !dbg !142
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !142
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !142
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !142
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !142
  %EQ = icmp eq i32 %strcmp, 0, !dbg !142
  ret i1 %EQ, !dbg !142
}

define i1 @String.operator.EQ.CHARS(i8* %str, %struct.String* %self) !dbg !143 {
entry:
  %str1 = alloca i8*, align 8, !dbg !144
  %self2 = alloca %struct.String*, align 8, !dbg !144
  store i8* %str, i8** %str1, align 8, !dbg !144
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !144
  br label %if.start, !dbg !144

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !144
  %EQ = icmp eq i8* %str3, null, !dbg !144
  br i1 %EQ, label %if.then, label %if.end, !dbg !144

if.end:                                           ; preds = %if.start
  %self6 = load %struct.String*, %struct.String** %self2, align 8, !dbg !145
  %value = getelementptr %struct.String, %struct.String* %self6, i32 0, i32 0, !dbg !145
  %DOT7 = load i8*, i8** %value, align 8, !dbg !145
  %str8 = load i8*, i8** %str1, align 8, !dbg !145
  %strcmp = call i32 @strcmp(i8* %DOT7, i8* %str8), !dbg !145
  %EQ9 = icmp eq i32 %strcmp, 0, !dbg !145
  ret i1 %EQ9, !dbg !145

if.then:                                          ; preds = %if.start
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !144
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !144
  %DOT = load i32, i32* %count, align 4, !dbg !144
  %EQ5 = icmp eq i32 %DOT, 0, !dbg !144
  ret i1 %EQ5, !dbg !144
}

define i1 @String.operator.NEQ.String(%struct.String* %v, %struct.String* %self) !dbg !146 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !147
  %self2 = alloca %struct.String*, align 8, !dbg !147
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !147
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !147
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !148
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !148
  %DOT = load i8*, i8** %value, align 8, !dbg !148
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !148
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !148
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !148
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !148
  %NEQ = icmp ne i32 %strcmp, 0, !dbg !148
  ret i1 %NEQ, !dbg !148
}

define i1 @String.operator.NEQ.CHARS(i8* %str, %struct.String* %self) !dbg !149 {
entry:
  %str1 = alloca i8*, align 8, !dbg !150
  %self2 = alloca %struct.String*, align 8, !dbg !150
  store i8* %str, i8** %str1, align 8, !dbg !150
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !150
  br label %if.start, !dbg !150

if.start:                                         ; preds = %entry
  %str3 = load i8*, i8** %str1, align 8, !dbg !150
  %EQ = icmp eq i8* %str3, null, !dbg !150
  br i1 %EQ, label %if.then, label %if.end, !dbg !150

if.end:                                           ; preds = %if.start
  %self5 = load %struct.String*, %struct.String** %self2, align 8, !dbg !151
  %value = getelementptr %struct.String, %struct.String* %self5, i32 0, i32 0, !dbg !151
  %DOT6 = load i8*, i8** %value, align 8, !dbg !151
  %str7 = load i8*, i8** %str1, align 8, !dbg !151
  %strcmp = call i32 @strcmp(i8* %DOT6, i8* %str7), !dbg !151
  %NEQ8 = icmp ne i32 %strcmp, 0, !dbg !151
  ret i1 %NEQ8, !dbg !151

if.then:                                          ; preds = %if.start
  %self4 = load %struct.String*, %struct.String** %self2, align 8, !dbg !150
  %count = getelementptr %struct.String, %struct.String* %self4, i32 0, i32 1, !dbg !150
  %DOT = load i32, i32* %count, align 4, !dbg !150
  %NEQ = icmp ne i32 %DOT, 0, !dbg !150
  ret i1 %NEQ, !dbg !150
}

define i1 @String.operator.LT.String(%struct.String* %v, %struct.String* %self) !dbg !152 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !153
  %self2 = alloca %struct.String*, align 8, !dbg !153
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !153
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !153
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !154
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !154
  %DOT = load i8*, i8** %value, align 8, !dbg !154
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !154
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !154
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !154
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !154
  %LT = icmp slt i32 %strcmp, 0, !dbg !154
  ret i1 %LT, !dbg !154
}

define i1 @String.operator.GT.String(%struct.String* %v, %struct.String* %self) !dbg !155 {
entry:
  %v1 = alloca %struct.String*, align 8, !dbg !156
  %self2 = alloca %struct.String*, align 8, !dbg !156
  store %struct.String* %v, %struct.String** %v1, align 8, !dbg !156
  store %struct.String* %self, %struct.String** %self2, align 8, !dbg !156
  %self3 = load %struct.String*, %struct.String** %self2, align 8, !dbg !157
  %value = getelementptr %struct.String, %struct.String* %self3, i32 0, i32 0, !dbg !157
  %DOT = load i8*, i8** %value, align 8, !dbg !157
  %v4 = load %struct.String*, %struct.String** %v1, align 8, !dbg !157
  %value5 = getelementptr %struct.String, %struct.String* %v4, i32 0, i32 0, !dbg !157
  %DOT6 = load i8*, i8** %value5, align 8, !dbg !157
  %strcmp = call i32 @strcmp(i8* %DOT, i8* %DOT6), !dbg !157
  %GT = icmp sgt i32 %strcmp, 0, !dbg !157
  ret i1 %GT, !dbg !157
}

define i8* @String.operator.output(%struct.String* %self) !dbg !158 {
entry:
  %self1 = alloca %struct.String*, align 8, !dbg !159
  store %struct.String* %self, %struct.String** %self1, align 8, !dbg !159
  %self2 = load %struct.String*, %struct.String** %self1, align 8, !dbg !159
  %value = getelementptr %struct.String, %struct.String* %self2, i32 0, i32 0, !dbg !159
  %DOT = load i8*, i8** %value, align 8, !dbg !159
  ret i8* %DOT, !dbg !159
}

declare i64 @time(i8*)

declare i8* @localtime(i64*)

declare i32 @strftime(i8*, i32, i8*, i8*)

declare i32 @socket(i32, i32, i32)

declare i32 @connect(i32, i8*, i32)

declare i32 @close(i32)

declare i32 @fork()

define void @log(i8* %msg) !dbg !160 {
entry:
  %msg1 = alloca i8*, align 8, !dbg !161
  store i8* %msg, i8** %msg1, align 8, !dbg !161
  %msg2 = load i8*, i8** %msg1, align 8, !dbg !162
  %msg3 = load i8*, i8** %msg1, align 8, !dbg !162
  %strlen = call i32 @strlen(i8* %msg3), !dbg !162
  %write = call i32 @write(i32 1, i8* %msg2, i32 %strlen), !dbg !162
  ret void, !dbg !162
}

define i8* @get_time() !dbg !163 {
entry:
  %buf = alloca i8*, align 8, !dbg !164
  %t = alloca i64, align 8, !dbg !164
  %tm = alloca i8*, align 8, !dbg !164
  store i8* null, i8** %buf, align 8, !dbg !164
  %heap = call i8* @calloc(i64 16, i64 1), !dbg !164
  store i8* %heap, i8** %buf, align 8, !dbg !164
  store i64 0, i64* %t, align 4, !dbg !164
  %time = call i64 @time(i8* null), !dbg !165
  store i64 %time, i64* %t, align 4, !dbg !165
  store i8* null, i8** %tm, align 8, !dbg !164
  %localtime = call i8* @localtime(i64* %t), !dbg !166
  store i8* %localtime, i8** %tm, align 8, !dbg !166
  %buf1 = load i8*, i8** %buf, align 8, !dbg !167
  %tm2 = load i8*, i8** %tm, align 8, !dbg !167
  %strftime = call i32 @strftime(i8* %buf1, i32 16, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR1, i32 0, i32 0), i8* %tm2), !dbg !167
  %buf3 = load i8*, i8** %buf, align 8, !dbg !167
  ret i8* %buf3, !dbg !167
}

define void @log_ts(i8* %color, i8* %tag, i8* %msg) !dbg !168 {
entry:
  %color1 = alloca i8*, align 8, !dbg !169
  %tag2 = alloca i8*, align 8, !dbg !169
  %msg3 = alloca i8*, align 8, !dbg !169
  %tbuf = alloca i8*, align 8, !dbg !169
  store i8* %color, i8** %color1, align 8, !dbg !169
  store i8* %tag, i8** %tag2, align 8, !dbg !169
  store i8* %msg, i8** %msg3, align 8, !dbg !169
  store i8* null, i8** %tbuf, align 8, !dbg !169
  %get_time = call i8* @get_time(), !dbg !170
  store i8* %get_time, i8** %tbuf, align 8, !dbg !170
  call void @log(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR2, i32 0, i32 0)), !dbg !171
  %tbuf4 = load i8*, i8** %tbuf, align 8, !dbg !172
  call void @log(i8* %tbuf4), !dbg !172
  call void @log(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR3, i32 0, i32 0)), !dbg !173
  %color5 = load i8*, i8** %color1, align 8, !dbg !174
  call void @log(i8* %color5), !dbg !174
  %tag6 = load i8*, i8** %tag2, align 8, !dbg !175
  call void @log(i8* %tag6), !dbg !175
  call void @log(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR4, i32 0, i32 0)), !dbg !176
  %msg7 = load i8*, i8** %msg3, align 8, !dbg !177
  call void @log(i8* %msg7), !dbg !177
  call void @log(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @STR5, i32 0, i32 0)), !dbg !178
  %tbuf8 = load i8*, i8** %tbuf, align 8, !dbg !179
  call void @free(i8* %tbuf8), !dbg !179
  ret void, !dbg !179
}

define void @SockAddr.init(%struct.SockAddr* %self) !dbg !180 {
entry:
  %self1 = alloca %struct.SockAddr*, align 8, !dbg !181
  store %struct.SockAddr* %self, %struct.SockAddr** %self1, align 8, !dbg !181
  %self2 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !181
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self2, i32 0, i32 0, !dbg !181
  %heap = call i8* @calloc(i64 16, i64 1), !dbg !181
  store i8* %heap, i8** %data, align 8, !dbg !181
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !182
  %data4 = getelementptr %struct.SockAddr, %struct.SockAddr* %self3, i32 0, i32 0, !dbg !182
  %DOT = load i8*, i8** %data4, align 8, !dbg !182
  %ACCESS = getelementptr i8, i8* %DOT, i32 0, !dbg !182
  store i8 16, i8* %ACCESS, align 1, !dbg !182
  %self5 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !183
  %data6 = getelementptr %struct.SockAddr, %struct.SockAddr* %self5, i32 0, i32 0, !dbg !183
  %DOT7 = load i8*, i8** %data6, align 8, !dbg !183
  %ACCESS8 = getelementptr i8, i8* %DOT7, i32 1, !dbg !183
  store i8 2, i8* %ACCESS8, align 1, !dbg !183
  ret void, !dbg !183
}

define void @SockAddr.set_port(i32 %port, %struct.SockAddr* %self) !dbg !184 {
entry:
  %port1 = alloca i32, align 4, !dbg !185
  %self2 = alloca %struct.SockAddr*, align 8, !dbg !185
  store i32 %port, i32* %port1, align 4, !dbg !185
  store %struct.SockAddr* %self, %struct.SockAddr** %self2, align 8, !dbg !185
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self2, align 8, !dbg !186
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self3, i32 0, i32 0, !dbg !186
  %DOT = load i8*, i8** %data, align 8, !dbg !186
  %ACCESS = getelementptr i8, i8* %DOT, i32 2, !dbg !186
  %port4 = load i32, i32* %port1, align 4, !dbg !186
  %RSHIFT = ashr i32 %port4, 8, !dbg !186
  %as = trunc i32 %RSHIFT to i8, !dbg !186
  store i8 %as, i8* %ACCESS, align 1, !dbg !186
  %self5 = load %struct.SockAddr*, %struct.SockAddr** %self2, align 8, !dbg !187
  %data6 = getelementptr %struct.SockAddr, %struct.SockAddr* %self5, i32 0, i32 0, !dbg !187
  %DOT7 = load i8*, i8** %data6, align 8, !dbg !187
  %ACCESS8 = getelementptr i8, i8* %DOT7, i32 3, !dbg !187
  %port9 = load i32, i32* %port1, align 4, !dbg !187
  %BAND = and i32 %port9, 255, !dbg !187
  %as10 = trunc i32 %BAND to i8, !dbg !187
  store i8 %as10, i8* %ACCESS8, align 1, !dbg !187
  ret void, !dbg !187
}

define void @SockAddr.set_ip(i32 %a, i32 %b, i32 %c, i32 %d, %struct.SockAddr* %self) !dbg !188 {
entry:
  %a1 = alloca i32, align 4, !dbg !189
  %b2 = alloca i32, align 4, !dbg !189
  %c3 = alloca i32, align 4, !dbg !189
  %d4 = alloca i32, align 4, !dbg !189
  %self5 = alloca %struct.SockAddr*, align 8, !dbg !189
  store i32 %a, i32* %a1, align 4, !dbg !189
  store i32 %b, i32* %b2, align 4, !dbg !189
  store i32 %c, i32* %c3, align 4, !dbg !189
  store i32 %d, i32* %d4, align 4, !dbg !189
  store %struct.SockAddr* %self, %struct.SockAddr** %self5, align 8, !dbg !189
  %self6 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !190
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self6, i32 0, i32 0, !dbg !190
  %DOT = load i8*, i8** %data, align 8, !dbg !190
  %ACCESS = getelementptr i8, i8* %DOT, i32 4, !dbg !190
  %a7 = load i32, i32* %a1, align 4, !dbg !190
  %as = trunc i32 %a7 to i8, !dbg !190
  store i8 %as, i8* %ACCESS, align 1, !dbg !190
  %self8 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !191
  %data9 = getelementptr %struct.SockAddr, %struct.SockAddr* %self8, i32 0, i32 0, !dbg !191
  %DOT10 = load i8*, i8** %data9, align 8, !dbg !191
  %ACCESS11 = getelementptr i8, i8* %DOT10, i32 5, !dbg !191
  %b12 = load i32, i32* %b2, align 4, !dbg !191
  %as13 = trunc i32 %b12 to i8, !dbg !191
  store i8 %as13, i8* %ACCESS11, align 1, !dbg !191
  %self14 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !192
  %data15 = getelementptr %struct.SockAddr, %struct.SockAddr* %self14, i32 0, i32 0, !dbg !192
  %DOT16 = load i8*, i8** %data15, align 8, !dbg !192
  %ACCESS17 = getelementptr i8, i8* %DOT16, i32 6, !dbg !192
  %c18 = load i32, i32* %c3, align 4, !dbg !192
  %as19 = trunc i32 %c18 to i8, !dbg !192
  store i8 %as19, i8* %ACCESS17, align 1, !dbg !192
  %self20 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !193
  %data21 = getelementptr %struct.SockAddr, %struct.SockAddr* %self20, i32 0, i32 0, !dbg !193
  %DOT22 = load i8*, i8** %data21, align 8, !dbg !193
  %ACCESS23 = getelementptr i8, i8* %DOT22, i32 7, !dbg !193
  %d24 = load i32, i32* %d4, align 4, !dbg !193
  %as25 = trunc i32 %d24 to i8, !dbg !193
  store i8 %as25, i8* %ACCESS23, align 1, !dbg !193
  ret void, !dbg !193
}

define void @SockAddr.delete(%struct.SockAddr* %self) !dbg !194 {
entry:
  %self1 = alloca %struct.SockAddr*, align 8, !dbg !195
  store %struct.SockAddr* %self, %struct.SockAddr** %self1, align 8, !dbg !195
  %self2 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !196
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self2, i32 0, i32 0, !dbg !196
  %DOT = load i8*, i8** %data, align 8, !dbg !196
  call void @free(i8* %DOT), !dbg !196
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !196
  ret void, !dbg !196
}

define void @banner() !dbg !197 {
entry:
  call void @log(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR6, i32 0, i32 0)), !dbg !198
  call void @log(i8* getelementptr inbounds ([123 x i8], [123 x i8]* @STR7, i32 0, i32 0)), !dbg !199
  call void @log(i8* getelementptr inbounds ([127 x i8], [127 x i8]* @STR8, i32 0, i32 0)), !dbg !200
  call void @log(i8* getelementptr inbounds ([103 x i8], [103 x i8]* @STR9, i32 0, i32 0)), !dbg !201
  call void @log(i8* getelementptr inbounds ([105 x i8], [105 x i8]* @STR10, i32 0, i32 0)), !dbg !202
  call void @log(i8* getelementptr inbounds ([127 x i8], [127 x i8]* @STR11, i32 0, i32 0)), !dbg !203
  call void @log(i8* getelementptr inbounds ([123 x i8], [123 x i8]* @STR12, i32 0, i32 0)), !dbg !204
  call void @log(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR13, i32 0, i32 0)), !dbg !205
  call void @log(i8* getelementptr inbounds ([148 x i8], [148 x i8]* @STR14, i32 0, i32 0)), !dbg !206
  call void @log(i8* getelementptr inbounds ([83 x i8], [83 x i8]* @STR15, i32 0, i32 0)), !dbg !207
  call void @log(i8* getelementptr inbounds ([149 x i8], [149 x i8]* @STR16, i32 0, i32 0)), !dbg !208
  ret void, !dbg !208
}

define void @Client.init(%struct.Client* %self) !dbg !209 {
entry:
  %self1 = alloca %struct.Client*, align 8, !dbg !210
  store %struct.Client* %self, %struct.Client** %self1, align 8, !dbg !210
  %self2 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !210
  %fd = getelementptr %struct.Client, %struct.Client* %self2, i32 0, i32 0, !dbg !210
  %socket = call i32 @socket(i32 2, i32 1, i32 0), !dbg !211
  store i32 %socket, i32* %fd, align 4, !dbg !211
  br label %if.start, !dbg !211

if.start:                                         ; preds = %entry
  %self3 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !211
  %fd4 = getelementptr %struct.Client, %struct.Client* %self3, i32 0, i32 0, !dbg !211
  %DOT = load i32, i32* %fd4, align 4, !dbg !211
  %LT = icmp slt i32 %DOT, 0, !dbg !211
  br i1 %LT, label %if.then, label %if.end, !dbg !211

if.end:                                           ; preds = %if.then, %if.start
  %self5 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !212
  %buf = getelementptr %struct.Client, %struct.Client* %self5, i32 0, i32 1, !dbg !212
  %heap = call i8* @calloc(i64 1024, i64 1), !dbg !212
  store i8* %heap, i8** %buf, align 8, !dbg !212
  %self6 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !212
  %rbuf = getelementptr %struct.Client, %struct.Client* %self6, i32 0, i32 2, !dbg !212
  %heap7 = call i8* @calloc(i64 1024, i64 1), !dbg !212
  store i8* %heap7, i8** %rbuf, align 8, !dbg !212
  ret void, !dbg !212

if.then:                                          ; preds = %if.start
  call void @log(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @STR17, i32 0, i32 0)), !dbg !213
  call void @exit(i32 1), !dbg !212
  br label %if.end, !dbg !212
}

define void @Client.delete(%struct.Client* %self) !dbg !214 {
entry:
  %self1 = alloca %struct.Client*, align 8, !dbg !215
  store %struct.Client* %self, %struct.Client** %self1, align 8, !dbg !215
  %self2 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !216
  %fd = getelementptr %struct.Client, %struct.Client* %self2, i32 0, i32 0, !dbg !216
  %DOT = load i32, i32* %fd, align 4, !dbg !216
  %close = call i32 @close(i32 %DOT), !dbg !216
  %self3 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !217
  %buf = getelementptr %struct.Client, %struct.Client* %self3, i32 0, i32 1, !dbg !217
  %DOT4 = load i8*, i8** %buf, align 8, !dbg !217
  call void @free(i8* %DOT4), !dbg !217
  %self5 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !218
  %rbuf = getelementptr %struct.Client, %struct.Client* %self5, i32 0, i32 2, !dbg !218
  %DOT6 = load i8*, i8** %rbuf, align 8, !dbg !218
  call void @free(i8* %DOT6), !dbg !218
  %self7 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !218
  ret void, !dbg !218
}

define i32 @main() !dbg !219 {
entry:
  %addr = alloca %struct.SockAddr, align 8, !dbg !220
  %cli = alloca %struct.Client, align 8, !dbg !220
  %r = alloca i32, align 4, !dbg !220
  %pid = alloca i32, align 4, !dbg !220
  call void @banner(), !dbg !220
  store %struct.SockAddr zeroinitializer, %struct.SockAddr* %addr, align 8, !dbg !220
  call void @SockAddr.set_port(i32 17000, %struct.SockAddr* %addr), !dbg !221
  call void @SockAddr.set_ip(i32 127, i32 0, i32 0, i32 1, %struct.SockAddr* %addr), !dbg !222
  store %struct.Client zeroinitializer, %struct.Client* %cli, align 8, !dbg !220
  br label %if.start, !dbg !220

if.start:                                         ; preds = %entry
  %fd = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !223
  %DOT = load i32, i32* %fd, align 4, !dbg !223
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %addr, i32 0, i32 0, !dbg !223
  %DOT1 = load i8*, i8** %data, align 8, !dbg !223
  %connect = call i32 @connect(i32 %DOT, i8* %DOT1, i32 16), !dbg !223
  %LT = icmp slt i32 %connect, 0, !dbg !223
  br i1 %LT, label %if.then, label %if.end, !dbg !223

if.end:                                           ; preds = %if.start
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR21, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR22, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR23, i32 0, i32 0)), !dbg !224
  store i32 0, i32* %r, align 4, !dbg !220
  store i32 0, i32* %r, align 4, !dbg !220
  store i32 0, i32* %pid, align 4, !dbg !220
  %fork = call i32 @fork(), !dbg !225
  store i32 %fork, i32* %pid, align 4, !dbg !225
  br label %if.start2, !dbg !225

if.then:                                          ; preds = %if.start
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR18, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR19, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @STR20, i32 0, i32 0)), !dbg !226
  ret i32 1, !dbg !226

if.start2:                                        ; preds = %if.end
  %pid5 = load i32, i32* %pid, align 4, !dbg !225
  %EQ = icmp eq i32 %pid5, 0, !dbg !225
  br i1 %EQ, label %if.then4, label %if.end3, !dbg !225

if.end3:                                          ; preds = %if.start2
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR31, i32 0, i32 0)), !dbg !227
  br label %while.start18, !dbg !227

if.then4:                                         ; preds = %if.start2
  br label %while.start, !dbg !225

while.start:                                      ; preds = %if.end10, %if.then4
  br i1 true, label %while.then, label %while.end, !dbg !225

while.then:                                       ; preds = %while.start
  %fd6 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !228
  %DOT7 = load i32, i32* %fd6, align 4, !dbg !228
  %rbuf = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !228
  %DOT8 = load i8*, i8** %rbuf, align 8, !dbg !228
  %read = call i32 @read(i32 %DOT7, i8* %DOT8, i32 1023), !dbg !228
  store i32 %read, i32* %r, align 4, !dbg !228
  br label %if.start9, !dbg !228

while.end:                                        ; preds = %if.then11, %while.start
  ret i32 0, !dbg !229

if.start9:                                        ; preds = %while.then
  %r12 = load i32, i32* %r, align 4, !dbg !228
  %LE = icmp sle i32 %r12, 0, !dbg !228
  br i1 %LE, label %if.then11, label %if.end10, !dbg !228

if.end10:                                         ; preds = %if.start9
  %rbuf13 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !230
  %DOT14 = load i8*, i8** %rbuf13, align 8, !dbg !230
  %r15 = load i32, i32* %r, align 4, !dbg !230
  %ACCESS = getelementptr i8, i8* %DOT14, i32 %r15, !dbg !230
  store i8 0, i8* %ACCESS, align 1, !dbg !230
  call void @log(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR27, i32 0, i32 0)), !dbg !231
  %rbuf16 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !232
  %DOT17 = load i8*, i8** %rbuf16, align 8, !dbg !232
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR28, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR29, i32 0, i32 0), i8* %DOT17), !dbg !232
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR30, i32 0, i32 0)), !dbg !229
  br label %while.start, !dbg !229

if.then11:                                        ; preds = %if.start9
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR24, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR25, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR26, i32 0, i32 0)), !dbg !233
  br label %while.end, !dbg !233

while.start18:                                    ; preds = %if.end39, %if.end3
  br i1 true, label %while.then19, label %while.end20, !dbg !227

while.then19:                                     ; preds = %while.start18
  %buf = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !234
  %DOT21 = load i8*, i8** %buf, align 8, !dbg !234
  %read22 = call i32 @read(i32 0, i8* %DOT21, i32 1023), !dbg !234
  store i32 %read22, i32* %r, align 4, !dbg !234
  br label %if.start23, !dbg !234

while.end20:                                      ; preds = %if.then40, %if.then25, %while.start18
  call void @SockAddr.delete(%struct.SockAddr* %addr), !dbg !235
  call void @Client.delete(%struct.Client* %cli), !dbg !235
  ret i32 0, !dbg !235

if.start23:                                       ; preds = %while.then19
  %r26 = load i32, i32* %r, align 4, !dbg !234
  %LE27 = icmp sle i32 %r26, 0, !dbg !234
  br i1 %LE27, label %if.then25, label %if.end24, !dbg !234

if.end24:                                         ; preds = %if.start23
  %buf28 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !236
  %r29 = load i32, i32* %r, align 4, !dbg !236
  %SUB = sub i32 %r29, 1, !dbg !236
  %DOT30 = load i8*, i8** %buf28, align 8, !dbg !236
  %ACCESS31 = getelementptr i8, i8* %DOT30, i32 %SUB, !dbg !236
  store i8 0, i8* %ACCESS31, align 1, !dbg !236
  %fd32 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !237
  %DOT33 = load i32, i32* %fd32, align 4, !dbg !237
  %buf34 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !237
  %DOT35 = load i8*, i8** %buf34, align 8, !dbg !237
  %r36 = load i32, i32* %r, align 4, !dbg !237
  %SUB37 = sub i32 %r36, 1, !dbg !237
  %write = call i32 @write(i32 %DOT33, i8* %DOT35, i32 %SUB37), !dbg !237
  br label %if.start38, !dbg !237

if.then25:                                        ; preds = %if.start23
  br label %while.end20, !dbg !234

if.start38:                                       ; preds = %if.end24
  %buf41 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !238
  %DOT42 = load i8*, i8** %buf41, align 8, !dbg !238
  %strcmp = call i32 @strcmp(i8* %DOT42, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR32, i32 0, i32 0)), !dbg !238
  %EQ43 = icmp eq i32 %strcmp, 0, !dbg !238
  br i1 %EQ43, label %if.then40, label %if.end39, !dbg !238

if.end39:                                         ; preds = %if.start38
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR33, i32 0, i32 0)), !dbg !235
  br label %while.start18, !dbg !235

if.then40:                                        ; preds = %if.start38
  br label %while.end20, !dbg !238
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "client.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd")
!4 = distinct !DISubprogram(name: "strjoin", linkageName: "strjoin", scope: null, file: !3, line: 58, type: !5, scopeLine: 58, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 58, scope: !4)
!8 = !DILocation(line: 61, scope: !4)
!9 = !DILocation(line: 63, scope: !4)
!10 = !DILocation(line: 62, scope: !4)
!11 = !DILocation(line: 66, scope: !4)
!12 = !DILocation(line: 70, scope: !4)
!13 = distinct !DISubprogram(name: "String.delete", linkageName: "String.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 0, scope: !13)
!15 = !DILocation(line: 82, scope: !13)
!16 = distinct !DISubprogram(name: "String.assign", linkageName: "String.assign", scope: null, file: !3, line: 86, type: !5, scopeLine: 86, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 86, scope: !16)
!18 = !DILocation(line: 88, scope: !16)
!19 = !DILocation(line: 95, scope: !16)
!20 = !DILocation(line: 96, scope: !16)
!21 = !DILocation(line: 94, scope: !16)
!22 = distinct !DISubprogram(name: "String.join", linkageName: "String.join", scope: null, file: !3, line: 99, type: !5, scopeLine: 99, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!23 = !DILocation(line: 99, scope: !22)
!24 = !DILocation(line: 101, scope: !22)
!25 = !DILocation(line: 102, scope: !22)
!26 = !DILocation(line: 103, scope: !22)
!27 = distinct !DISubprogram(name: "String.clear", linkageName: "String.clear", scope: null, file: !3, line: 105, type: !5, scopeLine: 105, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!28 = !DILocation(line: 105, scope: !27)
!29 = !DILocation(line: 106, scope: !27)
!30 = distinct !DISubprogram(name: "String.push", linkageName: "String.push", scope: null, file: !3, line: 109, type: !5, scopeLine: 109, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!31 = !DILocation(line: 109, scope: !30)
!32 = !DILocation(line: 116, scope: !30)
!33 = !DILocation(line: 118, scope: !30)
!34 = !DILocation(line: 115, scope: !30)
!35 = distinct !DISubprogram(name: "String.pop", linkageName: "String.pop", scope: null, file: !3, line: 120, type: !5, scopeLine: 120, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!36 = !DILocation(line: 120, scope: !35)
!37 = !DILocation(line: 124, scope: !35)
!38 = !DILocation(line: 125, scope: !35)
!39 = distinct !DISubprogram(name: "String.new", linkageName: "String.new", scope: null, file: !3, line: 130, type: !5, scopeLine: 130, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!40 = !DILocation(line: 130, scope: !39)
!41 = !DILocation(line: 133, scope: !39)
!42 = distinct !DISubprogram(name: "String.from", linkageName: "String.from", scope: null, file: !3, line: 136, type: !5, scopeLine: 136, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!43 = !DILocation(line: 136, scope: !42)
!44 = !DILocation(line: 137, scope: !42)
!45 = !DILocation(line: 138, scope: !42)
!46 = distinct !DISubprogram(name: "String.from_int", linkageName: "String.from_int", scope: null, file: !3, line: 141, type: !5, scopeLine: 141, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!47 = !DILocation(line: 141, scope: !46)
!48 = !DILocation(line: 142, scope: !46)
!49 = !DILocation(line: 144, scope: !46)
!50 = !DILocation(line: 154, scope: !46)
!51 = !DILocation(line: 158, scope: !46)
!52 = !DILocation(line: 161, scope: !46)
!53 = distinct !DISubprogram(name: "String.len", linkageName: "String.len", scope: null, file: !3, line: 167, type: !5, scopeLine: 167, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!54 = !DILocation(line: 167, scope: !53)
!55 = distinct !DISubprogram(name: "String.empty", linkageName: "String.empty", scope: null, file: !3, line: 170, type: !5, scopeLine: 170, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!56 = !DILocation(line: 170, scope: !55)
!57 = distinct !DISubprogram(name: "String.c_str", linkageName: "String.c_str", scope: null, file: !3, line: 173, type: !5, scopeLine: 173, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!58 = !DILocation(line: 173, scope: !57)
!59 = distinct !DISubprogram(name: "String.at", linkageName: "String.at", scope: null, file: !3, line: 176, type: !5, scopeLine: 176, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!60 = !DILocation(line: 176, scope: !59)
!61 = !DILocation(line: 179, scope: !59)
!62 = distinct !DISubprogram(name: "String.find", linkageName: "String.find", scope: null, file: !3, line: 183, type: !5, scopeLine: 183, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!63 = !DILocation(line: 183, scope: !62)
!64 = !DILocation(line: 186, scope: !62)
!65 = !DILocation(line: 189, scope: !62)
!66 = distinct !DISubprogram(name: "String.contains", linkageName: "String.contains", scope: null, file: !3, line: 194, type: !5, scopeLine: 194, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!67 = !DILocation(line: 194, scope: !66)
!68 = !DILocation(line: 195, scope: !66)
!69 = distinct !DISubprogram(name: "String.starts_with", linkageName: "String.starts_with", scope: null, file: !3, line: 197, type: !5, scopeLine: 197, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!70 = !DILocation(line: 197, scope: !69)
!71 = !DILocation(line: 199, scope: !69)
!72 = !DILocation(line: 201, scope: !69)
!73 = distinct !DISubprogram(name: "String.ends_with", linkageName: "String.ends_with", scope: null, file: !3, line: 203, type: !5, scopeLine: 203, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!74 = !DILocation(line: 203, scope: !73)
!75 = !DILocation(line: 205, scope: !73)
!76 = !DILocation(line: 208, scope: !73)
!77 = distinct !DISubprogram(name: "String.substr", linkageName: "String.substr", scope: null, file: !3, line: 212, type: !5, scopeLine: 212, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!78 = !DILocation(line: 212, scope: !77)
!79 = !DILocation(line: 213, scope: !77)
!80 = !DILocation(line: 221, scope: !77)
!81 = distinct !DISubprogram(name: "String.upper", linkageName: "String.upper", scope: null, file: !3, line: 225, type: !5, scopeLine: 225, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!82 = !DILocation(line: 225, scope: !81)
!83 = !DILocation(line: 226, scope: !81)
!84 = !DILocation(line: 229, scope: !81)
!85 = !DILocation(line: 234, scope: !81)
!86 = !DILocation(line: 232, scope: !81)
!87 = distinct !DISubprogram(name: "String.lower", linkageName: "String.lower", scope: null, file: !3, line: 238, type: !5, scopeLine: 238, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!88 = !DILocation(line: 238, scope: !87)
!89 = !DILocation(line: 239, scope: !87)
!90 = !DILocation(line: 242, scope: !87)
!91 = !DILocation(line: 247, scope: !87)
!92 = !DILocation(line: 245, scope: !87)
!93 = distinct !DISubprogram(name: "String.trim", linkageName: "String.trim", scope: null, file: !3, line: 251, type: !5, scopeLine: 251, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!94 = !DILocation(line: 251, scope: !93)
!95 = !DILocation(line: 254, scope: !93)
!96 = !DILocation(line: 261, scope: !93)
!97 = !DILocation(line: 266, scope: !93)
!98 = distinct !DISubprogram(name: "String.replace", linkageName: "String.replace", scope: null, file: !3, line: 268, type: !5, scopeLine: 268, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!99 = !DILocation(line: 268, scope: !98)
!100 = !DILocation(line: 269, scope: !98)
!101 = !DILocation(line: 270, scope: !98)
!102 = !DILocation(line: 272, scope: !98)
!103 = !DILocation(line: 280, scope: !98)
!104 = !DILocation(line: 276, scope: !98)
!105 = !DILocation(line: 277, scope: !98)
!106 = distinct !DISubprogram(name: "String.repeat", linkageName: "String.repeat", scope: null, file: !3, line: 284, type: !5, scopeLine: 284, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!107 = !DILocation(line: 284, scope: !106)
!108 = !DILocation(line: 285, scope: !106)
!109 = !DILocation(line: 288, scope: !106)
!110 = distinct !DISubprogram(name: "String.reverse", linkageName: "String.reverse", scope: null, file: !3, line: 292, type: !5, scopeLine: 292, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!111 = !DILocation(line: 292, scope: !110)
!112 = !DILocation(line: 293, scope: !110)
!113 = !DILocation(line: 296, scope: !110)
!114 = distinct !DISubprogram(name: "String.compare", linkageName: "String.compare", scope: null, file: !3, line: 302, type: !5, scopeLine: 302, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!115 = !DILocation(line: 302, scope: !114)
!116 = !DILocation(line: 303, scope: !114)
!117 = distinct !DISubprogram(name: "String.to_int", linkageName: "String.to_int", scope: null, file: !3, line: 307, type: !5, scopeLine: 307, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!118 = !DILocation(line: 307, scope: !117)
!119 = !DILocation(line: 308, scope: !117)
!120 = distinct !DISubprogram(name: "String.operator.ASSIGN.CHARS", linkageName: "String.operator.ASSIGN.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!121 = !DILocation(line: 0, scope: !120)
!122 = !DILocation(line: 313, scope: !120)
!123 = distinct !DISubprogram(name: "String.operator.ASSIGN.String", linkageName: "String.operator.ASSIGN.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!124 = !DILocation(line: 0, scope: !123)
!125 = !DILocation(line: 316, scope: !123)
!126 = distinct !DISubprogram(name: "String.operator.ADD.String", linkageName: "String.operator.ADD.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!127 = !DILocation(line: 0, scope: !126)
!128 = !DILocation(line: 321, scope: !126)
!129 = !DILocation(line: 322, scope: !126)
!130 = distinct !DISubprogram(name: "String.operator.ADD.CHARS", linkageName: "String.operator.ADD.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!131 = !DILocation(line: 0, scope: !130)
!132 = !DILocation(line: 326, scope: !130)
!133 = !DILocation(line: 327, scope: !130)
!134 = distinct !DISubprogram(name: "String.operator.ADD_ASS.String", linkageName: "String.operator.ADD_ASS.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!135 = !DILocation(line: 0, scope: !134)
!136 = !DILocation(line: 331, scope: !134)
!137 = distinct !DISubprogram(name: "String.operator.ADD_ASS.CHARS", linkageName: "String.operator.ADD_ASS.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!138 = !DILocation(line: 0, scope: !137)
!139 = !DILocation(line: 334, scope: !137)
!140 = distinct !DISubprogram(name: "String.operator.EQ.String", linkageName: "String.operator.EQ.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!141 = !DILocation(line: 0, scope: !140)
!142 = !DILocation(line: 339, scope: !140)
!143 = distinct !DISubprogram(name: "String.operator.EQ.CHARS", linkageName: "String.operator.EQ.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!144 = !DILocation(line: 0, scope: !143)
!145 = !DILocation(line: 343, scope: !143)
!146 = distinct !DISubprogram(name: "String.operator.NEQ.String", linkageName: "String.operator.NEQ.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!147 = !DILocation(line: 0, scope: !146)
!148 = !DILocation(line: 346, scope: !146)
!149 = distinct !DISubprogram(name: "String.operator.NEQ.CHARS", linkageName: "String.operator.NEQ.CHARS", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!150 = !DILocation(line: 0, scope: !149)
!151 = !DILocation(line: 350, scope: !149)
!152 = distinct !DISubprogram(name: "String.operator.LT.String", linkageName: "String.operator.LT.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!153 = !DILocation(line: 0, scope: !152)
!154 = !DILocation(line: 353, scope: !152)
!155 = distinct !DISubprogram(name: "String.operator.GT.String", linkageName: "String.operator.GT.String", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!156 = !DILocation(line: 0, scope: !155)
!157 = !DILocation(line: 356, scope: !155)
!158 = distinct !DISubprogram(name: "String.operator.output", linkageName: "String.operator.output", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!159 = !DILocation(line: 0, scope: !158)
!160 = distinct !DISubprogram(name: "log", linkageName: "log", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!161 = !DILocation(line: 3, scope: !160)
!162 = !DILocation(line: 4, scope: !160)
!163 = distinct !DISubprogram(name: "get_time", linkageName: "get_time", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!164 = !DILocation(line: 6, scope: !163)
!165 = !DILocation(line: 8, scope: !163)
!166 = !DILocation(line: 9, scope: !163)
!167 = !DILocation(line: 10, scope: !163)
!168 = distinct !DISubprogram(name: "log_ts", linkageName: "log_ts", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!169 = !DILocation(line: 13, scope: !168)
!170 = !DILocation(line: 14, scope: !168)
!171 = !DILocation(line: 15, scope: !168)
!172 = !DILocation(line: 16, scope: !168)
!173 = !DILocation(line: 17, scope: !168)
!174 = !DILocation(line: 18, scope: !168)
!175 = !DILocation(line: 19, scope: !168)
!176 = !DILocation(line: 20, scope: !168)
!177 = !DILocation(line: 21, scope: !168)
!178 = !DILocation(line: 22, scope: !168)
!179 = !DILocation(line: 23, scope: !168)
!180 = distinct !DISubprogram(name: "SockAddr.init", linkageName: "SockAddr.init", scope: null, file: !3, line: 40, type: !5, scopeLine: 40, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!181 = !DILocation(line: 40, scope: !180)
!182 = !DILocation(line: 42, scope: !180)
!183 = !DILocation(line: 43, scope: !180)
!184 = distinct !DISubprogram(name: "SockAddr.set_port", linkageName: "SockAddr.set_port", scope: null, file: !3, line: 45, type: !5, scopeLine: 45, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!185 = !DILocation(line: 45, scope: !184)
!186 = !DILocation(line: 46, scope: !184)
!187 = !DILocation(line: 47, scope: !184)
!188 = distinct !DISubprogram(name: "SockAddr.set_ip", linkageName: "SockAddr.set_ip", scope: null, file: !3, line: 49, type: !5, scopeLine: 49, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!189 = !DILocation(line: 49, scope: !188)
!190 = !DILocation(line: 50, scope: !188)
!191 = !DILocation(line: 51, scope: !188)
!192 = !DILocation(line: 52, scope: !188)
!193 = !DILocation(line: 53, scope: !188)
!194 = distinct !DISubprogram(name: "SockAddr.delete", linkageName: "SockAddr.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!195 = !DILocation(line: 0, scope: !194)
!196 = !DILocation(line: 56, scope: !194)
!197 = distinct !DISubprogram(name: "banner", linkageName: "banner", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!198 = !DILocation(line: 5, scope: !197)
!199 = !DILocation(line: 6, scope: !197)
!200 = !DILocation(line: 7, scope: !197)
!201 = !DILocation(line: 8, scope: !197)
!202 = !DILocation(line: 9, scope: !197)
!203 = !DILocation(line: 10, scope: !197)
!204 = !DILocation(line: 11, scope: !197)
!205 = !DILocation(line: 12, scope: !197)
!206 = !DILocation(line: 13, scope: !197)
!207 = !DILocation(line: 14, scope: !197)
!208 = !DILocation(line: 15, scope: !197)
!209 = distinct !DISubprogram(name: "Client.init", linkageName: "Client.init", scope: null, file: !3, line: 22, type: !5, scopeLine: 22, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!210 = !DILocation(line: 22, scope: !209)
!211 = !DILocation(line: 23, scope: !209)
!212 = !DILocation(line: 26, scope: !209)
!213 = !DILocation(line: 25, scope: !209)
!214 = distinct !DISubprogram(name: "Client.delete", linkageName: "Client.delete", scope: null, file: !3, type: !5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!215 = !DILocation(line: 0, scope: !214)
!216 = !DILocation(line: 31, scope: !214)
!217 = !DILocation(line: 32, scope: !214)
!218 = !DILocation(line: 33, scope: !214)
!219 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 35, type: !5, scopeLine: 35, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!220 = !DILocation(line: 36, scope: !219)
!221 = !DILocation(line: 39, scope: !219)
!222 = !DILocation(line: 40, scope: !219)
!223 = !DILocation(line: 44, scope: !219)
!224 = !DILocation(line: 48, scope: !219)
!225 = !DILocation(line: 51, scope: !219)
!226 = !DILocation(line: 45, scope: !219)
!227 = !DILocation(line: 66, scope: !219)
!228 = !DILocation(line: 55, scope: !219)
!229 = !DILocation(line: 63, scope: !219)
!230 = !DILocation(line: 60, scope: !219)
!231 = !DILocation(line: 61, scope: !219)
!232 = !DILocation(line: 62, scope: !219)
!233 = !DILocation(line: 57, scope: !219)
!234 = !DILocation(line: 68, scope: !219)
!235 = !DILocation(line: 78, scope: !219)
!236 = !DILocation(line: 72, scope: !219)
!237 = !DILocation(line: 73, scope: !219)
!238 = !DILocation(line: 75, scope: !219)
