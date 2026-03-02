; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/libft/isalnum.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/libft/isalnum.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@STR1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i1 @isalpha(i8 %c) !dbg !4 {
entry:
  %c1 = alloca i8, align 1, !dbg !7
  store i8 0, ptr %c1, align 1, !dbg !7
  store i8 %c, ptr %c1, align 1, !dbg !7
  %c2 = load i8, ptr %c1, align 1, !dbg !7
  %GE = icmp sge i8 %c2, 97, !dbg !7
  %c3 = load i8, ptr %c1, align 1, !dbg !7
  %LE = icmp sle i8 %c3, 122, !dbg !7
  %AND = and i1 %GE, %LE, !dbg !7
  ret i1 %AND, !dbg !7
}

define i1 @isdigit(i8 %c) !dbg !8 {
entry:
  %c1 = alloca i8, align 1, !dbg !9
  store i8 0, ptr %c1, align 1, !dbg !9
  store i8 %c, ptr %c1, align 1, !dbg !9
  %c2 = load i8, ptr %c1, align 1, !dbg !9
  %GE = icmp sge i8 %c2, 49, !dbg !9
  %c3 = load i8, ptr %c1, align 1, !dbg !9
  %LE = icmp sle i8 %c3, 57, !dbg !9
  %AND = and i1 %GE, %LE, !dbg !9
  ret i1 %AND, !dbg !9
}

define i1 @islanum(i8 %c) !dbg !10 {
entry:
  %c1 = alloca i8, align 1, !dbg !11
  store i8 0, ptr %c1, align 1, !dbg !11
  store i8 %c, ptr %c1, align 1, !dbg !11
  %c2 = load i8, ptr %c1, align 1, !dbg !11
  %isalpha = call i1 @isalpha(i8 %c2), !dbg !11
  %c3 = load i8, ptr %c1, align 1, !dbg !11
  %isdigit = call i1 @isdigit(i8 %c3), !dbg !11
  %OR = or i1 %isalpha, %isdigit, !dbg !11
  ret i1 %OR, !dbg !11
}

define i32 @main() !dbg !12 {
entry:
  br label %if.start, !dbg !13

if.start:                                         ; preds = %entry
  %islanum = call i1 @islanum(i8 49), !dbg !14
  br i1 %islanum, label %if.then, label %if.else, !dbg !14

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0, !dbg !15

if.then:                                          ; preds = %if.start
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 0), !dbg !16
  br label %if.end, !dbg !16

if.else:                                          ; preds = %if.start
  %printf1 = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 0), !dbg !15
  br label %if.end, !dbg !15
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "isalnum.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/libft")
!4 = distinct !DISubprogram(name: "isalpha", linkageName: "isalpha", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = distinct !DISubprogram(name: "isdigit", linkageName: "isdigit", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 4, scope: !8)
!10 = distinct !DISubprogram(name: "islanum", linkageName: "islanum", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2)
!11 = !DILocation(line: 5, scope: !10)
!12 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2)
!13 = !DILocation(line: 7, scope: !12)
!14 = !DILocation(line: 8, scope: !12)
!15 = !DILocation(line: 11, scope: !12)
!16 = !DILocation(line: 9, scope: !12)
