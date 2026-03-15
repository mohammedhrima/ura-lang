; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft/strlen.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft/strlen.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR2 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @strlen(i8* %str) !dbg !4 {
entry:
  %str1 = alloca i8*, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  store i8* %str, i8** %str1, align 8, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.then, %entry
  %str2 = load i8*, i8** %str1, align 8, !dbg !8
  %i3 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS = getelementptr i8, i8* %str2, i32 %i3, !dbg !8
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !8
  %NEQ = icmp ne i8 %ACC, 0, !dbg !8
  br i1 %NEQ, label %while.then, label %while.end, !dbg !8

while.then:                                       ; preds = %while.start
  %current = load i32, i32* %i, align 4, !dbg !8
  %ADD = add i32 %current, 1, !dbg !8
  store i32 %ADD, i32* %i, align 4, !dbg !8
  br label %while.start, !dbg !8

while.end:                                        ; preds = %while.start
  %i4 = load i32, i32* %i, align 4, !dbg !8
  ret i32 %i4, !dbg !8
}

define i32 @main() !dbg !9 {
entry:
  %len = alloca i32, align 4, !dbg !10
  store i32 0, i32* %len, align 4, !dbg !10
  %strlen = call i32 @strlen(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @STR0, i32 0, i32 0)), !dbg !11
  store i32 %strlen, i32* %len, align 4, !dbg !11
  %len1 = load i32, i32* %len, align 4, !dbg !11
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %len1), !dbg !11
  ret i32 0, !dbg !11
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "strlen.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft")
!4 = distinct !DISubprogram(name: "strlen", linkageName: "strlen", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 5, scope: !4)
!9 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 9, scope: !9)
!11 = !DILocation(line: 10, scope: !9)
