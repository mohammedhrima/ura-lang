; ModuleID = '/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmprollklfb.out'
source_filename = "/var/folders/71/w9dssg_s29v9962k19rk1__00000gp/T/tmprollklfb.out"
target triple = "arm64-apple-darwin25.5.0"

@trap_msg = private unnamed_addr constant [94 x i8] c"\1B[1m\1B[0;31mruntime error: \1B[0mtests/functions/basic/012.ura:6: Call to a null function value\0A\00", align 1

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %add = add i32 %n1, %n2
  ret i32 %add
}

define i32 @apply(i32 (i32)* %0, i32 %1) {
entry:
  %f = alloca i32 (i32)*, align 8
  store i32 (i32)* %0, i32 (i32)** %f, align 8
  %x = alloca i32, align 4
  store i32 %1, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %fn = load i32 (i32)*, i32 (i32)** %f, align 8
  %isnull = icmp eq i32 (i32)* %fn, null
  br i1 %isnull, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([94 x i8], [94 x i8]* @trap_msg, i32 0, i32 0), i64 93)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %call = call i32 %fn(i32 %x1)
  ret i32 %call
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

define void @bump(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %ref = load i32*, i32** %n, align 8
  %ref1 = load i32*, i32** %n, align 8
  %n2 = load i32, i32* %ref1, align 4
  %add = add i32 %n2, 1
  store i32 %add, i32* %ref, align 4
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 20, i32* %n, align 4
  call void @bump(i32* %n)
  %n1 = load i32, i32* %n, align 4
  %call = call i32 @apply(i32 (i32)* @twice, i32 %n1)
  ret i32 %call
}
