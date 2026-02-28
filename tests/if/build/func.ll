; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/if/func.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/if/func.ura"
target triple = "arm64-apple-macosx16.0.0"

define i1 @check() !dbg !4 {
entry:
  ret i1 true, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %a = alloca i32, align 4, !dbg !9
  store i32 0, ptr %a, align 4, !dbg !9
  store i32 1, ptr %a, align 4, !dbg !9
  br label %if.start, !dbg !9

if.start:                                         ; preds = %entry
  %check = call i1 @check(), !dbg !10
  br i1 %check, label %if.then, label %elif.start, !dbg !10

if.end:                                           ; No predecessors!
  ret i32 0, !dbg !10

if.then:                                          ; preds = %if.start
  ret i32 1, !dbg !10

elif.start:                                       ; preds = %if.start
  %a1 = load i32, ptr %a, align 4, !dbg !10
  %EQ = icmp eq i32 %a1, 2, !dbg !10
  br i1 %EQ, label %elif.then, label %if.else, !dbg !10

elif.then:                                        ; preds = %elif.start
  ret i32 2, !dbg !10

if.else:                                          ; preds = %elif.start
  ret i32 3, !dbg !10
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "func.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/if")
!4 = distinct !DISubprogram(name: "check", linkageName: "check", scope: null, file: !3, line: 2, type: !5, scopeLine: 2, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 2, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 5, scope: !8)
!10 = !DILocation(line: 7, scope: !8)
