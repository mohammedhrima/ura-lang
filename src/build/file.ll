; ModuleID = 'file.ura'
source_filename = "file.ura"
target triple = "arm64-apple-macosx16.0.0"

declare i32 @write(i32, ptr, i32)

; Function Attrs: sanitize_address
define i32 @putchar(i8 %c) #0 !dbg !4 {
entry:
  %c1 = alloca i8, align 1, !dbg !7
  store i8 0, ptr %c1, align 1, !dbg !7
  store i8 %c, ptr %c1, align 1, !dbg !7
  %str = alloca ptr, align 8, !dbg !8
  store ptr null, ptr %str, align 8, !dbg !8
  %stack = alloca [2 x i8], align 1, !dbg !8
  %stack2 = getelementptr [2 x i8], ptr %stack, i32 0, i32 0, !dbg !8
  store ptr %stack2, ptr %str, align 8, !dbg !8
  %str3 = load ptr, ptr %str, align 8, !dbg !9
  %ACCESS = getelementptr i8, ptr %str3, i32 10, !dbg !9
  %c4 = load i8, ptr %c1, align 1, !dbg !7
  store i8 %c4, ptr %ACCESS, align 1, !dbg !7
  %str5 = load ptr, ptr %str, align 8, !dbg !8
  %write = call i32 @write(i32 1, ptr %str5, i32 1), !dbg !10
  ret i32 0, !dbg !11
}

; Function Attrs: sanitize_address
define i32 @main() #0 !dbg !12 {
entry:
  %putchar = call i32 @putchar(i8 97), !dbg !13
  %putchar1 = call i32 @putchar(i8 10), !dbg !14
  ret i32 0, !dbg !15
}

attributes #0 = { sanitize_address }

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "file.ura", directory: ".")
!4 = distinct !DISubprogram(name: "putchar", linkageName: "putchar", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 4, scope: !4)
!9 = !DILocation(line: 5, scope: !4)
!10 = !DILocation(line: 6, scope: !4)
!11 = !DILocation(line: 7, scope: !4)
!12 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2)
!13 = !DILocation(line: 10, scope: !12)
!14 = !DILocation(line: 11, scope: !12)
!15 = !DILocation(line: 12, scope: !12)
