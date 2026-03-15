; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/fn/param/vars/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/fn/param/vars/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @square(i32 %n) !dbg !4 {
entry:
  %n1 = alloca i32, align 4, !dbg !7
  store i32 %n, i32* %n1, align 4, !dbg !7
  %n2 = load i32, i32* %n1, align 4, !dbg !7
  %n3 = load i32, i32* %n1, align 4, !dbg !7
  %MUL = mul i32 %n2, %n3, !dbg !7
  ret i32 %MUL, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %val = alloca i32, align 4, !dbg !9
  store i32 0, i32* %val, align 4, !dbg !9
  %square = call i32 @square(i32 5), !dbg !10
  store i32 %square, i32* %val, align 4, !dbg !10
  %val1 = load i32, i32* %val, align 4, !dbg !10
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %val1), !dbg !10
  ret i32 0, !dbg !10
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/fn/param/vars")
!4 = distinct !DISubprogram(name: "square", linkageName: "square", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 7, scope: !8)
