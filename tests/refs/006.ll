; ModuleID = 'exe.out'
source_filename = "exe.out"

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %score = alloca i32, align 4
  store i32 0, i32* %score, align 4
  %r = alloca i32*, align 8
  store i32* %score, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %cur = load i32, i32* %ref, align 4
  %add = add i32 %cur, 100
  store i32 %add, i32* %ref, align 4
  %ref1 = load i32*, i32** %r, align 8
  %cur2 = load i32, i32* %ref1, align 4
  %add3 = add i32 %cur2, 50
  store i32 %add3, i32* %ref1, align 4
  %ref4 = load i32*, i32** %r, align 8
  %cur5 = load i32, i32* %ref4, align 4
  %mul = mul i32 %cur5, 2
  store i32 %mul, i32* %ref4, align 4
  %ref6 = load i32*, i32** %r, align 8
  %cur7 = load i32, i32* %ref6, align 4
  %sub = sub i32 %cur7, 30
  store i32 %sub, i32* %ref6, align 4
  %score8 = load i32, i32* %score, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %score8, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
