; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i32 @foo() {
entry:
  ret i32 10
}

define i32 @bar() {
entry:
  ret i32 20
}

define i32 @zero() {
entry:
  ret i32 0
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 20, ptr %b, align 4
  %t1 = alloca i1, align 1
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %EQUAL = icmp eq i32 %a1, %b2
  store i1 %EQUAL, ptr %t1, align 1
  %t2 = alloca i1, align 1
  %a3 = load i32, ptr %a, align 4
  %EQUAL4 = icmp eq i32 %a3, 10
  store i1 %EQUAL4, ptr %t2, align 1
  %t3 = alloca i1, align 1
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %EQUAL6 = icmp eq i32 %a5, %foo
  store i1 %EQUAL6, ptr %t3, align 1
  %t4 = alloca i1, align 1
  %b7 = load i32, ptr %b, align 4
  %EQUAL8 = icmp eq i32 20, %b7
  store i1 %EQUAL8, ptr %t4, align 1
  %t5 = alloca i1, align 1
  store i1 true, ptr %t5, align 1
  %t6 = alloca i1, align 1
  %bar = call i32 @bar()
  %EQUAL9 = icmp eq i32 10, %bar
  store i1 %EQUAL9, ptr %t6, align 1
  %t7 = alloca i1, align 1
  %foo10 = call i32 @foo()
  %a11 = load i32, ptr %a, align 4
  %EQUAL12 = icmp eq i32 %foo10, %a11
  store i1 %EQUAL12, ptr %t7, align 1
  %t8 = alloca i1, align 1
  %bar13 = call i32 @bar()
  %EQUAL14 = icmp eq i32 %bar13, 20
  store i1 %EQUAL14, ptr %t8, align 1
  %t9 = alloca i1, align 1
  %foo15 = call i32 @foo()
  %bar16 = call i32 @bar()
  %EQUAL17 = icmp eq i32 %foo15, %bar16
  store i1 %EQUAL17, ptr %t9, align 1
  %t10 = alloca i1, align 1
  %zero = call i32 @zero()
  %EQUAL18 = icmp eq i32 %zero, 0
  store i1 %EQUAL18, ptr %t10, align 1
  %t11 = alloca i1, align 1
  %zero19 = call i32 @zero()
  %foo20 = call i32 @foo()
  %EQUAL21 = icmp eq i32 %zero19, %foo20
  store i1 %EQUAL21, ptr %t11, align 1
  %t12 = alloca i1, align 1
  %foo22 = call i32 @foo()
  %foo23 = call i32 @foo()
  %EQUAL24 = icmp eq i32 %foo22, %foo23
  store i1 %EQUAL24, ptr %t12, align 1
  %t13 = alloca i1, align 1
  %a25 = load i32, ptr %a, align 4
  %b26 = load i32, ptr %b, align 4
  %EQUAL27 = icmp eq i32 %a25, %b26
  store i1 %EQUAL27, ptr %t13, align 1
  %t14 = alloca i1, align 1
  %foo28 = call i32 @foo()
  %bar29 = call i32 @bar()
  %EQUAL30 = icmp eq i32 %foo28, %bar29
  store i1 %EQUAL30, ptr %t14, align 1
  %t15 = alloca i1, align 1
  %foo31 = call i32 @foo()
  %bar32 = call i32 @bar()
  %EQUAL33 = icmp eq i32 %foo31, %bar32
  store i1 %EQUAL33, ptr %t15, align 1
  ret i32 0
}
