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
  store i32 10, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 10, ptr %b, align 4
  %t1 = alloca i1, align 1
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %LE_EQ = icmp sle i32 %a1, %b2
  store i1 %LE_EQ, ptr %t1, align 1
  %t2 = alloca i1, align 1
  %a3 = load i32, ptr %a, align 4
  %LE_EQ4 = icmp sle i32 %a3, 15
  store i1 %LE_EQ4, ptr %t2, align 1
  %t3 = alloca i1, align 1
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %LE_EQ6 = icmp sle i32 %a5, %foo
  store i1 %LE_EQ6, ptr %t3, align 1
  %t4 = alloca i1, align 1
  %b7 = load i32, ptr %b, align 4
  %LE_EQ8 = icmp sle i32 10, %b7
  store i1 %LE_EQ8, ptr %t4, align 1
  %t5 = alloca i1, align 1
  store i1 true, ptr %t5, align 1
  %t6 = alloca i1, align 1
  %bar = call i32 @bar()
  %LE_EQ9 = icmp sle i32 10, %bar
  store i1 %LE_EQ9, ptr %t6, align 1
  %t7 = alloca i1, align 1
  %foo10 = call i32 @foo()
  %a11 = load i32, ptr %a, align 4
  %LE_EQ12 = icmp sle i32 %foo10, %a11
  store i1 %LE_EQ12, ptr %t7, align 1
  %t8 = alloca i1, align 1
  %bar13 = call i32 @bar()
  %LE_EQ14 = icmp sle i32 %bar13, 10
  store i1 %LE_EQ14, ptr %t8, align 1
  %t9 = alloca i1, align 1
  %foo15 = call i32 @foo()
  %bar16 = call i32 @bar()
  %LE_EQ17 = icmp sle i32 %foo15, %bar16
  store i1 %LE_EQ17, ptr %t9, align 1
  ret i32 0
}
