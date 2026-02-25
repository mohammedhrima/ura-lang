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
  %val1 = alloca i32, align 4
  store i32 10, ptr %val1, align 4
  %val2 = alloca i32, align 4
  store i32 20, ptr %val2, align 4
  %val3 = alloca i32, align 4
  store i32 30, ptr %val3, align 4
  %r1 = alloca ptr, align 8
  store ptr null, ptr %r1, align 8
  store ptr %val1, ptr %r1, align 8
  %ref_temp = alloca i32, align 4
  store i32 15, ptr %ref_temp, align 4
  call void @ref_assign(ptr %r1, ptr %ref_temp, i32 4)
  %r2 = alloca ptr, align 8
  store ptr null, ptr %r2, align 8
  store ptr %val2, ptr %r2, align 8
  %ptr = load ptr, ptr %r2, align 8
  %current = load i32, ptr %ptr, align 4
  %MUL = mul i32 %current, 2
  %op_temp = alloca i32, align 4
  store i32 %MUL, ptr %op_temp, align 4
  call void @ref_assign(ptr %r2, ptr %op_temp, i32 4)
  %r3 = alloca ptr, align 8
  store ptr null, ptr %r3, align 8
  store ptr %val3, ptr %r3, align 8
  %ptr1 = load ptr, ptr %r3, align 8
  %current2 = load i32, ptr %ptr1, align 4
  %SUB = sub i32 %current2, 5
  %op_temp3 = alloca i32, align 4
  store i32 %SUB, ptr %op_temp3, align 4
  call void @ref_assign(ptr %r3, ptr %op_temp3, i32 4)
  %result = alloca i32, align 4
  %ptr4 = load ptr, ptr %r1, align 8
  %r15 = load i32, ptr %ptr4, align 4
  %ptr6 = load ptr, ptr %r2, align 8
  %r27 = load i32, ptr %ptr6, align 4
  %ADD = add i32 %r15, %r27
  %ptr8 = load ptr, ptr %r3, align 8
  %r39 = load i32, ptr %ptr8, align 4
  %ADD10 = add i32 %ADD, %r39
  store i32 %ADD10, ptr %result, align 4
  %result11 = load i32, ptr %result, align 4
  ret i32 %result11
}
