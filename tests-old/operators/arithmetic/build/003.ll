; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpil0zmnh2.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpil0zmnh2.out"
target triple = "arm64-apple-darwin25.5.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 2, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %mul = mul i32 %a1, 10
  %add = add i32 %mul, 3
  store i32 %add, i32* %a, align 4
  %a2 = load i32, i32* %a, align 4
  ret i32 %a2
}
