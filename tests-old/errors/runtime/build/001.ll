; ModuleID = 'exe.out'
source_filename = "exe.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [115 x i8] c"runtime error: Division by zero\0A  tests/errors/runtime/001.ura:6:15\0A  |\0A6 |     c int = a / b\0A  |               ^\0A\00", align 1

define i32 @main() {
entry:
  %b = alloca i32, align 4
  store i32 0, i32* %b, align 4
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %c = alloca i32, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %iszero = icmp eq i32 %b2, 0
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([115 x i8], [115 x i8]* @trap_msg, i32 0, i32 0), i64 114)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %a1, %b2
  store i32 %div, i32* %c, align 4
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
