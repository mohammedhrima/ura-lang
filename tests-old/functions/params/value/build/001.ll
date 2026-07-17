; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpyvr2s3_y.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpyvr2s3_y.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @take_damage(i32 %0, i32 %1) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %atk = alloca i32, align 4
  store i32 %1, i32* %atk, align 4
  %hp1 = load i32, i32* %hp, align 4
  %atk2 = load i32, i32* %atk, align 4
  %sub = sub i32 %hp1, %atk2
  ret i32 %sub
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %call = call i32 @take_damage(i32 100, i32 18)
  store i32 %call, i32* %result, align 4
  %result1 = load i32, i32* %result, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %result1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
