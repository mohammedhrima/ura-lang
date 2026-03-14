; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.User = type { i8*, i32 }

@STR0 = private unnamed_addr constant [15 x i8] c"hello from %s\0A\00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c"mohammed\00", align 1

declare i32 @printf(i8*, i32, ...)

define void @User.init(%struct.User* %0) {
entry:
  store %struct.User zeroinitializer, %struct.User* %0, align 8
  ret void
}

define void @User.clean(%struct.User* %0) {
entry:
  ret void
}

define void @User.greed(%struct.User* %self) !dbg !4 {
entry:
  %self1 = alloca %struct.User*, align 8, !dbg !7
  store %struct.User* %self, %struct.User** %self1, align 8, !dbg !7
  %self2 = load %struct.User*, %struct.User** %self1, align 8, !dbg !8
  %name = getelementptr %struct.User, %struct.User* %self2, i32 0, i32 0, !dbg !8
  %DOT = load i8*, i8** %name, align 8, !dbg !8
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @STR0, i32 0, i32 0), i32 1, i8* %DOT), !dbg !8
  ret void, !dbg !8
}

define i32 @main() !dbg !9 {
entry:
  %user = alloca %struct.User, align 8, !dbg !10
  store %struct.User zeroinitializer, %struct.User* %user, align 8, !dbg !10
  call void @User.init(%struct.User* %user), !dbg !10
  %name = getelementptr %struct.User, %struct.User* %user, i32 0, i32 0, !dbg !10
  store i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR1, i32 0, i32 0), i8** %name, align 8, !dbg !10
  call void @User.greed(%struct.User* %user), !dbg !11
  call void @User.clean(%struct.User* %user), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "User.greed", linkageName: "User.greed", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
!8 = !DILocation(line: 9, scope: !4)
!9 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 11, scope: !9)
!11 = !DILocation(line: 14, scope: !9)
