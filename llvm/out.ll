; ModuleID = 'while_loop'
source_filename = "while_loop"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %loop_cond

loop_cond:                                        ; preds = %loop_body, %entry
  %i_val = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %i_val, 5
  br i1 %cmp, label %loop_body, label %end

loop_body:                                        ; preds = %loop_cond
  %i_loaded = load i32, ptr %i, align 4
  %i_inc = add i32 %i_loaded, 1
  store i32 %i_inc, ptr %i, align 4
  br label %loop_cond

end:                                              ; preds = %loop_cond
  %ret = load i32, ptr %i, align 4
  ret i32 %ret
}
