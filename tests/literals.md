# literals

## index

- 001 — bool literals: declare, zero-init, bool function, reassign
- 002 — char literals: declare, print, escapes, in a function
- 003 — float literals: declare, print, zero-init, in a function
- 004 — float arithmetic + comparison, compound assignment
- 005 — `double` declaration, arithmetic and compound assignment
- 006 — a float literal takes the type of its target
- 007 — casts between double, float and the integer types
- 008 — a return that does not match the function's type

## 001 — bool literals: declare, zero-init, bool function, reassign

```ura
// literals/001.ura - bool literals: declare, zero-init, bool function, reassign

fn flag() bool:
    return True

main():
    b bool = flag()
    c bool = False
    d bool
    c = True
    return 9
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn flag() : bool
└─ return
   └─ bool True

fn main() : i32
├─ = : bool
│  ├─ b : bool
│  └─ call flag : bool
├─ = : bool
│  ├─ c : bool
│  └─ bool False
├─ d : bool
├─ = : bool
│  ├─ c : bool
│  └─ bool True
└─ return
   └─ int 9
```

```out
```

```err
exit: 9
```

```ll

define i1 @flag() {
entry:
  ret i1 true
}

define i32 @main() {
entry:
  %b = alloca i1, align 1
  %call = call i1 @flag()
  store i1 %call, i1* %b, align 1
  %c = alloca i1, align 1
  store i1 false, i1* %c, align 1
  %d = alloca i1, align 1
  store i1 false, i1* %d, align 1
  store i1 true, i1* %c, align 1
  ret i32 9
}
```

## 002 — char literals: declare, print, escapes, in a function

```ura
// literals/002.ura - char literals: declare, print, escapes, in a function

fn grade() char:
    return 'A'

main():
    c char = 'z'
    output(c, "\n")
    output('!', "\n")
    output(grade(), "\n")
    output('\n')
    return 0
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn grade() : char
└─ return
   └─ char 'A'

fn main() : i32
├─ = : char
│  ├─ c : char
│  └─ char 'z'
├─ output : void
│  ├─ c : char
│  └─ chars "\n"
├─ output : void
│  ├─ char '!'
│  └─ chars "\n"
├─ output : void
│  ├─ call grade : char
│  └─ chars "\n"
├─ output : void
│  └─ char '\n'
└─ return
   └─ int 0
```

```out
z
!
A
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%c%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%c%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%c%s\00", align 1
@fmt.5 = private unnamed_addr constant [3 x i8] c"%c\00", align 1

define i8 @grade() {
entry:
  ret i8 65
}

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 122, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %c2i = sext i8 %c1 to i32
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %c2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 33, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call = call i8 @grade()
  %c2i2 = sext i8 %call to i32
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %c2i2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.5, i32 0, i32 0), i32 10)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — float literals: declare, print, zero-init, in a function

```ura
// literals/003.ura - float literals: declare, print, zero-init, in a function

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

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn half() : f32
└─ return
   └─ float 0.5

fn main() : i32
├─ = : f32
│  ├─ x : f32
│  └─ float 3.14
├─ output : void
│  ├─ x : f32
│  └─ chars "\n"
├─ output : void
│  ├─ float 2.71
│  └─ chars "\n"
├─ output : void
│  ├─ call half : f32
│  └─ chars "\n"
├─ y : f32
├─ output : void
│  ├─ y : f32
│  └─ chars "\n"
└─ return
   └─ int 0
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

## 004 — float arithmetic + comparison, compound assignment

```ura
// literals/004.ura - float arithmetic + comparison, compound assignment

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

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : f32
│  ├─ a : f32
│  └─ float 3
├─ = : f32
│  ├─ b : f32
│  └─ float 2
├─ output : void
│  ├─ + : f32
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ chars "\n"
├─ output : void
│  ├─ - : f32
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ chars "\n"
├─ output : void
│  ├─ * : f32
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ chars "\n"
├─ output : void
│  ├─ / : f32
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ chars "\n"
├─ output : void
│  ├─ > : bool
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ chars "\n"
├─ output : void
│  ├─ == : bool
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ chars "\n"
├─ += : f32
│  ├─ a : f32
│  └─ float 1.5
├─ output : void
│  ├─ a : f32
│  └─ chars "\n"
├─ *= : f32
│  ├─ a : f32
│  └─ float 2
├─ output : void
│  ├─ a : f32
│  └─ chars "\n"
└─ return
   └─ int 0
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
@trap_msg = private unnamed_addr constant [160 x i8] c"runtime error: Division by zero\0A  004.ura:9:14\0A  |\0A9 |     output(a / b, \22\\n\22)\0A  |              ^\0A\00", align 1
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

## 005 — `double` declaration, arithmetic and compound assignment

```ura
// literals/005.ura - the double type

main():
    d double = 3.141592653589793
    e double = 2.0
    output(d, " ", e, "\n")
    output(d + e, " ", d - e, " ", d * e, " ", d / e, "\n")
    output(d > e, " ", d == d, " ", d != e, "\n")

    c double = 10.0
    c += 2.5
    c -= 0.5
    c *= 2.0
    c /= 4.0
    output(c, "\n")

    // zero-init
    z double
    output(z, "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : f64
│  ├─ d : f64
│  └─ float 3.14159
├─ = : f64
│  ├─ e : f64
│  └─ float 2
├─ output : void
│  ├─ d : f64
│  ├─ chars " "
│  ├─ e : f64
│  └─ chars "\n"
├─ output : void
│  ├─ + : f64
│  │  ├─ d : f64
│  │  └─ e : f64
│  ├─ chars " "
│  ├─ - : f64
│  │  ├─ d : f64
│  │  └─ e : f64
│  ├─ chars " "
│  ├─ * : f64
│  │  ├─ d : f64
│  │  └─ e : f64
│  ├─ chars " "
│  ├─ / : f64
│  │  ├─ d : f64
│  │  └─ e : f64
│  └─ chars "\n"
├─ output : void
│  ├─ > : bool
│  │  ├─ d : f64
│  │  └─ e : f64
│  ├─ chars " "
│  ├─ == : bool
│  │  ├─ d : f64
│  │  └─ d : f64
│  ├─ chars " "
│  ├─ != : bool
│  │  ├─ d : f64
│  │  └─ e : f64
│  └─ chars "\n"
├─ = : f64
│  ├─ c : f64
│  └─ float 10
├─ += : f64
│  ├─ c : f64
│  └─ float 2.5
├─ -= : f64
│  ├─ c : f64
│  └─ float 0.5
├─ *= : f64
│  ├─ c : f64
│  └─ float 2
├─ /= : f64
│  ├─ c : f64
│  └─ float 4
├─ output : void
│  ├─ c : f64
│  └─ chars "\n"
├─ z : f64
└─ output : void
   ├─ z : f64
   └─ chars "\n"
```

```out
3.141593 2.000000
5.141593 1.141593 6.283185 1.570796
True True True
6.000000
0.000000
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%f%s%f%s\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c" \00", align 1
@trap_msg = private unnamed_addr constant [232 x i8] c"runtime error: Division by zero\0A  005.ura:7:50\0A  |\0A7 |     output(d + e, \22 \22, d - e, \22 \22, d * e, \22 \22, d / e, \22\\n\22)\0A  |                                                  ^\0A\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [17 x i8] c"%f%s%f%s%f%s%f%s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.11 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.12 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [13 x i8] c"%s%s%s%s%s%s\00", align 1
@trap_msg.15 = private unnamed_addr constant [147 x i8] c"runtime error: Division by zero\0A   005.ura:14:7\0A   |\0A14 |     c /= 4.0\0A   |       ^^\0A\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.19 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1

define i32 @main() {
entry:
  %d = alloca double, align 8
  store double 0x400921FB54442D18, double* %d, align 8
  %e = alloca double, align 8
  store double 2.000000e+00, double* %e, align 8
  %d1 = load double, double* %d, align 8
  %e2 = load double, double* %e, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), double %d1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), double %e2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %d3 = load double, double* %d, align 8
  %e4 = load double, double* %e, align 8
  %fadd = fadd double %d3, %e4
  %d5 = load double, double* %d, align 8
  %e6 = load double, double* %e, align 8
  %fsub = fsub double %d5, %e6
  %d7 = load double, double* %d, align 8
  %e8 = load double, double* %e, align 8
  %fmul = fmul double %d7, %e8
  %d9 = load double, double* %d, align 8
  %e10 = load double, double* %e, align 8
  %iszero = fcmp oeq double %e10, 0.000000e+00
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([232 x i8], [232 x i8]* @trap_msg, i32 0, i32 0), i64 231)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %fdiv = fdiv double %d9, %e10
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.6, i32 0, i32 0), double %fadd, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), double %fsub, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), double %fmul, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), double %fdiv, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %d11 = load double, double* %d, align 8
  %e12 = load double, double* %e, align 8
  %fgt = fcmp ogt double %d11, %e12
  %bool_str = select i1 %fgt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %d13 = load double, double* %d, align 8
  %d14 = load double, double* %d, align 8
  %feq = fcmp oeq double %d13, %d14
  %bool_str15 = select i1 %feq, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.8, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.9, i32 0, i32 0)
  %d16 = load double, double* %d, align 8
  %e17 = load double, double* %e, align 8
  %fne = fcmp une double %d16, %e17
  %bool_str18 = select i1 %fne, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.11, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.12, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.14, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0), i8* %bool_str15, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0), i8* %bool_str18, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %c = alloca double, align 8
  store double 1.000000e+01, double* %c, align 8
  %cur = load double, double* %c, align 8
  %fadd19 = fadd double %cur, 2.500000e+00
  store double %fadd19, double* %c, align 8
  %cur20 = load double, double* %c, align 8
  %fsub21 = fsub double %cur20, 5.000000e-01
  store double %fsub21, double* %c, align 8
  %cur22 = load double, double* %c, align 8
  %fmul23 = fmul double %cur22, 2.000000e+00
  store double %fmul23, double* %c, align 8
  %cur24 = load double, double* %c, align 8
  br i1 false, label %trap25, label %cont26

trap25:                                           ; preds = %cont
  %4 = call i64 @write(i32 2, i8* getelementptr inbounds ([147 x i8], [147 x i8]* @trap_msg.15, i32 0, i32 0), i64 146)
  call void @exit(i32 1)
  unreachable

cont26:                                           ; preds = %cont
  %fdiv27 = fdiv double %cur24, 4.000000e+00
  store double %fdiv27, double* %c, align 8
  %c28 = load double, double* %c, align 8
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.17, i32 0, i32 0), double %c28, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %z = alloca double, align 8
  store double 0.000000e+00, double* %z, align 8
  %z29 = load double, double* %z, align 8
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.19, i32 0, i32 0), double %z29, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 006 — a float literal takes the type of its target

```ura
// literals/006.ura - float literal inference

fn half() double:
    return 0.5

fn scale(x double) double:
    return x * 2.0

main():
    // same digits, two different types
    d double = 3.141592653589793
    f float  = 3.141592653589793
    // the f32 rounding is lossy, so widening it back does NOT equal d
    output((f as double) == d, "\n")

    // an expression built only from literals adopts too, and unary minus
    // desugars to 0 - x, so its synthesised zero must be a float
    output(scale(-2.5), " ", scale(1.0 + 0.5), "\n")

    // inference at a return
    output(half(), "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn half() : f64
└─ return
   └─ float 0.5

fn scale(x : f64) : f64
└─ return
   └─ * : f64
      ├─ x : f64
      └─ float 2

fn main() : i32
├─ = : f64
│  ├─ d : f64
│  └─ float 3.14159
├─ = : f32
│  ├─ f : f32
│  └─ float 3.14159
├─ output : void
│  ├─ == : bool
│  │  ├─ cast : f64
│  │  │  └─ f : f32
│  │  └─ d : f64
│  └─ chars "\n"
├─ output : void
│  ├─ call scale : f64
│  │  └─ - : f64
│  │     ├─ float 0
│  │     └─ float 2.5
│  ├─ chars " "
│  ├─ call scale : f64
│  │  └─ + : f64
│  │     ├─ float 1
│  │     └─ float 0.5
│  └─ chars "\n"
└─ output : void
   ├─ call half : f64
   └─ chars "\n"
```

```out
False
-5.000000 3.000000
0.500000
```

```err
```

```ll

@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [9 x i8] c"%f%s%f%s\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%f%s\00", align 1

define double @half() {
entry:
  ret double 5.000000e-01
}

define double @scale(double %0) {
entry:
  %x = alloca double, align 8
  store double %0, double* %x, align 8
  %x1 = load double, double* %x, align 8
  %fmul = fmul double %x1, 2.000000e+00
  ret double %fmul
}

define i32 @main() {
entry:
  %d = alloca double, align 8
  store double 0x400921FB54442D18, double* %d, align 8
  %f = alloca float, align 4
  store float 0x400921FB60000000, float* %f, align 4
  %f1 = load float, float* %f, align 4
  %cast = fpext float %f1 to double
  %d2 = load double, double* %d, align 8
  %feq = fcmp oeq double %cast, %d2
  %bool_str = select i1 %feq, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %call = call double @scale(double -2.500000e+00)
  %call3 = call double @scale(double 1.500000e+00)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.3, i32 0, i32 0), double %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), double %call3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %call4 = call double @half()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), double %call4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — casts between double, float and the integer types

```ura
// literals/007.ura - double casts

main():
    d double = 3.75
    f float  = 1.5
    output("d->float ", d as float, "  f->double ", f as double, "\n")
    output("d->int ", d as int, "  d->long ", d as long, "\n")
    output("d->short ", d as short, "  d->char ", d as char, "\n")
    output("int->d ", 7 as double, "  bool->d ", True as double, "\n")
    output("round trip ", d as int as double, "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : f64
│  ├─ d : f64
│  └─ float 3.75
├─ = : f32
│  ├─ f : f32
│  └─ float 1.5
├─ output : void
│  ├─ chars "d->float "
│  ├─ cast : f32
│  │  └─ d : f64
│  ├─ chars "  f->double "
│  ├─ cast : f64
│  │  └─ f : f32
│  └─ chars "\n"
├─ output : void
│  ├─ chars "d->int "
│  ├─ cast : i32
│  │  └─ d : f64
│  ├─ chars "  d->long "
│  ├─ cast : i64
│  │  └─ d : f64
│  └─ chars "\n"
├─ output : void
│  ├─ chars "d->short "
│  ├─ cast : i16
│  │  └─ d : f64
│  ├─ chars "  d->char "
│  ├─ cast : char
│  │  └─ d : f64
│  └─ chars "\n"
├─ output : void
│  ├─ chars "int->d "
│  ├─ cast : f64
│  │  └─ int 7
│  ├─ chars "  bool->d "
│  ├─ cast : f64
│  │  └─ bool True
│  └─ chars "\n"
└─ output : void
   ├─ chars "round trip "
   ├─ cast : f64
   │  └─ cast : i32
   │     └─ d : f64
   └─ chars "\n"
```

```out
d->float 3.750000  f->double 1.500000
d->int 3  d->long 3
d->short 3  d->char 
int->d 7.000000  bool->d 1.000000
round trip 3.000000
```

```err
```

```ll

@str = private unnamed_addr constant [10 x i8] c"d->float \00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"  f->double \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%f%s%f%s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"d->int \00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"  d->long \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [13 x i8] c"%s%d%s%lld%s\00", align 1
@str.7 = private unnamed_addr constant [10 x i8] c"d->short \00", align 1
@str.8 = private unnamed_addr constant [11 x i8] c"  d->char \00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [11 x i8] c"%s%d%s%c%s\00", align 1
@str.11 = private unnamed_addr constant [8 x i8] c"int->d \00", align 1
@str.12 = private unnamed_addr constant [11 x i8] c"  bool->d \00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [11 x i8] c"%s%f%s%f%s\00", align 1
@str.15 = private unnamed_addr constant [12 x i8] c"round trip \00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1

define i32 @main() {
entry:
  %d = alloca double, align 8
  store double 3.750000e+00, double* %d, align 8
  %f = alloca float, align 4
  store float 1.500000e+00, float* %f, align 4
  %d1 = load double, double* %d, align 8
  %cast = fptrunc double %d1 to float
  %f2d = fpext float %cast to double
  %f2 = load float, float* %f, align 4
  %cast3 = fpext float %f2 to double
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), double %cast3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %d4 = load double, double* %d, align 8
  %cast5 = fptosi double %d4 to i32
  %d6 = load double, double* %d, align 8
  %cast7 = fptosi double %d6 to i64
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %cast5, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i64 %cast7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %d8 = load double, double* %d, align 8
  %cast9 = fptosi double %d8 to i16
  %s2i = sext i16 %cast9 to i32
  %d10 = load double, double* %d, align 8
  %cast11 = fptosi double %d10 to i8
  %c2i = sext i8 %cast11 to i32
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.7, i32 0, i32 0), i32 %s2i, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.8, i32 0, i32 0), i32 %c2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.11, i32 0, i32 0), double 7.000000e+00, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.12, i32 0, i32 0), double 1.000000e+00, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %d12 = load double, double* %d, align 8
  %cast13 = fptosi double %d12 to i32
  %cast14 = sitofp i32 %cast13 to double
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.17, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.15, i32 0, i32 0), double %cast14, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 008 — a return that does not match the function's type

```ura
// literals/008.ura - return type mismatch

fn f() int:
    return 1.5

main():
    output(f(), "\n")
```

```tree
```

```out
```

```err
error: 'f' returns i32, but this returns f32
  008.ura:4:5
  |
4 |     return 1.5
  |     ^^^^^^
```

```ll
```
