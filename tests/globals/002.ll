; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals/002.ura"
target triple = "arm64-apple-macosx16.0.0"

@max = global i32 100
@flag = global i1 true
@STR0 = private unnamed_addr constant [8 x i8] c"max is \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [11 x i8] c"max is %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %flag = load i1, i1* @flag, align 1, !dbg !7
  br i1 %flag, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.then, %if.start
  ret i32 0, !dbg !7

if.then:                                          ; preds = %if.start
  %max = load i32, i32* @max, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %max), !dbg !7
  br label %if.end, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/globals")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
