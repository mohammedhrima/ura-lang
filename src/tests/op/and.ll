; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

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
  %AND = and i1 %a1, %b2
  store i1 %AND, ptr %t1, align 1
  %t2 = alloca i1, align 1
  %a3 = load i1, ptr %a, align 1
  %AND4 = and i1 %a3, true
  store i1 %AND4, ptr %t2, align 1
  %t3 = alloca i1, align 1
  %foo = call i1 @foo()
  %a5 = load i1, ptr %a, align 1
  %AND6 = and i1 %a5, %foo
  store i1 %AND6, ptr %t3, align 1
  %t4 = alloca i1, align 1
  %b7 = load i1, ptr %b, align 1
  %AND8 = and i1 false, %b7
  store i1 %AND8, ptr %t4, align 1
  %t5 = alloca i1, align 1
  store i1 true, ptr %t5, align 1
  %t6 = alloca i1, align 1
  %bar = call i1 @bar()
  %AND9 = and i1 true, %bar
  store i1 %AND9, ptr %t6, align 1
  %t7 = alloca i1, align 1
  %foo10 = call i1 @foo()
  %a11 = load i1, ptr %a, align 1
  %AND12 = and i1 %foo10, %a11
  store i1 %AND12, ptr %t7, align 1
  %t8 = alloca i1, align 1
  %bar13 = call i1 @bar()
  %AND14 = and i1 %bar13, false
  store i1 %AND14, ptr %t8, align 1
  %t9 = alloca i1, align 1
  %foo15 = call i1 @foo()
  %bar16 = call i1 @bar()
  %AND17 = and i1 %foo15, %bar16
  store i1 %AND17, ptr %t9, align 1
  %t10 = alloca i1, align 1
  %a18 = load i1, ptr %a, align 1
  %b19 = load i1, ptr %b, align 1
  %AND20 = and i1 %a18, %b19
  store i1 %AND20, ptr %t10, align 1
  %t11 = alloca i1, align 1
  store i1 false, ptr %t11, align 1
  %t12 = alloca i1, align 1
  %foo21 = call i1 @foo()
  %bar22 = call i1 @bar()
  %AND23 = and i1 %foo21, %bar22
  store i1 %AND23, ptr %t12, align 1
  %t13 = alloca i1, align 1
  %bar24 = call i1 @bar()
  %AND25 = and i1 false, %bar24
  store i1 %AND25, ptr %t13, align 1
  %t14 = alloca i1, align 1
  %foo26 = call i1 @foo()
  %b27 = load i1, ptr %b, align 1
  %AND28 = and i1 %b27, %foo26
  store i1 %AND28, ptr %t14, align 1
  ret i32 0
}
