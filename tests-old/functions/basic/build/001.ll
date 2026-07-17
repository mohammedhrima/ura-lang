; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpolx5950y.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpolx5950y.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [20 x i8] c"=== Entering floor \00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c" ===\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"BOSS: \00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c" appears!\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.5 = private unnamed_addr constant [14 x i8] c"The Lich King\00", align 1

define void @announce_floor(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %floor1 = load i32, i32* %floor, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str, i32 0, i32 0), i32 %floor1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @announce_boss(i8* %0) {
entry:
  %name = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %name1 = load i8*, i8** %name, align 8
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  call void @announce_floor(i32 1)
  call void @announce_floor(i32 5)
  call void @announce_boss(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}
