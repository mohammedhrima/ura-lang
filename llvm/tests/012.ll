; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [19 x i8] c"Before allocation\0A\00", align 1
@STR1 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
@STR2 = private unnamed_addr constant [11 x i8] c"<%s> <%c>\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

declare i32 @putchar(i8)

declare void @strcpy(ptr, ptr)

define i32 @main() {
entry:
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR0, i32 0)
  %str = alloca ptr, align 8
  %stack = alloca i8, i32 12, align 1
  call void @strcpy(ptr %stack, ptr @STR1)
  %c = alloca i8, align 1
  %ACCESS = getelementptr i8, ptr %stack, i32 1
  %ACCESS1 = load i8, ptr %ACCESS, align 1
  store i8 %ACCESS1, ptr %c, align 1
  %c2 = load i8, ptr %c, align 1
  %printf3 = call i32 (ptr, i32, ...) @printf(ptr @STR2, i32 2, ptr %stack, i8 %c2)
  ret i32 0
}
