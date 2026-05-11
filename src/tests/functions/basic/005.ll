; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/functions/basic/005.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/functions/basic/005.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @double_it(i32 %n) !dbg !4 {
entry:
  %n1 = alloca i32, align 4, !dbg !7
  store i32 %n, i32* %n1, align 4, !dbg !7
  %n2 = load i32, i32* %n1, align 4, !dbg !7
  %MUL = mul i32 %n2, 2, !dbg !7
  ret i32 %MUL, !dbg !7
}

define i32 @triple_it(i32 %n) !dbg !8 {
entry:
  %n1 = alloca i32, align 4, !dbg !9
  store i32 %n, i32* %n1, align 4, !dbg !9
  %n2 = load i32, i32* %n1, align 4, !dbg !9
  %MUL = mul i32 %n2, 3, !dbg !9
  ret i32 %MUL, !dbg !9
}

define i32 @apply(i32 (i32)* %cb, i32 %x) !dbg !10 {
entry:
  %cb1 = alloca i32 (i32)*, align 8, !dbg !11
  %x2 = alloca i32, align 4, !dbg !11
  store i32 (i32)* %cb, i32 (i32)** %cb1, align 8, !dbg !11
  store i32 %x, i32* %x2, align 4, !dbg !11
  %fn_ptr = load i32 (i32)*, i32 (i32)** %cb1, align 8, !dbg !12
  %x3 = load i32, i32* %x2, align 4, !dbg !12
  %indirect_call = call i32 %fn_ptr(i32 %x3), !dbg !12
  ret i32 %indirect_call, !dbg !12
}

define i32 @main() !dbg !13 {
entry:
  %apply = call i32 @apply(i32 (i32)* @double_it, i32 21), !dbg !14
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 %apply), !dbg !14
  %apply1 = call i32 @apply(i32 (i32)* @triple_it, i32 7), !dbg !15
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt.1, i32 0, i32 0), i32 %apply1), !dbg !15
  ret i32 0, !dbg !15
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "005.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/functions/basic")
!4 = distinct !DISubprogram(name: "double_it", linkageName: "double_it", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "triple_it", linkageName: "triple_it", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = distinct !DISubprogram(name: "apply", linkageName: "apply", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 9, scope: !10)
!12 = !DILocation(line: 10, scope: !10)
!13 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 12, type: !5, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!14 = !DILocation(line: 13, scope: !13)
!15 = !DILocation(line: 14, scope: !13)
