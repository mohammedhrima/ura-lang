; ModuleID = 'module'
source_filename = "module"

@err_header = private unnamed_addr constant [45 x i8] c"\0Aline %d: segmentation fault (array bounds)\0A\00", align 1
@err_msg = private unnamed_addr constant [27 x i8] c"error in accessing %s[%d]\0A\00", align 1
@err_pointer = private unnamed_addr constant [25 x i8] c"                   ^^^^\0A\00", align 1

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #0

define void @bounds_check(i32 %0, i32 %1, i32 %2, i32 %3, ptr %4) {
entry:
  %is_negative = icmp slt i32 %0, 0
  %is_out_of_bounds = icmp sge i32 %0, %1
  %is_invalid = or i1 %is_negative, %is_out_of_bounds
  br i1 %is_invalid, label %error, label %valid

error:                                            ; preds = %entry
  %5 = call i32 (ptr, ...) @printf(ptr @err_header, i32 %2)
  %6 = call i32 (ptr, ...) @printf(ptr @err_msg, ptr %4, i32 %0)
  %7 = call i32 (ptr, ...) @printf(ptr @err_pointer)
  call void @exit(i32 1)
  unreachable

valid:                                            ; preds = %entry
  ret void
}

declare i32 @printf(ptr, ...)

declare void @exit(i32)

define i32 @sum(i32 %0, ...) {
entry:
  %count = alloca i32, align 4
  store i32 %0, ptr %count, align 4
  %args = alloca ptr, align 8
  call void @llvm.va_start.p0(ptr %args)
  %total = alloca i32, align 4
  store i32 0, ptr %total, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.cond.0

while.cond.0:                                     ; preds = %while.body.1, %entry
  %i1 = load i32, ptr %i, align 4
  %count2 = load i32, ptr %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body.1, label %while.end.2

while.body.1:                                     ; preds = %while.cond.0
  %total3 = load i32, ptr %total, align 4
  %va_arg = va_arg ptr %args, i32
  %add = add i32 %total3, %va_arg
  store i32 %add, ptr %total, align 4
  %i4 = load i32, ptr %i, align 4
  %add5 = add i32 %i4, 1
  store i32 %add5, ptr %i, align 4
  br label %while.cond.0

while.end.2:                                      ; preds = %while.cond.0
  call void @llvm.va_end.p0(ptr %args)
  %total6 = load i32, ptr %total, align 4
  ret i32 %total6
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %sum = call i32 (i32, ...) @sum(i32 3, i32 10, i32 20, i32 30)
  store i32 %sum, ptr %result, align 4
  %result1 = load i32, ptr %result, align 4
  ret i32 %result1
}

attributes #0 = { nocallback nofree nosync nounwind willreturn }
