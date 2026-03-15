; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/arithmetic/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/arithmetic/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [18 x i8] c"atk + def:       \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [21 x i8] c"atk + def:       %d\0A\00", align 1
@STR2 = private unnamed_addr constant [18 x i8] c"atk - def:       \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [21 x i8] c"atk - def:       %d\0A\00", align 1
@STR4 = private unnamed_addr constant [18 x i8] c"kills * 50:      \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [21 x i8] c"kills * 50:      %d\0A\00", align 1
@STR6 = private unnamed_addr constant [18 x i8] c"total / count:   \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [21 x i8] c"total / count:   %d\0A\00", align 1
@STR8 = private unnamed_addr constant [18 x i8] c"atk % xp_cap:    \00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [22 x i8] c"atk %% xp_cap:    %d\0A\00", align 1
@STR10 = private unnamed_addr constant [18 x i8] c"10 + 5:          \00", align 1
@STR11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [21 x i8] c"10 + 5:          %d\0A\00", align 1
@STR12 = private unnamed_addr constant [18 x i8] c"base_atk + atk:  \00", align 1
@STR13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [21 x i8] c"base_atk + atk:  %d\0A\00", align 1
@STR14 = private unnamed_addr constant [18 x i8] c"2 + 3 * 4:       \00", align 1
@STR15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.7 = private unnamed_addr constant [21 x i8] c"2 + 3 * 4:       %d\0A\00", align 1
@STR16 = private unnamed_addr constant [18 x i8] c"(2 + 3) * 4:     \00", align 1
@STR17 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.8 = private unnamed_addr constant [21 x i8] c"(2 + 3) * 4:     %d\0A\00", align 1

define i32 @base_atk() !dbg !4 {
entry:
  ret i32 10, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %atk = alloca i32, align 4, !dbg !9
  %def = alloca i32, align 4, !dbg !9
  %kills = alloca i32, align 4, !dbg !9
  %total = alloca i32, align 4, !dbg !9
  %count = alloca i32, align 4, !dbg !9
  %xp_cap = alloca i32, align 4, !dbg !9
  store i32 0, i32* %atk, align 4, !dbg !9
  store i32 25, i32* %atk, align 4, !dbg !9
  store i32 0, i32* %def, align 4, !dbg !9
  store i32 8, i32* %def, align 4, !dbg !9
  store i32 0, i32* %kills, align 4, !dbg !9
  store i32 3, i32* %kills, align 4, !dbg !9
  store i32 0, i32* %total, align 4, !dbg !9
  store i32 100, i32* %total, align 4, !dbg !9
  store i32 0, i32* %count, align 4, !dbg !9
  store i32 4, i32* %count, align 4, !dbg !9
  store i32 0, i32* %xp_cap, align 4, !dbg !9
  store i32 70, i32* %xp_cap, align 4, !dbg !9
  %atk1 = load i32, i32* %atk, align 4, !dbg !9
  %def2 = load i32, i32* %def, align 4, !dbg !9
  %ADD = add i32 %atk1, %def2, !dbg !9
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %ADD), !dbg !9
  %atk3 = load i32, i32* %atk, align 4, !dbg !9
  %def4 = load i32, i32* %def, align 4, !dbg !9
  %SUB = sub i32 %atk3, %def4, !dbg !9
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %SUB), !dbg !9
  %kills5 = load i32, i32* %kills, align 4, !dbg !9
  %MUL = mul i32 %kills5, 50, !dbg !9
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %MUL), !dbg !9
  %total6 = load i32, i32* %total, align 4, !dbg !9
  %count7 = load i32, i32* %count, align 4, !dbg !9
  %DIV = sdiv i32 %total6, %count7, !dbg !9
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i32 %DIV), !dbg !9
  %atk8 = load i32, i32* %atk, align 4, !dbg !9
  %xp_cap9 = load i32, i32* %xp_cap, align 4, !dbg !9
  %MOD = srem i32 %atk8, %xp_cap9, !dbg !9
  %4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @output_fmt.4, i32 0, i32 0), i32 1, i32 %MOD), !dbg !9
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt.5, i32 0, i32 0), i32 1, i32 15), !dbg !9
  %base_atk = call i32 @base_atk(), !dbg !10
  %atk10 = load i32, i32* %atk, align 4, !dbg !10
  %ADD11 = add i32 %base_atk, %atk10, !dbg !10
  %6 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt.6, i32 0, i32 0), i32 1, i32 %ADD11), !dbg !10
  %7 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt.7, i32 0, i32 0), i32 1, i32 14), !dbg !10
  %8 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt.8, i32 0, i32 0), i32 1, i32 20), !dbg !10
  ret i32 0, !dbg !10
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/arithmetic")
!4 = distinct !DISubprogram(name: "base_atk", linkageName: "base_atk", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 27, scope: !8)
