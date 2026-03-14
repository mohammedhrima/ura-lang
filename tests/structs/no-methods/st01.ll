; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.User = type { i32, i8* }

@STR0 = private unnamed_addr constant [9 x i8] c"mohammed\00", align 1
@STR1 = private unnamed_addr constant [17 x i8] c"hello %s has %d\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i8* @strcpy(i8*, i8*)

define void @User.init(%struct.User* %0) {
entry:
  store %struct.User zeroinitializer, %struct.User* %0, align 8
  ret void
}

define void @User.clean(%struct.User* %0) {
entry:
  ret void
}

define i32 @main() !dbg !4 {
entry:
  %user = alloca %struct.User, align 8, !dbg !7
  %name = alloca i8*, align 8, !dbg !7
  store %struct.User zeroinitializer, %struct.User* %user, align 8, !dbg !7
  call void @User.init(%struct.User* %user), !dbg !7
  %age = getelementptr %struct.User, %struct.User* %user, i32 0, i32 0, !dbg !7
  store i32 28, i32* %age, align 4, !dbg !7
  store i8* null, i8** %name, align 8, !dbg !7
  %calloc = call i8* @calloc(i64 1, i64 10), !dbg !8
  store i8* %calloc, i8** %name, align 8, !dbg !8
  %name1 = load i8*, i8** %name, align 8, !dbg !9
  %strcpy = call i8* @strcpy(i8* %name1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR0, i32 0, i32 0)), !dbg !9
  %name2 = load i8*, i8** %name, align 8, !dbg !10
  %age3 = getelementptr %struct.User, %struct.User* %user, i32 0, i32 0, !dbg !10
  %DOT = load i32, i32* %age3, align 4, !dbg !10
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @STR1, i32 0, i32 0), i32 2, i8* %name2, i32 %DOT), !dbg !10
  %name4 = load i8*, i8** %name, align 8, !dbg !11
  call void @free(i8* %name4), !dbg !11
  call void @User.clean(%struct.User* %user), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
!8 = !DILocation(line: 11, scope: !4)
!9 = !DILocation(line: 12, scope: !4)
!10 = !DILocation(line: 13, scope: !4)
!11 = !DILocation(line: 14, scope: !4)
