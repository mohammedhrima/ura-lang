; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/utils.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server/utils.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.SockAddr = type { i8* }

@STR0 = private unnamed_addr constant [9 x i8] c"%H:%M:%S\00", align 1

declare i32 @write(i32, i8*, i32)

declare void @free(i8*)

declare i32 @strlen(i8*)

declare i64 @time(i8*)

declare i8* @localtime(i64*)

declare i32 @strftime(i8*, i32, i8*, i8*)

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

define void @SockAddr.init(%struct.SockAddr* %self) !dbg !14 {
entry:
  %self1 = alloca %struct.SockAddr*, align 8, !dbg !15
  store %struct.SockAddr* %self, %struct.SockAddr** %self1, align 8, !dbg !15
  %self2 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !15
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !15
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self3, i32 0, i32 0, !dbg !15
  %heap = call i8* @calloc(i64 16, i64 1), !dbg !15
  store i8* %heap, i8** %data, align 8, !dbg !15
  %self4 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !16
  %data5 = getelementptr %struct.SockAddr, %struct.SockAddr* %self4, i32 0, i32 0, !dbg !16
  %DOT = load i8*, i8** %data5, align 8, !dbg !16
  %ACCESS = getelementptr i8, i8* %DOT, i32 0, !dbg !16
  store i8 16, i8* %ACCESS, align 1, !dbg !16
  %self6 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !17
  %data7 = getelementptr %struct.SockAddr, %struct.SockAddr* %self6, i32 0, i32 0, !dbg !17
  %DOT8 = load i8*, i8** %data7, align 8, !dbg !17
  %ACCESS9 = getelementptr i8, i8* %DOT8, i32 1, !dbg !17
  store i8 2, i8* %ACCESS9, align 1, !dbg !17
  ret void, !dbg !17
}

define void @SockAddr.set_port(i32 %port, %struct.SockAddr* %self) !dbg !18 {
entry:
  %port1 = alloca i32, align 4, !dbg !19
  %self2 = alloca %struct.SockAddr*, align 8, !dbg !19
  store i32 %port, i32* %port1, align 4, !dbg !19
  store %struct.SockAddr* %self, %struct.SockAddr** %self2, align 8, !dbg !19
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self2, align 8, !dbg !20
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self3, i32 0, i32 0, !dbg !20
  %DOT = load i8*, i8** %data, align 8, !dbg !20
  %ACCESS = getelementptr i8, i8* %DOT, i32 2, !dbg !20
  %port4 = load i32, i32* %port1, align 4, !dbg !20
  %RSHIFT = ashr i32 %port4, 8, !dbg !20
  %as = trunc i32 %RSHIFT to i8, !dbg !20
  store i8 %as, i8* %ACCESS, align 1, !dbg !20
  %self5 = load %struct.SockAddr*, %struct.SockAddr** %self2, align 8, !dbg !21
  %data6 = getelementptr %struct.SockAddr, %struct.SockAddr* %self5, i32 0, i32 0, !dbg !21
  %DOT7 = load i8*, i8** %data6, align 8, !dbg !21
  %ACCESS8 = getelementptr i8, i8* %DOT7, i32 3, !dbg !21
  %port9 = load i32, i32* %port1, align 4, !dbg !21
  %BAND = and i32 %port9, 255, !dbg !21
  %as10 = trunc i32 %BAND to i8, !dbg !21
  store i8 %as10, i8* %ACCESS8, align 1, !dbg !21
  ret void, !dbg !21
}

define void @SockAddr.set_ip(i32 %a, i32 %b, i32 %c, i32 %d, %struct.SockAddr* %self) !dbg !22 {
entry:
  %a1 = alloca i32, align 4, !dbg !23
  %b2 = alloca i32, align 4, !dbg !23
  %c3 = alloca i32, align 4, !dbg !23
  %d4 = alloca i32, align 4, !dbg !23
  %self5 = alloca %struct.SockAddr*, align 8, !dbg !23
  store i32 %a, i32* %a1, align 4, !dbg !23
  store i32 %b, i32* %b2, align 4, !dbg !23
  store i32 %c, i32* %c3, align 4, !dbg !23
  store i32 %d, i32* %d4, align 4, !dbg !23
  store %struct.SockAddr* %self, %struct.SockAddr** %self5, align 8, !dbg !23
  %self6 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !24
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self6, i32 0, i32 0, !dbg !24
  %DOT = load i8*, i8** %data, align 8, !dbg !24
  %ACCESS = getelementptr i8, i8* %DOT, i32 4, !dbg !24
  %a7 = load i32, i32* %a1, align 4, !dbg !24
  %as = trunc i32 %a7 to i8, !dbg !24
  store i8 %as, i8* %ACCESS, align 1, !dbg !24
  %self8 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !25
  %data9 = getelementptr %struct.SockAddr, %struct.SockAddr* %self8, i32 0, i32 0, !dbg !25
  %DOT10 = load i8*, i8** %data9, align 8, !dbg !25
  %ACCESS11 = getelementptr i8, i8* %DOT10, i32 5, !dbg !25
  %b12 = load i32, i32* %b2, align 4, !dbg !25
  %as13 = trunc i32 %b12 to i8, !dbg !25
  store i8 %as13, i8* %ACCESS11, align 1, !dbg !25
  %self14 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !26
  %data15 = getelementptr %struct.SockAddr, %struct.SockAddr* %self14, i32 0, i32 0, !dbg !26
  %DOT16 = load i8*, i8** %data15, align 8, !dbg !26
  %ACCESS17 = getelementptr i8, i8* %DOT16, i32 6, !dbg !26
  %c18 = load i32, i32* %c3, align 4, !dbg !26
  %as19 = trunc i32 %c18 to i8, !dbg !26
  store i8 %as19, i8* %ACCESS17, align 1, !dbg !26
  %self20 = load %struct.SockAddr*, %struct.SockAddr** %self5, align 8, !dbg !27
  %data21 = getelementptr %struct.SockAddr, %struct.SockAddr* %self20, i32 0, i32 0, !dbg !27
  %DOT22 = load i8*, i8** %data21, align 8, !dbg !27
  %ACCESS23 = getelementptr i8, i8* %DOT22, i32 7, !dbg !27
  %d24 = load i32, i32* %d4, align 4, !dbg !27
  %as25 = trunc i32 %d24 to i8, !dbg !27
  store i8 %as25, i8* %ACCESS23, align 1, !dbg !27
  ret void, !dbg !27
}

define void @SockAddr.clean(%struct.SockAddr* %self) !dbg !28 {
entry:
  %self1 = alloca %struct.SockAddr*, align 8, !dbg !29
  store %struct.SockAddr* %self, %struct.SockAddr** %self1, align 8, !dbg !29
  %self2 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !30
  %data = getelementptr %struct.SockAddr, %struct.SockAddr* %self2, i32 0, i32 0, !dbg !30
  %DOT = load i8*, i8** %data, align 8, !dbg !30
  call void @free(i8* %DOT), !dbg !30
  %self3 = load %struct.SockAddr*, %struct.SockAddr** %self1, align 8, !dbg !30
  ret void, !dbg !30
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "utils.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-tcp-server")
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
!14 = distinct !DISubprogram(name: "SockAddr.init", linkageName: "SockAddr.init", scope: null, file: !3, line: 40, type: !5, scopeLine: 40, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 40, scope: !14)
!16 = !DILocation(line: 42, scope: !14)
!17 = !DILocation(line: 43, scope: !14)
!18 = distinct !DISubprogram(name: "SockAddr.set_port", linkageName: "SockAddr.set_port", scope: null, file: !3, line: 45, type: !5, scopeLine: 45, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 45, scope: !18)
!20 = !DILocation(line: 46, scope: !18)
!21 = !DILocation(line: 47, scope: !18)
!22 = distinct !DISubprogram(name: "SockAddr.set_ip", linkageName: "SockAddr.set_ip", scope: null, file: !3, line: 49, type: !5, scopeLine: 49, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!23 = !DILocation(line: 49, scope: !22)
!24 = !DILocation(line: 50, scope: !22)
!25 = !DILocation(line: 51, scope: !22)
!26 = !DILocation(line: 52, scope: !22)
!27 = !DILocation(line: 53, scope: !22)
!28 = distinct !DISubprogram(name: "SockAddr.clean", linkageName: "SockAddr.clean", scope: null, file: !3, line: 55, type: !5, scopeLine: 55, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!29 = !DILocation(line: 55, scope: !28)
!30 = !DILocation(line: 56, scope: !28)
