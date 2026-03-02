; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st03.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs/st03.ura"
target triple = "arm64-apple-macosx16.0.0"

@STR0 = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define void @ref_assign(ptr %0, ptr %1, i32 %2) {
entry:
  %current = load ptr, ptr %0, align 8
  %is_null = icmp eq ptr %current, null
  br i1 %is_null, label %bind, label %store

bind:                                             ; preds = %entry
  store ptr %1, ptr %0, align 8
  br label %ret

store:                                            ; preds = %entry
  %bound = load ptr, ptr %0, align 8
  call void @llvm.memcpy.p0.p0.i32(ptr %bound, ptr %1, i32 %2, i1 false)
  br label %ret

ret:                                              ; preds = %store, %bind
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i32(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i32, i1 immarg) #0

declare i32 @printf(ptr, i32, ...)

define void @foo(ptr %n) !dbg !4 {
entry:
  %n1 = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %n1, align 8, !dbg !7
  store ptr %n, ptr %n1, align 8, !dbg !7
  %n2 = load ptr, ptr %n1, align 8, !dbg !7
  %id = getelementptr { i32 }, ptr %n2, i32 0, i32 0, !dbg !7
  store i32 2, ptr %id, align 4, !dbg !7
  ret void, !dbg !7
}

define i32 @main() !dbg !8 {
entry:
  %user = alloca { i32 }, align 8, !dbg !9
  store { i32 } zeroinitializer, ptr %user, align 4, !dbg !9
  %id = getelementptr { i32 }, ptr %user, i32 0, i32 0, !dbg !9
  store i32 1, ptr %id, align 4, !dbg !9
  call void @foo(ptr %user), !dbg !10
  %id1 = getelementptr { i32 }, ptr %user, i32 0, i32 0, !dbg !11
  %DOT = load i32, ptr %id1, align 4, !dbg !11
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 1, i32 %DOT), !dbg !11
  ret i32 0, !dbg !11
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "st03.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/structs")
!4 = distinct !DISubprogram(name: "foo", linkageName: "foo", scope: null, file: !3, line: 6, type: !5, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 6, scope: !4)
!8 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 9, type: !5, scopeLine: 9, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 9, scope: !8)
!10 = !DILocation(line: 12, scope: !8)
!11 = !DILocation(line: 13, scope: !8)
