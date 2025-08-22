; ModuleID = 'simple_if'
source_filename = "simple_if"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %load_a = load i32, ptr %a, align 4
  %cmp = icmp slt i32 %load_a, 10
  br i1 %cmp, label %start_if, label %end_if

start_if:                                         ; preds = %entry
  store i32 3, ptr %a, align 4
  br label %end_if

end_if:                                           ; preds = %start_if, %entry
  %ret = load i32, ptr %a, align 4
  ret i32 %ret
}
