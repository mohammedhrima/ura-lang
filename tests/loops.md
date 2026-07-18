# loops

## index

- 001 — basic while: count enemy kills
- 002 — loop (infinite) with break + continue
- 003 — range-for both directions (a..b)
- 004 — array-for iterates elements by value
- 005 — for ref mutates array elements in place
- 006 — for without a loop variable
- 007 — for over a non-iterable
- 008 — loop without a colon
- 009 — for ref over a range

## 001 — basic while: count enemy kills

```ura
// loops/001.ura - basic while: count enemy kills

main():
    kills int = 0
    while kills < 5:
        kills = kills + 1
    output("<", kills, ">\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : int
│  ├─ kills : int
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ kills : int
│  │  └─ int 5
│  └─ = : int
│     ├─ kills : int
│     └─ + : int
│        ├─ kills : int
│        └─ int 1
└─ output : void
   ├─ chars "<"
   ├─ kills : int
   └─ chars ">\n"
```

```out
<5>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %kills = alloca i32, align 4
  store i32 0, i32* %kills, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %kills1 = load i32, i32* %kills, align 4
  %lt = icmp slt i32 %kills1, 5
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %kills2 = load i32, i32* %kills, align 4
  %add = add i32 %kills2, 1
  store i32 %add, i32* %kills, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %kills3 = load i32, i32* %kills, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %kills3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — loop (infinite) with break + continue

```ura
// loops/002.ura - loop (infinite) with break + continue

main():
    i int = 0
    loop:
        i = i + 1
        if i == 3:
            continue
        if i == 6:
            break
        output(i, " ")
    output("\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : int
│  ├─ i : int
│  └─ int 0
├─ loop
│  ├─ = : int
│  │  ├─ i : int
│  │  └─ + : int
│  │     ├─ i : int
│  │     └─ int 1
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ i : int
│  │  │  └─ int 3
│  │  └─ continue
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ i : int
│  │  │  └─ int 6
│  │  └─ break
│  └─ output : void
│     ├─ i : int
│     └─ chars " "
└─ output : void
   └─ chars "\n"
```

```out
1 2 4 5 
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %loop.body

loop.body:                                        ; preds = %endif3, %then, %entry
  %i1 = load i32, i32* %i, align 4
  %add = add i32 %i1, 1
  store i32 %add, i32* %i, align 4
  %i2 = load i32, i32* %i, align 4
  %eq = icmp eq i32 %i2, 3
  br i1 %eq, label %then, label %endif

loop.end:                                         ; preds = %then4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0

endif:                                            ; preds = %loop.body
  %i5 = load i32, i32* %i, align 4
  %eq6 = icmp eq i32 %i5, 6
  br i1 %eq6, label %then4, label %endif3

then:                                             ; preds = %loop.body
  br label %loop.body

endif3:                                           ; preds = %endif
  %i7 = load i32, i32* %i, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %i7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %loop.body

then4:                                            ; preds = %endif
  br label %loop.end
}

declare i32 @printf(i8*, ...)
```

## 003 — range-for both directions (a..b)

```ura
// loops/003.ura - range-for counts up when a<b, down when a>b (exclusive end)

main():
    for i in 0..5:
        output(i, " ")
    output("| ")
    for i in 5..0:
        output(i, " ")
    output("\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ for
│  ├─ i : int
│  ├─ range : int
│  │  ├─ int 0
│  │  └─ int 5
│  └─ output : void
│     ├─ i : int
│     └─ chars " "
├─ output : void
│  └─ chars "| "
├─ for
│  ├─ i : int
│  ├─ range : int
│  │  ├─ int 5
│  │  └─ int 0
│  └─ output : void
│     ├─ i : int
│     └─ chars " "
└─ output : void
   └─ chars "\n"
```

```out
0 1 2 3 4 | 5 4 3 2 1 
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"| \00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i1 = load i32, i32* %i, align 4
  %more = icmp ne i32 %i1, 5
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %i2 = load i32, i32* %i, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %i2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i3 = load i32, i32* %i, align 4
  %next = add i32 %i3, 1
  store i32 %next, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  %i4 = alloca i32, align 4
  store i32 5, i32* %i4, align 4
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc7, %for.end
  %i9 = load i32, i32* %i4, align 4
  %more10 = icmp ne i32 %i9, 0
  br i1 %more10, label %for.body6, label %for.end8

for.body6:                                        ; preds = %for.cond5
  %i11 = load i32, i32* %i4, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %i11, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br label %for.inc7

for.inc7:                                         ; preds = %for.body6
  %i12 = load i32, i32* %i4, align 4
  %next13 = add i32 %i12, -1
  store i32 %next13, i32* %i4, align 4
  br label %for.cond5

for.end8:                                         ; preds = %for.cond5
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — array-for iterates elements by value

```ura
// loops/004.ura - for x in arr iterates elements

main():
    nums int[] = [10, 20, 30]
    total int = 0
    for x in nums:
        total = total + x
    output("sum = ", total, "\n")
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
│     └─ int 30
├─ = : int
│  ├─ total : int
│  └─ int 0
├─ for
│  ├─ x : int
│  ├─ nums : int[]
│  └─ = : int
│     ├─ total : int
│     └─ + : int
│        ├─ total : int
│        └─ x : int
└─ output : void
   ├─ chars "sum = "
   ├─ total : int
   └─ chars "\n"
```

```out
sum = 60
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"sum = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %nums = alloca { i32*, i64 }, align 8
  %arr = alloca i32, i64 3, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 10, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 20, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 30, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %nums, align 8
  %total = alloca i32, align 4
  store i32 0, i32* %total, align 4
  %nums3 = load { i32*, i64 }, { i32*, i64 }* %nums, align 8
  %arr.data = extractvalue { i32*, i64 } %nums3, 0
  %arr.len4 = extractvalue { i32*, i64 } %nums3, 1
  %idx = alloca i64, align 8
  store i64 0, i64* %idx, align 4
  %x = alloca i32, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i = load i64, i64* %idx, align 4
  %more = icmp slt i64 %i, %arr.len4
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %elem = getelementptr i32, i32* %arr.data, i64 %i
  %x5 = load i32, i32* %elem, align 4
  store i32 %x5, i32* %x, align 4
  %total6 = load i32, i32* %total, align 4
  %x7 = load i32, i32* %x, align 4
  %add = add i32 %total6, %x7
  store i32 %add, i32* %total, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i8 = load i64, i64* %idx, align 4
  %next = add i64 %i8, 1
  store i64 %next, i64* %idx, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %total9 = load i32, i32* %total, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %total9, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — for ref mutates array elements in place

```ura
// loops/005.ura - for ref x in arr aliases each element (writes go through)

main():
    nums int[] = [1, 2, 3, 4]
    for ref x in nums:
        x = x * 2
    for x in nums:
        output(x, " ")
    output("\n")
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
│     ├─ int 1
│     ├─ int 2
│     ├─ int 3
│     └─ int 4
├─ for
│  ├─ x : int
│  ├─ nums : int[]
│  └─ = : int
│     ├─ x : int
│     └─ * : int
│        ├─ x : int
│        └─ int 2
├─ for
│  ├─ x : int
│  ├─ nums : int[]
│  └─ output : void
│     ├─ x : int
│     └─ chars " "
└─ output : void
   └─ chars "\n"
```

```out
2 4 6 8 
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %nums = alloca { i32*, i64 }, align 8
  %arr = alloca i32, i64 4, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 1, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 2, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 3, i32* %arr.init2, align 4
  %arr.init3 = getelementptr i32, i32* %arr, i64 3
  store i32 4, i32* %arr.init3, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 4, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %nums, align 8
  %nums4 = load { i32*, i64 }, { i32*, i64 }* %nums, align 8
  %arr.data = extractvalue { i32*, i64 } %nums4, 0
  %arr.len5 = extractvalue { i32*, i64 } %nums4, 1
  %idx = alloca i64, align 8
  store i64 0, i64* %idx, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i = load i64, i64* %idx, align 4
  %more = icmp slt i64 %i, %arr.len5
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %elem = getelementptr i32, i32* %arr.data, i64 %i
  %x = load i32, i32* %elem, align 4
  %mul = mul i32 %x, 2
  store i32 %mul, i32* %elem, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i6 = load i64, i64* %idx, align 4
  %next = add i64 %i6, 1
  store i64 %next, i64* %idx, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %nums7 = load { i32*, i64 }, { i32*, i64 }* %nums, align 8
  %arr.data8 = extractvalue { i32*, i64 } %nums7, 0
  %arr.len9 = extractvalue { i32*, i64 } %nums7, 1
  %idx10 = alloca i64, align 8
  store i64 0, i64* %idx10, align 4
  %x11 = alloca i32, align 4
  br label %for.cond12

for.cond12:                                       ; preds = %for.inc14, %for.end
  %i16 = load i64, i64* %idx10, align 4
  %more17 = icmp slt i64 %i16, %arr.len9
  br i1 %more17, label %for.body13, label %for.end15

for.body13:                                       ; preds = %for.cond12
  %elem18 = getelementptr i32, i32* %arr.data8, i64 %i16
  %x19 = load i32, i32* %elem18, align 4
  store i32 %x19, i32* %x11, align 4
  %x20 = load i32, i32* %x11, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %x20, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %for.inc14

for.inc14:                                        ; preds = %for.body13
  %i21 = load i64, i64* %idx10, align 4
  %next22 = add i64 %i21, 1
  store i64 %next22, i64* %idx10, align 4
  br label %for.cond12

for.end15:                                        ; preds = %for.cond12
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — for without a loop variable

```ura
main():
    for in 0..5:
        output(1)
```

```tree
```

```out
```

```err
error: Expected a loop variable after 'for'
  006.ura:2:5
  |
2 |     for in 0..5:
  |     ^^^
```

```ll
```

## 007 — for over a non-iterable

```ura
main():
    x int = 5
    for i in x:
        output(i)
```

```tree
```

```out
```

```err
error: 'for i in ...' expects a range (a..b) or an array
  007.ura:3:5
  |
3 |     for i in x:
  |     ^^^
```

```ll
```

## 008 — loop without a colon

```ura
main():
    loop
        output(1)
```

```tree
```

```out
```

```err
error: Expected ':' to open the 'loop' body
  008.ura:2:5
  |
2 |     loop
  |     ^^^^
```

```ll
```

## 009 — for ref over a range

```ura
main():
    for ref i in 0..5:
        output(i)
```

```tree
```

```out
```

```err
error: 'for ref' needs an array; a range yields values, not storage
  009.ura:2:5
  |
2 |     for ref i in 0..5:
  |     ^^^
```

```ll
```
