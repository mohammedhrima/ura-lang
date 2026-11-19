# milestone8

## index

- 001 — array: 2D array literal and nested indexing
- 002 — array: sized 2D matrix i32[N][M], zero-init and writes
- 003 — array: 3D sized array with runtime size
- 004 — array: heap array: new<i32>(N), zero-init, write, free
- 005 — array: heap array with a runtime size, new<i32>(n) + free
- 006 — array: multi-dim heap: new<i32[]>(N), a row each, free each
- 008 — array: arr.len on a literal and on multi-dim
- 012 — array: .len on a non-array
- 013 — array: array literals: one element, expressions, and strings
- 014 — array: fill a heap char buffer by index, print it, free it
- 015 — enum: a variant as an array index and in arithmetic
- 016 — errput: errput follows a bound ref? and prints null when unbound
- 017 — heap: new<T>(1) gives one zeroed struct, a ref points at it
- 021 — heap: operator drop frees the nodes a tree owns
- 022 — heap: a function fills a heap struct and returns a ref to it
- 025 — lexer: an index with nothing in it
- 026 — lexer: :: must be followed by a name
- 027 — operator: operator drop declares a destructor named Vec.drop
- 029 — operator: calling .drop() yourself is rejected
- 030 — operator: operator drop beside operator = does not warn
- 031 — operator: a destructor fires on break and on continue
- 032 — operator: drop on fall-through: an if block, each iteration, reverse order
- 033 — operator: a struct drops the structs it owns, at any depth, but not what it borrows
- 034 — operator: a heap-owning struct: create, deep-copy operator = and drop
- 035 — operator: a growable string struct: nullable buffer, grow, join and drop
- 036 — operator: a struct owning heap buffers: deep-copy operator = and drop
- 037 — ref: ref return: create a node on the heap and return a reference to it
- 038 — ref: ref? return: one branch returns a reference, the other returns null
- 039 — ref: declare a ref and bind it to a ref-returning call
- 040 — ref: a ref-returning call passed straight into a ref parameter
- 041 — ref: a reference function must return a reference, not a bare value
- 042 — ref: a field read chained directly on a ref-returning call
- 043 — struct: a heap array of structs: new<Room>(N), then free
- 044 — struct: a 2D heap array of structs frees every inner slice
- 045 — struct: an unknown type as a parameter, a return type, or an array element
- 046 — struct: destructor must take no parameters
- 047 — struct: destructor must return void
- 048 — variable: global variables: scalars, arrays, forward reference, shadowing
- 049 — variable: a global struct is initialised and dropped like a local
- 050 — variable: a global is dropped on an early return too

---

## 001 — array: 2D array literal and nested indexing

```ura
// stdout: m[0][0]=1 m[1][1]=4
// stdout: m[0][1]=99
// expect: 0

fn main() i32:
    m i32[][] = [[1, 2], [3, 4]]
    output("m[0][0]=", m[0][0], " m[1][1]=", m[1][1], "\n")
    m[0][1] = 99
    output("m[0][1]=", m[0][1], "\n")
    return 0
```

---

## 002 — array: sized 2D matrix i32[N][M], zero-init and writes

```ura
// stdout: g[0][0]=1 g[2][3]=99 g[1][1]=0
// expect: 0

fn main() i32:
    g i32[][] = i32[3][4]
    g[0][0] = 1
    g[2][3] = 99
    output("g[0][0]=", g[0][0], " g[2][3]=", g[2][3], " g[1][1]=", g[1][1], "\n")
    return 0
```

---

## 003 — array: 3D sized array with runtime size

```ura
// stdout: v[1][1][1]=7 v[0][0][0]=0
// expect: 0

fn main() i32:
    n i32 = 2
    v i32[][][] = i32[n][n][n]
    v[1][1][1] = 7
    output("v[1][1][1]=", v[1][1][1], " v[0][0][0]=", v[0][0][0], "\n")
    return 0
```

---

## 004 — array: heap array: new<i32>(N), zero-init, write, free

```ura
// stdout: a[0]=0 a[2]=42
// expect: 0

fn main() i32:
    a i32[] = new<i32>(5)
    a[2] = 42
    output("a[0]=", a[0], " a[2]=", a[2], "\n")
    free<i32>(a)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [17 x i8] c"a[0]=%d a[2]=%d\0A\00", align 1

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %"call.new<i32>" = call i32* @"new<i32>"(i32 5)
  store i32* %"call.new<i32>", i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 2
  store i32 42, i32* %"a[]", align 4
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 0
  %"$a3" = load i32*, i32** %a, align 8
  %"a[]4" = getelementptr inbounds i32, i32* %"$a3", i32 2
  %"$a[]" = load i32, i32* %"a[]2", align 4
  %"$a[]5" = load i32, i32* %"a[]4", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]5")
  %"$a6" = load i32*, i32** %a, align 8
  call void @"free<i32>"(i32* %"$a6")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 005 — array: heap array with a runtime size, new<i32>(n) + free

```ura
// stdout: a[5]=7 a[0]=0
// expect: 0

fn main() i32:
    n i32 = 3
    a i32[] = new<i32>(n * 2)
    a[5] = 7
    output("a[5]=", a[5], " a[0]=", a[0], "\n")
    free<i32>(a)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [17 x i8] c"a[5]=%d a[0]=%d\0A\00", align 1

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %a = alloca i32*, align 8
  store i32 3, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 2
  %"call.new<i32>" = call i32* @"new<i32>"(i32 %MUL)
  store i32* %"call.new<i32>", i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 5
  store i32 7, i32* %"a[]", align 4
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 5
  %"$a3" = load i32*, i32** %a, align 8
  %"a[]4" = getelementptr inbounds i32, i32* %"$a3", i32 0
  %"$a[]" = load i32, i32* %"a[]2", align 4
  %"$a[]5" = load i32, i32* %"a[]4", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]5")
  %"$a6" = load i32*, i32** %a, align 8
  call void @"free<i32>"(i32* %"$a6")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 006 — array: multi-dim heap: new<i32[]>(N), a row each, free each

```ura
// stdout: m[0][0]=1 m[1][2]=9 m[0][1]=0
// expect: 0

fn main() i32:
    m i32[][] = new<i32[]>(2)
    m[0] = new<i32>(3)
    m[1] = new<i32>(3)
    m[0][0] = 1
    m[1][2] = 9
    output("m[0][0]=", m[0][0], " m[1][2]=", m[1][2], " m[0][1]=", m[0][1], "\n")
    free<i32>(m[0])
    free<i32>(m[1])
    free<i32[]>(m)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [34 x i8] c"m[0][0]=%d m[1][2]=%d m[0][1]=%d\0A\00", align 1

define i32** @"new<i32[]>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32**
  ret i32** %AS1
}

define void @"free<i32[]>"(i32** %0) {
entry:
  %data = alloca i32**, align 8
  store i32** %0, i32*** %data, align 8
  %"$data" = load i32**, i32*** %data, align 8
  %AS = bitcast i32** %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %m = alloca i32**, align 8
  %"call.new<i32[]>" = call i32** @"new<i32[]>"(i32 2)
  store i32** %"call.new<i32[]>", i32*** %m, align 8
  %"$m" = load i32**, i32*** %m, align 8
  %"m[]" = getelementptr inbounds i32*, i32** %"$m", i32 0
  %"call.new<i32>" = call i32* @"new<i32>"(i32 3)
  store i32* %"call.new<i32>", i32** %"m[]", align 8
  %"$m1" = load i32**, i32*** %m, align 8
  %"m[]2" = getelementptr inbounds i32*, i32** %"$m1", i32 1
  %"call.new<i32>3" = call i32* @"new<i32>"(i32 3)
  store i32* %"call.new<i32>3", i32** %"m[]2", align 8
  %"$m4" = load i32**, i32*** %m, align 8
  %"m[]5" = getelementptr inbounds i32*, i32** %"$m4", i32 0
  %"$m[]" = load i32*, i32** %"m[]5", align 8
  %"m[][]" = getelementptr inbounds i32, i32* %"$m[]", i32 0
  store i32 1, i32* %"m[][]", align 4
  %"$m6" = load i32**, i32*** %m, align 8
  %"m[]7" = getelementptr inbounds i32*, i32** %"$m6", i32 1
  %"$m[]8" = load i32*, i32** %"m[]7", align 8
  %"m[][]9" = getelementptr inbounds i32, i32* %"$m[]8", i32 2
  store i32 9, i32* %"m[][]9", align 4
  %"$m10" = load i32**, i32*** %m, align 8
  %"m[]11" = getelementptr inbounds i32*, i32** %"$m10", i32 0
  %"$m[]12" = load i32*, i32** %"m[]11", align 8
  %"m[][]13" = getelementptr inbounds i32, i32* %"$m[]12", i32 0
  %"$m14" = load i32**, i32*** %m, align 8
  %"m[]15" = getelementptr inbounds i32*, i32** %"$m14", i32 1
  %"$m[]16" = load i32*, i32** %"m[]15", align 8
  %"m[][]17" = getelementptr inbounds i32, i32* %"$m[]16", i32 2
  %"$m18" = load i32**, i32*** %m, align 8
  %"m[]19" = getelementptr inbounds i32*, i32** %"$m18", i32 0
  %"$m[]20" = load i32*, i32** %"m[]19", align 8
  %"m[][]21" = getelementptr inbounds i32, i32* %"$m[]20", i32 1
  %"$m[][]" = load i32, i32* %"m[][]13", align 4
  %"$m[][]22" = load i32, i32* %"m[][]17", align 4
  %"$m[][]23" = load i32, i32* %"m[][]21", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([34 x i8], [34 x i8]* @str, i32 0, i32 0), i32 %"$m[][]", i32 %"$m[][]22", i32 %"$m[][]23")
  %"$m24" = load i32**, i32*** %m, align 8
  %"m[]25" = getelementptr inbounds i32*, i32** %"$m24", i32 0
  %"$m[]26" = load i32*, i32** %"m[]25", align 8
  call void @"free<i32>"(i32* %"$m[]26")
  %"$m27" = load i32**, i32*** %m, align 8
  %"m[]28" = getelementptr inbounds i32*, i32** %"$m27", i32 1
  %"$m[]29" = load i32*, i32** %"m[]28", align 8
  call void @"free<i32>"(i32* %"$m[]29")
  %"$m30" = load i32**, i32*** %m, align 8
  call void @"free<i32[]>"(i32** %"$m30")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 008 — array: arr.len on a literal and on multi-dim

```ura
// stdout: len = 4
// stdout: rows = 3 cols = 4
// expect: 0

fn main() i32:
    nums i32[] = [10, 20, 30, 40]
    output("len = ", nums.len, "\n")
    grid i32[][] = i32[3][4]
    output("rows = ", grid.len, " cols = ", grid[0].len, "\n")
    return 0
```

---

## 012 — array: .len on a non-array

```ura
// error: '.len' is only valid on an array, not i32
fn main() i32:
    x i32 = 5
    output(x.len)
    return 0
```

### stderr

```
error: test.ura:4:12 'x' is 'i32', not a struct
  |
4 |     output(x.len)
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 013 — array: array literals: one element, expressions, and strings

```ura
// stdout: 42 len 1
// stdout: 5 6 10 10
// stdout: ada grace alan len 3
// expect: 0

fn twice(n i32) i32:
    return n * 2

fn main() i32:
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
    return 0
```

---

## 014 — array: fill a heap char buffer by index, print it, free it

```ura
// stdout: DUNGEON
// expect: 0

fn main() i32:
    str char[] = new<char>(8)
    str[0] = 'D'
    str[1] = 'U'
    str[2] = 'N'
    str[3] = 'G'
    str[4] = 'E'
    str[5] = 'O'
    str[6] = 'N'
    str[7] = 0 as char
    output(str, "\n")
    free<char>(str)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %"call.new<i8>" = call i8* @"new<i8>"(i32 8)
  store i8* %"call.new<i8>", i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %"str[]" = getelementptr inbounds i8, i8* %"$str", i32 0
  store i8 68, i8* %"str[]", align 1
  %"$str1" = load i8*, i8** %str, align 8
  %"str[]2" = getelementptr inbounds i8, i8* %"$str1", i32 1
  store i8 85, i8* %"str[]2", align 1
  %"$str3" = load i8*, i8** %str, align 8
  %"str[]4" = getelementptr inbounds i8, i8* %"$str3", i32 2
  store i8 78, i8* %"str[]4", align 1
  %"$str5" = load i8*, i8** %str, align 8
  %"str[]6" = getelementptr inbounds i8, i8* %"$str5", i32 3
  store i8 71, i8* %"str[]6", align 1
  %"$str7" = load i8*, i8** %str, align 8
  %"str[]8" = getelementptr inbounds i8, i8* %"$str7", i32 4
  store i8 69, i8* %"str[]8", align 1
  %"$str9" = load i8*, i8** %str, align 8
  %"str[]10" = getelementptr inbounds i8, i8* %"$str9", i32 5
  store i8 79, i8* %"str[]10", align 1
  %"$str11" = load i8*, i8** %str, align 8
  %"str[]12" = getelementptr inbounds i8, i8* %"$str11", i32 6
  store i8 78, i8* %"str[]12", align 1
  %"$str13" = load i8*, i8** %str, align 8
  %"str[]14" = getelementptr inbounds i8, i8* %"$str13", i32 7
  store i8 0, i8* %"str[]14", align 1
  %"$str15" = load i8*, i8** %str, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$str15")
  %"$str16" = load i8*, i8** %str, align 8
  call void @"free<i8>"(i8* %"$str16")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 015 — enum: a variant as an array index and in arithmetic

```ura
// stdout: 14 2
// expect: 0
enum Slot: HEAD, BODY, LEGS

fn main() i32:
    gear i32[] = new<i32>(3)
    gear[Slot::HEAD] = 5
    gear[Slot::LEGS] = 9
    output(gear[Slot::HEAD] + gear[Slot::LEGS], " ", Slot::LEGS - Slot::HEAD, "\n")
    free<i32>(gear)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %gear = alloca i32*, align 8
  %"call.new<i32>" = call i32* @"new<i32>"(i32 3)
  store i32* %"call.new<i32>", i32** %gear, align 8
  %"$gear" = load i32*, i32** %gear, align 8
  %"gear[]" = getelementptr inbounds i32, i32* %"$gear", i32 0
  store i32 5, i32* %"gear[]", align 4
  %"$gear1" = load i32*, i32** %gear, align 8
  %"gear[]2" = getelementptr inbounds i32, i32* %"$gear1", i32 2
  store i32 9, i32* %"gear[]2", align 4
  %"$gear3" = load i32*, i32** %gear, align 8
  %"gear[]4" = getelementptr inbounds i32, i32* %"$gear3", i32 0
  %"$gear5" = load i32*, i32** %gear, align 8
  %"gear[]6" = getelementptr inbounds i32, i32* %"$gear5", i32 2
  %"$gear[]" = load i32, i32* %"gear[]4", align 4
  %"$gear[]7" = load i32, i32* %"gear[]6", align 4
  %ADD = add i32 %"$gear[]", %"$gear[]7"
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %ADD, i32 2)
  %"$gear8" = load i32*, i32** %gear, align 8
  call void @"free<i32>"(i32* %"$gear8")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 016 — errput: errput follows a bound ref? and prints null when unbound

```ura
// stderr: [0;31munbound Node{value: 1, next: null}
// expect: 1

struct Node:
    value i32
    next &Node

fn main() i32:
    head Node
    head.value = 1
    errput("unbound ", head, "\n")
    tail Node
    tail.value = 2
    head.next = &tail
    errput("bound ", head, "\n")
    return 0
```

---

## 017 — heap: new<T>(1) gives one zeroed struct, a ref points at it

```ura
// stdout: 0 0
// stdout: 7
// expect: 0
struct P:
    a i32
    b i32

fn main() i32:
    ps P[] = new<P>(1)
    p &P = &ps[0]
    output(p.a, " ", p.b, "\n")
    p.a = 7
    output(p.a, "\n")
    free<P>(ps)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%P = type { i32, i32 }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define %P* @"new<P>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", trunc (i64 mul nuw (i64 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i64), i64 2) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %P*
  ret %P* %AS1
}

define void @"free<P>"(%P* %0) {
entry:
  %data = alloca %P*, align 8
  store %P* %0, %P** %data, align 8
  %"$data" = load %P*, %P** %data, align 8
  %AS = bitcast %P* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %ps = alloca %P*, align 8
  %p = alloca %P*, align 8
  %"call.new<P>" = call %P* @"new<P>"(i32 1)
  store %P* %"call.new<P>", %P** %ps, align 8
  %"$ps" = load %P*, %P** %ps, align 8
  %"ps[]" = getelementptr inbounds %P, %P* %"$ps", i32 0
  store %P* %"ps[]", %P** %p, align 8
  %"*p" = load %P*, %P** %p, align 8
  %"*p.a" = getelementptr inbounds %P, %P* %"*p", i32 0, i32 0
  %"*p1" = load %P*, %P** %p, align 8
  %"*p.b" = getelementptr inbounds %P, %P* %"*p1", i32 0, i32 1
  %"$*p.a" = load i32, i32* %"*p.a", align 4
  %"$*p.b" = load i32, i32* %"*p.b", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$*p.a", i32 %"$*p.b")
  %"*p2" = load %P*, %P** %p, align 8
  %"*p.a3" = getelementptr inbounds %P, %P* %"*p2", i32 0, i32 0
  store i32 7, i32* %"*p.a3", align 4
  %"*p4" = load %P*, %P** %p, align 8
  %"*p.a5" = getelementptr inbounds %P, %P* %"*p4", i32 0, i32 0
  %"$*p.a6" = load i32, i32* %"*p.a5", align 4
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$*p.a6")
  %"$ps8" = load %P*, %P** %ps, align 8
  call void @"free<P>"(%P* %"$ps8")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 021 — heap: operator drop frees the nodes a tree owns

```ura
// stdout: 1 2 3
// stdout: freed
// expect: 0
struct Node:
    value i32
    left &Node
    right &Node

struct Tree:
    nodes Node[]

    pub fn create() Tree:
        t Tree
        t.nodes = new<Node>(3)
        return t

    operator drop():
        free<Node>(self.nodes)
        output("freed\n")

fn main() i32:
    tree Tree = Tree::create()
    root &Node = &tree.nodes[0]
    root.value = 1
    root.left = &tree.nodes[1]
    root.left.value = 2
    root.right = &tree.nodes[2]
    root.right.value = 3
    output(root.value, " ", root.left.value, " ", root.right.value, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node*, %Node* }
%Tree = type { %Node* }

@str = private unnamed_addr constant [7 x i8] c"freed\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

define %Node* @"new<Node>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Node*
  ret %Node* %AS1
}

define void @"free<Node>"(%Node* %0) {
entry:
  %data = alloca %Node*, align 8
  store %Node* %0, %Node** %data, align 8
  %"$data" = load %Node*, %Node** %data, align 8
  %AS = bitcast %Node* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define %Tree @Tree.create() {
entry:
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  %t.nodes = getelementptr inbounds %Tree, %Tree* %t, i32 0, i32 0
  %"call.new<Node>" = call %Node* @"new<Node>"(i32 3)
  store %Node* %"call.new<Node>", %Node** %t.nodes, align 8
  %"$t" = load %Tree, %Tree* %t, align 8
  ret %Tree %"$t"
}

define void @Tree.drop(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  store %Tree* %0, %Tree** %self, align 8
  %"*self" = load %Tree*, %Tree** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Tree, %Tree* %"*self", i32 0, i32 0
  %"$*self.nodes" = load %Node*, %Node** %"*self.nodes", align 8
  call void @"free<Node>"(%Node* %"$*self.nodes")
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  %tree = alloca %Tree, align 8
  %root = alloca %Node*, align 8
  %call.Tree.create = call %Tree @Tree.create()
  store %Tree %call.Tree.create, %Tree* %tree, align 8
  %tree.nodes = getelementptr inbounds %Tree, %Tree* %tree, i32 0, i32 0
  %"$tree.nodes" = load %Node*, %Node** %tree.nodes, align 8
  %"tree.nodes[]" = getelementptr inbounds %Node, %Node* %"$tree.nodes", i32 0
  store %Node* %"tree.nodes[]", %Node** %root, align 8
  %"*root" = load %Node*, %Node** %root, align 8
  %"*root.value" = getelementptr inbounds %Node, %Node* %"*root", i32 0, i32 0
  store i32 1, i32* %"*root.value", align 4
  %"*root1" = load %Node*, %Node** %root, align 8
  %"*root.left" = getelementptr inbounds %Node, %Node* %"*root1", i32 0, i32 1
  %tree.nodes2 = getelementptr inbounds %Tree, %Tree* %tree, i32 0, i32 0
  %"$tree.nodes3" = load %Node*, %Node** %tree.nodes2, align 8
  %"tree.nodes[]4" = getelementptr inbounds %Node, %Node* %"$tree.nodes3", i32 1
  store %Node* %"tree.nodes[]4", %Node** %"*root.left", align 8
  %"*root5" = load %Node*, %Node** %root, align 8
  %"*root.left6" = getelementptr inbounds %Node, %Node* %"*root5", i32 0, i32 1
  %"**root.left" = load %Node*, %Node** %"*root.left6", align 8
  %"**root.left.value" = getelementptr inbounds %Node, %Node* %"**root.left", i32 0, i32 0
  store i32 2, i32* %"**root.left.value", align 4
  %"*root7" = load %Node*, %Node** %root, align 8
  %"*root.right" = getelementptr inbounds %Node, %Node* %"*root7", i32 0, i32 2
  %tree.nodes8 = getelementptr inbounds %Tree, %Tree* %tree, i32 0, i32 0
  %"$tree.nodes9" = load %Node*, %Node** %tree.nodes8, align 8
  %"tree.nodes[]10" = getelementptr inbounds %Node, %Node* %"$tree.nodes9", i32 2
  store %Node* %"tree.nodes[]10", %Node** %"*root.right", align 8
  %"*root11" = load %Node*, %Node** %root, align 8
  %"*root.right12" = getelementptr inbounds %Node, %Node* %"*root11", i32 0, i32 2
  %"**root.right" = load %Node*, %Node** %"*root.right12", align 8
  %"**root.right.value" = getelementptr inbounds %Node, %Node* %"**root.right", i32 0, i32 0
  store i32 3, i32* %"**root.right.value", align 4
  %"*root13" = load %Node*, %Node** %root, align 8
  %"*root.value14" = getelementptr inbounds %Node, %Node* %"*root13", i32 0, i32 0
  %"*root15" = load %Node*, %Node** %root, align 8
  %"*root.left16" = getelementptr inbounds %Node, %Node* %"*root15", i32 0, i32 1
  %"**root.left17" = load %Node*, %Node** %"*root.left16", align 8
  %"**root.left.value18" = getelementptr inbounds %Node, %Node* %"**root.left17", i32 0, i32 0
  %"*root19" = load %Node*, %Node** %root, align 8
  %"*root.right20" = getelementptr inbounds %Node, %Node* %"*root19", i32 0, i32 2
  %"**root.right21" = load %Node*, %Node** %"*root.right20", align 8
  %"**root.right.value22" = getelementptr inbounds %Node, %Node* %"**root.right21", i32 0, i32 0
  %"$*root.value" = load i32, i32* %"*root.value14", align 4
  %"$**root.left.value" = load i32, i32* %"**root.left.value18", align 4
  %"$**root.right.value" = load i32, i32* %"**root.right.value22", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$*root.value", i32 %"$**root.left.value", i32 %"$**root.right.value")
  call void @Tree.drop(%Tree* %tree)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 022 — heap: a function fills a heap struct and returns a ref to it

```ura
// stdout: 9
// expect: 0
struct P:
    v i32

fn make(x i32, ps P[]) &P:
    ps[0].v = x
    return &ps[0]

fn main() i32:
    ps P[] = new<P>(1)
    q &P = make(9, ps)
    output(q.v, "\n")
    free<P>(ps)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%P = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define %P* @make(i32 %0, %P* %1) {
entry:
  %x = alloca i32, align 4
  %ps = alloca %P*, align 8
  store i32 %0, i32* %x, align 4
  store %P* %1, %P** %ps, align 8
  %"$ps" = load %P*, %P** %ps, align 8
  %"ps[]" = getelementptr inbounds %P, %P* %"$ps", i32 0
  %"ps[].v" = getelementptr inbounds %P, %P* %"ps[]", i32 0, i32 0
  %"$x" = load i32, i32* %x, align 4
  store i32 %"$x", i32* %"ps[].v", align 4
  %"$ps1" = load %P*, %P** %ps, align 8
  %"ps[]2" = getelementptr inbounds %P, %P* %"$ps1", i32 0
  ret %P* %"ps[]2"
}

define %P* @"new<P>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %P*
  ret %P* %AS1
}

define void @"free<P>"(%P* %0) {
entry:
  %data = alloca %P*, align 8
  store %P* %0, %P** %data, align 8
  %"$data" = load %P*, %P** %data, align 8
  %AS = bitcast %P* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %ps = alloca %P*, align 8
  %q = alloca %P*, align 8
  %ref = alloca %P*, align 8
  %"call.new<P>" = call %P* @"new<P>"(i32 1)
  store %P* %"call.new<P>", %P** %ps, align 8
  %"$ps" = load %P*, %P** %ps, align 8
  %call.make = call %P* @make(i32 9, %P* %"$ps")
  store %P* %call.make, %P** %ref, align 8
  %"*ref" = load %P*, %P** %ref, align 8
  store %P* %"*ref", %P** %q, align 8
  %"*q" = load %P*, %P** %q, align 8
  %"*q.v" = getelementptr inbounds %P, %P* %"*q", i32 0, i32 0
  %"$*q.v" = load i32, i32* %"*q.v", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*q.v")
  %"$ps1" = load %P*, %P** %ps, align 8
  call void @"free<P>"(%P* %"$ps1")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 025 — lexer: an index with nothing in it

```ura
// error: Unexpected token

fn main() i32:
    arr i32[] = new<i32>(3)
    x i32 = arr[]
    return 0
```

### stderr

```
error: test.ura:5:17 unexpected ']'
  |
5 |     x i32 = arr[]
  |                 ^
error: test.ura:5:17 expected ']' after the index
  |
5 |     x i32 = arr[]
  |                 ^
help: use x i32 = arr[]]
error: aborting due to 2 errors
```

### status

```
1
```

---

## 026 — lexer: :: must be followed by a name

```ura
// error: Expected a name after '::'
use "@/header"

fn main() i32:
    s String = String::
    return 0
```

### stderr

```
error: test.ura:5:22 expected a value after '::'
  |
5 |     s String = String::
  |                      ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 027 — operator: operator drop declares a destructor named Vec.drop

```ura
// stdout: done
// stdout: dropping 7
// expect: 0

struct Vec:
    x i32

    operator drop():
        output("dropping ", self.x, "\n")

fn main() i32:
    v Vec
    v.x = 7
    output("done\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32 }

@str = private unnamed_addr constant [13 x i8] c"dropping %d\0A\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1

define void @Vec.drop(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$*self.x")
  ret void
}

define i32 @main() {
entry:
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  store i32 7, i32* %v.x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  call void @Vec.drop(%Vec* %v)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 029 — operator: calling .drop() yourself is rejected

```ura
// error: operator drop runs by itself; write drop(v) to drop early

struct Vec:
    x i32

    operator drop():
        output("dropping\n")

fn main() i32:
    v Vec
    v.drop()
    return 0
```

### stderr

```
error: test.ura:11:7 operator drop is not a method
   |
11 |     v.drop()
   |       ^^^^
help: write drop(v) to drop it early
error: aborting due to 1 error
```

### status

```
1
```

---

## 030 — operator: operator drop beside operator = does not warn

```ura
// stdout: done
// stdout: drop 5
// expect: 0

struct Vec:
    x i32

    operator =(other &Vec) void:
        self.x = other.x

    operator drop():
        output("drop ", self.x, "\n")

fn main() i32:
    a Vec
    a.x = 5
    output("done\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32 }

@str = private unnamed_addr constant [9 x i8] c"drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1

define void @Vec.drop(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.x")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %a.x = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store i32 5, i32* %a.x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  call void @Vec.drop(%Vec* %a)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 031 — operator: a destructor fires on break and on continue

```ura
// stdout: drop 100
// stdout: drop 101
// stdout: after break
// stdout: drop 200
// stdout: drop 201
// stdout: after continue
// expect: 0

struct Tag:
    id i32

    pub fn create(n i32) Tag:
        t Tag
        t.id = n
        return t

    operator =(other &Tag) void:
        self.id = other.id

    operator drop():
        output("drop ", self.id, "\n")

fn main() i32:
    i i32 = 0
    while i < 3:
        b Tag = Tag::create(100 + i)
        if i == 1:
            break
        i = i + 1
    output("after break\n")

    k i32 = 0
    while k < 2:
        c Tag = Tag::create(200 + k)
        k = k + 1
        continue
    output("after continue\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Tag = type { i32 }

@str = private unnamed_addr constant [9 x i8] c"drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"after break\0A\00", align 1
@str.2 = private unnamed_addr constant [16 x i8] c"after continue\0A\00", align 1

define %Tag @Tag.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %t = alloca %Tag, align 8
  store i32 %0, i32* %n, align 4
  store %Tag zeroinitializer, %Tag* %t, align 4
  %t.id = getelementptr inbounds %Tag, %Tag* %t, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %t.id, align 4
  %"$t" = load %Tag, %Tag* %t, align 4
  ret %Tag %"$t"
}

define void @Tag.drop(%Tag* %0) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %"*self" = load %Tag*, %Tag** %self, align 8
  %"*self.id" = getelementptr inbounds %Tag, %Tag* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  ret void
}

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %b = alloca %Tag, align 8
  %k = alloca i32, align 4
  %c = alloca %Tag, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 100, %"$i2"
  %call.Tag.create = call %Tag @Tag.create(i32 %ADD)
  store %Tag %call.Tag.create, %Tag* %b, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i3" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i3", 1
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  call void @Tag.drop(%Tag* %b)
  br label %endloop

endif:                                            ; preds = %if1
  %"$i4" = load i32, i32* %i, align 4
  %ADD5 = add i32 %"$i4", 1
  store i32 %ADD5, i32* %i, align 4
  call void @Tag.drop(%Tag* %b)
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0))
  store i32 0, i32* %k, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %endloop
  %"$k" = load i32, i32* %k, align 4
  %LT6 = icmp slt i32 %"$k", 2
  br i1 %LT6, label %while2.then, label %endloop1

while2.then:                                      ; preds = %while2
  %"$k7" = load i32, i32* %k, align 4
  %ADD8 = add i32 200, %"$k7"
  %call.Tag.create9 = call %Tag @Tag.create(i32 %ADD8)
  store %Tag %call.Tag.create9, %Tag* %c, align 4
  %"$k10" = load i32, i32* %k, align 4
  %ADD11 = add i32 %"$k10", 1
  store i32 %ADD11, i32* %k, align 4
  call void @Tag.drop(%Tag* %c)
  br label %while2

endloop1:                                         ; preds = %while2
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 032 — operator: drop on fall-through: an if block, each iteration, reverse order

```ura
// stdout: inside
// stdout: drop 1
// stdout: after if
// stdout: drop 10
// stdout: drop 11
// stdout: drop 12
// stdout: after loop
// stdout: end of main
// stdout: drop 8
// stdout: drop 7
// expect: 0

struct Tag:
    id i32

    pub fn create(n i32) Tag:
        t Tag
        t.id = n
        return t

    operator =(other &Tag) void:
        self.id = other.id

    operator drop():
        output("drop ", self.id, "\n")

fn main() i32:
    if 1 < 2:
        x Tag = Tag::create(1)
        output("inside\n")
    output("after if\n")

    i i32 = 0
    while i < 3:
        y Tag = Tag::create(10 + i)
        i += 1
    output("after loop\n")

    p Tag = Tag::create(7)
    q Tag = Tag::create(8)
    output("end of main\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Tag = type { i32 }

@str = private unnamed_addr constant [9 x i8] c"drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"inside\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"after if\0A\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"after loop\0A\00", align 1
@str.4 = private unnamed_addr constant [13 x i8] c"end of main\0A\00", align 1

define %Tag @Tag.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %t = alloca %Tag, align 8
  store i32 %0, i32* %n, align 4
  store %Tag zeroinitializer, %Tag* %t, align 4
  %t.id = getelementptr inbounds %Tag, %Tag* %t, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %t.id, align 4
  %"$t" = load %Tag, %Tag* %t, align 4
  ret %Tag %"$t"
}

define void @Tag.drop(%Tag* %0) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %"*self" = load %Tag*, %Tag** %self, align 8
  %"*self.id" = getelementptr inbounds %Tag, %Tag* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  ret void
}

define i32 @main() {
entry:
  %x = alloca %Tag, align 8
  %i = alloca i32, align 4
  %y = alloca %Tag, align 8
  %p = alloca %Tag, align 8
  %q = alloca %Tag, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %call.Tag.create = call %Tag @Tag.create(i32 1)
  store %Tag %call.Tag.create, %Tag* %x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0))
  call void @Tag.drop(%Tag* %x)
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0))
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 10, %"$i2"
  %call.Tag.create3 = call %Tag @Tag.create(i32 %ADD)
  store %Tag %call.Tag.create3, %Tag* %y, align 4
  %"$i4" = load i32, i32* %i, align 4
  %ADD5 = add i32 %"$i4", 1
  store i32 %ADD5, i32* %i, align 4
  call void @Tag.drop(%Tag* %y)
  br label %while1

endloop:                                          ; preds = %while1
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0))
  %call.Tag.create7 = call %Tag @Tag.create(i32 7)
  store %Tag %call.Tag.create7, %Tag* %p, align 4
  %call.Tag.create8 = call %Tag @Tag.create(i32 8)
  store %Tag %call.Tag.create8, %Tag* %q, align 4
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.4, i32 0, i32 0))
  call void @Tag.drop(%Tag* %q)
  call void @Tag.drop(%Tag* %p)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 033 — operator: a struct drops the structs it owns, at any depth, but not what it borrows

```ura
// stdout: built
// stdout: drop 1
// stdout: drop 3
// stdout: drop 9
// expect: 0

struct Res:
    id i32

    pub fn create(n i32) Res:
        r Res
        r.id = n
        return r

    operator =(other &Res) void:
        self.id = other.id

    operator drop():
        if self.id != 0:
            output("drop ", self.id, "\n")

struct L3:
    res Res
struct L2:
    inner L3

// Owner declares no destructor of its own — everything below still drops
struct Owner:
    mid  L2
    also Res
    borrowed &Res

    pub fn create() Owner:
        o Owner
        o.mid.inner.res = Res::create(3)
        o.also          = Res::create(1)
        return o

fn main() i32:
    lent Res = Res::create(9)
    o Owner = Owner::create()
    o.borrowed = &lent
    output("built\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Res = type { i32 }
%Owner = type { %L2, %Res, %Res* }
%L2 = type { %L3 }
%L3 = type { %Res }

@str = private unnamed_addr constant [9 x i8] c"drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"built\0A\00", align 1

define %Res @Res.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %r = alloca %Res, align 8
  store i32 %0, i32* %n, align 4
  store %Res zeroinitializer, %Res* %r, align 4
  %r.id = getelementptr inbounds %Res, %Res* %r, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %r.id, align 4
  %"$r" = load %Res, %Res* %r, align 4
  ret %Res %"$r"
}

define void @Res.drop(%Res* %0) {
entry:
  %self = alloca %Res*, align 8
  store %Res* %0, %Res** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %Res*, %Res** %self, align 8
  %"*self.id" = getelementptr inbounds %Res, %Res* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %NQ = icmp ne i32 %"$*self.id", 0
  br i1 %NQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %Res*, %Res** %self, align 8
  %"*self.id2" = getelementptr inbounds %Res, %Res* %"*self1", i32 0, i32 0
  %"$*self.id3" = load i32, i32* %"*self.id2", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id3")
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  ret void
}

define %Owner @Owner.create() {
entry:
  %o = alloca %Owner, align 8
  store %Owner zeroinitializer, %Owner* %o, align 8
  %o.mid = getelementptr inbounds %Owner, %Owner* %o, i32 0, i32 0
  %o.mid.inner = getelementptr inbounds %L2, %L2* %o.mid, i32 0, i32 0
  %o.mid.inner.res = getelementptr inbounds %L3, %L3* %o.mid.inner, i32 0, i32 0
  %call.Res.create = call %Res @Res.create(i32 3)
  call void @Res.drop(%Res* %o.mid.inner.res)
  store %Res %call.Res.create, %Res* %o.mid.inner.res, align 4
  %o.also = getelementptr inbounds %Owner, %Owner* %o, i32 0, i32 1
  %call.Res.create1 = call %Res @Res.create(i32 1)
  call void @Res.drop(%Res* %o.also)
  store %Res %call.Res.create1, %Res* %o.also, align 4
  %"$o" = load %Owner, %Owner* %o, align 8
  ret %Owner %"$o"
}

define i32 @main() {
entry:
  %lent = alloca %Res, align 8
  %o = alloca %Owner, align 8
  %call.Res.create = call %Res @Res.create(i32 9)
  store %Res %call.Res.create, %Res* %lent, align 4
  %call.Owner.create = call %Owner @Owner.create()
  store %Owner %call.Owner.create, %Owner* %o, align 8
  %o.borrowed = getelementptr inbounds %Owner, %Owner* %o, i32 0, i32 2
  store %Res* %lent, %Res** %o.borrowed, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0))
  %o.also = getelementptr inbounds %Owner, %Owner* %o, i32 0, i32 1
  call void @Res.drop(%Res* %o.also)
  %o.mid = getelementptr inbounds %Owner, %Owner* %o, i32 0, i32 0
  %o.mid.inner = getelementptr inbounds %L2, %L2* %o.mid, i32 0, i32 0
  %o.mid.inner.res = getelementptr inbounds %L3, %L3* %o.mid.inner, i32 0, i32 0
  call void @Res.drop(%Res* %o.mid.inner.res)
  call void @Res.drop(%Res* %lent)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 034 — operator: a heap-owning struct: create, deep-copy operator = and drop

```ura
// stdout: b1: hello
// stdout: b2: world
// expect: 0

use "@/string"

struct Buffer:
    value char[]

    pub fn create() Buffer:
        b Buffer
        b.value = new<char>(64)
        return b

    operator drop():
        free<char>(self.value)

    operator = (s char[]) void:
        strcpy(self.value, s)

    operator = (b &Buffer) void:
        free<char>(self.value)
        self.value = new<char>(64)
        strcpy(self.value, b.value)

fn main() i32:
    b1 Buffer = Buffer::create()
    b1 = "hello"

    b2 Buffer = Buffer::create()
    b2 = b1

    strcpy(b2.value, "world")

    output("b1: ", b1.value, "\n")
    output("b2: ", b2.value, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Buffer = type { i8* }

@str = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"b1: %s\0A\00", align 1
@str.4 = private unnamed_addr constant [8 x i8] c"b2: %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

declare i8* @strcpy(i8*, i8*)

define %Buffer @Buffer.create() {
entry:
  %b = alloca %Buffer, align 8
  store %Buffer zeroinitializer, %Buffer* %b, align 8
  %b.value = getelementptr inbounds %Buffer, %Buffer* %b, i32 0, i32 0
  %"call.new<i8>" = call i8* @"new<i8>"(i32 64)
  store i8* %"call.new<i8>", i8** %b.value, align 8
  %"$b" = load %Buffer, %Buffer* %b, align 8
  ret %Buffer %"$b"
}

define void @Buffer.drop(%Buffer* %0) {
entry:
  %self = alloca %Buffer*, align 8
  store %Buffer* %0, %Buffer** %self, align 8
  %"*self" = load %Buffer*, %Buffer** %self, align 8
  %"*self.value" = getelementptr inbounds %Buffer, %Buffer* %"*self", i32 0, i32 0
  %"$*self.value" = load i8*, i8** %"*self.value", align 8
  call void @"free<i8>"(i8* %"$*self.value")
  ret void
}

define void @"Buffer.="(%Buffer* %0, i8* %1) {
entry:
  %self = alloca %Buffer*, align 8
  %s = alloca i8*, align 8
  store %Buffer* %0, %Buffer** %self, align 8
  store i8* %1, i8** %s, align 8
  %"*self" = load %Buffer*, %Buffer** %self, align 8
  %"*self.value" = getelementptr inbounds %Buffer, %Buffer* %"*self", i32 0, i32 0
  %"$*self.value" = load i8*, i8** %"*self.value", align 8
  %"$s" = load i8*, i8** %s, align 8
  %call.strcpy = call i8* @strcpy(i8* %"$*self.value", i8* %"$s")
  ret void
}

define void @"Buffer.=.1"(%Buffer* %0, %Buffer* %1) {
entry:
  %self = alloca %Buffer*, align 8
  %b = alloca %Buffer*, align 8
  store %Buffer* %0, %Buffer** %self, align 8
  store %Buffer* %1, %Buffer** %b, align 8
  %"*self" = load %Buffer*, %Buffer** %self, align 8
  %"*self.value" = getelementptr inbounds %Buffer, %Buffer* %"*self", i32 0, i32 0
  %"$*self.value" = load i8*, i8** %"*self.value", align 8
  call void @"free<i8>"(i8* %"$*self.value")
  %"*self1" = load %Buffer*, %Buffer** %self, align 8
  %"*self.value2" = getelementptr inbounds %Buffer, %Buffer* %"*self1", i32 0, i32 0
  %"call.new<i8>" = call i8* @"new<i8>"(i32 64)
  store i8* %"call.new<i8>", i8** %"*self.value2", align 8
  %"*self3" = load %Buffer*, %Buffer** %self, align 8
  %"*self.value4" = getelementptr inbounds %Buffer, %Buffer* %"*self3", i32 0, i32 0
  %"*b" = load %Buffer*, %Buffer** %b, align 8
  %"*b.value" = getelementptr inbounds %Buffer, %Buffer* %"*b", i32 0, i32 0
  %"$*self.value5" = load i8*, i8** %"*self.value4", align 8
  %"$*b.value" = load i8*, i8** %"*b.value", align 8
  %call.strcpy = call i8* @strcpy(i8* %"$*self.value5", i8* %"$*b.value")
  ret void
}

define i32 @main() {
entry:
  %b1 = alloca %Buffer, align 8
  %b2 = alloca %Buffer, align 8
  %copy = alloca %Buffer, align 8
  %call.Buffer.create = call %Buffer @Buffer.create()
  store %Buffer %call.Buffer.create, %Buffer* %b1, align 8
  call void @"Buffer.="(%Buffer* %b1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  %call.Buffer.create1 = call %Buffer @Buffer.create()
  store %Buffer %call.Buffer.create1, %Buffer* %b2, align 8
  store %Buffer zeroinitializer, %Buffer* %copy, align 8
  call void @"Buffer.=.1"(%Buffer* %copy, %Buffer* %b1)
  %"$copy" = load %Buffer, %Buffer* %copy, align 8
  call void @Buffer.drop(%Buffer* %b2)
  store %Buffer %"$copy", %Buffer* %b2, align 8
  %b2.value = getelementptr inbounds %Buffer, %Buffer* %b2, i32 0, i32 0
  %"$b2.value" = load i8*, i8** %b2.value, align 8
  %call.strcpy = call i8* @strcpy(i8* %"$b2.value", i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  %b1.value = getelementptr inbounds %Buffer, %Buffer* %b1, i32 0, i32 0
  %"$b1.value" = load i8*, i8** %b1.value, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i8* %"$b1.value")
  %b2.value2 = getelementptr inbounds %Buffer, %Buffer* %b2, i32 0, i32 0
  %"$b2.value3" = load i8*, i8** %b2.value2, align 8
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.4, i32 0, i32 0), i8* %"$b2.value3")
  call void @Buffer.drop(%Buffer* %b2)
  call void @Buffer.drop(%Buffer* %b1)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 035 — operator: a growable string struct: nullable buffer, grow, join and drop

```ura
// stdout: s0:    1x2345 (6)
// stdout: s1:    6x789 (5)
// stdout: s0+s1: 1x23456x789 (11)
// expect: 0

use "@/header"

struct String2:
    value char[]
    count i32
    size  i32

    pub fn create() String2:
        s String2
        s.size  = 10
        s.value = new<char>(10)
        return s

    operator drop():
        free<char>(self.value)

    fn grow(want i32):
        if self.size >= want: return
        if self.size == 0: self.size = 10
        while self.size < want:
            self.size *= 2
        bigger char[] = new<char>(self.size)
        if self.count > 0: strcpy(bigger, self.value)
        free<char>(self.value)
        self.value = bigger

    fn assign(str char[]):
        len i32 = strlen(str) as i32
        self.grow(len + 1)
        bzero(self.value, self.size as i64)
        strcpy(self.value, str)
        self.count = len

    fn join(str char[]):
        len i32 = strlen(str) as i32
        self.grow(self.count + len + 1)
        strcat(self.value, str)
        self.count += len

    operator = (str char[]) void:
        self.assign(str)

    operator = (v &String2) void:
        self.assign(v.value)

fn add(v &String2, w &String2) String2:
    res String2 = String2::create()
    res = v
    res.join(w.value)
    return res

fn main() i32:
    s0 String2 = String2::create()
    s0 = "1x2345"

    s1 String2 = String2::create()
    s1 = "6x789"

    s2 String2 = String2::create()
    s2 = add(&s0, &s1)

    output("s0:    ", s0.value, " (", s0.count, ")\n")
    output("s1:    ", s1.value, " (", s1.count, ")\n")
    output("s0+s1: ", s2.value, " (", s2.count, ")\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String2 = type { i8*, i32, i32 }

@EPERM = global i32 0
@ENOENT = global i32 0
@ESRCH = global i32 0
@EINTR = global i32 0
@EIO = global i32 0
@ENXIO = global i32 0
@E2BIG = global i32 0
@ENOEXEC = global i32 0
@EBADF = global i32 0
@ECHILD = global i32 0
@EACCES = global i32 0
@EFAULT = global i32 0
@ENOTBLK = global i32 0
@EBUSY = global i32 0
@EEXIST = global i32 0
@EXDEV = global i32 0
@ENODEV = global i32 0
@ENOTDIR = global i32 0
@EISDIR = global i32 0
@EINVAL = global i32 0
@ENFILE = global i32 0
@EMFILE = global i32 0
@ENOTTY = global i32 0
@ETXTBSY = global i32 0
@EFBIG = global i32 0
@ENOSPC = global i32 0
@ESPIPE = global i32 0
@EROFS = global i32 0
@EMLINK = global i32 0
@EPIPE = global i32 0
@EDOM = global i32 0
@ERANGE = global i32 0
@O_RDONLY = global i32 0
@O_WRONLY = global i32 0
@O_RDWR = global i32 0
@FD_CLOEXEC = global i32 0
@IPPROTO_IP = global i32 0
@IPPROTO_TCP = global i32 0
@IPPROTO_UDP = global i32 0
@IPPROTO_IPV6 = global i32 0
@SHUT_RD = global i32 0
@SHUT_WR = global i32 0
@SHUT_RDWR = global i32 0
@POLLIN = global i32 0
@POLLPRI = global i32 0
@POLLOUT = global i32 0
@POLLERR = global i32 0
@POLLHUP = global i32 0
@POLLNVAL = global i32 0
@INADDR_ANY = global i32 0
@INADDR_LOOPBACK = global i32 0
@INADDR_BROADCAST = global i32 0
@SIGHUP = global i32 0
@SIGINT = global i32 0
@SIGQUIT = global i32 0
@SIGILL = global i32 0
@SIGTRAP = global i32 0
@SIGABRT = global i32 0
@SIGFPE = global i32 0
@SIGKILL = global i32 0
@SIGSEGV = global i32 0
@SIGPIPE = global i32 0
@SIGALRM = global i32 0
@SIGTERM = global i32 0
@S_IFMT = global i32 0
@S_IFSOCK = global i32 0
@S_IFLNK = global i32 0
@S_IFREG = global i32 0
@S_IFBLK = global i32 0
@S_IFDIR = global i32 0
@S_IFCHR = global i32 0
@S_IFIFO = global i32 0
@S_ISUID = global i32 0
@S_ISGID = global i32 0
@S_ISVTX = global i32 0
@F_OK = global i32 0
@X_OK = global i32 0
@W_OK = global i32 0
@R_OK = global i32 0
@SEEK_SET = global i32 0
@SEEK_CUR = global i32 0
@SEEK_END = global i32 0
@STDIN_FILENO = global i32 0
@STDOUT_FILENO = global i32 0
@STDERR_FILENO = global i32 0
@str = private unnamed_addr constant [7 x i8] c"1x2345\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"6x789\00", align 1
@str.3 = private unnamed_addr constant [16 x i8] c"s0:    %s (%d)\0A\00", align 1
@str.4 = private unnamed_addr constant [16 x i8] c"s1:    %s (%d)\0A\00", align 1
@str.5 = private unnamed_addr constant [16 x i8] c"s0+s1: %s (%d)\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

declare i64 @strlen(i8*)

declare i8* @strcpy(i8*, i8*)

declare i8* @strcat(i8*, i8*)

declare void @bzero(i8*, i64)

define %String2 @String2.create() {
entry:
  %s = alloca %String2, align 8
  store %String2 zeroinitializer, %String2* %s, align 8
  %s.size = getelementptr inbounds %String2, %String2* %s, i32 0, i32 2
  store i32 10, i32* %s.size, align 4
  %s.value = getelementptr inbounds %String2, %String2* %s, i32 0, i32 0
  %"call.new<i8>" = call i8* @"new<i8>"(i32 10)
  store i8* %"call.new<i8>", i8** %s.value, align 8
  %"$s" = load %String2, %String2* %s, align 8
  ret %String2 %"$s"
}

define void @String2.drop(%String2* %0) {
entry:
  %self = alloca %String2*, align 8
  store %String2* %0, %String2** %self, align 8
  %"*self" = load %String2*, %String2** %self, align 8
  %"*self.value" = getelementptr inbounds %String2, %String2* %"*self", i32 0, i32 0
  %"$*self.value" = load i8*, i8** %"*self.value", align 8
  call void @"free<i8>"(i8* %"$*self.value")
  ret void
}

define void @String2.grow(%String2* %0, i32 %1) {
entry:
  %self = alloca %String2*, align 8
  %want = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String2* %0, %String2** %self, align 8
  store i32 %1, i32* %want, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String2*, %String2** %self, align 8
  %"*self.size" = getelementptr inbounds %String2, %String2* %"*self", i32 0, i32 2
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %"$want" = load i32, i32* %want, align 4
  %GE = icmp sge i32 %"$*self.size", %"$want"
  br i1 %GE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self3" = load %String2*, %String2** %self, align 8
  %"*self.size4" = getelementptr inbounds %String2, %String2* %"*self3", i32 0, i32 2
  %"$*self.size5" = load i32, i32* %"*self.size4", align 4
  %EQ = icmp eq i32 %"$*self.size5", 0
  br i1 %EQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"*self6" = load %String2*, %String2** %self, align 8
  %"*self.size7" = getelementptr inbounds %String2, %String2* %"*self6", i32 0, i32 2
  store i32 10, i32* %"*self.size7", align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %while2

while2:                                           ; preds = %while2.then, %endif1
  %"*self8" = load %String2*, %String2** %self, align 8
  %"*self.size9" = getelementptr inbounds %String2, %String2* %"*self8", i32 0, i32 2
  %"$*self.size10" = load i32, i32* %"*self.size9", align 4
  %"$want11" = load i32, i32* %want, align 4
  %LT = icmp slt i32 %"$*self.size10", %"$want11"
  br i1 %LT, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"*self12" = load %String2*, %String2** %self, align 8
  %"*self.size13" = getelementptr inbounds %String2, %String2* %"*self12", i32 0, i32 2
  %"$*self.size14" = load i32, i32* %"*self.size13", align 4
  %MUL = mul i32 %"$*self.size14", 2
  store i32 %MUL, i32* %"*self.size13", align 4
  br label %while2

endloop:                                          ; preds = %while2
  %"*self15" = load %String2*, %String2** %self, align 8
  %"*self.size16" = getelementptr inbounds %String2, %String2* %"*self15", i32 0, i32 2
  %"$*self.size17" = load i32, i32* %"*self.size16", align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$*self.size17")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  br label %if3

if3:                                              ; preds = %endloop
  %"*self18" = load %String2*, %String2** %self, align 8
  %"*self.count" = getelementptr inbounds %String2, %String2* %"*self18", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %if3.then, label %endif2

if3.then:                                         ; preds = %if3
  %"*self19" = load %String2*, %String2** %self, align 8
  %"*self.value" = getelementptr inbounds %String2, %String2* %"*self19", i32 0, i32 0
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.value" = load i8*, i8** %"*self.value", align 8
  %call.strcpy = call i8* @strcpy(i8* %"$bigger", i8* %"$*self.value")
  br label %endif2

endif2:                                           ; preds = %if3.then, %if3
  %"*self20" = load %String2*, %String2** %self, align 8
  %"*self.value21" = getelementptr inbounds %String2, %String2* %"*self20", i32 0, i32 0
  %"$*self.value22" = load i8*, i8** %"*self.value21", align 8
  call void @"free<i8>"(i8* %"$*self.value22")
  %"*self23" = load %String2*, %String2** %self, align 8
  %"*self.value24" = getelementptr inbounds %String2, %String2* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.value24", align 8
  ret void
}

define void @String2.assign(%String2* %0, i8* %1) {
entry:
  %self = alloca %String2*, align 8
  %str = alloca i8*, align 8
  %len = alloca i32, align 4
  store %String2* %0, %String2** %self, align 8
  store i8* %1, i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %call.strlen = call i64 @strlen(i8* %"$str")
  %AS = trunc i64 %call.strlen to i32
  store i32 %AS, i32* %len, align 4
  %"*self" = load %String2*, %String2** %self, align 8
  %"$len" = load i32, i32* %len, align 4
  %ADD = add i32 %"$len", 1
  call void @String2.grow(%String2* %"*self", i32 %ADD)
  %"*self1" = load %String2*, %String2** %self, align 8
  %"*self.value" = getelementptr inbounds %String2, %String2* %"*self1", i32 0, i32 0
  %"*self2" = load %String2*, %String2** %self, align 8
  %"*self.size" = getelementptr inbounds %String2, %String2* %"*self2", i32 0, i32 2
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %AS3 = sext i32 %"$*self.size" to i64
  %"$*self.value" = load i8*, i8** %"*self.value", align 8
  call void @bzero(i8* %"$*self.value", i64 %AS3)
  %"*self4" = load %String2*, %String2** %self, align 8
  %"*self.value5" = getelementptr inbounds %String2, %String2* %"*self4", i32 0, i32 0
  %"$*self.value6" = load i8*, i8** %"*self.value5", align 8
  %"$str7" = load i8*, i8** %str, align 8
  %call.strcpy = call i8* @strcpy(i8* %"$*self.value6", i8* %"$str7")
  %"*self8" = load %String2*, %String2** %self, align 8
  %"*self.count" = getelementptr inbounds %String2, %String2* %"*self8", i32 0, i32 1
  %"$len9" = load i32, i32* %len, align 4
  store i32 %"$len9", i32* %"*self.count", align 4
  ret void
}

define void @String2.join(%String2* %0, i8* %1) {
entry:
  %self = alloca %String2*, align 8
  %str = alloca i8*, align 8
  %len = alloca i32, align 4
  store %String2* %0, %String2** %self, align 8
  store i8* %1, i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %call.strlen = call i64 @strlen(i8* %"$str")
  %AS = trunc i64 %call.strlen to i32
  store i32 %AS, i32* %len, align 4
  %"*self" = load %String2*, %String2** %self, align 8
  %"*self1" = load %String2*, %String2** %self, align 8
  %"*self.count" = getelementptr inbounds %String2, %String2* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$len" = load i32, i32* %len, align 4
  %ADD = add i32 %"$*self.count", %"$len"
  %ADD2 = add i32 %ADD, 1
  call void @String2.grow(%String2* %"*self", i32 %ADD2)
  %"*self3" = load %String2*, %String2** %self, align 8
  %"*self.value" = getelementptr inbounds %String2, %String2* %"*self3", i32 0, i32 0
  %"$*self.value" = load i8*, i8** %"*self.value", align 8
  %"$str4" = load i8*, i8** %str, align 8
  %call.strcat = call i8* @strcat(i8* %"$*self.value", i8* %"$str4")
  %"*self5" = load %String2*, %String2** %self, align 8
  %"*self.count6" = getelementptr inbounds %String2, %String2* %"*self5", i32 0, i32 1
  %"$len7" = load i32, i32* %len, align 4
  %"$*self.count8" = load i32, i32* %"*self.count6", align 4
  %ADD9 = add i32 %"$*self.count8", %"$len7"
  store i32 %ADD9, i32* %"*self.count6", align 4
  ret void
}

define void @"String2.="(%String2* %0, i8* %1) {
entry:
  %self = alloca %String2*, align 8
  %str = alloca i8*, align 8
  store %String2* %0, %String2** %self, align 8
  store i8* %1, i8** %str, align 8
  %"*self" = load %String2*, %String2** %self, align 8
  %"$str" = load i8*, i8** %str, align 8
  call void @String2.assign(%String2* %"*self", i8* %"$str")
  ret void
}

define void @"String2.=.1"(%String2* %0, %String2* %1) {
entry:
  %self = alloca %String2*, align 8
  %v = alloca %String2*, align 8
  store %String2* %0, %String2** %self, align 8
  store %String2* %1, %String2** %v, align 8
  %"*self" = load %String2*, %String2** %self, align 8
  %"*v" = load %String2*, %String2** %v, align 8
  %"*v.value" = getelementptr inbounds %String2, %String2* %"*v", i32 0, i32 0
  %"$*v.value" = load i8*, i8** %"*v.value", align 8
  call void @String2.assign(%String2* %"*self", i8* %"$*v.value")
  ret void
}

define %String2 @add(%String2* %0, %String2* %1) {
entry:
  %v = alloca %String2*, align 8
  %w = alloca %String2*, align 8
  %res = alloca %String2, align 8
  %copy = alloca %String2, align 8
  store %String2* %0, %String2** %v, align 8
  store %String2* %1, %String2** %w, align 8
  %call.String2.create = call %String2 @String2.create()
  store %String2 %call.String2.create, %String2* %res, align 8
  store %String2 zeroinitializer, %String2* %copy, align 8
  %"*v" = load %String2*, %String2** %v, align 8
  call void @"String2.=.1"(%String2* %copy, %String2* %"*v")
  %"$copy" = load %String2, %String2* %copy, align 8
  call void @String2.drop(%String2* %res)
  store %String2 %"$copy", %String2* %res, align 8
  %"*w" = load %String2*, %String2** %w, align 8
  %"*w.value" = getelementptr inbounds %String2, %String2* %"*w", i32 0, i32 0
  %"$*w.value" = load i8*, i8** %"*w.value", align 8
  call void @String2.join(%String2* %res, i8* %"$*w.value")
  %"$res" = load %String2, %String2* %res, align 8
  ret %String2 %"$res"
}

define i32 @main() {
entry:
  %s0 = alloca %String2, align 8
  %s1 = alloca %String2, align 8
  %s2 = alloca %String2, align 8
  store i32 1, i32* @EPERM, align 4
  store i32 2, i32* @ENOENT, align 4
  store i32 3, i32* @ESRCH, align 4
  store i32 4, i32* @EINTR, align 4
  store i32 5, i32* @EIO, align 4
  store i32 6, i32* @ENXIO, align 4
  store i32 7, i32* @E2BIG, align 4
  store i32 8, i32* @ENOEXEC, align 4
  store i32 9, i32* @EBADF, align 4
  store i32 10, i32* @ECHILD, align 4
  store i32 13, i32* @EACCES, align 4
  store i32 14, i32* @EFAULT, align 4
  store i32 15, i32* @ENOTBLK, align 4
  store i32 16, i32* @EBUSY, align 4
  store i32 17, i32* @EEXIST, align 4
  store i32 18, i32* @EXDEV, align 4
  store i32 19, i32* @ENODEV, align 4
  store i32 20, i32* @ENOTDIR, align 4
  store i32 21, i32* @EISDIR, align 4
  store i32 22, i32* @EINVAL, align 4
  store i32 23, i32* @ENFILE, align 4
  store i32 24, i32* @EMFILE, align 4
  store i32 25, i32* @ENOTTY, align 4
  store i32 26, i32* @ETXTBSY, align 4
  store i32 27, i32* @EFBIG, align 4
  store i32 28, i32* @ENOSPC, align 4
  store i32 29, i32* @ESPIPE, align 4
  store i32 30, i32* @EROFS, align 4
  store i32 31, i32* @EMLINK, align 4
  store i32 32, i32* @EPIPE, align 4
  store i32 33, i32* @EDOM, align 4
  store i32 34, i32* @ERANGE, align 4
  store i32 0, i32* @O_RDONLY, align 4
  store i32 1, i32* @O_WRONLY, align 4
  store i32 2, i32* @O_RDWR, align 4
  store i32 1, i32* @FD_CLOEXEC, align 4
  store i32 0, i32* @IPPROTO_IP, align 4
  store i32 6, i32* @IPPROTO_TCP, align 4
  store i32 17, i32* @IPPROTO_UDP, align 4
  store i32 41, i32* @IPPROTO_IPV6, align 4
  store i32 0, i32* @SHUT_RD, align 4
  store i32 1, i32* @SHUT_WR, align 4
  store i32 2, i32* @SHUT_RDWR, align 4
  store i32 1, i32* @POLLIN, align 4
  store i32 2, i32* @POLLPRI, align 4
  store i32 4, i32* @POLLOUT, align 4
  store i32 8, i32* @POLLERR, align 4
  store i32 16, i32* @POLLHUP, align 4
  store i32 32, i32* @POLLNVAL, align 4
  store i32 0, i32* @INADDR_ANY, align 4
  store i32 2130706433, i32* @INADDR_LOOPBACK, align 4
  store i32 -1, i32* @INADDR_BROADCAST, align 4
  store i32 1, i32* @SIGHUP, align 4
  store i32 2, i32* @SIGINT, align 4
  store i32 3, i32* @SIGQUIT, align 4
  store i32 4, i32* @SIGILL, align 4
  store i32 5, i32* @SIGTRAP, align 4
  store i32 6, i32* @SIGABRT, align 4
  store i32 8, i32* @SIGFPE, align 4
  store i32 9, i32* @SIGKILL, align 4
  store i32 11, i32* @SIGSEGV, align 4
  store i32 13, i32* @SIGPIPE, align 4
  store i32 14, i32* @SIGALRM, align 4
  store i32 15, i32* @SIGTERM, align 4
  store i32 61440, i32* @S_IFMT, align 4
  store i32 49152, i32* @S_IFSOCK, align 4
  store i32 40960, i32* @S_IFLNK, align 4
  store i32 32768, i32* @S_IFREG, align 4
  store i32 24576, i32* @S_IFBLK, align 4
  store i32 16384, i32* @S_IFDIR, align 4
  store i32 8192, i32* @S_IFCHR, align 4
  store i32 4096, i32* @S_IFIFO, align 4
  store i32 2048, i32* @S_ISUID, align 4
  store i32 1024, i32* @S_ISGID, align 4
  store i32 512, i32* @S_ISVTX, align 4
  store i32 0, i32* @F_OK, align 4
  store i32 1, i32* @X_OK, align 4
  store i32 2, i32* @W_OK, align 4
  store i32 4, i32* @R_OK, align 4
  store i32 0, i32* @SEEK_SET, align 4
  store i32 1, i32* @SEEK_CUR, align 4
  store i32 2, i32* @SEEK_END, align 4
  store i32 0, i32* @STDIN_FILENO, align 4
  store i32 1, i32* @STDOUT_FILENO, align 4
  store i32 2, i32* @STDERR_FILENO, align 4
  %call.String2.create = call %String2 @String2.create()
  store %String2 %call.String2.create, %String2* %s0, align 8
  call void @"String2.="(%String2* %s0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0))
  %call.String2.create1 = call %String2 @String2.create()
  store %String2 %call.String2.create1, %String2* %s1, align 8
  call void @"String2.="(%String2* %s1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  %call.String2.create2 = call %String2 @String2.create()
  store %String2 %call.String2.create2, %String2* %s2, align 8
  %call.add = call %String2 @add(%String2* %s0, %String2* %s1)
  call void @String2.drop(%String2* %s2)
  store %String2 %call.add, %String2* %s2, align 8
  %s0.value = getelementptr inbounds %String2, %String2* %s0, i32 0, i32 0
  %s0.count = getelementptr inbounds %String2, %String2* %s0, i32 0, i32 1
  %"$s0.value" = load i8*, i8** %s0.value, align 8
  %"$s0.count" = load i32, i32* %s0.count, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.3, i32 0, i32 0), i8* %"$s0.value", i32 %"$s0.count")
  %s1.value = getelementptr inbounds %String2, %String2* %s1, i32 0, i32 0
  %s1.count = getelementptr inbounds %String2, %String2* %s1, i32 0, i32 1
  %"$s1.value" = load i8*, i8** %s1.value, align 8
  %"$s1.count" = load i32, i32* %s1.count, align 4
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.4, i32 0, i32 0), i8* %"$s1.value", i32 %"$s1.count")
  %s2.value = getelementptr inbounds %String2, %String2* %s2, i32 0, i32 0
  %s2.count = getelementptr inbounds %String2, %String2* %s2, i32 0, i32 1
  %"$s2.value" = load i8*, i8** %s2.value, align 8
  %"$s2.count" = load i32, i32* %s2.count, align 4
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.5, i32 0, i32 0), i8* %"$s2.value", i32 %"$s2.count")
  call void @String2.drop(%String2* %s2)
  call void @String2.drop(%String2* %s1)
  call void @String2.drop(%String2* %s0)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 036 — operator: a struct owning heap buffers: deep-copy operator = and drop

```ura
// stdout: The Bold Aldric enters!
// stdout: The Grim Borin enters!
// stdout: The Grim Borin enters!
// stdout: The Quick Cass enters!
// expect: 0
use "@/header"

struct Hero:
    name  char[]
    title char[]

    pub fn create() Hero:
        h Hero
        h.name  = new<char>(32)
        h.title = new<char>(32)
        return h

    fn setup(name char[], title char[]):
        strcpy(self.name,  name)
        strcpy(self.title, title)

    fn announce():
        output(self.title, " ", self.name, " enters!\n")

    operator = (other &Hero) void:
        free<char>(self.name)
        free<char>(self.title)
        self.name  = new<char>(32)
        self.title = new<char>(32)
        strcpy(self.name,  other.name)
        strcpy(self.title, other.title)

    operator drop():
        free<char>(self.name)
        free<char>(self.title)

fn main() i32:
    hero Hero = Hero::create()
    hero.setup("Aldric", "The Bold")
    hero.announce()

    other Hero = Hero::create()
    other.setup("Borin", "The Grim")
    hero = other
    hero.announce()

    // the copy owns its own buffers, so mutating the source leaves it alone
    other.setup("Cass", "The Quick")
    hero.announce()
    other.announce()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Hero = type { i8*, i8* }

@EPERM = global i32 0
@ENOENT = global i32 0
@ESRCH = global i32 0
@EINTR = global i32 0
@EIO = global i32 0
@ENXIO = global i32 0
@E2BIG = global i32 0
@ENOEXEC = global i32 0
@EBADF = global i32 0
@ECHILD = global i32 0
@EACCES = global i32 0
@EFAULT = global i32 0
@ENOTBLK = global i32 0
@EBUSY = global i32 0
@EEXIST = global i32 0
@EXDEV = global i32 0
@ENODEV = global i32 0
@ENOTDIR = global i32 0
@EISDIR = global i32 0
@EINVAL = global i32 0
@ENFILE = global i32 0
@EMFILE = global i32 0
@ENOTTY = global i32 0
@ETXTBSY = global i32 0
@EFBIG = global i32 0
@ENOSPC = global i32 0
@ESPIPE = global i32 0
@EROFS = global i32 0
@EMLINK = global i32 0
@EPIPE = global i32 0
@EDOM = global i32 0
@ERANGE = global i32 0
@O_RDONLY = global i32 0
@O_WRONLY = global i32 0
@O_RDWR = global i32 0
@FD_CLOEXEC = global i32 0
@IPPROTO_IP = global i32 0
@IPPROTO_TCP = global i32 0
@IPPROTO_UDP = global i32 0
@IPPROTO_IPV6 = global i32 0
@SHUT_RD = global i32 0
@SHUT_WR = global i32 0
@SHUT_RDWR = global i32 0
@POLLIN = global i32 0
@POLLPRI = global i32 0
@POLLOUT = global i32 0
@POLLERR = global i32 0
@POLLHUP = global i32 0
@POLLNVAL = global i32 0
@INADDR_ANY = global i32 0
@INADDR_LOOPBACK = global i32 0
@INADDR_BROADCAST = global i32 0
@SIGHUP = global i32 0
@SIGINT = global i32 0
@SIGQUIT = global i32 0
@SIGILL = global i32 0
@SIGTRAP = global i32 0
@SIGABRT = global i32 0
@SIGFPE = global i32 0
@SIGKILL = global i32 0
@SIGSEGV = global i32 0
@SIGPIPE = global i32 0
@SIGALRM = global i32 0
@SIGTERM = global i32 0
@S_IFMT = global i32 0
@S_IFSOCK = global i32 0
@S_IFLNK = global i32 0
@S_IFREG = global i32 0
@S_IFBLK = global i32 0
@S_IFDIR = global i32 0
@S_IFCHR = global i32 0
@S_IFIFO = global i32 0
@S_ISUID = global i32 0
@S_ISGID = global i32 0
@S_ISVTX = global i32 0
@F_OK = global i32 0
@X_OK = global i32 0
@W_OK = global i32 0
@R_OK = global i32 0
@SEEK_SET = global i32 0
@SEEK_CUR = global i32 0
@SEEK_END = global i32 0
@STDIN_FILENO = global i32 0
@STDOUT_FILENO = global i32 0
@STDERR_FILENO = global i32 0
@str = private unnamed_addr constant [15 x i8] c"%s %s enters!\0A\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"The Bold\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"Borin\00", align 1
@str.4 = private unnamed_addr constant [9 x i8] c"The Grim\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"Cass\00", align 1
@str.6 = private unnamed_addr constant [10 x i8] c"The Quick\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

declare i8* @strcpy(i8*, i8*)

define %Hero @Hero.create() {
entry:
  %h = alloca %Hero, align 8
  store %Hero zeroinitializer, %Hero* %h, align 8
  %h.name = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 0
  %"call.new<i8>" = call i8* @"new<i8>"(i32 32)
  store i8* %"call.new<i8>", i8** %h.name, align 8
  %h.title = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 1
  %"call.new<i8>1" = call i8* @"new<i8>"(i32 32)
  store i8* %"call.new<i8>1", i8** %h.title, align 8
  %"$h" = load %Hero, %Hero* %h, align 8
  ret %Hero %"$h"
}

define void @Hero.setup(%Hero* %0, i8* %1, i8* %2) {
entry:
  %self = alloca %Hero*, align 8
  %name = alloca i8*, align 8
  %title = alloca i8*, align 8
  store %Hero* %0, %Hero** %self, align 8
  store i8* %1, i8** %name, align 8
  store i8* %2, i8** %title, align 8
  %"*self" = load %Hero*, %Hero** %self, align 8
  %"*self.name" = getelementptr inbounds %Hero, %Hero* %"*self", i32 0, i32 0
  %"$*self.name" = load i8*, i8** %"*self.name", align 8
  %"$name" = load i8*, i8** %name, align 8
  %call.strcpy = call i8* @strcpy(i8* %"$*self.name", i8* %"$name")
  %"*self1" = load %Hero*, %Hero** %self, align 8
  %"*self.title" = getelementptr inbounds %Hero, %Hero* %"*self1", i32 0, i32 1
  %"$*self.title" = load i8*, i8** %"*self.title", align 8
  %"$title" = load i8*, i8** %title, align 8
  %call.strcpy2 = call i8* @strcpy(i8* %"$*self.title", i8* %"$title")
  ret void
}

define void @Hero.announce(%Hero* %0) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %"*self" = load %Hero*, %Hero** %self, align 8
  %"*self.title" = getelementptr inbounds %Hero, %Hero* %"*self", i32 0, i32 1
  %"*self1" = load %Hero*, %Hero** %self, align 8
  %"*self.name" = getelementptr inbounds %Hero, %Hero* %"*self1", i32 0, i32 0
  %"$*self.title" = load i8*, i8** %"*self.title", align 8
  %"$*self.name" = load i8*, i8** %"*self.name", align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i8* %"$*self.title", i8* %"$*self.name")
  ret void
}

define void @"Hero.="(%Hero* %0, %Hero* %1) {
entry:
  %self = alloca %Hero*, align 8
  %other = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  store %Hero* %1, %Hero** %other, align 8
  %"*self" = load %Hero*, %Hero** %self, align 8
  %"*self.name" = getelementptr inbounds %Hero, %Hero* %"*self", i32 0, i32 0
  %"$*self.name" = load i8*, i8** %"*self.name", align 8
  call void @"free<i8>"(i8* %"$*self.name")
  %"*self1" = load %Hero*, %Hero** %self, align 8
  %"*self.title" = getelementptr inbounds %Hero, %Hero* %"*self1", i32 0, i32 1
  %"$*self.title" = load i8*, i8** %"*self.title", align 8
  call void @"free<i8>"(i8* %"$*self.title")
  %"*self2" = load %Hero*, %Hero** %self, align 8
  %"*self.name3" = getelementptr inbounds %Hero, %Hero* %"*self2", i32 0, i32 0
  %"call.new<i8>" = call i8* @"new<i8>"(i32 32)
  store i8* %"call.new<i8>", i8** %"*self.name3", align 8
  %"*self4" = load %Hero*, %Hero** %self, align 8
  %"*self.title5" = getelementptr inbounds %Hero, %Hero* %"*self4", i32 0, i32 1
  %"call.new<i8>6" = call i8* @"new<i8>"(i32 32)
  store i8* %"call.new<i8>6", i8** %"*self.title5", align 8
  %"*self7" = load %Hero*, %Hero** %self, align 8
  %"*self.name8" = getelementptr inbounds %Hero, %Hero* %"*self7", i32 0, i32 0
  %"*other" = load %Hero*, %Hero** %other, align 8
  %"*other.name" = getelementptr inbounds %Hero, %Hero* %"*other", i32 0, i32 0
  %"$*self.name9" = load i8*, i8** %"*self.name8", align 8
  %"$*other.name" = load i8*, i8** %"*other.name", align 8
  %call.strcpy = call i8* @strcpy(i8* %"$*self.name9", i8* %"$*other.name")
  %"*self10" = load %Hero*, %Hero** %self, align 8
  %"*self.title11" = getelementptr inbounds %Hero, %Hero* %"*self10", i32 0, i32 1
  %"*other12" = load %Hero*, %Hero** %other, align 8
  %"*other.title" = getelementptr inbounds %Hero, %Hero* %"*other12", i32 0, i32 1
  %"$*self.title13" = load i8*, i8** %"*self.title11", align 8
  %"$*other.title" = load i8*, i8** %"*other.title", align 8
  %call.strcpy14 = call i8* @strcpy(i8* %"$*self.title13", i8* %"$*other.title")
  ret void
}

define void @Hero.drop(%Hero* %0) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %"*self" = load %Hero*, %Hero** %self, align 8
  %"*self.name" = getelementptr inbounds %Hero, %Hero* %"*self", i32 0, i32 0
  %"$*self.name" = load i8*, i8** %"*self.name", align 8
  call void @"free<i8>"(i8* %"$*self.name")
  %"*self1" = load %Hero*, %Hero** %self, align 8
  %"*self.title" = getelementptr inbounds %Hero, %Hero* %"*self1", i32 0, i32 1
  %"$*self.title" = load i8*, i8** %"*self.title", align 8
  call void @"free<i8>"(i8* %"$*self.title")
  ret void
}

define i32 @main() {
entry:
  %hero = alloca %Hero, align 8
  %other = alloca %Hero, align 8
  %copy = alloca %Hero, align 8
  store i32 1, i32* @EPERM, align 4
  store i32 2, i32* @ENOENT, align 4
  store i32 3, i32* @ESRCH, align 4
  store i32 4, i32* @EINTR, align 4
  store i32 5, i32* @EIO, align 4
  store i32 6, i32* @ENXIO, align 4
  store i32 7, i32* @E2BIG, align 4
  store i32 8, i32* @ENOEXEC, align 4
  store i32 9, i32* @EBADF, align 4
  store i32 10, i32* @ECHILD, align 4
  store i32 13, i32* @EACCES, align 4
  store i32 14, i32* @EFAULT, align 4
  store i32 15, i32* @ENOTBLK, align 4
  store i32 16, i32* @EBUSY, align 4
  store i32 17, i32* @EEXIST, align 4
  store i32 18, i32* @EXDEV, align 4
  store i32 19, i32* @ENODEV, align 4
  store i32 20, i32* @ENOTDIR, align 4
  store i32 21, i32* @EISDIR, align 4
  store i32 22, i32* @EINVAL, align 4
  store i32 23, i32* @ENFILE, align 4
  store i32 24, i32* @EMFILE, align 4
  store i32 25, i32* @ENOTTY, align 4
  store i32 26, i32* @ETXTBSY, align 4
  store i32 27, i32* @EFBIG, align 4
  store i32 28, i32* @ENOSPC, align 4
  store i32 29, i32* @ESPIPE, align 4
  store i32 30, i32* @EROFS, align 4
  store i32 31, i32* @EMLINK, align 4
  store i32 32, i32* @EPIPE, align 4
  store i32 33, i32* @EDOM, align 4
  store i32 34, i32* @ERANGE, align 4
  store i32 0, i32* @O_RDONLY, align 4
  store i32 1, i32* @O_WRONLY, align 4
  store i32 2, i32* @O_RDWR, align 4
  store i32 1, i32* @FD_CLOEXEC, align 4
  store i32 0, i32* @IPPROTO_IP, align 4
  store i32 6, i32* @IPPROTO_TCP, align 4
  store i32 17, i32* @IPPROTO_UDP, align 4
  store i32 41, i32* @IPPROTO_IPV6, align 4
  store i32 0, i32* @SHUT_RD, align 4
  store i32 1, i32* @SHUT_WR, align 4
  store i32 2, i32* @SHUT_RDWR, align 4
  store i32 1, i32* @POLLIN, align 4
  store i32 2, i32* @POLLPRI, align 4
  store i32 4, i32* @POLLOUT, align 4
  store i32 8, i32* @POLLERR, align 4
  store i32 16, i32* @POLLHUP, align 4
  store i32 32, i32* @POLLNVAL, align 4
  store i32 0, i32* @INADDR_ANY, align 4
  store i32 2130706433, i32* @INADDR_LOOPBACK, align 4
  store i32 -1, i32* @INADDR_BROADCAST, align 4
  store i32 1, i32* @SIGHUP, align 4
  store i32 2, i32* @SIGINT, align 4
  store i32 3, i32* @SIGQUIT, align 4
  store i32 4, i32* @SIGILL, align 4
  store i32 5, i32* @SIGTRAP, align 4
  store i32 6, i32* @SIGABRT, align 4
  store i32 8, i32* @SIGFPE, align 4
  store i32 9, i32* @SIGKILL, align 4
  store i32 11, i32* @SIGSEGV, align 4
  store i32 13, i32* @SIGPIPE, align 4
  store i32 14, i32* @SIGALRM, align 4
  store i32 15, i32* @SIGTERM, align 4
  store i32 61440, i32* @S_IFMT, align 4
  store i32 49152, i32* @S_IFSOCK, align 4
  store i32 40960, i32* @S_IFLNK, align 4
  store i32 32768, i32* @S_IFREG, align 4
  store i32 24576, i32* @S_IFBLK, align 4
  store i32 16384, i32* @S_IFDIR, align 4
  store i32 8192, i32* @S_IFCHR, align 4
  store i32 4096, i32* @S_IFIFO, align 4
  store i32 2048, i32* @S_ISUID, align 4
  store i32 1024, i32* @S_ISGID, align 4
  store i32 512, i32* @S_ISVTX, align 4
  store i32 0, i32* @F_OK, align 4
  store i32 1, i32* @X_OK, align 4
  store i32 2, i32* @W_OK, align 4
  store i32 4, i32* @R_OK, align 4
  store i32 0, i32* @SEEK_SET, align 4
  store i32 1, i32* @SEEK_CUR, align 4
  store i32 2, i32* @SEEK_END, align 4
  store i32 0, i32* @STDIN_FILENO, align 4
  store i32 1, i32* @STDOUT_FILENO, align 4
  store i32 2, i32* @STDERR_FILENO, align 4
  %call.Hero.create = call %Hero @Hero.create()
  store %Hero %call.Hero.create, %Hero* %hero, align 8
  call void @Hero.setup(%Hero* %hero, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0))
  call void @Hero.announce(%Hero* %hero)
  %call.Hero.create1 = call %Hero @Hero.create()
  store %Hero %call.Hero.create1, %Hero* %other, align 8
  call void @Hero.setup(%Hero* %other, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.4, i32 0, i32 0))
  store %Hero zeroinitializer, %Hero* %copy, align 8
  call void @"Hero.="(%Hero* %copy, %Hero* %other)
  %"$copy" = load %Hero, %Hero* %copy, align 8
  call void @Hero.drop(%Hero* %hero)
  store %Hero %"$copy", %Hero* %hero, align 8
  call void @Hero.announce(%Hero* %hero)
  call void @Hero.setup(%Hero* %other, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.6, i32 0, i32 0))
  call void @Hero.announce(%Hero* %hero)
  call void @Hero.announce(%Hero* %other)
  call void @Hero.drop(%Hero* %other)
  call void @Hero.drop(%Hero* %hero)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 037 — ref: ref return: create a node on the heap and return a reference to it

```ura
// stdout: 42
// expect: 0
struct Node:
    value i32
    next &Node

fn make(v i32, heap Node[]) &Node:
    heap[0].value = v
    return &heap[0]

fn main() i32:
    h Node[] = new<Node>(1)
    a &Node = make(42, h)
    output(a.value, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define %Node* @make(i32 %0, %Node* %1) {
entry:
  %v = alloca i32, align 4
  %heap = alloca %Node*, align 8
  store i32 %0, i32* %v, align 4
  store %Node* %1, %Node** %heap, align 8
  %"$heap" = load %Node*, %Node** %heap, align 8
  %"heap[]" = getelementptr inbounds %Node, %Node* %"$heap", i32 0
  %"heap[].value" = getelementptr inbounds %Node, %Node* %"heap[]", i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"heap[].value", align 4
  %"$heap1" = load %Node*, %Node** %heap, align 8
  %"heap[]2" = getelementptr inbounds %Node, %Node* %"$heap1", i32 0
  ret %Node* %"heap[]2"
}

define %Node* @"new<Node>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Node*
  ret %Node* %AS1
}

define i32 @main() {
entry:
  %h = alloca %Node*, align 8
  %a = alloca %Node*, align 8
  %ref = alloca %Node*, align 8
  %"call.new<Node>" = call %Node* @"new<Node>"(i32 1)
  store %Node* %"call.new<Node>", %Node** %h, align 8
  %"$h" = load %Node*, %Node** %h, align 8
  %call.make = call %Node* @make(i32 42, %Node* %"$h")
  store %Node* %call.make, %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  store %Node* %"*ref", %Node** %a, align 8
  %"*a" = load %Node*, %Node** %a, align 8
  %"*a.value" = getelementptr inbounds %Node, %Node* %"*a", i32 0, i32 0
  %"$*a.value" = load i32, i32* %"*a.value", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*a.value")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 038 — ref: ref? return: one branch returns a reference, the other returns null

```ura
// stdout: 7 True
// expect: 0
struct N:
    v i32
    nx &N

fn pick(h N[], want bool) &N:
    if want:
        return &h[0]
    return null

fn main() i32:
    h N[] = new<N>(1)
    h[0].v = 7
    r &N = pick(h, True)
    e &N = pick(h, False)
    output(r.v, " ", e == null, "\n")
    return 0
```

---

## 039 — ref: declare a ref and bind it to a ref-returning call

```ura
// stdout: 9
// expect: 0
struct Node:
    v i32
    next &Node

fn make(v i32, h Node[]) &Node:
    h[0].v = v
    return &h[0]

fn main() i32:
    h Node[] = new<Node>(1)
    n &Node = make(9, h)
    output(n.v, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define %Node* @make(i32 %0, %Node* %1) {
entry:
  %v = alloca i32, align 4
  %h = alloca %Node*, align 8
  store i32 %0, i32* %v, align 4
  store %Node* %1, %Node** %h, align 8
  %"$h" = load %Node*, %Node** %h, align 8
  %"h[]" = getelementptr inbounds %Node, %Node* %"$h", i32 0
  %"h[].v" = getelementptr inbounds %Node, %Node* %"h[]", i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"h[].v", align 4
  %"$h1" = load %Node*, %Node** %h, align 8
  %"h[]2" = getelementptr inbounds %Node, %Node* %"$h1", i32 0
  ret %Node* %"h[]2"
}

define %Node* @"new<Node>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Node*
  ret %Node* %AS1
}

define i32 @main() {
entry:
  %h = alloca %Node*, align 8
  %n = alloca %Node*, align 8
  %ref = alloca %Node*, align 8
  %"call.new<Node>" = call %Node* @"new<Node>"(i32 1)
  store %Node* %"call.new<Node>", %Node** %h, align 8
  %"$h" = load %Node*, %Node** %h, align 8
  %call.make = call %Node* @make(i32 9, %Node* %"$h")
  store %Node* %call.make, %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  store %Node* %"*ref", %Node** %n, align 8
  %"*n" = load %Node*, %Node** %n, align 8
  %"*n.v" = getelementptr inbounds %Node, %Node* %"*n", i32 0, i32 0
  %"$*n.v" = load i32, i32* %"*n.v", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*n.v")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 040 — ref: a ref-returning call passed straight into a ref parameter

```ura
// stdout: 5
// expect: 0
struct Node:
    v i32
    next &Node

struct Lst:
    head &Node

fn make(v i32, h Node[]) &Node:
    h[0].v = v
    return &h[0]

fn sethead(lst &Lst, node &Node):
    lst.head = &node

fn main() i32:
    h Node[] = new<Node>(1)
    lst Lst
    sethead(&lst, make(5, h))
    output(lst.head.v, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }
%Lst = type { %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define %Node* @make(i32 %0, %Node* %1) {
entry:
  %v = alloca i32, align 4
  %h = alloca %Node*, align 8
  store i32 %0, i32* %v, align 4
  store %Node* %1, %Node** %h, align 8
  %"$h" = load %Node*, %Node** %h, align 8
  %"h[]" = getelementptr inbounds %Node, %Node* %"$h", i32 0
  %"h[].v" = getelementptr inbounds %Node, %Node* %"h[]", i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"h[].v", align 4
  %"$h1" = load %Node*, %Node** %h, align 8
  %"h[]2" = getelementptr inbounds %Node, %Node* %"$h1", i32 0
  ret %Node* %"h[]2"
}

define void @sethead(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  %node = alloca %Node*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  store %Node* %1, %Node** %node, align 8
  %"*lst" = load %Lst*, %Lst** %lst, align 8
  %"*lst.head" = getelementptr inbounds %Lst, %Lst* %"*lst", i32 0, i32 0
  %"*node" = load %Node*, %Node** %node, align 8
  store %Node* %"*node", %Node** %"*lst.head", align 8
  ret void
}

define %Node* @"new<Node>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Node*
  ret %Node* %AS1
}

define i32 @main() {
entry:
  %h = alloca %Node*, align 8
  %lst = alloca %Lst, align 8
  %ref = alloca %Node*, align 8
  %"call.new<Node>" = call %Node* @"new<Node>"(i32 1)
  store %Node* %"call.new<Node>", %Node** %h, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %"$h" = load %Node*, %Node** %h, align 8
  %call.make = call %Node* @make(i32 5, %Node* %"$h")
  store %Node* %call.make, %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  call void @sethead(%Lst* %lst, %Node* %"*ref")
  %lst.head = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %"*lst.head" = load %Node*, %Node** %lst.head, align 8
  %"*lst.head.v" = getelementptr inbounds %Node, %Node* %"*lst.head", i32 0, i32 0
  %"$*lst.head.v" = load i32, i32* %"*lst.head.v", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*lst.head.v")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 041 — ref: a reference function must return a reference, not a bare value

```ura
// error: 'g' returns a reference; return `ref x` (or null), not a value
struct N:
    v i32

fn g(h N[]) &N:
    return h[0]

fn main() i32:
    h N[] = new<N>(1)
    output(g(h).v, "\n")
    return 0
```

### stderr

```
error: test.ura:6:13 expected '&N', found 'N'
  |
6 |     return h[0]
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 042 — ref: a field read chained directly on a ref-returning call

```ura
// stdout: 42
// expect: 0
struct Node:
    value i32
    next &Node

fn make(v i32, heap Node[]) &Node:
    heap[0].value = v
    return &heap[0]

fn main() i32:
    h Node[] = new<Node>(1)
    output(make(42, h).value, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define %Node* @make(i32 %0, %Node* %1) {
entry:
  %v = alloca i32, align 4
  %heap = alloca %Node*, align 8
  store i32 %0, i32* %v, align 4
  store %Node* %1, %Node** %heap, align 8
  %"$heap" = load %Node*, %Node** %heap, align 8
  %"heap[]" = getelementptr inbounds %Node, %Node* %"$heap", i32 0
  %"heap[].value" = getelementptr inbounds %Node, %Node* %"heap[]", i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"heap[].value", align 4
  %"$heap1" = load %Node*, %Node** %heap, align 8
  %"heap[]2" = getelementptr inbounds %Node, %Node* %"$heap1", i32 0
  ret %Node* %"heap[]2"
}

define %Node* @"new<Node>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Node*
  ret %Node* %AS1
}

define i32 @main() {
entry:
  %h = alloca %Node*, align 8
  %ref = alloca %Node*, align 8
  %"call.new<Node>" = call %Node* @"new<Node>"(i32 1)
  store %Node* %"call.new<Node>", %Node** %h, align 8
  %"$h" = load %Node*, %Node** %h, align 8
  %call.make = call %Node* @make(i32 42, %Node* %"$h")
  store %Node* %call.make, %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"*ref.value" = getelementptr inbounds %Node, %Node* %"*ref", i32 0, i32 0
  %"$*ref.value" = load i32, i32* %"*ref.value", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*ref.value")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 043 — struct: a heap array of structs: new<Room>(N), then free

```ura
// stdout: attic@9 2
// expect: 0

struct Room:
    name  char[]
    floor i32

fn main() i32:
    h Room[] = new<Room>(2)
    h[0].name  = "attic"
    h[0].floor = 9
    h[1].floor = 2
    output(h[0].name, "@", h[0].floor, " ", h[1].floor, "\n")
    free<Room>(h)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [6 x i8] c"attic\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"%s@%d %d\0A\00", align 1

define %Room* @"new<Room>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Room* getelementptr (%Room, %Room* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Room*
  ret %Room* %AS1
}

define void @"free<Room>"(%Room* %0) {
entry:
  %data = alloca %Room*, align 8
  store %Room* %0, %Room** %data, align 8
  %"$data" = load %Room*, %Room** %data, align 8
  %AS = bitcast %Room* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %h = alloca %Room*, align 8
  %"call.new<Room>" = call %Room* @"new<Room>"(i32 2)
  store %Room* %"call.new<Room>", %Room** %h, align 8
  %"$h" = load %Room*, %Room** %h, align 8
  %"h[]" = getelementptr inbounds %Room, %Room* %"$h", i32 0
  %"h[].name" = getelementptr inbounds %Room, %Room* %"h[]", i32 0, i32 0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8** %"h[].name", align 8
  %"$h1" = load %Room*, %Room** %h, align 8
  %"h[]2" = getelementptr inbounds %Room, %Room* %"$h1", i32 0
  %"h[].floor" = getelementptr inbounds %Room, %Room* %"h[]2", i32 0, i32 1
  store i32 9, i32* %"h[].floor", align 4
  %"$h3" = load %Room*, %Room** %h, align 8
  %"h[]4" = getelementptr inbounds %Room, %Room* %"$h3", i32 1
  %"h[].floor5" = getelementptr inbounds %Room, %Room* %"h[]4", i32 0, i32 1
  store i32 2, i32* %"h[].floor5", align 4
  %"$h6" = load %Room*, %Room** %h, align 8
  %"h[]7" = getelementptr inbounds %Room, %Room* %"$h6", i32 0
  %"h[].name8" = getelementptr inbounds %Room, %Room* %"h[]7", i32 0, i32 0
  %"$h9" = load %Room*, %Room** %h, align 8
  %"h[]10" = getelementptr inbounds %Room, %Room* %"$h9", i32 0
  %"h[].floor11" = getelementptr inbounds %Room, %Room* %"h[]10", i32 0, i32 1
  %"$h12" = load %Room*, %Room** %h, align 8
  %"h[]13" = getelementptr inbounds %Room, %Room* %"$h12", i32 1
  %"h[].floor14" = getelementptr inbounds %Room, %Room* %"h[]13", i32 0, i32 1
  %"$h[].name" = load i8*, i8** %"h[].name8", align 8
  %"$h[].floor" = load i32, i32* %"h[].floor11", align 4
  %"$h[].floor15" = load i32, i32* %"h[].floor14", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i8* %"$h[].name", i32 %"$h[].floor", i32 %"$h[].floor15")
  %"$h16" = load %Room*, %Room** %h, align 8
  call void @"free<Room>"(%Room* %"$h16")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 044 — struct: a 2D heap array of structs frees every inner slice

```ura
// stdout: 1 4
// expect: 0

struct Room:
    floor i32

fn main() i32:
    g Room[][] = new<Room[]>(2)
    g[0] = new<Room>(2)
    g[1] = new<Room>(2)
    g[0][0].floor = 1
    g[1][1].floor = 4
    output(g[0][0].floor, " ", g[1][1].floor, "\n")
    free<Room>(g[0])
    free<Room>(g[1])
    free<Room[]>(g)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define %Room** @"new<Room[]>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Room**
  ret %Room** %AS1
}

define void @"free<Room[]>"(%Room** %0) {
entry:
  %data = alloca %Room**, align 8
  store %Room** %0, %Room*** %data, align 8
  %"$data" = load %Room**, %Room*** %data, align 8
  %AS = bitcast %Room** %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define %Room* @"new<Room>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Room*
  ret %Room* %AS1
}

define void @"free<Room>"(%Room* %0) {
entry:
  %data = alloca %Room*, align 8
  store %Room* %0, %Room** %data, align 8
  %"$data" = load %Room*, %Room** %data, align 8
  %AS = bitcast %Room* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %g = alloca %Room**, align 8
  %"call.new<Room[]>" = call %Room** @"new<Room[]>"(i32 2)
  store %Room** %"call.new<Room[]>", %Room*** %g, align 8
  %"$g" = load %Room**, %Room*** %g, align 8
  %"g[]" = getelementptr inbounds %Room*, %Room** %"$g", i32 0
  %"call.new<Room>" = call %Room* @"new<Room>"(i32 2)
  store %Room* %"call.new<Room>", %Room** %"g[]", align 8
  %"$g1" = load %Room**, %Room*** %g, align 8
  %"g[]2" = getelementptr inbounds %Room*, %Room** %"$g1", i32 1
  %"call.new<Room>3" = call %Room* @"new<Room>"(i32 2)
  store %Room* %"call.new<Room>3", %Room** %"g[]2", align 8
  %"$g4" = load %Room**, %Room*** %g, align 8
  %"g[]5" = getelementptr inbounds %Room*, %Room** %"$g4", i32 0
  %"$g[]" = load %Room*, %Room** %"g[]5", align 8
  %"g[][]" = getelementptr inbounds %Room, %Room* %"$g[]", i32 0
  %"g[][].floor" = getelementptr inbounds %Room, %Room* %"g[][]", i32 0, i32 0
  store i32 1, i32* %"g[][].floor", align 4
  %"$g6" = load %Room**, %Room*** %g, align 8
  %"g[]7" = getelementptr inbounds %Room*, %Room** %"$g6", i32 1
  %"$g[]8" = load %Room*, %Room** %"g[]7", align 8
  %"g[][]9" = getelementptr inbounds %Room, %Room* %"$g[]8", i32 1
  %"g[][].floor10" = getelementptr inbounds %Room, %Room* %"g[][]9", i32 0, i32 0
  store i32 4, i32* %"g[][].floor10", align 4
  %"$g11" = load %Room**, %Room*** %g, align 8
  %"g[]12" = getelementptr inbounds %Room*, %Room** %"$g11", i32 0
  %"$g[]13" = load %Room*, %Room** %"g[]12", align 8
  %"g[][]14" = getelementptr inbounds %Room, %Room* %"$g[]13", i32 0
  %"g[][].floor15" = getelementptr inbounds %Room, %Room* %"g[][]14", i32 0, i32 0
  %"$g16" = load %Room**, %Room*** %g, align 8
  %"g[]17" = getelementptr inbounds %Room*, %Room** %"$g16", i32 1
  %"$g[]18" = load %Room*, %Room** %"g[]17", align 8
  %"g[][]19" = getelementptr inbounds %Room, %Room* %"$g[]18", i32 1
  %"g[][].floor20" = getelementptr inbounds %Room, %Room* %"g[][]19", i32 0, i32 0
  %"$g[][].floor" = load i32, i32* %"g[][].floor15", align 4
  %"$g[][].floor21" = load i32, i32* %"g[][].floor20", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$g[][].floor", i32 %"$g[][].floor21")
  %"$g22" = load %Room**, %Room*** %g, align 8
  %"g[]23" = getelementptr inbounds %Room*, %Room** %"$g22", i32 0
  %"$g[]24" = load %Room*, %Room** %"g[]23", align 8
  call void @"free<Room>"(%Room* %"$g[]24")
  %"$g25" = load %Room**, %Room*** %g, align 8
  %"g[]26" = getelementptr inbounds %Room*, %Room** %"$g25", i32 1
  %"$g[]27" = load %Room*, %Room** %"g[]26", align 8
  call void @"free<Room>"(%Room* %"$g[]27")
  %"$g28" = load %Room**, %Room*** %g, align 8
  call void @"free<Room[]>"(%Room** %"$g28")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 045 — struct: an unknown type as a parameter, a return type, or an array element

```ura
// error: Unknown type 'Widget'

fn process(p Widget):
    return

fn make() Widget:
    w Widget
    return w

fn main() i32:
    arr Widget[] = new<Widget>(3)
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:3:14 unknown type 'Widget'
  |
3 | fn process(p Widget):
  |              ^^^^^^
help: declare it: struct Widget:
error: test.ura:6:11 unknown type 'Widget'
  |
6 | fn make() Widget:
  |           ^^^^^^
help: declare it: struct Widget:
error: test.ura:7:7 unknown type 'Widget'
  |
7 |     w Widget
  |       ^^^^^^
help: declare it: struct Widget:
error: test.ura:11:24 unknown type 'Widget'
   |
11 |     arr Widget[] = new<Widget>(3)
   |                        ^^^^^^
help: declare it: struct Widget:
error: test.ura:11:24 unknown type 'Widget'
   |
11 |     arr Widget[] = new<Widget>(3)
   |                        ^^^^^^
help: declare it: struct Widget:
error: test.ura:11:24 unknown type 'Widget'
   |
11 |     arr Widget[] = new<Widget>(3)
   |                        ^^^^^^
help: declare it: struct Widget:
error: test.ura:11:24 unknown type 'Widget'
   |
11 |     arr Widget[] = new<Widget>(3)
   |                        ^^^^^^
help: declare it: struct Widget:
error: test.ura:11:24 unknown type 'Widget'
   |
11 |     arr Widget[] = new<Widget>(3)
   |                        ^^^^^^
help: declare it: struct Widget:
error: test.ura:11:24 unknown type 'Widget'
   |
11 |     arr Widget[] = new<Widget>(3)
   |                        ^^^^^^
help: declare it: struct Widget:
error: test.ura:11:24 unknown type 'Widget'
   |
11 |     arr Widget[] = new<Widget>(3)
   |                        ^^^^^^
help: declare it: struct Widget:
error: test.ura:11:9 unknown type 'Widget'
   |
11 |     arr Widget[] = new<Widget>(3)
   |         ^^^^^^
help: declare it: struct Widget:
error: aborting due to 11 errors
```

### status

```
1
```

---

## 046 — struct: destructor must take no parameters

```ura
// error: Destructor 'drop' takes no parameters; write 'operator drop():'
use "@/header"

struct Box:
    value i32

    operator drop(extra i32) void:
        return

fn main() i32:
    b Box
    b.value = 1
    return 0
```

### stderr

```
error: test.ura:7:5 operator drop takes no parameters
  |
7 |     operator drop(extra i32) void:
  |     ^^^^^^^^
help: write operator drop():
error: aborting due to 1 error
```

### status

```
1
```

---

## 047 — struct: destructor must return void

```ura
// error: Destructor 'drop' returns void; drop the return type
use "@/header"

struct Box:
    value i32

    operator drop() i32:
        return 0

fn main() i32:
    b Box
    b.value = 1
    return 0
```

### stderr

```
error: test.ura:7:5 operator drop returns nothing
  |
7 |     operator drop() i32:
  |     ^^^^^^^^
help: write operator drop():
error: aborting due to 1 error
```

### status

```
1
```

---

## 048 — variable: global variables: scalars, arrays, forward reference, shadowing

```ura
// stdout: ura 1.500000 True
// stdout: total starts 7
// stdout: total now 9
// stdout: xs 7 len 3  heap 9 len 2
// stdout: local total 99
// expect: 0

name  char[] = "ura"
ratio f32 = 1.5
flag  bool  = True
xs    i32[] = i32[3]
heap  i32[] = new<i32>(2)

fn touch():
    total += 1

fn main() i32:
    output(name, " ", ratio, " ", flag, "\n")
    // forward reference: total is declared below main
    output("total starts ", total, "\n")
    touch()
    touch()
    output("total now ", total, "\n")

    xs[0] = 7
    heap[1] = 9
    output("xs ", xs[0], " len ", xs.len)
    output("  heap ", heap[1], " len ", heap.len, "\n")
    free<i32>(heap)

    // a local shadows the global for the rest of this scope
    total i32 = 99
    output("local total ", total, "\n")
    return 0

total i32 = 7
```

---

## 049 — variable: a global struct is initialised and dropped like a local

```ura
// stdout: start 10
// stdout: after 12
// stdout: DROP at 12
// expect: 0

struct Counter:
    hits i32

    pub fn create(n i32) Counter:
        c Counter
        c.hits = n
        return c

    operator =(other &Counter) void:
        self.hits = other.hits

    fn bump():
        self.hits += 1

    operator drop():
        output("DROP at ", self.hits, "\n")

tally Counter = Counter::create(10)

fn touch():
    tally.bump()

fn main() i32:
    output("start ", tally.hits, "\n")
    touch()
    touch()
    output("after ", tally.hits, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Counter = type { i32 }

@tally = global %Counter zeroinitializer
@str = private unnamed_addr constant [12 x i8] c"DROP at %d\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"start %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"after %d\0A\00", align 1

define %Counter @Counter.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %c = alloca %Counter, align 8
  store i32 %0, i32* %n, align 4
  store %Counter zeroinitializer, %Counter* %c, align 4
  %c.hits = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %c.hits, align 4
  %"$c" = load %Counter, %Counter* %c, align 4
  ret %Counter %"$c"
}

define void @Counter.bump(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.hits" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"$*self.hits" = load i32, i32* %"*self.hits", align 4
  %ADD = add i32 %"$*self.hits", 1
  store i32 %ADD, i32* %"*self.hits", align 4
  ret void
}

define void @Counter.drop(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.hits" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"$*self.hits" = load i32, i32* %"*self.hits", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$*self.hits")
  ret void
}

define void @touch() {
entry:
  call void @Counter.bump(%Counter* @tally)
  ret void
}

define i32 @main() {
entry:
  %call.Counter.create = call %Counter @Counter.create(i32 10)
  store %Counter %call.Counter.create, %Counter* @tally, align 4
  %"$tally.hits" = load i32, i32* getelementptr inbounds (%Counter, %Counter* @tally, i32 0, i32 0), align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$tally.hits")
  call void @touch()
  call void @touch()
  %"$tally.hits1" = load i32, i32* getelementptr inbounds (%Counter, %Counter* @tally, i32 0, i32 0), align 4
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"$tally.hits1")
  call void @Counter.drop(%Counter* @tally)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 050 — variable: a global is dropped on an early return too

```ura
// stdout: body
// stdout: early
// stdout: DROP 2
// stdout: DROP 1
// expect: 0

struct Res:
    id i32

    pub fn create(n i32) Res:
        r Res
        r.id = n
        return r

    operator =(other &Res) void:
        self.id = other.id

    operator drop():
        output("DROP ", self.id, "\n")

a Res = Res::create(1)
b Res = Res::create(2)

fn main() i32:
    output("body\n")
    if 1 < 2:
        output("early\n")
        return 0
    output("unreachable\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Res = type { i32 }

@a = global %Res zeroinitializer
@b = global %Res zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"DROP %d\0A\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"body\0A\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"early\0A\00", align 1
@str.3 = private unnamed_addr constant [13 x i8] c"unreachable\0A\00", align 1

define %Res @Res.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %r = alloca %Res, align 8
  store i32 %0, i32* %n, align 4
  store %Res zeroinitializer, %Res* %r, align 4
  %r.id = getelementptr inbounds %Res, %Res* %r, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %r.id, align 4
  %"$r" = load %Res, %Res* %r, align 4
  ret %Res %"$r"
}

define void @Res.drop(%Res* %0) {
entry:
  %self = alloca %Res*, align 8
  store %Res* %0, %Res** %self, align 8
  %"*self" = load %Res*, %Res** %self, align 8
  %"*self.id" = getelementptr inbounds %Res, %Res* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  ret void
}

define i32 @main() {
entry:
  %call.Res.create = call %Res @Res.create(i32 1)
  store %Res %call.Res.create, %Res* @a, align 4
  %call.Res.create1 = call %Res @Res.create(i32 2)
  store %Res %call.Res.create1, %Res* @b, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0))
  call void @Res.drop(%Res* @b)
  call void @Res.drop(%Res* @a)
  ret i32 0

endif:                                            ; preds = %if0
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.3, i32 0, i32 0))
  call void @Res.drop(%Res* @b)
  call void @Res.drop(%Res* @a)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

