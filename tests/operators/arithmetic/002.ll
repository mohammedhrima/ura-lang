; ModuleID = 'exe.out'
source_filename = "exe.out"

@divzero_msg = private unnamed_addr constant [56 x i8] c"tests/operators/arithmetic/002.ura:4: division by zero\0A\00", align 1

define i32 @main() {
entry:
  br i1 false, label %divzero, label %cont

divzero:                                          ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([56 x i8], [56 x i8]* @divzero_msg, i32 0, i32 0), i64 55)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  ret i32 10
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
