; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpkyifg9m9.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpkyifg9m9.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [15 x i8] c"After combat:\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"HP: \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"MP: \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.7 = private unnamed_addr constant [5 x i8] c"XP: \00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %mp = alloca i32, align 4
  store i32 50, i32* %mp, align 4
  %xp = alloca i32, align 4
  store i32 0, i32* %xp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %sub = sub i32 %hp1, 12
  store i32 %sub, i32* %hp, align 4
  %cur = load i32, i32* %mp, align 4
  %sub2 = sub i32 %cur, 10
  store i32 %sub2, i32* %mp, align 4
  %cur3 = load i32, i32* %xp, align 4
  %add = add i32 %cur3, 50
  store i32 %add, i32* %xp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0))
  %hp4 = load i32, i32* %hp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i32 %hp4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %mp5 = load i32, i32* %mp, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i32 %mp5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %xp6 = load i32, i32* %xp, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.9, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0), i32 %xp6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
