; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/data_types/ref02.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/data_types/ref02.ura"
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

define i32 @main() !dbg !4 {
entry:
  %x = alloca i32, align 4, !dbg !7
  %r1 = alloca ptr, align 8, !dbg !7
  %r2 = alloca ptr, align 8, !dbg !7
  store ptr null, ptr %r2, align 8, !dbg !7
  store ptr null, ptr %r1, align 8, !dbg !7
  store i32 0, ptr %x, align 4, !dbg !7
  store i32 5, ptr %x, align 4, !dbg !7
  store ptr %x, ptr %r1, align 8, !dbg !7
  store ptr %x, ptr %r2, align 8, !dbg !7
  %ref_ptr = load ptr, ptr %r1, align 8, !dbg !7
  store i32 10, ptr %ref_ptr, align 4, !dbg !7
  %ref_ptr1 = load ptr, ptr %r2, align 8, !dbg !7
  store i32 15, ptr %ref_ptr1, align 4, !dbg !7
  %x2 = load i32, ptr %x, align 4, !dbg !8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 1, i32 %x2), !dbg !8
  ret i32 0, !dbg !8
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "ura", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!3 = !DIFile(filename: "ref02.ura", directory: "/Users/hrimamohammed/Desktop/Personal/ura-lang/tests/data_types")
!4 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !3, line: 3, type: !5, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{}
!7 = !DILocation(line: 3, scope: !4)
!8 = !DILocation(line: 9, scope: !4)
