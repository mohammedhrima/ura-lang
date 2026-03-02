; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/libft/strcmp.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/libft/strcmp.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@STR2 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i32 @strcmp(ptr %left, ptr %right) !dbg !4 {
entry:
  %left1 = alloca ptr, align 8, !dbg !7
  %right2 = alloca ptr, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  store i32 0, ptr %i, align 4, !dbg !7
  store ptr null, ptr %right2, align 8, !dbg !7
  store ptr null, ptr %left1, align 8, !dbg !7
  store ptr %left, ptr %left1, align 8, !dbg !7
  store ptr %right, ptr %right2, align 8, !dbg !7
  store i32 0, ptr %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.then, %entry
  %left3 = load ptr, ptr %left1, align 8, !dbg !8
  %i4 = load i32, ptr %i, align 4, !dbg !8
  %ACCESS = getelementptr i8, ptr %left3, i32 %i4, !dbg !8
  %right5 = load ptr, ptr %right2, align 8, !dbg !8
  %i6 = load i32, ptr %i, align 4, !dbg !8
  %ACCESS7 = getelementptr i8, ptr %right5, i32 %i6, !dbg !8
  %ACC = load i8, ptr %ACCESS, align 1, !dbg !8
  %ACC8 = load i8, ptr %ACCESS7, align 1, !dbg !8
  %EQ = icmp eq i8 %ACC, %ACC8, !dbg !8
  %left9 = load ptr, ptr %left1, align 8, !dbg !8
  %i10 = load i32, ptr %i, align 4, !dbg !8
  %ACCESS11 = getelementptr i8, ptr %left9, i32 %i10, !dbg !8
  %ACC12 = load i8, ptr %ACCESS11, align 1, !dbg !8
  %NEQ = icmp ne i8 %ACC12, 0, !dbg !8
  %AND = and i1 %EQ, %NEQ, !dbg !8
  br i1 %AND, label %while.then, label %while.end, !dbg !8

while.then:                                       ; preds = %while.start
  %current = load i32, ptr %i, align 4, !dbg !8
  %ADD = add i32 %current, 1, !dbg !8
  store i32 %ADD, ptr %i, align 4, !dbg !8
  br label %while.start, !dbg !8

while.end:                                        ; preds = %while.start
  %left13 = load ptr, ptr %left1, align 8, !dbg !9
  %i14 = load i32, ptr %i, align 4, !dbg !9
  %ACCESS15 = getelementptr i8, ptr %left13, i32 %i14, !dbg !9
  %ACC16 = load i8, ptr %ACCESS15, align 1, !dbg !9
  %as = sext i8 %ACC16 to i32, !dbg !9
  %right17 = load ptr, ptr %right2, align 8, !dbg !9
  %i18 = load i32, ptr %i, align 4, !dbg !9
  %ACCESS19 = getelementptr i8, ptr %right17, i32 %i18, !dbg !9
  %ACC20 = load i8, ptr %ACCESS19, align 1, !dbg !9
  %as21 = sext i8 %ACC20 to i32, !dbg !9
  %SUB = sub i32 %as, %as21, !dbg !9
  ret i32 %SUB, !dbg !9
}

define i32 @main() !dbg !10 {
entry:
  %len = alloca i32, align 4, !dbg !11
  store i32 0, ptr %len, align 4, !dbg !11
  %strcmp = call i32 @strcmp(ptr @STR0, ptr @STR1), !dbg !12
  store i32 %strcmp, ptr %len, align 4, !dbg !12
  %len1 = load i32, ptr %len, align 4, !dbg !13
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR2, i32 1, i32 %len1), !dbg !13
  ret i32 0, !dbg !13
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "strcmp.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/libft")
!4 = distinct !DISubprogram(name: "strcmp", linkageName: "strcmp", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 5, scope: !4)
!9 = !DILocation(line: 7, scope: !4)
!10 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2)
!11 = !DILocation(line: 9, scope: !10)
!12 = !DILocation(line: 10, scope: !10)
!13 = !DILocation(line: 11, scope: !10)
