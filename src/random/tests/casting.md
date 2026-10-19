# casting

## index

- 001 — `as` casting + i64 / i16 via explicit cast
- 002 — `as` converts between f32 and the integer types
- 003 — char arithmetic and an expression cast back to char
- 004 — a struct cannot be cast to or from a scalar
- 005 — casting a scalar to a struct

## 001 — `as` casting + i64 / i16 via explicit cast

```ura
// casting/001.ura - `as` casting + i64 / i16 via explicit cast

main():
    a i32   = 300
    big i64  = a as i64
    small i16 = a as i16
    c i32   = big as i32
    output(big, "\n")
    output(small, "\n")
    output(c, "\n")
    output(5 as i64, "\n")
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
│  └─ char[] "\n"
├─ output : void
│  ├─ small : i16
│  └─ char[] "\n"
├─ output : void
│  ├─ c : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ cast : i64
│  │  └─ int 5
│  └─ char[] "\n"
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

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%lld%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [9 x i8] c"%lld%.*s\00", align 1

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
  store i32 300, i32* %a, align 4
  %big = alloca i64, align 8
  %a1 = load i32, i32* %a, align 4
  %cast = sext i32 %a1 to i64
  store i64 %cast, i64* %big, align 8
  %small = alloca i16, align 2
  %a2 = load i32, i32* %a, align 4
  %cast3 = trunc i32 %a2 to i16
  store i16 %cast3, i16* %small, align 2
  %c = alloca i32, align 4
  %big4 = load i64, i64* %big, align 8
  %cast5 = trunc i64 %big4 to i32
  store i32 %cast5, i32* %c, align 4
  %big6 = load i64, i64* %big, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i64 %big6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %small7 = load i16, i16* %small, align 2
  %s2i = sext i16 %small7 to i32
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i32 %s2i, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %c8 = load i32, i32* %c, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i32 %c8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.6, i32 0, i32 0), i64 5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — `as` converts between f32 and the integer types

```ura
// casting/002.ura - f32 <-> i32 casts

main():
    i i32   = 300
    f f32 = i as f32
    output("i32->f32 ", f, "\n")
    output("f32->i32 ", 2.9 as i32, " ", (0.0 - 2.9) as i32, "\n")
    l i64  = 7 as i64
    output("i64->f32 ", l as f32, "\n")
    s i16 = 5 as i16
    output("i16->f32 ", s as f32, "\n")
    c char  = 'A'
    output("char->f32 ", c as f32, " char->i32 ", c as i32, "\n")
    b bool  = True
    output("bool->f32 ", b as f32, " bool->i32 ", b as i32, "\n")
    output("f32->i64 ", 9.75 as i64, "\n")
    output("round trip ", 4.9 as i32 as f32, "\n")
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
│  ├─ i : i32
│  └─ int 300
├─ = : f32
│  ├─ f : f32
│  └─ cast : f32
│     └─ i : i32
├─ output : void
│  ├─ char[] "i32->f32 "
│  ├─ f : f32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "f32->i32 "
│  ├─ cast : i32
│  │  └─ float 2.9
│  ├─ char[] " "
│  ├─ cast : i32
│  │  └─ - : f32
│  │     ├─ float 0
│  │     └─ float 2.9
│  └─ char[] "\n"
├─ = : i64
│  ├─ l : i64
│  └─ cast : i64
│     └─ int 7
├─ output : void
│  ├─ char[] "i64->f32 "
│  ├─ cast : f32
│  │  └─ l : i64
│  └─ char[] "\n"
├─ = : i16
│  ├─ s : i16
│  └─ cast : i16
│     └─ int 5
├─ output : void
│  ├─ char[] "i16->f32 "
│  ├─ cast : f32
│  │  └─ s : i16
│  └─ char[] "\n"
├─ = : char
│  ├─ c : char
│  └─ char 'A'
├─ output : void
│  ├─ char[] "char->f32 "
│  ├─ cast : f32
│  │  └─ c : char
│  ├─ char[] " char->i32 "
│  ├─ cast : i32
│  │  └─ c : char
│  └─ char[] "\n"
├─ = : bool
│  ├─ b : bool
│  └─ bool True
├─ output : void
│  ├─ char[] "bool->f32 "
│  ├─ cast : f32
│  │  └─ b : bool
│  ├─ char[] " bool->i32 "
│  ├─ cast : i32
│  │  └─ b : bool
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "f32->i64 "
│  ├─ cast : i64
│  │  └─ float 9.75
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "round trip "
   ├─ cast : f32
   │  └─ cast : i32
   │     └─ float 4.9
   └─ char[] "\n"
```

```out
i32->f32 300.000000
f32->i32 2 -2
i64->f32 7.000000
i16->f32 5.000000
char->f32 65.000000 char->i32 65
bool->f32 1.000000 bool->i32 1
f32->i64 9
round trip 4.000000
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"i32->f32 \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"f32->i32 \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.6 = private unnamed_addr constant [10 x i8] c"i64->f32 \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1
@str.9 = private unnamed_addr constant [10 x i8] c"i16->f32 \00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1
@str.12 = private unnamed_addr constant [11 x i8] c"char->f32 \00", align 1
@str.13 = private unnamed_addr constant [12 x i8] c" char->i32 \00", align 1
@str.14 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.15 = private unnamed_addr constant [17 x i8] c"%.*s%f%.*s%d%.*s\00", align 1
@str.16 = private unnamed_addr constant [11 x i8] c"bool->f32 \00", align 1
@str.17 = private unnamed_addr constant [12 x i8] c" bool->i32 \00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.19 = private unnamed_addr constant [17 x i8] c"%.*s%f%.*s%d%.*s\00", align 1
@str.20 = private unnamed_addr constant [10 x i8] c"f32->i64 \00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [13 x i8] c"%.*s%lld%.*s\00", align 1
@str.23 = private unnamed_addr constant [12 x i8] c"round trip \00", align 1
@str.24 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.25 = private unnamed_addr constant [11 x i8] c"%.*s%f%.*s\00", align 1

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
  %i = alloca i32, align 4
  store i32 300, i32* %i, align 4
  %f = alloca float, align 4
  %i1 = load i32, i32* %i, align 4
  %cast = sitofp i32 %i1 to float
  store float %cast, float* %f, align 4
  %f2 = load float, float* %f, align 4
  %f2d = fpext float %f2 to double
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), double %f2d, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.5, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 -2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %l = alloca i64, align 8
  store i64 7, i64* %l, align 8
  %l3 = load i64, i64* %l, align 8
  %cast4 = sitofp i64 %l3 to float
  %f2d5 = fpext float %cast4 to double
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.8, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.6, i32 0, i32 0), double %f2d5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %s = alloca i16, align 2
  store i16 5, i16* %s, align 2
  %s6 = load i16, i16* %s, align 2
  %cast7 = sitofp i16 %s6 to float
  %f2d8 = fpext float %cast7 to double
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.11, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.9, i32 0, i32 0), double %f2d8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %c = alloca i8, align 1
  store i8 65, i8* %c, align 1
  %c9 = load i8, i8* %c, align 1
  %cast10 = sitofp i8 %c9 to float
  %f2d11 = fpext float %cast10 to double
  %c12 = load i8, i8* %c, align 1
  %cast13 = sext i8 %c12 to i32
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.15, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.12, i32 0, i32 0), double %f2d11, i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.13, i32 0, i32 0), i32 %cast13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.14, i32 0, i32 0))
  %b = alloca i1, align 1
  store i1 true, i1* %b, align 1
  %b14 = load i1, i1* %b, align 1
  %cast15 = uitofp i1 %b14 to float
  %f2d16 = fpext float %cast15 to double
  %b17 = load i1, i1* %b, align 1
  %cast18 = zext i1 %b17 to i32
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.19, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.16, i32 0, i32 0), double %f2d16, i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.17, i32 0, i32 0), i32 %cast18, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.22, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.20, i32 0, i32 0), i64 9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.25, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.23, i32 0, i32 0), double 4.000000e+00, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.24, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — char arithmetic and an expression cast back to char

```ura
// casting/003.ura - char <-> i32 round trip

main():
    grade char = 'A'
    code  i32  = grade as i32
    // an expression cast back to char
    back  char = (code + 1) as char
    output(grade, " ", code, " ", back, "\n")

    // arithmetic on two char literals
    diff i32 = 'd' as i32 - 'a' as i32
    output("'d' - 'a' = ", diff, "\n")

    // an expression used as an array index
    roll i32 = 7
    names char[][] = ["orc", "spider", "dragon"]
    output(names[roll % 3], "\n")
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
├─ = : char
│  ├─ grade : char
│  └─ char 'A'
├─ = : i32
│  ├─ code : i32
│  └─ cast : i32
│     └─ grade : char
├─ = : char
│  ├─ back : char
│  └─ cast : char
│     └─ + : i32
│        ├─ code : i32
│        └─ int 1
├─ output : void
│  ├─ grade : char
│  ├─ char[] " "
│  ├─ code : i32
│  ├─ char[] " "
│  ├─ back : char
│  └─ char[] "\n"
├─ = : i32
│  ├─ diff : i32
│  └─ - : i32
│     ├─ cast : i32
│     │  └─ char 'd'
│     └─ cast : i32
│        └─ char 'a'
├─ output : void
│  ├─ char[] "'d' - 'a' = "
│  ├─ diff : i32
│  └─ char[] "\n"
├─ = : i32
│  ├─ roll : i32
│  └─ int 7
├─ = : array
│  ├─ names : char[][]
│  └─ array : char[][]
│     ├─ char[] "orc"
│     ├─ char[] "spider"
│     └─ char[] "dragon"
└─ output : void
   ├─ index : char[]
   │  ├─ names : char[][]
   │  └─ % : i32
   │     ├─ roll : i32
   │     └─ int 3
   └─ char[] "\n"
```

```out
A 65 B
'd' - 'a' = 3
spider
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [19 x i8] c"%c%.*s%d%.*s%c%.*s\00", align 1
@str.3 = private unnamed_addr constant [13 x i8] c"'d' - 'a' = \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"orc\00", align 1
@str.7 = private unnamed_addr constant [7 x i8] c"spider\00", align 1
@str.8 = private unnamed_addr constant [7 x i8] c"dragon\00", align 1
@trap_msg = private unnamed_addr constant [182 x i8] c"runtime error: Modulo by zero\0A   003.ura:17:23\0A   |\0A17 |     output(names[roll % 3], \22\\n\22)\0A   |                       ^\0A\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1

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
  %grade = alloca i8, align 1
  store i8 65, i8* %grade, align 1
  %code = alloca i32, align 4
  %grade1 = load i8, i8* %grade, align 1
  %cast = sext i8 %grade1 to i32
  store i32 %cast, i32* %code, align 4
  %back = alloca i8, align 1
  %code2 = load i32, i32* %code, align 4
  %add = add i32 %code2, 1
  %cast3 = trunc i32 %add to i8
  store i8 %cast3, i8* %back, align 1
  %grade4 = load i8, i8* %grade, align 1
  %c2i = sext i8 %grade4 to i32
  %code5 = load i32, i32* %code, align 4
  %back6 = load i8, i8* %back, align 1
  %c2i7 = sext i8 %back6 to i32
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt, i32 0, i32 0), i32 %c2i, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %code5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %c2i7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %diff = alloca i32, align 4
  store i32 3, i32* %diff, align 4
  %diff8 = load i32, i32* %diff, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.3, i32 0, i32 0), i32 %diff8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %roll = alloca i32, align 4
  store i32 7, i32* %roll, align 4
  %names = alloca { { i8*, i64 }*, i64 }, align 8
  %arr = alloca { i8*, i64 }, i64 3, align 8
  %arr.init = getelementptr { i8*, i64 }, { i8*, i64 }* %arr, i64 0
  store { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i64 3 }, { i8*, i64 }* %arr.init, align 8
  %arr.init9 = getelementptr { i8*, i64 }, { i8*, i64 }* %arr, i64 1
  store { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i32 0, i32 0), i64 6 }, { i8*, i64 }* %arr.init9, align 8
  %arr.init10 = getelementptr { i8*, i64 }, { i8*, i64 }* %arr, i64 2
  store { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0), i64 6 }, { i8*, i64 }* %arr.init10, align 8
  %arr.ptr = insertvalue { { i8*, i64 }*, i64 } undef, { i8*, i64 }* %arr, 0
  %arr.len = insertvalue { { i8*, i64 }*, i64 } %arr.ptr, i64 3, 1
  store { { i8*, i64 }*, i64 } %arr.len, { { i8*, i64 }*, i64 }* %names, align 8
  %names11 = load { { i8*, i64 }*, i64 }, { { i8*, i64 }*, i64 }* %names, align 8
  %arr.data = extractvalue { { i8*, i64 }*, i64 } %names11, 0
  %roll12 = load i32, i32* %roll, align 4
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %4 = call i64 @write(i32 2, i8* getelementptr inbounds ([182 x i8], [182 x i8]* @trap_msg, i32 0, i32 0), i64 181)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %mod = srem i32 %roll12, 3
  %arr.at = getelementptr { i8*, i64 }, { i8*, i64 }* %arr.data, i32 %mod
  %idx = load { i8*, i64 }, { i8*, i64 }* %arr.at, align 8
  %str.len = extractvalue { i8*, i64 } %idx, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %idx, 0
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.10, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 004 — a struct cannot be cast to or from a scalar

```ura
// casting/004.ura - structs are not castable

struct Point:
    x i32
    y i32

main():
    p Point
    n i32 = p as i32
    output(n, "\n")
```

```tree
```

```out
```

```err
error: Cannot cast Point to i32
  004.ura:9:15
  |
9 |     n i32 = p as i32
  |               ^^
```

```ll
```

## 005 — casting a scalar to a struct

```ura
// casting/005.ura - the other direction

struct Point:
    x i32
    y i32

main():
    p Point = 5 as Point
    output(p.x, "\n")
```

```tree
```

```out
```

```err
error: Cannot cast i32 to Point
  005.ura:8:17
  |
8 |     p Point = 5 as Point
  |                 ^^
```

```ll
```
