; ModuleID = 'example'
source_filename = "example"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %a_val = load i32, ptr %a, align 4
  %sum = add i32 %a_val, 2
  %b = alloca i32, align 4
  store i32 %sum, ptr %b, align 4
  ret i32 0
}
