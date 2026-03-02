; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st01.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st01.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [9 x i8] c"mohammed\00", align 1
@STR1 = private unnamed_addr constant [17 x i8] c"hello %s has %d\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

declare ptr @calloc(i32, i32)

declare void @free(ptr)

declare ptr @strcpy(ptr, ptr)

define i32 @main() !dbg !4 {
entry:
  %user = alloca { i32, ptr }, align 8, !dbg !7
  %name = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %name, align 8, !dbg !7
  store { i32, ptr } zeroinitializer, ptr %user, align 8, !dbg !7
  %age = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0, !dbg !7
  store i32 28, ptr %age, align 4, !dbg !7
  %calloc = call ptr @calloc(i32 1, i32 10), !dbg !8
  store ptr %calloc, ptr %name, align 8, !dbg !8
  %name1 = load ptr, ptr %name, align 8, !dbg !9
  %strcpy = call ptr @strcpy(ptr %name1, ptr @STR0), !dbg !9
  %name2 = load ptr, ptr %name, align 8, !dbg !10
  %age3 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0, !dbg !10
  %DOT = load i32, ptr %age3, align 4, !dbg !10
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 2, ptr %name2, i32 %DOT), !dbg !10
  %name4 = load ptr, ptr %name, align 8, !dbg !11
  call void @free(ptr %name4), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "st01.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 9, scope: !4)
!8 = !DILocation(line: 12, scope: !4)
!9 = !DILocation(line: 13, scope: !4)
!10 = !DILocation(line: 14, scope: !4)
!11 = !DILocation(line: 15, scope: !4)
