; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

define i32 @sum(i32 %0, ...) {
entry:
  %va_count = alloca i32, align 4
  store i32 %0, ptr %va_count, align 4
  %args = alloca [32 x i8], align 1
  %1 = getelementptr [32 x i8], ptr %args, i32 0
  call void @llvm.va_start.p0(ptr %1)
  %total = alloca i32, align 4
  store i32 0, ptr %total, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.cond.0

while.cond.0:                                     ; preds = %while.body.1, %entry
  %i1 = load i32, ptr %i, align 4
  %va_len = load i32, ptr %va_count, align 4
  %less = icmp slt i32 %i1, %va_len
  br i1 %less, label %while.body.1, label %while.end.2

while.body.1:                                     ; preds = %while.cond.0
  %total2 = load i32, ptr %total, align 4
  %va_arg = va_arg ptr %args, i32
  %add = add i32 %total2, %va_arg
  store i32 %add, ptr %total, align 4
  %i3 = load i32, ptr %i, align 4
  %add4 = add i32 %i3, 1
  store i32 %add4, ptr %i, align 4
  br label %while.cond.0

while.end.2:                                      ; preds = %while.cond.0
  %total5 = load i32, ptr %total, align 4
  ret i32 %total5
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #0

define i32 @mul(i32 %0, ...) {
entry:
  %va_count = alloca i32, align 4
  store i32 %0, ptr %va_count, align 4
  %args = alloca [32 x i8], align 1
  %1 = getelementptr [32 x i8], ptr %args, i32 0
  call void @llvm.va_start.p0(ptr %1)
  %total = alloca i32, align 4
  store i32 0, ptr %total, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.cond.3

while.cond.3:                                     ; preds = %while.body.4, %entry
  %i1 = load i32, ptr %i, align 4
  %va_len = load i32, ptr %va_count, align 4
  %less = icmp slt i32 %i1, %va_len
  br i1 %less, label %while.body.4, label %while.end.5

while.body.4:                                     ; preds = %while.cond.3
  %total2 = load i32, ptr %total, align 4
  %va_arg = va_arg ptr %args, i32
  %mul = mul i32 %total2, %va_arg
  store i32 %mul, ptr %total, align 4
  %i3 = load i32, ptr %i, align 4
  %add = add i32 %i3, 1
  store i32 %add, ptr %i, align 4
  br label %while.cond.3

while.end.5:                                      ; preds = %while.cond.3
  %total4 = load i32, ptr %total, align 4
  ret i32 %total4
}

define i32 @main() {
entry:
  %sum = call i32 (i32, ...) @sum(i32 7, i32 1, i32 2, i32 3, i32 4, i32 2, i32 3, i32 4)
  ret i32 %sum
}

attributes #0 = { nocallback nofree nosync nounwind willreturn }
