# lexer

## index

- 001 — `use` missing opening quote
- 002 — `use` with unclosed quote
- 003 — `link` missing opening quote
- 004 — `link` with unclosed quote
- 005 — unterminated string literal
- 006 — unterminated character literal
- 007 — unterminated block comment
- 008 — an unexpected character
- 009 — an unknown escape sequence
- 010 — `as` with no type after it
- 011 — an index with nothing in it
- 012 — a call with no closing parenthesis
- 013 — a function declaration missing its colon
- 014 — a function declaration missing its name
- 015 — a parameter missing its type
- 016 — a function declaration missing its return type
- 017 — `if` and `while` missing their colon
- 018 — `proto` not followed by `fn`
- 019 — a struct declaration missing its name
- 020 — an unexpected token in an expression

## 001 — `use` missing opening quote

```ura
// parse_errors/005 — `use` missing opening quote
use @/header

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Expected '"' after 'use'
  001.ura:2:5
  |
2 | use @/header
  |     ^
```

```ll
```

## 002 — `use` with unclosed quote

```ura
// parse_errors/006 — `use` with unclosed quote
use "@/header

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Unterminated 'use' path, expected closing '"'
  002.ura:2:5
  |
2 | use "@/header
  |     ^
```

```ll
```

## 003 — `link` missing opening quote

```ura
// errors/lexer/008 — `link` missing opening quote
link header.h

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Expected '"' after 'link'
  003.ura:2:6
  |
2 | link header.h
  |      ^
```

```ll
```

## 004 — `link` with unclosed quote

```ura
// errors/lexer/009 — `link` with unclosed quote
link "header.h

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Unterminated 'link' path, expected closing '"'
  004.ura:2:6
  |
2 | link "header.h
  |      ^
```

```ll
```

## 005 — unterminated string literal

```ura
// lexer/005.ura - a string with no closing quote

main():
    output("hello)
```

```tree
```

```out
```

```err
error: Unterminated string literal, expected '"'
  005.ura:4:12
  |
4 |     output("hello)
  |            ^
```

```ll
```

## 006 — unterminated character literal

```ura
// lexer/006.ura - a char with no closing quote

main():
    c char = 'a
```

```tree
```

```out
```

```err
error: Unterminated character literal, expected "'"
  006.ura:4:14
  |
4 |     c char = 'a
  |              ^
```

```ll
```

## 007 — unterminated block comment

```ura
// lexer/007.ura - a block comment that never closes

/* this comment never closes
main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Unterminated block comment, expected '*/'
  007.ura:5:1
  |
5 | /* this comment never closes
  | ^^
```

```ll
```

## 008 — an unexpected character

```ura
// lexer/008.ura - '#' is not part of the language

main():
    a i32 = 5 # 1
```

```tree
```

```out
```

```err
error: Unexpected character '#'
  008.ura:4:15
  |
4 |     a i32 = 5 # 1
  |               ^
```

```ll
```

## 009 — an unknown escape sequence

```ura
// lexer/009.ura - \q is not an escape

main():
    c char = '\q'
    output(c, "\n")
```

```tree
```

```out
```

```err
error: Unknown escape character: \q
  009.ura:4:15
  |
4 |     c char = '\q'
  |               ^^
```

```ll
```

## 010 — `as` with no type after it

```ura
// lexer/010.ura - a cast needs a target type

main():
    a i32 = 5 as
```

```tree
```

```out
```

```err
error: Expected a type
  010.ura:5:17
  |
5 |     a i32 = 5 as

  |                 ^
```

```ll
```

## 011 — an index with nothing in it

```ura
// lexer/011.ura - empty index expression

main():
    arr i32[] = new i32[3]
    x i32 = arr[]
```

```tree
```

```out
```

```err
error: Unexpected token
  011.ura:5:17
  |
5 |     x i32 = arr[]
  |                 ^
error: Expected ']' after array index
  011.ura:5:16
  |
5 |     x i32 = arr[]
  |                ^
```

```ll
```

## 012 — a call with no closing parenthesis

```ura
// lexer/012.ura - unclosed call

fn add(a i32, b i32) i32:
    return a + b

main():
    x i32 = add(1, 2
```

```tree
```

```out
```

```err
error: Unexpected token
  012.ura:8:21
  |
8 |     x i32 = add(1, 2

  |                     ^
error: Expected ')' after add arguments
  012.ura:7:13
  |
7 |     x i32 = add(1, 2
  |             ^^^
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
error: Unexpected token
[1m[0;31merror: [0mToo many errors, stopping
```

```ll
```

## 013 — a function declaration missing its colon

```ura
// lexer/013.ura - fn without ':'

fn answer() i32
    return 42

main():
    output(answer(), "\n")
```

```tree
```

```out
```

```err
error: Expected ':' after function answer
  013.ura:3:1
  |
3 | fn answer() i32
  | ^^
```

```ll
```

## 014 — a function declaration missing its name

```ura
// lexer/014.ura - fn without a name

fn () i32:
    return 0

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Expected a function name after 'fn'
  014.ura:3:1
  |
3 | fn () i32:
  | ^^
```

```ll
```

## 015 — a parameter missing its type

```ura
// lexer/015.ura - parameter without a type

fn add(a, b i32) i32:
    return a + b

main():
    output(add(1, 2), "\n")
```

```tree
```

```out
```

```err
error: Expected a type
  015.ura:3:9
  |
3 | fn add(a, b i32) i32:
  |         ^
error: Expected ')' after function add
  015.ura:3:1
  |
3 | fn add(a, b i32) i32:
  | ^^
error: Expected ':' after function add
  015.ura:3:1
  |
3 | fn add(a, b i32) i32:
  | ^^
```

```ll
```

## 016 — a function declaration missing its return type

```ura
// lexer/016.ura - fn without a return type

fn greet():
    output("hi\n")

main():
    greet()
```

```tree
```

```out
```

```err
error: Expected <data type> after function greet
  016.ura:3:1
  |
3 | fn greet():
  | ^^
```

```ll
```

## 017 — `if` and `while` missing their colon

```ura
// lexer/017.ura - block openers without ':'

main():
    x i32 = 5
    if x > 0
        output("positive\n")
```

```tree
```

```out
```

```err
error: Expected ':' to open the 'if' body
  017.ura:5:5
  |
5 |     if x > 0
  |     ^^
```

```ll
```

## 018 — `proto` not followed by `fn`

```ura
// lexer/018.ura - proto needs a fn

proto x i32

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Expected 'fn' after 'proto'
  018.ura:3:1
  |
3 | proto x i32
  | ^^^^^
```

```ll
```

## 019 — a struct declaration missing its name

```ura
// lexer/019.ura - struct without a name

struct:
    x i32

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Expected a struct name after 'struct'
  019.ura:3:1
  |
3 | struct:
  | ^^^^^^
```

```ll
```

## 020 — an unexpected token in an expression

```ura
// lexer/020.ura - '*' is not a prefix operator

main():
    x i32 = *
    return 0
```

```tree
```

```out
```

```err
error: Unexpected token
  020.ura:4:13
  |
4 |     x i32 = *
  |             ^
```

```ll
```
