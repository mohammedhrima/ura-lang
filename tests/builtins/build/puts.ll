; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins/puts.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins/puts.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [9 x i8] c"is digit\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"is not digit\00", align 1

declare i32 @puts(ptr)

define i1 @isdigit(i8 %c) !dbg !4 {
entry:
  %c1 = alloca i8, align 1, !dbg !7
  store i8 0, ptr %c1, align 1, !dbg !7
  store i8 %c, ptr %c1, align 1, !dbg !7
  %c2 = load i8, ptr %c1, align 1, !dbg !7
  %GE = icmp sge i8 %c2, 48, !dbg !7
  %c3 = load i8, ptr %c1, align 1, !dbg !7
  %LE = icmp sle i8 %c3, 57, !dbg !7
  %AND = and i1 %GE, %LE, !dbg !7
  ret i1 %AND, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  br label %if.start, !dbg !9

if.start:                                         ; preds = %entry
  %isdigit = call i1 @isdigit(i8 49), !dbg !10
  br i1 %isdigit, label %if.then, label %if.else, !dbg !10

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0, !dbg !11

if.then:                                          ; preds = %if.start
  %puts = call i32 @puts(ptr @STR0), !dbg !10
  br label %if.end, !dbg !10

if.else:                                          ; preds = %if.start
  %puts1 = call i32 @puts(ptr @STR1), !dbg !11
  br label %if.end, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "puts.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins")
!4 = distinct !DISubprogram(name: "isdigit", linkageName: "isdigit", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 6, scope: !8)
!10 = !DILocation(line: 7, scope: !8)
!11 = !DILocation(line: 8, scope: !8)
