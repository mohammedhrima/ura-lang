; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %stack = alloca i8, i32 10, align 1
  %cast = ptrtoint ptr %stack to i32
  store i32 %cast, ptr %a, align 4
  %a1 = load i32, ptr %a, align 4
  ret i32 %a1
}
