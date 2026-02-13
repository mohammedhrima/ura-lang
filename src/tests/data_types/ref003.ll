; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@fmt = private unnamed_addr constant [62 x i8] c"\0A\1B[0;31mRuntime Error: \1B[0mNull pointer dereference at %s:%d\0A\00", align 1
@file = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@file.1 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@file.2 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@file.3 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1

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
  %base = alloca i32, align 4
  store i32 100, ptr %base, align 4
  %ref1 = alloca ptr, align 8
  store ptr null, ptr %ref1, align 8
  store ptr %base, ptr %ref1, align 8
  %ref2 = alloca ptr, align 8
  store ptr null, ptr %ref2, align 8
  store ptr %base, ptr %ref2, align 8
  %ptr = load ptr, ptr %ref1, align 8
  %0 = call ptr @__null_check(ptr %ptr, i32 3, ptr @file)
  %current = load i32, ptr %0, align 4
  %add = add i32 %current, 20
  %op_temp = alloca i32, align 4
  store i32 %add, ptr %op_temp, align 4
  call void @ref_assign(ptr %ref1, ptr %op_temp, i32 4)
  %ptr1 = load ptr, ptr %ref2, align 8
  %1 = call ptr @__null_check(ptr %ptr1, i32 4, ptr @file.1)
  %current2 = load i32, ptr %1, align 4
  %sub = sub i32 %current2, 10
  %op_temp3 = alloca i32, align 4
  store i32 %sub, ptr %op_temp3, align 4
  call void @ref_assign(ptr %ref2, ptr %op_temp3, i32 4)
  %result = alloca i32, align 4
  %ptr4 = load ptr, ptr %ref1, align 8
  %2 = call ptr @__null_check(ptr %ptr4, i32 3, ptr @file.2)
  %ref15 = load i32, ptr %2, align 4
  %ptr6 = load ptr, ptr %ref2, align 8
  %3 = call ptr @__null_check(ptr %ptr6, i32 4, ptr @file.3)
  %ref27 = load i32, ptr %3, align 4
  %ADD = add i32 %ref15, %ref27
  store i32 %ADD, ptr %result, align 4
  %result8 = load i32, ptr %result, align 4
  ret i32 %result8
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
