; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpc77r53gv.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmpc77r53gv.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [192 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mDivision by zero\0A  \1B[2mtests/errors/runtime/004.ura:6:17\1B[0m\0A  \1B[1m\1B[34m|\1B[0m\0A\1B[1m\1B[34m6 |\1B[0m     c float = a / b\0A  \1B[1m\1B[34m|\1B[0m                 \1B[1;31m^\1B[0m\0A\00", align 1

define i32 @main() {
entry:
  %b = alloca float, align 4
  store float 0.000000e+00, float* %b, align 4
  %a = alloca float, align 4
  store float 1.000000e+01, float* %a, align 4
  %c = alloca float, align 4
  %a1 = load float, float* %a, align 4
  %b2 = load float, float* %b, align 4
  %iszero = fcmp oeq float %b2, 0.000000e+00
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([192 x i8], [192 x i8]* @trap_msg, i32 0, i32 0), i64 191)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %fdiv = fdiv float %a1, %b2
  store float %fdiv, float* %c, align 4
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
