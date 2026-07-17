; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpe_s9qwzk.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpe_s9qwzk.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [244 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mreference 'm' used before it was bound - assign 'm = ref <target>' first\0A  \1B[2mtests/errors/runtime/005.ura:5:12\1B[0m\0A  \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m5 |\1B[0m     output(m?, \22\\n\22)\0A  \1B[1m\1B[34m|\1B[0m            \1B[1;31m^\1B[0m\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %m = alloca i32*, align 8
  store i32* null, i32** %m, align 8
  %ref = load i32*, i32** %m, align 8
  %unbound = icmp eq i32* %ref, null
  br i1 %unbound, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([244 x i8], [244 x i8]* @trap_msg, i32 0, i32 0), i64 243)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %m1 = load i32, i32* %ref, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %m1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
