; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpsdcash_q.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpsdcash_q.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [214 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mDivision by zero\0A  \1B[2mtests/operators/arithmetic/002.ura:4:23\1B[0m\0A  \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m4 |\1B[0m     return 2 * 3 + 20 / 4 - 1\0A  \1B[1m\1B[34m|\1B[0m                       \1B[1;31m^\1B[0m\0A\00", align 1

define i32 @main() {
entry:
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([214 x i8], [214 x i8]* @trap_msg, i32 0, i32 0), i64 213)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  ret i32 10
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
