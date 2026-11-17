# milestone6

## index

- 001 — calls: output a struct
- 002 — function: multi line string
- 003 — builtin: output picks the spec from each argument
- 004 — builtin: output with literals and escapes
- 005 — errput: errput writes to stderr, output writes to stdout
- 006 — errput: errput auto-formats every scalar type, like output
- 007 — errput: errput expands a struct with the default dump
- 008 — errput: errput honours operator output()
- 009 — errput: errput prints arrays and nested arrays
- 010 — errput: errput() with no arguments still emits the colour pair
- 011 — errput: errput and output keep separate struct printers
- 012 — output: basic output: strings, ints, floats, bools
- 013 — output: output with expressions: arithmetic inline
- 014 — output: output all supported types in one block
- 015 — output: output() auto-formats each argument by its type
- 016 — string: String constructors and accessors
- 017 — string: assign, push, pop, join and clear
- 018 — string: find, contains, starts_with and ends_with
- 019 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse
- 020 — string: operators: + += == != < > against String and against a literal
- 021 — string: to_int, compare and chained transformations
- 022 — struct: output() prints a struct and its nested struct
- 023 — struct: output() follows a bound 'ref?' and prints null when unbound
- 024 — struct: output() prints [Circular] instead of looping forever
- 025 — struct: output() expands array fields, including 2D
- 026 — output: output and errput print every scalar type in one call
- 027 — input: read two lines from stdin

---

## 001 — calls: output a struct

```ura
struct User:
    a i32

fn main() i32:
    u User
    output(u)
    return 0
```

---

## 002 — function: multi line string

```ura
fn foo(ptr i32):

fn main() i32:
   a i32 = 10
   foo(a)
   output(
        "hello "
        "world",
        12, "\n")
```

---

## 003 — builtin: output picks the spec from each argument

```ura
fn main() i32:
   n i32 = 42
   c i8 = 'z'
   s i8[] = "hi"
   flag bool = n > 1
   output(n, " ", c, " ", s, " ", flag, "\n")
   return 0
```

---

## 004 — builtin: output with literals and escapes

```ura
fn main() i32:
   n i32 = 3
   output("n=", n, "\ttab\n")
   output("no newline")
   output("\n")
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

## 010 — errput: errput() with no arguments still emits the colour pair

```ura
// stderr: [0;31m[0m[0;31mafter the empty call
// expect: 1

fn main() i32:
    errput()
    errput("after the empty call\n")
    return 0
```

---

## 011 — errput: errput and output keep separate struct printers

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

## 012 — output: basic output: strings, ints, floats, bools

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

## 013 — output: output with expressions: arithmetic inline

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

## 014 — output: output all supported types in one block

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

## 015 — output: output() auto-formats each argument by its type

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

## 016 — string: String constructors and accessors

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

## 017 — string: assign, push, pop, join and clear

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

## 018 — string: find, contains, starts_with and ends_with

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

## 019 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse

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

## 020 — string: operators: + += == != < > against String and against a literal

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

## 021 — string: to_int, compare and chained transformations

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

## 022 — struct: output() prints a struct and its nested struct

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

## 023 — struct: output() follows a bound 'ref?' and prints null when unbound

```ura
// stdout: unbound Node{value: 1, next: null}
// stdout: bound   Node{value: 1, next: ref Node{value: 2, next: null}}
// expect: 0

struct Node:
    value i32
    next &Node

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

## 024 — struct: output() prints [Circular] instead of looping forever

```ura
// stdout: cycle Node{value: 1, next: ref Node{value: 2, next: ref [Circular]}}
// stdout: self  Node{value: 2, next: ref Node{value: 1, next: ref [Circular]}}
// expect: 0

struct Node:
    value i32
    next &Node

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

## 025 — struct: output() expands array fields, including 2D

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

---

## 026 — output: output and errput print every scalar type in one call

```ura
// stdout: hp: 100 speed: 1.500000 alive: True grade: S 100%
// stderr: bad value: 7
// expect: 0
fn main() i32:
    hp i32 = 100
    speed f32 = 1.5
    alive bool = True
    grade char = 'S'
    output("hp: ", hp, " speed: ", speed, " alive: ", alive, " grade: ", grade, " 100%\n")
    errput("bad value: ", 7, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %speed = alloca float, align 4
  %alive = alloca i1, align 1
  %grade = alloca i8, align 1
  %arr = alloca [5 x i8], align 1
  %arr1 = alloca [9 x i8], align 1
  %arr2 = alloca [9 x i8], align 1
  %arr3 = alloca [9 x i8], align 1
  %arr4 = alloca [7 x i8], align 1
  %bool.text = alloca i8*, align 8
  %arr5 = alloca [5 x i8], align 1
  %arr6 = alloca [6 x i8], align 1
  %arr8 = alloca [44 x i8], align 1
  %arr9 = alloca [12 x i8], align 1
  %arr10 = alloca [2 x i8], align 1
  %arr11 = alloca [15 x i8], align 1
  store i32 100, i32* %hp, align 4
  store float 1.500000e+00, float* %speed, align 4
  store i1 true, i1* %alive, align 1
  store i8 83, i8* %grade, align 1
  store [5 x i8] c"hp: \00", [5 x i8]* %arr, align 1
  %0 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  store [9 x i8] c" speed: \00", [9 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [9 x i8], [9 x i8]* %arr1, i32 0, i32 0
  store [9 x i8] c" alive: \00", [9 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [9 x i8], [9 x i8]* %arr2, i32 0, i32 0
  store [9 x i8] c" grade: \00", [9 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [9 x i8], [9 x i8]* %arr3, i32 0, i32 0
  store [7 x i8] c" 100%\0A\00", [7 x i8]* %arr4, align 1
  %4 = getelementptr inbounds [7 x i8], [7 x i8]* %arr4, i32 0, i32 0
  %"$hp" = load i32, i32* %hp, align 4
  %"$speed" = load float, float* %speed, align 4
  %AS = fpext float %"$speed" to double
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$alive", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store [5 x i8] c"True\00", [5 x i8]* %arr5, align 1
  %5 = getelementptr inbounds [5 x i8], [5 x i8]* %arr5, i32 0, i32 0
  store i8* %5, i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store [6 x i8] c"False\00", [6 x i8]* %arr6, align 1
  %6 = getelementptr inbounds [6 x i8], [6 x i8]* %arr6, i32 0, i32 0
  store i8* %6, i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$grade" = load i8, i8* %grade, align 1
  %AS7 = sext i8 %"$grade" to i32
  store [44 x i8] c"hp: %d speed: %f alive: %s grade: %c 100%%\0A\00", [44 x i8]* %arr8, align 1
  %7 = getelementptr inbounds [44 x i8], [44 x i8]* %arr8, i32 0, i32 0
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %7, i32 %"$hp", double %AS, i8* %"$bool.text", i32 %AS7)
  store [12 x i8] c"bad value: \00", [12 x i8]* %arr9, align 1
  %8 = getelementptr inbounds [12 x i8], [12 x i8]* %arr9, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr10, align 1
  %9 = getelementptr inbounds [2 x i8], [2 x i8]* %arr10, i32 0, i32 0
  store [15 x i8] c"bad value: %d\0A\00", [15 x i8]* %arr11, align 1
  %10 = getelementptr inbounds [15 x i8], [15 x i8]* %arr11, i32 0, i32 0
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* %10, i32 7)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 027 — input: read two lines from stdin

```ura
// stdin: Aldric
// stdin: 30
// stdout: name? hello Aldric
// stdout: age? you said 30
// expect: 0
fn main() i32:
    output("name? ")
    name i8[] = input()
    output("hello ", name, "\n")
    output("age? ")
    age i8[] = input()
    output("you said ", age, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@stdin = external global i8*

define i32 @main() {
entry:
  %arr = alloca [7 x i8], align 1
  %arr1 = alloca [7 x i8], align 1
  %name = alloca i8*, align 8
  %arr2 = alloca [7 x i8], align 1
  %arr3 = alloca [2 x i8], align 1
  %arr4 = alloca [10 x i8], align 1
  %arr6 = alloca [6 x i8], align 1
  %arr7 = alloca [6 x i8], align 1
  %age = alloca i8*, align 8
  %arr10 = alloca [10 x i8], align 1
  %arr11 = alloca [2 x i8], align 1
  %arr12 = alloca [13 x i8], align 1
  store [7 x i8] c"name? \00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store [7 x i8] c"name? \00", [7 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [7 x i8], [7 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1)
  store i8* null, i8** %name, align 8
  %call.ura.input = call i8* @ura.input()
  store i8* %call.ura.input, i8** %name, align 8
  store [7 x i8] c"hello \00", [7 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [7 x i8], [7 x i8]* %arr2, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [2 x i8], [2 x i8]* %arr3, i32 0, i32 0
  %"$name" = load i8*, i8** %name, align 8
  store [10 x i8] c"hello %s\0A\00", [10 x i8]* %arr4, align 1
  %4 = getelementptr inbounds [10 x i8], [10 x i8]* %arr4, i32 0, i32 0
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %4, i8* %"$name")
  store [6 x i8] c"age? \00", [6 x i8]* %arr6, align 1
  %5 = getelementptr inbounds [6 x i8], [6 x i8]* %arr6, i32 0, i32 0
  store [6 x i8] c"age? \00", [6 x i8]* %arr7, align 1
  %6 = getelementptr inbounds [6 x i8], [6 x i8]* %arr7, i32 0, i32 0
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %6)
  %call.ura.input9 = call i8* @ura.input()
  store i8* %call.ura.input9, i8** %age, align 8
  store [10 x i8] c"you said \00", [10 x i8]* %arr10, align 1
  %7 = getelementptr inbounds [10 x i8], [10 x i8]* %arr10, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr11, align 1
  %8 = getelementptr inbounds [2 x i8], [2 x i8]* %arr11, i32 0, i32 0
  %"$age" = load i8*, i8** %age, align 8
  store [13 x i8] c"you said %s\0A\00", [13 x i8]* %arr12, align 1
  %9 = getelementptr inbounds [13 x i8], [13 x i8]* %arr12, i32 0, i32 0
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %9, i8* %"$age")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)

declare i64 @getline(i8**, i64*, i8*)

declare i8* @strchr(i8*, i32)

declare i8* @strdup(i8*)

define i8* @ura.input() {
entry:
  %line = alloca i8*, align 8
  %cap = alloca i64, align 8
  %len = alloca i64, align 8
  %arr = alloca [1 x i8], align 1
  %newline = alloca i8*, align 8
  store i8* null, i8** %line, align 8
  store i64 0, i64* %cap, align 8
  %"$stdin" = load i8*, i8** @stdin, align 8
  %call.getline = call i64 @getline(i8** %line, i64* %cap, i8* %"$stdin")
  store i64 %call.getline, i64* %len, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"$len" = load i64, i64* %len, align 8
  %LT = icmp slt i64 %"$len", 1
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store [1 x i8] zeroinitializer, [1 x i8]* %arr, align 1
  %0 = getelementptr inbounds [1 x i8], [1 x i8]* %arr, i32 0, i32 0
  %call.strdup = call i8* @strdup(i8* %0)
  ret i8* %call.strdup

endif:                                            ; preds = %if0
  %"$line" = load i8*, i8** %line, align 8
  %call.strchr = call i8* @strchr(i8* %"$line", i32 10)
  store i8* %call.strchr, i8** %newline, align 8
  br label %if1

if1:                                              ; preds = %endif
  %"$newline" = load i8*, i8** %newline, align 8
  %NQ = icmp ne i8* %"$newline", null
  br i1 %NQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"*newline" = load i8*, i8** %newline, align 8
  store i8 0, i8* %"*newline", align 1
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  %"$line2" = load i8*, i8** %line, align 8
  ret i8* %"$line2"
}
```

