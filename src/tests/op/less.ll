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
  store i32 8, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 12, ptr %b, align 4
  %t1 = alloca i1, align 1
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %LESS = icmp slt i32 %a1, %b2
  store i1 %LESS, ptr %t1, align 1
  %t2 = alloca i1, align 1
  %a3 = load i32, ptr %a, align 4
  %LESS4 = icmp slt i32 %a3, 10
  store i1 %LESS4, ptr %t2, align 1
  %t3 = alloca i1, align 1
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %LESS6 = icmp slt i32 %a5, %foo
  store i1 %LESS6, ptr %t3, align 1
  %t4 = alloca i1, align 1
  %b7 = load i32, ptr %b, align 4
  %LESS8 = icmp slt i32 5, %b7
  store i1 %LESS8, ptr %t4, align 1
  %t5 = alloca i1, align 1
  store i1 true, ptr %t5, align 1
  %t6 = alloca i1, align 1
  %bar = call i32 @bar()
  %LESS9 = icmp slt i32 4, %bar
  store i1 %LESS9, ptr %t6, align 1
  %t7 = alloca i1, align 1
  %foo10 = call i32 @foo()
  %b11 = load i32, ptr %b, align 4
  %LESS12 = icmp slt i32 %foo10, %b11
  store i1 %LESS12, ptr %t7, align 1
  %t8 = alloca i1, align 1
  %bar13 = call i32 @bar()
  %LESS14 = icmp slt i32 %bar13, 15
  store i1 %LESS14, ptr %t8, align 1
  %t9 = alloca i1, align 1
  %foo15 = call i32 @foo()
  %bar16 = call i32 @bar()
  %LESS17 = icmp slt i32 %foo15, %bar16
  store i1 %LESS17, ptr %t9, align 1
  ret i32 0
}
