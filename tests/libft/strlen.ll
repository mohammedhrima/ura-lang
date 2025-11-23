; ModuleID = 'moduleName'
source_filename = "moduleName"

@STR = private constant [4 x i8] c"abc\00"

define i32 @strlen(ptr %str) {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while

while:                                            ; preds = %while_bloc, %entry
  %i1 = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %str, i32 %i1
  %str2 = load i8, ptr %ACCESS, align 1
  %NOT_EQ = icmp ne i8 %str2, 0
  br i1 %NOT_EQ, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i3 = load i32, ptr %i, align 4
  %ADD = add i32 %i3, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
  %i4 = load i32, ptr %i, align 4
  ret i32 %i4
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strlen = call i32 @strlen(ptr @STR)
  store i32 %strlen, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}
