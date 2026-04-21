; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/value/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/value/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @take_damage(i32 %hp, i32 %atk) !dbg !4 {
entry:
  %hp1 = alloca i32, align 4, !dbg !7
  %atk2 = alloca i32, align 4, !dbg !7
  store i32 %hp, i32* %hp1, align 4, !dbg !7
  store i32 %atk, i32* %atk2, align 4, !dbg !7
  %hp3 = load i32, i32* %hp1, align 4, !dbg !7
  %atk4 = load i32, i32* %atk2, align 4, !dbg !7
  %SUB = sub i32 %hp3, %atk4, !dbg !7
  ret i32 %SUB, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %result = alloca i32, align 4, !dbg !9
  store i32 0, i32* %result, align 4, !dbg !9
  %take_damage = call i32 @take_damage(i32 100, i32 18), !dbg !10
  store i32 %take_damage, i32* %result, align 4, !dbg !10
  %result1 = load i32, i32* %result, align 4, !dbg !10
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %result1), !dbg !10
  ret i32 0, !dbg !10
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/value")
!4 = distinct !DISubprogram(name: "take_damage", linkageName: "take_damage", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 7, scope: !8)
