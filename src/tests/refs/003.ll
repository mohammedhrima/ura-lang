; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %xp = alloca i32, align 4, !dbg !7
  %xp_gain = alloca i32, align 4, !dbg !7
  %r = alloca i32*, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  store i32 0, i32* %xp, align 4, !dbg !7
  store i32 0, i32* %xp, align 4, !dbg !7
  store i32 0, i32* %xp_gain, align 4, !dbg !7
  store i32 50, i32* %xp_gain, align 4, !dbg !7
  store i32* %xp, i32** %r, align 8, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.then, %entry
  %i1 = load i32, i32* %i, align 4, !dbg !7
  %LT = icmp slt i32 %i1, 4, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %ref_ptr = load i32*, i32** %r, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !7
  %xp_gain2 = load i32, i32* %xp_gain, align 4, !dbg !7
  %ADD = add i32 %ref_val, %xp_gain2, !dbg !7
  %ref_ptr3 = load i32*, i32** %r, align 8, !dbg !7
  store i32 %ADD, i32* %ref_ptr3, align 4, !dbg !7
  %i4 = load i32, i32* %i, align 4, !dbg !7
  %ADD5 = add i32 %i4, 1, !dbg !7
  store i32 %ADD5, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.end:                                        ; preds = %while.start
  %xp6 = load i32, i32* %xp, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %xp6), !dbg !7
  ret i32 0, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/refs")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
