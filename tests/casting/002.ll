; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/casting/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/casting/002.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [13 x i8] c"Score:      \00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c" / \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [21 x i8] c"Score:      %d / %d\0A\00", align 1
@STR3 = private unnamed_addr constant [13 x i8] c"Completion: \00", align 1
@STR4 = private unnamed_addr constant [3 x i8] c"%\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [18 x i8] c"Completion: %f%%\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %score = alloca i32, align 4, !dbg !7
  %max_score = alloca i32, align 4, !dbg !7
  %pct = alloca float, align 4, !dbg !7
  store i32 0, i32* %score, align 4, !dbg !7
  store i32 847, i32* %score, align 4, !dbg !7
  store i32 0, i32* %max_score, align 4, !dbg !7
  store i32 1000, i32* %max_score, align 4, !dbg !7
  store float 0.000000e+00, float* %pct, align 4, !dbg !7
  %score1 = load i32, i32* %score, align 4, !dbg !7
  %max_score2 = load i32, i32* %max_score, align 4, !dbg !7
  %DIV = sdiv i32 %score1, %max_score2, !dbg !7
  %MUL = mul i32 %DIV, float 1.000000e+02, !dbg !7
  store i32 %MUL, float* %pct, align 4, !dbg !7
  %score3 = load i32, i32* %score, align 4, !dbg !7
  %max_score4 = load i32, i32* %max_score, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt, i32 0, i32 0), i32 2, i32 %score3, i32 %max_score4), !dbg !7
  %pct5 = load float, float* %pct, align 4, !dbg !7
  %f2d = fpext float %pct5 to double, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, double %f2d), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/casting")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
