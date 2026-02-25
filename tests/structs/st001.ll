; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

@STR0 = private unnamed_addr constant [4 x i8] c"abc\00", align 1

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %user = alloca { i32, ptr }, align 8
  %user.age = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0
  store i32 1, ptr %user.age, align 4
  %user.name = getelementptr { i32, ptr }, ptr %user, i32 0, i32 1
  store ptr @STR0, ptr %user.name, align 8
  %user.name1 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 1
  %DOT = load ptr, ptr %user.name1, align 8
  %puts = call i32 @puts(ptr %DOT)
  %user.age2 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0
  %DOT3 = load i32, ptr %user.age2, align 4
  ret i32 %DOT3
}
