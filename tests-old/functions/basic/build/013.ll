; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpl529zp5z.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpl529zp5z.out"
target triple = "arm64-apple-darwin25.5.0"

define i32 @main() {
entry:
  %call = call i32 @dbl(i32 21)
  ret i32 %call
}

define i32 @dbl(i32 %0) {
entry:
  %k = alloca i32, align 4
  store i32 %0, i32* %k, align 4
  %k1 = load i32, i32* %k, align 4
  %k2 = load i32, i32* %k, align 4
  %add = add i32 %k1, %k2
  ret i32 %add
}
