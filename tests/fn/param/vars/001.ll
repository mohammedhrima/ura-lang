; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define i32 @square(i32 %n) !dbg !4 {
entry:
  %n1 = alloca i32, align 4, !dbg !7
  store i32 %n, i32* %n1, align 4, !dbg !7
  %n2 = load i32, i32* %n1, align 4, !dbg !7
  %n3 = load i32, i32* %n1, align 4, !dbg !7
  %MUL = mul i32 %n2, %n3, !dbg !7
  ret i32 %MUL, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %val = alloca i32, align 4, !dbg !9
  store i32 0, i32* %val, align 4, !dbg !9
  %square = call i32 @square(i32 5), !dbg !10
  store i32 %square, i32* %val, align 4, !dbg !10
  %val1 = load i32, i32* %val, align 4, !dbg !11
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0), i32 1, i32 %val1), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "square", linkageName: "square", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 7, scope: !8)
!10 = !DILocation(line: 8, scope: !8)
!11 = !DILocation(line: 9, scope: !8)
