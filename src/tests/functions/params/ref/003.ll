; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/ref/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/ref/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @add_xp(i32* %a, i32* %b) !dbg !4 {
entry:
  %a1 = alloca i32*, align 8, !dbg !7
  %b2 = alloca i32*, align 8, !dbg !7
  %result = alloca i32, align 4, !dbg !7
  store i32* %a, i32** %a1, align 8, !dbg !7
  store i32* %b, i32** %b2, align 8, !dbg !7
  store i32 0, i32* %result, align 4, !dbg !7
  %ref_ptr = load i32*, i32** %a1, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !7
  %ref_ptr3 = load i32*, i32** %b2, align 8, !dbg !7
  %ref_val4 = load i32, i32* %ref_ptr3, align 4, !dbg !7
  %ADD = add i32 %ref_val, %ref_val4, !dbg !7
  store i32 %ADD, i32* %result, align 4, !dbg !7
  %result5 = load i32, i32* %result, align 4, !dbg !7
  ret i32 %result5, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %kill_xp = alloca i32, align 4, !dbg !9
  %bonus_xp = alloca i32, align 4, !dbg !9
  %z = alloca i32, align 4, !dbg !9
  store i32 0, i32* %kill_xp, align 4, !dbg !9
  store i32 30, i32* %kill_xp, align 4, !dbg !9
  store i32 0, i32* %bonus_xp, align 4, !dbg !9
  store i32 20, i32* %bonus_xp, align 4, !dbg !9
  store i32 0, i32* %z, align 4, !dbg !9
  %add_xp = call i32 @add_xp(i32* %kill_xp, i32* %bonus_xp), !dbg !10
  store i32 %add_xp, i32* %z, align 4, !dbg !10
  %z1 = load i32, i32* %z, align 4, !dbg !10
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %z1), !dbg !10
  ret i32 0, !dbg !10
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/ref")
!4 = distinct !DISubprogram(name: "add_xp", linkageName: "add_xp", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 7, scope: !8)
!10 = !DILocation(line: 11, scope: !8)
