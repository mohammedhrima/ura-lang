; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [13 x i8] c"Hello world\0A\00", align 1
@STR1 = private unnamed_addr constant [13 x i8] c"Hello world\0A\00", align 1

declare i32 @write(i32, ptr, i32)

declare void @strcpy(ptr, ptr)

declare i32 @strlen(ptr)

declare void @free(ptr)

define i32 @main() !dbg !4 {
entry:
  %str = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %str, align 8, !dbg !7
  %heap = call ptr @calloc(i64 20, i64 1), !dbg !7
  store ptr %heap, ptr %str, align 8, !dbg !7
  %str1 = load ptr, ptr %str, align 8, !dbg !8
  call void @strcpy(ptr %str1, ptr @STR0), !dbg !8
  %str2 = load ptr, ptr %str, align 8, !dbg !9
  %strlen = call i32 @strlen(ptr @STR1), !dbg !9
  %write = call i32 @write(i32 1, ptr %str2, i32 %strlen), !dbg !9
  %str3 = load ptr, ptr %str, align 8, !dbg !10
  call void @free(ptr %str3), !dbg !10
  ret i32 0, !dbg !10
}

declare ptr @calloc(i64, i64)

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/src")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
!9 = !DILocation(line: 9, scope: !4)
!10 = !DILocation(line: 10, scope: !4)
