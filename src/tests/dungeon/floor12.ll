; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/dungeon/floor12.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/dungeon/floor12.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [8 x i8] c"Score: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"/\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"  Rating: \00", align 1
@STR3 = private unnamed_addr constant [3 x i8] c"%\0A\00", align 1
@output_fmt = private unnamed_addr constant [28 x i8] c"Score: %d/%d  Rating: %f%%\0A\00", align 1
@STR4 = private unnamed_addr constant [8 x i8] c"Grade: \00", align 1
@STR5 = private unnamed_addr constant [6 x i8] c" \E2\86\92 \00", align 1
@STR6 = private unnamed_addr constant [6 x i8] c" \E2\86\92 \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [25 x i8] c"Grade: %c \E2\86\92 %d \E2\86\92 %c\0A\00", align 1
@STR8 = private unnamed_addr constant [13 x i8] c"'d' - 'a' = \00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [16 x i8] c"'d' - 'a' = %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %score = alloca i32, align 4, !dbg !7
  %max_score = alloca i32, align 4, !dbg !7
  %pct = alloca float, align 4, !dbg !7
  %grade = alloca i8, align 1, !dbg !7
  %code = alloca i32, align 4, !dbg !7
  %back = alloca i8, align 1, !dbg !7
  %diff = alloca i32, align 4, !dbg !7
  store i32 0, i32* %score, align 4, !dbg !7
  store i32 847, i32* %score, align 4, !dbg !7
  store i32 0, i32* %max_score, align 4, !dbg !7
  store i32 1000, i32* %max_score, align 4, !dbg !7
  store float 0.000000e+00, float* %pct, align 4, !dbg !7
  %score1 = load i32, i32* %score, align 4, !dbg !7
  %as = sitofp i32 %score1 to float, !dbg !7
  %max_score2 = load i32, i32* %max_score, align 4, !dbg !7
  %as3 = sitofp i32 %max_score2 to float, !dbg !7
  %DIV = fdiv float %as, %as3, !dbg !7
  %MUL = fmul float %DIV, 1.000000e+02, !dbg !7
  store float %MUL, float* %pct, align 4, !dbg !7
  %score4 = load i32, i32* %score, align 4, !dbg !7
  %max_score5 = load i32, i32* %max_score, align 4, !dbg !7
  %pct6 = load float, float* %pct, align 4, !dbg !7
  %f2d = fpext float %pct6 to double, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @output_fmt, i32 0, i32 0), i32 3, i32 %score4, i32 %max_score5, double %f2d), !dbg !7
  store i8 0, i8* %grade, align 1, !dbg !7
  store i8 65, i8* %grade, align 1, !dbg !7
  store i32 0, i32* %code, align 4, !dbg !7
  %grade7 = load i8, i8* %grade, align 1, !dbg !7
  %as8 = sext i8 %grade7 to i32, !dbg !7
  store i32 %as8, i32* %code, align 4, !dbg !7
  store i8 0, i8* %back, align 1, !dbg !7
  %code9 = load i32, i32* %code, align 4, !dbg !7
  %ADD = add i32 %code9, 1, !dbg !7
  %as10 = trunc i32 %ADD to i8, !dbg !7
  store i8 %as10, i8* %back, align 1, !dbg !7
  %grade11 = load i8, i8* %grade, align 1, !dbg !7
  %code12 = load i32, i32* %code, align 4, !dbg !7
  %back13 = load i8, i8* %back, align 1, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @output_fmt.1, i32 0, i32 0), i32 3, i8 %grade11, i32 %code12, i8 %back13), !dbg !7
  store i32 0, i32* %diff, align 4, !dbg !7
  store i32 3, i32* %diff, align 4, !dbg !7
  %diff14 = load i32, i32* %diff, align 4, !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %diff14), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor12.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/dungeon")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 2, type: !5, scopeLine: 2, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 2, scope: !4)
