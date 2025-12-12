; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@STR0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@err_fmt = private unnamed_addr constant [51 x i8] c"Error: index %d out of bounds (size: %d) at %s:%d\0A\00", align 1
@filename = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1
@filename.1 = private unnamed_addr constant [60 x i8] c"/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura\00", align 1

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  %stack = alloca [2 x i8], align 1
  %stack1 = getelementptr [2 x i8], ptr %stack, i32 0, i32 0
  store ptr %stack1, ptr %str, align 8
  %str2 = load ptr, ptr %str, align 8
  call void @__bounds_check(i32 10, i32 2, i32 2, ptr @filename)
  %access = getelementptr i8, ptr %str2, i32 10
  store i8 51, ptr %access, align 1
  %str3 = load ptr, ptr %str, align 8
  call void @__bounds_check(i32 100, i32 2, i32 3, ptr @filename.1)
  %access4 = getelementptr i8, ptr %str3, i32 100
  store i8 51, ptr %access4, align 1
  ret i32 0
}

declare i32 @printf(ptr, ...)

declare void @exit(i32)

define void @__bounds_check(i32 %0, i32 %1, i32 %2, ptr %3) {
entry:
  %ge0 = icmp sge i32 %0, 0
  %ltsize = icmp slt i32 %0, %1
  %valid = and i1 %ge0, %ltsize
  br i1 %valid, label %ok, label %error

error:                                            ; preds = %entry
  %4 = call i32 (ptr, ...) @printf(ptr @err_fmt, i32 %0, i32 %1, ptr %3, i32 %2)
  call void @exit(i32 1)
  unreachable

ok:                                               ; preds = %entry
  ret void
}
