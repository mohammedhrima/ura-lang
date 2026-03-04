; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [11 x i8] c"<%d> <%d>\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define void @swap(i32* %a, i32* %b) !dbg !4 {
entry:
  %a1 = alloca i32*, align 8, !dbg !7
  %b2 = alloca i32*, align 8, !dbg !7
  %tmp = alloca i32, align 4, !dbg !7
  store i32* %a, i32** %a1, align 8, !dbg !7
  store i32* %b, i32** %b2, align 8, !dbg !7
  %ref_ptr = load i32*, i32** %a1, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr, align 4, !dbg !7
  store i32 %ref_val, i32* %tmp, align 4, !dbg !7
  %ref_ptr3 = load i32*, i32** %b2, align 8, !dbg !7
  %ref_val4 = load i32, i32* %ref_ptr3, align 4, !dbg !7
  %ref_ptr5 = load i32*, i32** %a1, align 8, !dbg !7
  store i32 %ref_val4, i32* %ref_ptr5, align 4, !dbg !7
  %tmp6 = load i32, i32* %tmp, align 4, !dbg !7
  %ref_ptr7 = load i32*, i32** %b2, align 8, !dbg !7
  store i32 %tmp6, i32* %ref_ptr7, align 4, !dbg !7
  ret void, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %x = alloca i32, align 4, !dbg !9
  %y = alloca i32, align 4, !dbg !9
  store i32 10, i32* %x, align 4, !dbg !9
  store i32 20, i32* %y, align 4, !dbg !9
  call void @swap(i32* %x, i32* %y), !dbg !10
  %x1 = load i32, i32* %x, align 4, !dbg !11
  %y2 = load i32, i32* %y, align 4, !dbg !11
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @STR0, i32 0, i32 0), i32 2, i32 %x1, i32 %y2), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "swap", linkageName: "swap", scope: null, file: !3, line: 4, type: !5, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 4, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!9 = !DILocation(line: 9, scope: !8)
!10 = !DILocation(line: 12, scope: !8)
!11 = !DILocation(line: 13, scope: !8)
