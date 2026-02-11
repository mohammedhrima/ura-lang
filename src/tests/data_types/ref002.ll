; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 13, ptr %a, align 4
  %b = alloca ptr, align 8
  store ptr %a, ptr %b, align 8
  %c = alloca i32, align 4
  %ref_ptr = load ptr, ptr %b, align 8
  %deref = load i32, ptr %ref_ptr, align 4
  store i32 %deref, ptr %c, align 4
  %c1 = load i32, ptr %c, align 4
  ret i32 %c1
}
