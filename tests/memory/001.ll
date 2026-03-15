; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory/001.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory/001.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"Map: \00", align 1
@STR1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [9 x i8] c"Map: %s\0A\00", align 1

declare void @free(i8*)

define i32 @main() !dbg !4 {
entry:
  %map = alloca i8*, align 8, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  store i8* null, i8** %map, align 8, !dbg !7
  %heap = call i8* @calloc(i64 50, i64 1), !dbg !7
  store i8* %heap, i8** %map, align 8, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.then, %entry
  %i1 = load i32, i32* %i, align 4, !dbg !7
  %LT = icmp slt i32 %i1, 49, !dbg !7
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
  %ACCESS6 = getelementptr i8, i8* %map5, i32 24, !dbg !9
  store i8 64, i8* %ACCESS6, align 1, !dbg !9
  %map7 = load i8*, i8** %map, align 8, !dbg !10
  %ACCESS8 = getelementptr i8, i8* %map7, i32 49, !dbg !10
  store i8 0, i8* %ACCESS8, align 1, !dbg !10
  %map9 = load i8*, i8** %map, align 8, !dbg !10
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @output_fmt, i32 0, i32 0), i32 1, i8* %map9), !dbg !10
  %map10 = load i8*, i8** %map, align 8, !dbg !11
  call void @free(i8* %map10), !dbg !11
  ret i32 0, !dbg !11
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "001.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/memory")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = !DILocation(line: 9, scope: !4)
!9 = !DILocation(line: 11, scope: !4)
!10 = !DILocation(line: 12, scope: !4)
!11 = !DILocation(line: 14, scope: !4)
