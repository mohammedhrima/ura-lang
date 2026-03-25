; ModuleID = 'tests/dungeon/floor10a.ura'
source_filename = "tests/dungeon/floor10a.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare void @free(i8*)

define i32 @main() !dbg !4 {
entry:
  %map = alloca i8*, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  %buf = alloca i8*, align 8, !dbg !7
  store i8* null, i8** %map, align 8, !dbg !7
  %heap = call i8* @calloc(i64 51, i64 1), !dbg !7
  store i8* %heap, i8** %map, align 8, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.then, %entry
  %i1 = load i32, i32* %i, align 4, !dbg !7
  %LT = icmp slt i32 %i1, 50, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %map2 = load i8*, i8** %map, align 8, !dbg !8
  %i3 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS = getelementptr i8, i8* %map2, i32 %i3, !dbg !8
  store i8 46, i8* %ACCESS, align 1, !dbg !8
  %i4 = load i32, i32* %i, align 4, !dbg !8
  %ADD = add i32 %i4, 1, !dbg !8
  store i32 %ADD, i32* %i, align 4, !dbg !8
  br label %while.start, !dbg !8

while.end:                                        ; preds = %while.start
  %map5 = load i8*, i8** %map, align 8, !dbg !9
  %ACCESS6 = getelementptr i8, i8* %map5, i32 25, !dbg !9
  store i8 64, i8* %ACCESS6, align 1, !dbg !9
  %map7 = load i8*, i8** %map, align 8, !dbg !10
  %ACCESS8 = getelementptr i8, i8* %map7, i32 50, !dbg !10
  store i8 0, i8* %ACCESS8, align 1, !dbg !10
  %map9 = load i8*, i8** %map, align 8, !dbg !10
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %map9), !dbg !10
  %map10 = load i8*, i8** %map, align 8, !dbg !11
  call void @free(i8* %map10), !dbg !11
  store i8* null, i8** %buf, align 8, !dbg !7
  %stack = alloca [16 x i8], align 1, !dbg !7
  %stack11 = getelementptr [16 x i8], [16 x i8]* %stack, i32 0, i32 0, !dbg !7
  store i8* %stack11, i8** %buf, align 8, !dbg !7
  %buf12 = load i8*, i8** %buf, align 8, !dbg !12
  %ACCESS13 = getelementptr i8, i8* %buf12, i32 0, !dbg !12
  store i8 72, i8* %ACCESS13, align 1, !dbg !12
  %buf14 = load i8*, i8** %buf, align 8, !dbg !13
  %ACCESS15 = getelementptr i8, i8* %buf14, i32 1, !dbg !13
  store i8 80, i8* %ACCESS15, align 1, !dbg !13
  %buf16 = load i8*, i8** %buf, align 8, !dbg !14
  %ACCESS17 = getelementptr i8, i8* %buf16, i32 2, !dbg !14
  store i8 0, i8* %ACCESS17, align 1, !dbg !14
  %buf18 = load i8*, i8** %buf, align 8, !dbg !14
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i8* %buf18), !dbg !14
  ret i32 0, !dbg !14
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor10a.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
!9 = !DILocation(line: 10, scope: !4)
!10 = !DILocation(line: 11, scope: !4)
!11 = !DILocation(line: 13, scope: !4)
!12 = !DILocation(line: 16, scope: !4)
!13 = !DILocation(line: 17, scope: !4)
!14 = !DILocation(line: 18, scope: !4)
