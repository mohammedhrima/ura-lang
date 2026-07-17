# literals / float

## index

- 001 — float literals: declare, print, zero-init, in a function
- 002 — float arithmetic + comparison, compound assignment

## 001 — float literals: declare, print, zero-init, in a function

```ura
// literals/float/001.ura - float literals: declare, print, zero-init, in a function

fn half() float:
    return 0.5

main():
    x float = 3.14
    output(x, "\n")
    output(2.71, "\n")
    output(half(), "\n")
    y float
    output(y, "\n")
    return 0
```

```out
3.140000
2.710000
0.500000
0.000000
```

```err
```

```ll

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
```

## 002 — float arithmetic + comparison, compound assignment

```ura
// literals/float/002.ura - float arithmetic + comparison, compound assignment

main():
    a float = 3.0
    b float = 2.0
    output(a + b, "\n")
    output(a - b, "\n")
    output(a * b, "\n")
    output(a / b, "\n")
    output(a > b, "\n")
    output(a == b, "\n")
    a += 1.5
    output(a, "\n")
    a *= 2.0
    output(a, "\n")
    return 0
```

```out
5.000000
1.000000
6.000000
1.500000
True
False
4.500000
9.000000
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@trap_msg = private unnamed_addr constant [160 x i8] c"runtime error: Division by zero\0A  002.ura:9:14\0A  |\0A9 |     output(a / b, \22\\n\22)\0A  |              ^\0A\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.9 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.10 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1

define i32 @main() {
entry:
  %a = alloca float, align 4
  store float 3.000000e+00, float* %a, align 4
  %b = alloca float, align 4
  store float 2.000000e+00, float* %b, align 4
  %a1 = load float, float* %a, align 4
  %b2 = load float, float* %b, align 4
  %fadd = fadd float %a1, %b2
  %f2d = fpext float %fadd to double
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %a3 = load float, float* %a, align 4
  %b4 = load float, float* %b, align 4
  %fsub = fsub float %a3, %b4
  %f2d5 = fpext float %fsub to double
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), double %f2d5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %a6 = load float, float* %a, align 4
  %b7 = load float, float* %b, align 4
  %fmul = fmul float %a6, %b7
  %f2d8 = fpext float %fmul to double
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), double %f2d8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a9 = load float, float* %a, align 4
  %b10 = load float, float* %b, align 4
  %iszero = fcmp oeq float %b10, 0.000000e+00
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %3 = call i64 @write(i32 2, i8* getelementptr inbounds ([160 x i8], [160 x i8]* @trap_msg, i32 0, i32 0), i64 159)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %fdiv = fdiv float %a9, %b10
  %f2d11 = fpext float %fdiv to double
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), double %f2d11, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %a12 = load float, float* %a, align 4
  %b13 = load float, float* %b, align 4
  %fgt = fcmp ogt float %a12, %b13
  %bool_str = select i1 %fgt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %a14 = load float, float* %a, align 4
  %b15 = load float, float* %b, align 4
  %feq = fcmp oeq float %a14, %b15
  %bool_str16 = select i1 %feq, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.9, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.10, i32 0, i32 0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i8* %bool_str16, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.11, i32 0, i32 0))
  %cur = load float, float* %a, align 4
  %fadd17 = fadd float %cur, 1.500000e+00
  store float %fadd17, float* %a, align 4
  %a18 = load float, float* %a, align 4
  %f2d19 = fpext float %a18 to double
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), double %f2d19, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %cur20 = load float, float* %a, align 4
  %fmul21 = fmul float %cur20, 2.000000e+00
  store float %fmul21, float* %a, align 4
  %a22 = load float, float* %a, align 4
  %f2d23 = fpext float %a22 to double
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.16, i32 0, i32 0), double %f2d23, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```
