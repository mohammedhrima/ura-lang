; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"is 1\0A\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"not 1 but 2\0A\00", align 1
@STR2 = private unnamed_addr constant [14 x i8] c"not 1, not 2\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %a = alloca i32, align 4, !dbg !7
  store i32 0, i32* %a, align 4, !dbg !7
  store i32 2, i32* %a, align 4, !dbg !7
  br label %if.start, !dbg !7

if.start:                                         ; preds = %entry
  %a1 = load i32, i32* %a, align 4, !dbg !7
  %EQ = icmp eq i32 %a1, 1, !dbg !7
  br i1 %EQ, label %if.then, label %elif.start, !dbg !7

if.end:                                           ; preds = %if.else, %elif.then, %if.then
  ret i32 0, !dbg !8

if.then:                                          ; preds = %if.start
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0), i32 0), !dbg !9
  br label %if.end, !dbg !9

elif.start:                                       ; preds = %if.start
  %a2 = load i32, i32* %a, align 4, !dbg !9
  %EQ3 = icmp eq i32 %a2, 2, !dbg !9
  br i1 %EQ3, label %elif.then, label %if.else, !dbg !9

elif.then:                                        ; preds = %elif.start
  %printf4 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @STR1, i32 0, i32 0), i32 0), !dbg !10
  br label %if.end, !dbg !10

if.else:                                          ; preds = %elif.start
  %printf5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @STR2, i32 0, i32 0), i32 0), !dbg !8
  br label %if.end, !dbg !8
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 11, scope: !4)
!9 = !DILocation(line: 7, scope: !4)
!10 = !DILocation(line: 9, scope: !4)
