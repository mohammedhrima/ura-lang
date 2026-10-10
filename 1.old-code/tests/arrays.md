# arrays

## index

- 001 — 1D i32 array: literal, index read, indexed write
- 002 — variable index and element arithmetic
- 003 — arrays of bool, char, f32
- 004 — sized constructor i32[N] with zero-init
- 005 — VLA: runtime-sized stack array
- 006 — 2D array literal and nested indexing
- 007 — sized 2D matrix i32[N][M], zero-init and writes
- 008 — 3D sized array with runtime size
- 009 — ? bounds guard: in-bounds access is fine
- 010 — ? bounds guard: out-of-bounds access traps at runtime
- 011 — heap array: new i32[N], zero-init, write, clean
- 012 — heap array with runtime size (new i32[n]) + clean
- 013 — clean nulls the array; a[i]? then traps (no use-after-free)
- 014 — multi-dim heap: new i32[N][M] + recursive clean
- 015 — slice a[1..4] is a view (shared storage, exclusive end)
- 016 — ? on a slice traps when the range is out of bounds
- 017 — arr.len on stack, heap, slice, and multi-dim
- 018 — indexing a non-array
- 019 — non-integer index
- 020 — array literal with mixed element types
- 021 — array size is not an integer
- 022 — clean on a non-array
- 023 — new without an array type
- 024 — missing closing bracket on index
- 025 — empty array literal has no element type
- 026 — range bounds must be integers
- 027 — .len on a non-array
- 028 — unknown member
- 029 — array literals: one element, expressions, and strings
- 030 — fill a heap char buffer by index, print it, clean it

## 001 — 1D i32 array: literal, index read, indexed write

```ura
// arrays/001.ura - 1D i32 array: literal, index read, indexed write

main():
    a i32[] = [10, 20, 30]
    output("a[0]=", a[0], " a[1]=", a[1], " a[2]=", a[2], "\n")
    a[1] = 99
    output("after a[1]=99: ", a[1], "\n")
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
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
├─ output : void
│  ├─ char[] "a[0]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 0
│  ├─ char[] " a[1]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 1
│  ├─ char[] " a[2]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 2
│  └─ char[] "\n"
├─ = : i32
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 1
│  └─ int 99
└─ output : void
   ├─ char[] "after a[1]=99: "
   ├─ index : i32
   │  ├─ a : i32[]
   │  └─ int 1
   └─ char[] "\n"
```

```out
a[0]=10 a[1]=20 a[2]=30
after a[1]=99: 99
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"a[0]=\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" a[1]=\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c" a[2]=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [23 x i8] c"%.*s%d%.*s%d%.*s%d%.*s\00", align 1
@str.4 = private unnamed_addr constant [16 x i8] c"after a[1]=99: \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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
  store i32 10, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 20, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 30, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %a3 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a3, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 0
  %idx = load i32, i32* %arr.at, align 4
  %a4 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data5 = extractvalue { i32*, i64 } %a4, 0
  %arr.at6 = getelementptr i32, i32* %arr.data5, i32 1
  %idx7 = load i32, i32* %arr.at6, align 4
  %a8 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data9 = extractvalue { i32*, i64 } %a8, 0
  %arr.at10 = getelementptr i32, i32* %arr.data9, i32 2
  %idx11 = load i32, i32* %arr.at10, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %idx7, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %idx11, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a12 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data13 = extractvalue { i32*, i64 } %a12, 0
  %arr.at14 = getelementptr i32, i32* %arr.data13, i32 1
  store i32 99, i32* %arr.at14, align 4
  %a15 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data16 = extractvalue { i32*, i64 } %a15, 0
  %arr.at17 = getelementptr i32, i32* %arr.data16, i32 1
  %idx18 = load i32, i32* %arr.at17, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.6, i32 0, i32 0), i32 15, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.4, i32 0, i32 0), i32 %idx18, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — variable index and element arithmetic

```ura
// arrays/002.ura - variable index and element arithmetic

main():
    a i32[] = [5, 10, 15, 20]
    i i32 = 3
    output("a[i]=", a[i], " a[i-1]=", a[i - 1], "\n")
    output("a[0]+a[3]=", a[0] + a[3], "\n")
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
│     ├─ int 5
│     ├─ int 10
│     ├─ int 15
│     └─ int 20
├─ = : i32
│  ├─ i : i32
│  └─ int 3
├─ output : void
│  ├─ char[] "a[i]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ i : i32
│  ├─ char[] " a[i-1]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ - : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "a[0]+a[3]="
   ├─ + : i32
   │  ├─ index : i32
   │  │  ├─ a : i32[]
   │  │  └─ int 0
   │  └─ index : i32
   │     ├─ a : i32[]
   │     └─ int 3
   └─ char[] "\n"
```

```out
a[i]=20 a[i-1]=15
a[0]+a[3]=25
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"a[i]=\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c" a[i-1]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"a[0]+a[3]=\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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
  %arr = alloca i32, i64 4, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 5, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 10, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 15, i32* %arr.init2, align 4
  %arr.init3 = getelementptr i32, i32* %arr, i64 3
  store i32 20, i32* %arr.init3, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 4, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %i = alloca i32, align 4
  store i32 3, i32* %i, align 4
  %a4 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i32, i32* %arr.data, i32 %i5
  %idx = load i32, i32* %arr.at, align 4
  %a6 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data7 = extractvalue { i32*, i64 } %a6, 0
  %i8 = load i32, i32* %i, align 4
  %sub = sub i32 %i8, 1
  %arr.at9 = getelementptr i32, i32* %arr.data7, i32 %sub
  %idx10 = load i32, i32* %arr.at9, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %idx10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %a11 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data12 = extractvalue { i32*, i64 } %a11, 0
  %arr.at13 = getelementptr i32, i32* %arr.data12, i32 0
  %idx14 = load i32, i32* %arr.at13, align 4
  %a15 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data16 = extractvalue { i32*, i64 } %a15, 0
  %arr.at17 = getelementptr i32, i32* %arr.data16, i32 3
  %idx18 = load i32, i32* %arr.at17, align 4
  %add = add i32 %idx14, %idx18
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %add, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — arrays of bool, char, f32

```ura
// arrays/003.ura - arrays of bool, char, f32

main():
    b bool[]  = [True, False, True]
    c char[]  = ['a', 'b', 'c']
    f f32[] = [1.5, 2.5, 3.0]
    output("b[0]=", b[0], " c[1]=", c[1], " f[2]=", f[2], "\n")
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
│  ├─ b : bool[]
│  └─ array : bool[]
│     ├─ bool True
│     ├─ bool False
│     └─ bool True
├─ = : array
│  ├─ c : char[]
│  └─ array : char[]
│     ├─ char 'a'
│     ├─ char 'b'
│     └─ char 'c'
├─ = : array
│  ├─ f : f32[]
│  └─ array : f32[]
│     ├─ float 1.5
│     ├─ float 2.5
│     └─ float 3
└─ output : void
   ├─ char[] "b[0]="
   ├─ index : bool
   │  ├─ b : bool[]
   │  └─ int 0
   ├─ char[] " c[1]="
   ├─ index : char
   │  ├─ c : char[]
   │  └─ int 1
   ├─ char[] " f[2]="
   ├─ index : f32
   │  ├─ f : f32[]
   │  └─ int 2
   └─ char[] "\n"
```

```out
b[0]=True c[1]=b f[2]=3.000000
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"b[0]=\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" c[1]=\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c" f[2]=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [23 x i8] c"%.*s%s%.*s%c%.*s%f%.*s\00", align 1

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
  %b = alloca { i1*, i64 }, align 8
  %arr = alloca i1, i64 3, align 1
  %arr.init = getelementptr i1, i1* %arr, i64 0
  store i1 true, i1* %arr.init, align 1
  %arr.init1 = getelementptr i1, i1* %arr, i64 1
  store i1 false, i1* %arr.init1, align 1
  %arr.init2 = getelementptr i1, i1* %arr, i64 2
  store i1 true, i1* %arr.init2, align 1
  %arr.ptr = insertvalue { i1*, i64 } undef, i1* %arr, 0
  %arr.len = insertvalue { i1*, i64 } %arr.ptr, i64 3, 1
  store { i1*, i64 } %arr.len, { i1*, i64 }* %b, align 8
  %c = alloca { i8*, i64 }, align 8
  %arr3 = alloca i8, i64 3, align 1
  %arr.init4 = getelementptr i8, i8* %arr3, i64 0
  store i8 97, i8* %arr.init4, align 1
  %arr.init5 = getelementptr i8, i8* %arr3, i64 1
  store i8 98, i8* %arr.init5, align 1
  %arr.init6 = getelementptr i8, i8* %arr3, i64 2
  store i8 99, i8* %arr.init6, align 1
  %arr.ptr7 = insertvalue { i8*, i64 } undef, i8* %arr3, 0
  %arr.len8 = insertvalue { i8*, i64 } %arr.ptr7, i64 3, 1
  store { i8*, i64 } %arr.len8, { i8*, i64 }* %c, align 8
  %f = alloca { float*, i64 }, align 8
  %arr9 = alloca float, i64 3, align 4
  %arr.init10 = getelementptr float, float* %arr9, i64 0
  store float 1.500000e+00, float* %arr.init10, align 4
  %arr.init11 = getelementptr float, float* %arr9, i64 1
  store float 2.500000e+00, float* %arr.init11, align 4
  %arr.init12 = getelementptr float, float* %arr9, i64 2
  store float 3.000000e+00, float* %arr.init12, align 4
  %arr.ptr13 = insertvalue { float*, i64 } undef, float* %arr9, 0
  %arr.len14 = insertvalue { float*, i64 } %arr.ptr13, i64 3, 1
  store { float*, i64 } %arr.len14, { float*, i64 }* %f, align 8
  %b15 = load { i1*, i64 }, { i1*, i64 }* %b, align 8
  %arr.data = extractvalue { i1*, i64 } %b15, 0
  %arr.at = getelementptr i1, i1* %arr.data, i32 0
  %idx = load i1, i1* %arr.at, align 1
  %bool_str = select i1 %idx, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %c16 = load { i8*, i64 }, { i8*, i64 }* %c, align 8
  %arr.data17 = extractvalue { i8*, i64 } %c16, 0
  %arr.at18 = getelementptr i8, i8* %arr.data17, i32 1
  %idx19 = load i8, i8* %arr.at18, align 1
  %c2i = sext i8 %idx19 to i32
  %f20 = load { float*, i64 }, { float*, i64 }* %f, align 8
  %arr.data21 = extractvalue { float*, i64 } %f20, 0
  %arr.at22 = getelementptr float, float* %arr.data21, i32 2
  %idx23 = load float, float* %arr.at22, align 4
  %f2d = fpext float %idx23 to double
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8* %bool_str, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %c2i, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), double %f2d, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — sized constructor i32[N] with zero-init

```ura
// arrays/004.ura - sized constructor i32[N] with zero-init

main():
    a i32[] = i32[5]
    output("zero-init: ", a[0], " ", a[4], "\n")
    a[2] = 42
    output("a[2]=", a[2], "\n")
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
│     └─ int 5
├─ output : void
│  ├─ char[] "zero-init: "
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 0
│  ├─ char[] " "
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 4
│  └─ char[] "\n"
├─ = : i32
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 2
│  └─ int 42
└─ output : void
   ├─ char[] "a[2]="
   ├─ index : i32
   │  ├─ a : i32[]
   │  └─ int 2
   └─ char[] "\n"
```

```out
zero-init: 0 0
a[2]=42
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [12 x i8] c"zero-init: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"a[2]=\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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
  %arr = alloca i32, i64 5, align 4
  %2 = bitcast i32* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 20, i1 false)
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 5, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %a1 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a1, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 0
  %idx = load i32, i32* %arr.at, align 4
  %a2 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data3 = extractvalue { i32*, i64 } %a2, 0
  %arr.at4 = getelementptr i32, i32* %arr.data3, i32 4
  %idx5 = load i32, i32* %arr.at4, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %idx, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %idx5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %a6 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data7 = extractvalue { i32*, i64 } %a6, 0
  %arr.at8 = getelementptr i32, i32* %arr.data7, i32 2
  store i32 42, i32* %arr.at8, align 4
  %a9 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data10 = extractvalue { i32*, i64 } %a9, 0
  %arr.at11 = getelementptr i32, i32* %arr.data10, i32 2
  %idx12 = load i32, i32* %arr.at11, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i32 %idx12, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i32 @printf(i8*, ...)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 005 — VLA: runtime-sized stack array

```ura
// arrays/005.ura - VLA: runtime-sized stack array

main():
    n i32 = 4
    a i32[] = i32[n * 2]
    a[7] = 100
    output("a[0]=", a[0], " a[7]=", a[7], "\n")
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
│  ├─ n : i32
│  └─ int 4
├─ = : array
│  ├─ a : i32[]
│  └─ array : i32[]
│     └─ * : i32
│        ├─ n : i32
│        └─ int 2
├─ = : i32
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 7
│  └─ int 100
└─ output : void
   ├─ char[] "a[0]="
   ├─ index : i32
   │  ├─ a : i32[]
   │  └─ int 0
   ├─ char[] " a[7]="
   ├─ index : i32
   │  ├─ a : i32[]
   │  └─ int 7
   └─ char[] "\n"
```

```out
a[0]=0 a[7]=100
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"a[0]=\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" a[7]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1

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
  %n = alloca i32, align 4
  store i32 4, i32* %n, align 4
  %a = alloca { i32*, i64 }, align 8
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 2
  %n2 = sext i32 %mul to i64
  %arr = alloca i32, i64 %n2, align 4
  %bytes = mul i64 %n2, 4
  %2 = bitcast i32* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 %bytes, i1 false)
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 %n2, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %a3 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a3, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 7
  store i32 100, i32* %arr.at, align 4
  %a4 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data5 = extractvalue { i32*, i64 } %a4, 0
  %arr.at6 = getelementptr i32, i32* %arr.data5, i32 0
  %idx = load i32, i32* %arr.at6, align 4
  %a7 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data8 = extractvalue { i32*, i64 } %a7, 0
  %arr.at9 = getelementptr i32, i32* %arr.data8, i32 7
  %idx10 = load i32, i32* %arr.at9, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %idx10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i32 @printf(i8*, ...)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 006 — 2D array literal and nested indexing

```ura
// arrays/006.ura - 2D array literal and nested indexing

main():
    m i32[][] = [[1, 2], [3, 4]]
    output("m[0][0]=", m[0][0], " m[1][1]=", m[1][1], "\n")
    m[0][1] = 99
    output("m[0][1]=", m[0][1], "\n")
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
│  ├─ m : i32[][]
│  └─ array : i32[][]
│     ├─ array : i32[]
│     │  ├─ int 1
│     │  └─ int 2
│     └─ array : i32[]
│        ├─ int 3
│        └─ int 4
├─ output : void
│  ├─ char[] "m[0][0]="
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ m : i32[][]
│  │  │  └─ int 0
│  │  └─ int 0
│  ├─ char[] " m[1][1]="
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ m : i32[][]
│  │  │  └─ int 1
│  │  └─ int 1
│  └─ char[] "\n"
├─ = : i32
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ m : i32[][]
│  │  │  └─ int 0
│  │  └─ int 1
│  └─ int 99
└─ output : void
   ├─ char[] "m[0][1]="
   ├─ index : i32
   │  ├─ index : i32[]
   │  │  ├─ m : i32[][]
   │  │  └─ int 0
   │  └─ int 1
   └─ char[] "\n"
```

```out
m[0][0]=1 m[1][1]=4
m[0][1]=99
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"m[0][0]=\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c" m[1][1]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"m[0][1]=\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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
  %m = alloca { { i32*, i64 }*, i64 }, align 8
  %arr = alloca { i32*, i64 }, i64 2, align 8
  %arr1 = alloca i32, i64 2, align 4
  %arr.init = getelementptr i32, i32* %arr1, i64 0
  store i32 1, i32* %arr.init, align 4
  %arr.init2 = getelementptr i32, i32* %arr1, i64 1
  store i32 2, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr1, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 2, 1
  %arr.init3 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr, i64 0
  store { i32*, i64 } %arr.len, { i32*, i64 }* %arr.init3, align 8
  %arr4 = alloca i32, i64 2, align 4
  %arr.init5 = getelementptr i32, i32* %arr4, i64 0
  store i32 3, i32* %arr.init5, align 4
  %arr.init6 = getelementptr i32, i32* %arr4, i64 1
  store i32 4, i32* %arr.init6, align 4
  %arr.ptr7 = insertvalue { i32*, i64 } undef, i32* %arr4, 0
  %arr.len8 = insertvalue { i32*, i64 } %arr.ptr7, i64 2, 1
  %arr.init9 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr, i64 1
  store { i32*, i64 } %arr.len8, { i32*, i64 }* %arr.init9, align 8
  %arr.ptr10 = insertvalue { { i32*, i64 }*, i64 } undef, { i32*, i64 }* %arr, 0
  %arr.len11 = insertvalue { { i32*, i64 }*, i64 } %arr.ptr10, i64 2, 1
  store { { i32*, i64 }*, i64 } %arr.len11, { { i32*, i64 }*, i64 }* %m, align 8
  %m12 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data = extractvalue { { i32*, i64 }*, i64 } %m12, 0
  %arr.at = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data, i32 0
  %idx = load { i32*, i64 }, { i32*, i64 }* %arr.at, align 8
  %arr.data13 = extractvalue { i32*, i64 } %idx, 0
  %arr.at14 = getelementptr i32, i32* %arr.data13, i32 0
  %idx15 = load i32, i32* %arr.at14, align 4
  %m16 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data17 = extractvalue { { i32*, i64 }*, i64 } %m16, 0
  %arr.at18 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data17, i32 1
  %idx19 = load { i32*, i64 }, { i32*, i64 }* %arr.at18, align 8
  %arr.data20 = extractvalue { i32*, i64 } %idx19, 0
  %arr.at21 = getelementptr i32, i32* %arr.data20, i32 1
  %idx22 = load i32, i32* %arr.at21, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %idx15, i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %idx22, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %m23 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data24 = extractvalue { { i32*, i64 }*, i64 } %m23, 0
  %arr.at25 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data24, i32 0
  %idx26 = load { i32*, i64 }, { i32*, i64 }* %arr.at25, align 8
  %arr.data27 = extractvalue { i32*, i64 } %idx26, 0
  %arr.at28 = getelementptr i32, i32* %arr.data27, i32 1
  store i32 99, i32* %arr.at28, align 4
  %m29 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data30 = extractvalue { { i32*, i64 }*, i64 } %m29, 0
  %arr.at31 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data30, i32 0
  %idx32 = load { i32*, i64 }, { i32*, i64 }* %arr.at31, align 8
  %arr.data33 = extractvalue { i32*, i64 } %idx32, 0
  %arr.at34 = getelementptr i32, i32* %arr.data33, i32 1
  %idx35 = load i32, i32* %arr.at34, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i32 %idx35, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — sized 2D matrix i32[N][M], zero-init and writes

```ura
// arrays/007.ura - sized 2D matrix i32[N][M], zero-init and writes

main():
    g i32[][] = i32[3][4]
    g[0][0] = 1
    g[2][3] = 99
    output("g[0][0]=", g[0][0], " g[2][3]=", g[2][3], " g[1][1]=", g[1][1], "\n")
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
│  ├─ g : i32[][]
│  └─ array : i32[][]
│     ├─ int 3
│     └─ int 4
├─ = : i32
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ g : i32[][]
│  │  │  └─ int 0
│  │  └─ int 0
│  └─ int 1
├─ = : i32
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ g : i32[][]
│  │  │  └─ int 2
│  │  └─ int 3
│  └─ int 99
└─ output : void
   ├─ char[] "g[0][0]="
   ├─ index : i32
   │  ├─ index : i32[]
   │  │  ├─ g : i32[][]
   │  │  └─ int 0
   │  └─ int 0
   ├─ char[] " g[2][3]="
   ├─ index : i32
   │  ├─ index : i32[]
   │  │  ├─ g : i32[][]
   │  │  └─ int 2
   │  └─ int 3
   ├─ char[] " g[1][1]="
   ├─ index : i32
   │  ├─ index : i32[]
   │  │  ├─ g : i32[][]
   │  │  └─ int 1
   │  └─ int 1
   └─ char[] "\n"
```

```out
g[0][0]=1 g[2][3]=99 g[1][1]=0
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"g[0][0]=\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c" g[2][3]=\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c" g[1][1]=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [23 x i8] c"%.*s%d%.*s%d%.*s%d%.*s\00", align 1

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
  %g = alloca { { i32*, i64 }*, i64 }, align 8
  %arr = alloca { i32*, i64 }, i64 3, align 8
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 8
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i1 = load i64, i64* %i, align 8
  %more = icmp slt i64 %i1, 3
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr2 = alloca i32, i64 4, align 4
  %2 = bitcast i32* %arr2 to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 16, i1 false)
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr2, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 4, 1
  %i3 = load i64, i64* %i, align 8
  %arr.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr, i64 %i3
  store { i32*, i64 } %arr.len, { i32*, i64 }* %arr.slot, align 8
  %next = add i64 %i3, 1
  store i64 %next, i64* %i, align 8
  br label %arr.cond

arr.end:                                          ; preds = %arr.cond
  %arr.ptr4 = insertvalue { { i32*, i64 }*, i64 } undef, { i32*, i64 }* %arr, 0
  %arr.len5 = insertvalue { { i32*, i64 }*, i64 } %arr.ptr4, i64 3, 1
  store { { i32*, i64 }*, i64 } %arr.len5, { { i32*, i64 }*, i64 }* %g, align 8
  %g6 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %g, align 8
  %arr.data = extractvalue { { i32*, i64 }*, i64 } %g6, 0
  %arr.at = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data, i32 0
  %idx = load { i32*, i64 }, { i32*, i64 }* %arr.at, align 8
  %arr.data7 = extractvalue { i32*, i64 } %idx, 0
  %arr.at8 = getelementptr i32, i32* %arr.data7, i32 0
  store i32 1, i32* %arr.at8, align 4
  %g9 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %g, align 8
  %arr.data10 = extractvalue { { i32*, i64 }*, i64 } %g9, 0
  %arr.at11 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data10, i32 2
  %idx12 = load { i32*, i64 }, { i32*, i64 }* %arr.at11, align 8
  %arr.data13 = extractvalue { i32*, i64 } %idx12, 0
  %arr.at14 = getelementptr i32, i32* %arr.data13, i32 3
  store i32 99, i32* %arr.at14, align 4
  %g15 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %g, align 8
  %arr.data16 = extractvalue { { i32*, i64 }*, i64 } %g15, 0
  %arr.at17 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data16, i32 0
  %idx18 = load { i32*, i64 }, { i32*, i64 }* %arr.at17, align 8
  %arr.data19 = extractvalue { i32*, i64 } %idx18, 0
  %arr.at20 = getelementptr i32, i32* %arr.data19, i32 0
  %idx21 = load i32, i32* %arr.at20, align 4
  %g22 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %g, align 8
  %arr.data23 = extractvalue { { i32*, i64 }*, i64 } %g22, 0
  %arr.at24 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data23, i32 2
  %idx25 = load { i32*, i64 }, { i32*, i64 }* %arr.at24, align 8
  %arr.data26 = extractvalue { i32*, i64 } %idx25, 0
  %arr.at27 = getelementptr i32, i32* %arr.data26, i32 3
  %idx28 = load i32, i32* %arr.at27, align 4
  %g29 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %g, align 8
  %arr.data30 = extractvalue { { i32*, i64 }*, i64 } %g29, 0
  %arr.at31 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data30, i32 1
  %idx32 = load { i32*, i64 }, { i32*, i64 }* %arr.at31, align 8
  %arr.data33 = extractvalue { i32*, i64 } %idx32, 0
  %arr.at34 = getelementptr i32, i32* %arr.data33, i32 1
  %idx35 = load i32, i32* %arr.at34, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %idx21, i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %idx28, i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %idx35, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i32 @printf(i8*, ...)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 008 — 3D sized array with runtime size

```ura
// arrays/008.ura - 3D sized array with runtime size

main():
    n i32 = 2
    v i32[][][] = i32[n][n][n]
    v[1][1][1] = 7
    output("v[1][1][1]=", v[1][1][1], " v[0][0][0]=", v[0][0][0], "\n")
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
│  ├─ n : i32
│  └─ int 2
├─ = : array
│  ├─ v : i32[][][]
│  └─ array : i32[][][]
│     ├─ n : i32
│     ├─ n : i32
│     └─ n : i32
├─ = : i32
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ index : i32[][]
│  │  │  │  ├─ v : i32[][][]
│  │  │  │  └─ int 1
│  │  │  └─ int 1
│  │  └─ int 1
│  └─ int 7
└─ output : void
   ├─ char[] "v[1][1][1]="
   ├─ index : i32
   │  ├─ index : i32[]
   │  │  ├─ index : i32[][]
   │  │  │  ├─ v : i32[][][]
   │  │  │  └─ int 1
   │  │  └─ int 1
   │  └─ int 1
   ├─ char[] " v[0][0][0]="
   ├─ index : i32
   │  ├─ index : i32[]
   │  │  ├─ index : i32[][]
   │  │  │  ├─ v : i32[][][]
   │  │  │  └─ int 0
   │  │  └─ int 0
   │  └─ int 0
   └─ char[] "\n"
```

```out
v[1][1][1]=7 v[0][0][0]=0
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [12 x i8] c"v[1][1][1]=\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c" v[0][0][0]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1

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
  %n = alloca i32, align 4
  store i32 2, i32* %n, align 4
  %v = alloca { { { i32*, i64 }*, i64 }*, i64 }, align 8
  %n1 = load i32, i32* %n, align 4
  %n2 = sext i32 %n1 to i64
  %n3 = load i32, i32* %n, align 4
  %n4 = sext i32 %n3 to i64
  %n5 = load i32, i32* %n, align 4
  %n6 = sext i32 %n5 to i64
  %arr = alloca { { i32*, i64 }*, i64 }, i64 %n2, align 8
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 8
  br label %arr.cond

arr.cond:                                         ; preds = %arr.end12, %entry
  %i7 = load i64, i64* %i, align 8
  %more = icmp slt i64 %i7, %n2
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr8 = alloca { i32*, i64 }, i64 %n4, align 8
  %i9 = alloca i64, align 8
  store i64 0, i64* %i9, align 8
  br label %arr.cond10

arr.end:                                          ; preds = %arr.cond
  %arr.ptr22 = insertvalue { { { i32*, i64 }*, i64 }*, i64 } undef, { { i32*, i64 }*, i64 }* %arr, 0
  %arr.len23 = insertvalue { { { i32*, i64 }*, i64 }*, i64 } %arr.ptr22, i64 %n2, 1
  store { { { i32*, i64 }*, i64 }*, i64 } %arr.len23, { { { i32*, i64 }*, i64 }*, i64 }* %v, align 8
  %v24 = load { { { i32*, i64 }*, i64 }*, i64 }, { { { i32*, i64 }*, i64 }*, i64 }* %v, align 8
  %arr.data = extractvalue { { { i32*, i64 }*, i64 }*, i64 } %v24, 0
  %arr.at = getelementptr { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr.data, i32 1
  %idx = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr.at, align 8
  %arr.data25 = extractvalue { { i32*, i64 }*, i64 } %idx, 0
  %arr.at26 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data25, i32 1
  %idx27 = load { i32*, i64 }, { i32*, i64 }* %arr.at26, align 8
  %arr.data28 = extractvalue { i32*, i64 } %idx27, 0
  %arr.at29 = getelementptr i32, i32* %arr.data28, i32 1
  store i32 7, i32* %arr.at29, align 4
  %v30 = load { { { i32*, i64 }*, i64 }*, i64 }, { { { i32*, i64 }*, i64 }*, i64 }* %v, align 8
  %arr.data31 = extractvalue { { { i32*, i64 }*, i64 }*, i64 } %v30, 0
  %arr.at32 = getelementptr { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr.data31, i32 1
  %idx33 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr.at32, align 8
  %arr.data34 = extractvalue { { i32*, i64 }*, i64 } %idx33, 0
  %arr.at35 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data34, i32 1
  %idx36 = load { i32*, i64 }, { i32*, i64 }* %arr.at35, align 8
  %arr.data37 = extractvalue { i32*, i64 } %idx36, 0
  %arr.at38 = getelementptr i32, i32* %arr.data37, i32 1
  %idx39 = load i32, i32* %arr.at38, align 4
  %v40 = load { { { i32*, i64 }*, i64 }*, i64 }, { { { i32*, i64 }*, i64 }*, i64 }* %v, align 8
  %arr.data41 = extractvalue { { { i32*, i64 }*, i64 }*, i64 } %v40, 0
  %arr.at42 = getelementptr { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr.data41, i32 0
  %idx43 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr.at42, align 8
  %arr.data44 = extractvalue { { i32*, i64 }*, i64 } %idx43, 0
  %arr.at45 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data44, i32 0
  %idx46 = load { i32*, i64 }, { i32*, i64 }* %arr.at45, align 8
  %arr.data47 = extractvalue { i32*, i64 } %idx46, 0
  %arr.at48 = getelementptr i32, i32* %arr.data47, i32 0
  %idx49 = load i32, i32* %arr.at48, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %idx39, i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i32 %idx49, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0

arr.cond10:                                       ; preds = %arr.body11, %arr.body
  %i13 = load i64, i64* %i9, align 8
  %more14 = icmp slt i64 %i13, %n4
  br i1 %more14, label %arr.body11, label %arr.end12

arr.body11:                                       ; preds = %arr.cond10
  %arr15 = alloca i32, i64 %n6, align 4
  %bytes = mul i64 %n6, 4
  %3 = bitcast i32* %arr15 to i8*
  call void @llvm.memset.p0i8.i64(i8* %3, i8 0, i64 %bytes, i1 false)
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr15, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 %n6, 1
  %i16 = load i64, i64* %i9, align 8
  %arr.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr8, i64 %i16
  store { i32*, i64 } %arr.len, { i32*, i64 }* %arr.slot, align 8
  %next = add i64 %i16, 1
  store i64 %next, i64* %i9, align 8
  br label %arr.cond10

arr.end12:                                        ; preds = %arr.cond10
  %arr.ptr17 = insertvalue { { i32*, i64 }*, i64 } undef, { i32*, i64 }* %arr8, 0
  %arr.len18 = insertvalue { { i32*, i64 }*, i64 } %arr.ptr17, i64 %n4, 1
  %i19 = load i64, i64* %i, align 8
  %arr.slot20 = getelementptr { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr, i64 %i19
  store { { i32*, i64 }*, i64 } %arr.len18, { { i32*, i64 }*, i64 }* %arr.slot20, align 8
  %next21 = add i64 %i19, 1
  store i64 %next21, i64* %i, align 8
  br label %arr.cond
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i32 @printf(i8*, ...)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 009 — ? bounds guard: in-bounds access is fine

```ura
// arrays/009.ura - ? bounds guard: in-bounds access is fine

main():
    a i32[] = [10, 20, 30]
    i i32 = 2
    output("a[i]?=", a[i]?, "\n")
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
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
├─ = : i32
│  ├─ i : i32
│  └─ int 2
└─ output : void
   ├─ char[] "a[i]?="
   ├─ index : i32
   │  ├─ a : i32[]
   │  └─ i : i32
   └─ char[] "\n"
```

```out
a[i]?=30
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"a[i]?=\00", align 1
@trap_msg = private unnamed_addr constant [188 x i8] c"runtime error: array index out of bounds\0A  009.ura:6:23\0A  |\0A6 |     output(\22a[i]?=\22, a[i]?, \22\\n\22)\0A  |                       ^\0A\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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
  store i32 10, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 20, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 30, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %i = alloca i32, align 4
  store i32 2, i32* %i, align 4
  %a3 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a3, 0
  %i4 = load i32, i32* %i, align 4
  %arr.len5 = extractvalue { i32*, i64 } %a3, 1
  %idx = sext i32 %i4 to i64
  %oob.low = icmp slt i64 %idx, 0
  %oob.high = icmp sge i64 %idx, %arr.len5
  %oob = or i1 %oob.low, %oob.high
  br i1 %oob, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([188 x i8], [188 x i8]* @trap_msg, i32 0, i32 0), i64 187)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %arr.at = getelementptr i32, i32* %arr.data, i32 %i4
  %idx6 = load i32, i32* %arr.at, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %idx6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 010 — ? bounds guard: out-of-bounds access traps at runtime

```ura
// arrays/010.ura - ? bounds guard: out-of-bounds access traps at runtime

main():
    a i32[] = [10, 20, 30]
    output(a[5]?)
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
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
└─ output : void
   └─ index : i32
      ├─ a : i32[]
      └─ int 5
```

```out
```

```err
runtime error: array index out of bounds
  010.ura:5:13
  |
5 |     output(a[5]?)
  |             ^
exit: 1
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [162 x i8] c"runtime error: array index out of bounds\0A  010.ura:5:13\0A  |\0A5 |     output(a[5]?)\0A  |             ^\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%d\00", align 1

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
  store i32 10, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 20, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 30, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %a3 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a3, 0
  %arr.len4 = extractvalue { i32*, i64 } %a3, 1
  %oob.high = icmp sge i64 5, %arr.len4
  %oob = or i1 false, %oob.high
  br i1 %oob, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([162 x i8], [162 x i8]* @trap_msg, i32 0, i32 0), i64 161)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %arr.at = getelementptr i32, i32* %arr.data, i32 5
  %idx = load i32, i32* %arr.at, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i32 %idx)
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 011 — heap array: new i32[N], zero-init, write, clean

```ura
// arrays/011.ura - heap array: new i32[N], zero-init, write, clean

main():
    a i32[] = new i32[5]
    a[2] = 42
    output("a[0]=", a[0], " a[2]=", a[2], "\n")
    clean a
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
│     └─ int 5
├─ = : i32
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 2
│  └─ int 42
├─ output : void
│  ├─ char[] "a[0]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 0
│  ├─ char[] " a[2]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 2
│  └─ char[] "\n"
└─ clean : void
   └─ a : i32[]
```

```out
a[0]=0 a[2]=42
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"a[0]=\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" a[2]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1

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
  %heap = call i8* @calloc(i64 5, i64 4)
  %arr = bitcast i8* %heap to i32*
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 5, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %a1 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a1, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 2
  store i32 42, i32* %arr.at, align 4
  %a2 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data3 = extractvalue { i32*, i64 } %a2, 0
  %arr.at4 = getelementptr i32, i32* %arr.data3, i32 0
  %idx = load i32, i32* %arr.at4, align 4
  %a5 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data6 = extractvalue { i32*, i64 } %a5, 0
  %arr.at7 = getelementptr i32, i32* %arr.data6, i32 2
  %idx8 = load i32, i32* %arr.at7, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %idx8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %arr9 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data10 = extractvalue { i32*, i64 } %arr9, 0
  %free.ptr = bitcast i32* %arr.data10 to i8*
  call void @free(i8* %free.ptr)
  store { i32*, i64 } zeroinitializer, { i32*, i64 }* %a, align 8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```

## 012 — heap array with runtime size (new i32[n]) + clean

```ura
// arrays/012.ura - heap array with runtime size (new i32[n]) + clean

main():
    n i32 = 3
    a i32[] = new i32[n * 2]
    a[5] = 7
    output("a[5]=", a[5], " a[0]=", a[0], "\n")
    clean a
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
│  ├─ n : i32
│  └─ int 3
├─ = : array
│  ├─ a : i32[]
│  └─ array : i32[]
│     └─ * : i32
│        ├─ n : i32
│        └─ int 2
├─ = : i32
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 5
│  └─ int 7
├─ output : void
│  ├─ char[] "a[5]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 5
│  ├─ char[] " a[0]="
│  ├─ index : i32
│  │  ├─ a : i32[]
│  │  └─ int 0
│  └─ char[] "\n"
└─ clean : void
   └─ a : i32[]
```

```out
a[5]=7 a[0]=0
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"a[5]=\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" a[0]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1

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
  %n = alloca i32, align 4
  store i32 3, i32* %n, align 4
  %a = alloca { i32*, i64 }, align 8
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 2
  %n2 = sext i32 %mul to i64
  %heap = call i8* @calloc(i64 %n2, i64 4)
  %arr = bitcast i8* %heap to i32*
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 %n2, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %a3 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a3, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 5
  store i32 7, i32* %arr.at, align 4
  %a4 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data5 = extractvalue { i32*, i64 } %a4, 0
  %arr.at6 = getelementptr i32, i32* %arr.data5, i32 5
  %idx = load i32, i32* %arr.at6, align 4
  %a7 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data8 = extractvalue { i32*, i64 } %a7, 0
  %arr.at9 = getelementptr i32, i32* %arr.data8, i32 0
  %idx10 = load i32, i32* %arr.at9, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %idx10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %arr11 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data12 = extractvalue { i32*, i64 } %arr11, 0
  %free.ptr = bitcast i32* %arr.data12 to i8*
  call void @free(i8* %free.ptr)
  store { i32*, i64 } zeroinitializer, { i32*, i64 }* %a, align 8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```

## 013 — clean nulls the array; a[i]? then traps (no use-after-free)

```ura
// arrays/013.ura - clean nulls the array; a[i]? then traps instead of use-after-free

main():
    a i32[] = new i32[5]
    clean a
    output(a[0]?)
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
│     └─ int 5
├─ clean : void
│  └─ a : i32[]
└─ output : void
   └─ index : i32
      ├─ a : i32[]
      └─ int 0
```

```out
```

```err
runtime error: array index out of bounds
  013.ura:6:13
  |
6 |     output(a[0]?)
  |             ^
exit: 1
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [162 x i8] c"runtime error: array index out of bounds\0A  013.ura:6:13\0A  |\0A6 |     output(a[0]?)\0A  |             ^\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%d\00", align 1

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
  %heap = call i8* @calloc(i64 5, i64 4)
  %arr = bitcast i8* %heap to i32*
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 5, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %arr1 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %arr1, 0
  %free.ptr = bitcast i32* %arr.data to i8*
  call void @free(i8* %free.ptr)
  store { i32*, i64 } zeroinitializer, { i32*, i64 }* %a, align 8
  %a2 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data3 = extractvalue { i32*, i64 } %a2, 0
  %arr.len4 = extractvalue { i32*, i64 } %a2, 1
  %oob.high = icmp sge i64 0, %arr.len4
  %oob = or i1 false, %oob.high
  br i1 %oob, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([162 x i8], [162 x i8]* @trap_msg, i32 0, i32 0), i64 161)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %arr.at = getelementptr i32, i32* %arr.data3, i32 0
  %idx = load i32, i32* %arr.at, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i32 %idx)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 014 — multi-dim heap: new i32[N][M] + recursive clean

```ura
// arrays/014.ura - multi-dim heap: new i32[N][M] + recursive clean

main():
    m i32[][] = new i32[2][3]
    m[0][0] = 1
    m[1][2] = 9
    output("m[0][0]=", m[0][0], " m[1][2]=", m[1][2], " m[0][1]=", m[0][1], "\n")
    clean m
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
│  ├─ m : i32[][]
│  └─ array : i32[][]
│     ├─ int 2
│     └─ int 3
├─ = : i32
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ m : i32[][]
│  │  │  └─ int 0
│  │  └─ int 0
│  └─ int 1
├─ = : i32
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ m : i32[][]
│  │  │  └─ int 1
│  │  └─ int 2
│  └─ int 9
├─ output : void
│  ├─ char[] "m[0][0]="
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ m : i32[][]
│  │  │  └─ int 0
│  │  └─ int 0
│  ├─ char[] " m[1][2]="
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ m : i32[][]
│  │  │  └─ int 1
│  │  └─ int 2
│  ├─ char[] " m[0][1]="
│  ├─ index : i32
│  │  ├─ index : i32[]
│  │  │  ├─ m : i32[][]
│  │  │  └─ int 0
│  │  └─ int 1
│  └─ char[] "\n"
└─ clean : void
   └─ m : i32[][]
```

```out
m[0][0]=1 m[1][2]=9 m[0][1]=0
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"m[0][0]=\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c" m[1][2]=\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c" m[0][1]=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [23 x i8] c"%.*s%d%.*s%d%.*s%d%.*s\00", align 1

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
  %m = alloca { { i32*, i64 }*, i64 }, align 8
  %heap = call i8* @calloc(i64 2, i64 16)
  %arr = bitcast i8* %heap to { i32*, i64 }*
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 8
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i1 = load i64, i64* %i, align 8
  %more = icmp slt i64 %i1, 2
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %heap2 = call i8* @calloc(i64 3, i64 4)
  %arr3 = bitcast i8* %heap2 to i32*
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr3, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  %i4 = load i64, i64* %i, align 8
  %arr.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr, i64 %i4
  store { i32*, i64 } %arr.len, { i32*, i64 }* %arr.slot, align 8
  %next = add i64 %i4, 1
  store i64 %next, i64* %i, align 8
  br label %arr.cond

arr.end:                                          ; preds = %arr.cond
  %arr.ptr5 = insertvalue { { i32*, i64 }*, i64 } undef, { i32*, i64 }* %arr, 0
  %arr.len6 = insertvalue { { i32*, i64 }*, i64 } %arr.ptr5, i64 2, 1
  store { { i32*, i64 }*, i64 } %arr.len6, { { i32*, i64 }*, i64 }* %m, align 8
  %m7 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data = extractvalue { { i32*, i64 }*, i64 } %m7, 0
  %arr.at = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data, i32 0
  %idx = load { i32*, i64 }, { i32*, i64 }* %arr.at, align 8
  %arr.data8 = extractvalue { i32*, i64 } %idx, 0
  %arr.at9 = getelementptr i32, i32* %arr.data8, i32 0
  store i32 1, i32* %arr.at9, align 4
  %m10 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data11 = extractvalue { { i32*, i64 }*, i64 } %m10, 0
  %arr.at12 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data11, i32 1
  %idx13 = load { i32*, i64 }, { i32*, i64 }* %arr.at12, align 8
  %arr.data14 = extractvalue { i32*, i64 } %idx13, 0
  %arr.at15 = getelementptr i32, i32* %arr.data14, i32 2
  store i32 9, i32* %arr.at15, align 4
  %m16 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data17 = extractvalue { { i32*, i64 }*, i64 } %m16, 0
  %arr.at18 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data17, i32 0
  %idx19 = load { i32*, i64 }, { i32*, i64 }* %arr.at18, align 8
  %arr.data20 = extractvalue { i32*, i64 } %idx19, 0
  %arr.at21 = getelementptr i32, i32* %arr.data20, i32 0
  %idx22 = load i32, i32* %arr.at21, align 4
  %m23 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data24 = extractvalue { { i32*, i64 }*, i64 } %m23, 0
  %arr.at25 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data24, i32 1
  %idx26 = load { i32*, i64 }, { i32*, i64 }* %arr.at25, align 8
  %arr.data27 = extractvalue { i32*, i64 } %idx26, 0
  %arr.at28 = getelementptr i32, i32* %arr.data27, i32 2
  %idx29 = load i32, i32* %arr.at28, align 4
  %m30 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data31 = extractvalue { { i32*, i64 }*, i64 } %m30, 0
  %arr.at32 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data31, i32 0
  %idx33 = load { i32*, i64 }, { i32*, i64 }* %arr.at32, align 8
  %arr.data34 = extractvalue { i32*, i64 } %idx33, 0
  %arr.at35 = getelementptr i32, i32* %arr.data34, i32 1
  %idx36 = load i32, i32* %arr.at35, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %idx22, i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %idx29, i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %idx36, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %arr37 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data38 = extractvalue { { i32*, i64 }*, i64 } %arr37, 0
  %arr.len39 = extractvalue { { i32*, i64 }*, i64 } %arr37, 1
  %i40 = alloca i64, align 8
  store i64 0, i64* %i40, align 8
  br label %free.cond

free.cond:                                        ; preds = %free.body, %arr.end
  %i41 = load i64, i64* %i40, align 8
  %more42 = icmp slt i64 %i41, %arr.len39
  br i1 %more42, label %free.body, label %free.end

free.body:                                        ; preds = %free.cond
  %free.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data38, i64 %i41
  %inner = load { i32*, i64 }, { i32*, i64 }* %free.slot, align 8
  %arr.data43 = extractvalue { i32*, i64 } %inner, 0
  %free.ptr = bitcast i32* %arr.data43 to i8*
  call void @free(i8* %free.ptr)
  %i44 = load i64, i64* %i40, align 8
  %next45 = add i64 %i44, 1
  store i64 %next45, i64* %i40, align 8
  br label %free.cond

free.end:                                         ; preds = %free.cond
  %free.ptr46 = bitcast { i32*, i64 }* %arr.data38 to i8*
  call void @free(i8* %free.ptr46)
  store { { i32*, i64 }*, i64 } zeroinitializer, { { i32*, i64 }*, i64 }* %m, align 8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```

## 015 — slice a[1..4] is a view (shared storage, exclusive end)

```ura
// arrays/015.ura - slice a[1..4] is a view (shares storage, exclusive end)

main():
    a i32[] = [10, 20, 30, 40, 50]
    mid i32[] = a[1..4]
    output("mid: ", mid[0], " ", mid[1], " ", mid[2], "\n")
    mid[0] = 99
    output("a[1]=", a[1], "\n")
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
│     ├─ int 10
│     ├─ int 20
│     ├─ int 30
│     ├─ int 40
│     └─ int 50
├─ = : array
│  ├─ mid : i32[]
│  └─ index : i32[]
│     ├─ a : i32[]
│     └─ range : i32
│        ├─ int 1
│        └─ int 4
├─ output : void
│  ├─ char[] "mid: "
│  ├─ index : i32
│  │  ├─ mid : i32[]
│  │  └─ int 0
│  ├─ char[] " "
│  ├─ index : i32
│  │  ├─ mid : i32[]
│  │  └─ int 1
│  ├─ char[] " "
│  ├─ index : i32
│  │  ├─ mid : i32[]
│  │  └─ int 2
│  └─ char[] "\n"
├─ = : i32
│  ├─ index : i32
│  │  ├─ mid : i32[]
│  │  └─ int 0
│  └─ int 99
└─ output : void
   ├─ char[] "a[1]="
   ├─ index : i32
   │  ├─ a : i32[]
   │  └─ int 1
   └─ char[] "\n"
```

```out
mid: 20 30 40
a[1]=99
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"mid: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [23 x i8] c"%.*s%d%.*s%d%.*s%d%.*s\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"a[1]=\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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
  %arr = alloca i32, i64 5, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 10, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 20, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 30, i32* %arr.init2, align 4
  %arr.init3 = getelementptr i32, i32* %arr, i64 3
  store i32 40, i32* %arr.init3, align 4
  %arr.init4 = getelementptr i32, i32* %arr, i64 4
  store i32 50, i32* %arr.init4, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 5, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %mid = alloca { i32*, i64 }, align 8
  %a5 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a5, 0
  %slice.data = getelementptr i32, i32* %arr.data, i64 1
  %arr.ptr6 = insertvalue { i32*, i64 } undef, i32* %slice.data, 0
  %arr.len7 = insertvalue { i32*, i64 } %arr.ptr6, i64 3, 1
  store { i32*, i64 } %arr.len7, { i32*, i64 }* %mid, align 8
  %mid8 = load { i32*, i64 }, { i32*, i64 }* %mid, align 8
  %arr.data9 = extractvalue { i32*, i64 } %mid8, 0
  %arr.at = getelementptr i32, i32* %arr.data9, i32 0
  %idx = load i32, i32* %arr.at, align 4
  %mid10 = load { i32*, i64 }, { i32*, i64 }* %mid, align 8
  %arr.data11 = extractvalue { i32*, i64 } %mid10, 0
  %arr.at12 = getelementptr i32, i32* %arr.data11, i32 1
  %idx13 = load i32, i32* %arr.at12, align 4
  %mid14 = load { i32*, i64 }, { i32*, i64 }* %mid, align 8
  %arr.data15 = extractvalue { i32*, i64 } %mid14, 0
  %arr.at16 = getelementptr i32, i32* %arr.data15, i32 2
  %idx17 = load i32, i32* %arr.at16, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %idx13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %idx17, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %mid18 = load { i32*, i64 }, { i32*, i64 }* %mid, align 8
  %arr.data19 = extractvalue { i32*, i64 } %mid18, 0
  %arr.at20 = getelementptr i32, i32* %arr.data19, i32 0
  store i32 99, i32* %arr.at20, align 4
  %a21 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data22 = extractvalue { i32*, i64 } %a21, 0
  %arr.at23 = getelementptr i32, i32* %arr.data22, i32 1
  %idx24 = load i32, i32* %arr.at23, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.6, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i32 %idx24, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 016 — ? on a slice traps when the range is out of bounds

```ura
// arrays/016.ura - ? on a slice traps when the range is out of bounds

main():
    a i32[] = [10, 20, 30]
    mid i32[] = a[1..9]?
    output(mid[0])
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
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
├─ = : array
│  ├─ mid : i32[]
│  └─ index : i32[]
│     ├─ a : i32[]
│     └─ range : i32
│        ├─ int 1
│        └─ int 9
└─ output : void
   └─ index : i32
      ├─ mid : i32[]
      └─ int 0
```

```out
```

```err
runtime error: slice range out of bounds
  016.ura:5:18
  |
5 |     mid i32[] = a[1..9]?
  |                  ^
exit: 1
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [174 x i8] c"runtime error: slice range out of bounds\0A  016.ura:5:18\0A  |\0A5 |     mid i32[] = a[1..9]?\0A  |                  ^\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%d\00", align 1

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
  store i32 10, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 20, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 30, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %mid = alloca { i32*, i64 }, align 8
  %a3 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data = extractvalue { i32*, i64 } %a3, 0
  %arr.len4 = extractvalue { i32*, i64 } %a3, 1
  %e.hi = icmp sgt i64 9, %arr.len4
  %b = or i1 false, %e.hi
  %bad = or i1 %b, false
  br i1 %bad, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([174 x i8], [174 x i8]* @trap_msg, i32 0, i32 0), i64 173)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %slice.data = getelementptr i32, i32* %arr.data, i64 1
  %arr.ptr5 = insertvalue { i32*, i64 } undef, i32* %slice.data, 0
  %arr.len6 = insertvalue { i32*, i64 } %arr.ptr5, i64 8, 1
  store { i32*, i64 } %arr.len6, { i32*, i64 }* %mid, align 8
  %mid7 = load { i32*, i64 }, { i32*, i64 }* %mid, align 8
  %arr.data8 = extractvalue { i32*, i64 } %mid7, 0
  %arr.at = getelementptr i32, i32* %arr.data8, i32 0
  %idx = load i32, i32* %arr.at, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i32 %idx)
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 017 — arr.len on stack, heap, slice, and multi-dim

```ura
// arrays/017.ura - arr.len reads the fat-pointer length (i32)

main():
    nums i32[] = [10, 20, 30, 40]
    output("len = ", nums.len, "\n")
    mid i32[] = nums[1..3]
    output("slice len = ", mid.len, "\n")
    grid i32[][] = i32[3][4]
    output("rows = ", grid.len, " cols = ", grid[0].len, "\n")
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
│  ├─ nums : i32[]
│  └─ array : i32[]
│     ├─ int 10
│     ├─ int 20
│     ├─ int 30
│     └─ int 40
├─ output : void
│  ├─ char[] "len = "
│  ├─ .len : u64
│  │  └─ nums : i32[]
│  └─ char[] "\n"
├─ = : array
│  ├─ mid : i32[]
│  └─ index : i32[]
│     ├─ nums : i32[]
│     └─ range : i32
│        ├─ int 1
│        └─ int 3
├─ output : void
│  ├─ char[] "slice len = "
│  ├─ .len : u64
│  │  └─ mid : i32[]
│  └─ char[] "\n"
├─ = : array
│  ├─ grid : i32[][]
│  └─ array : i32[][]
│     ├─ int 3
│     └─ int 4
└─ output : void
   ├─ char[] "rows = "
   ├─ .len : u64
   │  └─ grid : i32[][]
   ├─ char[] " cols = "
   ├─ .len : u64
   │  └─ index : i32[]
   │     ├─ grid : i32[][]
   │     └─ int 0
   └─ char[] "\n"
```

```out
len = 4
slice len = 2
rows = 3 cols = 4
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"len = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%llu%.*s\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"slice len = \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [13 x i8] c"%.*s%llu%.*s\00", align 1
@str.5 = private unnamed_addr constant [8 x i8] c"rows = \00", align 1
@str.6 = private unnamed_addr constant [9 x i8] c" cols = \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [21 x i8] c"%.*s%llu%.*s%llu%.*s\00", align 1

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
  %nums = alloca { i32*, i64 }, align 8
  %arr = alloca i32, i64 4, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 10, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 20, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 30, i32* %arr.init2, align 4
  %arr.init3 = getelementptr i32, i32* %arr, i64 3
  store i32 40, i32* %arr.init3, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 4, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %nums, align 8
  %nums4 = load { i32*, i64 }, { i32*, i64 }* %nums, align 8
  %len = extractvalue { i32*, i64 } %nums4, 1
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 %len, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %mid = alloca { i32*, i64 }, align 8
  %nums5 = load { i32*, i64 }, { i32*, i64 }* %nums, align 8
  %arr.data = extractvalue { i32*, i64 } %nums5, 0
  %slice.data = getelementptr i32, i32* %arr.data, i64 1
  %arr.ptr6 = insertvalue { i32*, i64 } undef, i32* %slice.data, 0
  %arr.len7 = insertvalue { i32*, i64 } %arr.ptr6, i64 2, 1
  store { i32*, i64 } %arr.len7, { i32*, i64 }* %mid, align 8
  %mid8 = load { i32*, i64 }, { i32*, i64 }* %mid, align 8
  %len9 = extractvalue { i32*, i64 } %mid8, 1
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.4, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i64 %len9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %grid = alloca { { i32*, i64 }*, i64 }, align 8
  %arr10 = alloca { i32*, i64 }, i64 3, align 8
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 8
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i11 = load i64, i64* %i, align 8
  %more = icmp slt i64 %i11, 3
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr12 = alloca i32, i64 4, align 4
  %4 = bitcast i32* %arr12 to i8*
  call void @llvm.memset.p0i8.i64(i8* %4, i8 0, i64 16, i1 false)
  %arr.ptr13 = insertvalue { i32*, i64 } undef, i32* %arr12, 0
  %arr.len14 = insertvalue { i32*, i64 } %arr.ptr13, i64 4, 1
  %i15 = load i64, i64* %i, align 8
  %arr.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr10, i64 %i15
  store { i32*, i64 } %arr.len14, { i32*, i64 }* %arr.slot, align 8
  %next = add i64 %i15, 1
  store i64 %next, i64* %i, align 8
  br label %arr.cond

arr.end:                                          ; preds = %arr.cond
  %arr.ptr16 = insertvalue { { i32*, i64 }*, i64 } undef, { i32*, i64 }* %arr10, 0
  %arr.len17 = insertvalue { { i32*, i64 }*, i64 } %arr.ptr16, i64 3, 1
  store { { i32*, i64 }*, i64 } %arr.len17, { { i32*, i64 }*, i64 }* %grid, align 8
  %grid18 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %len19 = extractvalue { { i32*, i64 }*, i64 } %grid18, 1
  %grid20 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %arr.data21 = extractvalue { { i32*, i64 }*, i64 } %grid20, 0
  %arr.at = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data21, i32 0
  %idx = load { i32*, i64 }, { i32*, i64 }* %arr.at, align 8
  %len22 = extractvalue { i32*, i64 } %idx, 1
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt.8, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.5, i32 0, i32 0), i64 %len19, i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.6, i32 0, i32 0), i64 %len22, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 018 — indexing a non-array

```ura
main():
    a i32 = 5
    output(a[0])
```

```tree
```

```out
```

```err
error: Cannot index 'i32', it is not an array
  018.ura:3:13
  |
3 |     output(a[0])
  |             ^
```

```ll
```

## 019 — non-integer index

```ura
main():
    a i32[] = [1, 2, 3]
    output(a[1.5])
```

```tree
```

```out
```

```err
error: Array index must be an integer, got f32
  019.ura:3:13
  |
3 |     output(a[1.5])
  |             ^
```

```ll
```

## 020 — array literal with mixed element types

```ura
main():
    a i32[] = [1, True, 3]
```

```tree
```

```out
```

```err
error: Array elements must all be the same type
  020.ura:2:19
  |
2 |     a i32[] = [1, True, 3]
  |                   ^^^^
```

```ll
```

## 021 — array size is not an integer

```ura
main():
    a i32[] = i32[1.5]
```

```tree
```

```out
```

```err
error: Array size must be an integer
  021.ura:2:19
  |
2 |     a i32[] = i32[1.5]
  |                   ^^^
```

```ll
```

## 022 — clean on a non-array

```ura
main():
    x i32 = 5
    clean x
```

```tree
```

```out
```

```err
error: 'clean' frees a heap array, not i32
  022.ura:3:5
  |
3 |     clean x
  |     ^^^^^
```

```ll
```

## 023 — new without an array type

```ura
main():
    a i32[] = new i32
```

```tree
```

```out
```

```err
error: Expected an array type after 'new' (e.g. new int[n])
  023.ura:2:15
  |
2 |     a i32[] = new i32
  |               ^^^
```

```ll
```

## 024 — missing closing bracket on index

```ura
main():
    a i32[] = [1, 2, 3]
    output(a[0)
```

```tree
```

```out
```

```err
error: Expected ']' after array index
  024.ura:3:13
  |
3 |     output(a[0)
  |             ^
```

```ll
```

## 025 — empty array literal has no element type

```ura
main():
    a i32[] = []
```

```tree
```

```out
```

```err
error: Empty array literal has no element type
  025.ura:2:15
  |
2 |     a i32[] = []
  |               ^
```

```ll
```

## 026 — range bounds must be integers

```ura
main():
    a i32[] = [1, 2, 3]
    mid i32[] = a[1.5..3]
```

```tree
```

```out
```

```err
error: Range bounds must be integers
  026.ura:3:22
  |
3 |     mid i32[] = a[1.5..3]
  |                      ^^
```

```ll
```

## 027 — .len on a non-array

```ura
main():
    x i32 = 5
    output(x.len)
```

```tree
```

```out
```

```err
error: '.len' is only valid on an array, not i32
  027.ura:3:13
  |
3 |     output(x.len)
  |             ^
```

```ll
```

## 028 — unknown member

```ura
main():
    a i32[] = [1, 2, 3]
    output(a.size)
```

```tree
```

```out
```

```err
error: Unknown member '.size'
  028.ura:3:13
  |
3 |     output(a.size)
  |             ^
```

```ll
```

## 029 — array literals: one element, expressions, and strings

```ura
// arrays/029.ura - literal shapes beyond constants

fn twice(n i32) i32:
    return n * 2

main():
    // a single-element literal
    one i32[] = [42]
    output(one[0], " len ", one.len, "\n")

    // elements that are not compile-time constants
    x i32 = 5
    ex i32[] = [x, x + 1, x * 2, twice(x)]
    for v in ex:
        output(v, " ")
    output("\n")

    // an array of strings
    names char[][] = ["ada", "grace", "alan"]
    for n in names:
        output(n, " ")
    output("len ", names.len, "\n")
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

fn twice(n : i32) : i32
└─ return
   └─ * : i32
      ├─ n : i32
      └─ int 2

fn main() : i32
├─ = : array
│  ├─ one : i32[]
│  └─ array : i32[]
│     └─ int 42
├─ output : void
│  ├─ index : i32
│  │  ├─ one : i32[]
│  │  └─ int 0
│  ├─ char[] " len "
│  ├─ .len : u64
│  │  └─ one : i32[]
│  └─ char[] "\n"
├─ = : i32
│  ├─ x : i32
│  └─ int 5
├─ = : array
│  ├─ ex : i32[]
│  └─ array : i32[]
│     ├─ x : i32
│     ├─ + : i32
│     │  ├─ x : i32
│     │  └─ int 1
│     ├─ * : i32
│     │  ├─ x : i32
│     │  └─ int 2
│     └─ call twice : i32
│        └─ x : i32
├─ for
│  ├─ v : i32
│  ├─ ex : i32[]
│  └─ output : void
│     ├─ v : i32
│     └─ char[] " "
├─ output : void
│  └─ char[] "\n"
├─ = : array
│  ├─ names : char[][]
│  └─ array : char[][]
│     ├─ char[] "ada"
│     ├─ char[] "grace"
│     └─ char[] "alan"
├─ for
│  ├─ n : char[]
│  ├─ names : char[][]
│  └─ output : void
│     ├─ n : char[]
│     └─ char[] " "
└─ output : void
   ├─ char[] "len "
   ├─ .len : u64
   │  └─ names : char[][]
   └─ char[] "\n"
```

```out
42 len 1
5 6 10 10 
ada grace alan len 3
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c" len \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%d%.*s%llu%.*s\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.3 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"ada\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"grace\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"alan\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.10 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.11 = private unnamed_addr constant [5 x i8] c"len \00", align 1
@str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.13 = private unnamed_addr constant [13 x i8] c"%.*s%llu%.*s\00", align 1

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

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 2
  ret i32 %mul
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %one = alloca { i32*, i64 }, align 8
  %arr = alloca i32, i64 1, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 42, i32* %arr.init, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 1, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %one, align 8
  %one1 = load { i32*, i64 }, { i32*, i64 }* %one, align 8
  %arr.data = extractvalue { i32*, i64 } %one1, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 0
  %idx = load i32, i32* %arr.at, align 4
  %one2 = load { i32*, i64 }, { i32*, i64 }* %one, align 8
  %len = extractvalue { i32*, i64 } %one2, 1
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i32 %idx, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 %len, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %x = alloca i32, align 4
  store i32 5, i32* %x, align 4
  %ex = alloca { i32*, i64 }, align 8
  %arr3 = alloca i32, i64 4, align 4
  %x4 = load i32, i32* %x, align 4
  %arr.init5 = getelementptr i32, i32* %arr3, i64 0
  store i32 %x4, i32* %arr.init5, align 4
  %x6 = load i32, i32* %x, align 4
  %add = add i32 %x6, 1
  %arr.init7 = getelementptr i32, i32* %arr3, i64 1
  store i32 %add, i32* %arr.init7, align 4
  %x8 = load i32, i32* %x, align 4
  %mul = mul i32 %x8, 2
  %arr.init9 = getelementptr i32, i32* %arr3, i64 2
  store i32 %mul, i32* %arr.init9, align 4
  %x10 = load i32, i32* %x, align 4
  %call = call i32 @twice(i32 %x10)
  %arr.init11 = getelementptr i32, i32* %arr3, i64 3
  store i32 %call, i32* %arr.init11, align 4
  %arr.ptr12 = insertvalue { i32*, i64 } undef, i32* %arr3, 0
  %arr.len13 = insertvalue { i32*, i64 } %arr.ptr12, i64 4, 1
  store { i32*, i64 } %arr.len13, { i32*, i64 }* %ex, align 8
  %ex14 = load { i32*, i64 }, { i32*, i64 }* %ex, align 8
  %arr.data15 = extractvalue { i32*, i64 } %ex14, 0
  %arr.len16 = extractvalue { i32*, i64 } %ex14, 1
  %idx17 = alloca i64, align 8
  store i64 0, i64* %idx17, align 8
  %v = alloca i32, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i = load i64, i64* %idx17, align 8
  %more = icmp slt i64 %i, %arr.len16
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %elem = getelementptr i32, i32* %arr.data15, i64 %i
  %x18 = load i32, i32* %elem, align 4
  store i32 %x18, i32* %v, align 4
  %v19 = load i32, i32* %v, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.3, i32 0, i32 0), i32 %v19, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i20 = load i64, i64* %idx17, align 8
  %next = add i64 %i20, 1
  store i64 %next, i64* %idx17, align 8
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %names = alloca { { i8*, i64 }*, i64 }, align 8
  %arr21 = alloca { i8*, i64 }, i64 3, align 8
  %arr.init22 = getelementptr { i8*, i64 }, { i8*, i64 }* %arr21, i64 0
  store { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i64 3 }, { i8*, i64 }* %arr.init22, align 8
  %arr.init23 = getelementptr { i8*, i64 }, { i8*, i64 }* %arr21, i64 1
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i64 5 }, { i8*, i64 }* %arr.init23, align 8
  %arr.init24 = getelementptr { i8*, i64 }, { i8*, i64 }* %arr21, i64 2
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0), i64 4 }, { i8*, i64 }* %arr.init24, align 8
  %arr.ptr25 = insertvalue { { i8*, i64 }*, i64 } undef, { i8*, i64 }* %arr21, 0
  %arr.len26 = insertvalue { { i8*, i64 }*, i64 } %arr.ptr25, i64 3, 1
  store { { i8*, i64 }*, i64 } %arr.len26, { { i8*, i64 }*, i64 }* %names, align 8
  %names27 = load { { i8*, i64 }*, i64 }, { { i8*, i64 }*, i64 }* %names, align 8
  %arr.data28 = extractvalue { { i8*, i64 }*, i64 } %names27, 0
  %arr.len29 = extractvalue { { i8*, i64 }*, i64 } %names27, 1
  %idx30 = alloca i64, align 8
  store i64 0, i64* %idx30, align 8
  %n = alloca { i8*, i64 }, align 8
  br label %for.cond31

for.cond31:                                       ; preds = %for.inc33, %for.end
  %i35 = load i64, i64* %idx30, align 8
  %more36 = icmp slt i64 %i35, %arr.len29
  br i1 %more36, label %for.body32, label %for.end34

for.body32:                                       ; preds = %for.cond31
  %elem37 = getelementptr { i8*, i64 }, { i8*, i64 }* %arr.data28, i64 %i35
  %x38 = load { i8*, i64 }, { i8*, i64 }* %elem37, align 8
  store { i8*, i64 } %x38, { i8*, i64 }* %n, align 8
  %n39 = load { i8*, i64 }, { i8*, i64 }* %n, align 8
  %str.len = extractvalue { i8*, i64 } %n39, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %n39, 0
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.10, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  br label %for.inc33

for.inc33:                                        ; preds = %for.body32
  %i40 = load i64, i64* %idx30, align 8
  %next41 = add i64 %i40, 1
  store i64 %next41, i64* %idx30, align 8
  br label %for.cond31

for.end34:                                        ; preds = %for.cond31
  %names42 = load { { i8*, i64 }*, i64 }, { { i8*, i64 }*, i64 }* %names, align 8
  %len43 = extractvalue { { i8*, i64 }*, i64 } %names42, 1
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.13, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.11, i32 0, i32 0), i64 %len43, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.12, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 030 — fill a heap char buffer by index, print it, clean it

```ura
// arrays/030.ura - fill a heap char buffer by index, print it, clean it

main():
    str char[] = new char[8]
    str[0] = 'D'
    str[1] = 'U'
    str[2] = 'N'
    str[3] = 'G'
    str[4] = 'E'
    str[5] = 'O'
    str[6] = 'N'
    str[7] = 0 as char
    output(str, "\n")
    clean str
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
│  ├─ str : char[]
│  └─ array : char[]
│     └─ int 8
├─ = : char
│  ├─ index : char
│  │  ├─ str : char[]
│  │  └─ int 0
│  └─ char 'D'
├─ = : char
│  ├─ index : char
│  │  ├─ str : char[]
│  │  └─ int 1
│  └─ char 'U'
├─ = : char
│  ├─ index : char
│  │  ├─ str : char[]
│  │  └─ int 2
│  └─ char 'N'
├─ = : char
│  ├─ index : char
│  │  ├─ str : char[]
│  │  └─ int 3
│  └─ char 'G'
├─ = : char
│  ├─ index : char
│  │  ├─ str : char[]
│  │  └─ int 4
│  └─ char 'E'
├─ = : char
│  ├─ index : char
│  │  ├─ str : char[]
│  │  └─ int 5
│  └─ char 'O'
├─ = : char
│  ├─ index : char
│  │  ├─ str : char[]
│  │  └─ int 6
│  └─ char 'N'
├─ = : char
│  ├─ index : char
│  │  ├─ str : char[]
│  │  └─ int 7
│  └─ cast : char
│     └─ int 0
├─ output : void
│  ├─ str : char[]
│  └─ char[] "\n"
└─ clean : void
   └─ str : char[]
```

```out
DUNGEON
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
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
  %str = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 8, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 8, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str1, 0
  %arr.at = getelementptr i8, i8* %arr.data, i32 0
  store i8 68, i8* %arr.at, align 1
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data3 = extractvalue { i8*, i64 } %str2, 0
  %arr.at4 = getelementptr i8, i8* %arr.data3, i32 1
  store i8 85, i8* %arr.at4, align 1
  %str5 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data6 = extractvalue { i8*, i64 } %str5, 0
  %arr.at7 = getelementptr i8, i8* %arr.data6, i32 2
  store i8 78, i8* %arr.at7, align 1
  %str8 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data9 = extractvalue { i8*, i64 } %str8, 0
  %arr.at10 = getelementptr i8, i8* %arr.data9, i32 3
  store i8 71, i8* %arr.at10, align 1
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data12 = extractvalue { i8*, i64 } %str11, 0
  %arr.at13 = getelementptr i8, i8* %arr.data12, i32 4
  store i8 69, i8* %arr.at13, align 1
  %str14 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data15 = extractvalue { i8*, i64 } %str14, 0
  %arr.at16 = getelementptr i8, i8* %arr.data15, i32 5
  store i8 79, i8* %arr.at16, align 1
  %str17 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data18 = extractvalue { i8*, i64 } %str17, 0
  %arr.at19 = getelementptr i8, i8* %arr.data18, i32 6
  store i8 78, i8* %arr.at19, align 1
  %str20 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data21 = extractvalue { i8*, i64 } %str20, 0
  %arr.at22 = getelementptr i8, i8* %arr.data21, i32 7
  store i8 0, i8* %arr.at22, align 1
  %str23 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %str.len = extractvalue { i8*, i64 } %str23, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %str23, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %arr = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data24 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data24)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %str, align 8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```
