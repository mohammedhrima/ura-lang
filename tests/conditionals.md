# conditionals

## index

- 001 — basic if: first floor greeting
- 002 — if-else: alive or dead check
- 003 — if-elif-else: floor difficulty rating

## 001 — basic if: first floor greeting

```ura
// conditionals/001.ura - basic if: first floor greeting

main():
    floor int = 1
    if floor == 1:
        output("Fresh start\n")
```

```tree
fn main() : int
├─ = : int
│  ├─ floor : int
│  └─ int 1
└─ if
   ├─ condition == : bool
   │  ├─ floor : int
   │  └─ int 1
   └─ output : void
      └─ chars "Fresh start\n"
```

```out
Fresh start
```

```err
```

```ll

@str = private unnamed_addr constant [13 x i8] c"Fresh start\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %floor = alloca i32, align 4
  store i32 1, i32* %floor, align 4
  %floor1 = load i32, i32* %floor, align 4
  %eq = icmp eq i32 %floor1, 1
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %then, %entry
  ret i32 0

then:                                             ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 002 — if-else: alive or dead check

```ura
// conditionals/002.ura - if-else: alive or dead check

main():
    hp int = 0
    if hp > 0:
        output("Hero is alive\n")
    else:
        output("Hero has fallen\n")
```

```tree
fn main() : int
├─ = : int
│  ├─ hp : int
│  └─ int 0
└─ if
   ├─ condition > : bool
   │  ├─ hp : int
   │  └─ int 0
   ├─ output : void
   │  └─ chars "Hero is alive\n"
   └─ else
      └─ output : void
         └─ chars "Hero has fallen\n"
```

```out
Hero has fallen
```

```err
```

```ll

@str = private unnamed_addr constant [15 x i8] c"Hero is alive\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [17 x i8] c"Hero has fallen\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 0, i32* %hp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %gt = icmp sgt i32 %hp1, 0
  br i1 %gt, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 003 — if-elif-else: floor difficulty rating

```ura
// conditionals/003.ura - if-elif-else: floor difficulty rating

main():
    floor int = 6
    if floor < 3:
        output("Easy\n")
    elif floor < 7:
        output("Normal\n")
    else:
        output("Hard\n")
```

```tree
fn main() : int
├─ = : int
│  ├─ floor : int
│  └─ int 6
└─ if
   ├─ condition < : bool
   │  ├─ floor : int
   │  └─ int 3
   ├─ output : void
   │  └─ chars "Easy\n"
   └─ elif
      ├─ condition < : bool
      │  ├─ floor : int
      │  └─ int 7
      ├─ output : void
      │  └─ chars "Normal\n"
      └─ else
         └─ output : void
            └─ chars "Hard\n"
```

```out
Normal
```

```err
```

```ll

@str = private unnamed_addr constant [6 x i8] c"Easy\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"Normal\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"Hard\0A\00", align 1
@fmt.4 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %floor = alloca i32, align 4
  store i32 6, i32* %floor, align 4
  %floor1 = load i32, i32* %floor, align 4
  %lt = icmp slt i32 %floor1, 3
  br i1 %lt, label %then, label %next

endif:                                            ; preds = %next3, %then2, %then
  ret i32 0

then:                                             ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %floor4 = load i32, i32* %floor, align 4
  %lt5 = icmp slt i32 %floor4, 7
  br i1 %lt5, label %then2, label %next3

then2:                                            ; preds = %next
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0))
  br label %endif

next3:                                            ; preds = %next
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```
