# casting

## index

- 001 — `as` casting + long / short via explicit cast
- 002 — `as` converts between float and the integer types

## 001 — `as` casting + long / short via explicit cast

```ura
// casting/001.ura - `as` casting + long / short via explicit cast

main():
    a int   = 300
    big long  = a as long
    small short = a as short
    c int   = big as int
    output(big, "\n")
    output(small, "\n")
    output(c, "\n")
    output(5 as long, "\n")
    return 0
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 300
├─ = : i64
│  ├─ big : i64
│  └─ cast : i64
│     └─ a : i32
├─ = : i16
│  ├─ small : i16
│  └─ cast : i16
│     └─ a : i32
├─ = : i32
│  ├─ c : i32
│  └─ cast : i32
│     └─ big : i64
├─ output : void
│  ├─ big : i64
│  └─ chars "\n"
├─ output : void
│  ├─ small : i16
│  └─ chars "\n"
├─ output : void
│  ├─ c : i32
│  └─ chars "\n"
├─ output : void
│  ├─ cast : i64
│  │  └─ int 5
│  └─ chars "\n"
└─ return
   └─ int 0
```

```out
300
300
300
5
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%lld%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%lld%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 300, i32* %a, align 4
  %big = alloca i64, align 8
  %a1 = load i32, i32* %a, align 4
  %cast = sext i32 %a1 to i64
  store i64 %cast, i64* %big, align 4
  %small = alloca i16, align 2
  %a2 = load i32, i32* %a, align 4
  %cast3 = trunc i32 %a2 to i16
  store i16 %cast3, i16* %small, align 2
  %c = alloca i32, align 4
  %big4 = load i64, i64* %big, align 4
  %cast5 = trunc i64 %big4 to i32
  store i32 %cast5, i32* %c, align 4
  %big6 = load i64, i64* %big, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i64 %big6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %small7 = load i16, i16* %small, align 2
  %s2i = sext i16 %small7 to i32
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %s2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %c8 = load i32, i32* %c, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %c8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i64 5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — `as` converts between float and the integer types

```ura
// casting/002.ura - float <-> int casts

main():
    i int   = 300
    f float = i as float
    output("int->float ", f, "\n")
    output("float->int ", 2.9 as int, " ", (0.0 - 2.9) as int, "\n")
    l long  = 7 as long
    output("long->float ", l as float, "\n")
    s short = 5 as short
    output("short->float ", s as float, "\n")
    c char  = 'A'
    output("char->float ", c as float, " char->int ", c as int, "\n")
    b bool  = True
    output("bool->float ", b as float, " bool->int ", b as int, "\n")
    output("float->long ", 9.75 as long, "\n")
    output("round trip ", 4.9 as int as float, "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : i32
│  ├─ i : i32
│  └─ int 300
├─ = : f32
│  ├─ f : f32
│  └─ cast : f32
│     └─ i : i32
├─ output : void
│  ├─ chars "int->float "
│  ├─ f : f32
│  └─ chars "\n"
├─ output : void
│  ├─ chars "float->int "
│  ├─ cast : i32
│  │  └─ float 2.9
│  ├─ chars " "
│  ├─ cast : i32
│  │  └─ - : f32
│  │     ├─ float 0
│  │     └─ float 2.9
│  └─ chars "\n"
├─ = : i64
│  ├─ l : i64
│  └─ cast : i64
│     └─ int 7
├─ output : void
│  ├─ chars "long->float "
│  ├─ cast : f32
│  │  └─ l : i64
│  └─ chars "\n"
├─ = : i16
│  ├─ s : i16
│  └─ cast : i16
│     └─ int 5
├─ output : void
│  ├─ chars "short->float "
│  ├─ cast : f32
│  │  └─ s : i16
│  └─ chars "\n"
├─ = : char
│  ├─ c : char
│  └─ char 'A'
├─ output : void
│  ├─ chars "char->float "
│  ├─ cast : f32
│  │  └─ c : char
│  ├─ chars " char->int "
│  ├─ cast : i32
│  │  └─ c : char
│  └─ chars "\n"
├─ = : bool
│  ├─ b : bool
│  └─ bool True
├─ output : void
│  ├─ chars "bool->float "
│  ├─ cast : f32
│  │  └─ b : bool
│  ├─ chars " bool->int "
│  ├─ cast : i32
│  │  └─ b : bool
│  └─ chars "\n"
├─ output : void
│  ├─ chars "float->long "
│  ├─ cast : i64
│  │  └─ float 9.75
│  └─ chars "\n"
└─ output : void
   ├─ chars "round trip "
   ├─ cast : f32
   │  └─ cast : i32
   │     └─ float 4.9
   └─ chars "\n"
```

```out
int->float 300.000000
float->int 2 -2
long->float 7.000000
short->float 5.000000
char->float 65.000000 char->int 65
bool->float 1.000000 bool->int 1
float->long 9
round trip 4.000000
```

```err
```

```ll

@str = private unnamed_addr constant [12 x i8] c"int->float \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"float->int \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.6 = private unnamed_addr constant [13 x i8] c"long->float \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.9 = private unnamed_addr constant [14 x i8] c"short->float \00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.12 = private unnamed_addr constant [13 x i8] c"char->float \00", align 1
@str.13 = private unnamed_addr constant [12 x i8] c" char->int \00", align 1
@str.14 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.15 = private unnamed_addr constant [11 x i8] c"%s%f%s%d%s\00", align 1
@str.16 = private unnamed_addr constant [13 x i8] c"bool->float \00", align 1
@str.17 = private unnamed_addr constant [12 x i8] c" bool->int \00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.19 = private unnamed_addr constant [11 x i8] c"%s%f%s%d%s\00", align 1
@str.20 = private unnamed_addr constant [13 x i8] c"float->long \00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [9 x i8] c"%s%lld%s\00", align 1
@str.23 = private unnamed_addr constant [12 x i8] c"round trip \00", align 1
@str.24 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.25 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 300, i32* %i, align 4
  %f = alloca float, align 4
  %i1 = load i32, i32* %i, align 4
  %cast = sitofp i32 %i1 to float
  store float %cast, float* %f, align 4
  %f2 = load float, float* %f, align 4
  %f2d = fpext float %f2 to double
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 -2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %l = alloca i64, align 8
  store i64 7, i64* %l, align 4
  %l3 = load i64, i64* %l, align 4
  %cast4 = sitofp i64 %l3 to float
  %f2d5 = fpext float %cast4 to double
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.6, i32 0, i32 0), double %f2d5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %s = alloca i16, align 2
  store i16 5, i16* %s, align 2
  %s6 = load i16, i16* %s, align 2
  %cast7 = sitofp i16 %s6 to float
  %f2d8 = fpext float %cast7 to double
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.9, i32 0, i32 0), double %f2d8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %c = alloca i8, align 1
  store i8 65, i8* %c, align 1
  %c9 = load i8, i8* %c, align 1
  %cast10 = sitofp i8 %c9 to float
  %f2d11 = fpext float %cast10 to double
  %c12 = load i8, i8* %c, align 1
  %cast13 = sext i8 %c12 to i32
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.15, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.12, i32 0, i32 0), double %f2d11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.13, i32 0, i32 0), i32 %cast13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.14, i32 0, i32 0))
  %b = alloca i1, align 1
  store i1 true, i1* %b, align 1
  %b14 = load i1, i1* %b, align 1
  %cast15 = uitofp i1 %b14 to float
  %f2d16 = fpext float %cast15 to double
  %b17 = load i1, i1* %b, align 1
  %cast18 = zext i1 %b17 to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.19, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.16, i32 0, i32 0), double %f2d16, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.17, i32 0, i32 0), i32 %cast18, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.22, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.20, i32 0, i32 0), i64 9, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.25, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.23, i32 0, i32 0), double 4.000000e+00, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.24, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
