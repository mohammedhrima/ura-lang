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
  %NOT = xor i1 %a1, true
  store i1 %NOT, ptr %t1, align 1
  %t2 = alloca i1, align 1
  %b2 = load i1, ptr %b, align 1
  %NOT3 = xor i1 %b2, true
  store i1 %NOT3, ptr %t2, align 1
  %t3 = alloca i1, align 1
  store i1 false, ptr %t3, align 1
  %t4 = alloca i1, align 1
  store i1 true, ptr %t4, align 1
  %t5 = alloca i1, align 1
  %foo = call i1 @foo()
  %NOT4 = xor i1 %foo, true
  store i1 %NOT4, ptr %t5, align 1
  %t6 = alloca i1, align 1
  %bar = call i1 @bar()
  %NOT5 = xor i1 %bar, true
  store i1 %NOT5, ptr %t6, align 1
  %t7 = alloca i1, align 1
  %a6 = load i1, ptr %a, align 1
  %NOT7 = xor i1 %a6, true
  %NOT8 = xor i1 %NOT7, true
  store i1 %NOT8, ptr %t7, align 1
  %t8 = alloca i1, align 1
  %b9 = load i1, ptr %b, align 1
  %NOT10 = xor i1 %b9, true
  %NOT11 = xor i1 %NOT10, true
  store i1 %NOT11, ptr %t8, align 1
  %t9 = alloca i1, align 1
  store i1 true, ptr %t9, align 1
  %t10 = alloca i1, align 1
  store i1 false, ptr %t10, align 1
  %t11 = alloca i1, align 1
  %foo12 = call i1 @foo()
  %NOT13 = xor i1 %foo12, true
  %NOT14 = xor i1 %NOT13, true
  store i1 %NOT14, ptr %t11, align 1
  %t12 = alloca i1, align 1
  %bar15 = call i1 @bar()
  %NOT16 = xor i1 %bar15, true
  %NOT17 = xor i1 %NOT16, true
  store i1 %NOT17, ptr %t12, align 1
  ret i32 0
}
