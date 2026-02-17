; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i8 97, ptr %a, align 1
  %b = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  store i32 %a1, ptr %b, align 4
  %b2 = load i32, ptr %b, align 4
  ret i32 %b2
}
