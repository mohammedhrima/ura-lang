; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Name = type { i8* }
%struct.User = type { i32, %struct.Name }

@STR0 = private unnamed_addr constant [16 x i8] c"call Name.init\0A\00", align 1
@STR1 = private unnamed_addr constant [17 x i8] c"call Name.clean\0A\00", align 1
@STR2 = private unnamed_addr constant [16 x i8] c"call User.init\0A\00", align 1
@STR3 = private unnamed_addr constant [5 x i8] c"user\00", align 1
@STR4 = private unnamed_addr constant [17 x i8] c"call User.clean\0A\00", align 1
@STR5 = private unnamed_addr constant [17 x i8] c"hello %s has %d\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

declare void @free(i8*)

declare i8* @strdup(i8*)

define void @Name.init(%struct.Name* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.Name*, align 8, !dbg !7
  store %struct.Name* %self, %struct.Name** %self1, align 8, !dbg !7
  %self2 = load %struct.Name*, %struct.Name** %self1, align 8, !dbg !7
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR0, i32 0, i32 0), i32 0), !dbg !8
  ret void, !dbg !8
}

define void @Name.setValue(i8* %value, %struct.Name* %self) !dbg !9 {
entry:
  %value1 = alloca i8*, align 8, !dbg !10
  %self2 = alloca %struct.Name*, align 8, !dbg !10
  store i8* %value, i8** %value1, align 8, !dbg !10
  store %struct.Name* %self, %struct.Name** %self2, align 8, !dbg !10
  %self3 = load %struct.Name*, %struct.Name** %self2, align 8, !dbg !10
  %value4 = getelementptr %struct.Name, %struct.Name* %self3, i32 0, i32 0, !dbg !10
  %value5 = load i8*, i8** %value1, align 8, !dbg !11
  %strdup = call i8* @strdup(i8* %value5), !dbg !11
  store i8* %strdup, i8** %value4, align 8, !dbg !11
  ret void, !dbg !11
}

define void @Name.clean(%struct.Name* %self) !dbg !12 {
entry:
  %self1 = alloca %struct.Name*, align 8, !dbg !13
  store %struct.Name* %self, %struct.Name** %self1, align 8, !dbg !13
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @STR1, i32 0, i32 0), i32 0), !dbg !14
  %self2 = load %struct.Name*, %struct.Name** %self1, align 8, !dbg !15
  %value = getelementptr %struct.Name, %struct.Name* %self2, i32 0, i32 0, !dbg !15
  %DOT = load i8*, i8** %value, align 8, !dbg !15
  call void @free(i8* %DOT), !dbg !15
  %self3 = load %struct.Name*, %struct.Name** %self1, align 8, !dbg !15
  ret void, !dbg !15
}

define void @User.init(%struct.User* %self) !dbg !16 {
entry:
  %self1 = alloca %struct.User*, align 8, !dbg !17
  store %struct.User* %self, %struct.User** %self1, align 8, !dbg !17
  %self2 = load %struct.User*, %struct.User** %self1, align 8, !dbg !17
  %name = getelementptr %struct.User, %struct.User* %self2, i32 0, i32 1, !dbg !17
  call void @Name.init(%struct.Name* %name), !dbg !17
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @STR2, i32 0, i32 0), i32 0), !dbg !18
  %self3 = load %struct.User*, %struct.User** %self1, align 8, !dbg !18
  %age = getelementptr %struct.User, %struct.User* %self3, i32 0, i32 0, !dbg !18
  store i32 10, i32* %age, align 4, !dbg !18
  %self4 = load %struct.User*, %struct.User** %self1, align 8, !dbg !19
  %name5 = getelementptr %struct.User, %struct.User* %self4, i32 0, i32 1, !dbg !19
  call void @Name.setValue(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR3, i32 0, i32 0), %struct.Name* %name5), !dbg !19
  ret void, !dbg !19
}

define void @User.clean(%struct.User* %self) !dbg !20 {
entry:
  %self1 = alloca %struct.User*, align 8, !dbg !21
  store %struct.User* %self, %struct.User** %self1, align 8, !dbg !21
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @STR4, i32 0, i32 0), i32 0), !dbg !22
  %self2 = load %struct.User*, %struct.User** %self1, align 8, !dbg !22
  %name = getelementptr %struct.User, %struct.User* %self2, i32 0, i32 1, !dbg !22
  call void @Name.clean(%struct.Name* %name), !dbg !22
  ret void, !dbg !22
}

define i32 @main() !dbg !23 {
entry:
  %user = alloca %struct.User, align 8, !dbg !24
  store %struct.User zeroinitializer, %struct.User* %user, align 8, !dbg !24
  call void @User.init(%struct.User* %user), !dbg !24
  %name = getelementptr %struct.User, %struct.User* %user, i32 0, i32 1, !dbg !25
  %value = getelementptr %struct.Name, %struct.Name* %name, i32 0, i32 0, !dbg !25
  %DOT = load i8*, i8** %value, align 8, !dbg !25
  %age = getelementptr %struct.User, %struct.User* %user, i32 0, i32 0, !dbg !25
  %DOT1 = load i32, i32* %age, align 4, !dbg !25
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @STR5, i32 0, i32 0), i32 2, i8* %DOT, i32 %DOT1), !dbg !25
  call void @User.clean(%struct.User* %user), !dbg !25
  ret i32 0, !dbg !25
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "Name.init", linkageName: "Name.init", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
!9 = distinct !DISubprogram(name: "Name.setValue", linkageName: "Name.setValue", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 9, scope: !9)
!11 = !DILocation(line: 10, scope: !9)
!12 = distinct !DISubprogram(name: "Name.clean", linkageName: "Name.clean", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!13 = !DILocation(line: 12, scope: !12)
!14 = !DILocation(line: 13, scope: !12)
!15 = !DILocation(line: 14, scope: !12)
!16 = distinct !DISubprogram(name: "User.init", linkageName: "User.init", scope: null, file: !3, line: 19, type: !5, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!17 = !DILocation(line: 19, scope: !16)
!18 = !DILocation(line: 20, scope: !16)
!19 = !DILocation(line: 22, scope: !16)
!20 = distinct !DISubprogram(name: "User.clean", linkageName: "User.clean", scope: null, file: !3, line: 23, type: !5, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!21 = !DILocation(line: 23, scope: !20)
!22 = !DILocation(line: 24, scope: !20)
!23 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 26, type: !5, scopeLine: 26, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!24 = !DILocation(line: 26, scope: !23)
!25 = !DILocation(line: 28, scope: !23)
