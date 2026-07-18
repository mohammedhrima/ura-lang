# errors / arrays

## index

- 001 — indexing a non-array
- 002 — non-integer index
- 003 — array literal with mixed element types
- 004 — array size is not an integer
- 005 — clean on a non-array
- 006 — new without an array type
- 007 — missing closing bracket on index
- 008 — empty array literal has no element type
- 009 — range bounds must be integers
- 010 — .len on a non-array
- 011 — unknown member

## 001 — indexing a non-array

```ura
main():
    a int = 5
    output(a[0])
```

```tree
```

```out
```

```err
error: Cannot index 'int', it is not an array
  001.ura:3:13
  |
3 |     output(a[0])
  |             ^
```

```ll
```

## 002 — non-integer index

```ura
main():
    a int[] = [1, 2, 3]
    output(a[1.5])
```

```tree
```

```out
```

```err
error: Array index must be an integer, got float
  002.ura:3:13
  |
3 |     output(a[1.5])
  |             ^
```

```ll
```

## 003 — array literal with mixed element types

```ura
main():
    a int[] = [1, True, 3]
```

```tree
```

```out
```

```err
error: Array elements must all be the same type
  003.ura:2:19
  |
2 |     a int[] = [1, True, 3]
  |                   ^^^^
```

```ll
```

## 004 — array size is not an integer

```ura
main():
    a int[] = int[1.5]
```

```tree
```

```out
```

```err
error: Array size must be an integer
  004.ura:2:19
  |
2 |     a int[] = int[1.5]
  |                   ^^^
```

```ll
```

## 005 — clean on a non-array

```ura
main():
    x int = 5
    clean x
```

```tree
```

```out
```

```err
error: 'clean' expects an array
  005.ura:3:5
  |
3 |     clean x
  |     ^^^^^
```

```ll
```

## 006 — new without an array type

```ura
main():
    a int[] = new int
```

```tree
```

```out
```

```err
error: Expected an array type after 'new' (e.g. new int[n])
  006.ura:2:15
  |
2 |     a int[] = new int
  |               ^^^
```

```ll
```

## 007 — missing closing bracket on index

```ura
main():
    a int[] = [1, 2, 3]
    output(a[0)
```

```tree
```

```out
```

```err
error: Expected ']' after array index
  007.ura:3:13
  |
3 |     output(a[0)
  |             ^
```

```ll
```

## 008 — empty array literal has no element type

```ura
main():
    a int[] = []
```

```tree
```

```out
```

```err
error: Empty array literal has no element type
  008.ura:2:15
  |
2 |     a int[] = []
  |               ^
```

```ll
```

## 009 — range bounds must be integers

```ura
main():
    a int[] = [1, 2, 3]
    mid int[] = a[1.5..3]
```

```tree
```

```out
```

```err
error: Range bounds must be integers
  009.ura:3:22
  |
3 |     mid int[] = a[1.5..3]
  |                      ^^
```

```ll
```

## 010 — .len on a non-array

```ura
main():
    x int = 5
    output(x.len)
```

```tree
```

```out
```

```err
error: '.len' is only valid on an array, not int
  010.ura:3:13
  |
3 |     output(x.len)
  |             ^
```

```ll
```

## 011 — unknown member

```ura
main():
    a int[] = [1, 2, 3]
    output(a.size)
```

```tree
```

```out
```

```err
error: Unknown member '.size'
  011.ura:3:13
  |
3 |     output(a.size)
  |             ^
```

```ll
```
