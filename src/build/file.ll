; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@fmt = private unnamed_addr constant [62 x i8] c"\0A\1B[0;31mRuntime Error: \1B[0mNull pointer dereference at %s:%d\0A\00", align 1
@file = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@file.1 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1

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

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 13, ptr %a, align 4
  %b = alloca ptr, align 8
  store ptr null, ptr %b, align 8
  br label %if.start

if.start:                                         ; preds = %entry
  %ptr = load ptr, ptr %b, align 8
  %0 = call ptr @__null_check(ptr %ptr, i32 3, ptr @file)
  %b1 = load i32, ptr %0, align 4
  %EQ = icmp eq i32 %b1, 13
  br i1 %EQ, label %if.then, label %if.end

if.end:                                           ; preds = %if.then, %if.start
  %ptr2 = load ptr, ptr %b, align 8
  %1 = call ptr @__null_check(ptr %ptr2, i32 3, ptr @file.1)
  %b3 = load i32, ptr %1, align 4
  ret i32 %b3

if.then:                                          ; preds = %if.start
  store ptr %a, ptr %b, align 8
  br label %if.end
}
