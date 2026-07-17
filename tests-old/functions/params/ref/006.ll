; ModuleID = 'exe.out'
source_filename = "exe.out"

define i32 @tick(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %ref = load i32*, i32** %n, align 8
  %ref1 = load i32*, i32** %n, align 8
  %n2 = load i32, i32* %ref1, align 4
  %add = add i32 %n2, 1
  store i32 %add, i32* %ref, align 4
  %ref3 = load i32*, i32** %n, align 8
  %n4 = load i32, i32* %ref3, align 4
  ret i32 %n4
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %call = call i32 @tick(i32* %n)
  %call1 = call i32 @tick(i32* %n)
  %n2 = load i32, i32* %n, align 4
  ret i32 %n2
}
