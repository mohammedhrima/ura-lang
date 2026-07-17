; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp7lxjb45k.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp7lxjb45k.out"
target triple = "arm64-apple-darwin25.5.0"

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %add = add i32 %a1, %b2
  ret i32 %add
}

define i32 @main() {
entry:
  %call = call i32 @add(i32 40, i32 2)
  ret i32 %call
}
