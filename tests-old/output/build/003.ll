; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpm5hilemv.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpm5hilemv.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"Name:  \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"HP:    \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.6 = private unnamed_addr constant [8 x i8] c"Grade: \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%s%c%s\00", align 1
@str.9 = private unnamed_addr constant [8 x i8] c"Alive: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.12 = private unnamed_addr constant [8 x i8] c"Speed: \00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  %alive = alloca i1, align 1
  store i1 true, i1* %alive, align 1
  %speed = alloca float, align 4
  store float 1.750000e+00, float* %speed, align 4
  %name1 = load i8*, i8** %name, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %hp2 = load i32, i32* %hp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %hp2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %grade3 = load i8, i8* %grade, align 1
  %c2i = sext i8 %grade3 to i32
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.6, i32 0, i32 0), i32 %c2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %alive4 = load i1, i1* %alive, align 1
  %bool_str = select i1 %alive4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.9, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %speed5 = load float, float* %speed, align 4
  %f2d = fpext float %speed5 to double
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.12, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
