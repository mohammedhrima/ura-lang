; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@STR0 = private unnamed_addr constant [6 x i8] c"CHARS\00", align 1
@STR1 = private unnamed_addr constant [6 x i8] c"<%s>\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store ptr @STR0, ptr %str, align 8
  %str1 = load ptr, ptr %str, align 8
  %printf = call i32 (ptr, ...) @printf(ptr @STR1, ptr %str1)
  ret i32 0
}
