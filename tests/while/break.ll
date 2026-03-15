; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/while/break.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/while/break.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@output_fmt = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %a = alloca i32, align 4, !dbg !7
  store i32 0, i32* %a, align 4, !dbg !7
  store i32 1, i32* %a, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %if.end, %entry
  %a1 = load i32, i32* %a, align 4, !dbg !7
  %LT = icmp slt i32 %a1, 10, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %a2 = load i32, i32* %a, align 4, !dbg !7
  %ADD = add i32 %a2, 1, !dbg !7
  store i32 %ADD, i32* %a, align 4, !dbg !7
  br label %if.start, !dbg !7

while.end:                                        ; preds = %if.then, %while.start
  %a4 = load i32, i32* %a, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %a4), !dbg !7
  ret i32 0, !dbg !7

if.start:                                         ; preds = %while.then
  %a3 = load i32, i32* %a, align 4, !dbg !7
  %EQ = icmp eq i32 %a3, 2, !dbg !7
  br i1 %EQ, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.start
  br label %while.start, !dbg !7

if.then:                                          ; preds = %if.start
  br label %while.end, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "break.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/while")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
