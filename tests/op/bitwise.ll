; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [28 x i8] c"60 & 13 = %d (expected 12)\0A\00", align 1
@STR1 = private unnamed_addr constant [28 x i8] c"60 | 13 = %d (expected 61)\0A\00", align 1
@STR2 = private unnamed_addr constant [28 x i8] c"60 ^ 13 = %d (expected 49)\0A\00", align 1
@STR3 = private unnamed_addr constant [25 x i8] c"~60 = %d (expected -61)\0A\00", align 1
@STR4 = private unnamed_addr constant [26 x i8] c"1 << 3 = %d (expected 8)\0A\00", align 1
@STR5 = private unnamed_addr constant [28 x i8] c"60 >> 2 = %d (expected 15)\0A\00", align 1
@STR6 = private unnamed_addr constant [41 x i8] c"(60 & 13) | (1 << 4) = %d (expected 28)\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

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
  %r3 = load i32, i32* %r, align 4, !dbg !8
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @STR0, i32 0, i32 0), i32 1, i32 %r3), !dbg !8
  %a4 = load i32, i32* %a, align 4, !dbg !8
  %b5 = load i32, i32* %b, align 4, !dbg !8
  %BOR = or i32 %a4, %b5, !dbg !8
  store i32 %BOR, i32* %r, align 4, !dbg !8
  %r6 = load i32, i32* %r, align 4, !dbg !9
  %printf7 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @STR1, i32 0, i32 0), i32 1, i32 %r6), !dbg !9
  %a8 = load i32, i32* %a, align 4, !dbg !9
  %b9 = load i32, i32* %b, align 4, !dbg !9
  %BXOR = xor i32 %a8, %b9, !dbg !9
  store i32 %BXOR, i32* %r, align 4, !dbg !9
  %r10 = load i32, i32* %r, align 4, !dbg !10
  %printf11 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @STR2, i32 0, i32 0), i32 1, i32 %r10), !dbg !10
  %a12 = load i32, i32* %a, align 4, !dbg !10
  %BNOT = xor i32 %a12, -1, !dbg !10
  store i32 %BNOT, i32* %r, align 4, !dbg !10
  %r13 = load i32, i32* %r, align 4, !dbg !11
  %printf14 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @STR3, i32 0, i32 0), i32 1, i32 %r13), !dbg !11
  store i32 8, i32* %r, align 4, !dbg !11
  %r15 = load i32, i32* %r, align 4, !dbg !12
  %printf16 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @STR4, i32 0, i32 0), i32 1, i32 %r15), !dbg !12
  store i32 15, i32* %r, align 4, !dbg !12
  %r17 = load i32, i32* %r, align 4, !dbg !13
  %printf18 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @STR5, i32 0, i32 0), i32 1, i32 %r17), !dbg !13
  %a19 = load i32, i32* %a, align 4, !dbg !13
  %b20 = load i32, i32* %b, align 4, !dbg !13
  %BAND21 = and i32 %a19, %b20, !dbg !13
  %BOR22 = or i32 %BAND21, 16, !dbg !13
  store i32 %BOR22, i32* %r, align 4, !dbg !13
  %r23 = load i32, i32* %r, align 4, !dbg !14
  %printf24 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([41 x i8], [41 x i8]* @STR6, i32 0, i32 0), i32 1, i32 %r23), !dbg !14
  ret i32 0, !dbg !14
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 10, scope: !4)
!9 = !DILocation(line: 14, scope: !4)
!10 = !DILocation(line: 18, scope: !4)
!11 = !DILocation(line: 22, scope: !4)
!12 = !DILocation(line: 26, scope: !4)
!13 = !DILocation(line: 30, scope: !4)
!14 = !DILocation(line: 34, scope: !4)
