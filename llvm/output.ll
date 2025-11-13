; ModuleID = 'string_example'
source_filename = "string_example"

@str_literal = private constant [4 x i8] c"abc\00"

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store ptr @str_literal, ptr %str, align 8
  %c = alloca i32, align 4
  %str1 = load ptr, ptr %str, align 8
  %access = getelementptr i8, ptr %str1, i32 0
  %access_val = load i8, ptr %access, align 1
  %extended = zext i8 %access_val to i32
  store i32 %extended, ptr %c, align 4
  %a = alloca i32, align 4
  %c1 = load i32, ptr %c, align 4
  %ADD = add i32 %c1, 2
  store i32 %ADD, ptr %a, align 4
  %b = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  store i32 %a1, ptr %b, align 4
  ret i32 0
}
