# match

## index

- 001 — a single-value case matches
- 002 — a case with several values
- 003 — default runs when nothing matches
- 004 — break leaves the match early
- 005 — break exits the match, not the enclosing loop
- 006 — continue applies to the loop, skipping the rest of the iteration
- 007 — a case needs at least one value
- 008 — case value type must match the subject
- 009 — a case with no enclosing match
- 010 — a default with no enclosing match
- 011 — break needs a loop or match

## 001 — a single-value case matches

```ura
// match/001 — a single-value case matches
main():
    a int = 1
    match a:
        case 1:
            output("one\n")
        default:
            output("other\n")
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
│  └─ int 1
└─ match
   ├─ subject a : i32
   ├─ case
   │  ├─ value int 1
   │  └─ output : void
   │     └─ chars "one\n"
   └─ default
      └─ output : void
         └─ chars "other\n"
```

```out
one
```

```err
```

```ll

@str = private unnamed_addr constant [5 x i8] c"one\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"other\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a1, 1
  br i1 %eq, label %case.body, label %case.next

match.end:                                        ; preds = %case.next, %case.body
  ret i32 0

case.body:                                        ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  br label %match.end

case.next:                                        ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0))
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 002 — a case with several values

```ura
// match/002 — a case with several values
main():
    a int = 3
    match a:
        case 1, 2, 3:
            output("small\n")
        default:
            output("big\n")
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
│  └─ int 3
└─ match
   ├─ subject a : i32
   ├─ case
   │  ├─ value int 1
   │  ├─ value int 2
   │  ├─ value int 3
   │  └─ output : void
   │     └─ chars "small\n"
   └─ default
      └─ output : void
         └─ chars "big\n"
```

```out
small
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"small\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"big\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 3, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a1, 1
  %eq2 = icmp eq i32 %a1, 2
  %case.or = or i1 %eq, %eq2
  %eq3 = icmp eq i32 %a1, 3
  %case.or4 = or i1 %case.or, %eq3
  br i1 %case.or4, label %case.body, label %case.next

match.end:                                        ; preds = %case.next, %case.body
  ret i32 0

case.body:                                        ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0))
  br label %match.end

case.next:                                        ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0))
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 003 — default runs when nothing matches

```ura
// match/003 — default runs when nothing matches
main():
    a int = 99
    match a:
        case 1:
            output("one\n")
        default:
            output("fallback\n")
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
│  └─ int 99
└─ match
   ├─ subject a : i32
   ├─ case
   │  ├─ value int 1
   │  └─ output : void
   │     └─ chars "one\n"
   └─ default
      └─ output : void
         └─ chars "fallback\n"
```

```out
fallback
```

```err
```

```ll

@str = private unnamed_addr constant [5 x i8] c"one\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"fallback\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 99, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a1, 1
  br i1 %eq, label %case.body, label %case.next

match.end:                                        ; preds = %case.next, %case.body
  ret i32 0

case.body:                                        ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  br label %match.end

case.next:                                        ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0))
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 004 — break leaves the match early

```ura
// match/004 — break leaves the match early
main():
    a int = 1
    match a:
        case 1:
            output("before\n")
            break
            output("after\n")
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
│  └─ int 1
└─ match
   ├─ subject a : i32
   └─ case
      ├─ value int 1
      ├─ output : void
      │  └─ chars "before\n"
      ├─ break
      └─ output : void
         └─ chars "after\n"
```

```out
before
```

```err
```

```ll

@str = private unnamed_addr constant [8 x i8] c"before\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a1, 1
  br i1 %eq, label %case.body, label %match.end

match.end:                                        ; preds = %case.body, %entry
  ret i32 0

case.body:                                        ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0))
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 005 — break exits the match, not the enclosing loop

```ura
// match/005 — break exits the match, not the enclosing loop
main():
    i int = 0
    while i < 3:
        i = i + 1
        match i:
            case 2:
                break
        output(i, "\n")
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
└─ while
   ├─ condition < : bool
   │  ├─ i : i32
   │  └─ int 3
   ├─ = : i32
   │  ├─ i : i32
   │  └─ + : i32
   │     ├─ i : i32
   │     └─ int 1
   ├─ match
   │  ├─ subject i : i32
   │  └─ case
   │     ├─ value int 2
   │     └─ break
   └─ output : void
      ├─ i : i32
      └─ chars "\n"
```

```out
1
2
3
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %match.end, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 3
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %i2 = load i32, i32* %i, align 4
  %add = add i32 %i2, 1
  store i32 %add, i32* %i, align 4
  %i3 = load i32, i32* %i, align 4
  %eq = icmp eq i32 %i3, 2
  br i1 %eq, label %case.body, label %match.end

while.end:                                        ; preds = %while.cond
  ret i32 0

match.end:                                        ; preds = %case.body, %while.body
  %i4 = load i32, i32* %i, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %i4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %while.cond

case.body:                                        ; preds = %while.body
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 006 — continue applies to the loop, skipping the rest of the iteration

```ura
// match/006 — continue applies to the loop, skipping the rest of the iteration
main():
    i int = 0
    while i < 3:
        i = i + 1
        match i:
            case 2:
                continue
        output(i, "\n")
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
└─ while
   ├─ condition < : bool
   │  ├─ i : i32
   │  └─ int 3
   ├─ = : i32
   │  ├─ i : i32
   │  └─ + : i32
   │     ├─ i : i32
   │     └─ int 1
   ├─ match
   │  ├─ subject i : i32
   │  └─ case
   │     ├─ value int 2
   │     └─ continue
   └─ output : void
      ├─ i : i32
      └─ chars "\n"
```

```out
1
3
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %match.end, %case.body, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 3
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %i2 = load i32, i32* %i, align 4
  %add = add i32 %i2, 1
  store i32 %add, i32* %i, align 4
  %i3 = load i32, i32* %i, align 4
  %eq = icmp eq i32 %i3, 2
  br i1 %eq, label %case.body, label %match.end

while.end:                                        ; preds = %while.cond
  ret i32 0

match.end:                                        ; preds = %while.body
  %i4 = load i32, i32* %i, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %i4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %while.cond

case.body:                                        ; preds = %while.body
  br label %while.cond
}

declare i32 @printf(i8*, ...)
```

## 007 — a case needs at least one value

```ura
// errors/match/001 — a case needs at least one value
main():
    a int = 1
    match a:
        case:
            output("x\n")
```

```tree
```

```out
```

```err
error: Expected an expression after 'case'
  007.ura:5:9
  |
5 |         case:
  |         ^^^^
```

```ll
```

## 008 — case value type must match the subject

```ura
// errors/match/002 — case value type must match the subject
main():
    a int = 1
    match a:
        case "x":
            output("x\n")
```

```tree
```

```out
```

```err
error: This case value is chars but the subject is i32; they must be the same type
  008.ura:5:15
  |
5 |         case "x":
  |               ^
```

```ll
```

## 009 — a case with no enclosing match

```ura
// errors/match/003 — a case with no enclosing match
main():
    case 1:
        output("x\n")
```

```tree
```

```out
```

```err
error: 'case' without a matching 'match'
  009.ura:3:5
  |
3 |     case 1:
  |     ^^^^
```

```ll
```

## 010 — a default with no enclosing match

```ura
// errors/match/004 — a default with no enclosing match
main():
    default:
        output("x\n")
```

```tree
```

```out
```

```err
error: 'default' without a matching 'match'
  010.ura:3:5
  |
3 |     default:
  |     ^^^^^^^
```

```ll
```

## 011 — break needs a loop or match

```ura
// errors/match/005 — break needs a loop or match
main():
    break
```

```tree
```

```out
```

```err
error: 'break' outside a loop or match
  011.ura:3:5
  |
3 |     break
  |     ^^^^^
```

```ll
```
