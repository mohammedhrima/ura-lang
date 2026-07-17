; ModuleID = 'exe.out'
source_filename = "exe.out"

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %shield = alloca i32, align 4
  store i32 10, i32* %shield, align 4
  %mp = alloca i32, align 4
  store i32 80, i32* %mp, align 4
  %s = alloca i32*, align 8
  store i32* %shield, i32** %s, align 8
  %m = alloca i32*, align 8
  store i32* %mp, i32** %m, align 8
  %ref = load i32*, i32** %s, align 8
  %cur = load i32, i32* %ref, align 4
  %mul = mul i32 %cur, 2
  store i32 %mul, i32* %ref, align 4
  %ref1 = load i32*, i32** %m, align 8
  %cur2 = load i32, i32* %ref1, align 4
  %sub = sub i32 %cur2, 25
  store i32 %sub, i32* %ref1, align 4
  %shield3 = load i32, i32* %shield, align 4
  %mp4 = load i32, i32* %mp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %shield3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %mp4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
