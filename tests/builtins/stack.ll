; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@STR0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

declare i32 @write(i32, ptr, i32)

define i32 @putchar(i8 %c) {
entry:
  %c1 = alloca i8, align 1
  store i8 %c, ptr %c1, align 1
  %str = alloca ptr, align 8
  %stack = alloca [2 x i8], align 1
  %stack2 = getelementptr [2 x i8], ptr %stack, i32 0, i32 0
  store ptr %stack2, ptr %str, align 8
  %str3 = load ptr, ptr %str, align 8
  %ACCESS = getelementptr i8, ptr %str3, i32 0
  %c4 = load i8, ptr %c1, align 1
  store i8 %c4, ptr %ACCESS, align 1
  %str5 = load ptr, ptr %str, align 8
  %write = call i32 @write(i32 1, ptr %str5, i32 1)
  ret i32 0
}

define i32 @main() {
entry:
  %putchar = call i32 @putchar(i8 97)
  %putchar1 = call i32 @putchar(i8 10)
  ret i32 0
}
