; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpdd062pid.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpdd062pid.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [11 x i8] c"\E2\98\85 Level \00", align 1
@str.1 = private unnamed_addr constant [15 x i8] c"! HP+20 MP+10\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"Stats: HP=\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c" MP=\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.6 = private unnamed_addr constant [3 x i8] c"x=\00", align 1
@str.7 = private unnamed_addr constant [4 x i8] c" y=\00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define void @level_up(i32* %0, i32* %1, i32* %2) {
entry:
  %level = alloca i32*, align 8
  store i32* %0, i32** %level, align 8
  %hp = alloca i32*, align 8
  store i32* %1, i32** %hp, align 8
  %mp = alloca i32*, align 8
  store i32* %2, i32** %mp, align 8
  %ref = load i32*, i32** %level, align 8
  %ref1 = load i32*, i32** %level, align 8
  %level2 = load i32, i32* %ref1, align 4
  %add = add i32 %level2, 1
  store i32 %add, i32* %ref, align 4
  %ref3 = load i32*, i32** %hp, align 8
  %ref4 = load i32*, i32** %hp, align 8
  %hp5 = load i32, i32* %ref4, align 4
  %add6 = add i32 %hp5, 20
  store i32 %add6, i32* %ref3, align 4
  %ref7 = load i32*, i32** %mp, align 8
  %ref8 = load i32*, i32** %mp, align 8
  %mp9 = load i32, i32* %ref8, align 4
  %add10 = add i32 %mp9, 10
  store i32 %add10, i32* %ref7, align 4
  %ref11 = load i32*, i32** %level, align 8
  %level12 = load i32, i32* %ref11, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %level12, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @swap(i32* %0, i32* %1) {
entry:
  %a = alloca i32*, align 8
  store i32* %0, i32** %a, align 8
  %b = alloca i32*, align 8
  store i32* %1, i32** %b, align 8
  %tmp = alloca i32, align 4
  %ref = load i32*, i32** %a, align 8
  %a1 = load i32, i32* %ref, align 4
  store i32 %a1, i32* %tmp, align 4
  %ref2 = load i32*, i32** %a, align 8
  %ref3 = load i32*, i32** %b, align 8
  %b4 = load i32, i32* %ref3, align 4
  store i32 %b4, i32* %ref2, align 4
  %ref5 = load i32*, i32** %b, align 8
  %tmp6 = load i32, i32* %tmp, align 4
  store i32 %tmp6, i32* %ref5, align 4
  ret void
}

define i32 @main() {
entry:
  %lv = alloca i32, align 4
  store i32 1, i32* %lv, align 4
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %mp = alloca i32, align 4
  store i32 50, i32* %mp, align 4
  call void @level_up(i32* %lv, i32* %hp, i32* %mp)
  %hp1 = load i32, i32* %hp, align 4
  %mp2 = load i32, i32* %mp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), i32 %hp1, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i32 %mp2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %x = alloca i32, align 4
  store i32 3, i32* %x, align 4
  %y = alloca i32, align 4
  store i32 7, i32* %y, align 4
  call void @swap(i32* %x, i32* %y)
  %x3 = load i32, i32* %x, align 4
  %y4 = load i32, i32* %y, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.9, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.6, i32 0, i32 0), i32 %x3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i32 %y4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  ret i32 0
}
