; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd/server.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd/server.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.SockAddr = type { i8* }
%struct.Server = type { i32, i32, i8*, i8* }

@STR0 = private unnamed_addr constant [9 x i8] c"%H:%M:%S\00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c"\1B[0;90m[\00", align 1
@STR2 = private unnamed_addr constant [7 x i8] c"]\1B[0m \00", align 1
@STR3 = private unnamed_addr constant [6 x i8] c"\1B[0m \00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@STR5 = private unnamed_addr constant [8 x i8] c"\1B[0;32m\00", align 1
@STR6 = private unnamed_addr constant [92 x i8] c"  \E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\97  \E2\96\88\E2\96\88\E2\95\97 \E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97 \E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\0A\00", align 1
@STR7 = private unnamed_addr constant [98 x i8] c" \E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\96\88\E2\96\88\E2\95\91  \E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\96\88\E2\96\88\E2\95\97\E2\95\9A\E2\95\90\E2\95\90\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\95\9D\0A\00", align 1
@STR8 = private unnamed_addr constant [80 x i8] c" \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR9 = private unnamed_addr constant [80 x i8] c" \E2\96\88\E2\96\88\E2\95\91     \E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\95\94\E2\95\90\E2\95\90\E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR10 = private unnamed_addr constant [82 x i8] c" \E2\95\9A\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\96\88\E2\95\97\E2\96\88\E2\96\88\E2\95\91  \E2\96\88\E2\96\88\E2\95\91\E2\96\88\E2\96\88\E2\95\91  \E2\96\88\E2\96\88\E2\95\91   \E2\96\88\E2\96\88\E2\95\91   \0A\00", align 1
@STR11 = private unnamed_addr constant [80 x i8] c"  \E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\9D  \E2\95\9A\E2\95\90\E2\95\9D\E2\95\9A\E2\95\90\E2\95\9D  \E2\95\9A\E2\95\90\E2\95\9D   \E2\95\9A\E2\95\90\E2\95\9D   \0A\00", align 1
@STR12 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@STR13 = private unnamed_addr constant [112 x i8] c"\1B[0;90m\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\1B[0m\0A\00", align 1
@STR14 = private unnamed_addr constant [64 x i8] c"\1B[0;32m  URA Chat Server  \1B[0;90m\E2\94\82\1B[0m port \1B[0;33m17000\1B[0m\0A\00", align 1
@STR15 = private unnamed_addr constant [113 x i8] c"\1B[0;90m\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\1B[0m\0A\0A\00", align 1
@STR16 = private unnamed_addr constant [32 x i8] c"failed to create server socket\0A\00", align 1
@STR17 = private unnamed_addr constant [52 x i8] c"\1B[0;35mWelcome to URA Secure Server\0AType /help\0A\1B[0m\00", align 1
@STR18 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR19 = private unnamed_addr constant [9 x i8] c"[server]\00", align 1
@STR20 = private unnamed_addr constant [20 x i8] c"client disconnected\00", align 1
@STR21 = private unnamed_addr constant [6 x i8] c"/help\00", align 1
@STR22 = private unnamed_addr constant [37 x i8] c"Commands: /help /time /whoami /exit\0A\00", align 1
@STR23 = private unnamed_addr constant [6 x i8] c"/time\00", align 1
@STR24 = private unnamed_addr constant [8 x i8] c"/whoami\00", align 1
@STR25 = private unnamed_addr constant [38 x i8] c"You are connected to URA TCP Engine.\0A\00", align 1
@STR26 = private unnamed_addr constant [6 x i8] c"/exit\00", align 1
@STR27 = private unnamed_addr constant [17 x i8] c"Session closed.\0A\00", align 1
@STR28 = private unnamed_addr constant [6 x i8] c"\0D\1B[2K\00", align 1
@STR29 = private unnamed_addr constant [8 x i8] c"\1B[0;36m\00", align 1
@STR30 = private unnamed_addr constant [9 x i8] c"[client]\00", align 1
@STR31 = private unnamed_addr constant [23 x i8] c"\1B[0;32m[server]\1B[0m > \00", align 1
@STR32 = private unnamed_addr constant [23 x i8] c"\1B[0;32m[server]\1B[0m > \00", align 1
@STR33 = private unnamed_addr constant [23 x i8] c"\1B[0;32m[server]\1B[0m > \00", align 1
@STR34 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@STR35 = private unnamed_addr constant [9 x i8] c"[server]\00", align 1
@STR36 = private unnamed_addr constant [12 x i8] c"bind failed\00", align 1
@STR37 = private unnamed_addr constant [8 x i8] c"\1B[0;32m\00", align 1
@STR38 = private unnamed_addr constant [9 x i8] c"[server]\00", align 1
@STR39 = private unnamed_addr constant [27 x i8] c"listening on port 17000...\00", align 1
@STR40 = private unnamed_addr constant [8 x i8] c"\1B[0;32m\00", align 1
@STR41 = private unnamed_addr constant [9 x i8] c"[server]\00", align 1
@STR42 = private unnamed_addr constant [17 x i8] c"client connected\00", align 1

declare i32 @write(i32, i8*, i32)

declare i32 @read(i32, i8*, i32)

declare void @free(i8*)

declare void @exit(i32)

declare i32 @strlen(i8*)

declare i32 @strcmp(i8*, i8*)

declare i8* @strcpy(i8*, i8*)

declare i64 @time(i8*)

declare i8* @localtime(i64*)

declare i32 @strftime(i8*, i32, i8*, i8*)

declare i32 @socket(i32, i32, i32)

declare i32 @bind(i32, i8*, i32)

declare i32 @listen(i32, i32)

declare i32 @accept(i32, i8*, i8*)

declare i32 @close(i32)

declare i32 @setsockopt(i32, i32, i32, i8*, i32)

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
  call void @log(i8* getelementptr inbounds ([92 x i8], [92 x i8]* @STR6, i32 0, i32 0)), !dbg !45
  call void @log(i8* getelementptr inbounds ([98 x i8], [98 x i8]* @STR7, i32 0, i32 0)), !dbg !46
  call void @log(i8* getelementptr inbounds ([80 x i8], [80 x i8]* @STR8, i32 0, i32 0)), !dbg !47
  call void @log(i8* getelementptr inbounds ([80 x i8], [80 x i8]* @STR9, i32 0, i32 0)), !dbg !48
  call void @log(i8* getelementptr inbounds ([82 x i8], [82 x i8]* @STR10, i32 0, i32 0)), !dbg !49
  call void @log(i8* getelementptr inbounds ([80 x i8], [80 x i8]* @STR11, i32 0, i32 0)), !dbg !50
  call void @log(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR12, i32 0, i32 0)), !dbg !51
  call void @log(i8* getelementptr inbounds ([112 x i8], [112 x i8]* @STR13, i32 0, i32 0)), !dbg !52
  call void @log(i8* getelementptr inbounds ([64 x i8], [64 x i8]* @STR14, i32 0, i32 0)), !dbg !53
  call void @log(i8* getelementptr inbounds ([113 x i8], [113 x i8]* @STR15, i32 0, i32 0)), !dbg !54
  ret void, !dbg !54
}

define void @Server.init(%struct.Server* %self) !dbg !55 {
entry:
  %self1 = alloca %struct.Server*, align 8, !dbg !56
  store %struct.Server* %self, %struct.Server** %self1, align 8, !dbg !56
  %self2 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !56
  %self3 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !56
  %fd = getelementptr %struct.Server, %struct.Server* %self3, i32 0, i32 0, !dbg !56
  %socket = call i32 @socket(i32 2, i32 1, i32 0), !dbg !57
  store i32 %socket, i32* %fd, align 4, !dbg !57
  br label %if.start, !dbg !57

if.start:                                         ; preds = %entry
  %self4 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !57
  %fd5 = getelementptr %struct.Server, %struct.Server* %self4, i32 0, i32 0, !dbg !57
  %DOT = load i32, i32* %fd5, align 4, !dbg !57
  %LT = icmp slt i32 %DOT, 0, !dbg !57
  br i1 %LT, label %if.then, label %if.end, !dbg !57

if.end:                                           ; preds = %if.then, %if.start
  %self6 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !58
  %client_fd = getelementptr %struct.Server, %struct.Server* %self6, i32 0, i32 1, !dbg !58
  store i32 0, i32* %client_fd, align 4, !dbg !58
  %self7 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !58
  %buf = getelementptr %struct.Server, %struct.Server* %self7, i32 0, i32 2, !dbg !58
  %heap = call i8* @calloc(i64 1024, i64 1), !dbg !58
  store i8* %heap, i8** %buf, align 8, !dbg !58
  %self8 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !58
  %rbuf = getelementptr %struct.Server, %struct.Server* %self8, i32 0, i32 3, !dbg !58
  %heap9 = call i8* @calloc(i64 1024, i64 1), !dbg !58
  store i8* %heap9, i8** %rbuf, align 8, !dbg !58
  ret void, !dbg !58

if.then:                                          ; preds = %if.start
  call void @log(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @STR16, i32 0, i32 0)), !dbg !59
  call void @exit(i32 1), !dbg !58
  br label %if.end, !dbg !58
}

define void @Server.set_reuse(%struct.Server* %self) !dbg !60 {
entry:
  %self1 = alloca %struct.Server*, align 8, !dbg !61
  %opt = alloca i8*, align 8, !dbg !61
  store %struct.Server* %self, %struct.Server** %self1, align 8, !dbg !61
  store i8* null, i8** %opt, align 8, !dbg !61
  %stack = alloca [4 x i8], align 1, !dbg !61
  %stack2 = getelementptr [4 x i8], [4 x i8]* %stack, i32 0, i32 0, !dbg !61
  store i8* %stack2, i8** %opt, align 8, !dbg !61
  %opt3 = load i8*, i8** %opt, align 8, !dbg !62
  %ACCESS = getelementptr i8, i8* %opt3, i32 0, !dbg !62
  store i8 1, i8* %ACCESS, align 1, !dbg !62
  %self4 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !63
  %fd = getelementptr %struct.Server, %struct.Server* %self4, i32 0, i32 0, !dbg !63
  %DOT = load i32, i32* %fd, align 4, !dbg !63
  %opt5 = load i8*, i8** %opt, align 8, !dbg !63
  %setsockopt = call i32 @setsockopt(i32 %DOT, i32 65535, i32 4, i8* %opt5, i32 4), !dbg !63
  %self6 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !64
  %fd7 = getelementptr %struct.Server, %struct.Server* %self6, i32 0, i32 0, !dbg !64
  %DOT8 = load i32, i32* %fd7, align 4, !dbg !64
  %opt9 = load i8*, i8** %opt, align 8, !dbg !64
  %setsockopt10 = call i32 @setsockopt(i32 %DOT8, i32 65535, i32 512, i8* %opt9, i32 4), !dbg !64
  ret void, !dbg !64
}

define void @Server.handle_client(%struct.Server* %self) !dbg !65 {
entry:
  %self1 = alloca %struct.Server*, align 8, !dbg !66
  %r = alloca i32, align 4, !dbg !66
  %pid2 = alloca i32, align 4, !dbg !66
  %welcome = alloca i8*, align 8, !dbg !66
  %msg = alloca i8*, align 8, !dbg !66
  %buff = alloca i8*, align 8, !dbg !66
  %bye = alloca i8*, align 8, !dbg !66
  store %struct.Server* %self, %struct.Server** %self1, align 8, !dbg !66
  store i32 0, i32* %r, align 4, !dbg !66
  store i32 0, i32* %r, align 4, !dbg !66
  store i32 0, i32* %pid2, align 4, !dbg !66
  %fork = call i32 @fork(), !dbg !67
  store i32 %fork, i32* %pid2, align 4, !dbg !67
  br label %if.start, !dbg !67

if.start:                                         ; preds = %entry
  %pid22 = load i32, i32* %pid2, align 4, !dbg !67
  %EQ = icmp eq i32 %pid22, 0, !dbg !67
  br i1 %EQ, label %if.then, label %if.end, !dbg !67

if.end:                                           ; preds = %if.start
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR32, i32 0, i32 0)), !dbg !68
  br label %while.start78, !dbg !68

if.then:                                          ; preds = %if.start
  store i8* null, i8** %welcome, align 8, !dbg !66
  store i8* getelementptr inbounds ([52 x i8], [52 x i8]* @STR17, i32 0, i32 0), i8** %welcome, align 8, !dbg !66
  %self3 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !69
  %client_fd = getelementptr %struct.Server, %struct.Server* %self3, i32 0, i32 1, !dbg !69
  %DOT = load i32, i32* %client_fd, align 4, !dbg !69
  %welcome4 = load i8*, i8** %welcome, align 8, !dbg !69
  %welcome5 = load i8*, i8** %welcome, align 8, !dbg !69
  %strlen = call i32 @strlen(i8* %welcome5), !dbg !69
  %write = call i32 @write(i32 %DOT, i8* %welcome4, i32 %strlen), !dbg !69
  br label %while.start, !dbg !69

while.start:                                      ; preds = %if.end20, %if.then
  br i1 true, label %while.then, label %while.end, !dbg !69

while.then:                                       ; preds = %while.start
  %self6 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !70
  %client_fd7 = getelementptr %struct.Server, %struct.Server* %self6, i32 0, i32 1, !dbg !70
  %DOT8 = load i32, i32* %client_fd7, align 4, !dbg !70
  %self9 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !70
  %buf = getelementptr %struct.Server, %struct.Server* %self9, i32 0, i32 2, !dbg !70
  %DOT10 = load i8*, i8** %buf, align 8, !dbg !70
  %read = call i32 @read(i32 %DOT8, i8* %DOT10, i32 1023), !dbg !70
  store i32 %read, i32* %r, align 4, !dbg !70
  store i8* null, i8** %msg, align 8, !dbg !66
  br label %if.start11, !dbg !66

while.end:                                        ; preds = %elif.then62, %if.then13, %while.start
  ret void, !dbg !71

if.start11:                                       ; preds = %while.then
  %r14 = load i32, i32* %r, align 4, !dbg !66
  %LE = icmp sle i32 %r14, 0, !dbg !66
  br i1 %LE, label %if.then13, label %if.end12, !dbg !66

if.end12:                                         ; preds = %if.start11
  %self15 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !72
  %buf16 = getelementptr %struct.Server, %struct.Server* %self15, i32 0, i32 2, !dbg !72
  %DOT17 = load i8*, i8** %buf16, align 8, !dbg !72
  %r18 = load i32, i32* %r, align 4, !dbg !72
  %ACCESS = getelementptr i8, i8* %DOT17, i32 %r18, !dbg !72
  store i8 0, i8* %ACCESS, align 1, !dbg !72
  br label %if.start19, !dbg !72

if.then13:                                        ; preds = %if.start11
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR18, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR19, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR20, i32 0, i32 0)), !dbg !73
  br label %while.end, !dbg !73

if.start19:                                       ; preds = %if.end12
  %self22 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !74
  %buf23 = getelementptr %struct.Server, %struct.Server* %self22, i32 0, i32 2, !dbg !74
  %DOT24 = load i8*, i8** %buf23, align 8, !dbg !74
  %strcmp = call i32 @strcmp(i8* %DOT24, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR21, i32 0, i32 0)), !dbg !74
  %EQ25 = icmp eq i32 %strcmp, 0, !dbg !74
  br i1 %EQ25, label %if.then21, label %elif.start, !dbg !74

if.end20:                                         ; preds = %if.else, %elif.then48, %elif.then, %if.then21
  br label %while.start, !dbg !71

if.then21:                                        ; preds = %if.start19
  store i8* getelementptr inbounds ([37 x i8], [37 x i8]* @STR22, i32 0, i32 0), i8** %msg, align 8, !dbg !74
  %self26 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !75
  %client_fd27 = getelementptr %struct.Server, %struct.Server* %self26, i32 0, i32 1, !dbg !75
  %DOT28 = load i32, i32* %client_fd27, align 4, !dbg !75
  %msg29 = load i8*, i8** %msg, align 8, !dbg !75
  %msg30 = load i8*, i8** %msg, align 8, !dbg !75
  %strlen31 = call i32 @strlen(i8* %msg30), !dbg !75
  %write32 = call i32 @write(i32 %DOT28, i8* %msg29, i32 %strlen31), !dbg !75
  br label %if.end20, !dbg !75

elif.start:                                       ; preds = %if.start19
  %self34 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !76
  %buf35 = getelementptr %struct.Server, %struct.Server* %self34, i32 0, i32 2, !dbg !76
  %DOT36 = load i8*, i8** %buf35, align 8, !dbg !76
  %strcmp37 = call i32 @strcmp(i8* %DOT36, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR23, i32 0, i32 0)), !dbg !76
  %EQ38 = icmp eq i32 %strcmp37, 0, !dbg !76
  br i1 %EQ38, label %elif.then, label %elif.start33, !dbg !76

elif.then:                                        ; preds = %elif.start
  store i8* null, i8** %buff, align 8, !dbg !66
  %heap = call i8* @calloc(i64 100, i64 1), !dbg !66
  store i8* %heap, i8** %buff, align 8, !dbg !66
  %buff39 = load i8*, i8** %buff, align 8, !dbg !77
  %get_time = call i8* @get_time(), !dbg !77
  %strcpy = call i8* @strcpy(i8* %buff39, i8* %get_time), !dbg !77
  %self40 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !78
  %client_fd41 = getelementptr %struct.Server, %struct.Server* %self40, i32 0, i32 1, !dbg !78
  %DOT42 = load i32, i32* %client_fd41, align 4, !dbg !78
  %buff43 = load i8*, i8** %buff, align 8, !dbg !78
  %buff44 = load i8*, i8** %buff, align 8, !dbg !78
  %strlen45 = call i32 @strlen(i8* %buff44), !dbg !78
  %write46 = call i32 @write(i32 %DOT42, i8* %buff43, i32 %strlen45), !dbg !78
  %buff47 = load i8*, i8** %buff, align 8, !dbg !79
  call void @free(i8* %buff47), !dbg !79
  br label %if.end20, !dbg !79

elif.start33:                                     ; preds = %elif.start
  %self50 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !80
  %buf51 = getelementptr %struct.Server, %struct.Server* %self50, i32 0, i32 2, !dbg !80
  %DOT52 = load i8*, i8** %buf51, align 8, !dbg !80
  %strcmp53 = call i32 @strcmp(i8* %DOT52, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR24, i32 0, i32 0)), !dbg !80
  %EQ54 = icmp eq i32 %strcmp53, 0, !dbg !80
  br i1 %EQ54, label %elif.then48, label %elif.start49, !dbg !80

elif.then48:                                      ; preds = %elif.start33
  store i8* getelementptr inbounds ([38 x i8], [38 x i8]* @STR25, i32 0, i32 0), i8** %msg, align 8, !dbg !80
  %self55 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !81
  %client_fd56 = getelementptr %struct.Server, %struct.Server* %self55, i32 0, i32 1, !dbg !81
  %DOT57 = load i32, i32* %client_fd56, align 4, !dbg !81
  %msg58 = load i8*, i8** %msg, align 8, !dbg !81
  %msg59 = load i8*, i8** %msg, align 8, !dbg !81
  %strlen60 = call i32 @strlen(i8* %msg59), !dbg !81
  %write61 = call i32 @write(i32 %DOT57, i8* %msg58, i32 %strlen60), !dbg !81
  br label %if.end20, !dbg !81

elif.start49:                                     ; preds = %elif.start33
  %self63 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !82
  %buf64 = getelementptr %struct.Server, %struct.Server* %self63, i32 0, i32 2, !dbg !82
  %DOT65 = load i8*, i8** %buf64, align 8, !dbg !82
  %strcmp66 = call i32 @strcmp(i8* %DOT65, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR26, i32 0, i32 0)), !dbg !82
  %EQ67 = icmp eq i32 %strcmp66, 0, !dbg !82
  br i1 %EQ67, label %elif.then62, label %if.else, !dbg !82

elif.then62:                                      ; preds = %elif.start49
  store i8* null, i8** %bye, align 8, !dbg !66
  store i8* getelementptr inbounds ([17 x i8], [17 x i8]* @STR27, i32 0, i32 0), i8** %bye, align 8, !dbg !66
  %self68 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !83
  %client_fd69 = getelementptr %struct.Server, %struct.Server* %self68, i32 0, i32 1, !dbg !83
  %DOT70 = load i32, i32* %client_fd69, align 4, !dbg !83
  %bye71 = load i8*, i8** %bye, align 8, !dbg !83
  %bye72 = load i8*, i8** %bye, align 8, !dbg !83
  %strlen73 = call i32 @strlen(i8* %bye72), !dbg !83
  %write74 = call i32 @write(i32 %DOT70, i8* %bye71, i32 %strlen73), !dbg !83
  br label %while.end, !dbg !83

if.else:                                          ; preds = %elif.start49
  call void @log(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR28, i32 0, i32 0)), !dbg !84
  %self75 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !85
  %buf76 = getelementptr %struct.Server, %struct.Server* %self75, i32 0, i32 2, !dbg !85
  %DOT77 = load i8*, i8** %buf76, align 8, !dbg !85
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR29, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR30, i32 0, i32 0), i8* %DOT77), !dbg !85
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR31, i32 0, i32 0)), !dbg !71
  br label %if.end20, !dbg !71

while.start78:                                    ; preds = %if.end85, %if.end
  br i1 true, label %while.then79, label %while.end80, !dbg !68

while.then79:                                     ; preds = %while.start78
  %self81 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !86
  %rbuf = getelementptr %struct.Server, %struct.Server* %self81, i32 0, i32 3, !dbg !86
  %DOT82 = load i8*, i8** %rbuf, align 8, !dbg !86
  %read83 = call i32 @read(i32 0, i8* %DOT82, i32 1023), !dbg !86
  store i32 %read83, i32* %r, align 4, !dbg !86
  br label %if.start84, !dbg !86

while.end80:                                      ; preds = %if.then86, %while.start78
  %self103 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !87
  %client_fd104 = getelementptr %struct.Server, %struct.Server* %self103, i32 0, i32 1, !dbg !87
  %DOT105 = load i32, i32* %client_fd104, align 4, !dbg !87
  %close = call i32 @close(i32 %DOT105), !dbg !87
  ret void, !dbg !87

if.start84:                                       ; preds = %while.then79
  %r87 = load i32, i32* %r, align 4, !dbg !86
  %LE88 = icmp sle i32 %r87, 0, !dbg !86
  br i1 %LE88, label %if.then86, label %if.end85, !dbg !86

if.end85:                                         ; preds = %if.start84
  %self89 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !88
  %rbuf90 = getelementptr %struct.Server, %struct.Server* %self89, i32 0, i32 3, !dbg !88
  %r91 = load i32, i32* %r, align 4, !dbg !88
  %SUB = sub i32 %r91, 1, !dbg !88
  %DOT92 = load i8*, i8** %rbuf90, align 8, !dbg !88
  %ACCESS93 = getelementptr i8, i8* %DOT92, i32 %SUB, !dbg !88
  store i8 0, i8* %ACCESS93, align 1, !dbg !88
  %self94 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !89
  %client_fd95 = getelementptr %struct.Server, %struct.Server* %self94, i32 0, i32 1, !dbg !89
  %DOT96 = load i32, i32* %client_fd95, align 4, !dbg !89
  %self97 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !89
  %rbuf98 = getelementptr %struct.Server, %struct.Server* %self97, i32 0, i32 3, !dbg !89
  %DOT99 = load i8*, i8** %rbuf98, align 8, !dbg !89
  %r100 = load i32, i32* %r, align 4, !dbg !89
  %SUB101 = sub i32 %r100, 1, !dbg !89
  %write102 = call i32 @write(i32 %DOT96, i8* %DOT99, i32 %SUB101), !dbg !89
  call void @log(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @STR33, i32 0, i32 0)), !dbg !90
  br label %while.start78, !dbg !90

if.then86:                                        ; preds = %if.start84
  br label %while.end80, !dbg !86
}

define void @Server.clean(%struct.Server* %self) !dbg !91 {
entry:
  %self1 = alloca %struct.Server*, align 8, !dbg !92
  store %struct.Server* %self, %struct.Server** %self1, align 8, !dbg !92
  %self2 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !93
  %buf = getelementptr %struct.Server, %struct.Server* %self2, i32 0, i32 2, !dbg !93
  %DOT = load i8*, i8** %buf, align 8, !dbg !93
  call void @free(i8* %DOT), !dbg !93
  %self3 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !94
  %rbuf = getelementptr %struct.Server, %struct.Server* %self3, i32 0, i32 3, !dbg !94
  %DOT4 = load i8*, i8** %rbuf, align 8, !dbg !94
  call void @free(i8* %DOT4), !dbg !94
  %self5 = load %struct.Server*, %struct.Server** %self1, align 8, !dbg !94
  ret void, !dbg !94
}

define i32 @main() !dbg !95 {
entry:
  %addr = alloca %struct.SockAddr, align 8, !dbg !96
  %port = alloca i32, align 4, !dbg !96
  %srv = alloca %struct.Server, align 8, !dbg !96
  %pid = alloca i32, align 4, !dbg !96
  call void @banner(), !dbg !96
  store %struct.SockAddr zeroinitializer, %struct.SockAddr* %addr, align 8, !dbg !96
  call void @SockAddr.init(%struct.SockAddr* %addr), !dbg !96
  store i32 0, i32* %port, align 4, !dbg !96
  store i32 17000, i32* %port, align 4, !dbg !96
  %port1 = load i32, i32* %port, align 4, !dbg !97
  call void @SockAddr.set_port(i32 %port1, %struct.SockAddr* %addr), !dbg !97
  call void @SockAddr.set_ip(i32 0, i32 0, i32 0, i32 0, %struct.SockAddr* %addr), !dbg !98
  store %struct.Server zeroinitializer, %struct.Server* %srv, align 8, !dbg !96
  call void @Server.init(%struct.Server* %srv), !dbg !96
  call void @Server.set_reuse(%struct.Server* %srv), !dbg !99
  br label %if.start, !dbg !99

if.start:                                         ; preds = %entry
  %fd = getelementptr %struct.Server, %struct.Server* %srv, i32 0, i32 0, !dbg !100
  %DOT = load i32, i32* %fd, align 4, !dbg !100
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %addr, i32 0, i32 0, !dbg !100
  %DOT2 = load i8*, i8** %data, align 8, !dbg !100
  %bind = call i32 @bind(i32 %DOT, i8* %DOT2, i32 16), !dbg !100
  %LT = icmp slt i32 %bind, 0, !dbg !100
  br i1 %LT, label %if.then, label %if.end, !dbg !100

if.end:                                           ; preds = %if.start
  %fd3 = getelementptr %struct.Server, %struct.Server* %srv, i32 0, i32 0, !dbg !101
  %DOT4 = load i32, i32* %fd3, align 4, !dbg !101
  %listen = call i32 @listen(i32 %DOT4, i32 5), !dbg !101
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR37, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR38, i32 0, i32 0), i8* getelementptr inbounds ([27 x i8], [27 x i8]* @STR39, i32 0, i32 0)), !dbg !102
  br label %while.start, !dbg !102

if.then:                                          ; preds = %if.start
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR34, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR35, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @STR36, i32 0, i32 0)), !dbg !103
  ret i32 1, !dbg !103

while.start:                                      ; preds = %if.end14, %if.then9, %if.end
  br i1 true, label %while.then, label %while.end, !dbg !102

while.then:                                       ; preds = %while.start
  %client_fd = getelementptr %struct.Server, %struct.Server* %srv, i32 0, i32 1, !dbg !102
  %fd5 = getelementptr %struct.Server, %struct.Server* %srv, i32 0, i32 0, !dbg !104
  %DOT6 = load i32, i32* %fd5, align 4, !dbg !104
  %accept = call i32 @accept(i32 %DOT6, i8* null, i8* null), !dbg !104
  store i32 %accept, i32* %client_fd, align 4, !dbg !104
  br label %if.start7, !dbg !104

while.end:                                        ; preds = %while.start
  call void @Server.clean(%struct.Server* %srv), !dbg !105
  call void @SockAddr.clean(%struct.SockAddr* %addr), !dbg !106
  call void @SockAddr.clean(%struct.SockAddr* %addr), !dbg !106
  call void @Server.clean(%struct.Server* %srv), !dbg !106
  ret i32 0, !dbg !106

if.start7:                                        ; preds = %while.then
  %client_fd10 = getelementptr %struct.Server, %struct.Server* %srv, i32 0, i32 1, !dbg !104
  %DOT11 = load i32, i32* %client_fd10, align 4, !dbg !104
  %LT12 = icmp slt i32 %DOT11, 0, !dbg !104
  br i1 %LT12, label %if.then9, label %if.end8, !dbg !104

if.end8:                                          ; preds = %if.start7
  call void @log_ts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR40, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR41, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @STR42, i32 0, i32 0)), !dbg !107
  store i32 0, i32* %pid, align 4, !dbg !96
  %fork = call i32 @fork(), !dbg !108
  store i32 %fork, i32* %pid, align 4, !dbg !108
  br label %if.start13, !dbg !108

if.then9:                                         ; preds = %if.start7
  br label %while.start, !dbg !104

if.start13:                                       ; preds = %if.end8
  %pid16 = load i32, i32* %pid, align 4, !dbg !108
  %EQ = icmp eq i32 %pid16, 0, !dbg !108
  br i1 %EQ, label %if.then15, label %if.end14, !dbg !108

if.end14:                                         ; preds = %if.start13
  %client_fd19 = getelementptr %struct.Server, %struct.Server* %srv, i32 0, i32 1, !dbg !109
  %DOT20 = load i32, i32* %client_fd19, align 4, !dbg !109
  %close21 = call i32 @close(i32 %DOT20), !dbg !109
  br label %while.start, !dbg !109

if.then15:                                        ; preds = %if.start13
  %fd17 = getelementptr %struct.Server, %struct.Server* %srv, i32 0, i32 0, !dbg !110
  %DOT18 = load i32, i32* %fd17, align 4, !dbg !110
  %close = call i32 @close(i32 %DOT18), !dbg !110
  call void @Server.handle_client(%struct.Server* %srv), !dbg !111
  ret i32 0, !dbg !111
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "server.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/cmd")
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
!44 = !DILocation(line: 5, scope: !43)
!45 = !DILocation(line: 6, scope: !43)
!46 = !DILocation(line: 7, scope: !43)
!47 = !DILocation(line: 8, scope: !43)
!48 = !DILocation(line: 9, scope: !43)
!49 = !DILocation(line: 10, scope: !43)
!50 = !DILocation(line: 11, scope: !43)
!51 = !DILocation(line: 12, scope: !43)
!52 = !DILocation(line: 13, scope: !43)
!53 = !DILocation(line: 14, scope: !43)
!54 = !DILocation(line: 15, scope: !43)
!55 = distinct !DISubprogram(name: "Server.init", linkageName: "Server.init", scope: null, file: !3, line: 23, type: !5, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!56 = !DILocation(line: 23, scope: !55)
!57 = !DILocation(line: 24, scope: !55)
!58 = !DILocation(line: 27, scope: !55)
!59 = !DILocation(line: 26, scope: !55)
!60 = distinct !DISubprogram(name: "Server.set_reuse", linkageName: "Server.set_reuse", scope: null, file: !3, line: 33, type: !5, scopeLine: 33, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!61 = !DILocation(line: 33, scope: !60)
!62 = !DILocation(line: 35, scope: !60)
!63 = !DILocation(line: 36, scope: !60)
!64 = !DILocation(line: 37, scope: !60)
!65 = distinct !DISubprogram(name: "Server.handle_client", linkageName: "Server.handle_client", scope: null, file: !3, line: 39, type: !5, scopeLine: 39, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!66 = !DILocation(line: 39, scope: !65)
!67 = !DILocation(line: 41, scope: !65)
!68 = !DILocation(line: 82, scope: !65)
!69 = !DILocation(line: 45, scope: !65)
!70 = !DILocation(line: 48, scope: !65)
!71 = !DILocation(line: 78, scope: !65)
!72 = !DILocation(line: 54, scope: !65)
!73 = !DILocation(line: 51, scope: !65)
!74 = !DILocation(line: 56, scope: !65)
!75 = !DILocation(line: 58, scope: !65)
!76 = !DILocation(line: 60, scope: !65)
!77 = !DILocation(line: 62, scope: !65)
!78 = !DILocation(line: 63, scope: !65)
!79 = !DILocation(line: 64, scope: !65)
!80 = !DILocation(line: 66, scope: !65)
!81 = !DILocation(line: 68, scope: !65)
!82 = !DILocation(line: 70, scope: !65)
!83 = !DILocation(line: 72, scope: !65)
!84 = !DILocation(line: 76, scope: !65)
!85 = !DILocation(line: 77, scope: !65)
!86 = !DILocation(line: 84, scope: !65)
!87 = !DILocation(line: 92, scope: !65)
!88 = !DILocation(line: 88, scope: !65)
!89 = !DILocation(line: 89, scope: !65)
!90 = !DILocation(line: 90, scope: !65)
!91 = distinct !DISubprogram(name: "Server.clean", linkageName: "Server.clean", scope: null, file: !3, line: 94, type: !5, scopeLine: 94, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!92 = !DILocation(line: 94, scope: !91)
!93 = !DILocation(line: 95, scope: !91)
!94 = !DILocation(line: 96, scope: !91)
!95 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 98, type: !5, scopeLine: 98, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!96 = !DILocation(line: 99, scope: !95)
!97 = !DILocation(line: 103, scope: !95)
!98 = !DILocation(line: 104, scope: !95)
!99 = !DILocation(line: 108, scope: !95)
!100 = !DILocation(line: 110, scope: !95)
!101 = !DILocation(line: 114, scope: !95)
!102 = !DILocation(line: 115, scope: !95)
!103 = !DILocation(line: 111, scope: !95)
!104 = !DILocation(line: 118, scope: !95)
!105 = !DILocation(line: 132, scope: !95)
!106 = !DILocation(line: 133, scope: !95)
!107 = !DILocation(line: 122, scope: !95)
!108 = !DILocation(line: 124, scope: !95)
!109 = !DILocation(line: 130, scope: !95)
!110 = !DILocation(line: 126, scope: !95)
!111 = !DILocation(line: 127, scope: !95)
