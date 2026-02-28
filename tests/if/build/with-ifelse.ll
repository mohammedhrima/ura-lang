; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/if/with-ifelse.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/if/with-ifelse.ura"
target triple = "arm64-apple-macosx16.0.0"

define i32 @main() !dbg !4 {
entry:
  %a = alloca i32, align 4, !dbg !7
  store i32 0, ptr %a, align 4, !dbg !7
  store i32 1, ptr %a, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %a1 = load i32, ptr %a, align 4, !dbg !7
  %EQ = icmp eq i32 %a1, 1, !dbg !7
  br i1 %EQ, label %if.then, label %elif.start, !dbg !7

if.end:                                           ; No predecessors!
  ret i32 0, !dbg !7

if.then:                                          ; preds = %if.start
  ret i32 1, !dbg !7

elif.start:                                       ; preds = %if.start
  %a2 = load i32, ptr %a, align 4, !dbg !7
  %EQ3 = icmp eq i32 %a2, 2, !dbg !7
  br i1 %EQ3, label %elif.then, label %if.else, !dbg !7

elif.then:                                        ; preds = %elif.start
  ret i32 2, !dbg !7

if.else:                                          ; preds = %elif.start
  ret i32 3, !dbg !7
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "with-ifelse.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/if")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
