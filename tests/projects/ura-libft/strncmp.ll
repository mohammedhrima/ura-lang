; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft/strncmp.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft/strncmp.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [4 x i8] c"abd\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @strncmp(i8* %left, i8* %right, i32 %n) !dbg !4 {
entry:
  %left1 = alloca i8*, align 8, !dbg !7
  %right2 = alloca i8*, align 8, !dbg !7
  %n3 = alloca i32, align 4, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  store i8* %left, i8** %left1, align 8, !dbg !7
  store i8* %right, i8** %right2, align 8, !dbg !7
  store i32 %n, i32* %n3, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %n4 = load i32, i32* %n3, align 4, !dbg !7
  %EQ = icmp eq i32 %n4, 0, !dbg !7
  br i1 %EQ, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.start
  br label %while.start, !dbg !7

if.then:                                          ; preds = %if.start
  ret i32 0, !dbg !7

while.start:                                      ; preds = %while.then, %if.end
  %i5 = load i32, i32* %i, align 4, !dbg !7
  %n6 = load i32, i32* %n3, align 4, !dbg !7
  %LT = icmp slt i32 %i5, %n6, !dbg !7
  %left7 = load i8*, i8** %left1, align 8, !dbg !8
  %i8 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS = getelementptr i8, i8* %left7, i32 %i8, !dbg !8
  %right9 = load i8*, i8** %right2, align 8, !dbg !8
  %i10 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS11 = getelementptr i8, i8* %right9, i32 %i10, !dbg !8
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !8
  %ACC12 = load i8, i8* %ACCESS11, align 1, !dbg !8
  %EQ13 = icmp eq i8 %ACC, %ACC12, !dbg !8
  %AND = and i1 %LT, %EQ13, !dbg !8
  %left14 = load i8*, i8** %left1, align 8, !dbg !8
  %i15 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS16 = getelementptr i8, i8* %left14, i32 %i15, !dbg !8
  %ACC17 = load i8, i8* %ACCESS16, align 1, !dbg !8
  %NEQ = icmp ne i8 %ACC17, 0, !dbg !8
  %AND18 = and i1 %AND, %NEQ, !dbg !8
  br i1 %AND18, label %while.then, label %while.end, !dbg !8

while.then:                                       ; preds = %while.start
  %current = load i32, i32* %i, align 4, !dbg !8
  %ADD = add i32 %current, 1, !dbg !8
  store i32 %ADD, i32* %i, align 4, !dbg !8
  br label %while.start, !dbg !8

while.end:                                        ; preds = %while.start
  br label %if.start19, !dbg !8

if.start19:                                       ; preds = %while.end
  %i22 = load i32, i32* %i, align 4, !dbg !8
  %n23 = load i32, i32* %n3, align 4, !dbg !8
  %EQ24 = icmp eq i32 %i22, %n23, !dbg !8
  br i1 %EQ24, label %if.then21, label %if.end20, !dbg !8

if.end20:                                         ; preds = %if.start19
  %left25 = load i8*, i8** %left1, align 8, !dbg !9
  %i26 = load i32, i32* %i, align 4, !dbg !9
  %ACCESS27 = getelementptr i8, i8* %left25, i32 %i26, !dbg !9
  %ACC28 = load i8, i8* %ACCESS27, align 1, !dbg !9
  %as = sext i8 %ACC28 to i32, !dbg !9
  %right29 = load i8*, i8** %right2, align 8, !dbg !9
  %i30 = load i32, i32* %i, align 4, !dbg !9
  %ACCESS31 = getelementptr i8, i8* %right29, i32 %i30, !dbg !9
  %ACC32 = load i8, i8* %ACCESS31, align 1, !dbg !9
  %as33 = sext i8 %ACC32 to i32, !dbg !9
  %SUB = sub i32 %as, %as33, !dbg !9
  ret i32 %SUB, !dbg !9

if.then21:                                        ; preds = %if.start19
  ret i32 0, !dbg !8
}

define i32 @main() !dbg !10 {
entry:
  %len = alloca i32, align 4, !dbg !11
  store i32 0, i32* %len, align 4, !dbg !11
  %strncmp = call i32 @strncmp(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @STR0, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @STR1, i32 0, i32 0), i32 4), !dbg !12
  store i32 %strncmp, i32* %len, align 4, !dbg !12
  %len1 = load i32, i32* %len, align 4, !dbg !12
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %len1), !dbg !12
  ret i32 0, !dbg !12
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "strncmp.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/projects/ura-libft")
!4 = distinct !DISubprogram(name: "strncmp", linkageName: "strncmp", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
!9 = !DILocation(line: 11, scope: !4)
!10 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 13, type: !5, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!11 = !DILocation(line: 13, scope: !10)
!12 = !DILocation(line: 14, scope: !10)
