; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

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
  store i32 10, ptr %b, align 4
  %t1 = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %SUB = sub i32 %a1, %b2
  store i32 %SUB, ptr %t1, align 4
  %t2 = alloca i32, align 4
  %a3 = load i32, ptr %a, align 4
  %SUB4 = sub i32 %a3, 5
  store i32 %SUB4, ptr %t2, align 4
  %t3 = alloca i32, align 4
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %SUB6 = sub i32 %a5, %foo
  store i32 %SUB6, ptr %t3, align 4
  %t4 = alloca i32, align 4
  %b7 = load i32, ptr %b, align 4
  %SUB8 = sub i32 30, %b7
  store i32 %SUB8, ptr %t4, align 4
  %t5 = alloca i32, align 4
  store i32 35, ptr %t5, align 4
  %t6 = alloca i32, align 4
  %bar = call i32 @bar()
  %SUB9 = sub i32 40, %bar
  store i32 %SUB9, ptr %t6, align 4
  %t7 = alloca i32, align 4
  %foo10 = call i32 @foo()
  %b11 = load i32, ptr %b, align 4
  %SUB12 = sub i32 %foo10, %b11
  store i32 %SUB12, ptr %t7, align 4
  %t8 = alloca i32, align 4
  %bar13 = call i32 @bar()
  %SUB14 = sub i32 %bar13, 3
  store i32 %SUB14, ptr %t8, align 4
  %t9 = alloca i32, align 4
  %bar15 = call i32 @bar()
  %foo16 = call i32 @foo()
  %SUB17 = sub i32 %bar15, %foo16
  store i32 %SUB17, ptr %t9, align 4
  ret i32 0
}
