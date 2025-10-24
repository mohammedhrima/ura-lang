; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@str_literal = private constant [5 x i8] c"abcd\00"

declare ptr @calloc(i32, i32)

declare i32 @puts(ptr)

define void @strcpy(ptr %dist, ptr %src) {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while

while:                                            ; preds = %while_bloc, %entry
  %i1 = load i32, ptr %i, align 4
  %access = getelementptr i8, ptr %src, i32 %i1
  %access_val = load i8, ptr %access, align 1
  %NOT_EQ = icmp ne i8 %access_val, 0
  br i1 %NOT_EQ, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i2 = load i32, ptr %i, align 4
  %access3 = getelementptr i8, ptr %dist, i32 %i2
  %access_val4 = load i8, ptr %access3, align 1
  %i5 = load i32, ptr %i, align 4
  %access6 = getelementptr i8, ptr %src, i32 %i5
  %access_val7 = load i8, ptr %access6, align 1
  store i8 %access_val7, ptr %access3, align 1
  %i8 = load i32, ptr %i, align 4
  %ADD = add i32 %i8, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
}

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store ptr @str_literal, ptr %str, align 8
  %tmp = alloca ptr, align 8
  %calloc = call ptr @calloc(i32 1, i32 10)
  store ptr %calloc, ptr %tmp, align 8
  %tmp1 = load ptr, ptr %tmp, align 8
  %str2 = load ptr, ptr %str, align 8
  %strcpy = call void @strcpy(ptr %tmp1, ptr %str2)
  %tmp3 = load ptr, ptr %tmp, align 8
  %puts = call i32 @puts(ptr %tmp3)
  ret i32 0
}
