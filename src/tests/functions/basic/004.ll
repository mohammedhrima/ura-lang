; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/functions/basic/004.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/functions/basic/004.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @add(i32 %a, i32 %b) !dbg !4 {
entry:
  %a1 = alloca i32, align 4, !dbg !7
  %b2 = alloca i32, align 4, !dbg !7
  store i32 %a, i32* %a1, align 4, !dbg !7
  store i32 %b, i32* %b2, align 4, !dbg !7
  %a3 = load i32, i32* %a1, align 4, !dbg !7
  %b4 = load i32, i32* %b2, align 4, !dbg !7
  %ADD = add i32 %a3, %b4, !dbg !7
  ret i32 %ADD, !dbg !7
}

define i32 @sub(i32 %a, i32 %b) !dbg !8 {
entry:
  %a1 = alloca i32, align 4, !dbg !9
  %b2 = alloca i32, align 4, !dbg !9
  store i32 %a, i32* %a1, align 4, !dbg !9
  store i32 %b, i32* %b2, align 4, !dbg !9
  %a3 = load i32, i32* %a1, align 4, !dbg !9
  %b4 = load i32, i32* %b2, align 4, !dbg !9
  %SUB = sub i32 %a3, %b4, !dbg !9
  ret i32 %SUB, !dbg !9
}

define i32 @main() !dbg !10 {
entry:
  %op = alloca i32 (i32, i32)*, align 8, !dbg !11
  store i32 (i32, i32)* null, i32 (i32, i32)** %op, align 8, !dbg !11
  store i32 (i32, i32)* @add, i32 (i32, i32)** %op, align 8, !dbg !11
  %fn_ptr = load i32 (i32, i32)*, i32 (i32, i32)** %op, align 8, !dbg !12
  %indirect_call = call i32 %fn_ptr(i32 2, i32 3), !dbg !12
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 %indirect_call), !dbg !12
  store i32 (i32, i32)* @sub, i32 (i32, i32)** %op, align 8, !dbg !12
  %fn_ptr1 = load i32 (i32, i32)*, i32 (i32, i32)** %op, align 8, !dbg !13
  %indirect_call2 = call i32 %fn_ptr1(i32 10, i32 4), !dbg !13
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt.1, i32 0, i32 0), i32 %indirect_call2), !dbg !13
  ret i32 0, !dbg !13
}

declare i32 @printf(i8*, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "004.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/src/tests/functions/basic")
!4 = distinct !DISubprogram(name: "add", linkageName: "add", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "sub", linkageName: "sub", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 6, scope: !8)
!10 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 9, scope: !10)
!12 = !DILocation(line: 11, scope: !10)
!13 = !DILocation(line: 14, scope: !10)
