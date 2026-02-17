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
  %base = alloca i32, align 4
  store i32 100, ptr %base, align 4
  %ref1 = alloca ptr, align 8
  store ptr null, ptr %ref1, align 8
  store ptr %base, ptr %ref1, align 8
  %ref2 = alloca ptr, align 8
  store ptr null, ptr %ref2, align 8
  store ptr %base, ptr %ref2, align 8
  %ptr = load ptr, ptr %ref1, align 8
  %current = load i32, ptr %ptr, align 4
  %ADD = add i32 %current, 20
  %op_temp = alloca i32, align 4
  store i32 %ADD, ptr %op_temp, align 4
  call void @ref_assign(ptr %ref1, ptr %op_temp, i32 4)
  %ptr1 = load ptr, ptr %ref2, align 8
  %current2 = load i32, ptr %ptr1, align 4
  %SUB = sub i32 %current2, 10
  %op_temp3 = alloca i32, align 4
  store i32 %SUB, ptr %op_temp3, align 4
  call void @ref_assign(ptr %ref2, ptr %op_temp3, i32 4)
  %result = alloca i32, align 4
  %ptr4 = load ptr, ptr %ref1, align 8
  %ref15 = load i32, ptr %ptr4, align 4
  %ptr6 = load ptr, ptr %ref2, align 8
  %ref27 = load i32, ptr %ptr6, align 4
  %ADD8 = add i32 %ref15, %ref27
  store i32 %ADD8, ptr %result, align 4
  %result9 = load i32, ptr %result, align 4
  ret i32 %result9
}
