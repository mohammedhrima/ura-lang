; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs/005.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs/005.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@STR2 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [9 x i8] c"<%d %d>\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %shield = alloca i32, align 4, !dbg !7
  %mp = alloca i32, align 4, !dbg !7
  %s = alloca i32*, align 8, !dbg !7
  %m = alloca i32*, align 8, !dbg !7
  store i32 0, i32* %shield, align 4, !dbg !7
  store i32 10, i32* %shield, align 4, !dbg !7
  store i32 0, i32* %mp, align 4, !dbg !7
  store i32 80, i32* %mp, align 4, !dbg !7
  store i32* %shield, i32** %s, align 8, !dbg !7
  store i32* %mp, i32** %m, align 8, !dbg !7
  %ref_ptr = load i32*, i32** %s, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !7
  %MUL = mul i32 %ref_val, 2, !dbg !7
  %ref_ptr1 = load i32*, i32** %s, align 8, !dbg !7
  store i32 %MUL, i32* %ref_ptr1, align 4, !dbg !7
  %ref_ptr2 = load i32*, i32** %m, align 8, !dbg !7
  %ref_val3 = load i32, i32* %ref_ptr2, align 4, !dbg !7
  %SUB = sub i32 %ref_val3, 25, !dbg !7
  %ref_ptr4 = load i32*, i32** %m, align 8, !dbg !7
  store i32 %SUB, i32* %ref_ptr4, align 4, !dbg !7
  %shield5 = load i32, i32* %shield, align 4, !dbg !7
  %mp6 = load i32, i32* %mp, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @output_fmt, i32 0, i32 0), i32 2, i32 %shield5, i32 %mp6), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "005.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
