; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [8 x i8] c"x:  %d\0A\00", align 1
@STR1 = private unnamed_addr constant [8 x i8] c"y:  %d\0A\00", align 1
@STR2 = private unnamed_addr constant [8 x i8] c"r1: %p\0A\00", align 1
@STR3 = private unnamed_addr constant [8 x i8] c"r2: %p\0A\00", align 1

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %x = alloca i32, align 4, !dbg !7
  %r1 = alloca i32*, align 8, !dbg !7
  %r2 = alloca i32*, align 8, !dbg !7
  %y = alloca i32, align 4, !dbg !7
  store i32 5, i32* %x, align 4, !dbg !7
  store i32* %x, i32** %r1, align 8, !dbg !7
  store i32* %x, i32** %r2, align 8, !dbg !7
  %ref_ptr = load i32*, i32** %r1, align 8, !dbg !7
  store i32 10, i32* %ref_ptr, align 4, !dbg !7
  %ref_ptr1 = load i32*, i32** %r2, align 8, !dbg !7
  store i32 15, i32* %ref_ptr1, align 4, !dbg !7
  %ref_ptr2 = load i32*, i32** %r1, align 8, !dbg !7
  %ref_val = load i32, i32* %ref_ptr2, align 4, !dbg !7
  store i32 %ref_val, i32* %y, align 4, !dbg !7
  %x3 = load i32, i32* %x, align 4, !dbg !8
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR0, i32 0, i32 0), i32 1, i32 %x3), !dbg !8
  %y4 = load i32, i32* %y, align 4, !dbg !9
  %printf5 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR1, i32 0, i32 0), i32 1, i32 %y4), !dbg !9
  %printf6 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR2, i32 0, i32 0), i32 1, i32** %r1), !dbg !10
  %printf7 = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR3, i32 0, i32 0), i32 1, i32** %r2), !dbg !11
  ret i32 0, !dbg !11
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
!9 = !DILocation(line: 12, scope: !4)
!10 = !DILocation(line: 13, scope: !4)
!11 = !DILocation(line: 14, scope: !4)
