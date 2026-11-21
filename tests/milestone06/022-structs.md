# milestone06

## index

- 022 — struct: output() prints a struct and its nested struct
- 023 — struct: output() follows a bound 'ref?' and prints null when unbound
- 024 — struct: output() prints [Circular] instead of looping forever
- 025 — struct: output() expands array fields, including 2D
- 026 — output: output and errput print every scalar type in one call
- 027 — input: read two lines from stdin
- 028 — $ strings: values inside {} formatted into a String
- 029 — $ strings: \{ and \} are literal braces
- 030 — operator $: a struct formats itself inside {}
- 031 — operator $: a struct without it can't go inside {}
- 032 — io: io::out, io::err and io::input from ura-core

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i8*, i32 }
%Dungeon = type { i8*, %Room }

@str = private unnamed_addr constant [5 x i8] c"hall\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"flat   \00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"Room{\00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"name: %s\00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c", floor: %d\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@str.7 = private unnamed_addr constant [5 x i8] c"keep\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"gate\00", align 1
@str.9 = private unnamed_addr constant [8 x i8] c"nested \00", align 1
@str.10 = private unnamed_addr constant [9 x i8] c"Dungeon{\00", align 1
@str.11 = private unnamed_addr constant [9 x i8] c"name: %s\00", align 1
@str.12 = private unnamed_addr constant [10 x i8] c", entry: \00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"Room{\00", align 1
@str.14 = private unnamed_addr constant [9 x i8] c"name: %s\00", align 1
@str.15 = private unnamed_addr constant [12 x i8] c", floor: %d\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@str.19 = private unnamed_addr constant [6 x i8] c"two: \00", align 1
@str.20 = private unnamed_addr constant [6 x i8] c"Room{\00", align 1
@str.21 = private unnamed_addr constant [9 x i8] c"name: %s\00", align 1
@str.22 = private unnamed_addr constant [12 x i8] c", floor: %d\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.24 = private unnamed_addr constant [6 x i8] c" and \00", align 1
@str.25 = private unnamed_addr constant [6 x i8] c"Room{\00", align 1
@str.26 = private unnamed_addr constant [9 x i8] c"name: %s\00", align 1
@str.27 = private unnamed_addr constant [12 x i8] c", floor: %d\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.29 = private unnamed_addr constant [7 x i8] c" done\0A\00", align 1

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  %d = alloca %Dungeon, align 8
  store %Room zeroinitializer, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %r.name, align 8
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  store i32 2, i32* %r.floor, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0))
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  %r.name2 = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.name" = load i8*, i8** %r.name2, align 8
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %"$r.name")
  %r.floor4 = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  %"$r.floor" = load i32, i32* %r.floor4, align 4
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i32 %"$r.floor")
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %d.name = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0), i8** %d.name, align 8
  %d.entry = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.name = getelementptr inbounds %Room, %Room* %d.entry, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0), i8** %d.entry.name, align 8
  %d.entry8 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.floor = getelementptr inbounds %Room, %Room* %d.entry8, i32 0, i32 1
  store i32 1, i32* %d.entry.floor, align 4
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.9, i32 0, i32 0))
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.10, i32 0, i32 0))
  %d.name11 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  %"$d.name" = load i8*, i8** %d.name11, align 8
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.11, i32 0, i32 0), i8* %"$d.name")
  %d.entry13 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %call.dprintf14 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0))
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0))
  %d.entry.name16 = getelementptr inbounds %Room, %Room* %d.entry13, i32 0, i32 0
  %"$d.entry.name" = load i8*, i8** %d.entry.name16, align 8
  %call.dprintf17 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.14, i32 0, i32 0), i8* %"$d.entry.name")
  %d.entry.floor18 = getelementptr inbounds %Room, %Room* %d.entry13, i32 0, i32 1
  %"$d.entry.floor" = load i32, i32* %d.entry.floor18, align 4
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.15, i32 0, i32 0), i32 %"$d.entry.floor")
  %call.dprintf20 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0))
  %call.dprintf22 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %call.dprintf23 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0))
  %call.dprintf24 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.20, i32 0, i32 0))
  %r.name25 = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.name26" = load i8*, i8** %r.name25, align 8
  %call.dprintf27 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.21, i32 0, i32 0), i8* %"$r.name26")
  %r.floor28 = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  %"$r.floor29" = load i32, i32* %r.floor28, align 4
  %call.dprintf30 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.22, i32 0, i32 0), i32 %"$r.floor29")
  %call.dprintf31 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.24, i32 0, i32 0))
  %d.entry33 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %call.dprintf34 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.25, i32 0, i32 0))
  %d.entry.name35 = getelementptr inbounds %Room, %Room* %d.entry33, i32 0, i32 0
  %"$d.entry.name36" = load i8*, i8** %d.entry.name35, align 8
  %call.dprintf37 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.26, i32 0, i32 0), i8* %"$d.entry.name36")
  %d.entry.floor38 = getelementptr inbounds %Room, %Room* %d.entry33, i32 0, i32 1
  %"$d.entry.floor39" = load i32, i32* %d.entry.floor38, align 4
  %call.dprintf40 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.27, i32 0, i32 0), i32 %"$d.entry.floor39")
  %call.dprintf41 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %call.dprintf42 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.29, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [44 x i8] c"hp: %d speed: %f alive: %s grade: %c 100%%\0A\00", align 1
@str.3 = private unnamed_addr constant [15 x i8] c"bad value: %d\0A\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %speed = alloca float, align 4
  %alive = alloca i1, align 1
  %grade = alloca i8, align 1
  %bool.text = alloca i8*, align 8
  store i32 100, i32* %hp, align 4
  store float 1.500000e+00, float* %speed, align 4
  store i1 true, i1* %alive, align 1
  store i8 83, i8* %grade, align 1
  %"$hp" = load i32, i32* %hp, align 4
  %"$speed" = load float, float* %speed, align 4
  %AS = fpext float %"$speed" to double
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$alive", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$grade" = load i8, i8* %grade, align 1
  %AS1 = sext i8 %"$grade" to i32
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([44 x i8], [44 x i8]* @str.2, i32 0, i32 0), i32 %"$hp", double %AS, i8* %"$bool.text", i32 %AS1)
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i32 0, i32 0), i32 7)
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

@ura.read_file = global i8* null
@ura.read_fd = global i32 0

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
  %call.ura.read = call i8* @ura.read(i32 0)
  store i8* %call.ura.read, i8** %name, align 8
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
  %call.ura.read9 = call i8* @ura.read(i32 0)
  store i8* %call.ura.read9, i8** %age, align 8
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

declare i8* @fdopen(i32, i8*)

define i8* @ura.read(i32 %0) {
entry:
  %fd = alloca i32, align 4
  %arr = alloca [2 x i8], align 1
  %line = alloca i8*, align 8
  %cap = alloca i64, align 8
  %len = alloca i64, align 8
  %arr6 = alloca [1 x i8], align 1
  %newline = alloca i8*, align 8
  store i32 %0, i32* %fd, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$ura.read_file" = load i8*, i8** @ura.read_file, align 8
  %EQ = icmp eq i8* %"$ura.read_file", null
  %"$fd" = load i32, i32* %fd, align 4
  %"$ura.read_fd" = load i32, i32* @ura.read_fd, align 4
  %NQ = icmp ne i32 %"$fd", %"$ura.read_fd"
  %OR = or i1 %EQ, %NQ
  br i1 %OR, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store [2 x i8] c"r\00", [2 x i8]* %arr, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"$fd3" = load i32, i32* %fd, align 4
  %call.fdopen = call i8* @fdopen(i32 %"$fd3", i8* %1)
  store i8* %call.fdopen, i8** @ura.read_file, align 8
  %"$fd4" = load i32, i32* %fd, align 4
  store i32 %"$fd4", i32* @ura.read_fd, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  store i8* null, i8** %line, align 8
  store i64 0, i64* %cap, align 8
  %"$ura.read_file5" = load i8*, i8** @ura.read_file, align 8
  %call.getline = call i64 @getline(i8** %line, i64* %cap, i8* %"$ura.read_file5")
  store i64 %call.getline, i64* %len, align 8
  br label %if1

if1:                                              ; preds = %endif
  %"$len" = load i64, i64* %len, align 8
  %LT = icmp slt i64 %"$len", 1
  br i1 %LT, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  store [1 x i8] zeroinitializer, [1 x i8]* %arr6, align 1
  %2 = getelementptr inbounds [1 x i8], [1 x i8]* %arr6, i32 0, i32 0
  %call.strdup = call i8* @strdup(i8* %2)
  ret i8* %call.strdup

endif1:                                           ; preds = %if1
  %"$line" = load i8*, i8** %line, align 8
  %call.strchr = call i8* @strchr(i8* %"$line", i32 10)
  store i8* %call.strchr, i8** %newline, align 8
  br label %if2

if2:                                              ; preds = %endif1
  %"$newline" = load i8*, i8** %newline, align 8
  %NQ7 = icmp ne i8* %"$newline", null
  br i1 %NQ7, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*newline" = load i8*, i8** %newline, align 8
  store i8 0, i8* %"*newline", align 1
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$line8" = load i8*, i8** %line, align 8
  ret i8* %"$line8"
}
```
---

## 028 — $ strings: values inside {} formatted into a String

```ura
// stdout: test.ura:7 hp 10 2.500000 True c
// stdout: name: ura, 浦
// expect: 0
fn main() i32:
    hp i32 = 10
    name String = "ura"
    s String = $"{ura::filename}:{ura::line} hp {hp} {2.5f} {hp == 10} {'c'}"
    output(s, "\n")
    bay unic = '浦'
    output($"name: {name}, {bay}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [27 x i8] c"test.ura:%d hp %d %f %s %c\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.5 = private unnamed_addr constant [14 x i8] c"name: %s, %s\0A\00", align 1
@str.6 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %name = alloca %String, align 8
  %s = alloca %String, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %bay = alloca i21, align 4
  %format.size9 = alloca i32, align 4
  %arr = alloca [5 x i8], align 1
  %format.text15 = alloca i8*, align 8
  %tmp = alloca %String, align 8
  store i32 10, i32* %hp, align 4
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 }, %String* %name, align 8
  store %String zeroinitializer, %String* %s, align 8
  %"$hp" = load i32, i32* %hp, align 4
  %EQ = icmp eq i32 %"$hp", 10
  store i32 0, i32* %format.size, align 4
  %"$hp1" = load i32, i32* %hp, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %EQ, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str.3, i32 0, i32 0), i32 7, i32 %"$hp1", double 2.500000e+00, i8* %"$bool.text", i32 99)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text2" = load i8*, i8** %bool.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str.3, i32 0, i32 0), i32 7, i32 %"$hp1", double 2.500000e+00, i8* %"$bool.text2", i32 99)
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  store %String %2, %String* %s, align 8
  %call.String.chars = call i8* @String.chars(%String* %s)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars)
  store i21 28006, i21* %bay, align 4
  %call.String.chars8 = call i8* @String.chars(%String* %name)
  store i32 0, i32* %format.size9, align 4
  %"$bay" = load i21, i21* %bay, align 4
  store [5 x i8] zeroinitializer, [5 x i8]* %arr, align 1
  %3 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  %AS10 = sext i21 %"$bay" to i32
  %call.ura.unic = call i8* @ura.unic(i32 %AS10, i8* %3)
  %call.snprintf11 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars8, i8* %call.ura.unic)
  store i32 %call.snprintf11, i32* %format.size9, align 4
  %"$format.size12" = load i32, i32* %format.size9, align 4
  %ADD13 = add i32 %"$format.size12", 1
  %AS14 = sext i32 %ADD13 to i64
  %call.calloc16 = call i8* @calloc(i64 %AS14, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc16, i8** %format.text15, align 8
  %"$format.text17" = load i8*, i8** %format.text15, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text17", i64 %AS14, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars8, i8* %call.ura.unic)
  %"$format.size19" = load i32, i32* %format.size9, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %"$format.text21" = load i8*, i8** %format.text15, align 8
  %"$format.size22" = load i32, i32* %format.size9, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text21", 0
  %5 = insertvalue %String %4, i32 %"$format.size22", 1
  %6 = insertvalue %String %5, i32 %ADD20, 2
  store %String %6, %String* %tmp, align 8
  %call.String.chars23 = call i8* @String.chars(%String* %tmp)
  %call.dprintf24 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.6, i32 0, i32 0), i8* %call.String.chars23)
  call void @String.drop(%String* %tmp)
  call void @String.drop(%String* %s)
  call void @String.drop(%String* %name)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)

declare i32 @snprintf(i8*, i64, i8*, ...)

define i8* @ura.unic(i32 %0, i8* %1) {
entry:
  %code = alloca i32, align 4
  %text = alloca i8*, align 8
  store i32 %0, i32* %code, align 4
  store i8* %1, i8** %text, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"$code" = load i32, i32* %code, align 4
  %LT = icmp slt i32 %"$code", 128
  br i1 %LT, label %if8.then, label %elif9

if8.then:                                         ; preds = %if8
  %"$code1" = load i32, i32* %code, align 4
  %DIV = sdiv i32 %"$code1", 1
  %ADD = add i32 %DIV, 0
  %MOD = srem i32 %DIV, 64
  %ADD2 = add i32 %MOD, 128
  %AS = trunc i32 %ADD to i8
  %"$text" = load i8*, i8** %text, align 8
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 0
  store i8 %AS, i8* %"text[]", align 1
  %"$text3" = load i8*, i8** %text, align 8
  %"text[]4" = getelementptr inbounds i8, i8* %"$text3", i32 1
  store i8 0, i8* %"text[]4", align 1
  br label %endif

elif9:                                            ; preds = %if8
  %"$code5" = load i32, i32* %code, align 4
  %LT6 = icmp slt i32 %"$code5", 2048
  br i1 %LT6, label %elif9.then, label %elif10

elif9.then:                                       ; preds = %elif9
  %"$code7" = load i32, i32* %code, align 4
  %DIV8 = sdiv i32 %"$code7", 1
  %ADD9 = add i32 %DIV8, 192
  %MOD10 = srem i32 %DIV8, 64
  %ADD11 = add i32 %MOD10, 128
  %AS12 = trunc i32 %ADD11 to i8
  %"$text13" = load i8*, i8** %text, align 8
  %"text[]14" = getelementptr inbounds i8, i8* %"$text13", i32 1
  store i8 %AS12, i8* %"text[]14", align 1
  %"$code15" = load i32, i32* %code, align 4
  %DIV16 = sdiv i32 %"$code15", 64
  %ADD17 = add i32 %DIV16, 192
  %MOD18 = srem i32 %DIV16, 64
  %ADD19 = add i32 %MOD18, 128
  %AS20 = trunc i32 %ADD17 to i8
  %"$text21" = load i8*, i8** %text, align 8
  %"text[]22" = getelementptr inbounds i8, i8* %"$text21", i32 0
  store i8 %AS20, i8* %"text[]22", align 1
  %"$text23" = load i8*, i8** %text, align 8
  %"text[]24" = getelementptr inbounds i8, i8* %"$text23", i32 2
  store i8 0, i8* %"text[]24", align 1
  br label %endif

elif10:                                           ; preds = %elif9
  %"$code25" = load i32, i32* %code, align 4
  %LT26 = icmp slt i32 %"$code25", 65536
  br i1 %LT26, label %elif10.then, label %elif11

elif10.then:                                      ; preds = %elif10
  %"$code27" = load i32, i32* %code, align 4
  %DIV28 = sdiv i32 %"$code27", 1
  %ADD29 = add i32 %DIV28, 224
  %MOD30 = srem i32 %DIV28, 64
  %ADD31 = add i32 %MOD30, 128
  %AS32 = trunc i32 %ADD31 to i8
  %"$text33" = load i8*, i8** %text, align 8
  %"text[]34" = getelementptr inbounds i8, i8* %"$text33", i32 2
  store i8 %AS32, i8* %"text[]34", align 1
  %"$code35" = load i32, i32* %code, align 4
  %DIV36 = sdiv i32 %"$code35", 64
  %ADD37 = add i32 %DIV36, 224
  %MOD38 = srem i32 %DIV36, 64
  %ADD39 = add i32 %MOD38, 128
  %AS40 = trunc i32 %ADD39 to i8
  %"$text41" = load i8*, i8** %text, align 8
  %"text[]42" = getelementptr inbounds i8, i8* %"$text41", i32 1
  store i8 %AS40, i8* %"text[]42", align 1
  %"$code43" = load i32, i32* %code, align 4
  %DIV44 = sdiv i32 %"$code43", 4096
  %ADD45 = add i32 %DIV44, 224
  %MOD46 = srem i32 %DIV44, 64
  %ADD47 = add i32 %MOD46, 128
  %AS48 = trunc i32 %ADD45 to i8
  %"$text49" = load i8*, i8** %text, align 8
  %"text[]50" = getelementptr inbounds i8, i8* %"$text49", i32 0
  store i8 %AS48, i8* %"text[]50", align 1
  %"$text51" = load i8*, i8** %text, align 8
  %"text[]52" = getelementptr inbounds i8, i8* %"$text51", i32 3
  store i8 0, i8* %"text[]52", align 1
  br label %endif

elif11:                                           ; preds = %elif10
  %"$code53" = load i32, i32* %code, align 4
  %LT54 = icmp slt i32 %"$code53", 1114112
  br i1 %LT54, label %elif11.then, label %endif

elif11.then:                                      ; preds = %elif11
  %"$code55" = load i32, i32* %code, align 4
  %DIV56 = sdiv i32 %"$code55", 1
  %ADD57 = add i32 %DIV56, 240
  %MOD58 = srem i32 %DIV56, 64
  %ADD59 = add i32 %MOD58, 128
  %AS60 = trunc i32 %ADD59 to i8
  %"$text61" = load i8*, i8** %text, align 8
  %"text[]62" = getelementptr inbounds i8, i8* %"$text61", i32 3
  store i8 %AS60, i8* %"text[]62", align 1
  %"$code63" = load i32, i32* %code, align 4
  %DIV64 = sdiv i32 %"$code63", 64
  %ADD65 = add i32 %DIV64, 240
  %MOD66 = srem i32 %DIV64, 64
  %ADD67 = add i32 %MOD66, 128
  %AS68 = trunc i32 %ADD67 to i8
  %"$text69" = load i8*, i8** %text, align 8
  %"text[]70" = getelementptr inbounds i8, i8* %"$text69", i32 2
  store i8 %AS68, i8* %"text[]70", align 1
  %"$code71" = load i32, i32* %code, align 4
  %DIV72 = sdiv i32 %"$code71", 4096
  %ADD73 = add i32 %DIV72, 240
  %MOD74 = srem i32 %DIV72, 64
  %ADD75 = add i32 %MOD74, 128
  %AS76 = trunc i32 %ADD75 to i8
  %"$text77" = load i8*, i8** %text, align 8
  %"text[]78" = getelementptr inbounds i8, i8* %"$text77", i32 1
  store i8 %AS76, i8* %"text[]78", align 1
  %"$code79" = load i32, i32* %code, align 4
  %DIV80 = sdiv i32 %"$code79", 262144
  %ADD81 = add i32 %DIV80, 240
  %MOD82 = srem i32 %DIV80, 64
  %ADD83 = add i32 %MOD82, 128
  %AS84 = trunc i32 %ADD81 to i8
  %"$text85" = load i8*, i8** %text, align 8
  %"text[]86" = getelementptr inbounds i8, i8* %"$text85", i32 0
  store i8 %AS84, i8* %"text[]86", align 1
  %"$text87" = load i8*, i8** %text, align 8
  %"text[]88" = getelementptr inbounds i8, i8* %"$text87", i32 4
  store i8 0, i8* %"text[]88", align 1
  br label %endif

endif:                                            ; preds = %elif11.then, %elif11, %elif10.then, %elif9.then, %if8.then
  %"$text89" = load i8*, i8** %text, align 8
  ret i8* %"$text89"
}
```

---

## 029 — $ strings: \{ and \} are literal braces

```ura
// stdout: set { x: 5 }
// stdout: {}
// expect: 0
fn main() i32:
    x i32 = 5
    output($"set \{ x: {x} \}\n")
    output("\{\}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [15 x i8] c"set { x: %d }\0A\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"{}\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp = alloca %String, align 8
  store i32 5, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$x")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$x")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  store %String %2, %String* %tmp, align 8
  %call.String.chars = call i8* @String.chars(%String* %tmp)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  call void @String.drop(%String* %tmp)
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 030 — operator $: a struct formats itself inside {}

```ura
// stdout: p = (1, 2)
// stdout: line = (1, 2) -> (3, 4)
// stdout: 0 (0, 0)
// stdout: 1 (1, 1)
// expect: 0
struct Point:
    x i32
    y i32
    operator $() String:
        return $"({self.x}, {self.y})"

struct Line:
    a Point
    b Point
    operator $() String:
        return $"{self.a} -> {self.b}"

fn main() i32:
    p Point
    p.x = 1
    p.y = 2
    output($"p = {p}\n")
    l Line
    l.a = p
    l.b.x = 3
    l.b.y = 4
    output($"line = {l}\n")
    i i32 = 0
    while i < 2:
        q Point
        q.x = i
        q.y = i
        output($"{i} {q}\n")
        i += 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Point = type { i32, i32 }
%Line = type { %Point, %Point }

@str = private unnamed_addr constant [9 x i8] c"(%d, %d)\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"%s -> %s\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"p = %s\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"line = %s\0A\00", align 1
@str.5 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.6 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@str.7 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

define %String @"Point.$"(%Point* %0) {
entry:
  %self = alloca %Point*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Point* %0, %Point** %self, align 8
  %"*self" = load %Point*, %Point** %self, align 8
  %"*self.x" = getelementptr inbounds %Point, %Point* %"*self", i32 0, i32 0
  %"*self1" = load %Point*, %Point** %self, align 8
  %"*self.y" = getelementptr inbounds %Point, %Point* %"*self1", i32 0, i32 1
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %2 = insertvalue %String %1, i32 %"$format.size6", 1
  %3 = insertvalue %String %2, i32 %ADD4, 2
  ret %String %3
}

define %String @"Line.$"(%Line* %0) {
entry:
  %self = alloca %Line*, align 8
  %tmp = alloca %String, align 8
  %tmp3 = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Line* %0, %Line** %self, align 8
  %"*self" = load %Line*, %Line** %self, align 8
  %"*self.a" = getelementptr inbounds %Line, %Line* %"*self", i32 0, i32 0
  %"call.Point.$" = call %String @"Point.$"(%Point* %"*self.a")
  store %String %"call.Point.$", %String* %tmp, align 8
  %call.String.chars = call i8* @String.chars(%String* %tmp)
  %"*self1" = load %Line*, %Line** %self, align 8
  %"*self.b" = getelementptr inbounds %Line, %Line* %"*self1", i32 0, i32 1
  %"call.Point.$2" = call %String @"Point.$"(%Point* %"*self.b")
  store %String %"call.Point.$2", %String* %tmp3, align 8
  %call.String.chars4 = call i8* @String.chars(%String* %tmp3)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars, i8* %call.String.chars4)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars, i8* %call.String.chars4)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %2 = insertvalue %String %1, i32 %"$format.size9", 1
  %3 = insertvalue %String %2, i32 %ADD7, 2
  call void @String.drop(%String* %tmp3)
  call void @String.drop(%String* %tmp)
  ret %String %3
}

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %tmp = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp6 = alloca %String, align 8
  %l = alloca %Line, align 8
  %tmp9 = alloca %String, align 8
  %format.size11 = alloca i32, align 4
  %format.text16 = alloca i8*, align 8
  %tmp24 = alloca %String, align 8
  %i = alloca i32, align 4
  %q = alloca %Point, align 8
  %tmp30 = alloca %String, align 8
  %format.size32 = alloca i32, align 4
  %format.text38 = alloca i8*, align 8
  %tmp46 = alloca %String, align 8
  store %Point zeroinitializer, %Point* %p, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  store i32 1, i32* %p.x, align 4
  %p.y = getelementptr inbounds %Point, %Point* %p, i32 0, i32 1
  store i32 2, i32* %p.y, align 4
  %"call.Point.$" = call %String @"Point.$"(%Point* %p)
  store %String %"call.Point.$", %String* %tmp, align 8
  %call.String.chars = call i8* @String.chars(%String* %tmp)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  store %String %2, %String* %tmp6, align 8
  %call.String.chars7 = call i8* @String.chars(%String* %tmp6)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars7)
  call void @String.drop(%String* %tmp6)
  call void @String.drop(%String* %tmp)
  store %Line zeroinitializer, %Line* %l, align 4
  %l.a = getelementptr inbounds %Line, %Line* %l, i32 0, i32 0
  %"$p" = load %Point, %Point* %p, align 4
  store %Point %"$p", %Point* %l.a, align 4
  %l.b = getelementptr inbounds %Line, %Line* %l, i32 0, i32 1
  %l.b.x = getelementptr inbounds %Point, %Point* %l.b, i32 0, i32 0
  store i32 3, i32* %l.b.x, align 4
  %l.b8 = getelementptr inbounds %Line, %Line* %l, i32 0, i32 1
  %l.b.y = getelementptr inbounds %Point, %Point* %l.b8, i32 0, i32 1
  store i32 4, i32* %l.b.y, align 4
  %"call.Line.$" = call %String @"Line.$"(%Line* %l)
  store %String %"call.Line.$", %String* %tmp9, align 8
  %call.String.chars10 = call i8* @String.chars(%String* %tmp9)
  %call.snprintf12 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars10)
  store i32 %call.snprintf12, i32* %format.size11, align 4
  %"$format.size13" = load i32, i32* %format.size11, align 4
  %ADD14 = add i32 %"$format.size13", 1
  %AS15 = sext i32 %ADD14 to i64
  %call.calloc17 = call i8* @calloc(i64 %AS15, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc17, i8** %format.text16, align 8
  %"$format.text18" = load i8*, i8** %format.text16, align 8
  %call.snprintf19 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text18", i64 %AS15, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars10)
  %"$format.size20" = load i32, i32* %format.size11, align 4
  %ADD21 = add i32 %"$format.size20", 1
  %"$format.text22" = load i8*, i8** %format.text16, align 8
  %"$format.size23" = load i32, i32* %format.size11, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text22", 0
  %4 = insertvalue %String %3, i32 %"$format.size23", 1
  %5 = insertvalue %String %4, i32 %ADD21, 2
  store %String %5, %String* %tmp24, align 8
  %call.String.chars25 = call i8* @String.chars(%String* %tmp24)
  %call.dprintf26 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars25)
  call void @String.drop(%String* %tmp24)
  call void @String.drop(%String* %tmp9)
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 2
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  store %Point zeroinitializer, %Point* %q, align 4
  %q.x = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  %"$i27" = load i32, i32* %i, align 4
  store i32 %"$i27", i32* %q.x, align 4
  %q.y = getelementptr inbounds %Point, %Point* %q, i32 0, i32 1
  %"$i28" = load i32, i32* %i, align 4
  store i32 %"$i28", i32* %q.y, align 4
  %"call.Point.$29" = call %String @"Point.$"(%Point* %q)
  store %String %"call.Point.$29", %String* %tmp30, align 8
  %call.String.chars31 = call i8* @String.chars(%String* %tmp30)
  %"$i33" = load i32, i32* %i, align 4
  %call.snprintf34 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.6, i32 0, i32 0), i32 %"$i33", i8* %call.String.chars31)
  store i32 %call.snprintf34, i32* %format.size32, align 4
  %"$format.size35" = load i32, i32* %format.size32, align 4
  %ADD36 = add i32 %"$format.size35", 1
  %AS37 = sext i32 %ADD36 to i64
  %call.calloc39 = call i8* @calloc(i64 %AS37, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc39, i8** %format.text38, align 8
  %"$format.text40" = load i8*, i8** %format.text38, align 8
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text40", i64 %AS37, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.6, i32 0, i32 0), i32 %"$i33", i8* %call.String.chars31)
  %"$format.size42" = load i32, i32* %format.size32, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %"$format.text44" = load i8*, i8** %format.text38, align 8
  %"$format.size45" = load i32, i32* %format.size32, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text44", 0
  %7 = insertvalue %String %6, i32 %"$format.size45", 1
  %8 = insertvalue %String %7, i32 %ADD43, 2
  store %String %8, %String* %tmp46, align 8
  %call.String.chars47 = call i8* @String.chars(%String* %tmp46)
  %call.dprintf48 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.7, i32 0, i32 0), i8* %call.String.chars47)
  call void @String.drop(%String* %tmp46)
  call void @String.drop(%String* %tmp30)
  %"$i49" = load i32, i32* %i, align 4
  %ADD50 = add i32 %"$i49", 1
  store i32 %ADD50, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 031 — operator $: a struct without it can't go inside {}

```ura
// error: 'Point' has no operator '$'
struct Point:
    x i32

fn main() i32:
    p Point
    output($"p = {p}\n")
    return 0
```

### stderr

```
error: test.ura:7:19 'Point' has no operator '$'
  |
7 |     output($"p = {p}\n")
  |                   ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 032 — io: io::out, io::err and io::input from ura-core

```ura
// stdin: Aldric
// stdout: name? hello Aldric
// stdout: 3 letters? no, 6
// expect: 0
fn main() i32:
    io::out("name? ")
    name String = io::input()
    io::out($"hello {name}\n")
    io::err("this goes to stderr\n")
    io::out($"3 letters? no, {name.len()}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [7 x i8] c"name? \00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"hello %s\0A\00", align 1
@str.2 = private unnamed_addr constant [21 x i8] c"this goes to stderr\0A\00", align 1
@str.3 = private unnamed_addr constant [19 x i8] c"3 letters? no, %d\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if5

if5:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if6

if6:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

declare i64 @read(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define void @"io::err"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 2, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define %String @"io::input"() {
entry:
  %line = alloca %String, align 8
  %c = alloca i8*, align 8
  store %String zeroinitializer, %String* %line, align 8
  %"call.new<i8>" = call i8* @"new<i8>"(i32 1)
  store i8* %"call.new<i8>", i8** %c, align 8
  br label %while7

while7:                                           ; preds = %while7.then, %entry
  %"$c" = load i8*, i8** %c, align 8
  %call.read = call i64 @read(i32 0, i8* %"$c", i64 1)
  %EQ = icmp eq i64 %call.read, 1
  %"$c1" = load i8*, i8** %c, align 8
  %"c[]" = getelementptr inbounds i8, i8* %"$c1", i32 0
  %"$c[]" = load i8, i8* %"c[]", align 1
  %NQ = icmp ne i8 %"$c[]", 10
  %AND = and i1 %EQ, %NQ
  br i1 %AND, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"$c2" = load i8*, i8** %c, align 8
  %"c[]3" = getelementptr inbounds i8, i8* %"$c2", i32 0
  %"$c[]4" = load i8, i8* %"c[]3", align 1
  call void @String.append(%String* %line, i8 %"$c[]4")
  br label %while7

endloop:                                          ; preds = %while7
  %"$c5" = load i8*, i8** %c, align 8
  call void @"free<i8>"(i8* %"$c5")
  %"$line" = load %String, %String* %line, align 8
  ret %String %"$line"
}

define i32 @main() {
entry:
  %name = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 6, i32 0 })
  %"call.io::input" = call %String @"io::input"()
  store %String %"call.io::input", %String* %name, align 8
  %call.String.chars = call i8* @String.chars(%String* %name)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  call void @"io::err"(%String { i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i32 20, i32 0 })
  %call.String.len = call i32 @String.len(%String* %name)
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.3, i32 0, i32 0), i32 %call.String.len)
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.3, i32 0, i32 0), i32 %call.String.len)
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %4 = insertvalue %String %3, i32 %"$format.size18", 1
  %5 = insertvalue %String %4, i32 %ADD16, 2
  call void @"io::out"(%String %5)
  call void @String.drop(%String* %name)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

