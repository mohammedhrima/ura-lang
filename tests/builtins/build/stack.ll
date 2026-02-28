; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins/stack.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins/stack.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [12 x i8] c"Hello world\00", align 1
@STR1 = private unnamed_addr constant [12 x i8] c"Hello world\00", align 1

declare i32 @write(i32, ptr, i32)

declare void @strcpy(ptr, ptr)

declare i32 @strlen(ptr)

define i32 @main() !dbg !4 {
entry:
  %str = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %str, align 8, !dbg !7
  %stack = alloca [20 x i8], align 1, !dbg !7
  %stack1 = getelementptr [20 x i8], ptr %stack, i32 0, i32 0, !dbg !7
  store ptr %stack1, ptr %str, align 8, !dbg !7
  %str2 = load ptr, ptr %str, align 8, !dbg !8
  call void @strcpy(ptr %str2, ptr @STR0), !dbg !8
  %str3 = load ptr, ptr %str, align 8, !dbg !9
  %strlen = call i32 @strlen(ptr @STR1), !dbg !9
  %write = call i32 @write(i32 1, ptr %str3, i32 %strlen), !dbg !9
  ret i32 0, !dbg !9
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "stack.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/builtins")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 5, type: !5, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 5, scope: !4)
!8 = !DILocation(line: 7, scope: !4)
!9 = !DILocation(line: 8, scope: !4)
