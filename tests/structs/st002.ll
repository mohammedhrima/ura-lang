; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

@STR0 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@STR1 = private unnamed_addr constant [4 x i8] c"car\00", align 1
@STR2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(ptr, i32, ...)

define i32 @main() {
entry:
  %user = alloca { ptr, i32, { { ptr, i32 }, { ptr } } }, align 8
  %name = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 0
  store ptr @STR0, ptr %name, align 8
  %age = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 1
  store i32 123, ptr %age, align 4
  %car = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 2
  store ptr @STR1, ptr %car, align 8
  %car1 = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 2
  %user2 = load { { ptr, i32 }, { ptr } }, ptr %car1, align 8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR2, i32 1, { { ptr, i32 }, { ptr } } %user2)
  %age3 = getelementptr { ptr, i32, { { ptr, i32 }, { ptr } } }, ptr %user, i32 0, i32 1
  %user4 = load i32, ptr %age3, align 4
  ret i32 %user4
}
