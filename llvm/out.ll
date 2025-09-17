; ModuleID = 'variadic_module'
source_filename = "variadic_module"

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #0

define i32 @sum(i32 %count, ...) {
entry:
  %args = alloca [24 x i8], align 1
  %total = alloca i32, align 4
  store i32 0, ptr %total, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  call void @llvm.va_start.p0(ptr %args)
  br label %loop_header

loop_header:                                      ; preds = %loop_body, %entry
  %i_val = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %i_val, %count
  br i1 %cmp, label %loop_body, label %loop_exit

loop_body:                                        ; preds = %loop_header
  %offset = mul i32 %i_val, 8
  %arg_ptr = getelementptr inbounds [24 x i8], ptr %args, i32 0, i32 %offset
  %arg_val = load i32, ptr %arg_ptr, align 4
  %total_val = load i32, ptr %total, align 4
  %new_total = add i32 %total_val, %arg_val
  store i32 %new_total, ptr %total, align 4
  %new_i = add i32 %i_val, 1
  store i32 %new_i, ptr %i, align 4
  br label %loop_header

loop_exit:                                        ; preds = %loop_header
  call void @llvm.va_end.p0(ptr %args)
  %final_total = load i32, ptr %total, align 4
  ret i32 %final_total
}

define i32 @main() {
entry:
  %result = call i32 (i32, ...) @sum(i32 3, i32 10, i32 20, i32 30)
  ret i32 %result
}

attributes #0 = { nocallback nofree nosync nounwind willreturn }
