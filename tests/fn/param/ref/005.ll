; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define void @print_val(i32 %n) !dbg !4 {
entry:
  %n1 = alloca i32, align 4, !dbg !7
  store i32 %n, i32* %n1, align 4, !dbg !7
  %n2 = load i32, i32* %n1, align 4, !dbg !8
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0), i32 1, i32 %n2), !dbg !8
  ret void, !dbg !8
}

define i32 @main() !dbg !9 {
entry:
  %a = alloca i32, align 4, !dbg !10
  %r = alloca i32*, align 8, !dbg !10
  store i32 42, i32* %a, align 4, !dbg !10
  store i32* %a, i32** %r, align 8, !dbg !10
  %ref_ptr = load i32*, i32** %r, align 8, !dbg !11
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !11
  call void @print_val(i32 %ref_val), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "print_val", linkageName: "print_val", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = !DILocation(line: 5, scope: !4)
!9 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!10 = !DILocation(line: 7, scope: !9)
!11 = !DILocation(line: 10, scope: !9)
