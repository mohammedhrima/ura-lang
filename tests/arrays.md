# arrays

## index

- 001 — 1D int array: literal, index read, indexed write
- 002 — variable index and element arithmetic
- 003 — arrays of bool, char, float
- 004 — sized constructor int[N] with zero-init
- 005 — VLA: runtime-sized stack array
- 006 — 2D array literal and nested indexing
- 007 — sized 2D matrix int[N][M], zero-init and writes
- 008 — 3D sized array with runtime size
- 009 — ? bounds guard: in-bounds access is fine
- 010 — ? bounds guard: out-of-bounds access traps at runtime
- 011 — heap array: new int[N], zero-init, write, clean
- 012 — heap array with runtime size (new int[n]) + clean
- 013 — clean nulls the array; a[i]? then traps (no use-after-free)
- 014 — multi-dim heap: new int[N][M] + recursive clean
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

## 001 — 1D int array: literal, index read, indexed write

```ura
// arrays/001.ura - 1D int array: literal, index read, indexed write

main():
    a int[] = [10, 20, 30]
    output("a[0]=", a[0], " a[1]=", a[1], " a[2]=", a[2], "\n")
    a[1] = 99
    output("after a[1]=99: ", a[1], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
├─ output : void
│  ├─ chars "a[0]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 0
│  ├─ chars " a[1]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 1
│  ├─ chars " a[2]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 2
│  └─ chars "\n"
├─ = : int
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 1
│  └─ int 99
└─ output : void
   ├─ chars "after a[1]=99: "
   ├─ index : int
   │  ├─ a : int[]
   │  └─ int 1
   └─ chars "\n"
```

```out
a[0]=10 a[1]=20 a[2]=30
after a[1]=99: 99
```

```err
```

```ll

@str = private unnamed_addr constant [6 x i8] c"a[0]=\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" a[1]=\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c" a[2]=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%s%d%s%d%s%d%s\00", align 1
@str.4 = private unnamed_addr constant [16 x i8] c"after a[1]=99: \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %idx7, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %idx11, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a12 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data13 = extractvalue { i32*, i64 } %a12, 0
  %arr.at14 = getelementptr i32, i32* %arr.data13, i32 1
  store i32 99, i32* %arr.at14, align 4
  %a15 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data16 = extractvalue { i32*, i64 } %a15, 0
  %arr.at17 = getelementptr i32, i32* %arr.data16, i32 1
  %idx18 = load i32, i32* %arr.at17, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.4, i32 0, i32 0), i32 %idx18, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — variable index and element arithmetic

```ura
// arrays/002.ura - variable index and element arithmetic

main():
    a int[] = [5, 10, 15, 20]
    i int = 3
    output("a[i]=", a[i], " a[i-1]=", a[i - 1], "\n")
    output("a[0]+a[3]=", a[0] + a[3], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     ├─ int 5
│     ├─ int 10
│     ├─ int 15
│     └─ int 20
├─ = : int
│  ├─ i : int
│  └─ int 3
├─ output : void
│  ├─ chars "a[i]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ i : int
│  ├─ chars " a[i-1]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ - : int
│  │     ├─ i : int
│  │     └─ int 1
│  └─ chars "\n"
└─ output : void
   ├─ chars "a[0]+a[3]="
   ├─ + : int
   │  ├─ index : int
   │  │  ├─ a : int[]
   │  │  └─ int 0
   │  └─ index : int
   │     ├─ a : int[]
   │     └─ int 3
   └─ chars "\n"
```

```out
a[i]=20 a[i-1]=15
a[0]+a[3]=25
```

```err
```

```ll

@str = private unnamed_addr constant [6 x i8] c"a[i]=\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c" a[i-1]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"a[0]+a[3]=\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %idx10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %a11 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data12 = extractvalue { i32*, i64 } %a11, 0
  %arr.at13 = getelementptr i32, i32* %arr.data12, i32 0
  %idx14 = load i32, i32* %arr.at13, align 4
  %a15 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data16 = extractvalue { i32*, i64 } %a15, 0
  %arr.at17 = getelementptr i32, i32* %arr.data16, i32 3
  %idx18 = load i32, i32* %arr.at17, align 4
  %add = add i32 %idx14, %idx18
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %add, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — arrays of bool, char, float

```ura
// arrays/003.ura - arrays of bool, char, float

main():
    b bool[]  = [True, False, True]
    c char[]  = ['a', 'b', 'c']
    f float[] = [1.5, 2.5, 3.0]
    output("b[0]=", b[0], " c[1]=", c[1], " f[2]=", f[2], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
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
│  ├─ f : float[]
│  └─ array : float[]
│     ├─ float 1.5
│     ├─ float 2.5
│     └─ float 3
└─ output : void
   ├─ chars "b[0]="
   ├─ index : bool
   │  ├─ b : bool[]
   │  └─ int 0
   ├─ chars " c[1]="
   ├─ index : char
   │  ├─ c : char[]
   │  └─ int 1
   ├─ chars " f[2]="
   ├─ index : float
   │  ├─ f : float[]
   │  └─ int 2
   └─ chars "\n"
```

```out
b[0]=True c[1]=b f[2]=3.000000
```

```err
```

```ll

@str = private unnamed_addr constant [6 x i8] c"b[0]=\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" c[1]=\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c" f[2]=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%s%s%s%c%s%f%s\00", align 1

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %c2i, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — sized constructor int[N] with zero-init

```ura
// arrays/004.ura - sized constructor int[N] with zero-init

main():
    a int[] = int[5]
    output("zero-init: ", a[0], " ", a[4], "\n")
    a[2] = 42
    output("a[2]=", a[2], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     └─ int 5
├─ output : void
│  ├─ chars "zero-init: "
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 0
│  ├─ chars " "
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 4
│  └─ chars "\n"
├─ = : int
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 2
│  └─ int 42
└─ output : void
   ├─ chars "a[2]="
   ├─ index : int
   │  ├─ a : int[]
   │  └─ int 2
   └─ chars "\n"
```

```out
zero-init: 0 0
a[2]=42
```

```err
```

```ll

@str = private unnamed_addr constant [12 x i8] c"zero-init: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"a[2]=\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %a = alloca { i32*, i64 }, align 8
  %arr = alloca i32, i64 5, align 4
  %0 = bitcast i32* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 20, i1 false)
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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %idx5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %a6 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data7 = extractvalue { i32*, i64 } %a6, 0
  %arr.at8 = getelementptr i32, i32* %arr.data7, i32 2
  store i32 42, i32* %arr.at8, align 4
  %a9 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data10 = extractvalue { i32*, i64 } %a9, 0
  %arr.at11 = getelementptr i32, i32* %arr.data10, i32 2
  %idx12 = load i32, i32* %arr.at11, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i32 %idx12, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
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
    n int = 4
    a int[] = int[n * 2]
    a[7] = 100
    output("a[0]=", a[0], " a[7]=", a[7], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : int
│  ├─ n : int
│  └─ int 4
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     └─ * : int
│        ├─ n : int
│        └─ int 2
├─ = : int
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 7
│  └─ int 100
└─ output : void
   ├─ chars "a[0]="
   ├─ index : int
   │  ├─ a : int[]
   │  └─ int 0
   ├─ chars " a[7]="
   ├─ index : int
   │  ├─ a : int[]
   │  └─ int 7
   └─ chars "\n"
```

```out
a[0]=0 a[7]=100
```

```err
```

```ll

@str = private unnamed_addr constant [6 x i8] c"a[0]=\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" a[7]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 4, i32* %n, align 4
  %a = alloca { i32*, i64 }, align 8
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 2
  %n2 = sext i32 %mul to i64
  %arr = alloca i32, i64 %n2, align 4
  %bytes = mul i64 %n2, 4
  %0 = bitcast i32* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 %bytes, i1 false)
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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %idx10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
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
    m int[][] = [[1, 2], [3, 4]]
    output("m[0][0]=", m[0][0], " m[1][1]=", m[1][1], "\n")
    m[0][1] = 99
    output("m[0][1]=", m[0][1], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ m : int[][]
│  └─ array : int[][]
│     ├─ array : int[]
│     │  ├─ int 1
│     │  └─ int 2
│     └─ array : int[]
│        ├─ int 3
│        └─ int 4
├─ output : void
│  ├─ chars "m[0][0]="
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ m : int[][]
│  │  │  └─ int 0
│  │  └─ int 0
│  ├─ chars " m[1][1]="
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ m : int[][]
│  │  │  └─ int 1
│  │  └─ int 1
│  └─ chars "\n"
├─ = : int
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ m : int[][]
│  │  │  └─ int 0
│  │  └─ int 1
│  └─ int 99
└─ output : void
   ├─ chars "m[0][1]="
   ├─ index : int
   │  ├─ index : int[]
   │  │  ├─ m : int[][]
   │  │  └─ int 0
   │  └─ int 1
   └─ chars "\n"
```

```out
m[0][0]=1 m[1][1]=4
m[0][1]=99
```

```err
```

```ll

@str = private unnamed_addr constant [9 x i8] c"m[0][0]=\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c" m[1][1]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"m[0][1]=\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %idx15, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %idx22, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i32 %idx35, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — sized 2D matrix int[N][M], zero-init and writes

```ura
// arrays/007.ura - sized 2D matrix int[N][M], zero-init and writes

main():
    g int[][] = int[3][4]
    g[0][0] = 1
    g[2][3] = 99
    output("g[0][0]=", g[0][0], " g[2][3]=", g[2][3], " g[1][1]=", g[1][1], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ g : int[][]
│  └─ array : int[][]
│     ├─ int 3
│     └─ int 4
├─ = : int
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ g : int[][]
│  │  │  └─ int 0
│  │  └─ int 0
│  └─ int 1
├─ = : int
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ g : int[][]
│  │  │  └─ int 2
│  │  └─ int 3
│  └─ int 99
└─ output : void
   ├─ chars "g[0][0]="
   ├─ index : int
   │  ├─ index : int[]
   │  │  ├─ g : int[][]
   │  │  └─ int 0
   │  └─ int 0
   ├─ chars " g[2][3]="
   ├─ index : int
   │  ├─ index : int[]
   │  │  ├─ g : int[][]
   │  │  └─ int 2
   │  └─ int 3
   ├─ chars " g[1][1]="
   ├─ index : int
   │  ├─ index : int[]
   │  │  ├─ g : int[][]
   │  │  └─ int 1
   │  └─ int 1
   └─ chars "\n"
```

```out
g[0][0]=1 g[2][3]=99 g[1][1]=0
```

```err
```

```ll

@str = private unnamed_addr constant [9 x i8] c"g[0][0]=\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c" g[2][3]=\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c" g[1][1]=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%s%d%s%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %g = alloca { { i32*, i64 }*, i64 }, align 8
  %arr = alloca { i32*, i64 }, i64 3, align 8
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i1 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i1, 3
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr2 = alloca i32, i64 4, align 4
  %0 = bitcast i32* %arr2 to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 16, i1 false)
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr2, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 4, 1
  %i3 = load i64, i64* %i, align 4
  %arr.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr, i64 %i3
  store { i32*, i64 } %arr.len, { i32*, i64 }* %arr.slot, align 8
  %next = add i64 %i3, 1
  store i64 %next, i64* %i, align 4
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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %idx21, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %idx28, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %idx35, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
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
    n int = 2
    v int[][][] = int[n][n][n]
    v[1][1][1] = 7
    output("v[1][1][1]=", v[1][1][1], " v[0][0][0]=", v[0][0][0], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : int
│  ├─ n : int
│  └─ int 2
├─ = : array
│  ├─ v : int[][][]
│  └─ array : int[][][]
│     ├─ n : int
│     ├─ n : int
│     └─ n : int
├─ = : int
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ index : int[][]
│  │  │  │  ├─ v : int[][][]
│  │  │  │  └─ int 1
│  │  │  └─ int 1
│  │  └─ int 1
│  └─ int 7
└─ output : void
   ├─ chars "v[1][1][1]="
   ├─ index : int
   │  ├─ index : int[]
   │  │  ├─ index : int[][]
   │  │  │  ├─ v : int[][][]
   │  │  │  └─ int 1
   │  │  └─ int 1
   │  └─ int 1
   ├─ chars " v[0][0][0]="
   ├─ index : int
   │  ├─ index : int[]
   │  │  ├─ index : int[][]
   │  │  │  ├─ v : int[][][]
   │  │  │  └─ int 0
   │  │  └─ int 0
   │  └─ int 0
   └─ chars "\n"
```

```out
v[1][1][1]=7 v[0][0][0]=0
```

```err
```

```ll

@str = private unnamed_addr constant [12 x i8] c"v[1][1][1]=\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c" v[0][0][0]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define i32 @main() {
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
  store i64 0, i64* %i, align 4
  br label %arr.cond

arr.cond:                                         ; preds = %arr.end12, %entry
  %i7 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i7, %n2
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr8 = alloca { i32*, i64 }, i64 %n4, align 8
  %i9 = alloca i64, align 8
  store i64 0, i64* %i9, align 4
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %idx39, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i32 %idx49, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0

arr.cond10:                                       ; preds = %arr.body11, %arr.body
  %i13 = load i64, i64* %i9, align 4
  %more14 = icmp slt i64 %i13, %n4
  br i1 %more14, label %arr.body11, label %arr.end12

arr.body11:                                       ; preds = %arr.cond10
  %arr15 = alloca i32, i64 %n6, align 4
  %bytes = mul i64 %n6, 4
  %1 = bitcast i32* %arr15 to i8*
  call void @llvm.memset.p0i8.i64(i8* %1, i8 0, i64 %bytes, i1 false)
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr15, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 %n6, 1
  %i16 = load i64, i64* %i9, align 4
  %arr.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr8, i64 %i16
  store { i32*, i64 } %arr.len, { i32*, i64 }* %arr.slot, align 8
  %next = add i64 %i16, 1
  store i64 %next, i64* %i9, align 4
  br label %arr.cond10

arr.end12:                                        ; preds = %arr.cond10
  %arr.ptr17 = insertvalue { { i32*, i64 }*, i64 } undef, { i32*, i64 }* %arr8, 0
  %arr.len18 = insertvalue { { i32*, i64 }*, i64 } %arr.ptr17, i64 %n4, 1
  %i19 = load i64, i64* %i, align 4
  %arr.slot20 = getelementptr { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr, i64 %i19
  store { { i32*, i64 }*, i64 } %arr.len18, { { i32*, i64 }*, i64 }* %arr.slot20, align 8
  %next21 = add i64 %i19, 1
  store i64 %next21, i64* %i, align 4
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
    a int[] = [10, 20, 30]
    i int = 2
    output("a[i]?=", a[i]?, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
├─ = : int
│  ├─ i : int
│  └─ int 2
└─ output : void
   ├─ chars "a[i]?="
   ├─ index : int
   │  ├─ a : int[]
   │  └─ i : int
   └─ chars "\n"
```

```out
a[i]?=30
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"a[i]?=\00", align 1
@trap_msg = private unnamed_addr constant [188 x i8] c"runtime error: array index out of bounds\0A  009.ura:6:23\0A  |\0A6 |     output(\22a[i]?=\22, a[i]?, \22\\n\22)\0A  |                       ^\0A\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
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
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([188 x i8], [188 x i8]* @trap_msg, i32 0, i32 0), i64 187)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %arr.at = getelementptr i32, i32* %arr.data, i32 %i4
  %idx6 = load i32, i32* %arr.at, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %idx6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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
    a int[] = [10, 20, 30]
    output(a[5]?)
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
└─ output : void
   └─ index : int
      ├─ a : int[]
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

@trap_msg = private unnamed_addr constant [162 x i8] c"runtime error: array index out of bounds\0A  010.ura:5:13\0A  |\0A5 |     output(a[5]?)\0A  |             ^\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define i32 @main() {
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
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([162 x i8], [162 x i8]* @trap_msg, i32 0, i32 0), i64 161)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %arr.at = getelementptr i32, i32* %arr.data, i32 5
  %idx = load i32, i32* %arr.at, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i32 %idx)
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 011 — heap array: new int[N], zero-init, write, clean

```ura
// arrays/011.ura - heap array: new int[N], zero-init, write, clean

main():
    a int[] = new int[5]
    a[2] = 42
    output("a[0]=", a[0], " a[2]=", a[2], "\n")
    clean a
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     └─ int 5
├─ = : int
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 2
│  └─ int 42
├─ output : void
│  ├─ chars "a[0]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 0
│  ├─ chars " a[2]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 2
│  └─ chars "\n"
└─ clean : void
   └─ a : int[]
```

```out
a[0]=0 a[2]=42
```

```err
```

```ll

@str = private unnamed_addr constant [6 x i8] c"a[0]=\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" a[2]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %idx8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
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

## 012 — heap array with runtime size (new int[n]) + clean

```ura
// arrays/012.ura - heap array with runtime size (new int[n]) + clean

main():
    n int = 3
    a int[] = new int[n * 2]
    a[5] = 7
    output("a[5]=", a[5], " a[0]=", a[0], "\n")
    clean a
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : int
│  ├─ n : int
│  └─ int 3
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     └─ * : int
│        ├─ n : int
│        └─ int 2
├─ = : int
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 5
│  └─ int 7
├─ output : void
│  ├─ chars "a[5]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 5
│  ├─ chars " a[0]="
│  ├─ index : int
│  │  ├─ a : int[]
│  │  └─ int 0
│  └─ chars "\n"
└─ clean : void
   └─ a : int[]
```

```out
a[5]=7 a[0]=0
```

```err
```

```ll

@str = private unnamed_addr constant [6 x i8] c"a[5]=\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c" a[0]=\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %idx10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
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
    a int[] = new int[5]
    clean a
    output(a[0]?)
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     └─ int 5
├─ clean : void
│  └─ a : int[]
└─ output : void
   └─ index : int
      ├─ a : int[]
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

@trap_msg = private unnamed_addr constant [162 x i8] c"runtime error: array index out of bounds\0A  013.ura:6:13\0A  |\0A6 |     output(a[0]?)\0A  |             ^\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define i32 @main() {
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
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([162 x i8], [162 x i8]* @trap_msg, i32 0, i32 0), i64 161)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %arr.at = getelementptr i32, i32* %arr.data3, i32 0
  %idx = load i32, i32* %arr.at, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i32 %idx)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 014 — multi-dim heap: new int[N][M] + recursive clean

```ura
// arrays/014.ura - multi-dim heap: new int[N][M] + recursive clean

main():
    m int[][] = new int[2][3]
    m[0][0] = 1
    m[1][2] = 9
    output("m[0][0]=", m[0][0], " m[1][2]=", m[1][2], " m[0][1]=", m[0][1], "\n")
    clean m
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ m : int[][]
│  └─ array : int[][]
│     ├─ int 2
│     └─ int 3
├─ = : int
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ m : int[][]
│  │  │  └─ int 0
│  │  └─ int 0
│  └─ int 1
├─ = : int
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ m : int[][]
│  │  │  └─ int 1
│  │  └─ int 2
│  └─ int 9
├─ output : void
│  ├─ chars "m[0][0]="
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ m : int[][]
│  │  │  └─ int 0
│  │  └─ int 0
│  ├─ chars " m[1][2]="
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ m : int[][]
│  │  │  └─ int 1
│  │  └─ int 2
│  ├─ chars " m[0][1]="
│  ├─ index : int
│  │  ├─ index : int[]
│  │  │  ├─ m : int[][]
│  │  │  └─ int 0
│  │  └─ int 1
│  └─ chars "\n"
└─ clean : void
   └─ m : int[][]
```

```out
m[0][0]=1 m[1][2]=9 m[0][1]=0
```

```err
```

```ll

@str = private unnamed_addr constant [9 x i8] c"m[0][0]=\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c" m[1][2]=\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c" m[0][1]=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%s%d%s%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %m = alloca { { i32*, i64 }*, i64 }, align 8
  %heap = call i8* @calloc(i64 2, i64 16)
  %arr = bitcast i8* %heap to { i32*, i64 }*
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i1 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i1, 2
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %heap2 = call i8* @calloc(i64 3, i64 4)
  %arr3 = bitcast i8* %heap2 to i32*
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr3, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  %i4 = load i64, i64* %i, align 4
  %arr.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr, i64 %i4
  store { i32*, i64 } %arr.len, { i32*, i64 }* %arr.slot, align 8
  %next = add i64 %i4, 1
  store i64 %next, i64* %i, align 4
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %idx22, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %idx29, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %idx36, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %arr37 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %m, align 8
  %arr.data38 = extractvalue { { i32*, i64 }*, i64 } %arr37, 0
  %arr.len39 = extractvalue { { i32*, i64 }*, i64 } %arr37, 1
  %i40 = alloca i64, align 8
  store i64 0, i64* %i40, align 4
  br label %free.cond

free.cond:                                        ; preds = %free.body, %arr.end
  %i41 = load i64, i64* %i40, align 4
  %more42 = icmp slt i64 %i41, %arr.len39
  br i1 %more42, label %free.body, label %free.end

free.body:                                        ; preds = %free.cond
  %free.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data38, i64 %i41
  %inner = load { i32*, i64 }, { i32*, i64 }* %free.slot, align 8
  %arr.data43 = extractvalue { i32*, i64 } %inner, 0
  %free.ptr = bitcast i32* %arr.data43 to i8*
  call void @free(i8* %free.ptr)
  %i44 = load i64, i64* %i40, align 4
  %next45 = add i64 %i44, 1
  store i64 %next45, i64* %i40, align 4
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
    a int[] = [10, 20, 30, 40, 50]
    mid int[] = a[1..4]
    output("mid: ", mid[0], " ", mid[1], " ", mid[2], "\n")
    mid[0] = 99
    output("a[1]=", a[1], "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     ├─ int 10
│     ├─ int 20
│     ├─ int 30
│     ├─ int 40
│     └─ int 50
├─ = : array
│  ├─ mid : int[]
│  └─ index : int[]
│     ├─ a : int[]
│     └─ range : int
│        ├─ int 1
│        └─ int 4
├─ output : void
│  ├─ chars "mid: "
│  ├─ index : int
│  │  ├─ mid : int[]
│  │  └─ int 0
│  ├─ chars " "
│  ├─ index : int
│  │  ├─ mid : int[]
│  │  └─ int 1
│  ├─ chars " "
│  ├─ index : int
│  │  ├─ mid : int[]
│  │  └─ int 2
│  └─ chars "\n"
├─ = : int
│  ├─ index : int
│  │  ├─ mid : int[]
│  │  └─ int 0
│  └─ int 99
└─ output : void
   ├─ chars "a[1]="
   ├─ index : int
   │  ├─ a : int[]
   │  └─ int 1
   └─ chars "\n"
```

```out
mid: 20 30 40
a[1]=99
```

```err
```

```ll

@str = private unnamed_addr constant [6 x i8] c"mid: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%s%d%s%d%s%d%s\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"a[1]=\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %idx13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %idx17, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %mid18 = load { i32*, i64 }, { i32*, i64 }* %mid, align 8
  %arr.data19 = extractvalue { i32*, i64 } %mid18, 0
  %arr.at20 = getelementptr i32, i32* %arr.data19, i32 0
  store i32 99, i32* %arr.at20, align 4
  %a21 = load { i32*, i64 }, { i32*, i64 }* %a, align 8
  %arr.data22 = extractvalue { i32*, i64 } %a21, 0
  %arr.at23 = getelementptr i32, i32* %arr.data22, i32 1
  %idx24 = load i32, i32* %arr.at23, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i32 %idx24, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 016 — ? on a slice traps when the range is out of bounds

```ura
// arrays/016.ura - ? on a slice traps when the range is out of bounds

main():
    a int[] = [10, 20, 30]
    mid int[] = a[1..9]?
    output(mid[0])
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
├─ = : array
│  ├─ mid : int[]
│  └─ index : int[]
│     ├─ a : int[]
│     └─ range : int
│        ├─ int 1
│        └─ int 9
└─ output : void
   └─ index : int
      ├─ mid : int[]
      └─ int 0
```

```out
```

```err
runtime error: slice range out of bounds
  016.ura:5:18
  |
5 |     mid int[] = a[1..9]?
  |                  ^
exit: 1
```

```ll

@trap_msg = private unnamed_addr constant [174 x i8] c"runtime error: slice range out of bounds\0A  016.ura:5:18\0A  |\0A5 |     mid int[] = a[1..9]?\0A  |                  ^\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define i32 @main() {
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
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([174 x i8], [174 x i8]* @trap_msg, i32 0, i32 0), i64 173)
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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i32 %idx)
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 017 — arr.len on stack, heap, slice, and multi-dim

```ura
// arrays/017.ura - arr.len reads the fat-pointer length (int)

main():
    nums int[] = [10, 20, 30, 40]
    output("len = ", nums.len, "\n")
    mid int[] = nums[1..3]
    output("slice len = ", mid.len, "\n")
    grid int[][] = int[3][4]
    output("rows = ", grid.len, " cols = ", grid[0].len, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : array
│  ├─ nums : int[]
│  └─ array : int[]
│     ├─ int 10
│     ├─ int 20
│     ├─ int 30
│     └─ int 40
├─ output : void
│  ├─ chars "len = "
│  ├─ .len : int
│  │  └─ nums : int[]
│  └─ chars "\n"
├─ = : array
│  ├─ mid : int[]
│  └─ index : int[]
│     ├─ nums : int[]
│     └─ range : int
│        ├─ int 1
│        └─ int 3
├─ output : void
│  ├─ chars "slice len = "
│  ├─ .len : int
│  │  └─ mid : int[]
│  └─ chars "\n"
├─ = : array
│  ├─ grid : int[][]
│  └─ array : int[][]
│     ├─ int 3
│     └─ int 4
└─ output : void
   ├─ chars "rows = "
   ├─ .len : int
   │  └─ grid : int[][]
   ├─ chars " cols = "
   ├─ .len : int
   │  └─ index : int[]
   │     ├─ grid : int[][]
   │     └─ int 0
   └─ chars "\n"
```

```out
len = 4
slice len = 2
rows = 3 cols = 4
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"len = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"slice len = \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [8 x i8] c"rows = \00", align 1
@str.6 = private unnamed_addr constant [9 x i8] c" cols = \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define i32 @main() {
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
  %len5 = trunc i64 %len to i32
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %len5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %mid = alloca { i32*, i64 }, align 8
  %nums6 = load { i32*, i64 }, { i32*, i64 }* %nums, align 8
  %arr.data = extractvalue { i32*, i64 } %nums6, 0
  %slice.data = getelementptr i32, i32* %arr.data, i64 1
  %arr.ptr7 = insertvalue { i32*, i64 } undef, i32* %slice.data, 0
  %arr.len8 = insertvalue { i32*, i64 } %arr.ptr7, i64 2, 1
  store { i32*, i64 } %arr.len8, { i32*, i64 }* %mid, align 8
  %mid9 = load { i32*, i64 }, { i32*, i64 }* %mid, align 8
  %len10 = extractvalue { i32*, i64 } %mid9, 1
  %len11 = trunc i64 %len10 to i32
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 %len11, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %grid = alloca { { i32*, i64 }*, i64 }, align 8
  %arr12 = alloca { i32*, i64 }, i64 3, align 8
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i13 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i13, 3
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr14 = alloca i32, i64 4, align 4
  %2 = bitcast i32* %arr14 to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 16, i1 false)
  %arr.ptr15 = insertvalue { i32*, i64 } undef, i32* %arr14, 0
  %arr.len16 = insertvalue { i32*, i64 } %arr.ptr15, i64 4, 1
  %i17 = load i64, i64* %i, align 4
  %arr.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr12, i64 %i17
  store { i32*, i64 } %arr.len16, { i32*, i64 }* %arr.slot, align 8
  %next = add i64 %i17, 1
  store i64 %next, i64* %i, align 4
  br label %arr.cond

arr.end:                                          ; preds = %arr.cond
  %arr.ptr18 = insertvalue { { i32*, i64 }*, i64 } undef, { i32*, i64 }* %arr12, 0
  %arr.len19 = insertvalue { { i32*, i64 }*, i64 } %arr.ptr18, i64 3, 1
  store { { i32*, i64 }*, i64 } %arr.len19, { { i32*, i64 }*, i64 }* %grid, align 8
  %grid20 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %len21 = extractvalue { { i32*, i64 }*, i64 } %grid20, 1
  %len22 = trunc i64 %len21 to i32
  %grid23 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %arr.data24 = extractvalue { { i32*, i64 }*, i64 } %grid23, 0
  %arr.at = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data24, i32 0
  %idx = load { i32*, i64 }, { i32*, i64 }* %arr.at, align 8
  %len25 = extractvalue { i32*, i64 } %idx, 1
  %len26 = trunc i64 %len25 to i32
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.8, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.5, i32 0, i32 0), i32 %len22, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.6, i32 0, i32 0), i32 %len26, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
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
    a int = 5
    output(a[0])
```

```tree
```

```out
```

```err
error: Cannot index 'int', it is not an array
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
    a int[] = [1, 2, 3]
    output(a[1.5])
```

```tree
```

```out
```

```err
error: Array index must be an integer, got float
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
    a int[] = [1, True, 3]
```

```tree
```

```out
```

```err
error: Array elements must all be the same type
  020.ura:2:19
  |
2 |     a int[] = [1, True, 3]
  |                   ^^^^
```

```ll
```

## 021 — array size is not an integer

```ura
main():
    a int[] = int[1.5]
```

```tree
```

```out
```

```err
error: Array size must be an integer
  021.ura:2:19
  |
2 |     a int[] = int[1.5]
  |                   ^^^
```

```ll
```

## 022 — clean on a non-array

```ura
main():
    x int = 5
    clean x
```

```tree
```

```out
```

```err
error: 'clean' expects an array
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
    a int[] = new int
```

```tree
```

```out
```

```err
error: Expected an array type after 'new' (e.g. new int[n])
  023.ura:2:15
  |
2 |     a int[] = new int
  |               ^^^
```

```ll
```

## 024 — missing closing bracket on index

```ura
main():
    a int[] = [1, 2, 3]
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
    a int[] = []
```

```tree
```

```out
```

```err
error: Empty array literal has no element type
  025.ura:2:15
  |
2 |     a int[] = []
  |               ^
```

```ll
```

## 026 — range bounds must be integers

```ura
main():
    a int[] = [1, 2, 3]
    mid int[] = a[1.5..3]
```

```tree
```

```out
```

```err
error: Range bounds must be integers
  026.ura:3:22
  |
3 |     mid int[] = a[1.5..3]
  |                      ^^
```

```ll
```

## 027 — .len on a non-array

```ura
main():
    x int = 5
    output(x.len)
```

```tree
```

```out
```

```err
error: '.len' is only valid on an array, not int
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
    a int[] = [1, 2, 3]
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
