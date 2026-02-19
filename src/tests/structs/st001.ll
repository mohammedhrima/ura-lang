; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

@STR0 = private unnamed_addr constant [4 x i8] c"abc\00", align 1

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %user = alloca { i32, ptr }, align 8
  %age = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0
  store i32 1, ptr %age, align 4
  %name = getelementptr { i32, ptr }, ptr %user, i32 0, i32 1
  store ptr @STR0, ptr %name, align 8
  %name1 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 1
  %user2 = load ptr, ptr %name1, align 8
  %puts = call i32 @puts(ptr %user2)
  %age3 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0
  %user4 = load i32, ptr %age3, align 4
  ret i32 %user4
}
