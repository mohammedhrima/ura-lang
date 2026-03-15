; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/data_types/array01.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/data_types/array01.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [13 x i8] c"Hello world\0A\00", align 1
@output_fmt = private unnamed_addr constant [2 x i8] c"?\00", align 1

declare void @strcpy(i8*, i8*)

declare void @free(i8*)

define i32 @main() !dbg !4 {
entry:
  %str = alloca i8*, align 8, !dbg !7
  store i8* null, i8** %str, align 8, !dbg !7
  %heap = call i8* @calloc(i64 20, i64 1), !dbg !7
  store i8* %heap, i8** %str, align 8, !dbg !7
  %str1 = load i8*, i8** %str, align 8, !dbg !8
  call void @strcpy(i8* %str1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @STR0, i32 0, i32 0)), !dbg !8
  %str2 = load i8*, i8** %str, align 8, !dbg !9
  %ACCESS = getelementptr i8, i8* %str2, i32 0, !dbg !9
  store i8 114, i8* %ACCESS, align 1, !dbg !9
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !9
  %str3 = load i8*, i8** %str, align 8, !dbg !10
  call void @free(i8* %str3), !dbg !10
  ret i32 0, !dbg !10
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "array01.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/data_types")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
!9 = !DILocation(line: 9, scope: !4)
!10 = !DILocation(line: 11, scope: !4)
