# operators / arithmetic

## index

- 001 — all arithmetic ops in dungeon combat context
- 002 — arithmetic with precedence (2*3 + 20/4 - 1)
- 003 — reassign an existing variable (2*10 + 3)
- 004 — modulo

## 001 — all arithmetic ops in dungeon combat context

```ura
// operators/arithmetic/001.ura - all arithmetic ops in dungeon combat context

fn base_atk() int:
    return 10

fn clamp(t float) float:
    if t < 0.05:
        return 0.05
    return t

main():
    atk     int = 25
    def     int = 8
    kills   int = 3
    total   int = 100
    count   int = 4
    xp_cap  int = 70

    // var + var
    output("atk + def:       ", atk + def,       "\n")
    // var - var
    output("atk - def:       ", atk - def,       "\n")
    // var * var
    output("kills * 50:      ", kills * 50,      "\n")
    // var / var
    output("total / count:   ", total / count,   "\n")
    // var % var
    output("atk % xp_cap:    ", atk % xp_cap,    "\n")
    // val + val
    output("10 + 5:          ", 10 + 5,           "\n")
    // fn + var
    output("base_atk + atk:  ", base_atk() + atk,"\n")
    // chained with precedence
    output("2 + 3 * 4:       ", 2 + 3 * 4,       "\n")
    output("(2 + 3) * 4:     ", (2 + 3) * 4,     "\n")

    a float = 1.5
    b float = 0.3

    output("add:  ", a + b, "\n")  // 1.8
    output("sub:  ", a - b, "\n")  // 1.2
    output("mul:  ", a * b, "\n")  // 0.45
    output("div:  ", a / b, "\n")  // 5.0

    output("lt:   ", a < b,  "\n")  // 0 (false)
    output("gt:   ", a > b,  "\n")  // 1 (true)
    output("lte:  ", a <= b, "\n")  // 0
    output("gte:  ", a >= b, "\n")  // 1

    t float = 0.8 - 0.28
    output("clamp(0.52): ", clamp(t), "\n")  // 0.52
    output("clamp(0.01): ", clamp(0.01), "\n")  // 0.05
```

```tree
fn base_atk() : int
└─ return
   └─ int 10

fn clamp(t : float) : float
├─ if
│  ├─ condition < : bool
│  │  ├─ t : float
│  │  └─ float 0.05
│  └─ return
│     └─ float 0.05
└─ return
   └─ t : float

fn main() : int
├─ = : int
│  ├─ atk : int
│  └─ int 25
├─ = : int
│  ├─ def : int
│  └─ int 8
├─ = : int
│  ├─ kills : int
│  └─ int 3
├─ = : int
│  ├─ total : int
│  └─ int 100
├─ = : int
│  ├─ count : int
│  └─ int 4
├─ = : int
│  ├─ xp_cap : int
│  └─ int 70
├─ output : void
│  ├─ chars "atk + def:       "
│  ├─ + : int
│  │  ├─ atk : int
│  │  └─ def : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "atk - def:       "
│  ├─ - : int
│  │  ├─ atk : int
│  │  └─ def : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "kills * 50:      "
│  ├─ * : int
│  │  ├─ kills : int
│  │  └─ int 50
│  └─ chars "\n"
├─ output : void
│  ├─ chars "total / count:   "
│  ├─ / : int
│  │  ├─ total : int
│  │  └─ count : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "atk % xp_cap:    "
│  ├─ % : int
│  │  ├─ atk : int
│  │  └─ xp_cap : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "10 + 5:          "
│  ├─ + : int
│  │  ├─ int 10
│  │  └─ int 5
│  └─ chars "\n"
├─ output : void
│  ├─ chars "base_atk + atk:  "
│  ├─ + : int
│  │  ├─ call base_atk : int
│  │  └─ atk : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "2 + 3 * 4:       "
│  ├─ + : int
│  │  ├─ int 2
│  │  └─ * : int
│  │     ├─ int 3
│  │     └─ int 4
│  └─ chars "\n"
├─ output : void
│  ├─ chars "(2 + 3) * 4:     "
│  ├─ * : int
│  │  ├─ + : int
│  │  │  ├─ int 2
│  │  │  └─ int 3
│  │  └─ int 4
│  └─ chars "\n"
├─ = : float
│  ├─ a : float
│  └─ float 1.5
├─ = : float
│  ├─ b : float
│  └─ float 0.3
├─ output : void
│  ├─ chars "add:  "
│  ├─ + : float
│  │  ├─ a : float
│  │  └─ b : float
│  └─ chars "\n"
├─ output : void
│  ├─ chars "sub:  "
│  ├─ - : float
│  │  ├─ a : float
│  │  └─ b : float
│  └─ chars "\n"
├─ output : void
│  ├─ chars "mul:  "
│  ├─ * : float
│  │  ├─ a : float
│  │  └─ b : float
│  └─ chars "\n"
├─ output : void
│  ├─ chars "div:  "
│  ├─ / : float
│  │  ├─ a : float
│  │  └─ b : float
│  └─ chars "\n"
├─ output : void
│  ├─ chars "lt:   "
│  ├─ < : bool
│  │  ├─ a : float
│  │  └─ b : float
│  └─ chars "\n"
├─ output : void
│  ├─ chars "gt:   "
│  ├─ > : bool
│  │  ├─ a : float
│  │  └─ b : float
│  └─ chars "\n"
├─ output : void
│  ├─ chars "lte:  "
│  ├─ <= : bool
│  │  ├─ a : float
│  │  └─ b : float
│  └─ chars "\n"
├─ output : void
│  ├─ chars "gte:  "
│  ├─ >= : bool
│  │  ├─ a : float
│  │  └─ b : float
│  └─ chars "\n"
├─ = : float
│  ├─ t : float
│  └─ - : float
│     ├─ float 0.8
│     └─ float 0.28
├─ output : void
│  ├─ chars "clamp(0.52): "
│  ├─ call clamp : float
│  │  └─ t : float
│  └─ chars "\n"
└─ output : void
   ├─ chars "clamp(0.01): "
   ├─ call clamp : float
   │  └─ float 0.01
   └─ chars "\n"
```

```out
atk + def:       33
atk - def:       17
kills * 50:      150
total / count:   25
atk % xp_cap:    25
10 + 5:          15
base_atk + atk:  35
2 + 3 * 4:       14
(2 + 3) * 4:     20
add:  1.800000
sub:  1.200000
mul:  0.450000
div:  5.000000
lt:   False
gt:   True
lte:  False
gte:  True
clamp(0.52): 0.520000
clamp(0.01): 0.050000
```

```err
```

```ll

@str = private unnamed_addr constant [18 x i8] c"atk + def:       \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [18 x i8] c"atk - def:       \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [18 x i8] c"kills * 50:      \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.8 = private unnamed_addr constant [18 x i8] c"total / count:   \00", align 1
@trap_msg = private unnamed_addr constant [221 x i8] c"runtime error: Division by zero\0A   001.ura:26:39\0A   |\0A26 |     output(\22total / count:   \22, total / count,   \22\\n\22)\0A   |                                       ^\0A\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.11 = private unnamed_addr constant [18 x i8] c"atk % xp_cap:    \00", align 1
@trap_msg.12 = private unnamed_addr constant [217 x i8] c"runtime error: Modulo by zero\0A   001.ura:28:37\0A   |\0A28 |     output(\22atk % xp_cap:    \22, atk % xp_cap,    \22\\n\22)\0A   |                                     ^\0A\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.15 = private unnamed_addr constant [18 x i8] c"10 + 5:          \00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.18 = private unnamed_addr constant [18 x i8] c"base_atk + atk:  \00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.21 = private unnamed_addr constant [18 x i8] c"2 + 3 * 4:       \00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.23 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.24 = private unnamed_addr constant [18 x i8] c"(2 + 3) * 4:     \00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.27 = private unnamed_addr constant [7 x i8] c"add:  \00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.30 = private unnamed_addr constant [7 x i8] c"sub:  \00", align 1
@str.31 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.32 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.33 = private unnamed_addr constant [7 x i8] c"mul:  \00", align 1
@str.34 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.35 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.36 = private unnamed_addr constant [7 x i8] c"div:  \00", align 1
@trap_msg.37 = private unnamed_addr constant [193 x i8] c"runtime error: Division by zero\0A   001.ura:43:24\0A   |\0A43 |     output(\22div:  \22, a / b, \22\\n\22)  // 5.0\0A   |                        ^\0A\00", align 1
@str.38 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.39 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.40 = private unnamed_addr constant [7 x i8] c"lt:   \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.41 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.42 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.43 = private unnamed_addr constant [7 x i8] c"gt:   \00", align 1
@true_str.44 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.45 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.46 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.47 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.48 = private unnamed_addr constant [7 x i8] c"lte:  \00", align 1
@true_str.49 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.50 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.51 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.52 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.53 = private unnamed_addr constant [7 x i8] c"gte:  \00", align 1
@true_str.54 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.55 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.56 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.57 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.58 = private unnamed_addr constant [14 x i8] c"clamp(0.52): \00", align 1
@str.59 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.60 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.61 = private unnamed_addr constant [14 x i8] c"clamp(0.01): \00", align 1
@str.62 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.63 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1

define i32 @base_atk() {
entry:
  ret i32 10
}

define float @clamp(float %0) {
entry:
  %t = alloca float, align 4
  store float %0, float* %t, align 4
  %t1 = load float, float* %t, align 4
  %flt = fcmp olt float %t1, 0x3FA99999A0000000
  br i1 %flt, label %then, label %endif

endif:                                            ; preds = %entry
  %t2 = load float, float* %t, align 4
  ret float %t2

then:                                             ; preds = %entry
  ret float 0x3FA99999A0000000
}

define i32 @main() {
entry:
  %atk = alloca i32, align 4
  store i32 25, i32* %atk, align 4
  %def = alloca i32, align 4
  store i32 8, i32* %def, align 4
  %kills = alloca i32, align 4
  store i32 3, i32* %kills, align 4
  %total = alloca i32, align 4
  store i32 100, i32* %total, align 4
  %count = alloca i32, align 4
  store i32 4, i32* %count, align 4
  %xp_cap = alloca i32, align 4
  store i32 70, i32* %xp_cap, align 4
  %atk1 = load i32, i32* %atk, align 4
  %def2 = load i32, i32* %def, align 4
  %add = add i32 %atk1, %def2
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %add, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %atk3 = load i32, i32* %atk, align 4
  %def4 = load i32, i32* %def, align 4
  %sub = sub i32 %atk3, %def4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.2, i32 0, i32 0), i32 %sub, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %kills5 = load i32, i32* %kills, align 4
  %mul = mul i32 %kills5, 50
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.5, i32 0, i32 0), i32 %mul, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %total6 = load i32, i32* %total, align 4
  %count7 = load i32, i32* %count, align 4
  %iszero = icmp eq i32 %count7, 0
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %3 = call i64 @write(i32 2, i8* getelementptr inbounds ([221 x i8], [221 x i8]* @trap_msg, i32 0, i32 0), i64 220)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %total6, %count7
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.8, i32 0, i32 0), i32 %div, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %atk8 = load i32, i32* %atk, align 4
  %xp_cap9 = load i32, i32* %xp_cap, align 4
  %iszero10 = icmp eq i32 %xp_cap9, 0
  br i1 %iszero10, label %trap11, label %cont12

trap11:                                           ; preds = %cont
  %5 = call i64 @write(i32 2, i8* getelementptr inbounds ([217 x i8], [217 x i8]* @trap_msg.12, i32 0, i32 0), i64 216)
  call void @exit(i32 1)
  unreachable

cont12:                                           ; preds = %cont
  %mod = srem i32 %atk8, %xp_cap9
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.11, i32 0, i32 0), i32 %mod, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.17, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.15, i32 0, i32 0), i32 15, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %call = call i32 @base_atk()
  %atk13 = load i32, i32* %atk, align 4
  %add14 = add i32 %call, %atk13
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.20, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.18, i32 0, i32 0), i32 %add14, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.23, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.21, i32 0, i32 0), i32 14, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.26, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.24, i32 0, i32 0), i32 20, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %a = alloca float, align 4
  store float 1.500000e+00, float* %a, align 4
  %b = alloca float, align 4
  store float 0x3FD3333340000000, float* %b, align 4
  %a15 = load float, float* %a, align 4
  %b16 = load float, float* %b, align 4
  %fadd = fadd float %a15, %b16
  %f2d = fpext float %fadd to double
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.29, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.27, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %a17 = load float, float* %a, align 4
  %b18 = load float, float* %b, align 4
  %fsub = fsub float %a17, %b18
  %f2d19 = fpext float %fsub to double
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.32, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.30, i32 0, i32 0), double %f2d19, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.31, i32 0, i32 0))
  %a20 = load float, float* %a, align 4
  %b21 = load float, float* %b, align 4
  %fmul = fmul float %a20, %b21
  %f2d22 = fpext float %fmul to double
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.35, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.33, i32 0, i32 0), double %f2d22, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.34, i32 0, i32 0))
  %a23 = load float, float* %a, align 4
  %b24 = load float, float* %b, align 4
  %iszero25 = fcmp oeq float %b24, 0.000000e+00
  br i1 %iszero25, label %trap26, label %cont27

trap26:                                           ; preds = %cont12
  %14 = call i64 @write(i32 2, i8* getelementptr inbounds ([193 x i8], [193 x i8]* @trap_msg.37, i32 0, i32 0), i64 192)
  call void @exit(i32 1)
  unreachable

cont27:                                           ; preds = %cont12
  %fdiv = fdiv float %a23, %b24
  %f2d28 = fpext float %fdiv to double
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.39, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.36, i32 0, i32 0), double %f2d28, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.38, i32 0, i32 0))
  %a29 = load float, float* %a, align 4
  %b30 = load float, float* %b, align 4
  %flt = fcmp olt float %a29, %b30
  %bool_str = select i1 %flt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.42, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.40, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.41, i32 0, i32 0))
  %a31 = load float, float* %a, align 4
  %b32 = load float, float* %b, align 4
  %fgt = fcmp ogt float %a31, %b32
  %bool_str33 = select i1 %fgt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.44, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.45, i32 0, i32 0)
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.47, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.43, i32 0, i32 0), i8* %bool_str33, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
  %a34 = load float, float* %a, align 4
  %b35 = load float, float* %b, align 4
  %fle = fcmp ole float %a34, %b35
  %bool_str36 = select i1 %fle, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.49, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.50, i32 0, i32 0)
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.52, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.48, i32 0, i32 0), i8* %bool_str36, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.51, i32 0, i32 0))
  %a37 = load float, float* %a, align 4
  %b38 = load float, float* %b, align 4
  %fge = fcmp oge float %a37, %b38
  %bool_str39 = select i1 %fge, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.54, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.55, i32 0, i32 0)
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.57, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.53, i32 0, i32 0), i8* %bool_str39, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.56, i32 0, i32 0))
  %t = alloca float, align 4
  store float 0x3FE0A3D700000000, float* %t, align 4
  %t40 = load float, float* %t, align 4
  %call41 = call float @clamp(float %t40)
  %f2d42 = fpext float %call41 to double
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.60, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.58, i32 0, i32 0), double %f2d42, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.59, i32 0, i32 0))
  %call43 = call float @clamp(float 0x3F847AE140000000)
  %f2d44 = fpext float %call43 to double
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.63, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.61, i32 0, i32 0), double %f2d44, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.62, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 002 — arithmetic with precedence (2*3 + 20/4 - 1)

```ura
// operators/arithmetic/002.ura - arithmetic with precedence (2*3 + 20/4 - 1)

main():
    return 2 * 3 + 20 / 4 - 1
```

```tree
fn main() : int
└─ return
   └─ - : int
      ├─ + : int
      │  ├─ * : int
      │  │  ├─ int 2
      │  │  └─ int 3
      │  └─ / : int
      │     ├─ int 20
      │     └─ int 4
      └─ int 1
```

```out
```

```err
exit: 10
```

```ll

@trap_msg = private unnamed_addr constant [175 x i8] c"runtime error: Division by zero\0A  002.ura:4:23\0A  |\0A4 |     return 2 * 3 + 20 / 4 - 1\0A  |                       ^\0A\00", align 1

define i32 @main() {
entry:
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([175 x i8], [175 x i8]* @trap_msg, i32 0, i32 0), i64 174)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  ret i32 10
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 003 — reassign an existing variable (2*10 + 3)

```ura
// operators/arithmetic/003.ura - reassign an existing variable (2*10 + 3)

main():
    a int = 2
    a = a * 10 + 3
    return a
```

```tree
fn main() : int
├─ = : int
│  ├─ a : int
│  └─ int 2
├─ = : int
│  ├─ a : int
│  └─ + : int
│     ├─ * : int
│     │  ├─ a : int
│     │  └─ int 10
│     └─ int 3
└─ return
   └─ a : int
```

```out
```

```err
exit: 23
```

```ll

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 2, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %mul = mul i32 %a1, 10
  %add = add i32 %mul, 3
  store i32 %add, i32* %a, align 4
  %a2 = load i32, i32* %a, align 4
  ret i32 %a2
}
```

## 004 — modulo

```ura
// operators/arithmetic/004.ura - modulo

main():
    return 17 % 5
```

```tree
fn main() : int
└─ return
   └─ % : int
      ├─ int 17
      └─ int 5
```

```out
```

```err
exit: 2
```

```ll

@trap_msg = private unnamed_addr constant [153 x i8] c"runtime error: Modulo by zero\0A  004.ura:4:15\0A  |\0A4 |     return 17 % 5\0A  |               ^\0A\00", align 1

define i32 @main() {
entry:
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  ret i32 2
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```
