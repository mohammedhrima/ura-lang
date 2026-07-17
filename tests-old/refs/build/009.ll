; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpeqc46tmg.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpeqc46tmg.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 42, i32* %x, align 4
  %m = alloca i32*, align 8
  store i32* %x, i32** %m, align 8
  %ref = load i32*, i32** %m, align 8
  %m1 = load i32, i32* %ref, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %m1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load i32*, i32** %m, align 8
  store i32 7, i32* %ref2, align 4
  %x3 = load i32, i32* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %x3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
