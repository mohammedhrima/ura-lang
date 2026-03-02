; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st02.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st02.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c"car\00", align 1
@STR2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %user = alloca { ptr, i32, { { ptr, i32 }, { ptr } } }, align 8, !dbg !7
  store { ptr, i32, { { ptr, i32 }, { ptr } } } zeroinitializer, ptr %user, align 8, !dbg !7
  %name = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 0, !dbg !7
  store ptr @STR0, ptr %name, align 8, !dbg !7
  %age = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 1, !dbg !7
  store i32 123, ptr %age, align 4, !dbg !7
  %car = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 2, !dbg !7
  %model = getelementptr { { ptr, i32 }, { ptr } }, ptr %car, i32 0, i32 0, !dbg !7
  %name1 = getelementptr { ptr, i32 }, ptr %model, i32 0, i32 0, !dbg !7
  store ptr @STR1, ptr %name1, align 8, !dbg !7
  %car2 = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 2, !dbg !8
  %model3 = getelementptr { { ptr, i32 }, { ptr } }, ptr %car2, i32 0, i32 0, !dbg !8
  %name4 = getelementptr { ptr, i32 }, ptr %model3, i32 0, i32 0, !dbg !8
  %DOT = load ptr, ptr %name4, align 8, !dbg !8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR2, i32 1, ptr %DOT), !dbg !8
  ret i32 0, !dbg !8
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "st02.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 20, type: !5, scopeLine: 20, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 20, scope: !4)
!8 = !DILocation(line: 25, scope: !4)
