; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpkenpvkf8.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpkenpvkf8.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [15 x i8] c"Damage dealt: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [15 x i8] c"XP gained:    \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [15 x i8] c"Gold split:   \00", align 1
@trap_msg = private unnamed_addr constant [225 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mDivision by zero\0A  \1B[2mtests/output/002.ura:6:34\1B[0m\0A  \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m6 |\1B[0m     output(\22Gold split:   \22, 120 / 4, \22\\n\22)\0A  \1B[1m\1B[34m|\1B[0m                                  \1B[1;31m^\1B[0m\0A\00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.8 = private unnamed_addr constant [15 x i8] c"Bonus:        \00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.2, i32 0, i32 0), i32 150, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([225 x i8], [225 x i8]* @trap_msg, i32 0, i32 0), i64 224)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.5, i32 0, i32 0), i32 30, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.8, i32 0, i32 0), i32 37, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
