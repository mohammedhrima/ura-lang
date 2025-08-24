; ModuleID = './file.w'
source_filename = "./file.w"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 0, ptr %b, align 4
  %a1 = load i32, ptr %a, align 4
  %MORE = icmp sgt i32 %a1, 10
  br <null operand!>, label %if, label %elif

if:                                               ; preds = %entry
  store i32 1, ptr %b, align 4
  br label %end_if

elif:                                             ; preds = %entry
  %a2 = load i32, ptr %a, align 4
  %MORE3 = icmp sgt i32 %a2, 1
  br i1 %MORE3, label %bloc, label %end_if

bloc:                                             ; preds = %elif

end_if:                                           ; preds = %end_if, %elif, %if
  br label %end_if
  %b4 = load i32, ptr %b, align 4
  ret i32 %b4
}
