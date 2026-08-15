# heap

## index

- 001 — `new T` allocates one zeroed struct and binds it to a ref
- 002 — `clean` frees a heap struct and nulls the reference
- 003 — `clean` on an unbound reference is a no-op
- 004 — cleaning twice is safe
- 005 — a guarded read after `clean` traps
- 006 — `operator drop` cascades, freeing a whole tree
- 007 — a function returns `new T` from a `ref?` return
- 008 — `clean` on a value struct is still an error
- 009 — `new` without an array type is still an error

## 001 — `new T` allocates one zeroed struct and binds it to a ref

```ura
// heap/001 — new T allocates one zeroed struct and binds it to a ref
struct P:
    a i32
    b i32

main():
    ref? p P = new P
    output(p.a, " ", p.b, "\n")
    p.a = 7
    output(p.a, "\n")
    clean p
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

struct P
├─ a : i32
└─ b : i32

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ new P
├─ output : void
│  ├─ .a : i32
│  │  └─ p : STRUCT_CALL
│  ├─ char[] " "
│  ├─ .b : i32
│  │  └─ p : STRUCT_CALL
│  └─ char[] "\n"
├─ = : i32
│  ├─ .a : i32
│  │  └─ p : STRUCT_CALL
│  └─ int 7
├─ output : void
│  ├─ .a : i32
│  │  └─ p : STRUCT_CALL
│  └─ char[] "\n"
└─ clean : void
   └─ p : STRUCT_CALL
```

```out
0 0
7
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%P = type { i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

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
  %p = alloca %P*, align 8
  %heap = call i8* @calloc(i64 1, i64 8)
  %arr = bitcast i8* %heap to %P*
  store %P* %arr, %P** %p, align 8
  %ref = load %P*, %P** %p, align 8
  %a = getelementptr %P, %P* %ref, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  %ref2 = load %P*, %P** %p, align 8
  %b = getelementptr %P, %P* %ref2, i32 0, i32 1
  %b3 = load i32, i32* %b, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %a1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %b3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %ref4 = load %P*, %P** %p, align 8
  %a5 = getelementptr %P, %P* %ref4, i32 0, i32 0
  store i32 7, i32* %a5, align 4
  %ref6 = load %P*, %P** %p, align 8
  %a7 = getelementptr %P, %P* %ref6, i32 0, i32 0
  %a8 = load i32, i32* %a7, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.3, i32 0, i32 0), i32 %a8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %ref9 = load %P*, %P** %p, align 8
  %isnull = icmp eq %P* %ref9, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  %free.ptr = bitcast %P* %ref9 to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %P* null, %P** %p, align 8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```

## 002 — `clean` frees a heap struct and nulls the reference

```ura
// heap/002 — clean frees a heap struct and nulls the reference
struct P:
    a i32

main():
    ref? p P = new P
    p.a = 3
    output(p.a, "\n")
    clean p
    output(p == null, "\n")
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

struct P
└─ a : i32

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ new P
├─ = : i32
│  ├─ .a : i32
│  │  └─ p : STRUCT_CALL
│  └─ int 3
├─ output : void
│  ├─ .a : i32
│  │  └─ p : STRUCT_CALL
│  └─ char[] "\n"
├─ clean : void
│  └─ p : STRUCT_CALL
└─ output : void
   ├─ == : bool
   │  ├─ p : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   └─ char[] "\n"
```

```out
3
True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%P = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1

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
  %p = alloca %P*, align 8
  %heap = call i8* @calloc(i64 1, i64 4)
  %arr = bitcast i8* %heap to %P*
  store %P* %arr, %P** %p, align 8
  %ref = load %P*, %P** %p, align 8
  %a = getelementptr %P, %P* %ref, i32 0, i32 0
  store i32 3, i32* %a, align 4
  %ref1 = load %P*, %P** %p, align 8
  %a2 = getelementptr %P, %P* %ref1, i32 0, i32 0
  %a3 = load i32, i32* %a2, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %a3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref4 = load %P*, %P** %p, align 8
  %isnull = icmp eq %P* %ref4, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  %free.ptr = bitcast %P* %ref4 to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %P* null, %P** %p, align 8
  %ref5 = load %P*, %P** %p, align 8
  %refcmp = icmp eq %P* %ref5, null
  %bool_str = select i1 %refcmp, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```

## 003 — `clean` on an unbound reference is a no-op

```ura
// heap/003 — clean on an unbound reference is a no-op
struct P:
    a i32

main():
    ref? p P
    clean p
    output("ok\n")
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

struct P
└─ a : i32

fn main() : i32
├─ p : STRUCT_CALL
├─ clean : void
│  └─ p : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%P = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
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
  %p = alloca %P*, align 8
  store %P* null, %P** %p, align 8
  %ref = load %P*, %P** %p, align 8
  %isnull = icmp eq %P* %ref, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  %free.ptr = bitcast %P* %ref to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %P* null, %P** %p, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare void @free(i8*)

declare i32 @printf(i8*, ...)
```

## 004 — cleaning twice is safe

```ura
// heap/004 — cleaning twice is safe
struct P:
    a i32

main():
    ref? p P = new P
    clean p
    clean p
    output("ok\n")
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

struct P
└─ a : i32

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ new P
├─ clean : void
│  └─ p : STRUCT_CALL
├─ clean : void
│  └─ p : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%P = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
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
  %p = alloca %P*, align 8
  %heap = call i8* @calloc(i64 1, i64 4)
  %arr = bitcast i8* %heap to %P*
  store %P* %arr, %P** %p, align 8
  %ref = load %P*, %P** %p, align 8
  %isnull = icmp eq %P* %ref, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  %free.ptr = bitcast %P* %ref to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %P* null, %P** %p, align 8
  %ref1 = load %P*, %P** %p, align 8
  %isnull4 = icmp eq %P* %ref1, null
  br i1 %isnull4, label %clean.done3, label %clean.live2

clean.live2:                                      ; preds = %clean.done
  %free.ptr5 = bitcast %P* %ref1 to i8*
  call void @free(i8* %free.ptr5)
  br label %clean.done3

clean.done3:                                      ; preds = %clean.live2, %clean.done
  store %P* null, %P** %p, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i32 @printf(i8*, ...)
```

## 005 — a guarded read after `clean` traps

```ura
// heap/005 — a guarded read after clean traps
struct P:
    a i32

main():
    ref? p P = new P
    clean p
    output(p?.a, "\n")
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

struct P
└─ a : i32

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ new P
├─ clean : void
│  └─ p : STRUCT_CALL
└─ output : void
   ├─ .a : i32
   │  └─ p : STRUCT_CALL
   └─ char[] "\n"
```

```out
```

```err
runtime error: reference 'p' used before it was bound - assign 'p = ref <target>' first
  005.ura:8:12
  |
8 |     output(p?.a, "\n")
  |            ^
exit: 1
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%P = type { i32 }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [213 x i8] c"runtime error: reference 'p' used before it was bound - assign 'p = ref <target>' first\0A  005.ura:8:12\0A  |\0A8 |     output(p?.a, \22\\n\22)\0A  |            ^\0A\00", align 1
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

define i32 @main(i32 %0, i8** %1) {
entry:
  %p = alloca %P*, align 8
  %heap = call i8* @calloc(i64 1, i64 4)
  %arr = bitcast i8* %heap to %P*
  store %P* %arr, %P** %p, align 8
  %ref = load %P*, %P** %p, align 8
  %isnull = icmp eq %P* %ref, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  %free.ptr = bitcast %P* %ref to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %P* null, %P** %p, align 8
  %ref1 = load %P*, %P** %p, align 8
  %unbound = icmp eq %P* %ref1, null
  br i1 %unbound, label %trap, label %cont

trap:                                             ; preds = %clean.done
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([213 x i8], [213 x i8]* @trap_msg, i32 0, i32 0), i64 212)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %clean.done
  %a = getelementptr %P, %P* %ref1, i32 0, i32 0
  %a2 = load i32, i32* %a, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %a2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare void @free(i8*)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 006 — `operator drop` cascades, freeing a whole tree

```ura
// heap/006 — operator drop cascades, freeing a whole tree
struct Node:
    value i32
    ref? left Node
    ref? right Node

    operator drop:
        clean self.left
        clean self.right

main():
    ref? root Node = new Node
    root.value = 1
    ref? l Node = new Node
    l.value = 2
    ref? r Node = new Node
    r.value = 3
    root.left = ref l
    root.right = ref r
    output(root.value, " ", root.left.value, " ", root.right.value, "\n")
    clean root
    output("freed\n")
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

struct Node
├─ value : i32
├─ left : STRUCT_CALL
├─ right : STRUCT_CALL
└─ fn Node.drop(self : STRUCT_CALL) : void
   ├─ clean : void
   │  └─ .left : STRUCT_CALL
   │     └─ self : STRUCT_CALL
   └─ clean : void
      └─ .right : STRUCT_CALL
         └─ self : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ root : STRUCT_CALL
│  └─ new Node
├─ = : i32
│  ├─ .value : i32
│  │  └─ root : STRUCT_CALL
│  └─ int 1
├─ = : STRUCT_CALL
│  ├─ l : STRUCT_CALL
│  └─ new Node
├─ = : i32
│  ├─ .value : i32
│  │  └─ l : STRUCT_CALL
│  └─ int 2
├─ = : STRUCT_CALL
│  ├─ r : STRUCT_CALL
│  └─ new Node
├─ = : i32
│  ├─ .value : i32
│  │  └─ r : STRUCT_CALL
│  └─ int 3
├─ = : STRUCT_CALL
│  ├─ .left : STRUCT_CALL
│  │  └─ root : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ l : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ .right : STRUCT_CALL
│  │  └─ root : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ r : STRUCT_CALL
├─ output : void
│  ├─ .value : i32
│  │  └─ root : STRUCT_CALL
│  ├─ char[] " "
│  ├─ .value : i32
│  │  └─ .left : STRUCT_CALL
│  │     └─ root : STRUCT_CALL
│  ├─ char[] " "
│  ├─ .value : i32
│  │  └─ .right : STRUCT_CALL
│  │     └─ root : STRUCT_CALL
│  └─ char[] "\n"
├─ clean : void
│  └─ root : STRUCT_CALL
└─ output : void
   └─ char[] "freed\n"
```

```out
1 2 3
freed
```

```err
warning: Struct Node has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
  006.ura:2:1
  |
2 | struct Node:
  | ^^^^^^
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node*, %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [19 x i8] c"%d%.*s%d%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"freed\0A\00", align 1
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

define void @Node.drop(%Node* %0) {
entry:
  %self = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  %ref = load %Node*, %Node** %self, align 8
  %left = getelementptr %Node, %Node* %ref, i32 0, i32 1
  %ref1 = load %Node*, %Node** %left, align 8
  %ref2 = load %Node*, %Node** %self, align 8
  %left3 = getelementptr %Node, %Node* %ref2, i32 0, i32 1
  %isnull = icmp eq %Node* %ref1, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  call void @Node.drop(%Node* %ref1)
  %free.ptr = bitcast %Node* %ref1 to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %Node* null, %Node** %left3, align 8
  %ref4 = load %Node*, %Node** %self, align 8
  %right = getelementptr %Node, %Node* %ref4, i32 0, i32 2
  %ref5 = load %Node*, %Node** %right, align 8
  %ref6 = load %Node*, %Node** %self, align 8
  %right7 = getelementptr %Node, %Node* %ref6, i32 0, i32 2
  %isnull10 = icmp eq %Node* %ref5, null
  br i1 %isnull10, label %clean.done9, label %clean.live8

clean.live8:                                      ; preds = %clean.done
  call void @Node.drop(%Node* %ref5)
  %free.ptr11 = bitcast %Node* %ref5 to i8*
  call void @free(i8* %free.ptr11)
  br label %clean.done9

clean.done9:                                      ; preds = %clean.live8, %clean.done
  store %Node* null, %Node** %right7, align 8
  ret void
}

declare void @free(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %root = alloca %Node*, align 8
  %heap = call i8* @calloc(i64 1, i64 24)
  %arr = bitcast i8* %heap to %Node*
  store %Node* %arr, %Node** %root, align 8
  %ref = load %Node*, %Node** %root, align 8
  %value = getelementptr %Node, %Node* %ref, i32 0, i32 0
  store i32 1, i32* %value, align 4
  %l = alloca %Node*, align 8
  %heap1 = call i8* @calloc(i64 1, i64 24)
  %arr2 = bitcast i8* %heap1 to %Node*
  store %Node* %arr2, %Node** %l, align 8
  %ref3 = load %Node*, %Node** %l, align 8
  %value4 = getelementptr %Node, %Node* %ref3, i32 0, i32 0
  store i32 2, i32* %value4, align 4
  %r = alloca %Node*, align 8
  %heap5 = call i8* @calloc(i64 1, i64 24)
  %arr6 = bitcast i8* %heap5 to %Node*
  store %Node* %arr6, %Node** %r, align 8
  %ref7 = load %Node*, %Node** %r, align 8
  %value8 = getelementptr %Node, %Node* %ref7, i32 0, i32 0
  store i32 3, i32* %value8, align 4
  %ref9 = load %Node*, %Node** %l, align 8
  %ref10 = load %Node*, %Node** %root, align 8
  %left = getelementptr %Node, %Node* %ref10, i32 0, i32 1
  store %Node* %ref9, %Node** %left, align 8
  %ref11 = load %Node*, %Node** %r, align 8
  %ref12 = load %Node*, %Node** %root, align 8
  %right = getelementptr %Node, %Node* %ref12, i32 0, i32 2
  store %Node* %ref11, %Node** %right, align 8
  %ref13 = load %Node*, %Node** %root, align 8
  %value14 = getelementptr %Node, %Node* %ref13, i32 0, i32 0
  %value15 = load i32, i32* %value14, align 4
  %ref16 = load %Node*, %Node** %root, align 8
  %left17 = getelementptr %Node, %Node* %ref16, i32 0, i32 1
  %ref18 = load %Node*, %Node** %left17, align 8
  %value19 = getelementptr %Node, %Node* %ref18, i32 0, i32 0
  %value20 = load i32, i32* %value19, align 4
  %ref21 = load %Node*, %Node** %root, align 8
  %right22 = getelementptr %Node, %Node* %ref21, i32 0, i32 2
  %ref23 = load %Node*, %Node** %right22, align 8
  %value24 = getelementptr %Node, %Node* %ref23, i32 0, i32 0
  %value25 = load i32, i32* %value24, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt, i32 0, i32 0), i32 %value15, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %value20, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %value25, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %ref26 = load %Node*, %Node** %root, align 8
  %isnull = icmp eq %Node* %ref26, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  call void @Node.drop(%Node* %ref26)
  %free.ptr = bitcast %Node* %ref26 to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %Node* null, %Node** %root, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)
```

## 007 — a function returns `new T` from a `ref?` return

```ura
// heap/007 — a function returns new T from a ref? return
struct P:
    v i32

fn make(x i32) ref? P:
    ref? p P = new P
    p.v = x
    ret ref p

main():
    ref? q P = make(9)
    output(q.v, "\n")
    clean q
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

struct P
└─ v : i32

fn make(x : i32) : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ new P
├─ = : i32
│  ├─ .v : i32
│  │  └─ p : STRUCT_CALL
│  └─ x : i32
└─ return
   └─ ref : STRUCT_CALL
      └─ p : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ q : STRUCT_CALL
│  └─ call make : STRUCT_CALL
│     └─ int 9
├─ output : void
│  ├─ .v : i32
│  │  └─ q : STRUCT_CALL
│  └─ char[] "\n"
└─ clean : void
   └─ q : STRUCT_CALL
```

```out
9
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%P = type { i32 }

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

define %P* @make(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %p = alloca %P*, align 8
  %heap = call i8* @calloc(i64 1, i64 4)
  %arr = bitcast i8* %heap to %P*
  store %P* %arr, %P** %p, align 8
  %ref = load %P*, %P** %p, align 8
  %v = getelementptr %P, %P* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  store i32 %x1, i32* %v, align 4
  %ref2 = load %P*, %P** %p, align 8
  ret %P* %ref2
}

declare i8* @calloc(i64, i64)

define i32 @main(i32 %0, i8** %1) {
entry:
  %q = alloca %P*, align 8
  %call = call %P* @make(i32 9)
  store %P* %call, %P** %q, align 8
  %ref = load %P*, %P** %q, align 8
  %v = getelementptr %P, %P* %ref, i32 0, i32 0
  %v1 = load i32, i32* %v, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %v1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load %P*, %P** %q, align 8
  %isnull = icmp eq %P* %ref2, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  %free.ptr = bitcast %P* %ref2 to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %P* null, %P** %q, align 8
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```

## 008 — `clean` on a value struct is still an error

```ura
// heap/008 — clean on a value struct is still an error
struct P:
    v i32

main():
    p P
    clean p
```

```tree
```

```out
```

```err
error: 'clean' frees a heap array, not the struct P; a struct is destroyed at end of scope by its 'operator drop'
  008.ura:7:5
  |
7 |     clean p
  |     ^^^^^
```

```ll
```

## 009 — `new` without an array type is still an error

```ura
// heap/009 — new without an array type is still an error
main():
    a i32[] = new i32
```

```tree
```

```out
```

```err
error: Expected an array type after 'new' (e.g. new int[n])
  009.ura:3:15
  |
3 |     a i32[] = new i32
  |               ^^^
```

```ll
```
