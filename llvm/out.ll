; ModuleID = 'module'
source_filename = "module"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.cond.0

while.cond.0:                                     ; preds = %while.body.1, %entry
  %i1 = load i32, ptr %i, align 4
  %less = icmp slt i32 %i1, 5
  br i1 %less, label %while.body.1, label %while.end.2

while.body.1:                                     ; preds = %while.cond.0
  %i2 = load i32, ptr %i, align 4
  %add = add i32 %i2, 1
  store i32 %add, ptr %i, align 4
  br label %while.cond.0

while.end.2:                                      ; preds = %while.cond.0
  %i3 = load i32, ptr %i, align 4
  ret i32 %i3
}
