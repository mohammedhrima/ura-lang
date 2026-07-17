; ModuleID = 'exe.out'
source_filename = "exe.out"

@str = private unnamed_addr constant [17 x i8] c"HP after hit:   \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [17 x i8] c"MP after spell: \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %mp = alloca i32, align 4
  store i32 50, i32* %mp, align 4
  %shield = alloca i32*, align 8
  store i32* %hp, i32** %shield, align 8
  %ref = load i32*, i32** %shield, align 8
  %ref1 = load i32*, i32** %shield, align 8
  %shield2 = load i32, i32* %ref1, align 4
  %sub = sub i32 %shield2, 30
  store i32 %sub, i32* %ref, align 4
  %hp3 = load i32, i32* %hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %hp3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %mana = alloca i32*, align 8
  store i32* %mp, i32** %mana, align 8
  %ref4 = load i32*, i32** %mana, align 8
  %ref5 = load i32*, i32** %mana, align 8
  %mana6 = load i32, i32* %ref5, align 4
  %sub7 = sub i32 %mana6, 20
  store i32 %sub7, i32* %ref4, align 4
  %mp8 = load i32, i32* %mp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i32 %mp8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
