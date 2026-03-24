; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/variables/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/variables/002.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [15 x i8] c"After combat:\0A\00", align 1
@output_fmt = private unnamed_addr constant [15 x i8] c"After combat:\0A\00", align 1
@STR1 = private unnamed_addr constant [5 x i8] c"HP: \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [8 x i8] c"HP: %d\0A\00", align 1
@STR3 = private unnamed_addr constant [5 x i8] c"MP: \00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [8 x i8] c"MP: %d\0A\00", align 1
@STR5 = private unnamed_addr constant [5 x i8] c"XP: \00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.3 = private unnamed_addr constant [8 x i8] c"XP: %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %hp = alloca i32, align 4, !dbg !7
  %mp = alloca i32, align 4, !dbg !7
  %xp = alloca i32, align 4, !dbg !7
  store i32 0, i32* %hp, align 4, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  store i32 0, i32* %mp, align 4, !dbg !7
  store i32 50, i32* %mp, align 4, !dbg !7
  store i32 0, i32* %xp, align 4, !dbg !7
  store i32 0, i32* %xp, align 4, !dbg !7
  %hp1 = load i32, i32* %hp, align 4, !dbg !7
  %SUB = sub i32 %hp1, 12, !dbg !7
  store i32 %SUB, i32* %hp, align 4, !dbg !7
  %mp2 = load i32, i32* %mp, align 4, !dbg !7
  %SUB3 = sub i32 %mp2, 10, !dbg !7
  store i32 %SUB3, i32* %mp, align 4, !dbg !7
  %xp4 = load i32, i32* %xp, align 4, !dbg !7
  %ADD = add i32 %xp4, 50, !dbg !7
  store i32 %ADD, i32* %xp, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !7
  %hp5 = load i32, i32* %hp, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %hp5), !dbg !7
  %mp6 = load i32, i32* %mp, align 4, !dbg !7
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %mp6), !dbg !7
  %xp7 = load i32, i32* %xp, align 4, !dbg !7
  %3 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt.3, i32 0, i32 0), i32 1, i32 %xp7), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/variables")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
