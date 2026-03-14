; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.User = type { i32 }

@STR0 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define void @User.init(%struct.User* %0) {
entry:
  store %struct.User zeroinitializer, %struct.User* %0, align 4
  ret void
}

define void @User.clean(%struct.User* %0) {
entry:
  ret void
}

define void @foo(%struct.User* %n) !dbg !4 {
entry:
  %n1 = alloca %struct.User*, align 8, !dbg !7
  store %struct.User* %n, %struct.User** %n1, align 8, !dbg !7
  %n2 = load %struct.User*, %struct.User** %n1, align 8, !dbg !7
  %id = getelementptr %struct.User, %struct.User* %n2, i32 0, i32 0, !dbg !7
  store i32 2, i32* %id, align 4, !dbg !7
  ret void, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %user = alloca %struct.User, align 8, !dbg !9
  store %struct.User zeroinitializer, %struct.User* %user, align 4, !dbg !9
  call void @User.init(%struct.User* %user), !dbg !9
  %id = getelementptr %struct.User, %struct.User* %user, i32 0, i32 0, !dbg !9
  store i32 1, i32* %id, align 4, !dbg !9
  call void @foo(%struct.User* %user), !dbg !10
  %id1 = getelementptr %struct.User, %struct.User* %user, i32 0, i32 0, !dbg !11
  %DOT = load i32, i32* %id1, align 4, !dbg !11
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0), i32 1, i32 %DOT), !dbg !11
  call void @User.clean(%struct.User* %user), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "foo", linkageName: "foo", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 7, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 10, type: !5, scopeLine: 10, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 10, scope: !8)
!10 = !DILocation(line: 13, scope: !8)
!11 = !DILocation(line: 14, scope: !8)
