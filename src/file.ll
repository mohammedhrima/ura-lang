; ModuleID = 'file.w'
source_filename = "file.w"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 3, ptr %b, align 4
  %c = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %ADD = add i32 %a1, %b2
  store i32 %ADD, ptr %c, align 4
  ret i32 0
}
