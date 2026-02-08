; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  %stack = alloca [2 x i8], align 1
  %stack1 = getelementptr [2 x i8], ptr %stack, i32 0, i32 0
  %stack2 = load ptr, ptr %stack1, align 8
  store ptr %stack2, ptr %str, align 8
  %a = alloca i32, align 4
  %as = ptrtoint ptr %str to i32
  store i32 %as, ptr %a, align 4
  %a3 = load i32, ptr %a, align 4
  ret i32 %a3
}
