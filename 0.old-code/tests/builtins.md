# builtins

## index

- 001 — puts: simple messages, char comparison on hero grade
- 002 — proto printf: hello world with a format string
- 003 — typeof / sizeof on scalars
- 004 — typeof / sizeof on an array
- 005 — sizeof without parentheses
- 006 — typeof without parentheses
- 007 — default argument promotion in a user variadic call
- 008 — typeof and sizeof on chars, bool, and a literal
- 009 — custom putchar via write: print dungeon tiles
- 020 — sizeof of every scalar type
- 021 — sizeof of a struct honours ABI padding
- 022 — typeof of a struct gives its name

## 001 — puts: simple messages, char comparison on hero grade

```ura
// builtins/001.ura - puts: simple messages, char comparison on hero grade

proto fn puts(str pointer) i32

fn is_s_rank(grade char) bool:
    return grade == 'S'

main():
    grade char = 'S'
    if is_s_rank(grade):
        puts("S-rank hero — unstoppable")
    else:
        puts("keep training")
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

proto fn puts(str : pointer) : i32

fn is_s_rank(grade : char) : bool
└─ return
   └─ == : bool
      ├─ grade : char
      └─ char 'S'

fn main() : i32
├─ = : char
│  ├─ grade : char
│  └─ char 'S'
└─ if
   ├─ condition call is_s_rank : bool
   │  └─ grade : char
   ├─ call puts : i32
   │  └─ char[] "S-rank hero — unstoppable"
   └─ else
      └─ call puts : i32
         └─ char[] "keep training"
```

```out
S-rank hero — unstoppable
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [28 x i8] c"S-rank hero \E2\80\94 unstoppable\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"keep training\00", align 1

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

define i1 @is_s_rank(i8 %0) {
entry:
  %grade = alloca i8, align 1
  store i8 %0, i8* %grade, align 1
  %grade1 = load i8, i8* %grade, align 1
  %eq = icmp eq i8 %grade1, 83
  ret i1 %eq
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  %grade1 = load i8, i8* %grade, align 1
  %call = call i1 @is_s_rank(i8 %grade1)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %call2 = call i32 @puts(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %call3 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @puts(i8*)
```

## 002 — proto printf: hello world with a format string

```ura
// builtins/002.ura - proto printf: hello world with a format string

proto fn printf(fmt pointer, ...) i32

main():
    s char[] = "ura"
    printf("hello %s %d\n", s, 42)
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

proto fn printf(fmt : pointer, ...) : i32

fn main() : i32
├─ = : array
│  ├─ s : char[]
│  └─ char[] "ura"
├─ call printf : i32
│  ├─ char[] "hello %s %d\n"
│  ├─ s : char[]
│  └─ int 42
└─ return
   └─ int 0
```

```out
hello ura 42
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"hello %s %d\0A\00", align 1

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
  %s = alloca { i8*, i64 }, align 8
  store { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 3 }, { i8*, i64 }* %s, align 8
  %s1 = load { i8*, i64 }, { i8*, i64 }* %s, align 8
  %arr.data = extractvalue { i8*, i64 } %s1, 0
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i8* %arr.data, i32 42)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — typeof / sizeof on scalars

```ura
// builtins/003.ura - typeof / sizeof on scalars

main():
    x i32   = 5
    f f32 = 1.5
    output("typeof x = ", typeof(x), ", sizeof x = ", sizeof(x), "\n")
    output("typeof f = ", typeof(f), ", sizeof f = ", sizeof(f), "\n")
    output("sizeof i64 = ", sizeof(1 as i64), "\n")
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
│  └─ int 5
├─ = : f32
│  ├─ f : f32
│  └─ float 1.5
├─ output : void
│  ├─ char[] "typeof x = "
│  ├─ typeof : char[]
│  │  └─ x : i32
│  ├─ char[] ", sizeof x = "
│  ├─ sizeof : u64
│  │  └─ x : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "typeof f = "
│  ├─ typeof : char[]
│  │  └─ f : f32
│  ├─ char[] ", sizeof f = "
│  ├─ sizeof : u64
│  │  └─ f : f32
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "sizeof i64 = "
   ├─ sizeof : u64
   │  └─ cast : i64
   │     └─ int 1
   └─ char[] "\n"
```

```out
typeof x = i32, sizeof x = 4
typeof f = f32, sizeof f = 4
sizeof i64 = 8
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [12 x i8] c"typeof x = \00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"i32\00", align 1
@str.2 = private unnamed_addr constant [14 x i8] c", sizeof x = \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [21 x i8] c"%.*s%.*s%.*s%llu%.*s\00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c"typeof f = \00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"f32\00", align 1
@str.6 = private unnamed_addr constant [14 x i8] c", sizeof f = \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [21 x i8] c"%.*s%.*s%.*s%llu%.*s\00", align 1
@str.9 = private unnamed_addr constant [14 x i8] c"sizeof i64 = \00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [13 x i8] c"%.*s%llu%.*s\00", align 1

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
  store i32 5, i32* %x, align 4
  %f = alloca float, align 4
  store float 1.500000e+00, float* %f, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0), i64 4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt.8, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.6, i32 0, i32 0), i64 4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.11, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.9, i32 0, i32 0), i64 8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — typeof / sizeof on an array

```ura
// builtins/004.ura - typeof / sizeof on an array (fat-pointer struct)

main():
    a i32[] = [1, 2, 3]
    output("typeof a = ", typeof(a), ", sizeof a = ", sizeof(a), "\n")
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
├─ = : array
│  ├─ a : i32[]
│  └─ array : i32[]
│     ├─ int 1
│     ├─ int 2
│     └─ int 3
└─ output : void
   ├─ char[] "typeof a = "
   ├─ typeof : char[]
   │  └─ a : i32[]
   ├─ char[] ", sizeof a = "
   ├─ sizeof : u64
   │  └─ a : i32[]
   └─ char[] "\n"
```

```out
typeof a = array, sizeof a = 16
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [12 x i8] c"typeof a = \00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"array\00", align 1
@str.2 = private unnamed_addr constant [14 x i8] c", sizeof a = \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [21 x i8] c"%.*s%.*s%.*s%llu%.*s\00", align 1

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
  %a = alloca { i32*, i64 }, align 8
  %arr = alloca i32, i64 3, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 1, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 2, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 3, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 13, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0), i64 16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — sizeof without parentheses

```ura
main():
    x i32 = 5
    output(sizeof x)
```

```tree
```

```out
```

```err
error: Expected '(' after 'sizeof'
  005.ura:3:12
  |
3 |     output(sizeof x)
  |            ^^^^^^
error: Expected ')' after output arguments
  005.ura:3:5
  |
3 |     output(sizeof x)
  |     ^^^^^^
```

```ll
```

## 006 — typeof without parentheses

```ura
main():
    x i32 = 5
    output(typeof x)
```

```tree
```

```out
```

```err
error: Expected '(' after 'typeof'
  006.ura:3:12
  |
3 |     output(typeof x)
  |            ^^^^^^
error: Expected ')' after output arguments
  006.ura:3:5
  |
3 |     output(typeof x)
  |     ^^^^^^
```

```ll
```

## 007 — default argument promotion in a user variadic call

```ura
// builtins/007.ura - default argument promotion in a variadic call

proto fn printf(format pointer, ...) i32

main():
    f f32 = 3.5
    c char  = 'A'
    s i16 = 7 as i16
    b bool  = True
    l i64  = 123456789 as i64
    i i32   = 42

    printf("%.2f %c %d %d %d\n", f, c, s, b, i)
    printf("%ld\n", l)
    printf("%.2f %c %d\n", 2.5, 'Z', 9)
    printf("%.2f %d\n", f + f, i + 1)
    printf("%d %d\n", 1 < 2, 3 == 4)
    printf("none\n")
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

proto fn printf(format : pointer, ...) : i32

fn main() : i32
├─ = : f32
│  ├─ f : f32
│  └─ float 3.5
├─ = : char
│  ├─ c : char
│  └─ char 'A'
├─ = : i16
│  ├─ s : i16
│  └─ cast : i16
│     └─ int 7
├─ = : bool
│  ├─ b : bool
│  └─ bool True
├─ = : i64
│  ├─ l : i64
│  └─ cast : i64
│     └─ int 123456789
├─ = : i32
│  ├─ i : i32
│  └─ int 42
├─ call printf : i32
│  ├─ char[] "%.2f %c %d %d %d\n"
│  ├─ f : f32
│  ├─ c : char
│  ├─ s : i16
│  ├─ b : bool
│  └─ i : i32
├─ call printf : i32
│  ├─ char[] "%ld\n"
│  └─ l : i64
├─ call printf : i32
│  ├─ char[] "%.2f %c %d\n"
│  ├─ float 2.5
│  ├─ char 'Z'
│  └─ int 9
├─ call printf : i32
│  ├─ char[] "%.2f %d\n"
│  ├─ + : f32
│  │  ├─ f : f32
│  │  └─ f : f32
│  └─ + : i32
│     ├─ i : i32
│     └─ int 1
├─ call printf : i32
│  ├─ char[] "%d %d\n"
│  ├─ < : bool
│  │  ├─ int 1
│  │  └─ int 2
│  └─ == : bool
│     ├─ int 3
│     └─ int 4
└─ call printf : i32
   └─ char[] "none\n"
```

```out
3.50 A 7 1 42
123456789
2.50 Z 9
7.00 43
1 0
none
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [18 x i8] c"%.2f %c %d %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"%.2f %c %d\0A\00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"%.2f %d\0A\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"none\0A\00", align 1

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
  %f = alloca float, align 4
  store float 3.500000e+00, float* %f, align 4
  %c = alloca i8, align 1
  store i8 65, i8* %c, align 1
  %s = alloca i16, align 2
  store i16 7, i16* %s, align 2
  %b = alloca i1, align 1
  store i1 true, i1* %b, align 1
  %l = alloca i64, align 8
  store i64 123456789, i64* %l, align 8
  %i = alloca i32, align 4
  store i32 42, i32* %i, align 4
  %f1 = load float, float* %f, align 4
  %f2d = fpext float %f1 to double
  %c2 = load i8, i8* %c, align 1
  %i2i = sext i8 %c2 to i32
  %s3 = load i16, i16* %s, align 2
  %i2i4 = sext i16 %s3 to i32
  %b5 = load i1, i1* %b, align 1
  %u2i = zext i1 %b5 to i32
  %i6 = load i32, i32* %i, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), double %f2d, i32 %i2i, i32 %i2i4, i32 %u2i, i32 %i6)
  %l7 = load i64, i64* %l, align 8
  %call8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i64 %l7)
  %call9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), double 2.500000e+00, i32 90, i32 9)
  %f10 = load float, float* %f, align 4
  %f11 = load float, float* %f, align 4
  %fadd = fadd float %f10, %f11
  %f2d12 = fpext float %fadd to double
  %i13 = load i32, i32* %i, align 4
  %add = add i32 %i13, 1
  %call14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), double %f2d12, i32 %add)
  %call15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i32 1, i32 0)
  %call16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 008 — typeof and sizeof on chars, bool, and a literal

```ura
// builtins/008.ura - typeof/sizeof beyond the numeric types

main():
    s char[] = "hello"
    b bool  = True
    output(typeof(s), " ", typeof(b), "\n")
    output(sizeof("hello"), " ", sizeof(s), " ", sizeof(b), "\n")
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
├─ = : array
│  ├─ s : char[]
│  └─ char[] "hello"
├─ = : bool
│  ├─ b : bool
│  └─ bool True
├─ output : void
│  ├─ typeof : char[]
│  │  └─ s : char[]
│  ├─ char[] " "
│  ├─ typeof : char[]
│  │  └─ b : bool
│  └─ char[] "\n"
└─ output : void
   ├─ sizeof : u64
   │  └─ char[] "hello"
   ├─ char[] " "
   ├─ sizeof : u64
   │  └─ s : char[]
   ├─ char[] " "
   ├─ sizeof : u64
   │  └─ b : bool
   └─ char[] "\n"
```

```out
array bool
16 16 1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"array\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"bool\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%.*s%.*s%.*s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [25 x i8] c"%llu%.*s%llu%.*s%llu%.*s\00", align 1

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
  %s = alloca { i8*, i64 }, align 8
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 5 }, { i8*, i64 }* %s, align 8
  %b = alloca i1, align 1
  store i1 true, i1* %b, align 1
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @fmt.8, i32 0, i32 0), i64 16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0), i64 16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0), i64 1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 009 — custom putchar via write: print dungeon tiles

```ura
// builtins/009.ura - custom putchar via write: print dungeon tiles

fn putchar(c char) i32:
    buf char[] = [c, '\0']
    write(1, buf, 1)
    return 0

main():
    putchar('D')
    putchar('U')
    putchar('N')
    putchar('G')
    putchar('E')
    putchar('O')
    putchar('N')
    putchar('\n')
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

fn putchar(c : char) : i32
├─ = : array
│  ├─ buf : char[]
│  └─ array : char[]
│     ├─ c : char
│     └─ char ' '
├─ call write : i64
│  ├─ int 1
│  ├─ buf : char[]
│  └─ int 1
└─ return
   └─ int 0

fn main() : i32
├─ call putchar : i32
│  └─ char 'D'
├─ call putchar : i32
│  └─ char 'U'
├─ call putchar : i32
│  └─ char 'N'
├─ call putchar : i32
│  └─ char 'G'
├─ call putchar : i32
│  └─ char 'E'
├─ call putchar : i32
│  └─ char 'O'
├─ call putchar : i32
│  └─ char 'N'
├─ call putchar : i32
│  └─ char '\n'
└─ return
   └─ int 0
```

```out
DUNGEON
```

```err
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

define i32 @putchar(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %buf = alloca { i8*, i64 }, align 8
  %arr = alloca i8, i64 2, align 1
  %c1 = load i8, i8* %c, align 1
  %arr.init = getelementptr i8, i8* %arr, i64 0
  store i8 %c1, i8* %arr.init, align 1
  %arr.init2 = getelementptr i8, i8* %arr, i64 1
  store i8 0, i8* %arr.init2, align 1
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %arr, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 2, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %buf, align 8
  %buf3 = load { i8*, i64 }, { i8*, i64 }* %buf, align 8
  %arr.data = extractvalue { i8*, i64 } %buf3, 0
  %call = call i64 @write(i32 1, i8* %arr.data, i64 1)
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @putchar(i8 68)
  %call1 = call i32 @putchar(i8 85)
  %call2 = call i32 @putchar(i8 78)
  %call3 = call i32 @putchar(i8 71)
  %call4 = call i32 @putchar(i8 69)
  %call5 = call i32 @putchar(i8 79)
  %call6 = call i32 @putchar(i8 78)
  %call7 = call i32 @putchar(i8 10)
  ret i32 0
}
```

## 020 — sizeof of every scalar type

```ura
main():
    output(sizeof(i8), " ", sizeof(i16), " ", sizeof(i32), " ", sizeof(i64), "\n")
    output(sizeof(u8), " ", sizeof(u32), " ", sizeof(f32), " ", sizeof(f64), "\n")
    output(sizeof(bool), " ", sizeof(char), "\n")
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
├─ output : void
│  ├─ sizeof : u64
│  │  └─ i8 : i8
│  ├─ char[] " "
│  ├─ sizeof : u64
│  │  └─ short 0
│  ├─ char[] " "
│  ├─ sizeof : u64
│  │  └─ int 0
│  ├─ char[] " "
│  ├─ sizeof : u64
│  │  └─ long 0
│  └─ char[] "\n"
├─ output : void
│  ├─ sizeof : u64
│  │  └─ u8 : u8
│  ├─ char[] " "
│  ├─ sizeof : u64
│  │  └─ u32 : u32
│  ├─ char[] " "
│  ├─ sizeof : u64
│  │  └─ float 0
│  ├─ char[] " "
│  ├─ sizeof : u64
│  │  └─ f64 : f64
│  └─ char[] "\n"
└─ output : void
   ├─ sizeof : u64
   │  └─ bool False
   ├─ char[] " "
   ├─ sizeof : u64
   │  └─ char ' '
   └─ char[] "\n"
```

```out
1 2 4 8
1 4 4 8
1 1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [33 x i8] c"%llu%.*s%llu%.*s%llu%.*s%llu%.*s\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [33 x i8] c"%llu%.*s%llu%.*s%llu%.*s%llu%.*s\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [17 x i8] c"%llu%.*s%llu%.*s\00", align 1

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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @fmt, i32 0, i32 0), i64 1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i64 2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @fmt.8, i32 0, i32 0), i64 1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i64 4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0), i64 4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0), i64 8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.11, i32 0, i32 0), i64 1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0), i64 1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 021 — sizeof of a struct honours ABI padding

```ura
struct Point:
    x i32
    y i64

main():
    p Point
    output(sizeof(p), "\n")
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

struct Point
├─ x : i32
└─ y : i64

fn main() : i32
├─ p : STRUCT_CALL
└─ output : void
   ├─ sizeof : u64
   │  └─ p : STRUCT_CALL
   └─ char[] "\n"
```

```out
16
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Point = type { i32, i64 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%llu%.*s\00", align 1

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
  %p = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %p, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i64 16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 022 — typeof of a struct gives its name

```ura
struct Point:
    x i32
    y i64

main():
    p Point
    output(typeof(p), "\n")
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

struct Point
├─ x : i32
└─ y : i64

fn main() : i32
├─ p : STRUCT_CALL
└─ output : void
   ├─ typeof : char[]
   │  └─ p : STRUCT_CALL
   └─ char[] "\n"
```

```out
Point
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Point = type { i32, i64 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"Point\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1

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
  %p = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %p, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
