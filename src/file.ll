; ModuleID = './file.w'
source_filename = "./file.w"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 0, ptr %b, align 4
  %a1 = load i32, ptr %a, align 4
  %LESS = icmp slt i32 %a1, 10
  br i1 %LESS, label %if, label %end_if

if:                                               ; preds = %entry
  store i32 1, ptr %b, align 4
  %b2 = load i32, ptr %b, align 4
  %EQUAL = icmp eq i32 %b2, 1
  br i1 %EQUAL, label %if3, label %end_if4

end_if:                                           ; preds = %end_if4, %entry
  %b5 = load i32, ptr %b, align 4
  ret i32 %b5

if3:                                              ; preds = %if
  store i32 5, ptr %b, align 4
  br label %end_if4

end_if4:                                          ; preds = %if3, %if
  br label %end_if
}
