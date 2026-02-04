; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

define i32 @add_multiply(i32 %0, i32 %1, i32 %2) {
entry:
  %param = alloca i32, align 4
  store i32 %0, ptr %param, align 4
  %param1 = alloca i32, align 4
  store i32 %1, ptr %param1, align 4
  %param2 = alloca i32, align 4
  store i32 %2, ptr %param2, align 4
  %sum = alloca i32, align 4
  %a = load i32, ptr %param, align 4
  %b = load i32, ptr %param1, align 4
  %add = add i32 %a, %b
  store i32 %add, ptr %sum, align 4
  %result = alloca i32, align 4
  %sum3 = load i32, ptr %sum, align 4
  %c = load i32, ptr %param2, align 4
  %mul = mul i32 %sum3, %c
  store i32 %mul, ptr %result, align 4
  %result4 = load i32, ptr %result, align 4
  ret i32 %result4
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %add_multiply = call i32 @add_multiply(i32 5, i32 3, i32 2)
  store i32 %add_multiply, ptr %x, align 4
  %x1 = load i32, ptr %x, align 4
  ret i32 %x1
}
