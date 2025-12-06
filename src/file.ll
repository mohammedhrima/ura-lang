; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 97, ptr %c, align 1
  %n = alloca i32, align 4
  %c1 = load i8, ptr %c, align 1
  store i8 %c1, ptr %n, align 1
  %n2 = load i32, ptr %n, align 4
  %LESS = icmp slt i32 %n2, 0
  %n3 = load i32, ptr %n, align 4
  %MORE = icmp sgt i32 %n3, 127
  %OR = or i1 %LESS, %MORE
  br i1 %OR, label %if, label %end_if

if:                                               ; preds = %entry
  ret i32 -1
  br label %end_if

end_if:                                           ; preds = %if, %entry
  %n4 = load i32, ptr %n, align 4
  ret i32 %n4
}
