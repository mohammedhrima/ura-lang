; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/basic/client.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/basic/client.ura"
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
@STR14 = private unnamed_addr constant [63 x i8] c"\1B[0;36m  URA Chat Client  \1B[0;90m\E2\94\82\1B[0m connecting to \1B[0;33m\00", align 1
@STR15 = private unnamed_addr constant [2 x i8] c":\00", align 1
@STR16 = private unnamed_addr constant [6 x i8] c"\1B[0m\0A\00", align 1
@output_fmt = private unnamed_addr constant [73 x i8] c"\1B[0;36m  URA Chat Client  \1B[0;90m\E2\94\82\1B[0m connecting to \1B[0;33m%s:%d\1B[0m\0A\00", align 1
@STR17 = private unnamed_addr constant [149 x i8] c"\1B[0;90m\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\1B[0m\0A\0A\00", align 1
@STR18 = private unnamed_addr constant [32 x i8] c"failed to create client socket\0A\00", align 1
@STR19 = private unnamed_addr constant [10 x i8] c"127.0.0.1\00", align 1
@STR20 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR21 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR22 = private unnamed_addr constant [14 x i8] c"socket failed\00", align 1
@STR23 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR24 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR25 = private unnamed_addr constant [15 x i8] c"connect failed\00", align 1
@STR26 = private unnamed_addr constant [8 x i8] c"\1B[0;36m\00", align 1
@STR27 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR28 = private unnamed_addr constant [20 x i8] c"connected to server\00", align 1
@STR29 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR30 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR31 = private unnamed_addr constant [20 x i8] c"server disconnected\00", align 1
@STR32 = private unnamed_addr constant [6 x i8] c"\0D\1B[2K\00", align 1
@STR33 = private unnamed_addr constant [8 x i8] c"\1B[0;33m\00", align 1
@STR34 = private unnamed_addr constant [9 x i8] c"[server]\00", align 1
@STR35 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1
@STR36 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1
@STR37 = private unnamed_addr constant [23 x i8] c"\1B[0;36m[client]\1B[0m > \00", align 1

declare i32 @write(i32, i8*, i32)

declare i32 @read(i32, i8*, i32)

declare void @free(i8*)

declare void @exit(i32)

declare i32 @strlen(i8*)

declare void @bzero(i8*, i64)

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

define void @banner(i8* %host, i32 %port) !dbg !43 {
entry:
  %host1 = alloca i8*, align 8, !dbg !44
  %port2 = alloca i32, align 4, !dbg !44
  store i8* %host, i8** %host1, align 8, !dbg !44
  store i32 %port, i32* %port2, align 4, !dbg !44
  call void @log(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR5, i32 0, i32 0)), !dbg !45
  call void @log(i8* getelementptr inbounds ([123 x i8], [123 x i8]* @STR6, i32 0, i32 0)), !dbg !46
  call void @log(i8* getelementptr inbounds ([127 x i8], [127 x i8]* @STR7, i32 0, i32 0)), !dbg !47
  call void @log(i8* getelementptr inbounds ([103 x i8], [103 x i8]* @STR8, i32 0, i32 0)), !dbg !48
  call void @log(i8* getelementptr inbounds ([105 x i8], [105 x i8]* @STR9, i32 0, i32 0)), !dbg !49
  call void @log(i8* getelementptr inbounds ([127 x i8], [127 x i8]* @STR10, i32 0, i32 0)), !dbg !50
  call void @log(i8* getelementptr inbounds ([123 x i8], [123 x i8]* @STR11, i32 0, i32 0)), !dbg !51
  call void @log(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR12, i32 0, i32 0)), !dbg !52
  call void @log(i8* getelementptr inbounds ([148 x i8], [148 x i8]* @STR13, i32 0, i32 0)), !dbg !53
  %host3 = load i8*, i8** %host1, align 8, !dbg !53
  %port4 = load i32, i32* %port2, align 4, !dbg !53
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([73 x i8], [73 x i8]* @output_fmt, i32 0, i32 0), i32 2, i8* %host3, i32 %port4), !dbg !53
  call void @log(i8* getelementptr inbounds ([149 x i8], [149 x i8]* @STR17, i32 0, i32 0)), !dbg !54
  ret void, !dbg !54
}

declare i32 @printf(i8*, i32, ...)

define void @Client.clean(%struct.Client* %0) {
entry:
  ret void
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
  %self9 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !59
  %buf10 = getelementptr %struct.Client, %struct.Client* %self9, i32 0, i32 1, !dbg !59
  %DOT11 = load i8*, i8** %buf10, align 8, !dbg !59
  call void @bzero(i8* %DOT11, i64 1024), !dbg !59
  %self12 = load %struct.Client*, %struct.Client** %self1, align 8, !dbg !60
  %rbuf13 = getelementptr %struct.Client, %struct.Client* %self12, i32 0, i32 2, !dbg !60
  %DOT14 = load i8*, i8** %rbuf13, align 8, !dbg !60
  call void @bzero(i8* %DOT14, i64 1024), !dbg !60
  ret void, !dbg !60

if.then:                                          ; preds = %if.start
  call void @log(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @STR18, i32 0, i32 0)), !dbg !61
  call void @exit(i32 1), !dbg !58
  br label %if.end, !dbg !58
}

define i32 @main() !dbg !62 {
entry:
  %host = alloca i8*, align 8, !dbg !63
  %port = alloca i32, align 4, !dbg !63
  %addr = alloca %struct.SockAddr, align 8, !dbg !63
  %cli = alloca %struct.Client, align 8, !dbg !63
  %result = alloca i32, align 4, !dbg !63
  %r = alloca i32, align 4, !dbg !63
  %pid = alloca i32, align 4, !dbg !63
  store i8* null, i8** %host, align 8, !dbg !63
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR19, i32 0, i32 0), i8** %host, align 8, !dbg !63
  store i32 0, i32* %port, align 4, !dbg !63
  store i32 17000, i32* %port, align 4, !dbg !63
  %host1 = load i8*, i8** %host, align 8, !dbg !64
  %port2 = load i32, i32* %port, align 4, !dbg !64
  call void @banner(i8* %host1, i32 %port2), !dbg !64
  store %struct.SockAddr zeroinitializer, %struct.SockAddr* %addr, align 8, !dbg !63
  call void @SockAddr.init(%struct.SockAddr* %addr), !dbg !63
  %port3 = load i32, i32* %port, align 4, !dbg !65
  call void @SockAddr.set_port(i32 %port3, %struct.SockAddr* %addr), !dbg !65
  call void @SockAddr.set_ip(i32 127, i32 0, i32 0, i32 1, %struct.SockAddr* %addr), !dbg !66
  store %struct.Client zeroinitializer, %struct.Client* %cli, align 8, !dbg !63
  call void @Client.init(%struct.Client* %cli), !dbg !63
  br label %if.start, !dbg !63

if.start:                                         ; preds = %entry
  %fd = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !63
  %DOT = load i32, i32* %fd, align 4, !dbg !63
  %LT = icmp slt i32 %DOT, 0, !dbg !63
  br i1 %LT, label %if.then, label %if.end, !dbg !63

if.end:                                           ; preds = %if.start
  store i32 0, i32* %result, align 4, !dbg !63
  %fd4 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !67
  %DOT5 = load i32, i32* %fd4, align 4, !dbg !67
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %addr, i32 0, i32 0, !dbg !67
  %DOT6 = load i8*, i8** %data, align 8, !dbg !67
  %connect = call i32 @connect(i32 %DOT5, i8* %DOT6, i32 16), !dbg !67
  store i32 %connect, i32* %result, align 4, !dbg !67
  br label %if.start7, !dbg !67

if.then:                                          ; preds = %if.start
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR20, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR21, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR22, i32 0, i32 0)), !dbg !68
  ret i32 1, !dbg !68

if.start7:                                        ; preds = %if.end
  %result10 = load i32, i32* %result, align 4, !dbg !67
  %LT11 = icmp slt i32 %result10, 0, !dbg !67
  br i1 %LT11, label %if.then9, label %if.end8, !dbg !67

if.end8:                                          ; preds = %if.start7
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR26, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR27, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR28, i32 0, i32 0)), !dbg !69
  store i32 0, i32* %r, align 4, !dbg !63
  store i32 0, i32* %r, align 4, !dbg !63
  store i32 0, i32* %pid, align 4, !dbg !63
  %fork = call i32 @fork(), !dbg !70
  store i32 %fork, i32* %pid, align 4, !dbg !70
  br label %if.start12, !dbg !70

if.then9:                                         ; preds = %if.start7
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR23, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR24, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @STR25, i32 0, i32 0)), !dbg !71
  ret i32 1, !dbg !71

if.start12:                                       ; preds = %if.end8
  %pid15 = load i32, i32* %pid, align 4, !dbg !70
  %EQ = icmp eq i32 %pid15, 0, !dbg !70
  br i1 %EQ, label %if.then14, label %if.end13, !dbg !70

if.end13:                                         ; preds = %if.start12
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR36, i32 0, i32 0)), !dbg !72
  br label %while.start28, !dbg !72

if.then14:                                        ; preds = %if.start12
  br label %while.start, !dbg !70

while.start:                                      ; preds = %if.end20, %if.then14
  br i1 true, label %while.then, label %while.end, !dbg !70

while.then:                                       ; preds = %while.start
  %fd16 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !73
  %DOT17 = load i32, i32* %fd16, align 4, !dbg !73
  %rbuf = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !73
  %DOT18 = load i8*, i8** %rbuf, align 8, !dbg !73
  %read = call i32 @read(i32 %DOT17, i8* %DOT18, i32 1023), !dbg !73
  store i32 %read, i32* %r, align 4, !dbg !73
  br label %if.start19, !dbg !73

while.end:                                        ; preds = %if.then21, %while.start
  ret i32 0, !dbg !74

if.start19:                                       ; preds = %while.then
  %r22 = load i32, i32* %r, align 4, !dbg !73
  %LE = icmp sle i32 %r22, 0, !dbg !73
  br i1 %LE, label %if.then21, label %if.end20, !dbg !73

if.end20:                                         ; preds = %if.start19
  %rbuf23 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !75
  %DOT24 = load i8*, i8** %rbuf23, align 8, !dbg !75
  %r25 = load i32, i32* %r, align 4, !dbg !75
  %ACCESS = getelementptr i8, i8* %DOT24, i32 %r25, !dbg !75
  store i8 0, i8* %ACCESS, align 1, !dbg !75
  call void @log(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR32, i32 0, i32 0)), !dbg !76
  %rbuf26 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 2, !dbg !77
  %DOT27 = load i8*, i8** %rbuf26, align 8, !dbg !77
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR33, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR34, i32 0, i32 0), i8* %DOT27), !dbg !77
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR35, i32 0, i32 0)), !dbg !74
  br label %while.start, !dbg !74

if.then21:                                        ; preds = %if.start19
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR29, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR30, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR31, i32 0, i32 0)), !dbg !78
  br label %while.end, !dbg !78

while.start28:                                    ; preds = %if.end34, %if.end13
  br i1 true, label %while.then29, label %while.end30, !dbg !72

while.then29:                                     ; preds = %while.start28
  %buf = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !79
  %DOT31 = load i8*, i8** %buf, align 8, !dbg !79
  %read32 = call i32 @read(i32 0, i8* %DOT31, i32 1023), !dbg !79
  store i32 %read32, i32* %r, align 4, !dbg !79
  br label %if.start33, !dbg !79

while.end30:                                      ; preds = %if.then35, %while.start28
  %fd48 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !80
  %DOT49 = load i32, i32* %fd48, align 4, !dbg !80
  %close = call i32 @close(i32 %DOT49), !dbg !80
  call void @SockAddr.clean(%struct.SockAddr* %addr), !dbg !81
  call void @SockAddr.clean(%struct.SockAddr* %addr), !dbg !81
  call void @Client.clean(%struct.Client* %cli), !dbg !81
  ret i32 0, !dbg !81

if.start33:                                       ; preds = %while.then29
  %r36 = load i32, i32* %r, align 4, !dbg !79
  %LE37 = icmp sle i32 %r36, 0, !dbg !79
  br i1 %LE37, label %if.then35, label %if.end34, !dbg !79

if.end34:                                         ; preds = %if.start33
  %buf38 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !82
  %r39 = load i32, i32* %r, align 4, !dbg !82
  %SUB = sub i32 %r39, 1, !dbg !82
  %DOT40 = load i8*, i8** %buf38, align 8, !dbg !82
  %ACCESS41 = getelementptr i8, i8* %DOT40, i32 %SUB, !dbg !82
  store i8 0, i8* %ACCESS41, align 1, !dbg !82
  %fd42 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 0, !dbg !83
  %DOT43 = load i32, i32* %fd42, align 4, !dbg !83
  %buf44 = getelementptr %struct.Client, %struct.Client* %cli, i32 0, i32 1, !dbg !83
  %DOT45 = load i8*, i8** %buf44, align 8, !dbg !83
  %r46 = load i32, i32* %r, align 4, !dbg !83
  %SUB47 = sub i32 %r46, 1, !dbg !83
  %write = call i32 @write(i32 %DOT43, i8* %DOT45, i32 %SUB47), !dbg !83
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR37, i32 0, i32 0)), !dbg !84
  br label %while.start28, !dbg !84

if.then35:                                        ; preds = %if.start33
  br label %while.end30, !dbg !79
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "client.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/basic")
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
!43 = distinct !DISubprogram(name: "banner", linkageName: "banner", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
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
!54 = !DILocation(line: 15, scope: !43)
!55 = distinct !DISubprogram(name: "Client.init", linkageName: "Client.init", scope: null, file: !3, line: 22, type: !5, scopeLine: 22, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!56 = !DILocation(line: 22, scope: !55)
!57 = !DILocation(line: 23, scope: !55)
!58 = !DILocation(line: 26, scope: !55)
!59 = !DILocation(line: 29, scope: !55)
!60 = !DILocation(line: 30, scope: !55)
!61 = !DILocation(line: 25, scope: !55)
!62 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 32, type: !5, scopeLine: 32, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!63 = !DILocation(line: 32, scope: !62)
!64 = !DILocation(line: 35, scope: !62)
!65 = !DILocation(line: 38, scope: !62)
!66 = !DILocation(line: 39, scope: !62)
!67 = !DILocation(line: 46, scope: !62)
!68 = !DILocation(line: 43, scope: !62)
!69 = !DILocation(line: 51, scope: !62)
!70 = !DILocation(line: 54, scope: !62)
!71 = !DILocation(line: 48, scope: !62)
!72 = !DILocation(line: 67, scope: !62)
!73 = !DILocation(line: 57, scope: !62)
!74 = !DILocation(line: 64, scope: !62)
!75 = !DILocation(line: 61, scope: !62)
!76 = !DILocation(line: 62, scope: !62)
!77 = !DILocation(line: 63, scope: !62)
!78 = !DILocation(line: 59, scope: !62)
!79 = !DILocation(line: 69, scope: !62)
!80 = !DILocation(line: 76, scope: !62)
!81 = !DILocation(line: 77, scope: !62)
!82 = !DILocation(line: 72, scope: !62)
!83 = !DILocation(line: 73, scope: !62)
!84 = !DILocation(line: 74, scope: !62)
