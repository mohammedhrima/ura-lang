; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@fmt = private unnamed_addr constant [62 x i8] c"\0A\1B[0;31mRuntime Error: \1B[0mNull pointer dereference at %s:%d\0A\00", align 1

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

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
