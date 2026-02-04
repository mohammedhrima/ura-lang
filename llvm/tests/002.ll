; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 3, ptr %i, align 4
  %i1 = load i32, ptr %i, align 4
  %less = icmp slt i32 %i1, 1
  br i1 %less, label %if.then.1, label %if.else.2

if.end.0:                                         ; preds = %if.else.4, %if.then.3, %if.then.1
  %i9 = load i32, ptr %i, align 4
  ret i32 %i9

if.then.1:                                        ; preds = %entry
  %i2 = load i32, ptr %i, align 4
  %add = add i32 %i2, 2
  store i32 %add, ptr %i, align 4
  br label %if.end.0

if.else.2:                                        ; preds = %entry
  %i3 = load i32, ptr %i, align 4
  %less4 = icmp slt i32 %i3, 3
  br i1 %less4, label %if.then.3, label %if.else.4

if.then.3:                                        ; preds = %if.else.2
  %i5 = load i32, ptr %i, align 4
  %add6 = add i32 %i5, 4
  store i32 %add6, ptr %i, align 4
  br label %if.end.0

if.else.4:                                        ; preds = %if.else.2
  %i7 = load i32, ptr %i, align 4
  %add8 = add i32 %i7, 3
  store i32 %add8, ptr %i, align 4
  br label %if.end.0
}
