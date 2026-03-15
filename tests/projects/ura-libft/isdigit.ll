; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft/isdigit.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft/isdigit.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@output_fmt = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@STR1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1

define i1 @isdigit(i8 %c) !dbg !4 {
entry:
  %c1 = alloca i8, align 1, !dbg !7
  store i8 %c, i8* %c1, align 1, !dbg !7
  %c2 = load i8, i8* %c1, align 1, !dbg !7
  %GE = icmp sge i8 %c2, 49, !dbg !7
  %c3 = load i8, i8* %c1, align 1, !dbg !7
  %LE = icmp sle i8 %c3, 57, !dbg !7
  %AND = and i1 %GE, %LE, !dbg !7
  ret i1 %AND, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  br label %if.start, !dbg !9

if.start:                                         ; preds = %entry
  %isdigit = call i1 @isdigit(i8 49), !dbg !10
  br i1 %isdigit, label %if.then, label %if.else, !dbg !10

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0, !dbg !10

if.then:                                          ; preds = %if.start
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !10
  br label %if.end, !dbg !10

if.else:                                          ; preds = %if.start
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !10
  br label %if.end, !dbg !10
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "isdigit.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft")
!4 = distinct !DISubprogram(name: "isdigit", linkageName: "isdigit", scope: null, file: !3, line: 2, type: !5, scopeLine: 2, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 2, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 4, scope: !8)
!10 = !DILocation(line: 5, scope: !8)
