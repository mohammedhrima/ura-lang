; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/conditionals/003.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/conditionals/003.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"Easy\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"Easy\0A\00", align 1
@STR1 = private unnamed_addr constant [8 x i8] c"Normal\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [8 x i8] c"Normal\0A\00", align 1
@STR2 = private unnamed_addr constant [6 x i8] c"Hard\0A\00", align 1
@output_fmt.2 = private unnamed_addr constant [6 x i8] c"Hard\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %floor = alloca i32, align 4, !dbg !7
  store i32 0, i32* %floor, align 4, !dbg !7
  store i32 6, i32* %floor, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %floor1 = load i32, i32* %floor, align 4, !dbg !7
  %LT = icmp slt i32 %floor1, 3, !dbg !7
  br i1 %LT, label %if.then, label %elif.start, !dbg !7

if.end:                                           ; preds = %if.else, %elif.then, %if.then
  ret i32 0, !dbg !7

if.then:                                          ; preds = %if.start
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 0), !dbg !7
  br label %if.end, !dbg !7

elif.start:                                       ; preds = %if.start
  %floor2 = load i32, i32* %floor, align 4, !dbg !7
  %LT3 = icmp slt i32 %floor2, 7, !dbg !7
  br i1 %LT3, label %elif.then, label %if.else, !dbg !7

elif.then:                                        ; preds = %elif.start
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @output_fmt.1, i32 0, i32 0), i32 0), !dbg !7
  br label %if.end, !dbg !7

if.else:                                          ; preds = %elif.start
  %2 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt.2, i32 0, i32 0), i32 0), !dbg !7
  br label %if.end, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "003.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/conditionals")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
