# methods

## index

- 001 — a method reads self
- 002 — a method writes self
- 003 — a method reads and writes self
- 004 — a method with arguments
- 005 — a method calls another method
- 006 — a method returns a bool
- 007 — a method in a loop
- 008 — two structs, one method name
- 009 — a method on a nested attribute
- 010 — overloaded method
- 011 — a method on a ref receiver
- 012 — a method through a ref changes the caller

---

## 001 — a method reads self

```ura
// expect: 4
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    u User
    u.a = 4
    return u.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  ret i32 %load.load.dref.self.a
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %load.u.a, align 4
  %"get(&load.u)" = call i32 @User.get(%User* %u)
  ret i32 %"get(&load.u)"
}
```

---

## 002 — a method writes self

```ura
// expect: 9
struct User:
    a i32

    fn set(n i32):
        self.a = n

fn main() i32:
    u User
    u.set(9)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.n = load i32, i32* %n, align 4
  store i32 %load.n, i32* %load.dref.self.a, align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set(%User* %u, i32 9)
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a, align 4
  ret i32 %load.load.u.a
}
```

---

## 003 — a method reads and writes self

```ura
// expect: 7
struct User:
    a i32

    fn bump() i32:
        self.a += 1
        return self.a

fn main() i32:
    u User
    u.a = 5
    u.bump()
    return u.bump()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.bump(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %old = load i32, i32* %load.dref.self.a, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.a, align 4
  %dref.self1 = load %User*, %User** %self, align 8
  %load.dref.self.a2 = getelementptr inbounds %User, %User* %dref.self1, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a2, align 4
  ret i32 %load.load.dref.self.a
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %load.u.a, align 4
  %"bump(&load.u)" = call i32 @User.bump(%User* %u)
  %"bump(&load.u)1" = call i32 @User.bump(%User* %u)
  ret i32 %"bump(&load.u)1"
}
```

---

## 004 — a method with arguments

```ura
// expect: 8
struct User:
    a i32

    fn add(n i32) i32:
        return self.a + n

fn main() i32:
    u User
    u.a = 5
    return u.add(3)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.add(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  %load.n = load i32, i32* %n, align 4
  %ADD = add i32 %load.load.dref.self.a, %load.n
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %load.u.a, align 4
  %"add(&load.u,3)" = call i32 @User.add(%User* %u, i32 3)
  ret i32 %"add(&load.u,3)"
}
```

---

## 005 — a method calls another method

```ura
// expect: 12
struct User:
    a i32

    fn get() i32:
        return self.a

    fn twice() i32:
        return self.get() * 2

fn main() i32:
    u User
    u.a = 6
    return u.twice()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  ret i32 %load.load.dref.self.a
}

define i32 @User.twice(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %"get(&load.dref.self)" = call i32 @User.get(%User* %dref.self)
  %MUL = mul i32 %"get(&load.dref.self)", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 6, i32* %load.u.a, align 4
  %"twice(&load.u)" = call i32 @User.twice(%User* %u)
  ret i32 %"twice(&load.u)"
}
```

---

## 006 — a method returns a bool

```ura
// expect: 1
struct User:
    age i32

    fn adult() bool:
        return self.age >= 18

fn main() i32:
    u User
    u.age = 20
    if u.adult():
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i1 @User.adult(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.age = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.age = load i32, i32* %load.dref.self.age, align 4
  %GE = icmp sge i32 %load.load.dref.self.age, 18
  ret i1 %GE
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.age = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 20, i32* %load.u.age, align 4
  %"adult(&load.u)" = call i1 @User.adult(%User* %u)
  br i1 %"adult(&load.u)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 007 — a method in a loop

```ura
// stdout: 3
// expect: 3
proto printf(fmt i8[], ...) i32

struct Counter:
    n i32

    fn tick():
        self.n += 1

fn main() i32:
    c Counter
    i i32 = 0
    while i < 3:
        c.tick()
        i += 1
    printf("%d\n", c.n)
    return c.n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Counter = type { i32 }

@str = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(i8*, ...)

define void @Counter.tick(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %dref.self = load %Counter*, %Counter** %self, align 8
  %load.dref.self.n = getelementptr inbounds %Counter, %Counter* %dref.self, i32 0, i32 0
  %old = load i32, i32* %load.dref.self.n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.n, align 4
  ret void
}

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  %i = alloca i32, align 4
  %s = alloca [4 x i8], align 1
  store %Counter zeroinitializer, %Counter* %c, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  call void @Counter.tick(%Counter* %c)
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  %load.c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %load.load.c.n = load i32, i32* %load.c.n, align 4
  %"printf(str,load.load.c.n)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.load.c.n)
  %load.c.n1 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %load.load.c.n2 = load i32, i32* %load.c.n1, align 4
  ret i32 %load.load.c.n2
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 008 — two structs, one method name

```ura
// expect: 21   (1 + 2 * 10)
struct A:
    v i32

    fn get() i32:
        return self.v

struct B:
    v i32

    fn get() i32:
        return self.v * 10

fn main() i32:
    a A
    b B
    a.v = 1
    b.v = 2
    return a.get() + b.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%A = type { i32 }
%B = type { i32 }

define i32 @A.get(%A* %0) {
entry:
  %self = alloca %A*, align 8
  store %A* %0, %A** %self, align 8
  %dref.self = load %A*, %A** %self, align 8
  %load.dref.self.v = getelementptr inbounds %A, %A* %dref.self, i32 0, i32 0
  %load.load.dref.self.v = load i32, i32* %load.dref.self.v, align 4
  ret i32 %load.load.dref.self.v
}

define i32 @B.get(%B* %0) {
entry:
  %self = alloca %B*, align 8
  store %B* %0, %B** %self, align 8
  %dref.self = load %B*, %B** %self, align 8
  %load.dref.self.v = getelementptr inbounds %B, %B* %dref.self, i32 0, i32 0
  %load.load.dref.self.v = load i32, i32* %load.dref.self.v, align 4
  %MUL = mul i32 %load.load.dref.self.v, 10
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca %A, align 8
  %b = alloca %B, align 8
  store %A zeroinitializer, %A* %a, align 4
  store %B zeroinitializer, %B* %b, align 4
  %load.a.v = getelementptr inbounds %A, %A* %a, i32 0, i32 0
  store i32 1, i32* %load.a.v, align 4
  %load.b.v = getelementptr inbounds %B, %B* %b, i32 0, i32 0
  store i32 2, i32* %load.b.v, align 4
  %"get(&load.a)" = call i32 @A.get(%A* %a)
  %"get(&load.b)" = call i32 @B.get(%B* %b)
  %ADD = add i32 %"get(&load.a)", %"get(&load.b)"
  ret i32 %ADD
}
```

---

## 009 — a method on a nested attribute

```ura
// expect: 7
struct Point:
    x i32

    fn get() i32:
        return self.x

struct Rect:
    a Point
    b Point

fn main() i32:
    r Rect
    r.b.x = 7
    return r.b.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32 }
%Rect = type { %Point, %Point }

define i32 @Point.get(%Point* %0) {
entry:
  %self = alloca %Point*, align 8
  store %Point* %0, %Point** %self, align 8
  %dref.self = load %Point*, %Point** %self, align 8
  %load.dref.self.x = getelementptr inbounds %Point, %Point* %dref.self, i32 0, i32 0
  %load.load.dref.self.x = load i32, i32* %load.dref.self.x, align 4
  ret i32 %load.load.dref.self.x
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %load.r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %load.load.r.b.x = getelementptr inbounds %Point, %Point* %load.r.b, i32 0, i32 0
  store i32 7, i32* %load.load.r.b.x, align 4
  %load.r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %"get(&load.load.r.b)" = call i32 @Point.get(%Point* %load.r.b1)
  ret i32 %"get(&load.load.r.b)"
}
```

---

## 010 — overloaded method

```ura
// expect: 6   (1 + 5)
struct User:
    a i32

    fn set(n i32):
        self.a = n

    fn set():
        self.a = 1

fn main() i32:
    u User
    u.set()
    first i32 = u.a
    u.set(5)
    return first + u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.n = load i32, i32* %n, align 4
  store i32 %load.n, i32* %load.dref.self.a, align 4
  ret void
}

define void @User.set.1(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  store i32 1, i32* %load.dref.self.a, align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %first = alloca i32, align 4
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set.1(%User* %u)
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a, align 4
  store i32 %load.load.u.a, i32* %first, align 4
  call void @User.set(%User* %u, i32 5)
  %load.first = load i32, i32* %first, align 4
  %load.u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a2 = load i32, i32* %load.u.a1, align 4
  %ADD = add i32 %load.first, %load.load.u.a2
  ret i32 %ADD
}
```

---

## 011 — a method on a ref receiver

```ura
// expect: 4
struct User:
    a i32

    fn get() i32:
        return self.a

fn via(p &User) i32:
    return p.get()

fn main() i32:
    u User
    u.a = 4
    return via(&u)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  ret i32 %load.load.dref.self.a
}

define i32 @via(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %dref.p = load %User*, %User** %p, align 8
  %"get(&load.dref.p)" = call i32 @User.get(%User* %dref.p)
  ret i32 %"get(&load.dref.p)"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %load.u.a, align 4
  %"via(&u)" = call i32 @via(%User* %u)
  ret i32 %"via(&u)"
}
```

---

## 012 — a method through a ref changes the caller

```ura
// expect: 11
struct User:
    a i32

    fn set(n i32):
        self.a = n

fn reset(p &User):
    p.set(11)

fn main() i32:
    u User
    u.a = 3
    reset(&u)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.n = load i32, i32* %n, align 4
  store i32 %load.n, i32* %load.dref.self.a, align 4
  ret void
}

define void @reset(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %dref.p = load %User*, %User** %p, align 8
  call void @User.set(%User* %dref.p, i32 11)
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %load.u.a, align 4
  call void @reset(%User* %u)
  %load.u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a1, align 4
  ret i32 %load.load.u.a
}
```

