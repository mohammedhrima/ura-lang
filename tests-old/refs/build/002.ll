; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp_dpywgzg.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp_dpywgzg.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %a = alloca i32*, align 8
  store i32* %hp, i32** %a, align 8
  %b = alloca i32*, align 8
  store i32* %hp, i32** %b, align 8
  %ref = load i32*, i32** %a, align 8
  store i32 80, i32* %ref, align 4
  %ref1 = load i32*, i32** %b, align 8
  %ref2 = load i32*, i32** %b, align 8
  %b3 = load i32, i32* %ref2, align 4
  %sub = sub i32 %b3, 10
  store i32 %sub, i32* %ref1, align 4
  %hp4 = load i32, i32* %hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %hp4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
