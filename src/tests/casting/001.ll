; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/casting/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/casting/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"code \00", align 1
@STR1 = private unnamed_addr constant [11 x i8] c" as char: \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [21 x i8] c"code %d as char: %c\0A\00", align 1
@STR3 = private unnamed_addr constant [13 x i8] c"'S' as int: \00", align 1
@STR4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [16 x i8] c"'S' as int: %d\0A\00", align 1
@STR5 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@STR6 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@STR7 = private unnamed_addr constant [13 x i8] c"'d' - 'a' = \00", align 1
@STR8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [16 x i8] c"'d' - 'a' = %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %code = alloca i32, align 4, !dbg !7
  %grade = alloca i8, align 1, !dbg !7
  %c = alloca i8, align 1, !dbg !7
  %n = alloca i32, align 4, !dbg !7
  %left = alloca i8*, align 8, !dbg !7
  %right = alloca i8*, align 8, !dbg !7
  %diff = alloca i32, align 4, !dbg !7
  store i32 0, i32* %code, align 4, !dbg !7
  store i32 65, i32* %code, align 4, !dbg !7
  store i8 0, i8* %grade, align 1, !dbg !7
  %code1 = load i32, i32* %code, align 4, !dbg !7
  %as = trunc i32 %code1 to i8, !dbg !7
  store i8 %as, i8* %grade, align 1, !dbg !7
  %code2 = load i32, i32* %code, align 4, !dbg !7
  %grade3 = load i8, i8* %grade, align 1, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @output_fmt, i32 0, i32 0), i32 2, i32 %code2, i8 %grade3), !dbg !7
  store i8 0, i8* %c, align 1, !dbg !7
  store i8 83, i8* %c, align 1, !dbg !7
  store i32 0, i32* %n, align 4, !dbg !7
  %c4 = load i8, i8* %c, align 1, !dbg !7
  %as5 = sext i8 %c4 to i32, !dbg !7
  store i32 %as5, i32* %n, align 4, !dbg !7
  %n6 = load i32, i32* %n, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %n6), !dbg !7
  store i8* null, i8** %left, align 8, !dbg !7
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @STR5, i32 0, i32 0), i8** %left, align 8, !dbg !7
  store i8* null, i8** %right, align 8, !dbg !7
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @STR6, i32 0, i32 0), i8** %right, align 8, !dbg !7
  store i32 0, i32* %diff, align 4, !dbg !7
  %left7 = load i8*, i8** %left, align 8, !dbg !8
  %ACCESS = getelementptr i8, i8* %left7, i32 0, !dbg !8
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !8
  %as8 = sext i8 %ACC to i32, !dbg !8
  %right9 = load i8*, i8** %right, align 8, !dbg !8
  %ACCESS10 = getelementptr i8, i8* %right9, i32 0, !dbg !8
  %ACC11 = load i8, i8* %ACCESS10, align 1, !dbg !8
  %as12 = sext i8 %ACC11 to i32, !dbg !8
  %SUB = sub i32 %as8, %as12, !dbg !8
  store i32 %SUB, i32* %diff, align 4, !dbg !8
  %diff13 = load i32, i32* %diff, align 4, !dbg !8
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt.2, i32 0, i32 0), i32 1, i32 %diff13), !dbg !8
  ret i32 0, !dbg !8
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/casting")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 17, scope: !4)
