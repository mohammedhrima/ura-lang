; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs/004.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs/004.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [17 x i8] c"HP after hit:   \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [20 x i8] c"HP after hit:   %d\0A\00", align 1
@STR2 = private unnamed_addr constant [17 x i8] c"MP after spell: \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [20 x i8] c"MP after spell: %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %hp = alloca i32, align 4, !dbg !7
  %mp = alloca i32, align 4, !dbg !7
  %shield = alloca i32*, align 8, !dbg !7
  %mana = alloca i32*, align 8, !dbg !7
  store i32 0, i32* %hp, align 4, !dbg !7
  store i32 100, i32* %hp, align 4, !dbg !7
  store i32 0, i32* %mp, align 4, !dbg !7
  store i32 50, i32* %mp, align 4, !dbg !7
  store i32* %hp, i32** %shield, align 8, !dbg !7
  %ref_ptr = load i32*, i32** %shield, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !7
  %SUB = sub i32 %ref_val, 30, !dbg !7
  %ref_ptr1 = load i32*, i32** %shield, align 8, !dbg !7
  store i32 %SUB, i32* %ref_ptr1, align 4, !dbg !7
  %hp2 = load i32, i32* %hp, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %hp2), !dbg !7
  store i32* %mp, i32** %mana, align 8, !dbg !7
  %ref_ptr3 = load i32*, i32** %mana, align 8, !dbg !7
  %ref_val4 = load i32, i32* %ref_ptr3, align 4, !dbg !7
  %SUB5 = sub i32 %ref_val4, 20, !dbg !7
  %ref_ptr6 = load i32*, i32** %mana, align 8, !dbg !7
  store i32 %SUB5, i32* %ref_ptr6, align 4, !dbg !7
  %mp7 = load i32, i32* %mp, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %mp7), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "004.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
