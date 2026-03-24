; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [3 x i8] c"?\0A\00", align 1

declare void @free(i8*)

define i32 @main() !dbg !4 {
entry:
  %str = alloca i8*, align 8, !dbg !7
  store i8* null, i8** %str, align 8, !dbg !7
  %heap = call i8* @calloc(i64 8, i64 1), !dbg !7
  store i8* %heap, i8** %str, align 8, !dbg !7
  %str1 = load i8*, i8** %str, align 8, !dbg !8
  %ACCESS = getelementptr i8, i8* %str1, i32 0, !dbg !8
  store i8 68, i8* %ACCESS, align 1, !dbg !8
  %str2 = load i8*, i8** %str, align 8, !dbg !9
  %ACCESS3 = getelementptr i8, i8* %str2, i32 1, !dbg !9
  store i8 85, i8* %ACCESS3, align 1, !dbg !9
  %str4 = load i8*, i8** %str, align 8, !dbg !10
  %ACCESS5 = getelementptr i8, i8* %str4, i32 2, !dbg !10
  store i8 78, i8* %ACCESS5, align 1, !dbg !10
  %str6 = load i8*, i8** %str, align 8, !dbg !11
  %ACCESS7 = getelementptr i8, i8* %str6, i32 3, !dbg !11
  store i8 71, i8* %ACCESS7, align 1, !dbg !11
  %str8 = load i8*, i8** %str, align 8, !dbg !12
  %ACCESS9 = getelementptr i8, i8* %str8, i32 4, !dbg !12
  store i8 69, i8* %ACCESS9, align 1, !dbg !12
  %str10 = load i8*, i8** %str, align 8, !dbg !13
  %ACCESS11 = getelementptr i8, i8* %str10, i32 5, !dbg !13
  store i8 79, i8* %ACCESS11, align 1, !dbg !13
  %str12 = load i8*, i8** %str, align 8, !dbg !14
  %ACCESS13 = getelementptr i8, i8* %str12, i32 6, !dbg !14
  store i8 78, i8* %ACCESS13, align 1, !dbg !14
  %str14 = load i8*, i8** %str, align 8, !dbg !15
  %ACCESS15 = getelementptr i8, i8* %str14, i32 7, !dbg !15
  store i8 0, i8* %ACCESS15, align 1, !dbg !15
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !15
  %str16 = load i8*, i8** %str, align 8, !dbg !16
  call void @free(i8* %str16), !dbg !16
  ret i32 0, !dbg !16
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
!9 = !DILocation(line: 8, scope: !4)
!10 = !DILocation(line: 9, scope: !4)
!11 = !DILocation(line: 10, scope: !4)
!12 = !DILocation(line: 11, scope: !4)
!13 = !DILocation(line: 12, scope: !4)
!14 = !DILocation(line: 13, scope: !4)
!15 = !DILocation(line: 14, scope: !4)
!16 = !DILocation(line: 16, scope: !4)
