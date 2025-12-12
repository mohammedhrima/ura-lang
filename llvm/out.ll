; ModuleID = 'bounds_check_example'
source_filename = "bounds_check_example"

@STR0 = private unnamed_addr constant [374 x i8] c"\0A\1B[1;31m=================================================================\1B[0m\0A\1B[1;31mRUNTIME ERROR: Array Index Out of Bounds\1B[0m\0A\1B[1;31m=================================================================\1B[0m\0ALocation: \1B[1;33mline %d, column %d\1B[0m\0AIndex:    \1B[1;36m%d\1B[0m\0AMax size: \1B[1;36m%d\1B[0m\0A\1B[1;31m=================================================================\1B[0m\0A\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca [2 x i8], align 1
  %a1 = getelementptr [2 x i8], ptr %a, i32 0, i32 0
  call void @__ura_bounds_check(ptr %a1, i32 -1, i32 2, i32 10, i32 5)
  %access = getelementptr i8, ptr %a1, i32 -1
  store i8 97, ptr %access, align 1
  call void @__ura_bounds_check(ptr %a1, i32 0, i32 2, i32 15, i32 5)
  %access2 = getelementptr i8, ptr %a1, i32 0
  store i8 72, ptr %access2, align 1
  call void @__ura_bounds_check(ptr %a1, i32 1, i32 2, i32 20, i32 5)
  %access3 = getelementptr i8, ptr %a1, i32 1
  store i8 105, ptr %access3, align 1
  call void @__ura_bounds_check(ptr %a1, i32 2, i32 2, i32 25, i32 5)
  %access4 = getelementptr i8, ptr %a1, i32 2
  store i8 33, ptr %access4, align 1
  ret i32 0
}

declare i32 @printf(ptr, ...)

declare void @exit(i32)

define void @__ura_bounds_check(ptr %0, i32 %1, i32 %2, i32 %3, i32 %4) {
entry:
  %ge0 = icmp sge i32 %1, 0
  %ltsize = icmp slt i32 %1, %2
  %valid = and i1 %ge0, %ltsize
  br i1 %valid, label %ok, label %error

error:                                            ; preds = %entry
  %5 = call i32 (ptr, ...) @printf(ptr @STR0, i32 %3, i32 %4, i32 %1, i32 %2)
  call void @exit(i32 1)
  unreachable

ok:                                               ; preds = %entry
  ret void
}
