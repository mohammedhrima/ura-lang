# structs

## index

- 001 — init a struct
- 002 — modify struct attribute
- 003 — read struct attribute
- 004 — struct through a ref parameter
- 005 — method reads and writes self
- 006 — method with arguments
- 007 — method on a ref receiver
- 008 — overloaded method
- 009 — linked list

---

## 001 — init a struct

```ura
struct User:
   a i32
   b i32

fn main() i32:
   c i32
   u User
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %u = alloca %User, align 8
  ret i32 0
}
```

---

## 002 — modify struct attribute

```ura
struct User:
   a i32
   b i32

fn main() i32:
   c i32
   u User
   u.b = 10
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %u = alloca %User, align 8
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 10, i32* %u.b, align 4
  ret i32 0
}
```

---

## 003 — read struct attribute

```ura
struct User:
   a i32
   b i32

fn main() i32:
   u User
   u.b = 7
   return u.b
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 7, i32* %u.b, align 4
  %u.b1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %u.b2 = load i32, i32* %u.b1, align 4
  ret i32 %u.b2
}
```

---

## 004 — struct through a ref parameter

```ura
struct User:
   a i32

fn set(p &User):
   p.a = 3

fn main() i32:
   u User
   set(&u)
   return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @set(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %p1 = load %User*, %User** %p, align 8
  %p.a = getelementptr inbounds %User, %User* %p1, i32 0, i32 0
  store i32 3, i32* %p.a, align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  call void @set(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %u.a1 = load i32, i32* %u.a, align 4
  ret i32 %u.a1
}
```

---

## 005 — method reads and writes self

```ura
struct User:
   a i32

   fn method() i32:
      self.a = 10
      return self.a

fn main() i32:
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.method(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %self1 = load %User*, %User** %self, align 8
  %self.a = getelementptr inbounds %User, %User* %self1, i32 0, i32 0
  store i32 10, i32* %self.a, align 4
  %self2 = load %User*, %User** %self, align 8
  %self.a3 = getelementptr inbounds %User, %User* %self2, i32 0, i32 0
  %self.a4 = load i32, i32* %self.a3, align 4
  ret i32 %self.a4
}

define i32 @main() {
entry:
  ret i32 0
}
```

---

## 006 — method with arguments

```ura
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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.add(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %self1 = load %User*, %User** %self, align 8
  %self.a = getelementptr inbounds %User, %User* %self1, i32 0, i32 0
  %self.a2 = load i32, i32* %self.a, align 4
  %n3 = load i32, i32* %n, align 4
  %ADD = add i32 %self.a2, %n3
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %add = call i32 @User.add(%User* %u, i32 3)
  ret i32 %add
}
```

---

## 007 — method on a ref receiver

```ura
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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %self1 = load %User*, %User** %self, align 8
  %self.a = getelementptr inbounds %User, %User* %self1, i32 0, i32 0
  %self.a2 = load i32, i32* %self.a, align 4
  ret i32 %self.a2
}

define i32 @via(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %p1 = load %User*, %User** %p, align 8
  %get = call i32 @User.get(%User* %p1)
  ret i32 %get
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %u.a, align 4
  %via = call i32 @via(%User* %u)
  ret i32 %via
}
```

---

## 008 — overloaded method

```ura
struct User:
   a i32

   fn set(n i32):
      self.a = n

   fn set():
      self.a = 1

fn main() i32:
   u User
   u.set()
   return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %self2 = load %User*, %User** %self, align 8
  %self.a = getelementptr inbounds %User, %User* %self2, i32 0, i32 0
  store i32 %n1, i32* %self.a, align 4
  ret void
}

define void @User.set.1(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %self1 = load %User*, %User** %self, align 8
  %self.a = getelementptr inbounds %User, %User* %self1, i32 0, i32 0
  store i32 1, i32* %self.a, align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  call void @User.set.1(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %u.a1 = load i32, i32* %u.a, align 4
  ret i32 %u.a1
}
```

---

## 009 — linked list

```ura
struct Node:
   value i32
   next &Node

fn create(value i32) Node:
   res Node
   res.value = value
   res.next = null
   return res

fn main() i32:
   a Node = create(1)
   b Node = create(2)

   a.next = &b

   p &Node = &a
   sum i32 = 0
   while &p != null:
      sum += p.value
      p = &p.next

   // output("sum: ", sum, "\n")
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define %Node @create(i32 %0) {
entry:
  %value = alloca i32, align 4
  %res = alloca %Node, align 8
  store i32 %0, i32* %value, align 4
  %value1 = load i32, i32* %value, align 4
  %res.value = getelementptr inbounds %Node, %Node* %res, i32 0, i32 0
  store i32 %value1, i32* %res.value, align 4
  %res.next = getelementptr inbounds %Node, %Node* %res, i32 0, i32 1
  store %Node* null, %Node** %res.next, align 8
  %res2 = load %Node, %Node* %res, align 8
  ret %Node %res2
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %p = alloca %Node*, align 8
  %sum = alloca i32, align 4
  %create = call %Node @create(i32 1)
  store %Node %create, %Node* %a, align 8
  %create1 = call %Node @create(i32 2)
  store %Node %create1, %Node* %b, align 8
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store %Node* %a, %Node** %p, align 8
  store i32 0, i32* %sum, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %p2 = load %Node*, %Node** %p, align 8
  %NQ = icmp ne %Node* %p2, null
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %sum3 = load i32, i32* %sum, align 4
  %p4 = load %Node*, %Node** %p, align 8
  %p.value = getelementptr inbounds %Node, %Node* %p4, i32 0, i32 0
  %p.value5 = load i32, i32* %p.value, align 4
  %ADD = add i32 %sum3, %p.value5
  store i32 %ADD, i32* %sum, align 4
  %p6 = load %Node*, %Node** %p, align 8
  %p.next = getelementptr inbounds %Node, %Node* %p6, i32 0, i32 1
  %p.next7 = load %Node*, %Node** %p.next, align 8
  store %Node* %p.next7, %Node** %p, align 8
  br label %cond

endwhile:                                         ; preds = %cond
  ret i32 0
}
```

