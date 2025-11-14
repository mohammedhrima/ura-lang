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
  store i32 15, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 8, ptr %b, align 4
  %t1 = alloca i1, align 1
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %MORE = icmp sgt i32 %a1, %b2
  store i1 %MORE, ptr %t1, align 1
  %t2 = alloca i1, align 1
  %a3 = load i32, ptr %a, align 4
  %MORE4 = icmp sgt i32 %a3, 10
  store i1 %MORE4, ptr %t2, align 1
  %t3 = alloca i1, align 1
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %MORE6 = icmp sgt i32 %a5, %foo
  store i1 %MORE6, ptr %t3, align 1
  %t4 = alloca i1, align 1
  %b7 = load i32, ptr %b, align 4
  %MORE8 = icmp sgt i32 20, %b7
  store i1 %MORE8, ptr %t4, align 1
  %t5 = alloca i1, align 1
  store i1 true, ptr %t5, align 1
  %t6 = alloca i1, align 1
  %bar = call i32 @bar()
  %MORE9 = icmp sgt i32 15, %bar
  store i1 %MORE9, ptr %t6, align 1
  %t7 = alloca i1, align 1
  %bar10 = call i32 @bar()
  %b11 = load i32, ptr %b, align 4
  %MORE12 = icmp sgt i32 %bar10, %b11
  store i1 %MORE12, ptr %t7, align 1
  %t8 = alloca i1, align 1
  %bar13 = call i32 @bar()
  %MORE14 = icmp sgt i32 %bar13, 5
  store i1 %MORE14, ptr %t8, align 1
  %t9 = alloca i1, align 1
  %bar15 = call i32 @bar()
  %foo16 = call i32 @foo()
  %MORE17 = icmp sgt i32 %bar15, %foo16
  store i1 %MORE17, ptr %t9, align 1
  ret i32 0
}
