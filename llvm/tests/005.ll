; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

define i1 @isdigit(i8 %0) {
entry:
  %param = alloca i8, align 1
  store i8 %0, ptr %param, align 1
  %c = load i8, ptr %param, align 1
  %more_equal = icmp sge i8 %c, 48
  %c1 = load i8, ptr %param, align 1
  %less_equal = icmp sle i8 %c1, 57
  %and = and i1 %more_equal, %less_equal
  ret i1 %and
}

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 49, ptr %c, align 1
  %c1 = load i8, ptr %c, align 1
  %isdigit = call i1 @main.isdigit.0(i8 %c1)
  %c2 = load i8, ptr %c, align 1
  %isdigit3 = call i1 @main.isdigit.0(i8 %c2)
  br i1 %isdigit3, label %if.then.1, label %if.else.2

if.end.0:                                         ; preds = %if.else.2
  ret i32 12

if.then.1:                                        ; preds = %entry
  ret i32 11

if.else.2:                                        ; preds = %entry
  br label %if.end.0
}

define i1 @main.isdigit.0(i8 %0) {
entry:
  %param = alloca i8, align 1
  store i8 %0, ptr %param, align 1
  %c = load i8, ptr %param, align 1
  %more_equal = icmp sge i8 %c, 48
  %c1 = load i8, ptr %param, align 1
  %less_equal = icmp sle i8 %c1, 57
  %and = and i1 %more_equal, %less_equal
  ret i1 %and
}
