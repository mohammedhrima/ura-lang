; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

define i32 @main() {
entry:
  %a = alloca i1, align 1
  store i1 true, ptr %a, align 1
  %b = alloca i1, align 1
  %a1 = load i1, ptr %a, align 1
  %NOT = xor i1 %a1, true
  store i1 %NOT, ptr %b, align 1
  ret i32 0
}
