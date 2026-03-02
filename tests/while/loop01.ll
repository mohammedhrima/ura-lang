; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/while/loop01.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/while/loop01.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %a = alloca i32, align 4, !dbg !7
  store i32 0, ptr %a, align 4, !dbg !7
  store i32 1, ptr %a, align 4, !dbg !7
  br label %while.start, !dbg !7

while.start:                                      ; preds = %while.then, %entry
  %a1 = load i32, ptr %a, align 4, !dbg !7
  %LT = icmp slt i32 %a1, 10, !dbg !7
  br i1 %LT, label %while.then, label %while.end, !dbg !7

while.then:                                       ; preds = %while.start
  %a2 = load i32, ptr %a, align 4, !dbg !7
  %ADD = add i32 %a2, 1, !dbg !7
  store i32 %ADD, ptr %a, align 4, !dbg !7
  br label %while.start, !dbg !7

while.end:                                        ; preds = %while.start
  %a3 = load i32, ptr %a, align 4, !dbg !8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 1, i32 %a3), !dbg !8
  ret i32 0, !dbg !8
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "loop01.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/while")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
