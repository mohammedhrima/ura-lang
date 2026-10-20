# milestone10

## index

- 001 — loop: loop (infinite) with break + continue
- 002 — loop: range-for both directions (a..b)
- 003 — loop: array-for iterates elements by value
- 004 — loop: for ref mutates array elements in place
- 005 — loop: for without a loop variable
- 006 — loop: for over a non-iterable
- 007 — loop: for ref over a range
- 008 — loop: for over struct-array and 2D-array struct fields
- 009 — loop: by sets the step of a range
- 010 — loop: a by step must be a positive integer on a range
- 011 — loop: by needs a range on its left
- 012 — loop: break and continue inside a for
- 013 — loop: continue: skip wall tiles '#', count open spaces

---

## 001 — loop: loop (infinite) with break + continue

```ura
// stdout: 1 2 4 5
// expect: 0

fn main() i32:
    i i32 = 0
    loop:
        i = i + 1
        if i == 3:
            continue
        if i == 6:
            break
        output(i, " ")
    output("\n")
    return 0
```

---

## 002 — loop: range-for both directions (a..b)

```ura
// stdout: 0 1 2 3 4 | 5 4 3 2 1
// expect: 0

fn main() i32:
    for i in 0..5:
        output(i, " ")
    output("| ")
    for i in 5..0:
        output(i, " ")
    output("\n")
    return 0
```

---

## 003 — loop: array-for iterates elements by value

```ura
// stdout: sum = 60
// expect: 0

fn main() i32:
    nums i32[] = [10, 20, 30]
    total i32 = 0
    for x in nums:
        total = total + x
    output("sum = ", total, "\n")
    return 0
```

---

## 004 — loop: for ref mutates array elements in place

```ura
// stdout: 2 4 6 8
// expect: 0

fn main() i32:
    nums i32[] = [1, 2, 3, 4]
    for x &in nums:
        x = x * 2
    for x in nums:
        output(x, " ")
    output("\n")
    return 0
```

---

## 005 — loop: for without a loop variable

```ura
// error: Expected a loop variable after 'for'
fn main() i32:
    for in 0..5:
        output(1)
    return 0
```

---

## 006 — loop: for over a non-iterable

```ura
// error: 'for i in ...' expects a range (a..b) or an array
fn main() i32:
    x i32 = 5
    for i in x:
        output(i)
    return 0
```

---

## 007 — loop: for ref over a range

```ura
// error: 'for ref' needs an array; a range yields values, not storage
fn main() i32:
    for i &in 0..5:
        output(i)
    return 0
```

---

## 008 — loop: for over struct-array and 2D-array struct fields

```ura
// stdout: 0 0 0
// stdout: 0 0 9
// stdout: 0 1 2
// stdout: 0 10 20
// expect: 0

struct Room:
    floor i32

struct World:
    grid  i32[][]
    rooms Room[]

    pub fn create() World:
        w World
        w.grid  = new i32[2][3]
        w.rooms = new Room[3]
        i i32 = 0
        while i < 3:
            w.rooms[i].floor = i
            i += 1
        w.grid[1][2] = 9
        return w

    // nested for over a 2D field: 'row' must be typed i32[]
    fn show_grid():
        for row in self.grid:
            for v in row:
                output(v, " ")
            output("\n")

    // for over &a struct-array field mutates in place
    fn bump():
        for r in self.rooms:
            output(r.floor, " ")
        output("\n")
        for r &in self.rooms:
            r.floor = r.floor * 10
        for r in self.rooms:
            output(r.floor, " ")
        output("\n")

fn main() i32:
    w World = World::create()
    w.show_grid()
    w.bump()
    clean w.grid
    clean w.rooms
    return 0
```

---

## 009 — loop: by sets the step of a range

```ura
// stdout: 0 3 6 9
// stdout: 10 7 4 1
// stdout: 0 4 8 12 16
// stdout: 0
// expect: 0

fn main() i32:
    for i in 0..10 by 3:
        output(i, " ")
    output("\n")
    for i in 10..0 by 3:
        output(i, " ")
    output("\n")
    n i32 = 4
    for i in 0..20 by n:
        output(i, " ")
    output("\n")
    // a step wider than the span yields the start only
    for i in 0..3 by 10:
        output(i, " ")
    output("\n")
    return 0
```

---

## 010 — loop: a by step must be a positive integer on a range

```ura
// error: The 'by' step must be positive; a range counts down when its start is greater than its end

fn main() i32:
    for i in 0..10 by 0:
        output(i)
    return 0
```

---

## 011 — loop: by needs a range on its left

```ura
// error: 'by' sets the step of a range (a..b), so it needs one on its left

fn main() i32:
    xs i32[] = [1, 2, 3]
    for x in xs by 2:
        output(x)
    return 0
```

---

## 012 — loop: break and continue inside a for

```ura
// stdout: break:    0 1 2 3
// stdout: continue: 1 3 5 7
// stdout: nested:   00 01 10 11 20 21
// expect: 0

fn main() i32:
    output("break:    ")
    for i in 0..10:
        if i == 4:
            break
        output(i, " ")

    output("\ncontinue: ")
    for i in 0..8:
        if i % 2 == 0:
            continue
        output(i, " ")

    // break leaves only the inner for
    output("\nnested:   ")
    for i in 0..3:
        for j in 0..3:
            if j == 2:
                break
            output(i, j, " ")
    output("\n")
    return 0
```

---

## 013 — loop: continue: skip wall tiles '#', count open spaces

```ura
// stdout: Open tiles: 6
// expect: 0

fn main() i32:
    tiles char[] = ['#', '.', '#', '.', '.',
                    '#', '.', '@', '.', '#']
    open i32 = 0
    i    u64 = 0
    while i < tiles.len:
        if tiles[i] == '#':
            i = i + 1
            continue
        open = open + 1
        i = i + 1
    output("Open tiles: ", open, "\n")
    return 0
```

