; ModuleID = '/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn'
source_filename = "/Users/hrimamohammed/Desktop/Personal/pandu-lang/src/file.pn"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  br label %while

while:                                            ; preds = %while_bloc, %entry
  %a1 = load i32, ptr %a, align 4
  %LESS = icmp slt i32 %a1, 10
  br i1 %LESS, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %a2 = load i32, ptr %a, align 4
  %ADD = add i32 %a2, 1
  store i32 %ADD, ptr %a, align 4
  br label %while

end_while:                                        ; preds = %while
  %a3 = load i32, ptr %a, align 4
  ret i32 %a3
}
