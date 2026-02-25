; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

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

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 5, ptr %x, align 4
  %r1 = alloca ptr, align 8
  store ptr null, ptr %r1, align 8
  store ptr %x, ptr %r1, align 8
  %r2 = alloca ptr, align 8
  store ptr null, ptr %r2, align 8
  store ptr %x, ptr %r2, align 8
  %ref_temp = alloca i32, align 4
  store i32 10, ptr %ref_temp, align 4
  call void @ref_assign(ptr %r1, ptr %ref_temp, i32 4)
  %ref_temp1 = alloca i32, align 4
  store i32 15, ptr %ref_temp1, align 4
  call void @ref_assign(ptr %r2, ptr %ref_temp1, i32 4)
  %x2 = load i32, ptr %x, align 4
  ret i32 %x2
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
