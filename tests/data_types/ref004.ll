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
  store i32 1, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 2, ptr %b, align 4
  %choice = alloca i32, align 4
  store i32 1, ptr %choice, align 4
  %r = alloca ptr, align 8
  store ptr null, ptr %r, align 8
  br label %if.start

if.start:                                         ; preds = %entry
  %choice1 = load i32, ptr %choice, align 4
  %EQ = icmp eq i32 %choice1, 1
  br i1 %EQ, label %if.then, label %if.else

if.end:                                           ; preds = %if.else, %if.then
  %ref_temp = alloca i32, align 4
  store i32 100, ptr %ref_temp, align 4
  call void @ref_assign(ptr %r, ptr %ref_temp, i32 4)
  %a2 = load i32, ptr %a, align 4
  %b3 = load i32, ptr %b, align 4
  %ADD = add i32 %a2, %b3
  ret i32 %ADD

if.then:                                          ; preds = %if.start
  store ptr %a, ptr %r, align 8
  br label %if.end

if.else:                                          ; preds = %if.start
  store ptr %b, ptr %r, align 8
  br label %if.end
}
