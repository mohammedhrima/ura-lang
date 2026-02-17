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
  %current = alloca ptr, align 8
  store ptr null, ptr %current, align 8
  store ptr %val1, ptr %current, align 8
  %ref_temp = alloca i32, align 4
  store i32 15, ptr %ref_temp, align 4
  call void @ref_assign(ptr %current, ptr %ref_temp, i32 4)
  store ptr %val2, ptr %current, align 8
  %ref_temp1 = alloca i32, align 4
  store i32 25, ptr %ref_temp1, align 4
  call void @ref_assign(ptr %current, ptr %ref_temp1, i32 4)
  store ptr %val3, ptr %current, align 8
  %ptr = load ptr, ptr %current, align 8
  %current2 = load i32, ptr %ptr, align 4
  %ADD = add i32 %current2, 5
  %op_temp = alloca i32, align 4
  store i32 %ADD, ptr %op_temp, align 4
  call void @ref_assign(ptr %current, ptr %op_temp, i32 4)
  %sum = alloca i32, align 4
  %val13 = load i32, ptr %val1, align 4
  %val24 = load i32, ptr %val2, align 4
  %ADD5 = add i32 %val13, %val24
  %val36 = load i32, ptr %val3, align 4
  %ADD7 = add i32 %ADD5, %val36
  store i32 %ADD7, ptr %sum, align 4
  %sum8 = load i32, ptr %sum, align 4
  ret i32 %sum8
}
