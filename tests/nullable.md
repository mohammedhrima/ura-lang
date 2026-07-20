# nullable

## index

- 001 — a nullable return, a null comparison and a ?? default
- 002 — a non-null value widens into an optional
- 003 — an optional cannot be assigned to a non-optional
- 004 — ?? chains right to left until a non-null default
- 005 — a ?? chain must end in a non-null value
- 006 — ?? on a value that is never null is rejected
- 007 — comparing a never-null value to null is rejected
- 008 — null needs a target type

## 001 — a nullable return, a null comparison and a ?? default

```ura
// nullable/001.ura - a nullable return, a null comparison and a ?? default

fn find_weapon(chest i32) char[]?:
    if chest == 1: return "Iron Sword"
    if chest == 2: return "Magic Staff"
    return null

main():
    loot char[]? = find_weapon(1)
    if loot != null:
        output("Found: ", loot?, "\n")
    else:
        output("Chest is empty\n")
    loot = find_weapon(99)
    if loot != null:
        output("Found: ", loot?, "\n")
    else:
        output("Chest is empty\n")
    output("default: ", loot ?? "nothing", "\n")
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

fn find_weapon(chest : i32) : char[]
├─ if
│  ├─ condition == : bool
│  │  ├─ chest : i32
│  │  └─ int 1
│  └─ return
│     └─ char[] "Iron Sword"
├─ if
│  ├─ condition == : bool
│  │  ├─ chest : i32
│  │  └─ int 2
│  └─ return
│     └─ char[] "Magic Staff"
└─ return
   └─ NULL_LIT : char[]

fn main() : i32
├─ = : array
│  ├─ loot : char[]
│  └─ call find_weapon : char[]
│     └─ int 1
├─ if
│  ├─ condition != : bool
│  │  ├─ loot : char[]
│  │  └─ NULL_LIT : char[]
│  ├─ output : void
│  │  ├─ char[] "Found: "
│  │  ├─ loot : char[]
│  │  └─ char[] "\n"
│  └─ else
│     └─ output : void
│        └─ char[] "Chest is empty\n"
├─ = : array
│  ├─ loot : char[]
│  └─ call find_weapon : char[]
│     └─ int 99
├─ if
│  ├─ condition != : bool
│  │  ├─ loot : char[]
│  │  └─ NULL_LIT : char[]
│  ├─ output : void
│  │  ├─ char[] "Found: "
│  │  ├─ loot : char[]
│  │  └─ char[] "\n"
│  └─ else
│     └─ output : void
│        └─ char[] "Chest is empty\n"
└─ output : void
   ├─ char[] "default: "
   ├─ FALLBACK : char[]
   │  ├─ loot : char[]
   │  └─ char[] "nothing"
   └─ char[] "\n"
```

```out
Found: Iron Sword
Chest is empty
default: nothing
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [11 x i8] c"Iron Sword\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"Magic Staff\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"Found: \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.4 = private unnamed_addr constant [16 x i8] c"Chest is empty\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.6 = private unnamed_addr constant [8 x i8] c"Found: \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.9 = private unnamed_addr constant [16 x i8] c"Chest is empty\0A\00", align 1
@fmt.10 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.11 = private unnamed_addr constant [10 x i8] c"default: \00", align 1
@str.12 = private unnamed_addr constant [8 x i8] c"nothing\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1

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

define { i8*, i64 } @find_weapon(i32 %0) {
entry:
  %chest = alloca i32, align 4
  store i32 %0, i32* %chest, align 4
  %chest1 = load i32, i32* %chest, align 4
  %eq = icmp eq i32 %chest1, 1
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %chest4 = load i32, i32* %chest, align 4
  %eq5 = icmp eq i32 %chest4, 2
  br i1 %eq5, label %then3, label %endif2

then:                                             ; preds = %entry
  ret { i8*, i64 } { i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i64 10 }

endif2:                                           ; preds = %endif
  ret { i8*, i64 } zeroinitializer

then3:                                            ; preds = %endif
  ret { i8*, i64 } { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), i64 11 }
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %loot = alloca { i8*, i64 }, align 8
  %call = call { i8*, i64 } @find_weapon(i32 1)
  store { i8*, i64 } %call, { i8*, i64 }* %loot, align 8
  %loot1 = load { i8*, i64 }, { i8*, i64 }* %loot, align 8
  %opt.ptr = extractvalue { i8*, i64 } %loot1, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %next

endif:                                            ; preds = %next, %then
  %call3 = call { i8*, i64 } @find_weapon(i32 99)
  store { i8*, i64 } %call3, { i8*, i64 }* %loot, align 8
  %loot7 = load { i8*, i64 }, { i8*, i64 }* %loot, align 8
  %opt.ptr8 = extractvalue { i8*, i64 } %loot7, 0
  %nullcmp9 = icmp ne i8* %opt.ptr8, null
  br i1 %nullcmp9, label %then5, label %next6

then:                                             ; preds = %entry
  %loot2 = load { i8*, i64 }, { i8*, i64 }* %loot, align 8
  %str.len = extractvalue { i8*, i64 } %loot2, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %loot2, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 15, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.4, i32 0, i32 0))
  br label %endif

endif4:                                           ; preds = %next6, %then5
  %loot14 = load { i8*, i64 }, { i8*, i64 }* %loot, align 8
  %opt.ptr15 = extractvalue { i8*, i64 } %loot14, 0
  %isnull = icmp eq i8* %opt.ptr15, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.12, i32 0, i32 0), i64 7 }, { i8*, i64 } %loot14
  %str.len16 = extractvalue { i8*, i64 } %fallback, 1
  %len3217 = trunc i64 %str.len16 to i32
  %str.data18 = extractvalue { i8*, i64 } %fallback, 0
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.14, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.11, i32 0, i32 0), i32 %len3217, i8* %str.data18, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  ret i32 0

then5:                                            ; preds = %endif
  %loot10 = load { i8*, i64 }, { i8*, i64 }* %loot, align 8
  %str.len11 = extractvalue { i8*, i64 } %loot10, 1
  %len3212 = trunc i64 %str.len11 to i32
  %str.data13 = extractvalue { i8*, i64 } %loot10, 0
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.8, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.6, i32 0, i32 0), i32 %len3212, i8* %str.data13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  br label %endif4

next6:                                            ; preds = %endif
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.10, i32 0, i32 0), i32 15, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.9, i32 0, i32 0))
  br label %endif4
}

declare i32 @printf(i8*, ...)
```

## 002 — a non-null value widens into an optional

```ura
// nullable/002.ura - a non-null value widens into an optional

main():
    s1 char[]  = "hello"
    s2 char[]? = null
    s2 = s1
    output(s2 ?? "empty", "\n")
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
├─ = : array
│  ├─ s1 : char[]
│  └─ char[] "hello"
├─ = : array
│  ├─ s2 : char[]
│  └─ NULL_LIT : char[]
├─ = : array
│  ├─ s2 : char[]
│  └─ s1 : char[]
└─ output : void
   ├─ FALLBACK : char[]
   │  ├─ s2 : char[]
   │  └─ char[] "empty"
   └─ char[] "\n"
```

```out
hello
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"empty\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
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
  %s1 = alloca { i8*, i64 }, align 8
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 5 }, { i8*, i64 }* %s1, align 8
  %s2 = alloca { i8*, i64 }, align 8
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %s2, align 8
  %s11 = load { i8*, i64 }, { i8*, i64 }* %s1, align 8
  store { i8*, i64 } %s11, { i8*, i64 }* %s2, align 8
  %s22 = load { i8*, i64 }, { i8*, i64 }* %s2, align 8
  %opt.ptr = extractvalue { i8*, i64 } %s22, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i64 5 }, { i8*, i64 } %s22
  %str.len = extractvalue { i8*, i64 } %fallback, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %fallback, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — an optional cannot be assigned to a non-optional

```ura
// nullable/003.ura - an optional cannot be assigned to a non-optional

main():
    s1 char[]  = "hello"
    s2 char[]? = null
    s1 = s2
```

```tree
```

```out
```

```err
error: 's2' may be null; unwrap it with '?? <default>' or declare 's1' as optional
  003.ura:6:8
  |
6 |     s1 = s2
  |        ^
```

```ll
```

## 004 — ?? chains right to left until a non-null default

```ura
// nullable/004.ura - ?? chains right to left until a non-null default

main():
    s2 char[]? = null
    s3 char[]? = null
    s4 char[]? = "found"
    s1 char[]  = s2 ?? s3 ?? ""
    output("[", s1, "]\n")
    output("[", s2 ?? s3 ?? s4 ?? "deep", "]\n")
    s3 = "third"
    output("[", s2 ?? s3 ?? "", "]\n")
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
├─ = : array
│  ├─ s2 : char[]
│  └─ NULL_LIT : char[]
├─ = : array
│  ├─ s3 : char[]
│  └─ NULL_LIT : char[]
├─ = : array
│  ├─ s4 : char[]
│  └─ char[] "found"
├─ = : array
│  ├─ s1 : char[]
│  └─ FALLBACK : char[]
│     ├─ s2 : char[]
│     └─ FALLBACK : char[]
│        ├─ s3 : char[]
│        └─ char[] ""
├─ output : void
│  ├─ char[] "["
│  ├─ s1 : char[]
│  └─ char[] "]\n"
├─ output : void
│  ├─ char[] "["
│  ├─ FALLBACK : char[]
│  │  ├─ s2 : char[]
│  │  └─ FALLBACK : char[]
│  │     ├─ s3 : char[]
│  │     └─ FALLBACK : char[]
│  │        ├─ s4 : char[]
│  │        └─ char[] "deep"
│  └─ char[] "]\n"
├─ = : array
│  ├─ s3 : char[]
│  └─ char[] "third"
└─ output : void
   ├─ char[] "["
   ├─ FALLBACK : char[]
   │  ├─ s2 : char[]
   │  └─ FALLBACK : char[]
   │     ├─ s3 : char[]
   │     └─ char[] ""
   └─ char[] "]\n"
```

```out
[]
[found]
[third]
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"found\00", align 1
@str.1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"]\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"deep\00", align 1
@str.6 = private unnamed_addr constant [3 x i8] c"]\0A\00", align 1
@fmt.7 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.8 = private unnamed_addr constant [6 x i8] c"third\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@str.10 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.11 = private unnamed_addr constant [3 x i8] c"]\0A\00", align 1
@fmt.12 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1

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
  %s2 = alloca { i8*, i64 }, align 8
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %s2, align 8
  %s3 = alloca { i8*, i64 }, align 8
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %s3, align 8
  %s4 = alloca { i8*, i64 }, align 8
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 5 }, { i8*, i64 }* %s4, align 8
  %s1 = alloca { i8*, i64 }, align 8
  %s21 = load { i8*, i64 }, { i8*, i64 }* %s2, align 8
  %s32 = load { i8*, i64 }, { i8*, i64 }* %s3, align 8
  %opt.ptr = extractvalue { i8*, i64 } %s32, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.1, i32 0, i32 0), i64 0 }, { i8*, i64 } %s32
  %opt.ptr3 = extractvalue { i8*, i64 } %s21, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } %fallback, { i8*, i64 } %s21
  store { i8*, i64 } %fallback5, { i8*, i64 }* %s1, align 8
  %s16 = load { i8*, i64 }, { i8*, i64 }* %s1, align 8
  %str.len = extractvalue { i8*, i64 } %s16, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %s16, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %len32, i8* %str.data, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0))
  %s27 = load { i8*, i64 }, { i8*, i64 }* %s2, align 8
  %s38 = load { i8*, i64 }, { i8*, i64 }* %s3, align 8
  %s49 = load { i8*, i64 }, { i8*, i64 }* %s4, align 8
  %opt.ptr10 = extractvalue { i8*, i64 } %s49, 0
  %isnull11 = icmp eq i8* %opt.ptr10, null
  %fallback12 = select i1 %isnull11, { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i64 4 }, { i8*, i64 } %s49
  %opt.ptr13 = extractvalue { i8*, i64 } %s38, 0
  %isnull14 = icmp eq i8* %opt.ptr13, null
  %fallback15 = select i1 %isnull14, { i8*, i64 } %fallback12, { i8*, i64 } %s38
  %opt.ptr16 = extractvalue { i8*, i64 } %s27, 0
  %isnull17 = icmp eq i8* %opt.ptr16, null
  %fallback18 = select i1 %isnull17, { i8*, i64 } %fallback15, { i8*, i64 } %s27
  %str.len19 = extractvalue { i8*, i64 } %fallback18, 1
  %len3220 = trunc i64 %str.len19 to i32
  %str.data21 = extractvalue { i8*, i64 } %fallback18, 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.7, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 %len3220, i8* %str.data21, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.6, i32 0, i32 0))
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.8, i32 0, i32 0), i64 5 }, { i8*, i64 }* %s3, align 8
  %s222 = load { i8*, i64 }, { i8*, i64 }* %s2, align 8
  %s323 = load { i8*, i64 }, { i8*, i64 }* %s3, align 8
  %opt.ptr24 = extractvalue { i8*, i64 } %s323, 0
  %isnull25 = icmp eq i8* %opt.ptr24, null
  %fallback26 = select i1 %isnull25, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.10, i32 0, i32 0), i64 0 }, { i8*, i64 } %s323
  %opt.ptr27 = extractvalue { i8*, i64 } %s222, 0
  %isnull28 = icmp eq i8* %opt.ptr27, null
  %fallback29 = select i1 %isnull28, { i8*, i64 } %fallback26, { i8*, i64 } %s222
  %str.len30 = extractvalue { i8*, i64 } %fallback29, 1
  %len3231 = trunc i64 %str.len30 to i32
  %str.data32 = extractvalue { i8*, i64 } %fallback29, 0
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.12, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0), i32 %len3231, i8* %str.data32, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.11, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — a ?? chain must end in a non-null value

```ura
// nullable/005.ura - a ?? chain must end in a non-null value

main():
    s2 char[]? = null
    s3 char[]? = null
    s1 char[]  = s2 ?? s3
```

```tree
```

```out
```

```err
error: The right side of '??' may itself be null; it must not be
  005.ura:6:21
  |
6 |     s1 char[]  = s2 ?? s3
  |                     ^^
```

```ll
```

## 006 — ?? on a value that is never null is rejected

```ura
// nullable/006.ura - ?? on a value that is never null is rejected

main():
    s1 char[] = "hello"
    output(s1 ?? "h", "\n")
```

```tree
```

```out
```

```err
error: The left side of '??' is never null, so '??' does nothing
  006.ura:5:15
  |
5 |     output(s1 ?? "h", "\n")
  |               ^^
```

```ll
```

## 007 — comparing a never-null value to null is rejected

```ura
// nullable/007.ura - comparing a never-null value to null is rejected

main():
    s1 char[] = "hello"
    if s1 != null:
        output("never\n")
```

```tree
```

```out
```

```err
error: 's1' is never null, so this comparison is always the same
  007.ura:5:11
  |
5 |     if s1 != null:
  |           ^^
error: The 'if' condition must be a bool, got unknown
  007.ura:5:5
  |
5 |     if s1 != null:
  |     ^^
```

```ll
```

## 008 — null needs a target type

```ura
// nullable/008.ura - null needs a target type

main():
    x char[] = null
```

```tree
```

```out
```

```err
error: 'x' can never be null; declare it optional to assign null to it
  008.ura:4:14
  |
4 |     x char[] = null
  |              ^
```

```ll
```
