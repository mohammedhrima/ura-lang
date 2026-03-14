; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [13 x i8] c"hello world\0A\00", align 1
@STR1 = private unnamed_addr constant [6 x i8] c"alice\00", align 1
@STR2 = private unnamed_addr constant [10 x i8] c"name: %s\0A\00", align 1
@STR3 = private unnamed_addr constant [9 x i8] c"age: %d\0A\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"grade: %c\0A\00", align 1
@STR5 = private unnamed_addr constant [18 x i8] c"name: %s age: %d\0A\00", align 1
@STR6 = private unnamed_addr constant [12 x i8] c"x + y = %d\0A\00", align 1
@STR7 = private unnamed_addr constant [14 x i8] c"negative: %d\0A\00", align 1
@STR8 = private unnamed_addr constant [10 x i8] c"zero: %d\0A\00", align 1
@STR9 = private unnamed_addr constant [5 x i8] c"john\00", align 1
@STR10 = private unnamed_addr constant [4 x i8] c"doe\00", align 1
@STR11 = private unnamed_addr constant [18 x i8] c"full name: %s %s\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @STR0, i32 0, i32 0), i32 0), !dbg !7
  %name = alloca i8*, align 8, !dbg !7
  store i8* null, i8** %name, align 8, !dbg !7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR1, i32 0, i32 0), i8** %name, align 8, !dbg !7
  %name1 = load i8*, i8** %name, align 8, !dbg !8
  %printf2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR2, i32 0, i32 0), i32 1, i8* %name1), !dbg !8
  %age = alloca i32, align 4, !dbg !8
  store i32 0, i32* %age, align 4, !dbg !8
  store i32 30, i32* %age, align 4, !dbg !8
  %age3 = load i32, i32* %age, align 4, !dbg !9
  %printf4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @STR3, i32 0, i32 0), i32 1, i32 %age3), !dbg !9
  %grade = alloca i8, align 1, !dbg !9
  store i8 0, i8* %grade, align 1, !dbg !9
  store i8 65, i8* %grade, align 1, !dbg !9
  %grade5 = load i8, i8* %grade, align 1, !dbg !10
  %printf6 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR4, i32 0, i32 0), i32 1, i8 %grade5), !dbg !10
  %name7 = load i8*, i8** %name, align 8, !dbg !11
  %age8 = load i32, i32* %age, align 4, !dbg !11
  %printf9 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @STR5, i32 0, i32 0), i32 2, i8* %name7, i32 %age8), !dbg !11
  %x = alloca i32, align 4, !dbg !11
  store i32 0, i32* %x, align 4, !dbg !11
  store i32 10, i32* %x, align 4, !dbg !11
  %y = alloca i32, align 4, !dbg !11
  store i32 0, i32* %y, align 4, !dbg !11
  store i32 20, i32* %y, align 4, !dbg !11
  %x10 = load i32, i32* %x, align 4, !dbg !12
  %y11 = load i32, i32* %y, align 4, !dbg !12
  %ADD = add i32 %x10, %y11, !dbg !12
  %printf12 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @STR6, i32 0, i32 0), i32 1, i32 %ADD), !dbg !12
  %n = alloca i32, align 4, !dbg !12
  store i32 0, i32* %n, align 4, !dbg !12
  store i32 -5, i32* %n, align 4, !dbg !12
  %n13 = load i32, i32* %n, align 4, !dbg !13
  %printf14 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR7, i32 0, i32 0), i32 1, i32 %n13), !dbg !13
  %printf15 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @STR8, i32 0, i32 0), i32 1, i32 0), !dbg !14
  %first = alloca i8*, align 8, !dbg !14
  store i8* null, i8** %first, align 8, !dbg !14
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @STR9, i32 0, i32 0), i8** %first, align 8, !dbg !14
  %last = alloca i8*, align 8, !dbg !14
  store i8* null, i8** %last, align 8, !dbg !14
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @STR10, i32 0, i32 0), i8** %last, align 8, !dbg !14
  %first16 = load i8*, i8** %first, align 8, !dbg !15
  %last17 = load i8*, i8** %last, align 8, !dbg !15
  %printf18 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @STR11, i32 0, i32 0), i32 2, i8* %first16, i8* %last17), !dbg !15
  ret i32 0, !dbg !15
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
!9 = !DILocation(line: 11, scope: !4)
!10 = !DILocation(line: 14, scope: !4)
!11 = !DILocation(line: 16, scope: !4)
!12 = !DILocation(line: 20, scope: !4)
!13 = !DILocation(line: 23, scope: !4)
!14 = !DILocation(line: 25, scope: !4)
!15 = !DILocation(line: 29, scope: !4)
