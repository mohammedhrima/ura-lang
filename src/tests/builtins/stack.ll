; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [12 x i8] c"Hello world\00", align 1
@STR1 = private unnamed_addr constant [12 x i8] c"Hello world\00", align 1

declare i32 @write(i32, ptr, i32)

declare void @strcpy(ptr, ptr)

declare i32 @strlen(ptr)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  %stack = alloca [20 x i8], align 1
  %stack1 = getelementptr [20 x i8], ptr %stack, i32 0, i32 0
  store ptr %stack1, ptr %str, align 8
  %str2 = load ptr, ptr %str, align 8
  call void @strcpy(ptr %str2, ptr @STR0)
  %str3 = load ptr, ptr %str, align 8
  %strlen = call i32 @strlen(ptr @STR1)
  %write = call i32 @write(i32 1, ptr %str3, i32 %strlen)
  ret i32 0
}
