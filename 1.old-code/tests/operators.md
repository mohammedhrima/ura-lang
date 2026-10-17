# operators

## index

- 001 — all arithmetic ops in dungeon combat context
- 002 — arithmetic with precedence (2*3 + 20/4 - 1)
- 003 — reassign an existing variable (2*10 + 3)
- 004 — modulo
- 005 — bitwise & | ^ << >>
- 006 — all comparison ops on hero/enemy stats
- 007 — comparisons produce bools (with precedence)
- 008 — compound assignment += -= *= /= %=
- 009 — logical ops: &&, ||, !, and, or, not
- 010 — and / or with comparisons and precedence
- 011 — unary not / minus / bitwise-not
- 012 — compound bitwise assignment
- 013 — `operator` outside a struct definition
- 014 — `operator` missing operator symbol
- 015 — adding two structs without an `operator +` overload
- 016 — type mismatch between operands
- 017 — a comparison result is a bool, not an i32
- 018 — bitwise operator on f32 operands is rejected
- 019 — divide by zero prints file:line and exits 1
- 020 — f32 division by zero traps (not inf)

## 001 — all arithmetic ops in dungeon combat context

```ura
// operators/001.ura - all arithmetic ops in dungeon combat context

fn base_atk() i32:
    return 10

fn clamp(t f32) f32:
    if t < 0.05:
        return 0.05
    return t

main():
    atk     i32 = 25
    def     i32 = 8
    kills   i32 = 3
    total   i32 = 100
    count   i32 = 4
    xp_cap  i32 = 70

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

    a f32 = 1.5
    b f32 = 0.3

    output("add:  ", a + b, "\n")  // 1.8
    output("sub:  ", a - b, "\n")  // 1.2
    output("mul:  ", a * b, "\n")  // 0.45
    output("div:  ", a / b, "\n")  // 5.0

    output("lt:   ", a < b,  "\n")  // 0 (false)
    output("gt:   ", a > b,  "\n")  // 1 (true)
    output("lte:  ", a <= b, "\n")  // 0
    output("gte:  ", a >= b, "\n")  // 1

    t f32 = 0.8 - 0.28
    output("clamp(0.52): ", clamp(t), "\n")  // 0.52
    output("clamp(0.01): ", clamp(0.01), "\n")  // 0.05
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn base_atk() : i32
└─ return
   └─ int 10

fn clamp(t : f32) : f32
├─ if
│  ├─ condition < : bool
│  │  ├─ t : f32
│  │  └─ float 0.05
│  └─ return
│     └─ float 0.05
└─ return
   └─ t : f32

fn main() : i32
├─ = : i32
│  ├─ atk : i32
│  └─ int 25
├─ = : i32
│  ├─ def : i32
│  └─ int 8
├─ = : i32
│  ├─ kills : i32
│  └─ int 3
├─ = : i32
│  ├─ total : i32
│  └─ int 100
├─ = : i32
│  ├─ count : i32
│  └─ int 4
├─ = : i32
│  ├─ xp_cap : i32
│  └─ int 70
├─ output : void
│  ├─ char[] "atk + def:       "
│  ├─ + : i32
│  │  ├─ atk : i32
│  │  └─ def : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "atk - def:       "
│  ├─ - : i32
│  │  ├─ atk : i32
│  │  └─ def : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "kills * 50:      "
│  ├─ * : i32
│  │  ├─ kills : i32
│  │  └─ int 50
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "total / count:   "
│  ├─ / : i32
│  │  ├─ total : i32
│  │  └─ count : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "atk % xp_cap:    "
│  ├─ % : i32
│  │  ├─ atk : i32
│  │  └─ xp_cap : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "10 + 5:          "
│  ├─ + : i32
│  │  ├─ int 10
│  │  └─ int 5
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "base_atk + atk:  "
│  ├─ + : i32
│  │  ├─ call base_atk : i32
│  │  └─ atk : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "2 + 3 * 4:       "
│  ├─ + : i32
│  │  ├─ int 2
│  │  └─ * : i32
│  │     ├─ int 3
│  │     └─ int 4
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "(2 + 3) * 4:     "
│  ├─ * : i32
│  │  ├─ + : i32
│  │  │  ├─ int 2
│  │  │  └─ int 3
│  │  └─ int 4
│  └─ char[] "\n"
├─ = : f32
│  ├─ a : f32
│  └─ float 1.5
├─ = : f32
│  ├─ b : f32
│  └─ float 0.3
├─ output : void
│  ├─ char[] "add:  "
│  ├─ + : f32
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "sub:  "
│  ├─ - : f32
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "mul:  "
│  ├─ * : f32
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "div:  "
│  ├─ / : f32
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "lt:   "
│  ├─ < : bool
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "gt:   "
│  ├─ > : bool
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "lte:  "
│  ├─ <= : bool
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "gte:  "
│  ├─ >= : bool
│  │  ├─ a : f32
│  │  └─ b : f32
│  └─ char[] "\n"
├─ = : f32
│  ├─ t : f32
│  └─ - : f32
│     ├─ float 0.8
│     └─ float 0.28
├─ output : void
│  ├─ char[] "clamp(0.52): "
│  ├─ call clamp : f32
│  │  └─ t : f32
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "clamp(0.01): "
   ├─ call clamp : f32
   │  └─ float 0.01
   └─ char[] "\n"
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

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [18 x i8] c"atk + def:       \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [18 x i8] c"atk - def:       \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [18 x i8] c"kills * 50:      \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.8 = private unnamed_addr constant [18 x i8] c"total / count:   \00", align 1
@trap_msg = private unnamed_addr constant [221 x i8] c"runtime error: Division by zero\0A   001.ura:26:39\0A   |\0A26 |     output(\22total / count:   \22, total / count,   \22\\n\22)\0A   |                                       ^\0A\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.11 = private unnamed_addr constant [18 x i8] c"atk % xp_cap:    \00", align 1
@trap_msg.12 = private unnamed_addr constant [217 x i8] c"runtime error: Modulo by zero\0A   001.ura:28:37\0A   |\0A28 |     output(\22atk % xp_cap:    \22, atk % xp_cap,    \22\\n\22)\0A   |                                     ^\0A\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.15 = private unnamed_addr constant [18 x i8] c"10 + 5:          \00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.18 = private unnamed_addr constant [18 x i8] c"base_atk + atk:  \00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.21 = private unnamed_addr constant [18 x i8] c"2 + 3 * 4:       \00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.23 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.24 = private unnamed_addr constant [18 x i8] c"(2 + 3) * 4:     \00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.27 = private unnamed_addr constant [7 x i8] c"add:  \00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1
@str.30 = private unnamed_addr constant [7 x i8] c"sub:  \00", align 1
@str.31 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.32 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1
@str.33 = private unnamed_addr constant [7 x i8] c"mul:  \00", align 1
@str.34 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.35 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1
@str.36 = private unnamed_addr constant [7 x i8] c"div:  \00", align 1
@trap_msg.37 = private unnamed_addr constant [193 x i8] c"runtime error: Division by zero\0A   001.ura:43:24\0A   |\0A43 |     output(\22div:  \22, a / b, \22\\n\22)  // 5.0\0A   |                        ^\0A\00", align 1
@str.38 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.39 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1
@str.40 = private unnamed_addr constant [7 x i8] c"lt:   \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.41 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.42 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.43 = private unnamed_addr constant [7 x i8] c"gt:   \00", align 1
@true_str.44 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.45 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.46 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.47 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.48 = private unnamed_addr constant [7 x i8] c"lte:  \00", align 1
@true_str.49 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.50 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.51 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.52 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.53 = private unnamed_addr constant [7 x i8] c"gte:  \00", align 1
@true_str.54 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.55 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.56 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.57 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.58 = private unnamed_addr constant [14 x i8] c"clamp(0.52): \00", align 1
@str.59 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.60 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1
@str.61 = private unnamed_addr constant [14 x i8] c"clamp(0.01): \00", align 1
@str.62 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.63 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

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

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %add, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %atk3 = load i32, i32* %atk, align 4
  %def4 = load i32, i32* %def, align 4
  %sub = sub i32 %atk3, %def4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.2, i32 0, i32 0), i32 %sub, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %kills5 = load i32, i32* %kills, align 4
  %mul = mul i32 %kills5, 50
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.7, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.5, i32 0, i32 0), i32 %mul, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %total6 = load i32, i32* %total, align 4
  %count7 = load i32, i32* %count, align 4
  %iszero = icmp eq i32 %count7, 0
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %5 = call i64 @write(i32 2, i8* getelementptr inbounds ([221 x i8], [221 x i8]* @trap_msg, i32 0, i32 0), i64 220)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %total6, %count7
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.10, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.8, i32 0, i32 0), i32 %div, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %atk8 = load i32, i32* %atk, align 4
  %xp_cap9 = load i32, i32* %xp_cap, align 4
  %iszero10 = icmp eq i32 %xp_cap9, 0
  br i1 %iszero10, label %trap11, label %cont12

trap11:                                           ; preds = %cont
  %7 = call i64 @write(i32 2, i8* getelementptr inbounds ([217 x i8], [217 x i8]* @trap_msg.12, i32 0, i32 0), i64 216)
  call void @exit(i32 1)
  unreachable

cont12:                                           ; preds = %cont
  %mod = srem i32 %atk8, %xp_cap9
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.14, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.11, i32 0, i32 0), i32 %mod, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.17, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.15, i32 0, i32 0), i32 15, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %call = call i32 @base_atk()
  %atk13 = load i32, i32* %atk, align 4
  %add14 = add i32 %call, %atk13
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.20, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.18, i32 0, i32 0), i32 %add14, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.23, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.21, i32 0, i32 0), i32 14, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.26, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.24, i32 0, i32 0), i32 20, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %a = alloca float, align 4
  store float 1.500000e+00, float* %a, align 4
  %b = alloca float, align 4
  store float 0x3FD3333340000000, float* %b, align 4
  %a15 = load float, float* %a, align 4
  %b16 = load float, float* %b, align 4
  %fadd = fadd float %a15, %b16
  %f2d = fpext float %fadd to double
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.29, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.27, i32 0, i32 0), double %f2d, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %a17 = load float, float* %a, align 4
  %b18 = load float, float* %b, align 4
  %fsub = fsub float %a17, %b18
  %f2d19 = fpext float %fsub to double
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.32, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.30, i32 0, i32 0), double %f2d19, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.31, i32 0, i32 0))
  %a20 = load float, float* %a, align 4
  %b21 = load float, float* %b, align 4
  %fmul = fmul float %a20, %b21
  %f2d22 = fpext float %fmul to double
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.35, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.33, i32 0, i32 0), double %f2d22, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.34, i32 0, i32 0))
  %a23 = load float, float* %a, align 4
  %b24 = load float, float* %b, align 4
  %iszero25 = fcmp oeq float %b24, 0.000000e+00
  br i1 %iszero25, label %trap26, label %cont27

trap26:                                           ; preds = %cont12
  %16 = call i64 @write(i32 2, i8* getelementptr inbounds ([193 x i8], [193 x i8]* @trap_msg.37, i32 0, i32 0), i64 192)
  call void @exit(i32 1)
  unreachable

cont27:                                           ; preds = %cont12
  %fdiv = fdiv float %a23, %b24
  %f2d28 = fpext float %fdiv to double
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.39, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.36, i32 0, i32 0), double %f2d28, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.38, i32 0, i32 0))
  %a29 = load float, float* %a, align 4
  %b30 = load float, float* %b, align 4
  %flt = fcmp olt float %a29, %b30
  %bool_str = select i1 %flt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.42, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.40, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.41, i32 0, i32 0))
  %a31 = load float, float* %a, align 4
  %b32 = load float, float* %b, align 4
  %fgt = fcmp ogt float %a31, %b32
  %bool_str33 = select i1 %fgt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.44, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.45, i32 0, i32 0)
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.47, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.43, i32 0, i32 0), i8* %bool_str33, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
  %a34 = load float, float* %a, align 4
  %b35 = load float, float* %b, align 4
  %fle = fcmp ole float %a34, %b35
  %bool_str36 = select i1 %fle, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.49, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.50, i32 0, i32 0)
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.52, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.48, i32 0, i32 0), i8* %bool_str36, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.51, i32 0, i32 0))
  %a37 = load float, float* %a, align 4
  %b38 = load float, float* %b, align 4
  %fge = fcmp oge float %a37, %b38
  %bool_str39 = select i1 %fge, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.54, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.55, i32 0, i32 0)
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.57, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.53, i32 0, i32 0), i8* %bool_str39, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.56, i32 0, i32 0))
  %t = alloca float, align 4
  store float 0x3FE0A3D700000000, float* %t, align 4
  %t40 = load float, float* %t, align 4
  %call41 = call float @clamp(float %t40)
  %f2d42 = fpext float %call41 to double
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.60, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.58, i32 0, i32 0), double %f2d42, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.59, i32 0, i32 0))
  %call43 = call float @clamp(float 0x3F847AE140000000)
  %f2d44 = fpext float %call43 to double
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.63, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.61, i32 0, i32 0), double %f2d44, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.62, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 002 — arithmetic with precedence (2*3 + 20/4 - 1)

```ura
// operators/002.ura - arithmetic with precedence (2*3 + 20/4 - 1)

main():
    return 2 * 3 + 20 / 4 - 1
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
└─ return
   └─ - : i32
      ├─ + : i32
      │  ├─ * : i32
      │  │  ├─ int 2
      │  │  └─ int 3
      │  └─ / : i32
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

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [175 x i8] c"runtime error: Division by zero\0A  002.ura:4:23\0A  |\0A4 |     return 2 * 3 + 20 / 4 - 1\0A  |                       ^\0A\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([175 x i8], [175 x i8]* @trap_msg, i32 0, i32 0), i64 174)
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
// operators/003.ura - reassign an existing variable (2*10 + 3)

main():
    a i32 = 2
    a = a * 10 + 3
    return a
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 2
├─ = : i32
│  ├─ a : i32
│  └─ + : i32
│     ├─ * : i32
│     │  ├─ a : i32
│     │  └─ int 10
│     └─ int 3
└─ return
   └─ a : i32
```

```out
```

```err
exit: 23
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
// operators/004.ura - modulo

main():
    return 17 % 5
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
└─ return
   └─ % : i32
      ├─ int 17
      └─ int 5
```

```out
```

```err
exit: 2
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [153 x i8] c"runtime error: Modulo by zero\0A  004.ura:4:15\0A  |\0A4 |     return 17 % 5\0A  |               ^\0A\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  ret i32 2
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 005 — bitwise & | ^ << >>

```ura
// operators/005.ura - bitwise & | ^ << >>

main():
    a i32 = 12
    b i32 = 10
    output(a & b, "\n")
    output(a | b, "\n")
    output(a ^ b, "\n")
    output(a << 2, "\n")
    output(a >> 1, "\n")
    return 0
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 12
├─ = : i32
│  ├─ b : i32
│  └─ int 10
├─ output : void
│  ├─ & : i32
│  │  ├─ a : i32
│  │  └─ b : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ | : i32
│  │  ├─ a : i32
│  │  └─ b : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ ^ : i32
│  │  ├─ a : i32
│  │  └─ b : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ << : i32
│  │  ├─ a : i32
│  │  └─ int 2
│  └─ char[] "\n"
├─ output : void
│  ├─ >> : i32
│  │  ├─ a : i32
│  │  └─ int 1
│  └─ char[] "\n"
└─ return
   └─ int 0
```

```out
8
14
6
48
6
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca i32, align 4
  store i32 12, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %band = and i32 %a1, %b2
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %band, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %a3 = load i32, i32* %a, align 4
  %b4 = load i32, i32* %b, align 4
  %bor = or i32 %a3, %b4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i32 %bor, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %a5 = load i32, i32* %a, align 4
  %b6 = load i32, i32* %b, align 4
  %bxor = xor i32 %a5, %b6
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i32 %bxor, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a7 = load i32, i32* %a, align 4
  %shl = shl i32 %a7, 2
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i32 %shl, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %a8 = load i32, i32* %a, align 4
  %shr = ashr i32 %a8, 1
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i32 %shr, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — all comparison ops on hero/enemy stats

```ura
// operators/006.ura - all comparison ops on hero/enemy stats

fn enemy_atk() i32:
    return 18

main():
    hero_hp  i32 = 100
    orc_hp   i32 = 60
    min_hp   i32 = 100

    output("hero_hp == min_hp: ", hero_hp == min_hp, "\n")   // True
    output("hero_hp != orc_hp: ", hero_hp != orc_hp, "\n")   // True
    output("orc_hp < hero_hp:  ", orc_hp < hero_hp,  "\n")   // True
    output("orc_hp > hero_hp:  ", orc_hp > hero_hp,  "\n")   // False
    output("orc_hp <= 60:      ", orc_hp <= 60,       "\n")   // True
    output("hero_hp >= 100:    ", hero_hp >= 100,      "\n")   // True

    // comparison with fn call
    if enemy_atk() > 15:
        output("strong enemy\n")
    else:
        output("weak enemy\n")

    // chained via if
    if hero_hp == 100 and orc_hp < hero_hp:
        output("hero is at full health and winning\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn enemy_atk() : i32
└─ return
   └─ int 18

fn main() : i32
├─ = : i32
│  ├─ hero_hp : i32
│  └─ int 100
├─ = : i32
│  ├─ orc_hp : i32
│  └─ int 60
├─ = : i32
│  ├─ min_hp : i32
│  └─ int 100
├─ output : void
│  ├─ char[] "hero_hp == min_hp: "
│  ├─ == : bool
│  │  ├─ hero_hp : i32
│  │  └─ min_hp : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "hero_hp != orc_hp: "
│  ├─ != : bool
│  │  ├─ hero_hp : i32
│  │  └─ orc_hp : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "orc_hp < hero_hp:  "
│  ├─ < : bool
│  │  ├─ orc_hp : i32
│  │  └─ hero_hp : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "orc_hp > hero_hp:  "
│  ├─ > : bool
│  │  ├─ orc_hp : i32
│  │  └─ hero_hp : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "orc_hp <= 60:      "
│  ├─ <= : bool
│  │  ├─ orc_hp : i32
│  │  └─ int 60
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "hero_hp >= 100:    "
│  ├─ >= : bool
│  │  ├─ hero_hp : i32
│  │  └─ int 100
│  └─ char[] "\n"
├─ if
│  ├─ condition > : bool
│  │  ├─ call enemy_atk : i32
│  │  └─ int 15
│  ├─ output : void
│  │  └─ char[] "strong enemy\n"
│  └─ else
│     └─ output : void
│        └─ char[] "weak enemy\n"
└─ if
   ├─ condition and : bool
   │  ├─ == : bool
   │  │  ├─ hero_hp : i32
   │  │  └─ int 100
   │  └─ < : bool
   │     ├─ orc_hp : i32
   │     └─ hero_hp : i32
   └─ output : void
      └─ char[] "hero is at full health and winning\n"
```

```out
hero_hp == min_hp: True
hero_hp != orc_hp: True
orc_hp < hero_hp:  True
orc_hp > hero_hp:  False
orc_hp <= 60:      True
hero_hp >= 100:    True
strong enemy
hero is at full health and winning
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [20 x i8] c"hero_hp == min_hp: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.2 = private unnamed_addr constant [20 x i8] c"hero_hp != orc_hp: \00", align 1
@true_str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.7 = private unnamed_addr constant [20 x i8] c"orc_hp < hero_hp:  \00", align 1
@true_str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.12 = private unnamed_addr constant [20 x i8] c"orc_hp > hero_hp:  \00", align 1
@true_str.13 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.14 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.17 = private unnamed_addr constant [20 x i8] c"orc_hp <= 60:      \00", align 1
@true_str.18 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.19 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.21 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.22 = private unnamed_addr constant [20 x i8] c"hero_hp >= 100:    \00", align 1
@true_str.23 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.24 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.27 = private unnamed_addr constant [14 x i8] c"strong enemy\0A\00", align 1
@fmt.28 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.29 = private unnamed_addr constant [12 x i8] c"weak enemy\0A\00", align 1
@fmt.30 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.31 = private unnamed_addr constant [36 x i8] c"hero is at full health and winning\0A\00", align 1
@fmt.32 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @enemy_atk() {
entry:
  ret i32 18
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %hero_hp = alloca i32, align 4
  store i32 100, i32* %hero_hp, align 4
  %orc_hp = alloca i32, align 4
  store i32 60, i32* %orc_hp, align 4
  %min_hp = alloca i32, align 4
  store i32 100, i32* %min_hp, align 4
  %hero_hp1 = load i32, i32* %hero_hp, align 4
  %min_hp2 = load i32, i32* %min_hp, align 4
  %eq = icmp eq i32 %hero_hp1, %min_hp2
  %bool_str = select i1 %eq, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 19, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %hero_hp3 = load i32, i32* %hero_hp, align 4
  %orc_hp4 = load i32, i32* %orc_hp, align 4
  %ne = icmp ne i32 %hero_hp3, %orc_hp4
  %bool_str5 = select i1 %ne, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.3, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.4, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.6, i32 0, i32 0), i32 19, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.2, i32 0, i32 0), i8* %bool_str5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %orc_hp6 = load i32, i32* %orc_hp, align 4
  %hero_hp7 = load i32, i32* %hero_hp, align 4
  %lt = icmp slt i32 %orc_hp6, %hero_hp7
  %bool_str8 = select i1 %lt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.8, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.9, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.11, i32 0, i32 0), i32 19, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.7, i32 0, i32 0), i8* %bool_str8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %orc_hp9 = load i32, i32* %orc_hp, align 4
  %hero_hp10 = load i32, i32* %hero_hp, align 4
  %gt = icmp sgt i32 %orc_hp9, %hero_hp10
  %bool_str11 = select i1 %gt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.13, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.14, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.16, i32 0, i32 0), i32 19, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.12, i32 0, i32 0), i8* %bool_str11, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %orc_hp12 = load i32, i32* %orc_hp, align 4
  %le = icmp sle i32 %orc_hp12, 60
  %bool_str13 = select i1 %le, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.18, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.19, i32 0, i32 0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.21, i32 0, i32 0), i32 19, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.17, i32 0, i32 0), i8* %bool_str13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0))
  %hero_hp14 = load i32, i32* %hero_hp, align 4
  %ge = icmp sge i32 %hero_hp14, 100
  %bool_str15 = select i1 %ge, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.23, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.24, i32 0, i32 0)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.26, i32 0, i32 0), i32 19, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.22, i32 0, i32 0), i8* %bool_str15, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %call = call i32 @enemy_atk()
  %gt16 = icmp sgt i32 %call, 15
  br i1 %gt16, label %then, label %next

endif:                                            ; preds = %next, %then
  %hero_hp19 = load i32, i32* %hero_hp, align 4
  %eq20 = icmp eq i32 %hero_hp19, 100
  %orc_hp21 = load i32, i32* %orc_hp, align 4
  %hero_hp22 = load i32, i32* %hero_hp, align 4
  %lt23 = icmp slt i32 %orc_hp21, %hero_hp22
  %and = and i1 %eq20, %lt23
  br i1 %and, label %then18, label %endif17

then:                                             ; preds = %entry
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.28, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.27, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.30, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.29, i32 0, i32 0))
  br label %endif

endif17:                                          ; preds = %then18, %endif
  ret i32 0

then18:                                           ; preds = %endif
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.32, i32 0, i32 0), i32 35, i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str.31, i32 0, i32 0))
  br label %endif17
}

declare i32 @printf(i8*, ...)
```

## 007 — comparisons produce bools (with precedence)

```ura
// operators/007.ura - comparisons produce bools (with precedence)

main():
    a i32 = 5
    b i32 = 9
    t bool = a < b
    u bool = a == 5
    v bool = b != a
    w bool = b >= 9
    x bool = a <= 4
    y bool = a > b
    z bool = a is 5
    m bool = a + 1 < b * 2
    return 3
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 5
├─ = : i32
│  ├─ b : i32
│  └─ int 9
├─ = : bool
│  ├─ t : bool
│  └─ < : bool
│     ├─ a : i32
│     └─ b : i32
├─ = : bool
│  ├─ u : bool
│  └─ == : bool
│     ├─ a : i32
│     └─ int 5
├─ = : bool
│  ├─ v : bool
│  └─ != : bool
│     ├─ b : i32
│     └─ a : i32
├─ = : bool
│  ├─ w : bool
│  └─ >= : bool
│     ├─ b : i32
│     └─ int 9
├─ = : bool
│  ├─ x : bool
│  └─ <= : bool
│     ├─ a : i32
│     └─ int 4
├─ = : bool
│  ├─ y : bool
│  └─ > : bool
│     ├─ a : i32
│     └─ b : i32
├─ = : bool
│  ├─ z : bool
│  └─ == : bool
│     ├─ a : i32
│     └─ int 5
├─ = : bool
│  ├─ m : bool
│  └─ < : bool
│     ├─ + : i32
│     │  ├─ a : i32
│     │  └─ int 1
│     └─ * : i32
│        ├─ b : i32
│        └─ int 2
└─ return
   └─ int 3
```

```out
```

```err
exit: 3
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 9, i32* %b, align 4
  %t = alloca i1, align 1
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %lt = icmp slt i32 %a1, %b2
  store i1 %lt, i1* %t, align 1
  %u = alloca i1, align 1
  %a3 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a3, 5
  store i1 %eq, i1* %u, align 1
  %v = alloca i1, align 1
  %b4 = load i32, i32* %b, align 4
  %a5 = load i32, i32* %a, align 4
  %ne = icmp ne i32 %b4, %a5
  store i1 %ne, i1* %v, align 1
  %w = alloca i1, align 1
  %b6 = load i32, i32* %b, align 4
  %ge = icmp sge i32 %b6, 9
  store i1 %ge, i1* %w, align 1
  %x = alloca i1, align 1
  %a7 = load i32, i32* %a, align 4
  %le = icmp sle i32 %a7, 4
  store i1 %le, i1* %x, align 1
  %y = alloca i1, align 1
  %a8 = load i32, i32* %a, align 4
  %b9 = load i32, i32* %b, align 4
  %gt = icmp sgt i32 %a8, %b9
  store i1 %gt, i1* %y, align 1
  %z = alloca i1, align 1
  %a10 = load i32, i32* %a, align 4
  %eq11 = icmp eq i32 %a10, 5
  store i1 %eq11, i1* %z, align 1
  %m = alloca i1, align 1
  %a12 = load i32, i32* %a, align 4
  %add = add i32 %a12, 1
  %b13 = load i32, i32* %b, align 4
  %mul = mul i32 %b13, 2
  %lt14 = icmp slt i32 %add, %mul
  store i1 %lt14, i1* %m, align 1
  ret i32 3
}
```

## 008 — compound assignment += -= *= /= %=

```ura
// operators/008.ura - compound assignment += -= *= /= %=

main():
    x i32 = 10
    x += 5
    output(x, "\n")
    x -= 3
    output(x, "\n")
    x *= 4
    output(x, "\n")
    x /= 6
    output(x, "\n")
    x %= 5
    output(x, "\n")
    return 0
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ x : i32
│  └─ int 10
├─ += : i32
│  ├─ x : i32
│  └─ int 5
├─ output : void
│  ├─ x : i32
│  └─ char[] "\n"
├─ -= : i32
│  ├─ x : i32
│  └─ int 3
├─ output : void
│  ├─ x : i32
│  └─ char[] "\n"
├─ *= : i32
│  ├─ x : i32
│  └─ int 4
├─ output : void
│  ├─ x : i32
│  └─ char[] "\n"
├─ /= : i32
│  ├─ x : i32
│  └─ int 6
├─ output : void
│  ├─ x : i32
│  └─ char[] "\n"
├─ %= : i32
│  ├─ x : i32
│  └─ int 5
├─ output : void
│  ├─ x : i32
│  └─ char[] "\n"
└─ return
   └─ int 0
```

```out
15
12
48
8
3
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@trap_msg = private unnamed_addr constant [145 x i8] c"runtime error: Division by zero\0A   008.ura:11:7\0A   |\0A11 |     x /= 6\0A   |       ^^\0A\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@trap_msg.7 = private unnamed_addr constant [143 x i8] c"runtime error: Modulo by zero\0A   008.ura:13:7\0A   |\0A13 |     x %= 5\0A   |       ^^\0A\00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %x = alloca i32, align 4
  store i32 10, i32* %x, align 4
  %cur = load i32, i32* %x, align 4
  %add = add i32 %cur, 5
  store i32 %add, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %x1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %cur2 = load i32, i32* %x, align 4
  %sub = sub i32 %cur2, 3
  store i32 %sub, i32* %x, align 4
  %x3 = load i32, i32* %x, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %cur4 = load i32, i32* %x, align 4
  %mul = mul i32 %cur4, 4
  store i32 %mul, i32* %x, align 4
  %x5 = load i32, i32* %x, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i32 %x5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %cur6 = load i32, i32* %x, align 4
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %5 = call i64 @write(i32 2, i8* getelementptr inbounds ([145 x i8], [145 x i8]* @trap_msg, i32 0, i32 0), i64 144)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %cur6, 6
  store i32 %div, i32* %x, align 4
  %x7 = load i32, i32* %x, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i32 %x7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %cur8 = load i32, i32* %x, align 4
  br i1 false, label %trap9, label %cont10

trap9:                                            ; preds = %cont
  %7 = call i64 @write(i32 2, i8* getelementptr inbounds ([143 x i8], [143 x i8]* @trap_msg.7, i32 0, i32 0), i64 142)
  call void @exit(i32 1)
  unreachable

cont10:                                           ; preds = %cont
  %mod = srem i32 %cur8, 5
  store i32 %mod, i32* %x, align 4
  %x11 = load i32, i32* %x, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.9, i32 0, i32 0), i32 %x11, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 009 — logical ops: &&, ||, !, and, or, not

```ura
// operators/009.ura - logical ops: &&, ||, !, and, or, not

fn is_alive(hp i32) bool:
    return hp > 0

fn has_key(keys i32) bool:
    return keys > 0

main():
    hp       i32  = 60
    keys     i32  = 1
    exhausted bool = False

    // &&
    can_proceed bool = is_alive(hp) && has_key(keys) && !exhausted
    output("can_proceed: ", can_proceed, "\n")   // True

    // ||
    escape bool = hp < 10 || keys == 0
    output("escape:      ", escape,      "\n")   // False

    // !
    blocked bool = !can_proceed
    output("blocked:     ", blocked,     "\n")   // False

    // and / or / not keywords
    if is_alive(hp) and not exhausted:
        output("hero fights\n")

    if hp < 10 or keys == 0:
        output("hero retreats\n")
    else:
        output("hero advances\n")

    // f64 negation
    alive bool = is_alive(hp)
    output("!!alive: ", !(!alive), "\n")   // True
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn is_alive(hp : i32) : bool
└─ return
   └─ > : bool
      ├─ hp : i32
      └─ int 0

fn has_key(keys : i32) : bool
└─ return
   └─ > : bool
      ├─ keys : i32
      └─ int 0

fn main() : i32
├─ = : i32
│  ├─ hp : i32
│  └─ int 60
├─ = : i32
│  ├─ keys : i32
│  └─ int 1
├─ = : bool
│  ├─ exhausted : bool
│  └─ bool False
├─ = : bool
│  ├─ can_proceed : bool
│  └─ and : bool
│     ├─ and : bool
│     │  ├─ call is_alive : bool
│     │  │  └─ hp : i32
│     │  └─ call has_key : bool
│     │     └─ keys : i32
│     └─ not : bool
│        └─ exhausted : bool
├─ output : void
│  ├─ char[] "can_proceed: "
│  ├─ can_proceed : bool
│  └─ char[] "\n"
├─ = : bool
│  ├─ escape : bool
│  └─ or : bool
│     ├─ < : bool
│     │  ├─ hp : i32
│     │  └─ int 10
│     └─ == : bool
│        ├─ keys : i32
│        └─ int 0
├─ output : void
│  ├─ char[] "escape:      "
│  ├─ escape : bool
│  └─ char[] "\n"
├─ = : bool
│  ├─ blocked : bool
│  └─ not : bool
│     └─ can_proceed : bool
├─ output : void
│  ├─ char[] "blocked:     "
│  ├─ blocked : bool
│  └─ char[] "\n"
├─ if
│  ├─ condition and : bool
│  │  ├─ call is_alive : bool
│  │  │  └─ hp : i32
│  │  └─ not : bool
│  │     └─ exhausted : bool
│  └─ output : void
│     └─ char[] "hero fights\n"
├─ if
│  ├─ condition or : bool
│  │  ├─ < : bool
│  │  │  ├─ hp : i32
│  │  │  └─ int 10
│  │  └─ == : bool
│  │     ├─ keys : i32
│  │     └─ int 0
│  ├─ output : void
│  │  └─ char[] "hero retreats\n"
│  └─ else
│     └─ output : void
│        └─ char[] "hero advances\n"
├─ = : bool
│  ├─ alive : bool
│  └─ call is_alive : bool
│     └─ hp : i32
└─ output : void
   ├─ char[] "!!alive: "
   ├─ not : bool
   │  └─ not : bool
   │     └─ alive : bool
   └─ char[] "\n"
```

```out
can_proceed: True
escape:      False
blocked:     False
hero fights
hero advances
!!alive: True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [14 x i8] c"can_proceed: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.2 = private unnamed_addr constant [14 x i8] c"escape:      \00", align 1
@true_str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.7 = private unnamed_addr constant [14 x i8] c"blocked:     \00", align 1
@true_str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.12 = private unnamed_addr constant [13 x i8] c"hero fights\0A\00", align 1
@fmt.13 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.14 = private unnamed_addr constant [15 x i8] c"hero retreats\0A\00", align 1
@fmt.15 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.16 = private unnamed_addr constant [15 x i8] c"hero advances\0A\00", align 1
@fmt.17 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.18 = private unnamed_addr constant [10 x i8] c"!!alive: \00", align 1
@true_str.19 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.20 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i1 @is_alive(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %gt = icmp sgt i32 %hp1, 0
  ret i1 %gt
}

define i1 @has_key(i32 %0) {
entry:
  %keys = alloca i32, align 4
  store i32 %0, i32* %keys, align 4
  %keys1 = load i32, i32* %keys, align 4
  %gt = icmp sgt i32 %keys1, 0
  ret i1 %gt
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %hp = alloca i32, align 4
  store i32 60, i32* %hp, align 4
  %keys = alloca i32, align 4
  store i32 1, i32* %keys, align 4
  %exhausted = alloca i1, align 1
  store i1 false, i1* %exhausted, align 1
  %can_proceed = alloca i1, align 1
  %hp1 = load i32, i32* %hp, align 4
  %call = call i1 @is_alive(i32 %hp1)
  %keys2 = load i32, i32* %keys, align 4
  %call3 = call i1 @has_key(i32 %keys2)
  %and = and i1 %call, %call3
  %exhausted4 = load i1, i1* %exhausted, align 1
  %not = xor i1 %exhausted4, true
  %and5 = and i1 %and, %not
  store i1 %and5, i1* %can_proceed, align 1
  %can_proceed6 = load i1, i1* %can_proceed, align 1
  %bool_str = select i1 %can_proceed6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %escape = alloca i1, align 1
  %hp7 = load i32, i32* %hp, align 4
  %lt = icmp slt i32 %hp7, 10
  %keys8 = load i32, i32* %keys, align 4
  %eq = icmp eq i32 %keys8, 0
  %or = or i1 %lt, %eq
  store i1 %or, i1* %escape, align 1
  %escape9 = load i1, i1* %escape, align 1
  %bool_str10 = select i1 %escape9, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.3, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.4, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.6, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0), i8* %bool_str10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %blocked = alloca i1, align 1
  %can_proceed11 = load i1, i1* %can_proceed, align 1
  %not12 = xor i1 %can_proceed11, true
  store i1 %not12, i1* %blocked, align 1
  %blocked13 = load i1, i1* %blocked, align 1
  %bool_str14 = select i1 %blocked13, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.8, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.9, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.11, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.7, i32 0, i32 0), i8* %bool_str14, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %hp15 = load i32, i32* %hp, align 4
  %call16 = call i1 @is_alive(i32 %hp15)
  %exhausted17 = load i1, i1* %exhausted, align 1
  %not18 = xor i1 %exhausted17, true
  %and19 = and i1 %call16, %not18
  br i1 %and19, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %hp22 = load i32, i32* %hp, align 4
  %lt23 = icmp slt i32 %hp22, 10
  %keys24 = load i32, i32* %keys, align 4
  %eq25 = icmp eq i32 %keys24, 0
  %or26 = or i1 %lt23, %eq25
  br i1 %or26, label %then21, label %next

then:                                             ; preds = %entry
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.13, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.12, i32 0, i32 0))
  br label %endif

endif20:                                          ; preds = %next, %then21
  %alive = alloca i1, align 1
  %hp27 = load i32, i32* %hp, align 4
  %call28 = call i1 @is_alive(i32 %hp27)
  store i1 %call28, i1* %alive, align 1
  %alive29 = load i1, i1* %alive, align 1
  %not30 = xor i1 %alive29, true
  %not31 = xor i1 %not30, true
  %bool_str32 = select i1 %not31, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.19, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.20, i32 0, i32 0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.22, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.18, i32 0, i32 0), i8* %bool_str32, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  ret i32 0

then21:                                           ; preds = %endif
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.15, i32 0, i32 0), i32 14, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.14, i32 0, i32 0))
  br label %endif20

next:                                             ; preds = %endif
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.17, i32 0, i32 0), i32 14, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.16, i32 0, i32 0))
  br label %endif20
}

declare i32 @printf(i8*, ...)
```

## 010 — and / or with comparisons and precedence

```ura
// operators/010.ura - and / or with comparisons and precedence

main():
    a i32 = 5
    b i32 = 9
    output(a < b and b == 9, "\n")
    output(a > b or b == 9, "\n")
    output(a < b and a > b, "\n")
    output(a < b or a > b, "\n")
    return 0
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 5
├─ = : i32
│  ├─ b : i32
│  └─ int 9
├─ output : void
│  ├─ and : bool
│  │  ├─ < : bool
│  │  │  ├─ a : i32
│  │  │  └─ b : i32
│  │  └─ == : bool
│  │     ├─ b : i32
│  │     └─ int 9
│  └─ char[] "\n"
├─ output : void
│  ├─ or : bool
│  │  ├─ > : bool
│  │  │  ├─ a : i32
│  │  │  └─ b : i32
│  │  └─ == : bool
│  │     ├─ b : i32
│  │     └─ int 9
│  └─ char[] "\n"
├─ output : void
│  ├─ and : bool
│  │  ├─ < : bool
│  │  │  ├─ a : i32
│  │  │  └─ b : i32
│  │  └─ > : bool
│  │     ├─ a : i32
│  │     └─ b : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ or : bool
│  │  ├─ < : bool
│  │  │  ├─ a : i32
│  │  │  └─ b : i32
│  │  └─ > : bool
│  │     ├─ a : i32
│  │     └─ b : i32
│  └─ char[] "\n"
└─ return
   └─ int 0
```

```out
True
True
False
True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.9 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.10 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.12 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 9, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %lt = icmp slt i32 %a1, %b2
  %b3 = load i32, i32* %b, align 4
  %eq = icmp eq i32 %b3, 9
  %and = and i1 %lt, %eq
  %bool_str = select i1 %and, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %a4 = load i32, i32* %a, align 4
  %b5 = load i32, i32* %b, align 4
  %gt = icmp sgt i32 %a4, %b5
  %b6 = load i32, i32* %b, align 4
  %eq7 = icmp eq i32 %b6, 9
  %or = or i1 %gt, %eq7
  %bool_str8 = select i1 %or, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* %bool_str8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a9 = load i32, i32* %a, align 4
  %b10 = load i32, i32* %b, align 4
  %lt11 = icmp slt i32 %a9, %b10
  %a12 = load i32, i32* %a, align 4
  %b13 = load i32, i32* %b, align 4
  %gt14 = icmp sgt i32 %a12, %b13
  %and15 = and i1 %lt11, %gt14
  %bool_str16 = select i1 %and15, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.5, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.6, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i8* %bool_str16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %a17 = load i32, i32* %a, align 4
  %b18 = load i32, i32* %b, align 4
  %lt19 = icmp slt i32 %a17, %b18
  %a20 = load i32, i32* %a, align 4
  %b21 = load i32, i32* %b, align 4
  %gt22 = icmp sgt i32 %a20, %b21
  %or23 = or i1 %lt19, %gt22
  %bool_str24 = select i1 %or23, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.9, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.10, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.12, i32 0, i32 0), i8* %bool_str24, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.11, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 011 — unary not / minus / bitwise-not

```ura
// operators/011.ura - unary not / minus / bitwise-not

main():
    a i32 = 5
    t bool = True
    output(-a, "\n")
    output(not t, "\n")
    output(~a, "\n")
    output(not (a < 3), "\n")
    output(-a + 10, "\n")
    return 0
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 5
├─ = : bool
│  ├─ t : bool
│  └─ bool True
├─ output : void
│  ├─ - : i32
│  │  ├─ int 0
│  │  └─ a : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ not : bool
│  │  └─ t : bool
│  └─ char[] "\n"
├─ output : void
│  ├─ ~ : i32
│  │  └─ a : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ not : bool
│  │  └─ < : bool
│  │     ├─ a : i32
│  │     └─ int 3
│  └─ char[] "\n"
├─ output : void
│  ├─ + : i32
│  │  ├─ - : i32
│  │  │  ├─ int 0
│  │  │  └─ a : i32
│  │  └─ int 10
│  └─ char[] "\n"
└─ return
   └─ int 0
```

```out
-5
False
-6
True
5
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@true_str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %t = alloca i1, align 1
  store i1 true, i1* %t, align 1
  %a1 = load i32, i32* %a, align 4
  %sub = sub i32 0, %a1
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %sub, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %t2 = load i1, i1* %t, align 1
  %not = xor i1 %t2, true
  %bool_str = select i1 %not, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %a3 = load i32, i32* %a, align 4
  %not4 = xor i32 %a3, -1
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i32 %not4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a5 = load i32, i32* %a, align 4
  %lt = icmp slt i32 %a5, 3
  %not6 = xor i1 %lt, true
  %bool_str7 = select i1 %not6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.5, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.6, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i8* %bool_str7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %a8 = load i32, i32* %a, align 4
  %sub9 = sub i32 0, %a8
  %add = add i32 %sub9, 10
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i32 %add, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 012 — compound bitwise assignment

```ura
// operators/012.ura - &= |= ^= <<= >>=

main():
    a i32 = 12
    a &= 10
    b i32 = 12
    b |= 3
    c i32 = 12
    c ^= 10
    output("and ", a, "  or ", b, "  xor ", c, "\n")

    d i32 = 3
    d <<= 4
    e i32 = 48
    e >>= 2
    output("shl ", d, "  shr ", e, "\n")

    // an unsigned right shift is logical, not arithmetic
    u u32 = 4000000000 as u32
    u >>= 1
    output("unsigned shr ", u, "\n")

    // a signed one keeps the sign
    s i32 = 0 - 16
    s >>= 2
    output("signed shr ", s, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 12
├─ &= : i32
│  ├─ a : i32
│  └─ int 10
├─ = : i32
│  ├─ b : i32
│  └─ int 12
├─ |= : i32
│  ├─ b : i32
│  └─ int 3
├─ = : i32
│  ├─ c : i32
│  └─ int 12
├─ ^= : i32
│  ├─ c : i32
│  └─ int 10
├─ output : void
│  ├─ char[] "and "
│  ├─ a : i32
│  ├─ char[] "  or "
│  ├─ b : i32
│  ├─ char[] "  xor "
│  ├─ c : i32
│  └─ char[] "\n"
├─ = : i32
│  ├─ d : i32
│  └─ int 3
├─ <<= : i32
│  ├─ d : i32
│  └─ int 4
├─ = : i32
│  ├─ e : i32
│  └─ int 48
├─ >>= : i32
│  ├─ e : i32
│  └─ int 2
├─ output : void
│  ├─ char[] "shl "
│  ├─ d : i32
│  ├─ char[] "  shr "
│  ├─ e : i32
│  └─ char[] "\n"
├─ = : u32
│  ├─ u : u32
│  └─ cast : u32
│     └─ int 4000000000
├─ >>= : u32
│  ├─ u : u32
│  └─ int 1
├─ output : void
│  ├─ char[] "unsigned shr "
│  ├─ u : u32
│  └─ char[] "\n"
├─ = : i32
│  ├─ s : i32
│  └─ - : i32
│     ├─ int 0
│     └─ int 16
├─ >>= : i32
│  ├─ s : i32
│  └─ int 2
└─ output : void
   ├─ char[] "signed shr "
   ├─ s : i32
   └─ char[] "\n"
```

```out
and 8  or 15  xor 6
shl 48  shr 12
unsigned shr 2000000000
signed shr -4
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"and \00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"  or \00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"  xor \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [23 x i8] c"%.*s%d%.*s%d%.*s%d%.*s\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"shl \00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"  shr \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.8 = private unnamed_addr constant [14 x i8] c"unsigned shr \00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [11 x i8] c"%.*s%u%.*s\00", align 1
@str.11 = private unnamed_addr constant [12 x i8] c"signed shr \00", align 1
@str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.13 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca i32, align 4
  store i32 12, i32* %a, align 4
  %cur = load i32, i32* %a, align 4
  %band = and i32 %cur, 10
  store i32 %band, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 12, i32* %b, align 4
  %cur1 = load i32, i32* %b, align 4
  %bor = or i32 %cur1, 3
  store i32 %bor, i32* %b, align 4
  %c = alloca i32, align 4
  store i32 12, i32* %c, align 4
  %cur2 = load i32, i32* %c, align 4
  %bxor = xor i32 %cur2, 10
  store i32 %bxor, i32* %c, align 4
  %a3 = load i32, i32* %a, align 4
  %b4 = load i32, i32* %b, align 4
  %c5 = load i32, i32* %c, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @fmt, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i32 %a3, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 %b4, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %c5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %d = alloca i32, align 4
  store i32 3, i32* %d, align 4
  %cur6 = load i32, i32* %d, align 4
  %shl = shl i32 %cur6, 4
  store i32 %shl, i32* %d, align 4
  %e = alloca i32, align 4
  store i32 48, i32* %e, align 4
  %cur7 = load i32, i32* %e, align 4
  %shr = ashr i32 %cur7, 2
  store i32 %shr, i32* %e, align 4
  %d8 = load i32, i32* %d, align 4
  %e9 = load i32, i32* %e, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.7, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i32 %d8, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 %e9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %u = alloca i32, align 4
  store i32 -294967296, i32* %u, align 4
  %cur10 = load i32, i32* %u, align 4
  %shr11 = lshr i32 %cur10, 1
  store i32 %shr11, i32* %u, align 4
  %u12 = load i32, i32* %u, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.10, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.8, i32 0, i32 0), i32 %u12, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %s = alloca i32, align 4
  store i32 -16, i32* %s, align 4
  %cur13 = load i32, i32* %s, align 4
  %shr14 = ashr i32 %cur13, 2
  store i32 %shr14, i32* %s, align 4
  %s15 = load i32, i32* %s, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.13, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.11, i32 0, i32 0), i32 %s15, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.12, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 013 — `operator` outside a struct definition

```ura
// operators/013.ura - `operator` outside a struct definition
use "@/header"

operator + (a i32, b i32) i32:
    return a + b

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: 'operator' may only be declared inside a struct
  013.ura:4:1
  |
4 | operator + (a i32, b i32) i32:
  | ^^^^^^^^
error: Expected ')' after expression
  013.ura:4:12
  |
4 | operator + (a i32, b i32) i32:
  |            ^
```

```ll
```

## 014 — `operator` missing operator symbol

```ura
// operators/014.ura - `operator` missing operator symbol
use "@/header"

struct Vec:
    x i32

    operator () Vec:
        return self

main():
    v Vec
    v.x = 1
```

```tree
```

```out
```

```err
error: Expected an operator after 'operator' (one of + - * / % & | ^ << >> == != < > <= >= = += -= *= /= %= &= |= ^= <<= >>= drop)
  014.ura:7:5
  |
7 |     operator () Vec:
  |     ^^^^^^^^
```

```ll
```

## 015 — adding two structs without an `operator +` overload

```ura
// operators/015.ura - adding two structs without an `operator +` overload
use "@/header"

struct Vec:
    x i32
    y i32

main():
    a Vec
    b Vec
    c Vec = a + b
    output(c.x, "\n")
```

```tree
```

```out
```

```err
error: Struct Vec has no 'operator +' taking Vec; declare one inside the struct
   015.ura:11:15
   |
11 |     c Vec = a + b
   |               ^
```

```ll
```

## 016 — type mismatch between operands

```ura
// operators/016.ura - type mismatch between operands

fn twice(n i32) i32:
    return n + n
main():
    return twice + 1
```

```tree
```

```out
```

```err
error: Cannot use '+' with fn and i32
  016.ura:6:18
  |
6 |     return twice + 1
  |                  ^
```

```ll
```

## 017 — a comparison result is a bool, not an i32

```ura
// operators/017.ura - a comparison result is a bool, not an i32

main():
    a i32 = 5
    n i32 = a < 9
    return 0
```

```tree
```

```out
```

```err
error: Cannot assign bool to i32
  017.ura:5:11
  |
5 |     n i32 = a < 9
  |           ^
```

```ll
```

## 018 — bitwise operator on f32 operands is rejected

```ura
// operators/018.ura - bitwise operator on f32 operands is rejected

main():
    x f32 = 1.5 & 2.0
    return 0
```

```tree
```

```out
```

```err
error: Bitwise and shift operators require integer operands
  018.ura:4:17
  |
4 |     x f32 = 1.5 & 2.0
  |                 ^
```

```ll
```

## 019 — divide by zero prints file:line and exits 1

```ura
// operators/019.ura - divide by zero prints file:line and exits 1

main():
    b i32 = 0
    a i32 = 10
    c i32 = a / b
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ b : i32
│  └─ int 0
├─ = : i32
│  ├─ a : i32
│  └─ int 10
└─ = : i32
   ├─ c : i32
   └─ / : i32
      ├─ a : i32
      └─ b : i32
```

```out
```

```err
runtime error: Division by zero
  019.ura:6:15
  |
6 |     c i32 = a / b
  |               ^
exit: 1
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [155 x i8] c"runtime error: Division by zero\0A  019.ura:6:15\0A  |\0A6 |     c i32 = a / b\0A  |               ^\0A\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %b = alloca i32, align 4
  store i32 0, i32* %b, align 4
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %c = alloca i32, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %iszero = icmp eq i32 %b2, 0
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([155 x i8], [155 x i8]* @trap_msg, i32 0, i32 0), i64 154)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %a1, %b2
  store i32 %div, i32* %c, align 4
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 020 — f32 division by zero traps (not inf)

```ura
// operators/020.ura - f32 division by zero traps (not inf)

main():
    b f32 = 0.0
    a f32 = 10.0
    c f32 = a / b
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : f32
│  ├─ b : f32
│  └─ float 0
├─ = : f32
│  ├─ a : f32
│  └─ float 10
└─ = : f32
   ├─ c : f32
   └─ / : f32
      ├─ a : f32
      └─ b : f32
```

```out
```

```err
runtime error: Division by zero
  020.ura:6:15
  |
6 |     c f32 = a / b
  |               ^
exit: 1
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [155 x i8] c"runtime error: Division by zero\0A  020.ura:6:15\0A  |\0A6 |     c f32 = a / b\0A  |               ^\0A\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([155 x i8], [155 x i8]* @trap_msg, i32 0, i32 0), i64 154)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %fdiv = fdiv float %a1, %b2
  store float %fdiv, float* %c, align 4
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```
