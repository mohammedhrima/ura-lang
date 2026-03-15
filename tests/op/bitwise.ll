; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op/bitwise.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op/bitwise.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [11 x i8] c"60 & 13 = \00", align 1
@STR1 = private unnamed_addr constant [16 x i8] c" (expected 12)\0A\00", align 1
@output_fmt = private unnamed_addr constant [28 x i8] c"60 & 13 = %d (expected 12)\0A\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"60 | 13 = \00", align 1
@STR3 = private unnamed_addr constant [16 x i8] c" (expected 61)\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [28 x i8] c"60 | 13 = %d (expected 61)\0A\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"60 ^ 13 = \00", align 1
@STR5 = private unnamed_addr constant [16 x i8] c" (expected 49)\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [28 x i8] c"60 ^ 13 = %d (expected 49)\0A\00", align 1
@STR6 = private unnamed_addr constant [7 x i8] c"~60 = \00", align 1
@STR7 = private unnamed_addr constant [17 x i8] c" (expected -61)\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [25 x i8] c"~60 = %d (expected -61)\0A\00", align 1
@STR8 = private unnamed_addr constant [10 x i8] c"1 << 3 = \00", align 1
@STR9 = private unnamed_addr constant [15 x i8] c" (expected 8)\0A\00", align 1
@output_fmt.4 = private unnamed_addr constant [26 x i8] c"1 << 3 = %d (expected 8)\0A\00", align 1
@STR10 = private unnamed_addr constant [11 x i8] c"60 >> 2 = \00", align 1
@STR11 = private unnamed_addr constant [16 x i8] c" (expected 15)\0A\00", align 1
@output_fmt.5 = private unnamed_addr constant [28 x i8] c"60 >> 2 = %d (expected 15)\0A\00", align 1
@STR12 = private unnamed_addr constant [24 x i8] c"(60 & 13) | (1 << 4) = \00", align 1
@STR13 = private unnamed_addr constant [16 x i8] c" (expected 28)\0A\00", align 1
@output_fmt.6 = private unnamed_addr constant [41 x i8] c"(60 & 13) | (1 << 4) = %d (expected 28)\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %a = alloca i32, align 4, !dbg !7
  %b = alloca i32, align 4, !dbg !7
  %r = alloca i32, align 4, !dbg !7
  store i32 0, i32* %a, align 4, !dbg !7
  store i32 60, i32* %a, align 4, !dbg !7
  store i32 0, i32* %b, align 4, !dbg !7
  store i32 13, i32* %b, align 4, !dbg !7
  store i32 0, i32* %r, align 4, !dbg !7
  %a1 = load i32, i32* %a, align 4, !dbg !7
  %b2 = load i32, i32* %b, align 4, !dbg !7
  %BAND = and i32 %a1, %b2, !dbg !7
  store i32 %BAND, i32* %r, align 4, !dbg !7
  %r3 = load i32, i32* %r, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %r3), !dbg !7
  %a4 = load i32, i32* %a, align 4, !dbg !7
  %b5 = load i32, i32* %b, align 4, !dbg !7
  %BOR = or i32 %a4, %b5, !dbg !7
  store i32 %BOR, i32* %r, align 4, !dbg !7
  %r6 = load i32, i32* %r, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %r6), !dbg !7
  %a7 = load i32, i32* %a, align 4, !dbg !7
  %b8 = load i32, i32* %b, align 4, !dbg !7
  %BXOR = xor i32 %a7, %b8, !dbg !7
  store i32 %BXOR, i32* %r, align 4, !dbg !7
  %r9 = load i32, i32* %r, align 4, !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %r9), !dbg !7
  %a10 = load i32, i32* %a, align 4, !dbg !7
  %BNOT = xor i32 %a10, -1, !dbg !7
  store i32 %BNOT, i32* %r, align 4, !dbg !7
  %r11 = load i32, i32* %r, align 4, !dbg !7
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i32 %r11), !dbg !7
  store i32 8, i32* %r, align 4, !dbg !7
  %r12 = load i32, i32* %r, align 4, !dbg !7
  %4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @output_fmt.4, i32 0, i32 0), i32 1, i32 %r12), !dbg !7
  store i32 15, i32* %r, align 4, !dbg !7
  %r13 = load i32, i32* %r, align 4, !dbg !7
  %5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @output_fmt.5, i32 0, i32 0), i32 1, i32 %r13), !dbg !7
  %a14 = load i32, i32* %a, align 4, !dbg !7
  %b15 = load i32, i32* %b, align 4, !dbg !7
  %BAND16 = and i32 %a14, %b15, !dbg !7
  %BOR17 = or i32 %BAND16, 16, !dbg !7
  store i32 %BOR17, i32* %r, align 4, !dbg !7
  %r18 = load i32, i32* %r, align 4, !dbg !7
  %6 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([41 x i8], [41 x i8]* @output_fmt.6, i32 0, i32 0), i32 1, i32 %r18), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "bitwise.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/op")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
