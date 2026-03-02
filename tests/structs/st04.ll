; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st04.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st04.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [15 x i8] c"hello from %s\0A\00", align 1
@STR1 = private unnamed_addr constant [9 x i8] c"mohammed\00", align 1

declare i32 @printf(ptr, i32, ...)

define void @User.greed(ptr %self) !dbg !4 {
entry:
  %self1 = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %self1, align 8, !dbg !7
  store ptr %self, ptr %self1, align 8, !dbg !7
  %self2 = load ptr, ptr %self1, align 8, !dbg !8
  %name = getelementptr { ptr, i32 }, ptr %self2, i32 0, i32 0, !dbg !8
  %DOT = load ptr, ptr %name, align 8, !dbg !8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 1, ptr %DOT), !dbg !8
  ret void, !dbg !8
}

define i32 @main() !dbg !9 {
entry:
  %user = alloca { ptr, i32 }, align 8, !dbg !10
  store { ptr, i32 } zeroinitializer, ptr %user, align 8, !dbg !10
  %name = getelementptr { ptr, i32 }, ptr %user, i32 0, i32 0, !dbg !10
  store ptr @STR1, ptr %name, align 8, !dbg !10
  call void @User.greed(ptr %user), !dbg !11
  ret i32 0, !dbg !11
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "st04.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs")
!4 = distinct !DISubprogram(name: "User.greed", linkageName: "User.greed", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 7, scope: !4)
!8 = !DILocation(line: 8, scope: !4)
!9 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 10, type: !5, scopeLine: 10, spFlags: DISPFlagDefinition, unit: !2)
!10 = !DILocation(line: 10, scope: !9)
!11 = !DILocation(line: 13, scope: !9)
