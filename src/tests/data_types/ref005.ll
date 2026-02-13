; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@fmt = private unnamed_addr constant [62 x i8] c"\0A\1B[0;31mRuntime Error: \1B[0mNull pointer dereference at %s:%d\0A\00", align 1
@file = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1

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
  call void @llvm.memcpy.p0.p0.i32(ptr %bound, ptr %1, i32 %2, i1 false)
  br label %ret

ret:                                              ; preds = %store, %bind
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i32(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i32, i1 immarg) #0

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
  %0 = call ptr @__null_check(ptr %ptr, i32 8, ptr @file)
  %current2 = load i32, ptr %0, align 4
  %add = add i32 %current2, 5
  %op_temp = alloca i32, align 4
  store i32 %add, ptr %op_temp, align 4
  call void @ref_assign(ptr %current, ptr %op_temp, i32 4)
  %sum = alloca i32, align 4
  %val13 = load i32, ptr %val1, align 4
  %val24 = load i32, ptr %val2, align 4
  %ADD = add i32 %val13, %val24
  %val35 = load i32, ptr %val3, align 4
  %ADD6 = add i32 %ADD, %val35
  store i32 %ADD6, ptr %sum, align 4
  %sum7 = load i32, ptr %sum, align 4
  ret i32 %sum7
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
