; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

%struct.Color = type { i32, i32, i32, i32 }

@KEY_LEFT = global i32 263
@KEY_RIGHT = global i32 262
@KEY_SPACE = global i32 32
@STR0 = private unnamed_addr constant [10 x i8] c"KEY_LEFT=\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [13 x i8] c"KEY_LEFT=%d\0A\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"KEY_RIGHT=\00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [14 x i8] c"KEY_RIGHT=%d\0A\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"KEY_SPACE=\00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [14 x i8] c"KEY_SPACE=%d\0A\00", align 1

define void @Color.init(%struct.Color* %0) {
entry:
  store %struct.Color zeroinitializer, %struct.Color* %0, align 4
  ret void
}

define void @Color.clean(%struct.Color* %0) {
entry:
  ret void
}

define i32 @main() !dbg !4 {
entry:
  %KEY_LEFT = load i32, i32* @KEY_LEFT, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %KEY_LEFT), !dbg !7
  %KEY_RIGHT = load i32, i32* @KEY_RIGHT, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %KEY_RIGHT), !dbg !7
  %KEY_SPACE = load i32, i32* @KEY_SPACE, align 4, !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %KEY_SPACE), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
