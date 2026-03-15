; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd/client.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd/client.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.SockAddr = type { i8* }
%struct.Client = type { i32, i8*, i8* }

@STR0 = private unnamed_addr constant [9 x i8] c"%H:%M:%S\00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c"\1B[0;90m[\00", align 1
@STR2 = private unnamed_addr constant [7 x i8] c"]\1B[0m \00", align 1
@STR3 = private unnamed_addr constant [6 x i8] c"\1B[0m \00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@STR5 = private unnamed_addr constant [8 x i8] c"\1B[0;36m\00", align 1
@STR6 = private unnamed_addr constant [123 x i8] c"  \E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\97     \E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\96\88\E2\95\97   \E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\0A\00", align 1
@STR7 = private unnamed_addr constant [127 x i8] c" \E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97  \E2\96\88\E2\96\88\E2\95\91\E2\95\9A\E2\95\90\E2\95\90\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\9D\0A\00", align 1
@STR8 = private unnamed_addr constant [103 x i8] c" \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97  \E2\96\88\E2\96\88\E2\95\94\E2\96\88\E2\96\88\E2\95\97 \E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR9 = private unnamed_addr constant [105 x i8] c" \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\9D  \E2\96\88\E2\96\88\E2\95\91\E2\95\9A\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR10 = private unnamed_addr constant [127 x i8] c" \E2\95\9A\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\91 \E2\95\9A\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR11 = private unnamed_addr constant [123 x i8] c"  \E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\9D  \E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\9D   \E2\95\9A\E2\95\90\E2\95\9D   \0A\00", align 1
@STR12 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@STR13 = private unnamed_addr constant [148 x i8] c"\1B[0;90m\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\1B[0m\0A\00", align 1
@STR14 = private unnamed_addr constant [83 x i8] c"\1B[0;36m  URA Chat Client  \1B[0;90m\E2\94\82\1B[0m connecting to \1B[0;33m127.0.0.1:17000\1B[0m\0A\00", align 1
@STR15 = private unnamed_addr constant [149 x i8] c"\1B[0;90m\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\1B[0m\0A\0A\00", align 1
@STR16 = private unnamed_addr constant [32 x i8] c"failed to create client socket\0A\00", align 1
@STR17 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR18 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR19 = private unnamed_addr constant [15 x i8] c"connect failed\00", align 1
@STR20 = private unnamed_addr constant [8 x i8] c"\1B[0;36m\00", align 1
@STR21 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR22 = private unnamed_addr constant [20 x i8] c"connected to server\00", align 1
@STR23 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR24 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR25 = private unnamed_addr constant [20 x i8] c"server disconnected\00", align 1
@STR26 = private unnamed_addr constant [6 x i8] c"\0D\1B[2K\00", align 1
@STR27 = private unnamed_addr constant [8 x i8] c"\1B[0;33m\00", align 1
@STR28 = private unnamed_addr constant [9 x i8] c"[server]\00", align 1
@STR29 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1
@STR30 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1
@STR31 = private unnamed_addr constant [6 x i8] c"/exit\00", align 1
@STR32 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1

declare i32 @write(i32, i8*, i32)

declare i32 @read(i32, i8*, i32)

declare void @free(i8*)

declare void @exit(i32)

declare i32 @strlen(i8*)

declare i32 @strcmp(i8*, i8*)

declare i64 @time(i8*)

declare i8* @localtime(i64*)

declare i32 @strftime(i8*, i32, i8*, i8*)

declare i32 @socket(i32, i32, i32)

declare i32 @connect(i32, i8*, i32)

declare i32 @close(i32)

declare i32 @fork()

define void @log(i8* %msg) !dbg !4 {
entry:
  %msg1 = alloca i8*, align 8, !dbg !7
  store i8* %msg, i8** %msg1, align 8, !dbg !7
  %msg2 = load i8*, i8** %msg1, align 8, !dbg !8
  %msg3 = load i8*, i8** %msg1, align 8, !dbg !8
  %strlen = call i32 @strlen(i8* %msg3), !dbg !8
  %write = call i32 @write(i32 1, i8* %msg2, i32 %strlen), !dbg !8
  ret void, !dbg !8
}

define i8* @get_time() !dbg !9 {
entry:
  %buf = alloca i8*, align 8, !dbg !10
  %t = alloca i64, align 8, !dbg !10
  %tm = alloca i8*, align 8, !dbg !10
  store i8* null, i8** %buf, align 8, !dbg !10
  %heap = call i8* @calloc(i64 16, i64 1), !dbg !10
  store i8* %heap, i8** %buf, align 8, !dbg !10
  store i64 0, i64* %t, align 4, !dbg !10
  %time = call i64 @time(i8* null), !dbg !11
  store i64 %time, i64* %t, align 4, !dbg !11
  store i8* null, i8** %tm, align 8, !dbg !10
  %localtime = call i8* @localtime(i64* %t), !dbg !12
  store i8* %localtime, i8** %tm, align 8, !dbg !12
  %buf1 = load i8*, i8** %buf, align 8, !dbg !13
  %tm2 = load i8*, i8** %tm, align 8, !dbg !13
  %strftime = call i32 @strftime(i8* %buf1, i32 16, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR0, i32 0, i32 0), i8* %tm2), !dbg !13
  %buf3 = load i8*, i8** %buf, align 8, !dbg !13
  ret i8* %buf3, !dbg !13
}

declare i8* @calloc(i64, i64)

define void @log_ts(i8* %color, i8* %tag, i8* %msg) !dbg !14 {
entry:
  %color1 = alloca i8*, align 8, !dbg !15
  %tag2 = alloca i8*, align 8, !dbg !15
  %msg3 = alloca i8*, align 8, !dbg !15
  %tbuf = alloca i8*, align 8, !dbg !15
  store i8* %color, i8** %color1, align 8, !dbg !15
  store i8* %tag, i8** %tag2, align 8, !dbg !15
  store i8* %msg, i8** %msg3, align 8, !dbg !15
  store i8* null, i8** %tbuf, align 8, !dbg !15
  %get_time = call i8* @get_time(), !dbg !16
  store i8* %get_time, i8** %tbuf, align 8, !dbg !16
  call void @log(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR1, i32 0, i32 0)), !dbg !17
  %tbuf4 = load i8*, i8** %tbuf, align 8, !dbg !18
  call void @log(i8* %tbuf4), !dbg !18
  call void @log(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR2, i32 0, i32 0)), !dbg !19
  %color5 = load i8*, i8** %color1, align 8, !dbg !20
  call void @log(i8* %color5), !dbg !20
  %tag6 = load i8*, i8** %tag2, align 8, !dbg !21
  call void @log(i8* %tag6), !dbg !21
  call void @log(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR3, i32 0, i32 0)), !dbg !22
  %msg7 = load i8*, i8** %msg3, align 8, !dbg !23
  call void @log(i8* %msg7), !dbg !23
  call void @log(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @STR4, i32 0, i32 0)), !dbg !24
  %tbuf8 = load i8*, i8** %tbuf, align 8, !dbg !25
  call void @free(i8* %tbuf8), !dbg !25
  ret void, !dbg !25
}

define void @SockAddr.init(%struct.SockAddr* %self) !dbg !26 {
entry:
  %self1 = alloca %struct.SockAddr*, align 8, !dbg !27
  store %struct.SockAddr* %self, %struct.SockAddr** %self1, align 8, !dbg !27
  %self2 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !27
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !27
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self3, i32 0, i32 0, !dbg !27
  %heap = call i8* @calloc(i64 16, i64 1), !dbg !27
  store i8* %heap, i8** %data, align 8, !dbg !27
  %self4 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !28
  %data5 = getelementptr %struct.SockAddr, %struct.SockAddr* %self4, i32 0, i32 0, !dbg !28
  %DOT = load i8*, i8** %data5, align 8, !dbg !28
  %ACCESS = getelementptr i8, i8* %DOT, i32 0, !dbg !28
  store i8 16, i8* %ACCESS, align 1, !dbg !28
  %self6 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !29
  %data7 = getelementptr %struct.SockAddr, %struct.SockAddr* %self6, i32 0, i32 0, !dbg !29
  %DOT8 = load i8*, i8** %data7, align 8, !dbg !29
  %ACCESS9 = getelementptr i8, i8* %DOT8, i32 1, !dbg !29
  store i8 2, i8* %ACCESS9, align 1, !dbg !29
  ret void, !dbg !29
}

define void @SockAddr.set_port(i32 %port, %struct.SockAddr* %self) !dbg !30 {
entry:
  %port1 = alloca i32, align 4, !dbg !31
  %self2 = alloca %struct.SockAddr*, align 8, !dbg !31
  store i32 %port, i32* %port1, align 4, !dbg !31
  store %struct.SockAddr* %self, %struct.SockAddr** %self2, align 8, !dbg !31
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self2, align 8, !dbg !32
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self3, i32 0, i32 0, !dbg !32
  %DOT = load i8*, i8** %data, align 8, !dbg !32
  %ACCESS = getelementptr i8, i8* %DOT, i32 2, !dbg !32
  %port4 = load i32, i32* %port1, align 4, !dbg !32
  %RSHIFT = ashr i32 %port4, 8, !dbg !32
  %as = trunc i32 %RSHIFT to i8, !dbg !32
  store i8 %as, i8* %ACCESS, align 1, !dbg !32
  %self5 = load %struct.SockAddr*, %struct.SockAddr** %self2, align 8, !dbg !33
  %data6 = getelementptr %struct.SockAddr, %struct.SockAddr* %self5, i32 0, i32 0, !dbg !33
  %DOT7 = load i8*, i8** %data6, align 8, !dbg !33
  %ACCESS8 = getelementptr i8, i8* %DOT7, i32 3, !dbg !33
  %port9 = load i32, i32* %port1, align 4, !dbg !33
  %BAND = and i32 %port9, 255, !dbg !33
  %as10 = trunc i32 %BAND to i8, !dbg !33
  store i8 %as10, i8* %ACCESS8, align 1, !dbg !33
  ret void, !dbg !33
}

define void @SockAddr.set_ip(i32 %a, i32 %b, i32 %c, i32 %d, %struct.SockAddr* %self) !dbg !34 {
entry:
  %a1 = alloca i32, align 4, !dbg !35
  %b2 = alloca i32, align 4, !dbg !35
  %c3 = alloca i32, align 4, !dbg !35
  %d4 = alloca i32, align 4, !dbg !35
  %self5 = alloca %struct.SockAddr*, align 8, !dbg !35
  store i32 %a, i32* %a1, align 4, !dbg !35
  store i32 %b, i32* %b2, align 4, !dbg !35
  store i32 %c, i32* %c3, align 4, !dbg !35
  store i32 %d, i32* %d4, align 4, !dbg !35
  store %struct.SockAddr* %self, %struct.SockAddr** %self5, align 8, !dbg !35
  %self6 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !36
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self6, i32 0, i32 0, !dbg !36
  %DOT = load i8*, i8** %data, align 8, !dbg !36
  %ACCESS = getelementptr i8, i8* %DOT, i32 4, !dbg !36
  %a7 = load i32, i32* %a1, align 4, !dbg !36
  %as = trunc i32 %a7 to i8, !dbg !36
  store i8 %as, i8* %ACCESS, align 1, !dbg !36
  %self8 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !37
  %data9 = getelementptr %struct.SockAddr, %struct.SockAddr* %self8, i32 0, i32 0, !dbg !37
  %DOT10 = load i8*, i8** %data9, align 8, !dbg !37
  %ACCESS11 = getelementptr i8, i8* %DOT10, i32 5, !dbg !37
  %b12 = load i32, i32* %b2, align 4, !dbg !37
  %as13 = trunc i32 %b12 to i8, !dbg !37
  store i8 %as13, i8* %ACCESS11, align 1, !dbg !37
  %self14 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !38
  %data15 = getelementptr %struct.SockAddr, %struct.SockAddr* %self14, i32 0, i32 0, !dbg !38
  %DOT16 = load i8*, i8** %data15, align 8, !dbg !38
  %ACCESS17 = getelementptr i8, i8* %DOT16, i32 6, !dbg !38
  %c18 = load i32, i32* %c3, align 4, !dbg !38
  %as19 = trunc i32 %c18 to i8, !dbg !38
  store i8 %as19, i8* %ACCESS17, align 1, !dbg !38
  %self20 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !39
  %data21 = getelementptr %struct.SockAddr, %struct.SockAddr* %self20, i32 0, i32 0, !dbg !39
  %DOT22 = load i8*, i8** %data21, align 8, !dbg !39
  %ACCESS23 = getelementptr i8, i8* %DOT22, i32 7, !dbg !39
  %d24 = load i32, i32* %d4, align 4, !dbg !39
  %as25 = trunc i32 %d24 to i8, !dbg !39
  store i8 %as25, i8* %ACCESS23, align 1, !dbg !39
  ret void, !dbg !39
}

define void @SockAddr.clean(%struct.SockAddr* %self) !dbg !40 {
entry:
  %self1 = alloca %struct.SockAddr*, align 8, !dbg !41
  store %struct.SockAddr* %self, %struct.SockAddr** %self1, align 8, !dbg !41
  %self2 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !42
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self2, i32 0, i32 0, !dbg !42
  %DOT = load i8*, i8** %data, align 8, !dbg !42
  call void @free(i8* %DOT), !dbg !42
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !42
  ret void, !dbg !42
}

define void @banner() !dbg !43 {
entry:
  call void @log(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR5, i32 0, i32 0)), !dbg !44
  call void @log(i8* getelementptr inbounds ([123 x i8], [123 x i8]* @STR6, i32 0, i32 0)), !dbg !45
  call void @log(i8* getelementptr inbounds ([127 x i8], [127 x i8]* @STR7, i32 0, i32 0)), !dbg !46
  call void @log(i8* getelementptr inbounds ([103 x i8], [103 x i8]* @STR8, i32 0, i32 0)), !dbg !47
  call void @log(i8* getelementptr inbounds ([105 x i8], [105 x i8]* @STR9, i32 0, i32 0)), !dbg !48
  call void @log(i8* getelementptr inbounds ([127 x i8], [127 x i8]* @STR10, i32 0, i32 0)), !dbg !49
  call void @log(i8* getelementptr inbounds ([123 x i8], [123 x i8]* @STR11, i32 0, i32 0)), !dbg !50
  call void @log(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR12, i32 0, i32 0)), !dbg !51
  call void @log(i8* getelementptr inbounds ([148 x i8], [148 x i8]* @STR13, i32 0, i32 0)), !dbg !52
  call void @log(i8* getelementptr inbounds ([83 x i8], [83 x i8]* @STR14, i32 0, i32 0)), !dbg !53
  call void @log(i8* getelementptr inbounds ([149 x i8], [149 x i8]* @STR15, i32 0, i32 0)), !dbg !54
  ret void, !dbg !54
}

define void @Client.init(%struct.Client* %self) !dbg !55 {
entry:
  %self1 = alloca %struct.Client*, align 8, !dbg !56
  store %struct.Client* %self, %struct.Client** %self1, align 8, !dbg !56
  %self2 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !56
  %self3 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !56
  %fd = getelementptr %struct.Client, %struct.Client* %self3, i32 0, i32 0, !dbg !56
  %socket = call i32 @socket(i32 2, i32 1, i32 0), !dbg !57
  store i32 %socket, i32* %fd, align 4, !dbg !57
  br label %if.start, !dbg !57

if.start:                                         ; preds = %entry
  %self4 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !57
  %fd5 = getelementptr %struct.Client, %struct.Client* %self4, i32 0, i32 0, !dbg !57
  %DOT = load i32, i32* %fd5, align 4, !dbg !57
  %LT = icmp slt i32 %DOT, 0, !dbg !57
  br i1 %LT, label %if.then, label %if.end, !dbg !57

if.end:                                           ; preds = %if.then, %if.start
  %self6 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !58
  %buf = getelementptr %struct.Client, %struct.Client* %self6, i32 0, i32 1, !dbg !58
  %heap = call i8* @calloc(i64 1024, i64 1), !dbg !58
  store i8* %heap, i8** %buf, align 8, !dbg !58
  %self7 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !58
  %rbuf = getelementptr %struct.Client, %struct.Client* %self7, i32 0, i32 2, !dbg !58
  %heap8 = call i8* @calloc(i64 1024, i64 1), !dbg !58
  store i8* %heap8, i8** %rbuf, align 8, !dbg !58
  ret void, !dbg !58

if.then:                                          ; preds = %if.start
  call void @log(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @STR16, i32 0, i32 0)), !dbg !59
  call void @exit(i32 1), !dbg !58
  br label %if.end, !dbg !58
}

define void @Client.clean(%struct.Client* %self) !dbg !60 {
entry:
  %self1 = alloca %struct.Client*, align 8, !dbg !61
  store %struct.Client* %self, %struct.Client** %self1, align 8, !dbg !61
  %self2 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !62
  %fd = getelementptr %struct.Client, %struct.Client* %self2, i32 0, i32 0, !dbg !62
  %DOT = load i32, i32* %fd, align 4, !dbg !62
  %close = call i32 @close(i32 %DOT), !dbg !62
  %self3 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !63
  %buf = getelementptr %struct.Client, %struct.Client* %self3, i32 0, i32 1, !dbg !63
  %DOT4 = load i8*, i8** %buf, align 8, !dbg !63
  call void @free(i8* %DOT4), !dbg !63
  %self5 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !64
  %rbuf = getelementptr %struct.Client, %struct.Client* %self5, i32 0, i32 2, !dbg !64
  %DOT6 = load i8*, i8** %rbuf, align 8, !dbg !64
  call void @free(i8* %DOT6), !dbg !64
  %self7 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !64
  ret void, !dbg !64
}

define i32 @main() !dbg !65 {
entry:
  %addr = alloca %struct.SockAddr, align 8, !dbg !66
  %cli = alloca %struct.Client, align 8, !dbg !66
  %r = alloca i32, align 4, !dbg !66
  %pid = alloca i32, align 4, !dbg !66
  call void @banner(), !dbg !66
  store %struct.SockAddr zeroinitializer, %struct.SockAddr* %addr, align 8, !dbg !66
  call void @SockAddr.init(%struct.SockAddr* %addr), !dbg !66
  call void @SockAddr.set_port(i32 17000, %struct.SockAddr* %addr), !dbg !67
  call void @SockAddr.set_ip(i32 127, i32 0, i32 0, i32 1, %struct.SockAddr* %addr), !dbg !68
  store %struct.Client zeroinitializer, %struct.Client* %cli, align 8, !dbg !66
  call void @Client.init(%struct.Client* %cli), !dbg !66
  br label %if.start, !dbg !66

if.start:                                         ; preds = %entry
  %fd = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !69
  %DOT = load i32, i32* %fd, align 4, !dbg !69
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %addr, i32 0, i32 0, !dbg !69
  %DOT1 = load i8*, i8** %data, align 8, !dbg !69
  %connect = call i32 @connect(i32 %DOT, i8* %DOT1, i32 16), !dbg !69
  %LT = icmp slt i32 %connect, 0, !dbg !69
  br i1 %LT, label %if.then, label %if.end, !dbg !69

if.end:                                           ; preds = %if.start
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR20, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR21, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR22, i32 0, i32 0)), !dbg !70
  store i32 0, i32* %r, align 4, !dbg !66
  store i32 0, i32* %r, align 4, !dbg !66
  store i32 0, i32* %pid, align 4, !dbg !66
  %fork = call i32 @fork(), !dbg !71
  store i32 %fork, i32* %pid, align 4, !dbg !71
  br label %if.start2, !dbg !71

if.then:                                          ; preds = %if.start
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR17, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR18, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @STR19, i32 0, i32 0)), !dbg !72
  ret i32 1, !dbg !72

if.start2:                                        ; preds = %if.end
  %pid5 = load i32, i32* %pid, align 4, !dbg !71
  %EQ = icmp eq i32 %pid5, 0, !dbg !71
  br i1 %EQ, label %if.then4, label %if.end3, !dbg !71

if.end3:                                          ; preds = %if.start2
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR30, i32 0, i32 0)), !dbg !73
  br label %while.start18, !dbg !73

if.then4:                                         ; preds = %if.start2
  br label %while.start, !dbg !71

while.start:                                      ; preds = %if.end10, %if.then4
  br i1 true, label %while.then, label %while.end, !dbg !71

while.then:                                       ; preds = %while.start
  %fd6 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !74
  %DOT7 = load i32, i32* %fd6, align 4, !dbg !74
  %rbuf = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !74
  %DOT8 = load i8*, i8** %rbuf, align 8, !dbg !74
  %read = call i32 @read(i32 %DOT7, i8* %DOT8, i32 1023), !dbg !74
  store i32 %read, i32* %r, align 4, !dbg !74
  br label %if.start9, !dbg !74

while.end:                                        ; preds = %if.then11, %while.start
  ret i32 0, !dbg !75

if.start9:                                        ; preds = %while.then
  %r12 = load i32, i32* %r, align 4, !dbg !74
  %LE = icmp sle i32 %r12, 0, !dbg !74
  br i1 %LE, label %if.then11, label %if.end10, !dbg !74

if.end10:                                         ; preds = %if.start9
  %rbuf13 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !76
  %DOT14 = load i8*, i8** %rbuf13, align 8, !dbg !76
  %r15 = load i32, i32* %r, align 4, !dbg !76
  %ACCESS = getelementptr i8, i8* %DOT14, i32 %r15, !dbg !76
  store i8 0, i8* %ACCESS, align 1, !dbg !76
  call void @log(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR26, i32 0, i32 0)), !dbg !77
  %rbuf16 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !78
  %DOT17 = load i8*, i8** %rbuf16, align 8, !dbg !78
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR27, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR28, i32 0, i32 0), i8* %DOT17), !dbg !78
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR29, i32 0, i32 0)), !dbg !75
  br label %while.start, !dbg !75

if.then11:                                        ; preds = %if.start9
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR23, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR24, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR25, i32 0, i32 0)), !dbg !79
  br label %while.end, !dbg !79

while.start18:                                    ; preds = %if.end39, %if.end3
  br i1 true, label %while.then19, label %while.end20, !dbg !73

while.then19:                                     ; preds = %while.start18
  %buf = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !80
  %DOT21 = load i8*, i8** %buf, align 8, !dbg !80
  %read22 = call i32 @read(i32 0, i8* %DOT21, i32 1023), !dbg !80
  store i32 %read22, i32* %r, align 4, !dbg !80
  br label %if.start23, !dbg !80

while.end20:                                      ; preds = %if.then40, %if.then25, %while.start18
  call void @Client.clean(%struct.Client* %cli), !dbg !81
  call void @SockAddr.clean(%struct.SockAddr* %addr), !dbg !82
  call void @SockAddr.clean(%struct.SockAddr* %addr), !dbg !82
  call void @Client.clean(%struct.Client* %cli), !dbg !82
  ret i32 0, !dbg !82

if.start23:                                       ; preds = %while.then19
  %r26 = load i32, i32* %r, align 4, !dbg !80
  %LE27 = icmp sle i32 %r26, 0, !dbg !80
  br i1 %LE27, label %if.then25, label %if.end24, !dbg !80

if.end24:                                         ; preds = %if.start23
  %buf28 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !83
  %r29 = load i32, i32* %r, align 4, !dbg !83
  %SUB = sub i32 %r29, 1, !dbg !83
  %DOT30 = load i8*, i8** %buf28, align 8, !dbg !83
  %ACCESS31 = getelementptr i8, i8* %DOT30, i32 %SUB, !dbg !83
  store i8 0, i8* %ACCESS31, align 1, !dbg !83
  %fd32 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !84
  %DOT33 = load i32, i32* %fd32, align 4, !dbg !84
  %buf34 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !84
  %DOT35 = load i8*, i8** %buf34, align 8, !dbg !84
  %r36 = load i32, i32* %r, align 4, !dbg !84
  %SUB37 = sub i32 %r36, 1, !dbg !84
  %write = call i32 @write(i32 %DOT33, i8* %DOT35, i32 %SUB37), !dbg !84
  br label %if.start38, !dbg !84

if.then25:                                        ; preds = %if.start23
  br label %while.end20, !dbg !80

if.start38:                                       ; preds = %if.end24
  %buf41 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !85
  %DOT42 = load i8*, i8** %buf41, align 8, !dbg !85
  %strcmp = call i32 @strcmp(i8* %DOT42, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR31, i32 0, i32 0)), !dbg !85
  %EQ43 = icmp eq i32 %strcmp, 0, !dbg !85
  br i1 %EQ43, label %if.then40, label %if.end39, !dbg !85

if.end39:                                         ; preds = %if.start38
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR32, i32 0, i32 0)), !dbg !86
  br label %while.start18, !dbg !86

if.then40:                                        ; preds = %if.start38
  br label %while.end20, !dbg !85
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "client.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd")
!4 = distinct !DISubprogram(name: "log", linkageName: "log", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 4, scope: !4)
!9 = distinct !DISubprogram(name: "get_time", linkageName: "get_time", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 6, scope: !9)
!11 = !DILocation(line: 8, scope: !9)
!12 = !DILocation(line: 9, scope: !9)
!13 = !DILocation(line: 10, scope: !9)
!14 = distinct !DISubprogram(name: "log_ts", linkageName: "log_ts", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 13, scope: !14)
!16 = !DILocation(line: 14, scope: !14)
!17 = !DILocation(line: 15, scope: !14)
!18 = !DILocation(line: 16, scope: !14)
!19 = !DILocation(line: 17, scope: !14)
!20 = !DILocation(line: 18, scope: !14)
!21 = !DILocation(line: 19, scope: !14)
!22 = !DILocation(line: 20, scope: !14)
!23 = !DILocation(line: 21, scope: !14)
!24 = !DILocation(line: 22, scope: !14)
!25 = !DILocation(line: 23, scope: !14)
!26 = distinct !DISubprogram(name: "SockAddr.init", linkageName: "SockAddr.init", scope: null, file: !3, line: 40, type: !5, scopeLine: 40, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!27 = !DILocation(line: 40, scope: !26)
!28 = !DILocation(line: 42, scope: !26)
!29 = !DILocation(line: 43, scope: !26)
!30 = distinct !DISubprogram(name: "SockAddr.set_port", linkageName: "SockAddr.set_port", scope: null, file: !3, line: 45, type: !5, scopeLine: 45, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!31 = !DILocation(line: 45, scope: !30)
!32 = !DILocation(line: 46, scope: !30)
!33 = !DILocation(line: 47, scope: !30)
!34 = distinct !DISubprogram(name: "SockAddr.set_ip", linkageName: "SockAddr.set_ip", scope: null, file: !3, line: 49, type: !5, scopeLine: 49, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!35 = !DILocation(line: 49, scope: !34)
!36 = !DILocation(line: 50, scope: !34)
!37 = !DILocation(line: 51, scope: !34)
!38 = !DILocation(line: 52, scope: !34)
!39 = !DILocation(line: 53, scope: !34)
!40 = distinct !DISubprogram(name: "SockAddr.clean", linkageName: "SockAddr.clean", scope: null, file: !3, line: 55, type: !5, scopeLine: 55, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!41 = !DILocation(line: 55, scope: !40)
!42 = !DILocation(line: 56, scope: !40)
!43 = distinct !DISubprogram(name: "banner", linkageName: "banner", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!44 = !DILocation(line: 4, scope: !43)
!45 = !DILocation(line: 5, scope: !43)
!46 = !DILocation(line: 6, scope: !43)
!47 = !DILocation(line: 7, scope: !43)
!48 = !DILocation(line: 8, scope: !43)
!49 = !DILocation(line: 9, scope: !43)
!50 = !DILocation(line: 10, scope: !43)
!51 = !DILocation(line: 11, scope: !43)
!52 = !DILocation(line: 12, scope: !43)
!53 = !DILocation(line: 13, scope: !43)
!54 = !DILocation(line: 14, scope: !43)
!55 = distinct !DISubprogram(name: "Client.init", linkageName: "Client.init", scope: null, file: !3, line: 21, type: !5, scopeLine: 21, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!56 = !DILocation(line: 21, scope: !55)
!57 = !DILocation(line: 22, scope: !55)
!58 = !DILocation(line: 25, scope: !55)
!59 = !DILocation(line: 24, scope: !55)
!60 = distinct !DISubprogram(name: "Client.clean", linkageName: "Client.clean", scope: null, file: !3, line: 29, type: !5, scopeLine: 29, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!61 = !DILocation(line: 29, scope: !60)
!62 = !DILocation(line: 30, scope: !60)
!63 = !DILocation(line: 31, scope: !60)
!64 = !DILocation(line: 32, scope: !60)
!65 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 34, type: !5, scopeLine: 34, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!66 = !DILocation(line: 35, scope: !65)
!67 = !DILocation(line: 38, scope: !65)
!68 = !DILocation(line: 39, scope: !65)
!69 = !DILocation(line: 43, scope: !65)
!70 = !DILocation(line: 47, scope: !65)
!71 = !DILocation(line: 50, scope: !65)
!72 = !DILocation(line: 44, scope: !65)
!73 = !DILocation(line: 65, scope: !65)
!74 = !DILocation(line: 54, scope: !65)
!75 = !DILocation(line: 62, scope: !65)
!76 = !DILocation(line: 59, scope: !65)
!77 = !DILocation(line: 60, scope: !65)
!78 = !DILocation(line: 61, scope: !65)
!79 = !DILocation(line: 56, scope: !65)
!80 = !DILocation(line: 67, scope: !65)
!81 = !DILocation(line: 79, scope: !65)
!82 = !DILocation(line: 80, scope: !65)
!83 = !DILocation(line: 71, scope: !65)
!84 = !DILocation(line: 72, scope: !65)
!85 = !DILocation(line: 74, scope: !65)
!86 = !DILocation(line: 77, scope: !65)
