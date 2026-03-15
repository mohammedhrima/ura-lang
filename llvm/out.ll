; ModuleID = 'multi_args_example'
source_filename = "multi_args_example"

define i32 @add_multiply(i32 %0, i32 %1, i32 %2) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %sum = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  store i32 %1, ptr %b, align 4
  store i32 %2, ptr %c, align 4
  %a_val = load i32, ptr %a, align 4
  %b_val = load i32, ptr %b, align 4
  %c_val = load i32, ptr %c, align 4
  %sum_temp = add i32 %a_val, %b_val
  store i32 %sum_temp, ptr %sum, align 4
  %sum_val = load i32, ptr %sum, align 4
  %result_temp = mul i32 %sum_val, %c_val
  store i32 %result_temp, ptr %result, align 4
  %return_val = load i32, ptr %result, align 4
  ret i32 %return_val
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %call_result = call i32 @add_multiply(i32 5, i32 3, i32 2)
  store i32 %call_result, ptr %x, align 4
  %x_val = load i32, ptr %x, align 4
  ret i32 %x_val
}
