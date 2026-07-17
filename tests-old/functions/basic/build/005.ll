; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpxm48xc50.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpxm48xc50.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [204 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mCall to a null function value\0A   \1B[2mtests/functions/basic/005.ura:10:12\1B[0m\0A   \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m10 |\1B[0m     return cb(x)\0A   \1B[1m\1B[34m|\1B[0m            \1B[1;31m^^\1B[0m\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @double_it(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 2
  ret i32 %mul
}

define i32 @triple_it(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 3
  ret i32 %mul
}

define i32 @apply(i32 (i32)* %0, i32 %1) {
entry:
  %cb = alloca i32 (i32)*, align 8
  store i32 (i32)* %0, i32 (i32)** %cb, align 8
  %x = alloca i32, align 4
  store i32 %1, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %fn = load i32 (i32)*, i32 (i32)** %cb, align 8
  %isnull = icmp eq i32 (i32)* %fn, null
  br i1 %isnull, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([204 x i8], [204 x i8]* @trap_msg, i32 0, i32 0), i64 203)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %call = call i32 %fn(i32 %x1)
  ret i32 %call
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

define i32 @main() {
entry:
  %call = call i32 @apply(i32 (i32)* @double_it, i32 21)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %call1 = call i32 @apply(i32 (i32)* @triple_it, i32 7)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %call1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
