; ModuleID = 'exe.out'
source_filename = "exe.out"

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %add = add i32 %n1, %n2
  ret i32 %add
}

define i32 @apply(i32 (i32)* %0, i32 %1) {
entry:
  %f = alloca i32 (i32)*, align 8
  store i32 (i32)* %0, i32 (i32)** %f, align 8
  %x = alloca i32, align 4
  store i32 %1, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %fn = load i32 (i32)*, i32 (i32)** %f, align 8
  %call = call i32 %fn(i32 %x1)
  ret i32 %call
}

define i32 @main() {
entry:
  %call = call i32 @apply(i32 (i32)* @twice, i32 21)
  ret i32 %call
}
