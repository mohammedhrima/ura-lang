; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

define i32 @foo() {
entry:
  ret i32 5
}

define i32 @bar() {
entry:
  ret i32 10
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 20, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 4, ptr %b, align 4
  %t1 = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %DIV = sdiv i32 %a1, %b2
  store i32 %DIV, ptr %t1, align 4
  %t2 = alloca i32, align 4
  %a3 = load i32, ptr %a, align 4
  %DIV4 = sdiv i32 %a3, 2
  store i32 %DIV4, ptr %t2, align 4
  %t3 = alloca i32, align 4
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %DIV6 = sdiv i32 %a5, %foo
  store i32 %DIV6, ptr %t3, align 4
  %t4 = alloca i32, align 4
  %b7 = load i32, ptr %b, align 4
  %DIV8 = sdiv i32 100, %b7
  store i32 %DIV8, ptr %t4, align 4
  %t5 = alloca i32, align 4
  store i32 5, ptr %t5, align 4
  %t6 = alloca i32, align 4
  %foo9 = call i32 @foo()
  %DIV10 = sdiv i32 30, %foo9
  store i32 %DIV10, ptr %t6, align 4
  %t7 = alloca i32, align 4
  %bar = call i32 @bar()
  %b11 = load i32, ptr %b, align 4
  %DIV12 = sdiv i32 %bar, %b11
  store i32 %DIV12, ptr %t7, align 4
  %t8 = alloca i32, align 4
  %bar13 = call i32 @bar()
  %DIV14 = sdiv i32 %bar13, 2
  store i32 %DIV14, ptr %t8, align 4
  %t9 = alloca i32, align 4
  %bar15 = call i32 @bar()
  %foo16 = call i32 @foo()
  %DIV17 = sdiv i32 %bar15, %foo16
  store i32 %DIV17, ptr %t9, align 4
  ret i32 0
}
