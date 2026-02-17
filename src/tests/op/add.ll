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
  store i32 10, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 20, ptr %b, align 4
  %t1 = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %ADD = add i32 %a1, %b2
  store i32 %ADD, ptr %t1, align 4
  %t2 = alloca i32, align 4
  %a3 = load i32, ptr %a, align 4
  %ADD4 = add i32 %a3, 15
  store i32 %ADD4, ptr %t2, align 4
  %t3 = alloca i32, align 4
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %ADD6 = add i32 %a5, %foo
  store i32 %ADD6, ptr %t3, align 4
  %t4 = alloca i32, align 4
  %b7 = load i32, ptr %b, align 4
  %ADD8 = add i32 25, %b7
  store i32 %ADD8, ptr %t4, align 4
  %t5 = alloca i32, align 4
  store i32 70, ptr %t5, align 4
  %t6 = alloca i32, align 4
  %bar = call i32 @bar()
  %ADD9 = add i32 50, %bar
  store i32 %ADD9, ptr %t6, align 4
  %t7 = alloca i32, align 4
  %foo10 = call i32 @foo()
  %a11 = load i32, ptr %a, align 4
  %ADD12 = add i32 %foo10, %a11
  store i32 %ADD12, ptr %t7, align 4
  %t8 = alloca i32, align 4
  %bar13 = call i32 @bar()
  %ADD14 = add i32 %bar13, 100
  store i32 %ADD14, ptr %t8, align 4
  %t9 = alloca i32, align 4
  %foo15 = call i32 @foo()
  %bar16 = call i32 @bar()
  %ADD17 = add i32 %foo15, %bar16
  store i32 %ADD17, ptr %t9, align 4
  ret i32 0
}
