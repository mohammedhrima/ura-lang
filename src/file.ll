; ModuleID = '/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn'
source_filename = "/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn"

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store [4 x i8] c"abc\00", ptr %str, align 1
  %puts = call i32 @puts(ptr %str)
  ret i32 0
}
