; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft/strcmp.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft/strcmp.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR3 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @strcmp(i8* %left, i8* %right) !dbg !4 {
entry:
  %left1 = alloca i8*, align 8, !dbg !7
  %right2 = alloca i8*, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  store i8* %left, i8** %left1, align 8, !dbg !7
  store i8* %right, i8** %right2, align 8, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.then, %entry
  %left3 = load i8*, i8** %left1, align 8, !dbg !8
  %i4 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS = getelementptr i8, i8* %left3, i32 %i4, !dbg !8
  %right5 = load i8*, i8** %right2, align 8, !dbg !8
  %i6 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS7 = getelementptr i8, i8* %right5, i32 %i6, !dbg !8
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !8
  %ACC8 = load i8, i8* %ACCESS7, align 1, !dbg !8
  %EQ = icmp eq i8 %ACC, %ACC8, !dbg !8
  %left9 = load i8*, i8** %left1, align 8, !dbg !8
  %i10 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS11 = getelementptr i8, i8* %left9, i32 %i10, !dbg !8
  %ACC12 = load i8, i8* %ACCESS11, align 1, !dbg !8
  %NEQ = icmp ne i8 %ACC12, 0, !dbg !8
  %AND = and i1 %EQ, %NEQ, !dbg !8
  br i1 %AND, label %while.then, label %while.end, !dbg !8

while.then:                                       ; preds = %while.start
  %current = load i32, i32* %i, align 4, !dbg !8
  %ADD = add i32 %current, 1, !dbg !8
  store i32 %ADD, i32* %i, align 4, !dbg !8
  br label %while.start, !dbg !8

while.end:                                        ; preds = %while.start
  %left13 = load i8*, i8** %left1, align 8, !dbg !9
  %i14 = load i32, i32* %i, align 4, !dbg !9
  %ACCESS15 = getelementptr i8, i8* %left13, i32 %i14, !dbg !9
  %ACC16 = load i8, i8* %ACCESS15, align 1, !dbg !9
  %as = sext i8 %ACC16 to i32, !dbg !9
  %right17 = load i8*, i8** %right2, align 8, !dbg !9
  %i18 = load i32, i32* %i, align 4, !dbg !9
  %ACCESS19 = getelementptr i8, i8* %right17, i32 %i18, !dbg !9
  %ACC20 = load i8, i8* %ACCESS19, align 1, !dbg !9
  %as21 = sext i8 %ACC20 to i32, !dbg !9
  %SUB = sub i32 %as, %as21, !dbg !9
  ret i32 %SUB, !dbg !9
}

define i32 @main() !dbg !10 {
entry:
  %len = alloca i32, align 4, !dbg !11
  store i32 0, i32* %len, align 4, !dbg !11
  %strcmp = call i32 @strcmp(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @STR0, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @STR1, i32 0, i32 0)), !dbg !12
  store i32 %strcmp, i32* %len, align 4, !dbg !12
  %len1 = load i32, i32* %len, align 4, !dbg !12
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %len1), !dbg !12
  ret i32 0, !dbg !12
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "strcmp.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft")
!4 = distinct !DISubprogram(name: "strcmp", linkageName: "strcmp", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 5, scope: !4)
!9 = !DILocation(line: 7, scope: !4)
!10 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 9, scope: !10)
!12 = !DILocation(line: 10, scope: !10)
