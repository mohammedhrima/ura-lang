; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

@STR0 = private unnamed_addr constant [13 x i8] c"hello world\0A\00", align 1
@STR1 = private unnamed_addr constant [6 x i8] c"alice\00", align 1
@STR2 = private unnamed_addr constant [10 x i8] c"name: %s\0A\00", align 1
@STR3 = private unnamed_addr constant [9 x i8] c"age: %d\0A\00", align 1
@STR4 = private unnamed_addr constant [11 x i8] c"grade: %c\0A\00", align 1
@STR5 = private unnamed_addr constant [18 x i8] c"name: %s age: %d\0A\00", align 1
@STR6 = private unnamed_addr constant [12 x i8] c"x + y = %d\0A\00", align 1
@STR7 = private unnamed_addr constant [14 x i8] c"negative: %d\0A\00", align 1
@STR8 = private unnamed_addr constant [10 x i8] c"zero: %d\0A\00", align 1
@STR9 = private unnamed_addr constant [5 x i8] c"john\00", align 1
@STR10 = private unnamed_addr constant [4 x i8] c"doe\00", align 1
@STR11 = private unnamed_addr constant [18 x i8] c"full name: %s %s\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i32 @main() {
entry:
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 0)
  %name = alloca ptr, align 8
  store ptr @STR1, ptr %name, align 8
  %name1 = load ptr, ptr %name, align 8
  %printf2 = call i32 (ptr, i32, ...) @printf(ptr @STR2, i32 1, ptr %name1)
  %age = alloca i32, align 4
  store i32 30, ptr %age, align 4
  %age3 = load i32, ptr %age, align 4
  %printf4 = call i32 (ptr, i32, ...) @printf(ptr @STR3, i32 1, i32 %age3)
  %grade = alloca i8, align 1
  store i8 65, ptr %grade, align 1
  %grade5 = load i8, ptr %grade, align 1
  %printf6 = call i32 (ptr, i32, ...) @printf(ptr @STR4, i32 1, i8 %grade5)
  %name7 = load ptr, ptr %name, align 8
  %age8 = load i32, ptr %age, align 4
  %printf9 = call i32 (ptr, i32, ...) @printf(ptr @STR5, i32 2, ptr %name7, i32 %age8)
  %x = alloca i32, align 4
  store i32 10, ptr %x, align 4
  %y = alloca i32, align 4
  store i32 20, ptr %y, align 4
  %x10 = load i32, ptr %x, align 4
  %y11 = load i32, ptr %y, align 4
  %ADD = add i32 %x10, %y11
  %printf12 = call i32 (ptr, i32, ...) @printf(ptr @STR6, i32 1, i32 %ADD)
  %n = alloca i32, align 4
  store i32 -5, ptr %n, align 4
  %n13 = load i32, ptr %n, align 4
  %printf14 = call i32 (ptr, i32, ...) @printf(ptr @STR7, i32 1, i32 %n13)
  %printf15 = call i32 (ptr, i32, ...) @printf(ptr @STR8, i32 1, i32 0)
  %first = alloca ptr, align 8
  store ptr @STR9, ptr %first, align 8
  %last = alloca ptr, align 8
  store ptr @STR10, ptr %last, align 8
  %first16 = load ptr, ptr %first, align 8
  %last17 = load ptr, ptr %last, align 8
  %printf18 = call i32 (ptr, i32, ...) @printf(ptr @STR11, i32 2, ptr %first16, ptr %last17)
  ret i32 0
}
