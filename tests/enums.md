# enums

## index

- 001 — auto values start at 0 and count up
- 002 — variants on one line, comma-separated
- 003 — an explicit value resets the running counter
- 004 — an enum-typed variable compared to a variant
- 005 — match over an enum inside a function
- 006 — an enum used as a struct field type
- 007 — a variant as an array index and in arithmetic
- 008 — a variant used before the enum is declared
- 009 — an enum must declare at least one variant
- 010 — a variant name cannot be declared twice
- 011 — an explicit variant value must be an integer

## 001 — auto values start at 0 and count up

```ura
enum Color:
    RED,
    GREEN,
    BLUE,

main():
    output(RED, " ", GREEN, " ", BLUE, "\n")
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

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

fn main() : i32
└─ output : void
   ├─ int 0
   ├─ char[] " "
   ├─ int 1
   ├─ char[] " "
   ├─ int 2
   └─ char[] "\n"
```

```out
0 1 2
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [19 x i8] c"%d%.*s%d%.*s%d%.*s\00", align 1

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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt, i32 0, i32 0), i32 0, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — variants on one line, comma-separated

```ura
enum Color: RED, GREEN, BLUE

main():
    output(RED, GREEN, BLUE, "\n")
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

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

fn main() : i32
└─ output : void
   ├─ int 0
   ├─ int 1
   ├─ int 2
   └─ char[] "\n"
```

```out
012
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%d%d%d%.*s\00", align 1

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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 0, i32 1, i32 2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — an explicit value resets the running counter

```ura
enum Dir:
    NORTH = 1,
    EAST,
    SOUTH,
    WEST,

main():
    output(NORTH, EAST, SOUTH, WEST, "\n")
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

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

fn main() : i32
└─ output : void
   ├─ int 1
   ├─ int 2
   ├─ int 3
   ├─ int 4
   └─ char[] "\n"
```

```out
1234
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%d%d%d%.*s\00", align 1

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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 1, i32 2, i32 3, i32 4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — an enum-typed variable compared to a variant

```ura
enum Color: RED, GREEN, BLUE

main():
    c Color = GREEN
    if c == GREEN:
        output("green\n")
    else:
        output("other\n")
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

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

fn main() : i32
├─ = : i32
│  ├─ c : i32
│  └─ int 1
└─ if
   ├─ condition == : bool
   │  ├─ c : i32
   │  └─ int 1
   ├─ output : void
   │  └─ char[] "green\n"
   └─ else
      └─ output : void
         └─ char[] "other\n"
```

```out
green
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"green\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"other\0A\00", align 1
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
  %c = alloca i32, align 4
  store i32 1, i32* %c, align 4
  %c1 = load i32, i32* %c, align 4
  %eq = icmp eq i32 %c1, 1
  br i1 %eq, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 005 — match over an enum inside a function

```ura
enum Signal: LOW, HIGH, FLOAT

fn describe(s Signal) i32:
    match s:
        case LOW:
            return 0
        case HIGH:
            return 1
        default:
            return -1
    return -2

main():
    output(describe(HIGH), " ", describe(FLOAT), "\n")
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

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

fn describe(s : i32) : i32
├─ match
│  ├─ subject s : i32
│  ├─ case
│  │  ├─ value int 0
│  │  └─ return
│  │     └─ int 0
│  ├─ case
│  │  ├─ value int 1
│  │  └─ return
│  │     └─ int 1
│  └─ default
│     └─ return
│        └─ - : i32
│           ├─ int 0
│           └─ int 1
└─ return
   └─ - : i32
      ├─ int 0
      └─ int 2

fn main() : i32
└─ output : void
   ├─ call describe : i32
   │  └─ int 1
   ├─ char[] " "
   ├─ call describe : i32
   │  └─ int 2
   └─ char[] "\n"
```

```out
1 -1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

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

define i32 @describe(i32 %0) {
entry:
  %s = alloca i32, align 4
  store i32 %0, i32* %s, align 4
  %s1 = load i32, i32* %s, align 4
  %eq = icmp eq i32 %s1, 0
  br i1 %eq, label %case.body, label %case.next

match.end:                                        ; No predecessors!
  ret i32 -2

case.body:                                        ; preds = %entry
  ret i32 0

case.next:                                        ; preds = %entry
  %eq4 = icmp eq i32 %s1, 1
  br i1 %eq4, label %case.body2, label %case.next3

case.body2:                                       ; preds = %case.next
  ret i32 1

case.next3:                                       ; preds = %case.next
  ret i32 -1
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @describe(i32 1)
  %call1 = call i32 @describe(i32 2)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %call1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — an enum used as a struct field type

```ura
enum Kind: WARRIOR, MAGE, ROGUE

struct Hero:
    kind Kind
    hp i32

main():
    h Hero
    h.kind = MAGE
    h.hp = 30
    output(h.kind, " ", h.hp, "\n")
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

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

struct Hero
├─ kind : i32
└─ hp : i32

fn main() : i32
├─ h : STRUCT_CALL
├─ = : i32
│  ├─ .kind : i32
│  │  └─ h : STRUCT_CALL
│  └─ int 1
├─ = : i32
│  ├─ .hp : i32
│  │  └─ h : STRUCT_CALL
│  └─ int 30
└─ output : void
   ├─ .kind : i32
   │  └─ h : STRUCT_CALL
   ├─ char[] " "
   ├─ .hp : i32
   │  └─ h : STRUCT_CALL
   └─ char[] "\n"
```

```out
1 30
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Hero = type { i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

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
  %h = alloca %Hero, align 8
  store %Hero zeroinitializer, %Hero* %h, align 4
  %kind = getelementptr %Hero, %Hero* %h, i32 0, i32 0
  store i32 1, i32* %kind, align 4
  %hp = getelementptr %Hero, %Hero* %h, i32 0, i32 1
  store i32 30, i32* %hp, align 4
  %kind1 = getelementptr %Hero, %Hero* %h, i32 0, i32 0
  %kind2 = load i32, i32* %kind1, align 4
  %hp3 = getelementptr %Hero, %Hero* %h, i32 0, i32 1
  %hp4 = load i32, i32* %hp3, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %kind2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %hp4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — a variant as an array index and in arithmetic

```ura
enum Slot: HEAD, BODY, LEGS

main():
    gear i32[] = new i32[3]
    gear[HEAD] = 5
    gear[LEGS] = 9
    output(gear[HEAD] + gear[LEGS], " ", LEGS - HEAD, "\n")
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

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

fn main() : i32
├─ = : array
│  ├─ gear : i32[]
│  └─ array : i32[]
│     └─ int 3
├─ = : i32
│  ├─ index : i32
│  │  ├─ gear : i32[]
│  │  └─ int 0
│  └─ int 5
├─ = : i32
│  ├─ index : i32
│  │  ├─ gear : i32[]
│  │  └─ int 2
│  └─ int 9
└─ output : void
   ├─ + : i32
   │  ├─ index : i32
   │  │  ├─ gear : i32[]
   │  │  └─ int 0
   │  └─ index : i32
   │     ├─ gear : i32[]
   │     └─ int 2
   ├─ char[] " "
   ├─ - : i32
   │  ├─ int 2
   │  └─ int 0
   └─ char[] "\n"
```

```out
14 2
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

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
  %gear = alloca { i32*, i64 }, align 8
  %heap = call i8* @calloc(i64 3, i64 4)
  %arr = bitcast i8* %heap to i32*
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %gear, align 8
  %gear1 = load { i32*, i64 }, { i32*, i64 }* %gear, align 8
  %arr.data = extractvalue { i32*, i64 } %gear1, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 0
  store i32 5, i32* %arr.at, align 4
  %gear2 = load { i32*, i64 }, { i32*, i64 }* %gear, align 8
  %arr.data3 = extractvalue { i32*, i64 } %gear2, 0
  %arr.at4 = getelementptr i32, i32* %arr.data3, i32 2
  store i32 9, i32* %arr.at4, align 4
  %gear5 = load { i32*, i64 }, { i32*, i64 }* %gear, align 8
  %arr.data6 = extractvalue { i32*, i64 } %gear5, 0
  %arr.at7 = getelementptr i32, i32* %arr.data6, i32 0
  %idx = load i32, i32* %arr.at7, align 4
  %gear8 = load { i32*, i64 }, { i32*, i64 }* %gear, align 8
  %arr.data9 = extractvalue { i32*, i64 } %gear8, 0
  %arr.at10 = getelementptr i32, i32* %arr.data9, i32 2
  %idx11 = load i32, i32* %arr.at10, align 4
  %add = add i32 %idx, %idx11
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %add, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)
```

## 008 — a variant used before the enum is declared

```ura
fn first() Color:
    return RED

enum Color: RED, GREEN, BLUE

main():
    output(first(), "\n")
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

fn first() : i32
└─ return
   └─ int 0

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

fn main() : i32
└─ output : void
   ├─ call first : i32
   └─ char[] "\n"
```

```out
0
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

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

define i32 @first() {
entry:
  ret i32 0
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @first()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 009 — an enum must declare at least one variant

```ura
enum Nothing:

main():
    output(0, "\n")
```

```tree
```

```out
```

```err
error: Enum Nothing must declare at least one variant
  009.ura:1:1
  |
1 | enum Nothing:
  | ^^^^
```

```ll
```

## 010 — a variant name cannot be declared twice

```ura
enum C:
    RED, GREEN, RED

main():
    output(0, "\n")
```

```tree
```

```out
```

```err
error: Redeclaration of variable 'RED'
  010.ura:2:17
  |
2 |     RED, GREEN, RED
  |                 ^^^
```

```ll
```

## 011 — an explicit variant value must be an integer

```ura
enum C:
    A = hello

main():
    output(0, "\n")
```

```tree
```

```out
```

```err
error: Enum variant 'A' needs an integer value after '='
  011.ura:2:5
  |
2 |     A = hello
  |     ^
error: Enum C must declare at least one variant
  011.ura:1:1
  |
1 | enum C:
  | ^^^^
```

```ll
```
