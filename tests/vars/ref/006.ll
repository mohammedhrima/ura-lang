; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/vars/ref/006.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/vars/ref/006.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR3 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %a = alloca i32, align 4, !dbg !7
  %r = alloca i32*, align 8, !dbg !7
  store i32 0, i32* %a, align 4, !dbg !7
  store i32 10, i32* %a, align 4, !dbg !7
  store i32* %a, i32** %r, align 8, !dbg !7
  %ref_ptr = load i32*, i32** %r, align 8, !dbg !7
  %current = load i32, i32* %ref_ptr, align 4, !dbg !7
  %ADD = add i32 %current, 5, !dbg !7
  %ref_ptr1 = load i32*, i32** %r, align 8, !dbg !7
  store i32 %ADD, i32* %ref_ptr1, align 4, !dbg !7
  %a2 = load i32, i32* %a, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %a2), !dbg !7
  %ref_ptr3 = load i32*, i32** %r, align 8, !dbg !7
  %current4 = load i32, i32* %ref_ptr3, align 4, !dbg !7
  %MUL = mul i32 %current4, 2, !dbg !7
  %ref_ptr5 = load i32*, i32** %r, align 8, !dbg !7
  store i32 %MUL, i32* %ref_ptr5, align 4, !dbg !7
  %a6 = load i32, i32* %a, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %a6), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "006.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/vars/ref")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
