; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods/st01.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods/st01.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Name = type { i8* }
%struct.User = type { i32, %struct.Name }

@STR0 = private unnamed_addr constant [16 x i8] c"call Name.init\0A\00", align 1
@output_fmt = private unnamed_addr constant [16 x i8] c"call Name.init\0A\00", align 1
@STR1 = private unnamed_addr constant [17 x i8] c"call Name.clean\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [17 x i8] c"call Name.clean\0A\00", align 1
@STR2 = private unnamed_addr constant [16 x i8] c"call User.init\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [16 x i8] c"call User.init\0A\00", align 1
@STR3 = private unnamed_addr constant [5 x i8] c"user\00", align 1
@STR4 = private unnamed_addr constant [17 x i8] c"call User.clean\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [17 x i8] c"call User.clean\0A\00", align 1
@STR5 = private unnamed_addr constant [7 x i8] c"hello \00", align 1
@STR6 = private unnamed_addr constant [6 x i8] c" has \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [17 x i8] c"hello %s has %d\0A\00", align 1

declare void @free(i8*)

declare i8* @strdup(i8*)

define void @Name.init(%struct.Name* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Name*, align 8, !dbg !7
  store %struct.Name* %self, %struct.Name** %self1, align 8, !dbg !7
  %self2 = load %struct.Name*, %struct.Name** %self1, align 8, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !7
  ret void, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

define void @Name.setValue(i8* %value, %struct.Name* %self) !dbg !8 {
entry:
  %value1 = alloca i8*, align 8, !dbg !9
  %self2 = alloca %struct.Name*, align 8, !dbg !9
  store i8* %value, i8** %value1, align 8, !dbg !9
  store %struct.Name* %self, %struct.Name** %self2, align 8, !dbg !9
  %self3 = load %struct.Name*, %struct.Name** %self2, align 8, !dbg !9
  %value4 = getelementptr %struct.Name, %struct.Name* %self3, i32 0, i32 0, !dbg !9
  %value5 = load i8*, i8** %value1, align 8, !dbg !10
  %strdup = call i8* @strdup(i8* %value5), !dbg !10
  store i8* %strdup, i8** %value4, align 8, !dbg !10
  ret void, !dbg !10
}

define void @Name.clean(%struct.Name* %self) !dbg !11 {
entry:
  %self1 = alloca %struct.Name*, align 8, !dbg !12
  store %struct.Name* %self, %struct.Name** %self1, align 8, !dbg !12
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !12
  %self2 = load %struct.Name*, %struct.Name** %self1, align 8, !dbg !13
  %value = getelementptr %struct.Name, %struct.Name* %self2, i32 0, i32 0, !dbg !13
  %DOT = load i8*, i8** %value, align 8, !dbg !13
  call void @free(i8* %DOT), !dbg !13
  %self3 = load %struct.Name*, %struct.Name** %self1, align 8, !dbg !13
  ret void, !dbg !13
}

define void @User.init(%struct.User* %self) !dbg !14 {
entry:
  %self1 = alloca %struct.User*, align 8, !dbg !15
  store %struct.User* %self, %struct.User** %self1, align 8, !dbg !15
  %self2 = load %struct.User*, %struct.User** %self1, align 8, !dbg !15
  %name = getelementptr %struct.User, %struct.User* %self2, i32 0, i32 1, !dbg !15
  call void @Name.init(%struct.Name* %name), !dbg !15
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.2, i32 0, i32 0), i32 0), !dbg !15
  %self3 = load %struct.User*, %struct.User** %self1, align 8, !dbg !15
  %age = getelementptr %struct.User, %struct.User* %self3, i32 0, i32 0, !dbg !15
  store i32 10, i32* %age, align 4, !dbg !15
  %self4 = load %struct.User*, %struct.User** %self1, align 8, !dbg !16
  %name5 = getelementptr %struct.User, %struct.User* %self4, i32 0, i32 1, !dbg !16
  call void @Name.setValue(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR3, i32 0, i32 0), %struct.Name* %name5), !dbg !16
  ret void, !dbg !16
}

define void @User.clean(%struct.User* %self) !dbg !17 {
entry:
  %self1 = alloca %struct.User*, align 8, !dbg !18
  store %struct.User* %self, %struct.User** %self1, align 8, !dbg !18
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.3, i32 0, i32 0), i32 0), !dbg !18
  %self2 = load %struct.User*, %struct.User** %self1, align 8, !dbg !18
  %name = getelementptr %struct.User, %struct.User* %self2, i32 0, i32 1, !dbg !18
  call void @Name.clean(%struct.Name* %name), !dbg !18
  ret void, !dbg !18
}

define i32 @main() !dbg !19 {
entry:
  %user = alloca %struct.User, align 8, !dbg !20
  store %struct.User zeroinitializer, %struct.User* %user, align 8, !dbg !20
  call void @User.init(%struct.User* %user), !dbg !20
  %name = getelementptr %struct.User, %struct.User* %user, i32 0, i32 1, !dbg !20
  %value = getelementptr %struct.Name, %struct.Name* %name, i32 0, i32 0, !dbg !20
  %0 = load i8*, i8** %value, align 8, !dbg !20
  %age = getelementptr %struct.User, %struct.User* %user, i32 0, i32 0, !dbg !20
  %1 = load i32, i32* %age, align 4, !dbg !20
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.4, i32 0, i32 0), i32 2, i8* %0, i32 %1), !dbg !20
  call void @User.clean(%struct.User* %user), !dbg !20
  ret i32 0, !dbg !20
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "st01.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/structs/methods")
!4 = distinct !DISubprogram(name: "Name.init", linkageName: "Name.init", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = distinct !DISubprogram(name: "Name.setValue", linkageName: "Name.setValue", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 9, scope: !8)
!10 = !DILocation(line: 10, scope: !8)
!11 = distinct !DISubprogram(name: "Name.clean", linkageName: "Name.clean", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!12 = !DILocation(line: 12, scope: !11)
!13 = !DILocation(line: 14, scope: !11)
!14 = distinct !DISubprogram(name: "User.init", linkageName: "User.init", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!15 = !DILocation(line: 19, scope: !14)
!16 = !DILocation(line: 22, scope: !14)
!17 = distinct !DISubprogram(name: "User.clean", linkageName: "User.clean", scope: null, file: !3, line: 23, type: !5, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!18 = !DILocation(line: 23, scope: !17)
!19 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 26, type: !5, scopeLine: 26, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!20 = !DILocation(line: 26, scope: !19)
