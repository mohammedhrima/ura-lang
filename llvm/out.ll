; ModuleID = 'putchar_example'
source_filename = "putchar_example"

@str = private unnamed_addr constant [5 x i8] c"abcd\00", align 1

declare i32 @putchar(i32)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store ptr @str, ptr %str, align 8
  %str_val = load ptr, ptr %str, align 8
  %elem_ptr = getelementptr i8, ptr %str_val, i32 0
  %c = load i8, ptr %elem_ptr, align 1
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %c_val = load i8, ptr %c1, align 1
  %c_ext = sext i8 %c_val to i32
  %0 = call i32 @putchar(i32 %c_ext)
  ret i32 0
}
