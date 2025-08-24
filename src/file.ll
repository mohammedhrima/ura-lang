; ModuleID = './file.w'
source_filename = "./file.w"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  br label %while

while:                                            ; preds = %end_if, %if, %entry
  %a1 = load i32, ptr %a, align 4
  %LESS = icmp slt i32 %a1, 10
  br i1 %LESS, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %a2 = load i32, ptr %a, align 4
  %EQUAL = icmp eq i32 %a2, 9
  br i1 %EQUAL, label %if, label %else

end_while:                                        ; preds = %while
  %a4 = load i32, ptr %a, align 4
  ret i32 %a4

if:                                               ; preds = %while_bloc
  br label %while
  br label %end_if

else:                                             ; preds = %while_bloc
  %a3 = load i32, ptr %a, align 4
  %ADD = add i32 %a3, 1
  store i32 %ADD, ptr %a, align 4
  br label %end_if

end_if:                                           ; preds = %else, %if
  br label %while
}
