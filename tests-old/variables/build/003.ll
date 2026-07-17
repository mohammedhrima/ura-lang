; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpdmkw6vfx.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpdmkw6vfx.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"name:  \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"hp:    \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.6 = private unnamed_addr constant [8 x i8] c"xp:    \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [9 x i8] c"%s%lld%s\00", align 1
@str.9 = private unnamed_addr constant [8 x i8] c"level: \00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.12 = private unnamed_addr constant [8 x i8] c"grade: \00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [7 x i8] c"%s%c%s\00", align 1
@str.15 = private unnamed_addr constant [8 x i8] c"alive: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.18 = private unnamed_addr constant [8 x i8] c"ratio: \00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %xp = alloca i64, align 8
  store i64 999999, i64* %xp, align 4
  %level = alloca i16, align 2
  store i16 12, i16* %level, align 2
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  %alive = alloca i1, align 1
  store i1 true, i1* %alive, align 1
  %ratio = alloca float, align 4
  store float 0x3FEB333340000000, float* %ratio, align 4
  %name1 = load i8*, i8** %name, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %hp2 = load i32, i32* %hp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %hp2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %xp3 = load i64, i64* %xp, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.8, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.6, i32 0, i32 0), i64 %xp3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %level4 = load i16, i16* %level, align 2
  %s2i = sext i16 %level4 to i32
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.9, i32 0, i32 0), i32 %s2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %grade5 = load i8, i8* %grade, align 1
  %c2i = sext i8 %grade5 to i32
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.12, i32 0, i32 0), i32 %c2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %alive6 = load i1, i1* %alive, align 1
  %bool_str = select i1 %alive6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.17, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.15, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %ratio7 = load float, float* %ratio, align 4
  %f2d = fpext float %ratio7 to double
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.20, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.18, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
