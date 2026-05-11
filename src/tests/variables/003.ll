; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/variables/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/variables/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@STR1 = private unnamed_addr constant [8 x i8] c"name:  \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [11 x i8] c"name:  %s\0A\00", align 1
@STR3 = private unnamed_addr constant [8 x i8] c"hp:    \00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [11 x i8] c"hp:    %d\0A\00", align 1
@STR5 = private unnamed_addr constant [8 x i8] c"xp:    \00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [13 x i8] c"xp:    %lld\0A\00", align 1
@STR7 = private unnamed_addr constant [8 x i8] c"level: \00", align 1
@STR8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [11 x i8] c"level: %d\0A\00", align 1
@STR9 = private unnamed_addr constant [8 x i8] c"grade: \00", align 1
@STR10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [11 x i8] c"grade: %c\0A\00", align 1
@STR11 = private unnamed_addr constant [8 x i8] c"alive: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@STR12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [11 x i8] c"alive: %s\0A\00", align 1
@STR13 = private unnamed_addr constant [8 x i8] c"ratio: \00", align 1
@STR14 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [11 x i8] c"ratio: %f\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %name = alloca i8*, align 8, !dbg !7
  %hp = alloca i32, align 4, !dbg !7
  %xp = alloca i64, align 8, !dbg !7
  %level = alloca i2, align 1, !dbg !7
  %grade = alloca i8, align 1, !dbg !7
  %alive = alloca i1, align 1, !dbg !7
  %ratio = alloca float, align 4, !dbg !7
  store i8* null, i8** %name, align 8, !dbg !7
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @STR0, i32 0, i32 0), i8** %name, align 8, !dbg !7
  store i32 0, i32* %hp, align 4, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  store i64 0, i64* %xp, align 4, !dbg !7
  store i64 999999, i64* %xp, align 4, !dbg !7
  store i2 0, i2* %level, align 1, !dbg !7
  store i2 0, i2* %level, align 1, !dbg !7
  store i8 0, i8* %grade, align 1, !dbg !7
  store i8 83, i8* %grade, align 1, !dbg !7
  store i1 false, i1* %alive, align 1, !dbg !7
  store i1 true, i1* %alive, align 1, !dbg !7
  store float 0.000000e+00, float* %ratio, align 4, !dbg !7
  store float 0x3FEB333340000000, float* %ratio, align 4, !dbg !7
  %name1 = load i8*, i8** %name, align 8, !dbg !7
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt, i32 0, i32 0), i8* %name1), !dbg !7
  %hp2 = load i32, i32* %hp, align 4, !dbg !7
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.1, i32 0, i32 0), i32 %hp2), !dbg !7
  %xp3 = load i64, i64* %xp, align 4, !dbg !7
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @output_fmt.2, i32 0, i32 0), i64 %xp3), !dbg !7
  %level4 = load i2, i2* %level, align 1, !dbg !7
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.3, i32 0, i32 0), i2 %level4), !dbg !7
  %grade5 = load i8, i8* %grade, align 1, !dbg !7
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.4, i32 0, i32 0), i8 %grade5), !dbg !7
  %alive6 = load i1, i1* %alive, align 1, !dbg !7
  %bool_str = select i1 %alive6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0), !dbg !7
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.5, i32 0, i32 0), i8* %bool_str), !dbg !7
  %ratio7 = load float, float* %ratio, align 4, !dbg !7
  %f2d = fpext float %ratio7 to double, !dbg !7
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @output_fmt.6, i32 0, i32 0), double %f2d), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/variables")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
