; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

define i32 @foo() {
entry:
  ret i32 10
}

define i32 @bar() {
entry:
  ret i32 20
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
  %NOT_EQ = icmp ne i32 %a1, %b2
  store i1 %NOT_EQ, ptr %t1, align 1
  %t2 = alloca i1, align 1
  %a3 = load i32, ptr %a, align 4
  %NOT_EQ4 = icmp ne i32 %a3, 10
  store i1 %NOT_EQ4, ptr %t2, align 1
  %t3 = alloca i1, align 1
  %foo = call i32 @foo()
  %a5 = load i32, ptr %a, align 4
  %NOT_EQ6 = icmp ne i32 %a5, %foo
  store i1 %NOT_EQ6, ptr %t3, align 1
  %t4 = alloca i1, align 1
  %b7 = load i32, ptr %b, align 4
  %NOT_EQ8 = icmp ne i32 20, %b7
  store i1 %NOT_EQ8, ptr %t4, align 1
  %t5 = alloca i1, align 1
  store i1 false, ptr %t5, align 1
  %t6 = alloca i1, align 1
  %bar = call i32 @bar()
  %NOT_EQ9 = icmp ne i32 10, %bar
  store i1 %NOT_EQ9, ptr %t6, align 1
  %t7 = alloca i1, align 1
  %foo10 = call i32 @foo()
  %a11 = load i32, ptr %a, align 4
  %NOT_EQ12 = icmp ne i32 %foo10, %a11
  store i1 %NOT_EQ12, ptr %t7, align 1
  %t8 = alloca i1, align 1
  %bar13 = call i32 @bar()
  %NOT_EQ14 = icmp ne i32 %bar13, 20
  store i1 %NOT_EQ14, ptr %t8, align 1
  %t9 = alloca i1, align 1
  %foo15 = call i32 @foo()
  %bar16 = call i32 @bar()
  %NOT_EQ17 = icmp ne i32 %foo15, %bar16
  store i1 %NOT_EQ17, ptr %t9, align 1
  %n1 = alloca i1, align 1
  %a18 = load i32, ptr %a, align 4
  %b19 = load i32, ptr %b, align 4
  %EQUAL = icmp eq i32 %a18, %b19
  %NOT = xor i1 %EQUAL, true
  store i1 %NOT, ptr %n1, align 1
  %n2 = alloca i1, align 1
  %a20 = load i32, ptr %a, align 4
  %EQUAL21 = icmp eq i32 %a20, 10
  %NOT22 = xor i1 %EQUAL21, true
  store i1 %NOT22, ptr %n2, align 1
  %n3 = alloca i1, align 1
  %foo23 = call i32 @foo()
  %a24 = load i32, ptr %a, align 4
  %EQUAL25 = icmp eq i32 %a24, %foo23
  %NOT26 = xor i1 %EQUAL25, true
  store i1 %NOT26, ptr %n3, align 1
  %n4 = alloca i1, align 1
  %b27 = load i32, ptr %b, align 4
  %EQUAL28 = icmp eq i32 20, %b27
  %NOT29 = xor i1 %EQUAL28, true
  store i1 %NOT29, ptr %n4, align 1
  %n5 = alloca i1, align 1
  store i1 false, ptr %n5, align 1
  %n6 = alloca i1, align 1
  %bar30 = call i32 @bar()
  %EQUAL31 = icmp eq i32 10, %bar30
  %NOT32 = xor i1 %EQUAL31, true
  store i1 %NOT32, ptr %n6, align 1
  %n7 = alloca i1, align 1
  %foo33 = call i32 @foo()
  %a34 = load i32, ptr %a, align 4
  %EQUAL35 = icmp eq i32 %foo33, %a34
  %NOT36 = xor i1 %EQUAL35, true
  store i1 %NOT36, ptr %n7, align 1
  %n8 = alloca i1, align 1
  %bar37 = call i32 @bar()
  %EQUAL38 = icmp eq i32 %bar37, 20
  %NOT39 = xor i1 %EQUAL38, true
  store i1 %NOT39, ptr %n8, align 1
  %n9 = alloca i1, align 1
  %foo40 = call i32 @foo()
  %bar41 = call i32 @bar()
  %EQUAL42 = icmp eq i32 %foo40, %bar41
  %NOT43 = xor i1 %EQUAL42, true
  store i1 %NOT43, ptr %n9, align 1
  %t10 = alloca i1, align 1
  %a44 = load i32, ptr %a, align 4
  %b45 = load i32, ptr %b, align 4
  %EQUAL46 = icmp eq i32 %a44, %b45
  %NOT47 = xor i1 %EQUAL46, true
  store i1 %NOT47, ptr %t10, align 1
  %t11 = alloca i1, align 1
  %foo48 = call i32 @foo()
  %bar49 = call i32 @bar()
  %EQUAL50 = icmp eq i32 %foo48, %bar49
  %NOT51 = xor i1 %EQUAL50, true
  store i1 %NOT51, ptr %t11, align 1
  ret i32 0
}
