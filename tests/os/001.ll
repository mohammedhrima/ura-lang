; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/os/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/os/001.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Os = type { i32, i8** }

@os = global %struct.Os zeroinitializer
@STR0 = private unnamed_addr constant [7 x i8] c"argc: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [10 x i8] c"argc: %d\0A\00", align 1
@STR2 = private unnamed_addr constant [10 x i8] c"argv[0]: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [13 x i8] c"argv[0]: %s\0A\00", align 1

define void @Os.init(%struct.Os* %0) {
entry:
  store %struct.Os zeroinitializer, %struct.Os* %0, align 8
  ret void
}

define void @Os.clean(%struct.Os* %0) {
entry:
  ret void
}

define i32 @main(i32 %0, i8** %1) !dbg !4 {
entry:
  call void @Os.init(%struct.Os* @os), !dbg !7
  store i32 %0, i32* getelementptr inbounds (%struct.Os, %struct.Os* @os, i32 0, i32 0), align 4, !dbg !7
  store i8** %1, i8*** getelementptr inbounds (%struct.Os, %struct.Os* @os, i32 0, i32 1), align 8, !dbg !7
  %2 = load i32, i32* getelementptr inbounds (%struct.Os, %struct.Os* @os, i32 0, i32 0), align 4, !dbg !7
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %2), !dbg !7
  %DOT = load i8**, i8*** getelementptr inbounds (%struct.Os, %struct.Os* @os, i32 0, i32 1), align 8, !dbg !8
  %ACCESS = getelementptr i8*, i8** %DOT, i32 0, !dbg !8
  %4 = load i8*, i8** %ACCESS, align 8, !dbg !8
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %4), !dbg !8
  call void @Os.clean(%struct.Os* @os), !dbg !8
  ret i32 0, !dbg !8
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/os")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 6, scope: !4)
