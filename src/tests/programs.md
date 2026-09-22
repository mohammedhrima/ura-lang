# programs

## index

- 001 — linked list walked through ref attributes

---

## 001 — linked list walked through ref attributes

```ura
struct Node:
    value i32
    next &Node

fn main() i32:
    c Node
    c.value = 3
    c.next = null
    b Node
    b.value = 2
    b.next = &c
    a Node
    a.value = 1
    a.next = &b
    sum i32 = 0
    p &Node = &a
    while &p != null:
        sum += p.value
        p = &p.next
    return sum + a.next.next.value * 10
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %c = alloca %Node, align 8
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %c.next = getelementptr inbounds %Node, %Node* %c, i32 0, i32 1
  store %Node* null, %Node** %c.next, align 8
  %b = alloca %Node, align 8
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  %a = alloca %Node, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %sum = alloca i32, align 4
  store i32 0, i32* %sum, align 4
  %p = alloca %Node*, align 8
  store %Node* %a, %Node** %p, align 8
  br label %cond

cond:                                             ; preds = %then, %entry
  %p1 = load %Node*, %Node** %p, align 8
  %NQ = icmp ne %Node* %p1, null
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %sum2 = load i32, i32* %sum, align 4
  %p3 = load %Node*, %Node** %p, align 8
  %p.value = getelementptr inbounds %Node, %Node* %p3, i32 0, i32 0
  %p.value4 = load i32, i32* %p.value, align 4
  %ADD = add i32 %sum2, %p.value4
  store i32 %ADD, i32* %sum, align 4
  %p5 = load %Node*, %Node** %p, align 8
  %p.next = getelementptr inbounds %Node, %Node* %p5, i32 0, i32 1
  %p.next6 = load %Node*, %Node** %p.next, align 8
  store %Node* %p.next6, %Node** %p, align 8
  br label %cond

endwhile:                                         ; preds = %cond
  %sum7 = load i32, i32* %sum, align 4
  %a.next8 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %a.next9 = load %Node*, %Node** %a.next8, align 8
  %a.next.next = getelementptr inbounds %Node, %Node* %a.next9, i32 0, i32 1
  %a.next.next10 = load %Node*, %Node** %a.next.next, align 8
  %a.next.next.value = getelementptr inbounds %Node, %Node* %a.next.next10, i32 0, i32 0
  %a.next.next.value11 = load i32, i32* %a.next.next.value, align 4
  %MUL = mul i32 %a.next.next.value11, 10
  %ADD12 = add i32 %sum7, %MUL
  ret i32 %ADD12
}
```

