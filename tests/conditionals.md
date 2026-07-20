# conditionals

## index

- 001 — basic if: first floor greeting
- 002 — if-else: alive or dead check
- 003 — if-elif-else: floor difficulty rating

## 001 — basic if: first floor greeting

```ura
// conditionals/001.ura - basic if: first floor greeting

main():
    floor i32 = 1
    if floor == 1:
        output("Fresh start\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

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
│  ├─ floor : i32
│  └─ int 1
└─ if
   ├─ condition == : bool
   │  ├─ floor : i32
   │  └─ int 1
   └─ output : void
      └─ char[] "Fresh start\n"
```

```out
Fresh start
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [13 x i8] c"Fresh start\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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
  %floor = alloca i32, align 4
  store i32 1, i32* %floor, align 4
  %floor1 = load i32, i32* %floor, align 4
  %eq = icmp eq i32 %floor1, 1
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %then, %entry
  ret i32 0

then:                                             ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 002 — if-else: alive or dead check

```ura
// conditionals/002.ura - if-else: alive or dead check

main():
    hp i32 = 0
    if hp > 0:
        output("Hero is alive\n")
    else:
        output("Hero has fallen\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

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
│  ├─ hp : i32
│  └─ int 0
└─ if
   ├─ condition > : bool
   │  ├─ hp : i32
   │  └─ int 0
   ├─ output : void
   │  └─ char[] "Hero is alive\n"
   └─ else
      └─ output : void
         └─ char[] "Hero has fallen\n"
```

```out
Hero has fallen
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [15 x i8] c"Hero is alive\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.1 = private unnamed_addr constant [17 x i8] c"Hero has fallen\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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
  %hp = alloca i32, align 4
  store i32 0, i32* %hp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %gt = icmp sgt i32 %hp1, 0
  br i1 %gt, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 14, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 16, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 003 — if-elif-else: floor difficulty rating

```ura
// conditionals/003.ura - if-elif-else: floor difficulty rating

main():
    floor i32 = 6
    if floor < 3:
        output("Easy\n")
    elif floor < 7:
        output("Normal\n")
    else:
        output("Hard\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

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
│  ├─ floor : i32
│  └─ int 6
└─ if
   ├─ condition < : bool
   │  ├─ floor : i32
   │  └─ int 3
   ├─ output : void
   │  └─ char[] "Easy\n"
   └─ elif
      ├─ condition < : bool
      │  ├─ floor : i32
      │  └─ int 7
      ├─ output : void
      │  └─ char[] "Normal\n"
      └─ else
         └─ output : void
            └─ char[] "Hard\n"
```

```out
Normal
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"Easy\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"Normal\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"Hard\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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
  %floor = alloca i32, align 4
  store i32 6, i32* %floor, align 4
  %floor1 = load i32, i32* %floor, align 4
  %lt = icmp slt i32 %floor1, 3
  br i1 %lt, label %then, label %next

endif:                                            ; preds = %next3, %then2, %then
  ret i32 0

then:                                             ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %floor4 = load i32, i32* %floor, align 4
  %lt5 = icmp slt i32 %floor4, 7
  br i1 %lt5, label %then2, label %next3

then2:                                            ; preds = %next
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0))
  br label %endif

next3:                                            ; preds = %next
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```
