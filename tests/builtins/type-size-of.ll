; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins/type-size-of.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins/type-size-of.ura"
target triple = "arm64-apple-macosx16.0.0"

@typeof = private unnamed_addr constant [6 x i8] c"CHARS\00", align 1
@STR0 = private unnamed_addr constant [18 x i8] c"type %s, size %d\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i32 @main() !dbg !4 {
entry:
  %type = alloca ptr, align 8, !dbg !7
  %size = alloca i32, align 4, !dbg !7
  store i32 0, ptr %size, align 4, !dbg !7
  store ptr null, ptr %type, align 8, !dbg !7
  store ptr @typeof, ptr %type, align 8, !dbg !7
  store i32 8, ptr %size, align 4, !dbg !7
  %type1 = load ptr, ptr %type, align 8, !dbg !8
  %size2 = load i32, ptr %size, align 4, !dbg !8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 2, ptr %type1, i32 %size2), !dbg !8
  ret i32 0, !dbg !8
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "type-size-of.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 6, scope: !4)
