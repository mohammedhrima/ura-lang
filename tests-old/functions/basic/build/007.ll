; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp8f66w2l8.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp8f66w2l8.out"
target triple = "arm64-apple-darwin25.5.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 7, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  ret i32 %a1
}
