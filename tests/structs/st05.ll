; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st05.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st05.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [9 x i8] c"new name\00", align 1
@STR1 = private unnamed_addr constant [22 x i8] c"hello from %s has %d\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define void @User.init(ptr %self) !dbg !4 {
entry:
  %self1 = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %self1, align 8, !dbg !7
  store ptr %self, ptr %self1, align 8, !dbg !7
  %self2 = load ptr, ptr %self1, align 8, !dbg !7
  %name = getelementptr { ptr, i32 }, ptr %self2, i32 0, i32 0, !dbg !7
  store ptr @STR0, ptr %name, align 8, !dbg !7
  %self3 = load ptr, ptr %self1, align 8, !dbg !7
  %age = getelementptr { ptr, i32 }, ptr %self3, i32 0, i32 1, !dbg !7
  store i32 10, ptr %age, align 4, !dbg !7
  ret void, !dbg !7
}

define void @User.greed(ptr %self) !dbg !8 {
entry:
  %self1 = alloca ptr, align 8, !dbg !9
  store ptr null, ptr %self1, align 8, !dbg !9
  store ptr %self, ptr %self1, align 8, !dbg !9
  %self2 = load ptr, ptr %self1, align 8, !dbg !10
  %name = getelementptr { ptr, i32 }, ptr %self2, i32 0, i32 0, !dbg !10
  %DOT = load ptr, ptr %name, align 8, !dbg !10
  %self3 = load ptr, ptr %self1, align 8, !dbg !10
  %age = getelementptr { ptr, i32 }, ptr %self3, i32 0, i32 1, !dbg !10
  %DOT4 = load i32, ptr %age, align 4, !dbg !10
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 2, ptr %DOT, i32 %DOT4), !dbg !10
  ret void, !dbg !10
}

define i32 @main() !dbg !11 {
entry:
  %user = alloca { ptr, i32 }, align 8, !dbg !12
  store { ptr, i32 } zeroinitializer, ptr %user, align 8, !dbg !12
  call void @User.init(ptr %user), !dbg !12
  call void @User.greed(ptr %user), !dbg !13
  ret i32 0, !dbg !13
}

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "st05.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs")
!4 = distinct !DISubprogram(name: "User.init", linkageName: "User.init", scope: null, file: !3, line: 7, type: !5, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 7, scope: !4)
!8 = distinct !DISubprogram(name: "User.greed", linkageName: "User.greed", scope: null, file: !3, line: 11, type: !5, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 11, scope: !8)
!10 = !DILocation(line: 12, scope: !8)
!11 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 14, type: !5, scopeLine: 14, spFlags: DISPFlagDefinition, unit: !2)
!12 = !DILocation(line: 14, scope: !11)
!13 = !DILocation(line: 16, scope: !11)
