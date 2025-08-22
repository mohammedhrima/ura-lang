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
  br i1 %MORE, label %start_if, label %end_if

start_if:                                         ; preds = %entry
  store i32 1, ptr %b, align 4
  br label %end_if

end_if:                                           ; preds = %start_if, %entry
  store i32 2, ptr %b, align 4
  %b2 = load i32, ptr %b, align 4
  ret i32 %b2
}
