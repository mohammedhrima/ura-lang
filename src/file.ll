; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@STR0 = private unnamed_addr constant [4 x i8] c"abc\00", align 1

define i32 @strlen(ptr %str) {
entry:
  %str1 = alloca ptr, align 8
  store ptr %str, ptr %str1, align 8
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while

while:                                            ; preds = %while_bloc, %entry
  %str2 = load ptr, ptr %str1, align 8
  %idx = load i32, ptr %i, align 4
  %ACCESS = getelementptr i8, ptr %str2, i32 %idx
  %str3 = load i8, ptr %ACCESS, align 1
  %NOT_EQ = icmp ne i8 %str3, 0
  br i1 %NOT_EQ, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i4 = load i32, ptr %i, align 4
  %ADD = add i32 %i4, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
  %i5 = load i32, ptr %i, align 4
  ret i32 %i5
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strlen = call i32 @strlen(ptr @STR0)
  store i32 %strlen, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}
