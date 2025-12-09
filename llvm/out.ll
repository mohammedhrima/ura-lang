; ModuleID = 'module'
source_filename = "module"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca ptr, align 8
  store ptr %a, ptr %b, align 8
  %b_val = load ptr, ptr %b, align 8
  store i32 2, ptr %b_val, align 4
  %result = load i32, ptr %b_val, align 4
  ret i32 %result
}
