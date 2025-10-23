; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@str_literal = private constant [5 x i8] c"abcd\00"

declare i32 @putchar(i32)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store ptr @str_literal, ptr %str, align 8
  %c = alloca i8, align 1
  %str1 = load ptr, ptr %str, align 8
  %access = getelementptr i8, ptr %str1, i32 1
  %access_val = load i8, ptr %access, align 1
  store i8 %access_val, ptr %c, align 1
  %c2 = load i8, ptr %c, align 1
  %putchar = call i32 @putchar(i8 %c2)
  ret i32 0
}
