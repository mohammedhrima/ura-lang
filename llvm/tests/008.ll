; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

define void @func(ptr %0) {
entry:
  store i32 2, ptr %0, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 12, ptr %a, align 4
  %c = alloca i32, align 4
  call void @func(ptr %a)
  %a1 = load i32, ptr %a, align 4
  ret i32 %a1
}
