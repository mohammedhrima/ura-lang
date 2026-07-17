; ModuleID = 'exe.out'
source_filename = "exe.out"
target triple = "arm64-apple-darwin25.5.0"

define i1 @flag() {
entry:
  ret i1 true
}

define i32 @main() {
entry:
  %b = alloca i1, align 1
  %call = call i1 @flag()
  store i1 %call, i1* %b, align 1
  %c = alloca i1, align 1
  store i1 false, i1* %c, align 1
  %d = alloca i1, align 1
  store i1 false, i1* %d, align 1
  store i1 true, i1* %c, align 1
  ret i32 9
}
