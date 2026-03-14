; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %base = alloca i32, align 4, !dbg !7
  %ref1 = alloca i32*, align 8, !dbg !7
  %ref2 = alloca i32*, align 8, !dbg !7
  %result = alloca i32, align 4, !dbg !7
  store i32 0, i32* %base, align 4, !dbg !7
  store i32 100, i32* %base, align 4, !dbg !7
  store i32* %base, i32** %ref1, align 8, !dbg !7
  store i32* %base, i32** %ref2, align 8, !dbg !7
  %ref_ptr = load i32*, i32** %ref1, align 8, !dbg !7
  %current = load i32, i32* %ref_ptr, align 4, !dbg !7
  %ADD = add i32 %current, 20, !dbg !7
  %ref_ptr1 = load i32*, i32** %ref1, align 8, !dbg !7
  store i32 %ADD, i32* %ref_ptr1, align 4, !dbg !7
  %ref_ptr2 = load i32*, i32** %ref2, align 8, !dbg !7
  %current3 = load i32, i32* %ref_ptr2, align 4, !dbg !7
  %SUB = sub i32 %current3, 10, !dbg !7
  %ref_ptr4 = load i32*, i32** %ref2, align 8, !dbg !7
  store i32 %SUB, i32* %ref_ptr4, align 4, !dbg !7
  store i32 0, i32* %result, align 4, !dbg !7
  %ref_ptr5 = load i32*, i32** %ref1, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr5, align 4, !dbg !7
  %ref_ptr6 = load i32*, i32** %ref2, align 8, !dbg !7
  %ref_val7 = load i32, i32* %ref_ptr6, align 4, !dbg !7
  %ADD8 = add i32 %ref_val, %ref_val7, !dbg !7
  store i32 %ADD8, i32* %result, align 4, !dbg !7
  %result9 = load i32, i32* %result, align 4, !dbg !8
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0), i32 1, i32 %result9), !dbg !8
  ret i32 0, !dbg !8
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
!8 = !DILocation(line: 13, scope: !4)
