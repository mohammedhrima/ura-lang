# errors / structs

## index

- 001 — a field declared twice
- 002 — a field whose type does not exist
- 003 — a struct that contains itself by value
- 004 — two structs that contain each other by value
- 005 — a variable of a type that was never declared
- 006 — reading a field the struct does not have
- 007 — reading a field from a scalar
- 008 — '.len' on a scalar
- 009 — a struct declared in a block is not visible outside it
- 010 — a struct declared twice in the same scope

## 001 — a field declared twice

```ura
// errors/structs/001.ura - a field declared twice

struct Player:
    name chars
    hp   int
    hp   int

main():
    p Player
```

```tree
```

```out
```

```err
error: Field 'hp' is already declared in this struct; rename it or remove the duplicate
  001.ura:6:5
  |
6 |     hp   int
  |     ^^
```

```ll
```

## 002 — a field whose type does not exist

```ura
// errors/structs/002.ura - a field whose type does not exist

struct Dungeon:
    entry Room

main():
    d Dungeon
```

```tree
```

```out
```

```err
error: Unknown type 'Room'
  002.ura:4:5
  |
4 |     entry Room
  |     ^^^^^
```

```ll
```

## 003 — a struct that contains itself by value

```ura
// errors/structs/003.ura - a struct that contains itself by value

struct Node:
    value int
    next  Node

main():
    n Node
```

```tree
```

```out
```

```err
error: Struct Node cannot contain itself by value; use an array or a 'ref?' so the size stays finite
  003.ura:3:1
  |
3 | struct Node:
  | ^^^^^^
```

```ll
```

## 004 — two structs that contain each other by value

```ura
// errors/structs/004.ura - two structs that contain each other by value

struct A:
    b B

struct B:
    a A

main():
    x A
```

```tree
```

```out
```

```err
error: Struct A cannot contain itself by value; use an array or a 'ref?' so the size stays finite
  004.ura:3:1
  |
3 | struct A:
  | ^^^^^^
error: Struct B cannot contain itself by value; use an array or a 'ref?' so the size stays finite
  004.ura:6:1
  |
6 | struct B:
  | ^^^^^^
```

```ll
```

## 005 — a variable of a type that was never declared

```ura
// errors/structs/005.ura - a variable of a type that was never declared

main():
    p Player
```

```tree
```

```out
```

```err
error: Unknown type 'Player'
  005.ura:4:5
  |
4 |     p Player
  |     ^
```

```ll
```

## 006 — reading a field the struct does not have

```ura
// errors/structs/006.ura - reading a field the struct does not have

struct Player:
    name chars
    hp   int

main():
    p Player
    output(p.nope)
```

```tree
```

```out
```

```err
error: Struct Player has no field 'nope'; check the spelling or declare it in the struct
  006.ura:9:13
  |
9 |     output(p.nope)
  |             ^
```

```ll
```

## 007 — reading a field from a scalar

```ura
// errors/structs/007.ura - reading a field from a scalar

main():
    x int = 3
    output(x.foo)
```

```tree
```

```out
```

```err
error: Cannot read '.foo' from int; only a struct has fields
  007.ura:5:13
  |
5 |     output(x.foo)
  |             ^
```

```ll
```

## 008 — '.len' on a scalar

```ura
// errors/structs/008.ura - '.len' on a scalar

main():
    x int = 3
    output(x.len)
```

```tree
```

```out
```

```err
error: '.len' is only valid on an array, not int
  008.ura:5:13
  |
5 |     output(x.len)
  |             ^
```

```ll
```

## 009 — a struct declared in a block is not visible outside it

```ura
// errors/structs/009.ura - a struct declared in a block is not visible outside it

main():
    n int = 1
    if n == 1:
        struct Inner:
            x int

    i Inner
```

```tree
```

```out
```

```err
error: Unknown type 'Inner'
  009.ura:9:5
  |
9 |     i Inner
  |     ^
```

```ll
```

## 010 — a struct declared twice in the same scope

```ura
// errors/structs/010.ura - a struct declared twice in the same scope

struct Player:
    hp int

struct Player:
    mp int

main():
    p Player
```

```tree
```

```out
```

```err
error: Redeclaration of struct 'Player'
  010.ura:6:1
  |
6 | struct Player:
  | ^^^^^^
```

```ll
```
