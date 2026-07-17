; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp92rcla_v.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp92rcla_v.out"
target triple = "arm64-apple-darwin25.5.0"

define i32 @square(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, %n2
  ret i32 %mul
}

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
  %call = call i32 @square(i32 5)
  %call1 = call i32 @add(i32 %call, i32 3)
  ret i32 %call1
}
