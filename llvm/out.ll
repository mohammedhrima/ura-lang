; ModuleID = 'example'
source_filename = "example"

define i32 @main() {
entry:
  %a = alloca [10 x i8], align 1
  %a1 = getelementptr [10 x i8], ptr %a, i32 0, i32 0
  %access = getelementptr i8, ptr %a1, i32 0
  store i8 72, ptr %access, align 1
  ret i32 0
}
