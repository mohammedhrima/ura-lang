; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@fmt = private unnamed_addr constant [62 x i8] c"\0A\1B[0;31mRuntime Error: \1B[0mNull pointer dereference at %s:%d\0A\00", align 1
@file = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@file.1 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@file.2 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@file.3 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@file.4 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1

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
  %0 = call ptr @__null_check(ptr %ptr, i32 11, ptr @file)
  %current = load i32, ptr %0, align 4
  %mul = mul i32 %current, 2
  %op_temp = alloca i32, align 4
  store i32 %mul, ptr %op_temp, align 4
  call void @ref_assign(ptr %r2, ptr %op_temp, i32 4)
  %r3 = alloca ptr, align 8
  store ptr null, ptr %r3, align 8
  store ptr %val3, ptr %r3, align 8
  %ptr1 = load ptr, ptr %r3, align 8
  %1 = call ptr @__null_check(ptr %ptr1, i32 15, ptr @file.1)
  %current2 = load i32, ptr %1, align 4
  %sub = sub i32 %current2, 5
  %op_temp3 = alloca i32, align 4
  store i32 %sub, ptr %op_temp3, align 4
  call void @ref_assign(ptr %r3, ptr %op_temp3, i32 4)
  %result = alloca i32, align 4
  %ptr4 = load ptr, ptr %r1, align 8
  %2 = call ptr @__null_check(ptr %ptr4, i32 7, ptr @file.2)
  %r15 = load i32, ptr %2, align 4
  %ptr6 = load ptr, ptr %r2, align 8
  %3 = call ptr @__null_check(ptr %ptr6, i32 11, ptr @file.3)
  %r27 = load i32, ptr %3, align 4
  %ADD = add i32 %r15, %r27
  %ptr8 = load ptr, ptr %r3, align 8
  %4 = call ptr @__null_check(ptr %ptr8, i32 15, ptr @file.4)
  %r39 = load i32, ptr %4, align 4
  %ADD10 = add i32 %ADD, %r39
  store i32 %ADD10, ptr %result, align 4
  %result11 = load i32, ptr %result, align 4
  ret i32 %result11
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
