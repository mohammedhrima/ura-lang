; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/ref/005.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/ref/005.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [5 x i8] c"HP: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [8 x i8] c"HP: %d\0A\00", align 1

define void @show_hp(i32 %hp) !dbg !4 {
entry:
  %hp1 = alloca i32, align 4, !dbg !7
  store i32 %hp, i32* %hp1, align 4, !dbg !7
  %hp2 = load i32, i32* %hp1, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %hp2), !dbg !7
  ret void, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !8 {
entry:
  %hero_hp = alloca i32, align 4, !dbg !9
  %r = alloca i32*, align 8, !dbg !9
  store i32 0, i32* %hero_hp, align 4, !dbg !9
  store i32 75, i32* %hero_hp, align 4, !dbg !9
  store i32* %hero_hp, i32** %r, align 8, !dbg !9
  %ref_ptr = load i32*, i32** %r, align 8, !dbg !10
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !10
  call void @show_hp(i32 %ref_val), !dbg !10
  ret i32 0, !dbg !10
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "005.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/functions/params/ref")
!4 = distinct !DISubprogram(name: "show_hp", linkageName: "show_hp", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 9, scope: !8)
