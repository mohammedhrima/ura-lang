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
  store i32 6, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 4, ptr %b, align 4
  %t1 = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  %MUL = mul i32 %a1, %b2
  store i32 %MUL, ptr %t1, align 4
  %t2 = alloca i32, align 4
  %a3 = load i32, ptr %a, align 4
  %MUL4 = mul i32 %a3, 3
  store i32 %MUL4, ptr %t2, align 4
  %t3 = alloca i32, align 4
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %MUL6 = mul i32 %a5, %foo
  store i32 %MUL6, ptr %t3, align 4
  %t4 = alloca i32, align 4
  %b7 = load i32, ptr %b, align 4
  %MUL8 = mul i32 7, %b7
  store i32 %MUL8, ptr %t4, align 4
  %t5 = alloca i32, align 4
  store i32 72, ptr %t5, align 4
  %t6 = alloca i32, align 4
  %bar = call i32 @bar()
  %MUL9 = mul i32 2, %bar
  store i32 %MUL9, ptr %t6, align 4
  %t7 = alloca i32, align 4
  %foo10 = call i32 @foo()
  %a11 = load i32, ptr %a, align 4
  %MUL12 = mul i32 %foo10, %a11
  store i32 %MUL12, ptr %t7, align 4
  %t8 = alloca i32, align 4
  %bar13 = call i32 @bar()
  %MUL14 = mul i32 %bar13, 5
  store i32 %MUL14, ptr %t8, align 4
  %t9 = alloca i32, align 4
  %foo15 = call i32 @foo()
  %bar16 = call i32 @bar()
  %MUL17 = mul i32 %foo15, %bar16
  store i32 %MUL17, ptr %t9, align 4
  ret i32 0
}
