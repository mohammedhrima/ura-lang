# milestone6

## index

- 001 — output: basic output: strings, ints, floats, bools
- 002 — output: output with expressions: arithmetic inline
- 003 — output: output all supported types in one block
- 004 — output: output() auto-formats each argument by its type
- 005 — errput: errput writes to stderr, output writes to stdout
- 006 — errput: errput auto-formats every scalar type, like output
- 007 — errput: errput expands a struct with the default dump
- 008 — errput: errput honours operator output()
- 009 — errput: errput prints arrays and nested arrays
- 010 — errput: errput follows a bound ref? and prints null when unbound
- 011 — errput: errput() with no arguments still emits the colour pair
- 012 — errput: errput and output keep separate struct printers
- 013 — string: String constructors and accessors
- 014 — string: assign, push, pop, join and clear
- 015 — string: find, contains, starts_with and ends_with
- 016 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse
- 017 — string: operators: + += == != < > against String and against a literal
- 018 — string: to_int, compare and chained transformations
- 019 — output: output() prints a struct and its nested struct
- 020 — output: output() follows a bound 'ref?' and prints null when unbound
- 021 — output: output() prints [Circular] instead of looping forever
- 022 — output: output() expands array fields, including 2D

---

## 001 — output: basic output: strings, ints, floats, bools

```ura
// stdout: === Aldric enters the dungeon ===
// stdout: Floor: 1
// stdout: Alive: True
// stdout: Speed: 1.500000
// stdout: Ready to fight!
// expect: 0

fn main() i32:
    output("=== Aldric enters the dungeon ===\n")
    output("Floor: ", 1, "\n")
    output("Alive: ", True, "\n")
    output("Speed: ", 1.5, "\n")
    output("Ready to fight!\n")
    return 0
```

---

## 002 — output: output with expressions: arithmetic inline

```ura
// stdout: Damage dealt: 17
// stdout: XP gained:    150
// stdout: Gold split:   30
// stdout: Bonus:        37
// expect: 0

fn main() i32:
    output("Damage dealt: ", 25 - 8, "\n")
    output("XP gained:    ", 3 * 50, "\n")
    output("Gold split:   ", 120 / 4, "\n")
    output("Bonus:        ", 7 + 3 * 10, "\n")
    return 0
```

---

## 003 — output: output all supported types in one block

```ura
// stdout: Name:  Aldric
// stdout: HP:    100
// stdout: Grade: S
// stdout: Alive: True
// stdout: Speed: 1.750000
// expect: 0

fn main() i32:
    name  char[] = "Aldric"
    hp    i32   = 100
    grade char  = 'S'
    alive bool  = True
    speed f32 = 1.75

    output("Name:  ", name,  "\n")
    output("HP:    ", hp,    "\n")
    output("Grade: ", grade, "\n")
    output("Alive: ", alive, "\n")
    output("Speed: ", speed, "\n")
    return 0
```

---

## 004 — output: output() auto-formats each argument by its type

```ura
// stdout: hello ura 42 True
// expect: 0

fn main() i32:
    name char[] = "ura"
    n i32 = 42
    ok bool = True
    output("hello ", name, " ", n, " ", ok, "\n")
    return 0
```

---

## 005 — errput: errput writes to stderr, output writes to stdout

```ura
// stdout: on stdout
// stderr: [0;31mon stderr
// expect: 1

fn main() i32:
    output("on stdout\n")
    errput("on stderr\n")
    return 0
```

---

## 006 — errput: errput auto-formats every scalar type, like output

```ura
// stderr: [0;31mhp=42 big=9000000000 small=-7
// expect: 1

fn main() i32:
    hp    i32  = 42
    big   i64  = 9000000000
    small i8   = -7
    alive bool = True
    grade char = 'A'
    speed f32  = 1.5
    exact f64  = 2.5
    errput("hp=", hp, " big=", big, " small=", small, "\n")
    errput("alive=", alive, " grade=", grade, "\n")
    errput("speed=", speed, " exact=", exact, "\n")
    return 0
```

---

## 007 — errput: errput expands a struct with the default dump

```ura
// stderr: [0;31mhero Hero{name: Aldric, stats: Stats{atk: 18, def: 7}}
// expect: 1

struct Stats:
    atk i32
    def i32

struct Hero:
    name  char[]
    stats Stats

fn main() i32:
    h Hero
    h.name = "Aldric"
    h.stats.atk = 18
    h.stats.def = 7
    errput("hero ", h, "\n")
    return 0
```

---

## 008 — errput: errput honours operator output()

```ura
// stderr: [0;31mtag custom printer
// expect: 1

struct Tag:
    name char[]

    operator output() char[]:
        ret self.name

fn main() i32:
    t Tag
    t.name = "custom printer"
    errput("tag ", t, "\n")
    return 0
```

---

## 009 — errput: errput prints arrays and nested arrays

```ura
// stderr: [0;31mnums [1, 2, 3]
// expect: 1

fn main() i32:
    nums i32[]   = [1, 2, 3]
    grid i32[][] = [[1, 2], [3, 4]]
    word char[]  = "runes"
    errput("nums ", nums, "\n")
    errput("grid ", grid, "\n")
    errput("word ", word, "\n")
    return 0
```

---

## 010 — errput: errput follows a bound ref? and prints null when unbound

```ura
// stderr: [0;31munbound Node{value: 1, next: null}
// expect: 1

struct Node:
    value i32
    ref? next Node

fn main() i32:
    ref? head Node = new Node
    head.value = 1
    errput("unbound ", head, "\n")
    ref? tail Node = new Node
    tail.value = 2
    head.next = &tail
    errput("bound ", head, "\n")
    clean tail
    clean head
    return 0
```

---

## 011 — errput: errput() with no arguments still emits the colour pair

```ura
// stderr: [0;31m[0m[0;31mafter the empty call
// expect: 1

fn main() i32:
    errput()
    errput("after the empty call\n")
    return 0
```

---

## 012 — errput: errput and output keep separate struct printers

```ura
// stdout: out Point{x: 3, y: 4}
// stderr: [0;31merr Point{x: 3, y: 4}
// expect: 1

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    p.x = 3
    p.y = 4
    errput("err ", p, "\n")
    output("out ", p, "\n")
    errput("err again ", p, "\n")
    return 0
```

---

## 013 — string: String constructors and accessors

```ura
// stdout: 0 True
// stdout: hello 5 False
// stdout: 0 True
// stdout: 42
// stdout: -123
// stdout: 0
// stdout: h o 0 0
// stdout: hello
// expect: 0
use "@/header"

fn main() i32:
    s1 String = String::create()
    output(s1.len(), " ", s1.empty(), "\n")

    s2 String = String::from("hello")
    output(s2.value, " ", s2.len(), " ", s2.empty(), "\n")

    s3 String = String::from("")
    output(s3.len(), " ", s3.empty(), "\n")

    s4 String = String::from_int(42)
    output(s4.value, "\n")

    s5 String = String::from_int(-123)
    output(s5.value, "\n")

    s6 String = String::from_int(0)
    output(s6.value, "\n")

    output(s2.at(0), " ", s2.at(4), " ",
           s2.at(-1) as i32, " ", s2.at(99) as i32, "\n")
    output(s2.c_str(), "\n")
    return 0
```

---

## 014 — string: assign, push, pop, join and clear

```ura
// stdout: abc 3
// stdout: hello world this is a longer string
// stdout: 0 True
// stdout: XYZ 3
// stdout: Z XY 2
// stdout: 0 0
// stdout: hello world
// stdout: 0 True
// expect: 0
use "@/header"

fn main() i32:
    s String = String::create()
    s = "abc"
    output(s.value, " ", s.len(), "\n")

    s = "hello world this is a longer string"
    output(s.value, "\n")

    s = ""
    output(s.len(), " ", s.empty(), "\n")

    s2 String = String::create()
    s2.push('X')
    s2.push('Y')
    s2.push('Z')
    output(s2.value, " ", s2.len(), "\n")

    c char = s2.pop()
    output(c, " ", s2.value, " ", s2.len(), "\n")

    s2.pop()
    s2.pop()
    c2 char = s2.pop()
    output(c2 as i32, " ", s2.len(), "\n")

    s3 String = String::from("hello")
    s3.join(" world")
    output(s3.value, "\n")

    s3.clear()
    output(s3.len(), " ", s3.empty(), "\n")
    return 0
```

---

## 015 — string: find, contains, starts_with and ends_with

```ura
// stdout: 6
// stdout: 0
// stdout: -1
// stdout: 0
// stdout: 10
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: True
// stdout: False
// stdout: True
// stdout: -1
// stdout: False
// stdout: True
// stdout: True
// expect: 0
use "@/header"

fn main() i32:
    s String = String::from("hello world")

    output(s.find("world"), "\n")
    output(s.find("hello"), "\n")
    output(s.find("xyz"), "\n")
    output(s.find(""), "\n")
    output(s.find("d"), "\n")

    output(s.contains("world"), "\n")
    output(s.contains("xyz"), "\n")

    output(s.starts_with("hello"), "\n")
    output(s.starts_with("world"), "\n")
    output(s.starts_with(""), "\n")

    output(s.ends_with("world"), "\n")
    output(s.ends_with("hello"), "\n")
    output(s.ends_with("d"), "\n")

    empty String = String::create()
    output(empty.find("x"), "\n")
    output(empty.contains("x"), "\n")
    output(empty.starts_with(""), "\n")
    output(empty.ends_with(""), "\n")
    return 0
```

---

## 016 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse

```ura
// stdout: HELLO WORLD
// stdout: hello world
// stdout: World
// stdout: Hello
// stdout: Hello World
// stdout: 0
// stdout: hello
// stdout: hello
// stdout: 0
// stdout: qux bar qux baz
// stdout: foo bar foo baz
// stdout: foo bar foo baz
// stdout: ababab
// stdout: 0
// stdout: edcba
// stdout: a
// expect: 0
use "@/header"

fn main() i32:
    s String = String::from("Hello World")

    u String = s.upper()
    output(u.value, "\n")

    l String = s.lower()
    output(l.value, "\n")

    sub String = s.substr(6, 5)
    output(sub.value, "\n")

    sub2 String = s.substr(0, 5)
    output(sub2.value, "\n")

    sub3 String = s.substr(0, 100)
    output(sub3.value, "\n")

    sub4 String = s.substr(-1, 5)
    output(sub4.len(), "\n")

    t1 String = String::from("   hello   ")
    t2 String = t1.trim()
    output(t2.value, "\n")

    t3 String = String::from("hello")
    t4 String = t3.trim()
    output(t4.value, "\n")

    t5 String = String::from("   ")
    t6 String = t5.trim()
    output(t6.len(), "\n")

    r1 String = String::from("foo bar foo baz")
    r2 String = r1.replace("foo", "qux")
    output(r2.value, "\n")

    r3 String = r1.replace("xyz", "qux")
    output(r3.value, "\n")

    r4 String = r1.replace("", "qux")
    output(r4.value, "\n")

    rep String = String::from("ab")
    rep2 String = rep.repeat(3)
    output(rep2.value, "\n")

    rep3 String = rep.repeat(0)
    output(rep3.len(), "\n")

    rev String = String::from("abcde")
    rev2 String = rev.reverse()
    output(rev2.value, "\n")

    rev3 String = String::from("a")
    rev4 String = rev3.reverse()
    output(rev4.value, "\n")
    return 0
```

---

## 017 — string: operators: + += == != < > against String and against a literal

```ura
// stdout: hello world
// stdout: hello there
// stdout: hello world
// stdout: testing
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: abc True
// expect: 0
use "@/header"

fn main() i32:
    a String = String::from("hello")
    b String = String::from(" world")
    c String = a + b
    output(c.value, "\n")

    d String = a + " there"
    output(d.value, "\n")

    a += b
    output(a.value, "\n")

    e String = String::from("test")
    e += "ing"
    output(e.value, "\n")

    x String = String::from("abc")
    y String = String::from("abc")
    z String = String::from("xyz")

    output(x == y, "\n")
    output(x == z, "\n")
    output(x != z, "\n")
    output(x != y, "\n")
    output(x < z, "\n")
    output(z < x, "\n")
    output(z > x, "\n")
    output(x > z, "\n")

    output(x == "abc", "\n")
    output(x == "xyz", "\n")
    output(x != "xyz", "\n")
    output(x != "abc", "\n")

    f String = String::create()
    g String = String::create()
    f = y
    output(f.value, " ", f == y, "\n")
    return 0
```

---

## 018 — string: to_int, compare and chained transformations

```ura
// stdout: 12345
// stdout: -99
// stdout: 0
// stdout: -1
// stdout: 1
// stdout: 0
// stdout: DLROW OLLEH
// stdout: 24
// stdout: 1000000
// stdout: -2000000
// expect: 0
use "@/header"

fn main() i32:
    s1 String = String::from("12345")
    output(s1.to_int(), "\n")

    s2 String = String::from("-99")
    output(s2.to_int(), "\n")

    s3 String = String::from("0")
    output(s3.to_int(), "\n")

    a String = String::from("apple")
    b String = String::from("banana")
    output(a.compare(&b), "\n")
    output(b.compare(&a), "\n")
    output(a.compare(&a), "\n")

    s String = String::from("Hello World")
    r String = s.upper()
    r2 String = r.reverse()
    output(r2.value, "\n")

    big String = String::from("aaaa")
    i i32 = 0
    while i < 5:
        big.join("bbbb")
        i += 1
    output(big.len(), "\n")

    n1 String = String::from_int(1000000)
    output(n1.value, "\n")

    n2 String = String::from_int(-2000000)
    output(n2.value, "\n")
    return 0
```

---

## 019 — output: output() prints a struct and its nested struct

```ura
// stdout: flat   Room{name: hall, floor: 2}
// stdout: nested Dungeon{name: keep, entry: Room{name: gate, floor: 1}}
// stdout: two: Room{name: hall, floor: 2} and Room{name: gate, floor: 1} done
// expect: 0

struct Room:
    name  char[]
    floor i32

struct Dungeon:
    name  char[]
    entry Room

fn main() i32:
    r Room
    r.name  = "hall"
    r.floor = 2
    output("flat   ", r, "\n")

    d Dungeon
    d.name = "keep"
    d.entry.name  = "gate"
    d.entry.floor = 1
    output("nested ", d, "\n")

    output("two: ", r, " and ", d.entry, " done\n")
    return 0
```

---

## 020 — output: output() follows a bound 'ref?' and prints null when unbound

```ura
// stdout: unbound Node{value: 1, next: null}
// stdout: bound   Node{value: 1, next: ref Node{value: 2, next: null}}
// expect: 0

struct Node:
    value i32
    ref? next Node

fn main() i32:
    a Node
    a.value = 1
    output("unbound ", a, "\n")

    b Node
    b.value = 2
    a.next = &b
    output("bound   ", a, "\n")
    return 0
```

---

## 021 — output: output() prints [Circular] instead of looping forever

```ura
// stdout: cycle Node{value: 1, next: ref Node{value: 2, next: ref [Circular]}}
// stdout: self  Node{value: 2, next: ref Node{value: 1, next: ref [Circular]}}
// expect: 0

struct Node:
    value i32
    ref? next Node

fn main() i32:
    a Node
    b Node
    a.value = 1
    b.value = 2
    a.next = &b
    b.next = &a
    output("cycle ", a, "\n")
    output("self  ", b, "\n")
    return 0
```

---

## 022 — output: output() expands array fields, including 2D

```ura
// stdout: Flat{tag: flat, rooms: [Room{floor: 3}, Room{floor: 4}]}
// stdout: Grid{tag: grid, rooms: [[Room{floor: 1}, Room{floor: 2}], [Room{floor: 3}, Room{floor: 4}]]}
// expect: 0

struct Room:
    floor i32

struct Flat:
    tag   char[]
    rooms Room[]

struct Grid:
    tag   char[]
    rooms Room[][]

fn main() i32:
    f Flat
    f.tag   = "flat"
    f.rooms = Room[2]
    f.rooms[0].floor = 3
    f.rooms[1].floor = 4
    output(f, "\n")

    g Grid
    g.tag   = "grid"
    g.rooms = Room[2][2]
    g.rooms[0][0].floor = 1
    g.rooms[0][1].floor = 2
    g.rooms[1][0].floor = 3
    g.rooms[1][1].floor = 4
    output(g, "\n")
    return 0
```

