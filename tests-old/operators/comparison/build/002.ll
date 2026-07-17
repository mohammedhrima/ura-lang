; ModuleID = 'exe.out'
source_filename = "exe.out"
target triple = "arm64-apple-darwin25.5.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 9, i32* %b, align 4
  %t = alloca i1, align 1
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %lt = icmp slt i32 %a1, %b2
  store i1 %lt, i1* %t, align 1
  %u = alloca i1, align 1
  %a3 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a3, 5
  store i1 %eq, i1* %u, align 1
  %v = alloca i1, align 1
  %b4 = load i32, i32* %b, align 4
  %a5 = load i32, i32* %a, align 4
  %ne = icmp ne i32 %b4, %a5
  store i1 %ne, i1* %v, align 1
  %w = alloca i1, align 1
  %b6 = load i32, i32* %b, align 4
  %ge = icmp sge i32 %b6, 9
  store i1 %ge, i1* %w, align 1
  %x = alloca i1, align 1
  %a7 = load i32, i32* %a, align 4
  %le = icmp sle i32 %a7, 4
  store i1 %le, i1* %x, align 1
  %y = alloca i1, align 1
  %a8 = load i32, i32* %a, align 4
  %b9 = load i32, i32* %b, align 4
  %gt = icmp sgt i32 %a8, %b9
  store i1 %gt, i1* %y, align 1
  %z = alloca i1, align 1
  %a10 = load i32, i32* %a, align 4
  %eq11 = icmp eq i32 %a10, 5
  store i1 %eq11, i1* %z, align 1
  %m = alloca i1, align 1
  %a12 = load i32, i32* %a, align 4
  %add = add i32 %a12, 1
  %b13 = load i32, i32* %b, align 4
  %mul = mul i32 %b13, 2
  %lt14 = icmp slt i32 %add, %mul
  store i1 %lt14, i1* %m, align 1
  ret i32 3
}
