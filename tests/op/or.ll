; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i1 @foo() {
entry:
  ret i1 true
}

define i1 @bar() {
entry:
  ret i1 false
}

define i32 @main() {
entry:
  %a = alloca i1, align 1
  store i1 true, ptr %a, align 1
  %b = alloca i1, align 1
  store i1 false, ptr %b, align 1
  %t1 = alloca i1, align 1
  %a1 = load i1, ptr %a, align 1
  %b2 = load i1, ptr %b, align 1
  %OR = or i1 %a1, %b2
  store i1 %OR, ptr %t1, align 1
  %t2 = alloca i1, align 1
  %b3 = load i1, ptr %b, align 1
  %OR4 = or i1 %b3, true
  store i1 %OR4, ptr %t2, align 1
  %t3 = alloca i1, align 1
  %foo = call i1 @foo()
  %b5 = load i1, ptr %b, align 1
  %OR6 = or i1 %b5, %foo
  store i1 %OR6, ptr %t3, align 1
  %t4 = alloca i1, align 1
  %a7 = load i1, ptr %a, align 1
  %OR8 = or i1 false, %a7
  store i1 %OR8, ptr %t4, align 1
  %t5 = alloca i1, align 1
  store i1 false, ptr %t5, align 1
  %t6 = alloca i1, align 1
  %bar = call i1 @bar()
  %OR9 = or i1 false, %bar
  store i1 %OR9, ptr %t6, align 1
  %t7 = alloca i1, align 1
  %bar10 = call i1 @bar()
  %a11 = load i1, ptr %a, align 1
  %OR12 = or i1 %bar10, %a11
  store i1 %OR12, ptr %t7, align 1
  %t8 = alloca i1, align 1
  %foo13 = call i1 @foo()
  %OR14 = or i1 %foo13, false
  store i1 %OR14, ptr %t8, align 1
  %t9 = alloca i1, align 1
  %bar15 = call i1 @bar()
  %foo16 = call i1 @foo()
  %OR17 = or i1 %bar15, %foo16
  store i1 %OR17, ptr %t9, align 1
  %t10 = alloca i1, align 1
  %a18 = load i1, ptr %a, align 1
  %b19 = load i1, ptr %b, align 1
  %OR20 = or i1 %a18, %b19
  store i1 %OR20, ptr %t10, align 1
  %t11 = alloca i1, align 1
  store i1 true, ptr %t11, align 1
  %t12 = alloca i1, align 1
  %bar21 = call i1 @bar()
  %foo22 = call i1 @foo()
  %OR23 = or i1 %bar21, %foo22
  store i1 %OR23, ptr %t12, align 1
  %t13 = alloca i1, align 1
  %bar24 = call i1 @bar()
  %OR25 = or i1 true, %bar24
  store i1 %OR25, ptr %t13, align 1
  %t14 = alloca i1, align 1
  %foo26 = call i1 @foo()
  %a27 = load i1, ptr %a, align 1
  %OR28 = or i1 %a27, %foo26
  store i1 %OR28, ptr %t14, align 1
  ret i32 0
}
