; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/bitwise/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/bitwise/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@POISONED = global i32 1
@FROZEN = global i32 2
@BURNING = global i32 4
@SHIELDED = global i32 8
@STR0 = private unnamed_addr constant [15 x i8] c"After apply:  \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [18 x i8] c"After apply:  %d\0A\00", align 1
@STR2 = private unnamed_addr constant [15 x i8] c"Has POISONED: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [18 x i8] c"Has POISONED: %d\0A\00", align 1
@STR4 = private unnamed_addr constant [15 x i8] c"Has FROZEN:   \00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [18 x i8] c"Has FROZEN:   %d\0A\00", align 1
@STR6 = private unnamed_addr constant [15 x i8] c"After toggle: \00", align 1
@STR7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [18 x i8] c"After toggle: %d\0A\00", align 1
@STR8 = private unnamed_addr constant [15 x i8] c"After cure:   \00", align 1
@STR9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [18 x i8] c"After cure:   %d\0A\00", align 1
@STR10 = private unnamed_addr constant [15 x i8] c"Tier2 shield: \00", align 1
@STR11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [18 x i8] c"Tier2 shield: %d\0A\00", align 1
@STR12 = private unnamed_addr constant [15 x i8] c"tier2 >> 2:   \00", align 1
@STR13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [18 x i8] c"tier2 >> 2:   %d\0A\00", align 1
@STR14 = private unnamed_addr constant [15 x i8] c"combo:        \00", align 1
@STR15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.7 = private unnamed_addr constant [18 x i8] c"combo:        %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %status = alloca i32, align 4, !dbg !7
  %tier2 = alloca i32, align 4, !dbg !7
  %combo = alloca i32, align 4, !dbg !7
  store i32 0, i32* %status, align 4, !dbg !7
  store i32 0, i32* %status, align 4, !dbg !7
  %status1 = load i32, i32* %status, align 4, !dbg !7
  %POISONED = load i32, i32* @POISONED, align 4, !dbg !7
  %BOR = or i32 %status1, %POISONED, !dbg !7
  store i32 %BOR, i32* %status, align 4, !dbg !7
  %status2 = load i32, i32* %status, align 4, !dbg !7
  %BURNING = load i32, i32* @BURNING, align 4, !dbg !7
  %BOR3 = or i32 %status2, %BURNING, !dbg !7
  store i32 %BOR3, i32* %status, align 4, !dbg !7
  %status4 = load i32, i32* %status, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %status4), !dbg !7
  %status5 = load i32, i32* %status, align 4, !dbg !7
  %POISONED6 = load i32, i32* @POISONED, align 4, !dbg !7
  %BAND = and i32 %status5, %POISONED6, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %BAND), !dbg !7
  %status7 = load i32, i32* %status, align 4, !dbg !7
  %FROZEN = load i32, i32* @FROZEN, align 4, !dbg !7
  %BAND8 = and i32 %status7, %FROZEN, !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %BAND8), !dbg !7
  %status9 = load i32, i32* %status, align 4, !dbg !7
  %BURNING10 = load i32, i32* @BURNING, align 4, !dbg !7
  %BXOR = xor i32 %status9, %BURNING10, !dbg !7
  store i32 %BXOR, i32* %status, align 4, !dbg !7
  %status11 = load i32, i32* %status, align 4, !dbg !7
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i32 %status11), !dbg !7
  %POISONED12 = load i32, i32* @POISONED, align 4, !dbg !7
  %BNOT = xor i32 %POISONED12, -1, !dbg !7
  %status13 = load i32, i32* %status, align 4, !dbg !7
  %BAND14 = and i32 %status13, %BNOT, !dbg !7
  store i32 %BAND14, i32* %status, align 4, !dbg !7
  %status15 = load i32, i32* %status, align 4, !dbg !7
  %4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.4, i32 0, i32 0), i32 1, i32 %status15), !dbg !7
  store i32 0, i32* %tier2, align 4, !dbg !7
  %SHIELDED = load i32, i32* @SHIELDED, align 4, !dbg !7
  %LSHIFT = shl i32 %SHIELDED, 1, !dbg !7
  store i32 %LSHIFT, i32* %tier2, align 4, !dbg !7
  %tier216 = load i32, i32* %tier2, align 4, !dbg !7
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.5, i32 0, i32 0), i32 1, i32 %tier216), !dbg !7
  %tier217 = load i32, i32* %tier2, align 4, !dbg !7
  %RSHIFT = ashr i32 %tier217, 2, !dbg !7
  %6 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.6, i32 0, i32 0), i32 1, i32 %RSHIFT), !dbg !7
  store i32 0, i32* %combo, align 4, !dbg !7
  %POISONED18 = load i32, i32* @POISONED, align 4, !dbg !7
  %BURNING19 = load i32, i32* @BURNING, align 4, !dbg !7
  %BAND20 = and i32 %POISONED18, %BURNING19, !dbg !7
  %BOR21 = or i32 %BAND20, 8, !dbg !7
  store i32 %BOR21, i32* %combo, align 4, !dbg !7
  %combo22 = load i32, i32* %combo, align 4, !dbg !7
  %7 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @output_fmt.7, i32 0, i32 0), i32 1, i32 %combo22), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/operators/bitwise")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 8, scope: !4)
