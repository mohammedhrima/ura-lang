; ModuleID = '/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn'
source_filename = "/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn"

define i32 @hello(i32 %0) {
entry:
  %a = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  ret i32 %a1
}

define i32 @main() {
entry:
  %hello = call i32 @hello(<null operand!>)
  %a = alloca i32, align 4
  ret i32 0
}
