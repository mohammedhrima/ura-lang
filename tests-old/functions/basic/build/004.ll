; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmps4rxlmcb.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmps4rxlmcb.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [214 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mCall to a null function value\0A   \1B[2mtests/functions/basic/004.ura:11:12\1B[0m\0A   \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m11 |\1B[0m     output(op(2, 3), \22\\n\22)\0A   \1B[1m\1B[34m|\1B[0m            \1B[1;31m^^\1B[0m\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@trap_msg.1 = private unnamed_addr constant [215 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mCall to a null function value\0A   \1B[2mtests/functions/basic/004.ura:14:12\1B[0m\0A   \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m14 |\1B[0m     output(op(10, 4), \22\\n\22)\0A   \1B[1m\1B[34m|\1B[0m            \1B[1;31m^^\1B[0m\0A\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %add = add i32 %a1, %b2
  ret i32 %add
}

define i32 @sub(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %sub = sub i32 %a1, %b2
  ret i32 %sub
}

define i32 @main() {
entry:
  %op = alloca i32 (i32, i32)*, align 8
  store i32 (i32, i32)* @add, i32 (i32, i32)** %op, align 8
  %fn = load i32 (i32, i32)*, i32 (i32, i32)** %op, align 8
  %isnull = icmp eq i32 (i32, i32)* %fn, null
  br i1 %isnull, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([214 x i8], [214 x i8]* @trap_msg, i32 0, i32 0), i64 213)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %call = call i32 %fn(i32 2, i32 3)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  store i32 (i32, i32)* @sub, i32 (i32, i32)** %op, align 8
  %fn1 = load i32 (i32, i32)*, i32 (i32, i32)** %op, align 8
  %isnull2 = icmp eq i32 (i32, i32)* %fn1, null
  br i1 %isnull2, label %trap3, label %cont4

trap3:                                            ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([215 x i8], [215 x i8]* @trap_msg.1, i32 0, i32 0), i64 214)
  call void @exit(i32 1)
  unreachable

cont4:                                            ; preds = %cont
  %call5 = call i32 %fn1(i32 10, i32 4)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 %call5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
