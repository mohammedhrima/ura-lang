; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpc1v7t2_g.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpc1v7t2_g.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [183 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mDivision by zero\0A  \1B[2mtests/refs/011.ura:6:14\1B[0m\0A  \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m6 |\1B[0m     output(r / 6, \22\\n\22)\0A  \1B[1m\1B[34m|\1B[0m              \1B[1;31m^\1B[0m\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@trap_msg.1 = private unnamed_addr constant [181 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mModulo by zero\0A  \1B[2mtests/refs/011.ura:7:14\1B[0m\0A  \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m7 |\1B[0m     output(r % 6, \22\\n\22)\0A  \1B[1m\1B[34m|\1B[0m              \1B[1;31m^\1B[0m\0A\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.13 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.14 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.15 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.19 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.21 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.23 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.24 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.25 = private unnamed_addr constant [7 x i8] c"%lld%s\00", align 1
@true_str.26 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.27 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.30 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.31 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.32 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.33 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.34 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.35 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.36 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.37 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 20, i32* %a, align 4
  %r = alloca i32*, align 8
  store i32* %a, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %r1 = load i32, i32* %ref, align 4
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([183 x i8], [183 x i8]* @trap_msg, i32 0, i32 0), i64 182)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %r1, 6
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %div, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load i32*, i32** %r, align 8
  %r3 = load i32, i32* %ref2, align 4
  br i1 false, label %trap4, label %cont5

trap4:                                            ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([181 x i8], [181 x i8]* @trap_msg.1, i32 0, i32 0), i64 180)
  call void @exit(i32 1)
  unreachable

cont5:                                            ; preds = %cont
  %mod = srem i32 %r3, 6
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 %mod, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %ref6 = load i32*, i32** %r, align 8
  %r7 = load i32, i32* %ref6, align 4
  %ne = icmp ne i32 %r7, 20
  %bool_str = select i1 %ne, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %ref8 = load i32*, i32** %r, align 8
  %r9 = load i32, i32* %ref8, align 4
  %lt = icmp slt i32 %r9, 25
  %bool_str10 = select i1 %lt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.6, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.7, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.9, i32 0, i32 0), i8* %bool_str10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  %ref11 = load i32*, i32** %r, align 8
  %r12 = load i32, i32* %ref11, align 4
  %le = icmp sle i32 %r12, 20
  %bool_str13 = select i1 %le, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.10, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.11, i32 0, i32 0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.13, i32 0, i32 0), i8* %bool_str13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.12, i32 0, i32 0))
  %ref14 = load i32*, i32** %r, align 8
  %r15 = load i32, i32* %ref14, align 4
  %ge = icmp sge i32 %r15, 21
  %bool_str16 = select i1 %ge, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.14, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.15, i32 0, i32 0)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.17, i32 0, i32 0), i8* %bool_str16, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %ref17 = load i32*, i32** %r, align 8
  %r18 = load i32, i32* %ref17, align 4
  %bor = or i32 %r18, 3
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.19, i32 0, i32 0), i32 %bor, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %ref19 = load i32*, i32** %r, align 8
  %r20 = load i32, i32* %ref19, align 4
  %bxor = xor i32 %r20, 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.21, i32 0, i32 0), i32 %bxor, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0))
  %ref21 = load i32*, i32** %r, align 8
  %r22 = load i32, i32* %ref21, align 4
  %shr = ashr i32 %r22, 2
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.23, i32 0, i32 0), i32 %shr, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  %ref23 = load i32*, i32** %r, align 8
  %r24 = load i32, i32* %ref23, align 4
  %cast = sext i32 %r24 to i64
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.25, i32 0, i32 0), i64 %cast, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.24, i32 0, i32 0))
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  %b = alloca i1*, align 8
  store i1* %flag, i1** %b, align 8
  %ref25 = load i1*, i1** %b, align 8
  %b26 = load i1, i1* %ref25, align 1
  %and = and i1 %b26, false
  %bool_str27 = select i1 %and, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.26, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.27, i32 0, i32 0)
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.29, i32 0, i32 0), i8* %bool_str27, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %ref28 = load i1*, i1** %b, align 8
  %b29 = load i1, i1* %ref28, align 1
  %or = or i1 %b29, false
  %bool_str30 = select i1 %or, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.30, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.31, i32 0, i32 0)
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.33, i32 0, i32 0), i8* %bool_str30, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.32, i32 0, i32 0))
  %ref31 = load i1*, i1** %b, align 8
  %b32 = load i1, i1* %ref31, align 1
  %not = xor i1 %b32, true
  %bool_str33 = select i1 %not, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.34, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.35, i32 0, i32 0)
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.37, i32 0, i32 0), i8* %bool_str33, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.36, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
