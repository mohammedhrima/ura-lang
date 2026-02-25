; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

@STR0 = private unnamed_addr constant [9 x i8] c"mohammed\00", align 1
@STR1 = private unnamed_addr constant [17 x i8] c"hello %s has %d\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

declare ptr @calloc(i32, i32)

declare void @free(ptr)

declare ptr @strcpy(ptr, ptr)

define i32 @main() {
entry:
  %user = alloca { i32, ptr }, align 8
  %age = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0
  store i32 28, ptr %age, align 4
  %name = getelementptr { i32, ptr }, ptr %user, i32 0, i32 1
  %calloc = call ptr @calloc(i32 1, i32 10)
  store ptr %calloc, ptr %name, align 8
  %name1 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 1
  %user2 = load ptr, ptr %name1, align 8
  %strcpy = call ptr @strcpy(ptr %user2, ptr @STR0)
  %name3 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 1
  %user4 = load ptr, ptr %name3, align 8
  %age5 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0
  %user6 = load i32, ptr %age5, align 4
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 2, ptr %user4, i32 %user6)
  %name7 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 1
  %user8 = load ptr, ptr %name7, align 8
  call void @free(ptr %user8)
  %age9 = getelementptr { i32, ptr }, ptr %user, i32 0, i32 0
  %user10 = load i32, ptr %age9, align 4
  ret i32 %user10
}
