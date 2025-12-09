; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca ptr, align 8
  store ptr %a, ptr %b, align 8
  %ref_addr = load ptr, ptr %b, align 8
  store i32 2, ptr %ref_addr, align 4
  %a1 = load i32, ptr %a, align 4
  ret i32 %a1
}
