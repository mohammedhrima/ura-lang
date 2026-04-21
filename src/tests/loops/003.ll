; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/loops/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/loops/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [11 x i8] c"#.#..#.@.#\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"Open tiles: \00", align 1
@STR2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt = private unnamed_addr constant [16 x i8] c"Open tiles: %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %tiles = alloca i8*, align 8, !dbg !7
  %open = alloca i32, align 4, !dbg !7
  %i = alloca i32, align 4, !dbg !7
  store i8* null, i8** %tiles, align 8, !dbg !7
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR0, i32 0, i32 0), i8** %tiles, align 8, !dbg !7
  store i32 0, i32* %open, align 4, !dbg !7
  store i32 0, i32* %open, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  store i32 0, i32* %i, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %if.end, %if.then, %entry
  %i1 = load i32, i32* %i, align 4, !dbg !7
  %LT = icmp slt i32 %i1, 10, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  br label %if.start, !dbg !7

while.end:                                        ; preds = %while.start
  %open9 = load i32, i32* %open, align 4, !dbg !8
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %open9), !dbg !8
  ret i32 0, !dbg !8

if.start:                                         ; preds = %while.then
  %tiles2 = load i8*, i8** %tiles, align 8, !dbg !8
  %i3 = load i32, i32* %i, align 4, !dbg !8
  %ACCESS = getelementptr i8, i8* %tiles2, i32 %i3, !dbg !8
  %ACC = load i8, i8* %ACCESS, align 1, !dbg !8
  %EQ = icmp eq i8 %ACC, 35, !dbg !8
  br i1 %EQ, label %if.then, label %if.end, !dbg !8

if.end:                                           ; preds = %if.start
  %open5 = load i32, i32* %open, align 4, !dbg !8
  %ADD6 = add i32 %open5, 1, !dbg !8
  store i32 %ADD6, i32* %open, align 4, !dbg !8
  %i7 = load i32, i32* %i, align 4, !dbg !8
  %ADD8 = add i32 %i7, 1, !dbg !8
  store i32 %ADD8, i32* %i, align 4, !dbg !8
  br label %while.start, !dbg !8

if.then:                                          ; preds = %if.start
  %i4 = load i32, i32* %i, align 4, !dbg !8
  %ADD = add i32 %i4, 1, !dbg !8
  store i32 %ADD, i32* %i, align 4, !dbg !8
  br label %while.start, !dbg !8
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/loops")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
