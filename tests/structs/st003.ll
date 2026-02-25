; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

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
  %val = load i32, ptr %1, align 4
  store i32 %val, ptr %bound, align 4
  br label %ret

ret:                                              ; preds = %store, %bind
  ret void
}

declare i32 @printf(ptr, i32, ...)

define void @foo(ptr %n) {
entry:
  %n1 = alloca ptr, align 8
  store ptr %n, ptr %n1, align 8
  %n2 = load ptr, ptr %n1, align 8
  %id = getelementptr { i32 }, ptr %n2, i32 0, i32 0
  store i32 2, ptr %id, align 4
  ret void
}

define i32 @main() {
entry:
  %user = alloca { i32 }, align 8
  %id = getelementptr { i32 }, ptr %user, i32 0, i32 0
  store i32 1, ptr %id, align 4
  call void @foo(ptr %user)
  %id1 = getelementptr { i32 }, ptr %user, i32 0, i32 0
  %user2 = load i32, ptr %id1, align 4
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 1, i32 %user2)
  ret i32 0
}
