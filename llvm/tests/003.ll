; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [5 x i8] c"abcd\00", align 1

declare i32 @putchar(i8)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  %c = alloca i8, align 1
  %ACCESS = load i8, ptr getelementptr (i8, ptr @STR0, i32 1), align 1
  store i8 %ACCESS, ptr %c, align 1
  %c1 = load i8, ptr %c, align 1
  %putchar = call i32 @putchar(i8 %c1)
  ret i32 0
}
