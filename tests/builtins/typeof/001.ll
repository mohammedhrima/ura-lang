; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/typeof/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/typeof/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@typeof = private unnamed_addr constant [6 x i8] c"CHARS\00", align 1
@STR0 = private unnamed_addr constant [6 x i8] c"type \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [9 x i8] c"type %s\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %type = alloca i8*, align 8, !dbg !7
  store i8* null, i8** %type, align 8, !dbg !7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @typeof, i32 0, i32 0), i8** %type, align 8, !dbg !7
  %type1 = load i8*, i8** %type, align 8, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %type1), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/typeof")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
