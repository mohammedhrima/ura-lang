; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp5x22cola.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp5x22cola.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [198 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mCall to a null function value\0A  \1B[2mtests/functions/basic/010.ura:7:12\1B[0m\0A  \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m7 |\1B[0m     return f(21)\0A  \1B[1m\1B[34m|\1B[0m            \1B[1;31m^\1B[0m\0A\00", align 1

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %add = add i32 %n1, %n2
  ret i32 %add
}

define i32 @main() {
entry:
  %f = alloca i32 (i32)*, align 8
  store i32 (i32)* @twice, i32 (i32)** %f, align 8
  %fn = load i32 (i32)*, i32 (i32)** %f, align 8
  %isnull = icmp eq i32 (i32)* %fn, null
  br i1 %isnull, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([198 x i8], [198 x i8]* @trap_msg, i32 0, i32 0), i64 197)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %call = call i32 %fn(i32 21)
  ret i32 %call
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
