; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/output/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/output/001.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.File = type { i8*, i8* }
%struct.Logger = type { %struct.File, i8* }
%struct.App = type { %struct.Logger, i8* }

@STR0 = private unnamed_addr constant [16 x i8] c"call File.init\0A\00", align 1
@output_fmt = private unnamed_addr constant [16 x i8] c"call File.init\0A\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"w\00", align 1
@STR2 = private unnamed_addr constant [15 x i8] c"File.write -> \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [18 x i8] c"File.write -> %s\0A\00", align 1
@STR4 = private unnamed_addr constant [17 x i8] c"call File.clean\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [17 x i8] c"call File.clean\0A\00", align 1
@STR5 = private unnamed_addr constant [18 x i8] c"call Logger.init\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [18 x i8] c"call Logger.init\0A\00", align 1
@STR6 = private unnamed_addr constant [8 x i8] c"app.log\00", align 1
@STR7 = private unnamed_addr constant [7 x i8] c"[APP] \00", align 1
@STR8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [6 x i8] c"%s%s\0A\00", align 1
@STR9 = private unnamed_addr constant [19 x i8] c"call Logger.clean\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [19 x i8] c"call Logger.clean\0A\00", align 1
@STR10 = private unnamed_addr constant [15 x i8] c"call App.init\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [15 x i8] c"call App.init\0A\00", align 1
@STR11 = private unnamed_addr constant [7 x i8] c"UraApp\00", align 1
@STR12 = private unnamed_addr constant [20 x i8] c"application started\00", align 1
@STR13 = private unnamed_addr constant [16 x i8] c"call App.clean\0A\00", align 1
@output_fmt.7 = private unnamed_addr constant [16 x i8] c"call App.clean\0A\00", align 1

declare i8* @fopen(i8*, i8*)

declare i32 @fclose(i8*)

declare void @free(i8*)

declare i8* @strdup(i8*)

define void @File.init(%struct.File* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.File*, align 8, !dbg !7
  store %struct.File* %self, %struct.File** %self1, align 8, !dbg !7
  %self2 = load %struct.File*, %struct.File** %self1, align 8, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !7
  ret void, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

define void @File.setPath(i8* %path, %struct.File* %self) !dbg !8 {
entry:
  %path1 = alloca i8*, align 8, !dbg !9
  %self2 = alloca %struct.File*, align 8, !dbg !9
  store i8* %path, i8** %path1, align 8, !dbg !9
  store %struct.File* %self, %struct.File** %self2, align 8, !dbg !9
  %self3 = load %struct.File*, %struct.File** %self2, align 8, !dbg !9
  %path4 = getelementptr %struct.File, %struct.File* %self3, i32 0, i32 0, !dbg !9
  %path5 = load i8*, i8** %path1, align 8, !dbg !10
  %strdup = call i8* @strdup(i8* %path5), !dbg !10
  store i8* %strdup, i8** %path4, align 8, !dbg !10
  %self6 = load %struct.File*, %struct.File** %self2, align 8, !dbg !10
  %handle = getelementptr %struct.File, %struct.File* %self6, i32 0, i32 1, !dbg !10
  %path7 = load i8*, i8** %path1, align 8, !dbg !11
  %fopen = call i8* @fopen(i8* %path7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @STR1, i32 0, i32 0)), !dbg !11
  store i8* %fopen, i8** %handle, align 8, !dbg !11
  ret void, !dbg !11
}

define void @File.write(i8* %msg, %struct.File* %self) !dbg !12 {
entry:
  %msg1 = alloca i8*, align 8, !dbg !13
  %self2 = alloca %struct.File*, align 8, !dbg !13
  store i8* %msg, i8** %msg1, align 8, !dbg !13
  store %struct.File* %self, %struct.File** %self2, align 8, !dbg !13
  %msg3 = load i8*, i8** %msg1, align 8, !dbg !13
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %msg3), !dbg !13
  ret void, !dbg !13
}

define void @File.clean(%struct.File* %self) !dbg !14 {
entry:
  %self1 = alloca %struct.File*, align 8, !dbg !15
  store %struct.File* %self, %struct.File** %self1, align 8, !dbg !15
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.2, i32 0, i32 0), i32 0), !dbg !15
  %self2 = load %struct.File*, %struct.File** %self1, align 8, !dbg !16
  %handle = getelementptr %struct.File, %struct.File* %self2, i32 0, i32 1, !dbg !16
  %DOT = load i8*, i8** %handle, align 8, !dbg !16
  %fclose = call i32 @fclose(i8* %DOT), !dbg !16
  %self3 = load %struct.File*, %struct.File** %self1, align 8, !dbg !17
  %path = getelementptr %struct.File, %struct.File* %self3, i32 0, i32 0, !dbg !17
  %DOT4 = load i8*, i8** %path, align 8, !dbg !17
  call void @free(i8* %DOT4), !dbg !17
  %self5 = load %struct.File*, %struct.File** %self1, align 8, !dbg !17
  ret void, !dbg !17
}

define void @Logger.init(%struct.Logger* %self) !dbg !18 {
entry:
  %self1 = alloca %struct.Logger*, align 8, !dbg !19
  store %struct.Logger* %self, %struct.Logger** %self1, align 8, !dbg !19
  %self2 = load %struct.Logger*, %struct.Logger** %self1, align 8, !dbg !19
  %file = getelementptr %struct.Logger, %struct.Logger* %self2, i32 0, i32 0, !dbg !19
  call void @File.init(%struct.File* %file), !dbg !19
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.3, i32 0, i32 0), i32 0), !dbg !19
  %self3 = load %struct.Logger*, %struct.Logger** %self1, align 8, !dbg !20
  %file4 = getelementptr %struct.Logger, %struct.Logger* %self3, i32 0, i32 0, !dbg !20
  call void @File.setPath(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR6, i32 0, i32 0), %struct.File* %file4), !dbg !20
  %self5 = load %struct.Logger*, %struct.Logger** %self1, align 8, !dbg !20
  %prefix = getelementptr %struct.Logger, %struct.Logger* %self5, i32 0, i32 1, !dbg !20
  %strdup = call i8* @strdup(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR7, i32 0, i32 0)), !dbg !21
  store i8* %strdup, i8** %prefix, align 8, !dbg !21
  ret void, !dbg !21
}

define void @Logger.log(i8* %msg, %struct.Logger* %self) !dbg !22 {
entry:
  %msg1 = alloca i8*, align 8, !dbg !23
  %self2 = alloca %struct.Logger*, align 8, !dbg !23
  store i8* %msg, i8** %msg1, align 8, !dbg !23
  store %struct.Logger* %self, %struct.Logger** %self2, align 8, !dbg !23
  %self3 = load %struct.Logger*, %struct.Logger** %self2, align 8, !dbg !23
  %prefix = getelementptr %struct.Logger, %struct.Logger* %self3, i32 0, i32 1, !dbg !23
  %0 = load i8*, i8** %prefix, align 8, !dbg !23
  %msg4 = load i8*, i8** %msg1, align 8, !dbg !23
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt.4, i32 0, i32 0), i32 2, i8* %0, i8* %msg4), !dbg !23
  %msg5 = load i8*, i8** %msg1, align 8, !dbg !24
  %self6 = load %struct.Logger*, %struct.Logger** %self2, align 8, !dbg !24
  %file = getelementptr %struct.Logger, %struct.Logger* %self6, i32 0, i32 0, !dbg !24
  call void @File.write(i8* %msg5, %struct.File* %file), !dbg !24
  ret void, !dbg !24
}

define void @Logger.clean(%struct.Logger* %self) !dbg !25 {
entry:
  %self1 = alloca %struct.Logger*, align 8, !dbg !26
  store %struct.Logger* %self, %struct.Logger** %self1, align 8, !dbg !26
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @output_fmt.5, i32 0, i32 0), i32 0), !dbg !26
  %self2 = load %struct.Logger*, %struct.Logger** %self1, align 8, !dbg !27
  %prefix = getelementptr %struct.Logger, %struct.Logger* %self2, i32 0, i32 1, !dbg !27
  %DOT = load i8*, i8** %prefix, align 8, !dbg !27
  call void @free(i8* %DOT), !dbg !27
  %self3 = load %struct.Logger*, %struct.Logger** %self1, align 8, !dbg !27
  %file = getelementptr %struct.Logger, %struct.Logger* %self3, i32 0, i32 0, !dbg !27
  call void @File.clean(%struct.File* %file), !dbg !27
  ret void, !dbg !27
}

define void @App.init(%struct.App* %self) !dbg !28 {
entry:
  %self1 = alloca %struct.App*, align 8, !dbg !29
  store %struct.App* %self, %struct.App** %self1, align 8, !dbg !29
  %self2 = load %struct.App*, %struct.App** %self1, align 8, !dbg !29
  %logger = getelementptr %struct.App, %struct.App* %self2, i32 0, i32 0, !dbg !29
  call void @Logger.init(%struct.Logger* %logger), !dbg !29
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt.6, i32 0, i32 0), i32 0), !dbg !29
  %self3 = load %struct.App*, %struct.App** %self1, align 8, !dbg !30
  %logger4 = getelementptr %struct.App, %struct.App* %self3, i32 0, i32 0, !dbg !30
  call void @Logger.init(%struct.Logger* %logger4), !dbg !30
  %self5 = load %struct.App*, %struct.App** %self1, align 8, !dbg !30
  %name = getelementptr %struct.App, %struct.App* %self5, i32 0, i32 1, !dbg !30
  %strdup = call i8* @strdup(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR11, i32 0, i32 0)), !dbg !31
  store i8* %strdup, i8** %name, align 8, !dbg !31
  ret void, !dbg !31
}

define void @App.run(%struct.App* %self) !dbg !32 {
entry:
  %self1 = alloca %struct.App*, align 8, !dbg !33
  store %struct.App* %self, %struct.App** %self1, align 8, !dbg !33
  %self2 = load %struct.App*, %struct.App** %self1, align 8, !dbg !34
  %logger = getelementptr %struct.App, %struct.App* %self2, i32 0, i32 0, !dbg !34
  call void @Logger.log(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @STR12, i32 0, i32 0), %struct.Logger* %logger), !dbg !34
  %self3 = load %struct.App*, %struct.App** %self1, align 8, !dbg !35
  %name = getelementptr %struct.App, %struct.App* %self3, i32 0, i32 1, !dbg !35
  %DOT = load i8*, i8** %name, align 8, !dbg !35
  %self4 = load %struct.App*, %struct.App** %self1, align 8, !dbg !35
  %logger5 = getelementptr %struct.App, %struct.App* %self4, i32 0, i32 0, !dbg !35
  call void @Logger.log(i8* %DOT, %struct.Logger* %logger5), !dbg !35
  ret void, !dbg !35
}

define void @App.clean(%struct.App* %self) !dbg !36 {
entry:
  %self1 = alloca %struct.App*, align 8, !dbg !37
  store %struct.App* %self, %struct.App** %self1, align 8, !dbg !37
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.7, i32 0, i32 0), i32 0), !dbg !37
  %self2 = load %struct.App*, %struct.App** %self1, align 8, !dbg !38
  %name = getelementptr %struct.App, %struct.App* %self2, i32 0, i32 1, !dbg !38
  %DOT = load i8*, i8** %name, align 8, !dbg !38
  call void @free(i8* %DOT), !dbg !38
  %self3 = load %struct.App*, %struct.App** %self1, align 8, !dbg !38
  %logger = getelementptr %struct.App, %struct.App* %self3, i32 0, i32 0, !dbg !38
  call void @Logger.clean(%struct.Logger* %logger), !dbg !38
  ret void, !dbg !38
}

define i32 @main() !dbg !39 {
entry:
  %app = alloca %struct.App, align 8, !dbg !40
  store %struct.App zeroinitializer, %struct.App* %app, align 8, !dbg !40
  call void @App.init(%struct.App* %app), !dbg !40
  call void @App.run(%struct.App* %app), !dbg !41
  call void @App.clean(%struct.App* %app), !dbg !41
  ret i32 0, !dbg !41
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/output")
!4 = distinct !DISubprogram(name: "File.init", linkageName: "File.init", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
!8 = distinct !DISubprogram(name: "File.setPath", linkageName: "File.setPath", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 11, scope: !8)
!10 = !DILocation(line: 12, scope: !8)
!11 = !DILocation(line: 13, scope: !8)
!12 = distinct !DISubprogram(name: "File.write", linkageName: "File.write", scope: null, file: !3, line: 15, type: !5, scopeLine: 15, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 15, scope: !12)
!14 = distinct !DISubprogram(name: "File.clean", linkageName: "File.clean", scope: null, file: !3, line: 18, type: !5, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 18, scope: !14)
!16 = !DILocation(line: 20, scope: !14)
!17 = !DILocation(line: 21, scope: !14)
!18 = distinct !DISubprogram(name: "Logger.init", linkageName: "Logger.init", scope: null, file: !3, line: 28, type: !5, scopeLine: 28, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!19 = !DILocation(line: 28, scope: !18)
!20 = !DILocation(line: 30, scope: !18)
!21 = !DILocation(line: 31, scope: !18)
!22 = distinct !DISubprogram(name: "Logger.log", linkageName: "Logger.log", scope: null, file: !3, line: 33, type: !5, scopeLine: 33, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!23 = !DILocation(line: 33, scope: !22)
!24 = !DILocation(line: 35, scope: !22)
!25 = distinct !DISubprogram(name: "Logger.clean", linkageName: "Logger.clean", scope: null, file: !3, line: 37, type: !5, scopeLine: 37, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!26 = !DILocation(line: 37, scope: !25)
!27 = !DILocation(line: 39, scope: !25)
!28 = distinct !DISubprogram(name: "App.init", linkageName: "App.init", scope: null, file: !3, line: 46, type: !5, scopeLine: 46, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!29 = !DILocation(line: 46, scope: !28)
!30 = !DILocation(line: 48, scope: !28)
!31 = !DILocation(line: 49, scope: !28)
!32 = distinct !DISubprogram(name: "App.run", linkageName: "App.run", scope: null, file: !3, line: 51, type: !5, scopeLine: 51, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!33 = !DILocation(line: 51, scope: !32)
!34 = !DILocation(line: 52, scope: !32)
!35 = !DILocation(line: 53, scope: !32)
!36 = distinct !DISubprogram(name: "App.clean", linkageName: "App.clean", scope: null, file: !3, line: 55, type: !5, scopeLine: 55, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!37 = !DILocation(line: 55, scope: !36)
!38 = !DILocation(line: 57, scope: !36)
!39 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 60, type: !5, scopeLine: 60, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!40 = !DILocation(line: 60, scope: !39)
!41 = !DILocation(line: 62, scope: !39)
