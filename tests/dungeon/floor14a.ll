; ModuleID = 'tests/dungeon/floor14a.ura'
source_filename = "tests/dungeon/floor14a.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [8 x i8] c"dungeon\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c"hi\0A\00", align 1
@STR2 = private unnamed_addr constant [12 x i8] c"length: %d\0A\00", align 1

declare i32 @strlen(i8*)

declare i32 @write(i32, i8*, i32)

declare i32 @printf(i8*, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %n = alloca i32, align 4, !dbg !7
  store i32 0, i32* %n, align 4, !dbg !7
  %strlen = call i32 @strlen(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @STR0, i32 0, i32 0)), !dbg !8
  store i32 %strlen, i32* %n, align 4, !dbg !8
  %write = call i32 @write(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @STR1, i32 0, i32 0), i32 3), !dbg !9
  %n1 = load i32, i32* %n, align 4, !dbg !10
  %printf = call i32 (i8*, i32, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @STR2, i32 0, i32 0), i32 1, i32 %n1), !dbg !10
  ret i32 0, !dbg !10
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "floor14a.ura", directory: "tests/dungeon")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !6)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
!9 = !DILocation(line: 8, scope: !4)
!10 = !DILocation(line: 9, scope: !4)
