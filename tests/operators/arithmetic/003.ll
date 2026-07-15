; ModuleID = 'exe.out'
source_filename = "exe.out"

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
