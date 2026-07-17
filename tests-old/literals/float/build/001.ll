; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp958l10t4.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmp958l10t4.out"
target triple = "arm64-apple-darwin25.5.0"

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1

define float @half() {
entry:
  ret float 5.000000e-01
}

define i32 @main() {
entry:
  %x = alloca float, align 4
  store float 0x40091EB860000000, float* %x, align 4
  %x1 = load float, float* %x, align 4
  %f2d = fpext float %x1 to double
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), double 0x4005AE1480000000, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call = call float @half()
  %f2d2 = fpext float %call to double
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), double %f2d2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %y = alloca float, align 4
  store float 0.000000e+00, float* %y, align 4
  %y3 = load float, float* %y, align 4
  %f2d4 = fpext float %y3 to double
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), double %f2d4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
