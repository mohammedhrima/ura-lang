; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 2, ptr %b, align 4
  %pa = alloca i32, align 4
  %pb = alloca i32, align 4
  %pb1 = load i32, ptr %b, align 4
  store i32 %pb1, ptr %a, align 4
  %a2 = load i32, ptr %a, align 4
  ret i32 %a2
}
