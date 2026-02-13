; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@fmt = private unnamed_addr constant [62 x i8] c"\0A\1B[0;31mRuntime Error: \1B[0mNull pointer dereference at %s:%d\0A\00", align 1
@STR0 = private unnamed_addr constant [8 x i8] c"x = %d\0A\00", align 1

define ptr @__null_check(ptr %0, i32 %1, ptr %2) {
entry:
  %ptrint = ptrtoint ptr %0 to i64
  %isnull = icmp eq i64 %ptrint, 0
  br i1 %isnull, label %is_null, label %not_null

is_null:                                          ; preds = %entry
  %3 = call i32 (ptr, ...) @printf(ptr @fmt, ptr %2, i32 %1)
  call void @exit(i32 1)
  unreachable

not_null:                                         ; preds = %entry
  ret ptr %0
}

declare i32 @printf(ptr, ...)

declare void @exit(i32)

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
  %x = alloca i32, align 4
  store i32 111, ptr %x, align 4
  %x1 = load i32, ptr %x, align 4
  %printf = call i32 @printf(ptr @STR0, i32 %x1)
  %x2 = load i32, ptr %x, align 4
  ret i32 %x2
}
