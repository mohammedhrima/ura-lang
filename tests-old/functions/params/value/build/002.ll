; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpg98vryfe.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpg98vryfe.out"
target triple = "arm64-apple-darwin25.5.0"

define i32 @inc(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %add = add i32 %n1, 1
  store i32 %add, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  ret i32 %n2
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 5, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %call = call i32 @inc(i32 %n1)
  %n2 = load i32, i32* %n, align 4
  ret i32 %n2
}
