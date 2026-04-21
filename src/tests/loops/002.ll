; ModuleID = '/Users/hrimamohammed/Desktop/personal/ura-lang/tests/loops/002.ura'
source_filename = "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/loops/002.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [18 x i8] c"Bag full at slot \00", align 1
@STR1 = private unnamed_addr constant [11 x i8] c" \E2\80\94 stop\0A\00", align 1
@output_fmt = private unnamed_addr constant [30 x i8] c"Bag full at slot %d \E2\80\94 stop\0A\00", align 1
@STR2 = private unnamed_addr constant [14 x i8] c"Packing slot \00", align 1
@STR3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@output_fmt.1 = private unnamed_addr constant [17 x i8] c"Packing slot %d\0A\00", align 1

define i32 @main() !dbg !4 {
entry:
  %slot = alloca i32, align 4, !dbg !7
  store i32 0, i32* %slot, align 4, !dbg !7
  store i32 0, i32* %slot, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %if.end, %entry
  %slot1 = load i32, i32* %slot, align 4, !dbg !7
  %LT = icmp slt i32 %slot1, 8, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  br label %if.start, !dbg !7

while.end:                                        ; preds = %if.then, %while.start
  ret i32 0, !dbg !7

if.start:                                         ; preds = %while.then
  %slot2 = load i32, i32* %slot, align 4, !dbg !7
  %EQ = icmp eq i32 %slot2, 5, !dbg !7
  br i1 %EQ, label %if.then, label %if.end, !dbg !7

if.end:                                           ; preds = %if.start
  %slot4 = load i32, i32* %slot, align 4, !dbg !7
  %0 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @output_fmt.1, i32 0, i32 0), i32 1, i32 %slot4), !dbg !7
  %slot5 = load i32, i32* %slot, align 4, !dbg !7
  %ADD = add i32 %slot5, 1, !dbg !7
  store i32 %ADD, i32* %slot, align 4, !dbg !7
  br label %while.start, !dbg !7

if.then:                                          ; preds = %if.start
  %slot3 = load i32, i32* %slot, align 4, !dbg !7
  %1 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @output_fmt, i32 0, i32 0), i32 1, i32 %slot3), !dbg !7
  br label %while.end, !dbg !7
}

declare i32 @printf(i8*, i32, ...)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "002.ura", directory: "/Users/hrimamohammed/Desktop/personal/ura-lang/tests/loops")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
