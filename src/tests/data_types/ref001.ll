; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

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
  %val = load i32, ptr %1, align 4
  store i32 %val, ptr %bound, align 4
  br label %ret

ret:                                              ; preds = %store, %bind
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, ptr %a, align 4
  %b = alloca ptr, align 8
  store ptr null, ptr %b, align 8
  store ptr %a, ptr %b, align 8
  %ref_temp = alloca i32, align 4
  store i32 20, ptr %ref_temp, align 4
  call void @ref_assign(ptr %b, ptr %ref_temp, i32 4)
  %a1 = load i32, ptr %a, align 4
  ret i32 %a1
}
