# milestone8

## index

- 001 — array: 2D array literal and nested indexing
- 002 — array: sized 2D matrix i32[N][M], zero-init and writes
- 003 — array: 3D sized array with runtime size
- 004 — array: heap array: new i32[N], zero-init, write, clean
- 005 — array: heap array with runtime size (new i32[n]) + clean
- 006 — array: multi-dim heap: new i32[N][M] + recursive clean
- 007 — array: slice a[1..4] is a view (shared storage, exclusive end)
- 008 — array: arr.len on stack, heap, slice, and multi-dim
- 009 — array: clean on a non-array
- 010 — array: new without an array type
- 011 — array: range bounds must be integers
- 012 — array: .len on a non-array
- 013 — array: array literals: one element, expressions, and strings
- 014 — array: fill a heap char buffer by index, print it, clean it
- 015 — enum: a variant as an array index and in arithmetic
- 016 — errput: errput follows a bound ref? and prints null when unbound
- 017 — heap: new T allocates one zeroed struct and binds it to a ref
- 018 — heap: clean frees a heap struct and nulls the reference
- 019 — heap: clean on an unbound reference is a no-op
- 020 — heap: cleaning twice is safe
- 021 — heap: operator drop cascades, freeing a whole tree
- 022 — heap: a function returns new T from a ref? return
- 023 — heap: clean on a value struct is still an error
- 024 — heap: new without an array type is still an error
- 025 — lexer: an index with nothing in it
- 026 — lexer: :: must be followed by a name
- 027 — operator: operator drop declares a destructor named Vec.drop
- 028 — operator: 'clean' does not destroy a struct, even one with operator drop
- 029 — operator: calling .drop() yourself is rejected
- 030 — operator: operator drop beside operator = does not warn
- 031 — operator: a destructor fires on break and on continue
- 032 — operator: drop on fall-through: an if block, each iteration, reverse order
- 033 — operator: a struct drops the structs it owns, at any depth, but not what it borrows
- 034 — operator: a heap-owning struct: create, deep-copy operator = and drop
- 035 — operator: a growable string struct: nullable buffer, grow, join and drop
- 036 — operator: a struct owning heap buffers: deep-copy operator = and drop
- 037 — ref: ref return: create a node on the heap and return a reference to it
- 038 — ref: ref? return: one branch returns a reference, the other returns null
- 039 — ref: declare a ref and bind it to a ref-returning call
- 040 — ref: a ref-returning call passed straight into a ref parameter
- 041 — ref: a reference function must return a reference, not a bare value
- 042 — ref: a field read chained directly on a ref-returning call
- 043 — struct: a heap array of structs: new Room[N], then clean
- 044 — struct: a 2D heap array of structs frees every inner slice
- 045 — struct: an unknown type as a parameter, a return type, or an array element
- 046 — struct: destructor must take no parameters
- 047 — struct: destructor must return void
- 048 — variable: global variables: scalars, arrays, forward reference, shadowing
- 049 — variable: a global struct is initialised and dropped like a local
- 050 — variable: a global is dropped on an early return too

---

## 001 — array: 2D array literal and nested indexing

```ura
// stdout: m[0][0]=1 m[1][1]=4
// stdout: m[0][1]=99
// expect: 0

fn main() i32:
    m i32[][] = [[1, 2], [3, 4]]
    output("m[0][0]=", m[0][0], " m[1][1]=", m[1][1], "\n")
    m[0][1] = 99
    output("m[0][1]=", m[0][1], "\n")
    return 0
```

---

## 002 — array: sized 2D matrix i32[N][M], zero-init and writes

```ura
// stdout: g[0][0]=1 g[2][3]=99 g[1][1]=0
// expect: 0

fn main() i32:
    g i32[][] = i32[3][4]
    g[0][0] = 1
    g[2][3] = 99
    output("g[0][0]=", g[0][0], " g[2][3]=", g[2][3], " g[1][1]=", g[1][1], "\n")
    return 0
```

---

## 003 — array: 3D sized array with runtime size

```ura
// stdout: v[1][1][1]=7 v[0][0][0]=0
// expect: 0

fn main() i32:
    n i32 = 2
    v i32[][][] = i32[n][n][n]
    v[1][1][1] = 7
    output("v[1][1][1]=", v[1][1][1], " v[0][0][0]=", v[0][0][0], "\n")
    return 0
```

---

## 004 — array: heap array: new i32[N], zero-init, write, clean

```ura
// stdout: a[0]=0 a[2]=42
// expect: 0

fn main() i32:
    a i32[] = new i32[5]
    a[2] = 42
    output("a[0]=", a[0], " a[2]=", a[2], "\n")
    clean a
    return 0
```

---

## 005 — array: heap array with runtime size (new i32[n]) + clean

```ura
// stdout: a[5]=7 a[0]=0
// expect: 0

fn main() i32:
    n i32 = 3
    a i32[] = new i32[n * 2]
    a[5] = 7
    output("a[5]=", a[5], " a[0]=", a[0], "\n")
    clean a
    return 0
```

---

## 006 — array: multi-dim heap: new i32[N][M] + recursive clean

```ura
// stdout: m[0][0]=1 m[1][2]=9 m[0][1]=0
// expect: 0

fn main() i32:
    m i32[][] = new i32[2][3]
    m[0][0] = 1
    m[1][2] = 9
    output("m[0][0]=", m[0][0], " m[1][2]=", m[1][2], " m[0][1]=", m[0][1], "\n")
    clean m
    return 0
```

---

## 007 — array: slice a[1..4] is a view (shared storage, exclusive end)

```ura
// stdout: mid: 20 30 40
// stdout: a[1]=99
// expect: 0

fn main() i32:
    a i32[] = [10, 20, 30, 40, 50]
    mid i32[] = a[1..4]
    output("mid: ", mid[0], " ", mid[1], " ", mid[2], "\n")
    mid[0] = 99
    output("a[1]=", a[1], "\n")
    return 0
```

---

## 008 — array: arr.len on stack, heap, slice, and multi-dim

```ura
// stdout: len = 4
// stdout: slice len = 2
// stdout: rows = 3 cols = 4
// expect: 0

fn main() i32:
    nums i32[] = [10, 20, 30, 40]
    output("len = ", nums.len, "\n")
    mid i32[] = nums[1..3]
    output("slice len = ", mid.len, "\n")
    grid i32[][] = i32[3][4]
    output("rows = ", grid.len, " cols = ", grid[0].len, "\n")
    return 0
```

---

## 009 — array: clean on a non-array

```ura
// error: 'clean' frees a heap array, not i32
fn main() i32:
    x i32 = 5
    clean x
    return 0
```

---

## 010 — array: new without an array type

```ura
// error: Expected an array type after 'new' (e.g. new int[n])
fn main() i32:
    a i32[] = new i32
    return 0
```

---

## 011 — array: range bounds must be integers

```ura
// error: Range bounds must be integers
fn main() i32:
    a i32[] = [1, 2, 3]
    mid i32[] = a[1.5..3]
    return 0
```

---

## 012 — array: .len on a non-array

```ura
// error: '.len' is only valid on an array, not i32
fn main() i32:
    x i32 = 5
    output(x.len)
    return 0
```

---

## 013 — array: array literals: one element, expressions, and strings

```ura
// stdout: 42 len 1
// stdout: 5 6 10 10
// stdout: ada grace alan len 3
// expect: 0

fn twice(n i32) i32:
    return n * 2

fn main() i32:
    // a single-element literal
    one i32[] = [42]
    output(one[0], " len ", one.len, "\n")

    // elements that are not compile-time constants
    x i32 = 5
    ex i32[] = [x, x + 1, x * 2, twice(x)]
    for v in ex:
        output(v, " ")
    output("\n")

    // an array of strings
    names char[][] = ["ada", "grace", "alan"]
    for n in names:
        output(n, " ")
    output("len ", names.len, "\n")
    return 0
```

---

## 014 — array: fill a heap char buffer by index, print it, clean it

```ura
// stdout: DUNGEON
// expect: 0

fn main() i32:
    str char[] = new char[8]
    str[0] = 'D'
    str[1] = 'U'
    str[2] = 'N'
    str[3] = 'G'
    str[4] = 'E'
    str[5] = 'O'
    str[6] = 'N'
    str[7] = 0 as char
    output(str, "\n")
    clean str
    return 0
```

---

## 015 — enum: a variant as an array index and in arithmetic

```ura
// stdout: 14 2
// expect: 0
enum Slot: HEAD, BODY, LEGS

fn main() i32:
    gear i32[] = new i32[3]
    gear[HEAD] = 5
    gear[LEGS] = 9
    output(gear[HEAD] + gear[LEGS], " ", LEGS - HEAD, "\n")
    return 0
```

---

## 016 — errput: errput follows a bound ref? and prints null when unbound

```ura
// stderr: [0;31munbound Node{value: 1, next: null}
// expect: 1

struct Node:
    value i32
    next &Node

fn main() i32:
    head &Node = new Node
    head.value = 1
    errput("unbound ", head, "\n")
    tail &Node = new Node
    tail.value = 2
    head.next = &tail
    errput("bound ", head, "\n")
    clean tail
    clean head
    return 0
```

---

## 017 — heap: new T allocates one zeroed struct and binds it to a ref

```ura
// stdout: 0 0
// stdout: 7
// expect: 0
struct P:
    a i32
    b i32

fn main() i32:
    p &P = new P
    output(p.a, " ", p.b, "\n")
    p.a = 7
    output(p.a, "\n")
    clean p
    return 0
```

---

## 018 — heap: clean frees a heap struct and nulls the reference

```ura
// stdout: 3
// stdout: True
// expect: 0
struct P:
    a i32

fn main() i32:
    p &P = new P
    p.a = 3
    output(p.a, "\n")
    clean p
    output(p == null, "\n")
    return 0
```

---

## 019 — heap: clean on an unbound reference is a no-op

```ura
// stdout: ok
// expect: 0
struct P:
    a i32

fn main() i32:
    p &P
    clean p
    output("ok\n")
    return 0
```

---

## 020 — heap: cleaning twice is safe

```ura
// stdout: ok
// expect: 0
struct P:
    a i32

fn main() i32:
    p &P = new P
    clean p
    clean p
    output("ok\n")
    return 0
```

---

## 021 — heap: operator drop cascades, freeing a whole tree

```ura
// stdout: 1 2 3
// stdout: freed
// stderr: warning: Struct Node has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
// expect: 1
struct Node:
    value i32
    left &Node
    right &Node

    operator drop:
        clean self.left
        clean self.right

fn main() i32:
    root &Node = new Node
    root.value = 1
    l &Node = new Node
    l.value = 2
    r &Node = new Node
    r.value = 3
    root.left = &l
    root.right = &r
    output(root.value, " ", root.left.value, " ", root.right.value, "\n")
    clean root
    output("freed\n")
    return 0
```

---

## 022 — heap: a function returns new T from a ref? return

```ura
// stdout: 9
// expect: 0
struct P:
    v i32

fn make(x i32) &P:
    p &P = new P
    p.v = x
    ret &p

fn main() i32:
    q &P = make(9)
    output(q.v, "\n")
    clean q
    return 0
```

---

## 023 — heap: clean on a value struct is still an error

```ura
// error: 'clean' frees a heap array, not the struct P; a struct is destroyed at end of scope by its 'operator drop'
struct P:
    v i32

fn main() i32:
    p P
    clean p
    return 0
```

---

## 024 — heap: new without an array type is still an error

```ura
// error: Expected an array type after 'new' (e.g. new int[n])
fn main() i32:
    a i32[] = new i32
    return 0
```

---

## 025 — lexer: an index with nothing in it

```ura
// error: Unexpected token

fn main() i32:
    arr i32[] = new i32[3]
    x i32 = arr[]
    return 0
```

---

## 026 — lexer: :: must be followed by a name

```ura
// error: Expected a name after '::'
use "@/header"

fn main() i32:
    s String = String::new
    return 0
```

---

## 027 — operator: operator drop declares a destructor named Vec.drop

```ura
// stdout: done
// stdout: dropping 7
// stderr: warning: Struct Vec has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
// expect: 1

struct Vec:
    x i32

    operator drop:
        output("dropping ", self.x, "\n")

fn main() i32:
    v Vec
    v.x = 7
    output("done\n")
    return 0
```

---

## 028 — operator: 'clean' does not destroy a struct, even one with operator drop

```ura
// stderr: warning: Struct Vec has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
// expect: 1

struct Vec:
    x i32

    operator drop:
        output("dropping\n")

fn main() i32:
    v Vec
    clean v
    return 0
```

---

## 029 — operator: calling .drop() yourself is rejected

```ura
// stderr: warning: Struct Vec has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
// expect: 1

struct Vec:
    x i32

    operator drop:
        output("dropping\n")

fn main() i32:
    v Vec
    v.drop()
    return 0
```

---

## 030 — operator: operator drop beside operator = does not warn

```ura
// stdout: done
// stdout: drop 5
// expect: 0

struct Vec:
    x i32

    operator =(other Vec) void:
        self.x = other.x

    operator drop:
        output("drop ", self.x, "\n")

fn main() i32:
    a Vec
    a.x = 5
    output("done\n")
    return 0
```

---

## 031 — operator: a destructor fires on break and on continue

```ura
// stdout: drop 100
// stdout: drop 101
// stdout: after break
// stdout: drop 200
// stdout: drop 201
// stdout: after continue
// expect: 0

struct Tag:
    id i32

    pub fn create(n i32) Tag:
        t Tag
        t.id = n
        return t

    operator =(other Tag) void:
        self.id = other.id

    operator drop:
        output("drop ", self.id, "\n")

fn main() i32:
    i i32 = 0
    while i < 3:
        b Tag = Tag::create(100 + i)
        if i == 1:
            break
        i = i + 1
    output("after break\n")

    k i32 = 0
    while k < 2:
        c Tag = Tag::create(200 + k)
        k = k + 1
        continue
    output("after continue\n")
    return 0
```

---

## 032 — operator: drop on fall-through: an if block, each iteration, reverse order

```ura
// stdout: inside
// stdout: drop 1
// stdout: after if
// stdout: drop 10
// stdout: drop 11
// stdout: drop 12
// stdout: after loop
// stdout: end of main
// stdout: drop 8
// stdout: drop 7
// expect: 0

struct Tag:
    id i32

    pub fn create(n i32) Tag:
        t Tag
        t.id = n
        return t

    operator =(other Tag) void:
        self.id = other.id

    operator drop:
        output("drop ", self.id, "\n")

fn main() i32:
    if 1 < 2:
        x Tag = Tag::create(1)
        output("inside\n")
    output("after if\n")

    i i32 = 0
    while i < 3:
        y Tag = Tag::create(10 + i)
        i += 1
    output("after loop\n")

    p Tag = Tag::create(7)
    q Tag = Tag::create(8)
    output("end of main\n")
    return 0
```

---

## 033 — operator: a struct drops the structs it owns, at any depth, but not what it borrows

```ura
// stdout: built
// stdout: drop 1
// stdout: drop 3
// stdout: drop 9
// expect: 0

struct Res:
    id i32

    pub fn create(n i32) Res:
        r Res
        r.id = n
        return r

    operator =(other Res) void:
        self.id = other.id

    operator drop:
        output("drop ", self.id, "\n")

struct L3:
    res Res
struct L2:
    inner L3

// Owner declares no destructor of its own — everything below still drops
struct Owner:
    mid  L2
    also Res
    borrowed &Res

    pub fn create() Owner:
        o Owner
        o.mid.inner.res = Res::create(3)
        o.also          = Res::create(1)
        return o

fn main() i32:
    lent Res = Res::create(9)
    o Owner = Owner::create()
    o.borrowed = &lent
    output("built\n")
    return 0
```

---

## 034 — operator: a heap-owning struct: create, deep-copy operator = and drop

```ura
// stdout: b1: hello
// stdout: b2: world
// expect: 0

use "@/string"

struct Buffer:
    value char[]

    pub fn create() Buffer:
        b Buffer
        b.value = new char[64]
        return b

    operator drop:
        clean self.value

    operator = (s char[]) void:
        strcpy(self.value, s)

    operator = (b &Buffer) void:
        strcpy(self.value, b.value)

fn main() i32:
    bool Buffer = Buffer::create()
    bool = "hello"

    b2 Buffer = Buffer::create()
    b2 = bool

    strcpy(b2.value, "world")

    output("bool: ", bool.value, "\n")
    output("b2: ", b2.value, "\n")
    return 0
```

---

## 035 — operator: a growable string struct: nullable buffer, grow, join and drop

```ura
// stdout: s0:    1x2345 (6)
// stdout: s1:    6x789 (5)
// stdout: s0+s1: 1x23456x789 (11)
// expect: 0

use "@/header"

struct String2:
    value char[]
    count i32
    size  i32

    pub fn create() String2:
        s String2
        s.size  = 10
        s.value = new char[10]
        return s

    operator drop:
        clean self.value

    fn grow(want i32):
        if self.size >= want: return
        while self.size < want:
            self.size *= 2
        bigger char[] = new char[self.size]
        strcpy(bigger, self.value)
        clean self.value
        self.value = bigger

    fn assign(str char[]):
        if str == null:
            self.assign("")
            return
        len i32 = strlen(str) as i32
        self.grow(len + 1)
        bzero(self.value, self.size as i64)
        strcpy(self.value, str)
        self.count = len

    fn join(str char[]):
        if str == null: return
        len i32 = strlen(str) as i32
        self.grow(self.count + len + 1)
        strcat(self.value, str)
        self.count += len

    operator = (str char[]) void:
        self.assign(str)

    operator = (v &String2) void:
        self.assign(v.value)

fn add(v &String2, w &String2) String2:
    res String2 = String2::create()
    res = v
    res.join(w.value)
    return res

fn main() i32:
    s0 String2 = String2::create()
    s0 = "1x2345"

    s1 String2 = String2::create()
    s1 = "6x789"

    s2 String2 = String2::create()
    s2 = add(&s0, &s1)

    output("s0:    ", s0.value, " (", s0.count, ")\n")
    output("s1:    ", s1.value, " (", s1.count, ")\n")
    output("s0+s1: ", s2.value, " (", s2.count, ")\n")
    return 0
```

---

## 036 — operator: a struct owning heap buffers: deep-copy operator = and drop

```ura
// stdout: The Bold Aldric enters!
// stdout: The Grim Borin enters!
// stdout: The Grim Borin enters!
// stdout: The Quick Cass enters!
// expect: 0
use "@/header"

struct Hero:
    name  char[]
    title char[]

    pub fn create() Hero:
        h Hero
        h.name  = new char[32]
        h.title = new char[32]
        return h

    fn setup(name char[], title char[]):
        strcpy(self.name,  name)
        strcpy(self.title, title)

    fn announce():
        output(self.title, " ", self.name, " enters!\n")

    operator = (other &Hero) void:
        clean self.name
        clean self.title
        self.name  = new char[32]
        self.title = new char[32]
        strcpy(self.name,  other.name)
        strcpy(self.title, other.title)

    operator drop:
        clean self.name
        clean self.title

fn main() i32:
    hero Hero = Hero::create()
    hero.setup("Aldric", "The Bold")
    hero.announce()

    other Hero = Hero::create()
    other.setup("Borin", "The Grim")
    hero = other
    hero.announce()

    // the copy owns its own buffers, so mutating the source leaves it alone
    other.setup("Cass", "The Quick")
    hero.announce()
    other.announce()
    return 0
```

---

## 037 — ref: ref return: create a node on the heap and return a reference to it

```ura
// stdout: 42
// expect: 0
struct Node:
    value i32
    next &Node

fn make(v i32, heap Node[]) &Node:
    heap[0].value = v
    return &heap[0]

fn main() i32:
    h Node[] = new Node[1]
    a &Node = make(42, h)
    output(a.value, "\n")
    return 0
```

---

## 038 — ref: ref? return: one branch returns a reference, the other returns null

```ura
// stdout: 7 True
// expect: 0
struct N:
    v i32
    nx &N

fn pick(h N[], want bool) &N:
    if want:
        return &h[0]
    return null

fn main() i32:
    h N[] = new N[1]
    h[0].v = 7
    r &N = pick(h, True)
    e &N = pick(h, False)
    output(r.v, " ", e == null, "\n")
    return 0
```

---

## 039 — ref: declare a ref and bind it to a ref-returning call

```ura
// stdout: 9
// expect: 0
struct Node:
    v i32
    next &Node

fn make(v i32, h Node[]) &Node:
    h[0].v = v
    return &h[0]

fn main() i32:
    h Node[] = new Node[1]
    n &Node = make(9, h)
    output(n.v, "\n")
    return 0
```

---

## 040 — ref: a ref-returning call passed straight into a ref parameter

```ura
// stdout: 5
// expect: 0
struct Node:
    v i32
    next &Node

struct Lst:
    head &Node

fn make(v i32, h Node[]) &Node:
    h[0].v = v
    return &h[0]

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

## 041 — ref: a reference function must return a reference, not a bare value

```ura
// error: 'g' returns a reference; return `ref x` (or null), not a value
struct N:
    v i32

fn g(h N[]) &N:
    return h[0]

fn main() i32:
    h N[] = new N[1]
    output(g(h).v, "\n")
    return 0
```

---

## 042 — ref: a field read chained directly on a ref-returning call

```ura
// stdout: 42
// expect: 0
struct Node:
    value i32
    next &Node

fn make(v i32, heap Node[]) &Node:
    heap[0].value = v
    return &heap[0]

fn main() i32:
    h Node[] = new Node[1]
    output(make(42, h).value, "\n")
    return 0
```

---

## 043 — struct: a heap array of structs: new Room[N], then clean

```ura
// stdout: attic@9 2
// expect: 0

struct Room:
    name  char[]
    floor i32

fn main() i32:
    h Room[] = new Room[2]
    h[0].name  = "attic"
    h[0].floor = 9
    h[1].floor = 2
    output(h[0].name, "@", h[0].floor, " ", h[1].floor, "\n")
    clean h
    return 0
```

---

## 044 — struct: a 2D heap array of structs frees every inner slice

```ura
// stdout: 1 4
// expect: 0

struct Room:
    floor i32

fn main() i32:
    g Room[][] = new Room[2][2]
    g[0][0].floor = 1
    g[1][1].floor = 4
    output(g[0][0].floor, " ", g[1][1].floor, "\n")
    clean g
    return 0
```

---

## 045 — struct: an unknown type as a parameter, a return type, or an array element

```ura
// error: Unknown type 'Widget'

fn process(p Widget):
    return

fn make() Widget:
    w Widget
    return w

fn main() i32:
    arr Widget[] = new Widget[3]
    output("hi\n")
    return 0
```

---

## 046 — struct: destructor must take no parameters

```ura
// error: Destructor 'drop' takes no parameters; write 'operator drop:'
use "@/header"

struct Box:
    value i32

    operator drop(extra i32) void:
        return

fn main() i32:
    b Box
    b.value = 1
    return 0
```

---

## 047 — struct: destructor must return void

```ura
// error: Destructor 'drop' returns void; drop the return type
use "@/header"

struct Box:
    value i32

    operator drop i32:
        return 0

fn main() i32:
    b Box
    b.value = 1
    return 0
```

---

## 048 — variable: global variables: scalars, arrays, forward reference, shadowing

```ura
// stdout: ura 1.500000 True
// stdout: total starts 7
// stdout: total now 9
// stdout: xs 7 len 3  heap 9 len 2
// stdout: local total 99
// expect: 0

name  char[] = "ura"
ratio f32 = 1.5
flag  bool  = True
xs    i32[] = i32[3]
heap  i32[] = new i32[2]

fn touch():
    total += 1

fn main() i32:
    output(name, " ", ratio, " ", flag, "\n")
    // forward reference: total is declared below main
    output("total starts ", total, "\n")
    touch()
    touch()
    output("total now ", total, "\n")

    xs[0] = 7
    heap[1] = 9
    output("xs ", xs[0], " len ", xs.len)
    output("  heap ", heap[1], " len ", heap.len, "\n")
    clean heap

    // a local shadows the global for the rest of this scope
    total i32 = 99
    output("local total ", total, "\n")
    return 0

total i32 = 7
```

---

## 049 — variable: a global struct is initialised and dropped like a local

```ura
// stdout: start 10
// stdout: after 12
// stdout: DROP at 12
// expect: 0

struct Counter:
    hits i32

    pub fn create(n i32) Counter:
        c Counter
        c.hits = n
        return c

    operator =(other Counter) void:
        self.hits = other.hits

    fn bump():
        self.hits += 1

    operator drop:
        output("DROP at ", self.hits, "\n")

tally Counter = Counter::create(10)

fn touch():
    tally.bump()

fn main() i32:
    output("start ", tally.hits, "\n")
    touch()
    touch()
    output("after ", tally.hits, "\n")
    return 0
```

---

## 050 — variable: a global is dropped on an early return too

```ura
// stdout: body
// stdout: early
// stdout: DROP 2
// stdout: DROP 1
// expect: 0

struct Res:
    id i32

    pub fn create(n i32) Res:
        r Res
        r.id = n
        return r

    operator =(other Res) void:
        self.id = other.id

    operator drop:
        output("DROP ", self.id, "\n")

a Res = Res::create(1)
b Res = Res::create(2)

fn main() i32:
    output("body\n")
    if 1 < 2:
        output("early\n")
        return 0
    output("unreachable\n")
    return 0
```

