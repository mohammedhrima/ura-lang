; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp33q_d6w6.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp33q_d6w6.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [8 x i8] c"dungeon\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"hi\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"length: %d\0A\00", align 1

declare i32 @strlen(i8*)

declare i32 @write(i32, i8*, i32)

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %call = call i32 @strlen(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0))
  store i32 %call, i32* %n, align 4
  %call1 = call i32 @write(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 3)
  %n2 = load i32, i32* %n, align 4
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %n2)
  ret i32 0
}
