; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define i32 @add(i32* %a, i32* %b) !dbg !4 {
entry:
  %a1 = alloca i32*, align 8, !dbg !7
  %b2 = alloca i32*, align 8, !dbg !7
  %result = alloca i32, align 4, !dbg !7
  store i32* %a, i32** %a1, align 8, !dbg !7
  store i32* %b, i32** %b2, align 8, !dbg !7
  %ref_ptr = load i32*, i32** %a1, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !7
  %ref_ptr3 = load i32*, i32** %b2, align 8, !dbg !7
  %ref_val4 = load i32, i32* %ref_ptr3, align 4, !dbg !7
  %ADD = add i32 %ref_val, %ref_val4, !dbg !7
  store i32 %ADD, i32* %result, align 4, !dbg !7
  %result5 = load i32, i32* %result, align 4, !dbg !7
  ret i32 %result5, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %x = alloca i32, align 4, !dbg !9
  %y = alloca i32, align 4, !dbg !9
  %z = alloca i32, align 4, !dbg !9
  store i32 3, i32* %x, align 4, !dbg !9
  store i32 4, i32* %y, align 4, !dbg !9
  %add = call i32 @add(i32* %x, i32* %y), !dbg !10
  store i32 %add, i32* %z, align 4, !dbg !10
  %z1 = load i32, i32* %z, align 4, !dbg !11
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @STR0, i32 0, i32 0), i32 1, i32 %z1), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "add", linkageName: "add", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 8, type: !5, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 8, scope: !8)
!10 = !DILocation(line: 12, scope: !8)
!11 = !DILocation(line: 13, scope: !8)
