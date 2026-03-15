; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [28 x i8] c"S-rank hero \E2\80\94 unstoppable\00", align 1
@STR1 = private unnamed_addr constant [14 x i8] c"keep training\00", align 1

declare i32 @puts(i8*)

define i1 @is_s_rank(i8 %grade) !dbg !4 {
entry:
  %grade1 = alloca i8, align 1, !dbg !7
  store i8 %grade, i8* %grade1, align 1, !dbg !7
  %grade2 = load i8, i8* %grade1, align 1, !dbg !7
  %EQ = icmp eq i8 %grade2, 83, !dbg !7
  ret i1 %EQ, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %grade = alloca i8, align 1, !dbg !9
  store i8 0, i8* %grade, align 1, !dbg !9
  store i8 83, i8* %grade, align 1, !dbg !9
  br label %if.start, !dbg !9

if.start:                                         ; preds = %entry
  %grade1 = load i8, i8* %grade, align 1, !dbg !10
  %is_s_rank = call i1 @is_s_rank(i8 %grade1), !dbg !10
  br i1 %is_s_rank, label %if.then, label %if.else, !dbg !10

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0, !dbg !11

if.then:                                          ; preds = %if.start
  %puts = call i32 @puts(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @STR0, i32 0, i32 0)), !dbg !12
  br label %if.end, !dbg !12

if.else:                                          ; preds = %if.start
  %puts2 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR1, i32 0, i32 0)), !dbg !11
  br label %if.end, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/builtins/c-funcs")
!4 = distinct !DISubprogram(name: "is_s_rank", linkageName: "is_s_rank", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 8, scope: !8)
!10 = !DILocation(line: 10, scope: !8)
!11 = !DILocation(line: 13, scope: !8)
!12 = !DILocation(line: 11, scope: !8)
