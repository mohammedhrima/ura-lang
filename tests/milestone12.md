# milestone12

## index

- 001 — nullable: a nullable return, a null comparison and a ?? default
- 002 — nullable: a non-null value widens into an optional
- 003 — nullable: an optional cannot be assigned to a non-optional
- 004 — nullable: ?? chains right to left until a non-null default
- 005 — nullable: a ?? chain must end in a non-null value
- 006 — nullable: ?? on a value that is never null is rejected
- 007 — nullable: comparing a never-null value to null is rejected
- 008 — nullable: null needs a target type
- 009 — try: a throw is caught in the same function
- 010 — try: a throw propagates across calls to a caller's catch
- 011 — try: a rethrow inside a catch propagates outward
- 012 — try: a nested try: the inner catch handles it
- 013 — try: return inside a try returns from the function
- 014 — try: break inside a try exits the enclosing loop
- 015 — try: an uncaught throw prints its message and exits
- 016 — try: throwing a non-Error is a compile error
- 017 — try: catch without a matching try is a parse error
- 018 — try: code after a throw is unreachable and dropped
- 019 — ref?: ref? nullable: bound at declaration, guarded reads pass, write-through works
- 020 — ref?: a guarded use (m?) of an unbound ref? traps; raw m would segfault
- 021 — ref?: ref return: create a node on the heap and return a reference to it
- 022 — ref?: ref? return: one branch returns a reference, the other returns null
- 023 — ref?: linked list: build with ref, walk it with cur = ref cur.next
- 024 — ref?: a ref parameter rebinds a struct's ref field in place (add-front)
- 025 — ref?: ref? compared to null with == and !=, bound and unbound
- 026 — ref?: chained rebind removes the head: lst.head = ref lst.head.next
- 027 — ref?: declare a ref and bind it to a ref-returning call
- 028 — ref?: a ref-returning call passed straight into a ref parameter
- 029 — ref?: a ref declaration cannot be bound to a plain value
- 030 — ref?: a field read chained directly on a ref-returning call
- 031 — ref?: a 'ref?' field may refer to its own struct
- 032 — ref?: bind, read and write through a 'ref?' field
- 033 — ref?: a method and a field chain called on a ref? field

---

## 001 — nullable: a nullable return, a null comparison and a ?? default

```ura
// stdout: Found: Iron Sword
// stdout: Chest is empty
// stdout: default: nothing
// expect: 0

fn find_weapon(chest i32) char[]?:
    if chest == 1: return "Iron Sword"
    if chest == 2: return "Magic Staff"
    return null

fn main() i32:
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
    return 0
```

---

## 002 — nullable: a non-null value widens into an optional

```ura
// stdout: hello
// expect: 0

fn main() i32:
    s1 char[]  = "hello"
    s2 char[]? = null
    s2 = s1
    output(s2 ?? "empty", "\n")
    return 0
```

---

## 003 — nullable: an optional cannot be assigned to a non-optional

```ura
// error: 's2' may be null; unwrap it with '?? <default>' or declare 's1' as optional

fn main() i32:
    s1 char[]  = "hello"
    s2 char[]? = null
    s1 = s2
    return 0
```

---

## 004 — nullable: ?? chains right to left until a non-null default

```ura
// stdout: []
// stdout: [found]
// stdout: [third]
// expect: 0

fn main() i32:
    s2 char[]? = null
    s3 char[]? = null
    s4 char[]? = "found"
    s1 char[]  = s2 ?? s3 ?? ""
    output("[", s1, "]\n")
    output("[", s2 ?? s3 ?? s4 ?? "deep", "]\n")
    s3 = "third"
    output("[", s2 ?? s3 ?? "", "]\n")
    return 0
```

---

## 005 — nullable: a ?? chain must end in a non-null value

```ura
// error: The right side of '??' may itself be null; it must not be

fn main() i32:
    s2 char[]? = null
    s3 char[]? = null
    s1 char[]  = s2 ?? s3
    return 0
```

---

## 006 — nullable: ?? on a value that is never null is rejected

```ura
// error: The left side of '??' is never null, so '??' does nothing

fn main() i32:
    s1 char[] = "hello"
    output(s1 ?? "h", "\n")
    return 0
```

---

## 007 — nullable: comparing a never-null value to null is rejected

```ura
// error: 's1' is never null, so this comparison is always the same

fn main() i32:
    s1 char[] = "hello"
    if s1 != null:
        output("never\n")
    return 0
```

---

## 008 — nullable: null needs a target type

```ura
// error: 'x' can never be null; declare it optional to assign null to it

fn main() i32:
    x char[] = null
    return 0
```

---

## 009 — try: a throw is caught in the same function

```ura
// stdout: caught: boom
// expect: 0
use "@/error"

fn main() i32:
    try:
        throw Error::make("boom")
    catch e:
        output("caught: ", e.message, "\n")
    return 0
```

---

## 010 — try: a throw propagates across calls to a caller's catch

```ura
// stdout: caught: from deep
// stdout: after
// expect: 0
use "@/error"

fn deep():
    throw Error::make("from deep")

fn middle():
    deep()

fn main() i32:
    try:
        middle()
    catch e:
        output("caught: ", e.message, "\n")
    output("after\n")
    return 0
```

---

## 011 — try: a rethrow inside a catch propagates outward

```ura
// stdout: mid: first
// stdout: main: second
// expect: 0
use "@/error"

fn middle():
    try:
        throw Error::make("first")
    catch e:
        output("mid: ", e.message, "\n")
        throw Error::make("second")

fn main() i32:
    try:
        middle()
    catch e:
        output("main: ", e.message, "\n")
    return 0
```

---

## 012 — try: a nested try: the inner catch handles it

```ura
// stdout: inner: inner
// expect: 0
use "@/error"

fn main() i32:
    try:
        try:
            throw Error::make("inner")
        catch e:
            output("inner: ", e.message, "\n")
    catch e:
        output("outer: ", e.message, "\n")
    return 0
```

---

## 013 — try: return inside a try returns from the function

```ura
// stdout: ret = 5
// expect: 0
use "@/error"

fn pick() i32:
    try:
        return 5
    catch e:
        return 0

fn main() i32:
    output("ret = ", pick(), "\n")
    return 0
```

---

## 014 — try: break inside a try exits the enclosing loop

```ura
// stdout: i=0
// stdout: done
// expect: 0
use "@/error"

fn main() i32:
    for i in 0..3:
        try:
            if i == 1:
                break
            output("i=", i, "\n")
        catch e:
            output("x\n")
    output("done\n")
    return 0
```

---

## 015 — try: an uncaught throw prints its message and exits

```ura
// stderr: uncaught
// expect: 1
use "@/error"

fn main() i32:
    throw Error::make("uncaught\n")
    output("unreachable\n")
    return 0
```

---

## 016 — try: throwing a non-Error is a compile error

```ura
// error: 'throw' expects an Error, got i32
use "@/error"

fn main() i32:
    throw 5
    return 0
```

---

## 017 — try: catch without a matching try is a parse error

```ura
// error: 'catch' without a matching 'try'
fn main() i32:
    catch e:
        output("x\n")
    return 0
```

---

## 018 — try: code after a throw is unreachable and dropped

```ura
// stdout: caught: x
// expect: 0
use "@/error"

fn boom():
    throw Error::make("x")
    output("dead\n")

fn main() i32:
    try:
        boom()
    catch e:
        output("caught: ", e.message, "\n")
    return 0
```

---

## 019 — ref?: ref? nullable: bound at declaration, guarded reads pass, write-through works

```ura
// stdout: 42
// stdout: 7
// expect: 0

fn main() i32:
    x i32 = 42
    ref? m i32 = &x
    output(m, "\n")
    m = 7
    output(x, "\n")
    return 0
```

---

## 020 — ref?: a guarded use (m?) of an unbound ref? traps; raw m would segfault

```ura
// stderr: runtime error: reference 'm' used before it was bound - assign 'm = ref <target>' first
// expect: 1

fn main() i32:
    ref? m i32
    output(m?, "\n")
    return 0
```

---

## 021 — ref?: ref return: create a node on the heap and return a reference to it

```ura
// stdout: 42
// expect: 0
struct Node:
    value i32
    ref? next Node

fn make(v i32, heap Node[]) ref Node:
    heap[0].value = v
    return ref heap[0]

fn main() i32:
    h Node[] = new Node[1]
    ref? a Node = make(42, h)
    output(a.value, "\n")
    return 0
```

---

## 022 — ref?: ref? return: one branch returns a reference, the other returns null

```ura
// stdout: 7 True
// expect: 0
struct N:
    v i32
    ref? nx N

fn pick(h N[], want bool) ref? N:
    if want:
        return ref h[0]
    return null

fn main() i32:
    h N[] = new N[1]
    h[0].v = 7
    ref? r N = pick(h, True)
    ref? e N = pick(h, False)
    output(r.v, " ", e == null, "\n")
    return 0
```

---

## 023 — ref?: linked list: build with ref, walk it with cur = ref cur.next

```ura
// stdout: sum=60
// expect: 0
struct Node:
    value i32
    ref? next Node

fn main() i32:
    a Node
    a.value = 10
    b Node
    b.value = 20
    c Node
    c.value = 30
    a.next = &b
    b.next = &c
    ref? cur Node = &a
    sum i32 = 0
    while cur != null:
        sum += cur.value
        cur = &cur.next
    output("sum=", sum, "\n")
    return 0
```

---

## 024 — ref?: a ref parameter rebinds a struct's ref field in place (add-front)

```ura
// stdout: 2 1
// expect: 0
struct Node:
    content i32
    ref? next Node

struct Lst:
    ref? head Node

fn push(lst &Lst, node &Node):
    node.next = &lst.head
    lst.head  = &node

fn main() i32:
    lst Lst
    a Node
    a.content = 1
    b Node
    b.content = 2
    push(&lst, &a)
    push(&lst, &b)
    ref? cur Node = &lst.head
    while cur != null:
        output(cur.content, " ")
        cur = &cur.next
    output("\n")
    return 0
```

---

## 025 — ref?: ref? compared to null with == and !=, bound and unbound

```ura
// stdout: False True True
// expect: 0
struct N:
    v i32
    ref? nx N

fn main() i32:
    a N
    a.v = 1
    ref? p N = &a
    ref? q N = null
    output(p == null, " ", p != null, " ", q == null, "\n")
    return 0
```

---

## 026 — ref?: chained rebind removes the head: lst.head = ref lst.head.next

```ura
// stdout: 2
// expect: 0
struct Node:
    v i32
    ref? next Node

struct Lst:
    ref? head Node

fn main() i32:
    a Node
    a.v = 1
    b Node
    b.v = 2
    a.next = &b
    lst Lst
    lst.head = &a
    lst.head = &lst.head.next
    output(lst.head.v, "\n")
    return 0
```

---

## 027 — ref?: declare a ref and bind it to a ref-returning call

```ura
// stdout: 9
// expect: 0
struct Node:
    v i32
    ref? next Node

fn make(v i32, h Node[]) ref Node:
    h[0].v = v
    return ref h[0]

fn main() i32:
    h Node[] = new Node[1]
    ref? n Node = make(9, h)
    output(n.v, "\n")
    return 0
```

---

## 028 — ref?: a ref-returning call passed straight into a ref parameter

```ura
// stdout: 5
// expect: 0
struct Node:
    v i32
    ref? next Node

struct Lst:
    ref? head Node

fn make(v i32, h Node[]) ref Node:
    h[0].v = v
    return ref h[0]

fn sethead(lst &Lst, node &Node):
    lst.head = &node

fn main() i32:
    h Node[] = new Node[1]
    lst Lst
    sethead(&lst, make(5, h))
    output(lst.head.v, "\n")
    return 0
```

---

## 029 — ref?: a ref declaration cannot be bound to a plain value

```ura
// error: A reference must be bound to a variable (ref x)
fn main() i32:
    ref? m i32 = 5
    output(m, "\n")
    return 0
```

---

## 030 — ref?: a field read chained directly on a ref-returning call

```ura
// stdout: 42
// expect: 0
struct Node:
    value i32
    ref? next Node

fn make(v i32, heap Node[]) ref Node:
    heap[0].value = v
    return ref heap[0]

fn main() i32:
    h Node[] = new Node[1]
    output(make(42, h).value, "\n")
    return 0
```

---

## 031 — ref?: a 'ref?' field may refer to its own struct

```ura
// stdout: 1
// expect: 0

struct Node:
    value i32
    ref? next Node

fn main() i32:
    a Node
    a.value = 1
    output(a.value, "\n")
    return 0
```

---

## 032 — ref?: bind, read and write through a 'ref?' field

```ura
// stdout: 20 3 20
// expect: 0

struct Node:
    value i32
    ref? next Node

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 1
    b.value = 2
    c.value = 3
    a.next = &b
    b.next = &c
    a.next.value = 20
    output(a.next.value, " ", a.next.next.value, " ", b.value, "\n")
    return 0
```

---

## 033 — ref?: a method and a field chain called on a ref? field

```ura
// stdout: worth 5
// stdout: worth 7
// stdout: worth 9
// stdout: chained field 9
// expect: 0

struct Item:
    worth i32
    ref? next Item

    pub fn create(w i32) Item:
        it Item
        it.worth = w
        return it

    fn show():
        output("worth ", self.worth, "\n")

fn main() i32:
    head Item = Item::create(5)
    mid  Item = Item::create(7)
    tail Item = Item::create(9)
    head.next = &mid
    mid.next  = &tail
    head.show()
    head.next.show()
    head.next.next.show()
    output("chained field ", head.next.next.worth, "\n")
    return 0
```

