; ModuleID = 'calloc_example'
source_filename = "calloc_example"

@src_literal = private unnamed_addr constant [7 x i8] c"abcdef\00", align 1

declare ptr @calloc(i64, i64)

declare ptr @strcpy(ptr, ptr)

declare i32 @puts(ptr)

declare void @free(ptr)

define i32 @main() {
entry:
  %str_ptr = call ptr @calloc(i64 10, i64 1)
  %str = alloca ptr, align 8
  store ptr %str_ptr, ptr %str, align 8
  %str_val = load ptr, ptr %str, align 8
  %0 = call ptr @strcpy(ptr %str_val, ptr @src_literal)
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i_val = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %i_val, 2
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %str_val2 = load ptr, ptr %str, align 8
  %i_val2 = load i32, ptr %i, align 4
  %elem_ptr = getelementptr i8, ptr %str_val2, i32 %i_val2
  store i8 97, ptr %elem_ptr, align 1
  %i_plus_1 = add i32 %i_val2, 1
  store i32 %i_plus_1, ptr %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %str_val3 = load ptr, ptr %str, align 8
  %1 = call i32 @puts(ptr %str_val3)
  %str_val4 = load ptr, ptr %str, align 8
  call void @free(ptr %str_val4)
  ret i32 0
}
