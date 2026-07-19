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
- 010 — `for` over struct-array and 2D-array struct fields
- 010 — `for` over struct-array and 2D-array struct fields
- 011 — `by` sets the step of a range
- 012 — a `by` step must be a positive integer on a range
- 013 — `by` needs a range on its left

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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : i32
│  ├─ kills : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ kills : i32
│  │  └─ int 5
│  └─ = : i32
│     ├─ kills : i32
│     └─ + : i32
│        ├─ kills : i32
│        └─ int 1
└─ output : void
   ├─ chars "<"
   ├─ kills : i32
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : i32
│  ├─ i : i32
│  └─ int 0
├─ loop
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ + : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 3
│  │  └─ continue
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 6
│  │  └─ break
│  └─ output : void
│     ├─ i : i32
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 0
│  │  └─ int 5
│  └─ output : void
│     ├─ i : i32
│     └─ chars " "
├─ output : void
│  └─ chars "| "
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 5
│  │  └─ int 0
│  └─ output : void
│     ├─ i : i32
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
  %lt = icmp slt i32 %i1, 5
  %gt = icmp sgt i32 %i1, 5
  %more = select i1 true, i1 %lt, i1 %gt
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
  %lt10 = icmp slt i32 %i9, 0
  %gt11 = icmp sgt i32 %i9, 0
  %more12 = select i1 false, i1 %lt10, i1 %gt11
  br i1 %more12, label %for.body6, label %for.end8

for.body6:                                        ; preds = %for.cond5
  %i13 = load i32, i32* %i4, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %i13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br label %for.inc7

for.inc7:                                         ; preds = %for.body6
  %i14 = load i32, i32* %i4, align 4
  %next15 = add i32 %i14, -1
  store i32 %next15, i32* %i4, align 4
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : array
│  ├─ nums : i32[]
│  └─ array : i32[]
│     ├─ int 10
│     ├─ int 20
│     └─ int 30
├─ = : i32
│  ├─ total : i32
│  └─ int 0
├─ for
│  ├─ x : i32
│  ├─ nums : i32[]
│  └─ = : i32
│     ├─ total : i32
│     └─ + : i32
│        ├─ total : i32
│        └─ x : i32
└─ output : void
   ├─ chars "sum = "
   ├─ total : i32
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : array
│  ├─ nums : i32[]
│  └─ array : i32[]
│     ├─ int 1
│     ├─ int 2
│     ├─ int 3
│     └─ int 4
├─ for
│  ├─ x : i32
│  ├─ nums : i32[]
│  └─ = : i32
│     ├─ x : i32
│     └─ * : i32
│        ├─ x : i32
│        └─ int 2
├─ for
│  ├─ x : i32
│  ├─ nums : i32[]
│  └─ output : void
│     ├─ x : i32
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

## 010 — `for` over struct-array and 2D-array struct fields

```ura
// loops/010.ura - for over a struct field, by ref and nested

struct Room:
    floor int

struct World:
    grid  int[][]
    rooms Room[]

    pub fn create() World:
        w World
        w.grid  = new int[2][3]
        w.rooms = new Room[3]
        i int = 0
        while i < 3:
            w.rooms[i].floor = i
            i += 1
        w.grid[1][2] = 9
        return w

    // nested for over a 2D field: 'row' must be typed int[]
    fn show_grid() void:
        for row in self.grid:
            for v in row:
                output(v, " ")
            output("\n")

    // for ref over a struct-array field mutates in place
    fn bump() void:
        for r in self.rooms:
            output(r.floor, " ")
        output("\n")
        for ref r in self.rooms:
            r.floor = r.floor * 10
        for r in self.rooms:
            output(r.floor, " ")
        output("\n")

main():
    w World = World::create()
    w.show_grid()
    w.bump()
    clean w.grid
    clean w.rooms
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

struct Room
└─ floor : i32

struct World
├─ grid : i32[][]
├─ rooms : STRUCT_CALL[]
├─ fn World.create() : STRUCT_CALL
│  ├─ w : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .grid : i32[][]
│  │  │  └─ w : STRUCT_CALL
│  │  └─ array : i32[][]
│  │     ├─ int 2
│  │     └─ int 3
│  ├─ = : array
│  │  ├─ .rooms : STRUCT_CALL[]
│  │  │  └─ w : STRUCT_CALL
│  │  └─ array : STRUCT_CALL[]
│  │     └─ int 3
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 3
│  │  ├─ = : i32
│  │  │  ├─ .floor : i32
│  │  │  │  └─ index : STRUCT_CALL
│  │  │  │     ├─ .rooms : STRUCT_CALL[]
│  │  │  │     │  └─ w : STRUCT_CALL
│  │  │  │     └─ i : i32
│  │  │  └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  ├─ = : i32
│  │  ├─ index : i32
│  │  │  ├─ index : i32[]
│  │  │  │  ├─ .grid : i32[][]
│  │  │  │  │  └─ w : STRUCT_CALL
│  │  │  │  └─ int 1
│  │  │  └─ int 2
│  │  └─ int 9
│  └─ return
│     └─ w : STRUCT_CALL
├─ fn World.show_grid(self : STRUCT_CALL) : void
│  └─ for
│     ├─ row : i32[]
│     ├─ .grid : i32[][]
│     │  └─ self : STRUCT_CALL
│     ├─ for
│     │  ├─ v : i32
│     │  ├─ row : i32[]
│     │  └─ output : void
│     │     ├─ v : i32
│     │     └─ chars " "
│     └─ output : void
│        └─ chars "\n"
└─ fn World.bump(self : STRUCT_CALL) : void
   ├─ for
   │  ├─ r : STRUCT_CALL
   │  ├─ .rooms : STRUCT_CALL[]
   │  │  └─ self : STRUCT_CALL
   │  └─ output : void
   │     ├─ .floor : i32
   │     │  └─ r : STRUCT_CALL
   │     └─ chars " "
   ├─ output : void
   │  └─ chars "\n"
   ├─ for
   │  ├─ r : STRUCT_CALL
   │  ├─ .rooms : STRUCT_CALL[]
   │  │  └─ self : STRUCT_CALL
   │  └─ = : i32
   │     ├─ .floor : i32
   │     │  └─ r : STRUCT_CALL
   │     └─ * : i32
   │        ├─ .floor : i32
   │        │  └─ r : STRUCT_CALL
   │        └─ int 10
   ├─ for
   │  ├─ r : STRUCT_CALL
   │  ├─ .rooms : STRUCT_CALL[]
   │  │  └─ self : STRUCT_CALL
   │  └─ output : void
   │     ├─ .floor : i32
   │     │  └─ r : STRUCT_CALL
   │     └─ chars " "
   └─ output : void
      └─ chars "\n"

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ w : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ call show_grid : void
│  └─ w : STRUCT_CALL
├─ call bump : void
│  └─ w : STRUCT_CALL
├─ clean : void
│  └─ .grid : i32[][]
│     └─ w : STRUCT_CALL
└─ clean : void
   └─ .rooms : STRUCT_CALL[]
      └─ w : STRUCT_CALL
```

```out
0 0 0 
0 0 9 
0 1 2 
0 10 20 
```

```err
```

```ll

%World = type { { { i32*, i64 }*, i64 }, { %Room*, i64 } }
%Room = type { i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define %World @World.create() {
entry:
  %w = alloca %World, align 8
  store %World zeroinitializer, %World* %w, align 8
  %grid = getelementptr %World, %World* %w, i32 0, i32 0
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
  store { { i32*, i64 }*, i64 } %arr.len6, { { i32*, i64 }*, i64 }* %grid, align 8
  %rooms = getelementptr %World, %World* %w, i32 0, i32 1
  %heap7 = call i8* @calloc(i64 3, i64 4)
  %arr8 = bitcast i8* %heap7 to %Room*
  %arr.ptr9 = insertvalue { %Room*, i64 } undef, %Room* %arr8, 0
  %arr.len10 = insertvalue { %Room*, i64 } %arr.ptr9, i64 3, 1
  store { %Room*, i64 } %arr.len10, { %Room*, i64 }* %rooms, align 8
  %i11 = alloca i32, align 4
  store i32 0, i32* %i11, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %arr.end
  %i12 = load i32, i32* %i11, align 4
  %lt = icmp slt i32 %i12, 3
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %rooms13 = getelementptr %World, %World* %w, i32 0, i32 1
  %rooms14 = load { %Room*, i64 }, { %Room*, i64 }* %rooms13, align 8
  %arr.data = extractvalue { %Room*, i64 } %rooms14, 0
  %i15 = load i32, i32* %i11, align 4
  %arr.at = getelementptr %Room, %Room* %arr.data, i32 %i15
  %floor = getelementptr %Room, %Room* %arr.at, i32 0, i32 0
  %i16 = load i32, i32* %i11, align 4
  store i32 %i16, i32* %floor, align 4
  %cur = load i32, i32* %i11, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i11, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %grid17 = getelementptr %World, %World* %w, i32 0, i32 0
  %grid18 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid17, align 8
  %arr.data19 = extractvalue { { i32*, i64 }*, i64 } %grid18, 0
  %arr.at20 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data19, i32 1
  %idx = load { i32*, i64 }, { i32*, i64 }* %arr.at20, align 8
  %arr.data21 = extractvalue { i32*, i64 } %idx, 0
  %arr.at22 = getelementptr i32, i32* %arr.data21, i32 2
  store i32 9, i32* %arr.at22, align 4
  %w23 = load %World, %World* %w, align 8
  ret %World %w23
}

declare i8* @calloc(i64, i64)

define void @World.show_grid(%World* %0) {
entry:
  %self = alloca %World*, align 8
  store %World* %0, %World** %self, align 8
  %ref = load %World*, %World** %self, align 8
  %grid = getelementptr %World, %World* %ref, i32 0, i32 0
  %grid1 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %arr.data = extractvalue { { i32*, i64 }*, i64 } %grid1, 0
  %arr.len = extractvalue { { i32*, i64 }*, i64 } %grid1, 1
  %idx = alloca i64, align 8
  store i64 0, i64* %idx, align 4
  %row = alloca { i32*, i64 }, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i = load i64, i64* %idx, align 4
  %more = icmp slt i64 %i, %arr.len
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %elem = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data, i64 %i
  %x = load { i32*, i64 }, { i32*, i64 }* %elem, align 8
  store { i32*, i64 } %x, { i32*, i64 }* %row, align 8
  %row2 = load { i32*, i64 }, { i32*, i64 }* %row, align 8
  %arr.data3 = extractvalue { i32*, i64 } %row2, 0
  %arr.len4 = extractvalue { i32*, i64 } %row2, 1
  %idx5 = alloca i64, align 8
  store i64 0, i64* %idx5, align 4
  %v = alloca i32, align 4
  br label %for.cond6

for.inc:                                          ; preds = %for.end9
  %i16 = load i64, i64* %idx, align 4
  %next17 = add i64 %i16, 1
  store i64 %next17, i64* %idx, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void

for.cond6:                                        ; preds = %for.inc8, %for.body
  %i10 = load i64, i64* %idx5, align 4
  %more11 = icmp slt i64 %i10, %arr.len4
  br i1 %more11, label %for.body7, label %for.end9

for.body7:                                        ; preds = %for.cond6
  %elem12 = getelementptr i32, i32* %arr.data3, i64 %i10
  %x13 = load i32, i32* %elem12, align 4
  store i32 %x13, i32* %v, align 4
  %v14 = load i32, i32* %v, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %v14, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %for.inc8

for.inc8:                                         ; preds = %for.body7
  %i15 = load i64, i64* %idx5, align 4
  %next = add i64 %i15, 1
  store i64 %next, i64* %idx5, align 4
  br label %for.cond6

for.end9:                                         ; preds = %for.cond6
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %for.inc
}

declare i32 @printf(i8*, ...)

define void @World.bump(%World* %0) {
entry:
  %self = alloca %World*, align 8
  store %World* %0, %World** %self, align 8
  %ref = load %World*, %World** %self, align 8
  %rooms = getelementptr %World, %World* %ref, i32 0, i32 1
  %rooms1 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data = extractvalue { %Room*, i64 } %rooms1, 0
  %arr.len = extractvalue { %Room*, i64 } %rooms1, 1
  %idx = alloca i64, align 8
  store i64 0, i64* %idx, align 4
  %r = alloca %Room, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i = load i64, i64* %idx, align 4
  %more = icmp slt i64 %i, %arr.len
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %elem = getelementptr %Room, %Room* %arr.data, i64 %i
  %x = load %Room, %Room* %elem, align 4
  store %Room %x, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  %floor2 = load i32, i32* %floor, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %floor2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i3 = load i64, i64* %idx, align 4
  %next = add i64 %i3, 1
  store i64 %next, i64* %idx, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %ref4 = load %World*, %World** %self, align 8
  %rooms5 = getelementptr %World, %World* %ref4, i32 0, i32 1
  %rooms6 = load { %Room*, i64 }, { %Room*, i64 }* %rooms5, align 8
  %arr.data7 = extractvalue { %Room*, i64 } %rooms6, 0
  %arr.len8 = extractvalue { %Room*, i64 } %rooms6, 1
  %idx9 = alloca i64, align 8
  store i64 0, i64* %idx9, align 4
  br label %for.cond10

for.cond10:                                       ; preds = %for.inc12, %for.end
  %i14 = load i64, i64* %idx9, align 4
  %more15 = icmp slt i64 %i14, %arr.len8
  br i1 %more15, label %for.body11, label %for.end13

for.body11:                                       ; preds = %for.cond10
  %elem16 = getelementptr %Room, %Room* %arr.data7, i64 %i14
  %floor17 = getelementptr %Room, %Room* %elem16, i32 0, i32 0
  %floor18 = getelementptr %Room, %Room* %elem16, i32 0, i32 0
  %floor19 = load i32, i32* %floor18, align 4
  %mul = mul i32 %floor19, 10
  store i32 %mul, i32* %floor17, align 4
  br label %for.inc12

for.inc12:                                        ; preds = %for.body11
  %i20 = load i64, i64* %idx9, align 4
  %next21 = add i64 %i20, 1
  store i64 %next21, i64* %idx9, align 4
  br label %for.cond10

for.end13:                                        ; preds = %for.cond10
  %ref22 = load %World*, %World** %self, align 8
  %rooms23 = getelementptr %World, %World* %ref22, i32 0, i32 1
  %rooms24 = load { %Room*, i64 }, { %Room*, i64 }* %rooms23, align 8
  %arr.data25 = extractvalue { %Room*, i64 } %rooms24, 0
  %arr.len26 = extractvalue { %Room*, i64 } %rooms24, 1
  %idx27 = alloca i64, align 8
  store i64 0, i64* %idx27, align 4
  %r28 = alloca %Room, align 8
  br label %for.cond29

for.cond29:                                       ; preds = %for.inc31, %for.end13
  %i33 = load i64, i64* %idx27, align 4
  %more34 = icmp slt i64 %i33, %arr.len26
  br i1 %more34, label %for.body30, label %for.end32

for.body30:                                       ; preds = %for.cond29
  %elem35 = getelementptr %Room, %Room* %arr.data25, i64 %i33
  %x36 = load %Room, %Room* %elem35, align 4
  store %Room %x36, %Room* %r28, align 4
  %floor37 = getelementptr %Room, %Room* %r28, i32 0, i32 0
  %floor38 = load i32, i32* %floor37, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i32 %floor38, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  br label %for.inc31

for.inc31:                                        ; preds = %for.body30
  %i39 = load i64, i64* %idx27, align 4
  %next40 = add i64 %i39, 1
  store i64 %next40, i64* %idx27, align 4
  br label %for.cond29

for.end32:                                        ; preds = %for.cond29
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  %w = alloca %World, align 8
  %call = call %World @World.create()
  store %World %call, %World* %w, align 8
  call void @World.show_grid(%World* %w)
  call void @World.bump(%World* %w)
  %grid = getelementptr %World, %World* %w, i32 0, i32 0
  %arr = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %arr.data = extractvalue { { i32*, i64 }*, i64 } %arr, 0
  %arr.len = extractvalue { { i32*, i64 }*, i64 } %arr, 1
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %free.cond

free.cond:                                        ; preds = %free.body, %entry
  %i1 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i1, %arr.len
  br i1 %more, label %free.body, label %free.end

free.body:                                        ; preds = %free.cond
  %free.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data, i64 %i1
  %inner = load { i32*, i64 }, { i32*, i64 }* %free.slot, align 8
  %arr.data2 = extractvalue { i32*, i64 } %inner, 0
  %free.ptr = bitcast i32* %arr.data2 to i8*
  call void @free(i8* %free.ptr)
  %i3 = load i64, i64* %i, align 4
  %next = add i64 %i3, 1
  store i64 %next, i64* %i, align 4
  br label %free.cond

free.end:                                         ; preds = %free.cond
  %free.ptr4 = bitcast { i32*, i64 }* %arr.data to i8*
  call void @free(i8* %free.ptr4)
  store { { i32*, i64 }*, i64 } zeroinitializer, { { i32*, i64 }*, i64 }* %grid, align 8
  %rooms = getelementptr %World, %World* %w, i32 0, i32 1
  %arr5 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data6 = extractvalue { %Room*, i64 } %arr5, 0
  %free.ptr7 = bitcast %Room* %arr.data6 to i8*
  call void @free(i8* %free.ptr7)
  store { %Room*, i64 } zeroinitializer, { %Room*, i64 }* %rooms, align 8
  ret i32 0
}

declare void @free(i8*)
```

## 010 — `for` over struct-array and 2D-array struct fields

```ura
// loops/010.ura - for over a struct field, by ref and nested

struct Room:
    floor int

struct World:
    grid  int[][]
    rooms Room[]

    pub fn create() World:
        w World
        w.grid  = new int[2][3]
        w.rooms = new Room[3]
        i int = 0
        while i < 3:
            w.rooms[i].floor = i
            i += 1
        w.grid[1][2] = 9
        return w

    // nested for over a 2D field: 'row' must be typed int[]
    fn show_grid() void:
        for row in self.grid:
            for v in row:
                output(v, " ")
            output("\n")

    // for ref over a struct-array field mutates in place
    fn bump() void:
        for r in self.rooms:
            output(r.floor, " ")
        output("\n")
        for ref r in self.rooms:
            r.floor = r.floor * 10
        for r in self.rooms:
            output(r.floor, " ")
        output("\n")

main():
    w World = World::create()
    w.show_grid()
    w.bump()
    clean w.grid
    clean w.rooms
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

struct Room
└─ floor : i32

struct World
├─ grid : i32[][]
├─ rooms : STRUCT_CALL[]
├─ fn World.create() : STRUCT_CALL
│  ├─ w : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .grid : i32[][]
│  │  │  └─ w : STRUCT_CALL
│  │  └─ array : i32[][]
│  │     ├─ int 2
│  │     └─ int 3
│  ├─ = : array
│  │  ├─ .rooms : STRUCT_CALL[]
│  │  │  └─ w : STRUCT_CALL
│  │  └─ array : STRUCT_CALL[]
│  │     └─ int 3
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 3
│  │  ├─ = : i32
│  │  │  ├─ .floor : i32
│  │  │  │  └─ index : STRUCT_CALL
│  │  │  │     ├─ .rooms : STRUCT_CALL[]
│  │  │  │     │  └─ w : STRUCT_CALL
│  │  │  │     └─ i : i32
│  │  │  └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  ├─ = : i32
│  │  ├─ index : i32
│  │  │  ├─ index : i32[]
│  │  │  │  ├─ .grid : i32[][]
│  │  │  │  │  └─ w : STRUCT_CALL
│  │  │  │  └─ int 1
│  │  │  └─ int 2
│  │  └─ int 9
│  └─ return
│     └─ w : STRUCT_CALL
├─ fn World.show_grid(self : STRUCT_CALL) : void
│  └─ for
│     ├─ row : i32[]
│     ├─ .grid : i32[][]
│     │  └─ self : STRUCT_CALL
│     ├─ for
│     │  ├─ v : i32
│     │  ├─ row : i32[]
│     │  └─ output : void
│     │     ├─ v : i32
│     │     └─ chars " "
│     └─ output : void
│        └─ chars "\n"
└─ fn World.bump(self : STRUCT_CALL) : void
   ├─ for
   │  ├─ r : STRUCT_CALL
   │  ├─ .rooms : STRUCT_CALL[]
   │  │  └─ self : STRUCT_CALL
   │  └─ output : void
   │     ├─ .floor : i32
   │     │  └─ r : STRUCT_CALL
   │     └─ chars " "
   ├─ output : void
   │  └─ chars "\n"
   ├─ for
   │  ├─ r : STRUCT_CALL
   │  ├─ .rooms : STRUCT_CALL[]
   │  │  └─ self : STRUCT_CALL
   │  └─ = : i32
   │     ├─ .floor : i32
   │     │  └─ r : STRUCT_CALL
   │     └─ * : i32
   │        ├─ .floor : i32
   │        │  └─ r : STRUCT_CALL
   │        └─ int 10
   ├─ for
   │  ├─ r : STRUCT_CALL
   │  ├─ .rooms : STRUCT_CALL[]
   │  │  └─ self : STRUCT_CALL
   │  └─ output : void
   │     ├─ .floor : i32
   │     │  └─ r : STRUCT_CALL
   │     └─ chars " "
   └─ output : void
      └─ chars "\n"

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ w : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ call show_grid : void
│  └─ w : STRUCT_CALL
├─ call bump : void
│  └─ w : STRUCT_CALL
├─ clean : void
│  └─ .grid : i32[][]
│     └─ w : STRUCT_CALL
└─ clean : void
   └─ .rooms : STRUCT_CALL[]
      └─ w : STRUCT_CALL
```

```out
0 0 0 
0 0 9 
0 1 2 
0 10 20 
```

```err
```

```ll

%World = type { { { i32*, i64 }*, i64 }, { %Room*, i64 } }
%Room = type { i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define %World @World.create() {
entry:
  %w = alloca %World, align 8
  store %World zeroinitializer, %World* %w, align 8
  %grid = getelementptr %World, %World* %w, i32 0, i32 0
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
  store { { i32*, i64 }*, i64 } %arr.len6, { { i32*, i64 }*, i64 }* %grid, align 8
  %rooms = getelementptr %World, %World* %w, i32 0, i32 1
  %heap7 = call i8* @calloc(i64 3, i64 4)
  %arr8 = bitcast i8* %heap7 to %Room*
  %arr.ptr9 = insertvalue { %Room*, i64 } undef, %Room* %arr8, 0
  %arr.len10 = insertvalue { %Room*, i64 } %arr.ptr9, i64 3, 1
  store { %Room*, i64 } %arr.len10, { %Room*, i64 }* %rooms, align 8
  %i11 = alloca i32, align 4
  store i32 0, i32* %i11, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %arr.end
  %i12 = load i32, i32* %i11, align 4
  %lt = icmp slt i32 %i12, 3
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %rooms13 = getelementptr %World, %World* %w, i32 0, i32 1
  %rooms14 = load { %Room*, i64 }, { %Room*, i64 }* %rooms13, align 8
  %arr.data = extractvalue { %Room*, i64 } %rooms14, 0
  %i15 = load i32, i32* %i11, align 4
  %arr.at = getelementptr %Room, %Room* %arr.data, i32 %i15
  %floor = getelementptr %Room, %Room* %arr.at, i32 0, i32 0
  %i16 = load i32, i32* %i11, align 4
  store i32 %i16, i32* %floor, align 4
  %cur = load i32, i32* %i11, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i11, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %grid17 = getelementptr %World, %World* %w, i32 0, i32 0
  %grid18 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid17, align 8
  %arr.data19 = extractvalue { { i32*, i64 }*, i64 } %grid18, 0
  %arr.at20 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data19, i32 1
  %idx = load { i32*, i64 }, { i32*, i64 }* %arr.at20, align 8
  %arr.data21 = extractvalue { i32*, i64 } %idx, 0
  %arr.at22 = getelementptr i32, i32* %arr.data21, i32 2
  store i32 9, i32* %arr.at22, align 4
  %w23 = load %World, %World* %w, align 8
  ret %World %w23
}

declare i8* @calloc(i64, i64)

define void @World.show_grid(%World* %0) {
entry:
  %self = alloca %World*, align 8
  store %World* %0, %World** %self, align 8
  %ref = load %World*, %World** %self, align 8
  %grid = getelementptr %World, %World* %ref, i32 0, i32 0
  %grid1 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %arr.data = extractvalue { { i32*, i64 }*, i64 } %grid1, 0
  %arr.len = extractvalue { { i32*, i64 }*, i64 } %grid1, 1
  %idx = alloca i64, align 8
  store i64 0, i64* %idx, align 4
  %row = alloca { i32*, i64 }, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i = load i64, i64* %idx, align 4
  %more = icmp slt i64 %i, %arr.len
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %elem = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data, i64 %i
  %x = load { i32*, i64 }, { i32*, i64 }* %elem, align 8
  store { i32*, i64 } %x, { i32*, i64 }* %row, align 8
  %row2 = load { i32*, i64 }, { i32*, i64 }* %row, align 8
  %arr.data3 = extractvalue { i32*, i64 } %row2, 0
  %arr.len4 = extractvalue { i32*, i64 } %row2, 1
  %idx5 = alloca i64, align 8
  store i64 0, i64* %idx5, align 4
  %v = alloca i32, align 4
  br label %for.cond6

for.inc:                                          ; preds = %for.end9
  %i16 = load i64, i64* %idx, align 4
  %next17 = add i64 %i16, 1
  store i64 %next17, i64* %idx, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void

for.cond6:                                        ; preds = %for.inc8, %for.body
  %i10 = load i64, i64* %idx5, align 4
  %more11 = icmp slt i64 %i10, %arr.len4
  br i1 %more11, label %for.body7, label %for.end9

for.body7:                                        ; preds = %for.cond6
  %elem12 = getelementptr i32, i32* %arr.data3, i64 %i10
  %x13 = load i32, i32* %elem12, align 4
  store i32 %x13, i32* %v, align 4
  %v14 = load i32, i32* %v, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %v14, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %for.inc8

for.inc8:                                         ; preds = %for.body7
  %i15 = load i64, i64* %idx5, align 4
  %next = add i64 %i15, 1
  store i64 %next, i64* %idx5, align 4
  br label %for.cond6

for.end9:                                         ; preds = %for.cond6
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %for.inc
}

declare i32 @printf(i8*, ...)

define void @World.bump(%World* %0) {
entry:
  %self = alloca %World*, align 8
  store %World* %0, %World** %self, align 8
  %ref = load %World*, %World** %self, align 8
  %rooms = getelementptr %World, %World* %ref, i32 0, i32 1
  %rooms1 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data = extractvalue { %Room*, i64 } %rooms1, 0
  %arr.len = extractvalue { %Room*, i64 } %rooms1, 1
  %idx = alloca i64, align 8
  store i64 0, i64* %idx, align 4
  %r = alloca %Room, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i = load i64, i64* %idx, align 4
  %more = icmp slt i64 %i, %arr.len
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %elem = getelementptr %Room, %Room* %arr.data, i64 %i
  %x = load %Room, %Room* %elem, align 4
  store %Room %x, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  %floor2 = load i32, i32* %floor, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %floor2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i3 = load i64, i64* %idx, align 4
  %next = add i64 %i3, 1
  store i64 %next, i64* %idx, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %ref4 = load %World*, %World** %self, align 8
  %rooms5 = getelementptr %World, %World* %ref4, i32 0, i32 1
  %rooms6 = load { %Room*, i64 }, { %Room*, i64 }* %rooms5, align 8
  %arr.data7 = extractvalue { %Room*, i64 } %rooms6, 0
  %arr.len8 = extractvalue { %Room*, i64 } %rooms6, 1
  %idx9 = alloca i64, align 8
  store i64 0, i64* %idx9, align 4
  br label %for.cond10

for.cond10:                                       ; preds = %for.inc12, %for.end
  %i14 = load i64, i64* %idx9, align 4
  %more15 = icmp slt i64 %i14, %arr.len8
  br i1 %more15, label %for.body11, label %for.end13

for.body11:                                       ; preds = %for.cond10
  %elem16 = getelementptr %Room, %Room* %arr.data7, i64 %i14
  %floor17 = getelementptr %Room, %Room* %elem16, i32 0, i32 0
  %floor18 = getelementptr %Room, %Room* %elem16, i32 0, i32 0
  %floor19 = load i32, i32* %floor18, align 4
  %mul = mul i32 %floor19, 10
  store i32 %mul, i32* %floor17, align 4
  br label %for.inc12

for.inc12:                                        ; preds = %for.body11
  %i20 = load i64, i64* %idx9, align 4
  %next21 = add i64 %i20, 1
  store i64 %next21, i64* %idx9, align 4
  br label %for.cond10

for.end13:                                        ; preds = %for.cond10
  %ref22 = load %World*, %World** %self, align 8
  %rooms23 = getelementptr %World, %World* %ref22, i32 0, i32 1
  %rooms24 = load { %Room*, i64 }, { %Room*, i64 }* %rooms23, align 8
  %arr.data25 = extractvalue { %Room*, i64 } %rooms24, 0
  %arr.len26 = extractvalue { %Room*, i64 } %rooms24, 1
  %idx27 = alloca i64, align 8
  store i64 0, i64* %idx27, align 4
  %r28 = alloca %Room, align 8
  br label %for.cond29

for.cond29:                                       ; preds = %for.inc31, %for.end13
  %i33 = load i64, i64* %idx27, align 4
  %more34 = icmp slt i64 %i33, %arr.len26
  br i1 %more34, label %for.body30, label %for.end32

for.body30:                                       ; preds = %for.cond29
  %elem35 = getelementptr %Room, %Room* %arr.data25, i64 %i33
  %x36 = load %Room, %Room* %elem35, align 4
  store %Room %x36, %Room* %r28, align 4
  %floor37 = getelementptr %Room, %Room* %r28, i32 0, i32 0
  %floor38 = load i32, i32* %floor37, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i32 %floor38, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  br label %for.inc31

for.inc31:                                        ; preds = %for.body30
  %i39 = load i64, i64* %idx27, align 4
  %next40 = add i64 %i39, 1
  store i64 %next40, i64* %idx27, align 4
  br label %for.cond29

for.end32:                                        ; preds = %for.cond29
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  %w = alloca %World, align 8
  %call = call %World @World.create()
  store %World %call, %World* %w, align 8
  call void @World.show_grid(%World* %w)
  call void @World.bump(%World* %w)
  %grid = getelementptr %World, %World* %w, i32 0, i32 0
  %arr = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %arr.data = extractvalue { { i32*, i64 }*, i64 } %arr, 0
  %arr.len = extractvalue { { i32*, i64 }*, i64 } %arr, 1
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %free.cond

free.cond:                                        ; preds = %free.body, %entry
  %i1 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i1, %arr.len
  br i1 %more, label %free.body, label %free.end

free.body:                                        ; preds = %free.cond
  %free.slot = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data, i64 %i1
  %inner = load { i32*, i64 }, { i32*, i64 }* %free.slot, align 8
  %arr.data2 = extractvalue { i32*, i64 } %inner, 0
  %free.ptr = bitcast i32* %arr.data2 to i8*
  call void @free(i8* %free.ptr)
  %i3 = load i64, i64* %i, align 4
  %next = add i64 %i3, 1
  store i64 %next, i64* %i, align 4
  br label %free.cond

free.end:                                         ; preds = %free.cond
  %free.ptr4 = bitcast { i32*, i64 }* %arr.data to i8*
  call void @free(i8* %free.ptr4)
  store { { i32*, i64 }*, i64 } zeroinitializer, { { i32*, i64 }*, i64 }* %grid, align 8
  %rooms = getelementptr %World, %World* %w, i32 0, i32 1
  %arr5 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data6 = extractvalue { %Room*, i64 } %arr5, 0
  %free.ptr7 = bitcast %Room* %arr.data6 to i8*
  call void @free(i8* %free.ptr7)
  store { %Room*, i64 } zeroinitializer, { %Room*, i64 }* %rooms, align 8
  ret i32 0
}

declare void @free(i8*)
```

## 011 — `by` sets the step of a range

```ura
// loops/011.ura - 'by' sets the step of a range

main():
    for i in 0..10 by 3:
        output(i, " ")
    output("\n")
    for i in 10..0 by 3:
        output(i, " ")
    output("\n")
    n int = 4
    for i in 0..20 by n:
        output(i, " ")
    output("\n")
    // a step wider than the span yields the start only
    for i in 0..3 by 10:
        output(i, " ")
    output("\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 0
│  │  ├─ int 10
│  │  └─ int 3
│  └─ output : void
│     ├─ i : i32
│     └─ chars " "
├─ output : void
│  └─ chars "\n"
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 10
│  │  ├─ int 0
│  │  └─ int 3
│  └─ output : void
│     ├─ i : i32
│     └─ chars " "
├─ output : void
│  └─ chars "\n"
├─ = : i32
│  ├─ n : i32
│  └─ int 4
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 0
│  │  ├─ int 20
│  │  └─ n : i32
│  └─ output : void
│     ├─ i : i32
│     └─ chars " "
├─ output : void
│  └─ chars "\n"
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 0
│  │  ├─ int 3
│  │  └─ int 10
│  └─ output : void
│     ├─ i : i32
│     └─ chars " "
└─ output : void
   └─ chars "\n"
```

```out
0 3 6 9 
10 7 4 1 
0 4 8 12 16 
0 
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.11 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 10
  %gt = icmp sgt i32 %i1, 10
  %more = select i1 true, i1 %lt, i1 %gt
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %i2 = load i32, i32* %i, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %i2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i3 = load i32, i32* %i, align 4
  %next = add i32 %i3, 3
  store i32 %next, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %i4 = alloca i32, align 4
  store i32 10, i32* %i4, align 4
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc7, %for.end
  %i9 = load i32, i32* %i4, align 4
  %lt10 = icmp slt i32 %i9, 0
  %gt11 = icmp sgt i32 %i9, 0
  %more12 = select i1 false, i1 %lt10, i1 %gt11
  br i1 %more12, label %for.body6, label %for.end8

for.body6:                                        ; preds = %for.cond5
  %i13 = load i32, i32* %i4, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %i13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br label %for.inc7

for.inc7:                                         ; preds = %for.body6
  %i14 = load i32, i32* %i4, align 4
  %next15 = add i32 %i14, -3
  store i32 %next15, i32* %i4, align 4
  br label %for.cond5

for.end8:                                         ; preds = %for.cond5
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %n = alloca i32, align 4
  store i32 4, i32* %n, align 4
  %n16 = load i32, i32* %n, align 4
  %by.neg = sub i32 0, %n16
  %step = select i1 true, i32 %n16, i32 %by.neg
  %i17 = alloca i32, align 4
  store i32 0, i32* %i17, align 4
  br label %for.cond18

for.cond18:                                       ; preds = %for.inc20, %for.end8
  %i22 = load i32, i32* %i17, align 4
  %lt23 = icmp slt i32 %i22, 20
  %gt24 = icmp sgt i32 %i22, 20
  %more25 = select i1 true, i1 %lt23, i1 %gt24
  br i1 %more25, label %for.body19, label %for.end21

for.body19:                                       ; preds = %for.cond18
  %i26 = load i32, i32* %i17, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i32 %i26, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  br label %for.inc20

for.inc20:                                        ; preds = %for.body19
  %i27 = load i32, i32* %i17, align 4
  %next28 = add i32 %i27, %step
  store i32 %next28, i32* %i17, align 4
  br label %for.cond18

for.end21:                                        ; preds = %for.cond18
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %i29 = alloca i32, align 4
  store i32 0, i32* %i29, align 4
  br label %for.cond30

for.cond30:                                       ; preds = %for.inc32, %for.end21
  %i34 = load i32, i32* %i29, align 4
  %lt35 = icmp slt i32 %i34, 3
  %gt36 = icmp sgt i32 %i34, 3
  %more37 = select i1 true, i1 %lt35, i1 %gt36
  br i1 %more37, label %for.body31, label %for.end33

for.body31:                                       ; preds = %for.cond30
  %i38 = load i32, i32* %i29, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i32 %i38, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.11, i32 0, i32 0))
  br label %for.inc32

for.inc32:                                        ; preds = %for.body31
  %i39 = load i32, i32* %i29, align 4
  %next40 = add i32 %i39, 10
  store i32 %next40, i32* %i29, align 4
  br label %for.cond30

for.end33:                                        ; preds = %for.cond30
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 012 — a `by` step must be a positive integer on a range

```ura
// loops/012.ura - by rejects a non-positive step

main():
    for i in 0..10 by 0:
        output(i)
```

```tree
```

```out
```

```err
error: The 'by' step must be positive; a range counts down when its start is greater than its end
  012.ura:4:23
  |
4 |     for i in 0..10 by 0:
  |                       ^
```

```ll
```

## 013 — `by` needs a range on its left

```ura
// loops/013.ura - by needs a range, not an array

main():
    xs int[] = [1, 2, 3]
    for x in xs by 2:
        output(x)
```

```tree
```

```out
```

```err
error: 'by' sets the step of a range (a..b), so it needs one on its left
  013.ura:5:5
  |
5 |     for x in xs by 2:
  |     ^^^
```

```ll
```
