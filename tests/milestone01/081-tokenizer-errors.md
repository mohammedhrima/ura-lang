# milestone01

## index

- 081 — tokenizer error: unknown character
- 082 — tokenizer error: unclosed string
- 083 — tokenizer error: unclosed character
- 084 — tokenizer error: character with two letters
- 085 — tokenizer error: unknown escape
- 086 — tokenizer error: unclosed comment
- 087 — tokenizer error: number too big for i32
- 088 — tokenizer: indentation that is not a multiple of 4 works
- 089 — tokenizer error: two errors in one file: both are shown
- 090 — parser error: missing : after a function header
- 091 — parser error: missing ( after a function name
- 092 — parser error: missing ) after the parameters
- 093 — parser error: a function with no name
- 094 — parser error: a parameter with no type
- 095 — parser error: missing : after an if condition
- 096 — parser error: missing : after a while condition
- 097 — parser error: else with a condition
- 098 — parser error: elif without an if
- 099 — parser error: unclosed (
- 100 — parser error: nothing after =
- 101 — parser error: nothing after an operator
- 102 — parser error: unclosed ( in a call
- 103 — name error: undeclared variable
- 104 — name error: a typo in a variable name
- 105 — name error: a variable used before its declaration
- 106 — name error: a variable declared twice
- 107 — name error: unknown type
- 108 — name error: undeclared function
- 109 — name error: a typo in a function name
- 110 — name error: wrong number of arguments
- 111 — name error: a function declared twice
- 112 — name error: break outside a loop
- 113 — name error: continue outside a loop
- 114 — name error: calling a variable
- 115 — tokenizer: unexpected character
- 116 — tokenizer: unterminated character
- 117 — tokenizer: empty character
- 118 — tokenizer: character with two letters
- 119 — tokenizer: integer too big for i32
- 120 — functions: missing name
- 121 — functions: unclosed parameters
- 122 — functions: parameter without a type
- 123 — functions: missing comma between parameters
- 124 — functions: variadic not last
- 125 — functions: unknown return type
- 126 — functions: unknown parameter type
- 127 — functions: parameter declared twice
- 128 — functions: defined twice
- 129 — functions: overloaded on return type only
- 130 — functions: return at top level
- 131 — structs: dot on a non-struct
- 132 — variables: unknown type
- 133 — variables: declared twice
- 134 — variables: used before its declaration
- 135 — variables: used outside its block
- 136 — variables: assigned without a declaration
- 137 — expressions: missing operand
- 138 — expressions: unclosed parenthesis
- 139 — expressions: unexpected parenthesis
- 140 — expressions: assign to a literal
- 141 — expressions: assign to a call
- 142 — expressions: compare a number and a string
- 143 — expressions: add a number and a string
- 144 — expressions: and on numbers
- 145 — calls: unknown function
- 146 — calls: too many arguments
- 147 — calls: wrong argument type
- 148 — calls: missing comma between arguments
- 149 — calls: unclosed call
- 150 — calls: trailing comma
- 151 — calls: call a variable
- 152 — calls: use a void result
- 153 — types: string into i8
- 154 — types: bool into i32
- 155 — types: number into bool
- 156 — types: number too big for i8
- 157 — types: wrong return type
- 158 — types: value returned from a void fn
- 159 — types: bare return in a fn with a type
- 160 — types: number as an if condition
- 161 — types: number as a while condition
- 162 — control flow: continue outside a loop
- 163 — control flow: elif without if
- 164 — control flow: else without if
- 165 — control flow: missing colon after if
- 166 — control flow: missing colon after elif
- 167 — control flow: missing colon after else
- 168 — control flow: missing colon after while
- 169 — control flow: if without a condition
- 170 — control flow: else with a condition
- 171 — recovery: three analysis errors
- 172 — recovery: two syntax errors in two functions
- 173 — diagnostics: error past line 9
- 174 — function: bare return in a void function
- 175 — type: compound assignment/unary operator
- 176 — type: boolean type
- 177 — builtin: puts: simple messages, char comparison on hero grade
- 178 — builtin: proto printf: hello world with a format string
- 179 — builtin: custom putchar via write: print dungeon tiles
- 180 — if: basic if: first floor greeting
- 181 — if: if-else: alive or dead check
- 182 — if: if-elif-else: floor difficulty rating
- 183 — function: void fn: announce dungeon floor
- 184 — function: fn with return value + single-line fns
- 185 — function: multi-fn: clamp, damage, is_dead
- 186 — function: return an integer literal
- 187 — function: declare a variable and return it
- 188 — function: function parameters and a direct call
- 189 — function: forward reference: call a function declared later
- 190 — function: a call used as an argument to another call
- 191 — function: value parameters: take_damage
- 192 — function: a by-value parameter does not change the caller
- 193 — function: redeclaring a function
- 194 — function: calling a function that does not exist
- 195 — function: too many arguments
- 196 — function: an argument of the wrong type
- 197 — function: two parameters with the same name
- 198 — function: a variadic call missing its required arguments
- 199 — function: a bare return in a void function
- 200 — lexer: unterminated string literal
- 201 — lexer: unterminated character literal
- 202 — lexer: unterminated block comment
- 203 — lexer: an unexpected character
- 204 — lexer: an unknown escape sequence
- 205 — lexer: as with no type after it
- 206 — lexer: a call with no closing parenthesis
- 207 — lexer: a function declaration missing its colon
- 208 — lexer: a function declaration missing its name
- 209 — lexer: a parameter missing its type
- 210 — function: a fn without a return type returns nothing
- 211 — lexer: if and while missing their colon
- 212 — lexer: proto not followed by fn
- 213 — lexer: a struct declaration missing its name
- 214 — lexer: an unexpected token in an expression
- 215 — lexer: member access with no member name after .
- 216 — literal: bool literals: declare, zero-init, bool function, reassign
- 217 — literal: char literals: declare, print, escapes, in a function
- 218 — loop: basic while: count enemy kills

---

## 081 — tokenizer error: unknown character

```ura
// error: unknown character '`'
// help: remove it
fn main() i32:
    x i32 = 5 ` 3
    return x
```

### stderr

```
error: test.ura:4:15 unknown character '`'
  |
4 |     x i32 = 5 ` 3
  |               ^
help: remove it
error: aborting due to 1 error
```

### status

```
1
```

---

## 082 — tokenizer error: unclosed string

```ura
// error: unclosed string
// help: add " at the end
fn main() i32:
    s i8[] = "abc
    return 0
```

### stderr

```
error: test.ura:4:14 unclosed string
  |
4 |     s i8[] = "abc
  |              ^^^^
help: use s i8[] = "abc"
error: aborting due to 1 error
```

### status

```
1
```

---

## 083 — tokenizer error: unclosed character

```ura
// error: unclosed character
// help: add ' after the character
fn main() i32:
    c char = 'a
    return 0
```

### stderr

```
error: test.ura:4:14 unclosed character
  |
4 |     c char = 'a
  |              ^^
help: use c char = 'a'
error: aborting due to 1 error
```

### status

```
1
```

---

## 084 — tokenizer error: character with two letters

```ura
// error: a character holds one letter, not 2
// help: use "ab" for a string
fn main() i32:
    c char = 'ab'
    return 0
```

### stderr

```
error: test.ura:4:14 a character holds one letter, not 2
  |
4 |     c char = 'ab'
  |              ^^^^
help: use "ab" for a string
error: aborting due to 1 error
```

### status

```
1
```

---

## 085 — tokenizer error: unknown escape

```ura
// error: unknown escape '\q'
// help: the escapes are \n \t \r \0 \\ \" \'
fn main() i32:
    s i8[] = "a\qb"
    return 0
```

### stderr

```
error: test.ura:4:16 unknown escape '\q'
  |
4 |     s i8[] = "a\qb"
  |                ^^
help: the escapes are \n \t \r \0 \\ \" \'
error: aborting due to 1 error
```

### status

```
1
```

---

## 086 — tokenizer error: unclosed comment

```ura
// error: unclosed comment
// help: add */ to close it
fn main() i32:
    /* this never ends
    return 0
```

### stderr

```
error: test.ura:4:5 unclosed comment
  |
4 |     /* this never ends
  |     ^^
help: add */ to close it
error: aborting due to 1 error
```

### status

```
1
```

---

## 087 — tokenizer error: number too big for i32

```ura
// error: 99999999999 doesn't fit in i32
// help: i32 goes from -2147483648 to 2147483647
fn main() i32:
    x i32 = 99999999999
    return 0
```

### stderr

```
error: test.ura:4:13 99999999999 doesn't fit in i32
  |
4 |     x i32 = 99999999999
  |             ^^^^^^^^^^^
help: i32 goes from -2147483648 to 2147483647
error: aborting due to 1 error
```

### status

```
1
```

---

## 088 — tokenizer: indentation that is not a multiple of 4 works

```ura
// expect: 7
fn main() i32:
   a i32 = 7
   if a == 7:
         return a
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
  %a = alloca i32, align 4
  store i32 7, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 7
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 089 — tokenizer error: two errors in one file: both are shown

```ura
// error: unknown character '`'
// error: unknown character '@'
fn main() i32:
    x i32 = 5 ` 3
    y i32 = 2 @ 1
    return 0
```

### stderr

```
error: test.ura:4:15 unknown character '`'
  |
4 |     x i32 = 5 ` 3
  |               ^
help: remove it
error: test.ura:5:15 unknown character '@'
  |
5 |     y i32 = 2 @ 1
  |               ^
help: remove it
error: aborting due to 2 errors
```

### status

```
1
```

---

## 090 — parser error: missing : after a function header

```ura
// error: expected ':' after the function header
// help: use fn main() i32:
fn main() i32
    return 0
```

### stderr

```
error: test.ura:3:11 expected ':' after the function header
  |
3 | fn main() i32
  |           ^^^
help: use fn main() i32:
error: aborting due to 1 error
```

### status

```
1
```

---

## 091 — parser error: missing ( after a function name

```ura
// error: expected '(' after the function name
// help: use fn main() i32:
fn main i32:
    return 0
```

### stderr

```
error: test.ura:3:9 expected '(' after the function name
  |
3 | fn main i32:
  |         ^^^
help: use fn main() i32:
error: aborting due to 1 error
```

### status

```
1
```

---

## 092 — parser error: missing ) after the parameters

```ura
// error: expected ')' after the parameters
// help: use fn add(a i32, b i32) i32:
fn add(a i32, b i32 i32:
    return a + b

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:21 expected ')' after the parameters
  |
3 | fn add(a i32, b i32 i32:
  |                     ^^^
help: use fn add(a i32, b i32) i32:
error: aborting due to 1 error
```

### status

```
1
```

---

## 093 — parser error: a function with no name

```ura
// error: expected a name after 'fn'
// help: use fn name() i32:
fn () i32:
    return 0
```

### stderr

```
error: test.ura:3:4 expected a name after 'fn'
  |
3 | fn () i32:
  |    ^
help: use fn name() i32:
error: aborting due to 1 error
```

### status

```
1
```

---

## 094 — parser error: a parameter with no type

```ura
// error: parameter 'a' has no type
// help: use a i32
fn twice(a) i32:
    return a * 2

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:10 parameter 'a' has no type
  |
3 | fn twice(a) i32:
  |          ^
help: like: a i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 095 — parser error: missing : after an if condition

```ura
// error: expected ':' after the if condition
// help: use if x > 1:
fn main() i32:
    x i32 = 2
    if x > 1
        return 1
    return 0
```

### stderr

```
error: test.ura:5:12 expected ':' after the if condition
  |
5 |     if x > 1
  |            ^
help: use if x > 1:
error: aborting due to 1 error
```

### status

```
1
```

---

## 096 — parser error: missing : after a while condition

```ura
// error: expected ':' after the while condition
// help: use while i < 3:
fn main() i32:
    i i32 = 0
    while i < 3
        i += 1
    return i
```

### stderr

```
error: test.ura:5:15 expected ':' after the while condition
  |
5 |     while i < 3
  |               ^
help: use while i < 3:
error: aborting due to 1 error
```

### status

```
1
```

---

## 097 — parser error: else with a condition

```ura
// error: else has no condition
// help: use elif x > 1:
fn main() i32:
    x i32 = 2
    if x == 0:
        return 0
    else x > 1:
        return 1
    return 2
```

### stderr

```
error: test.ura:7:5 else has no condition
  |
7 |     else x > 1:
  |     ^^^^
help: use elif x > 1:
error: aborting due to 1 error
```

### status

```
1
```

---

## 098 — parser error: elif without an if

```ura
// error: elif without an if before it
// help: use if x == 1:
fn main() i32:
    x i32 = 1
    elif x == 1:
        return 1
    return 0
```

### stderr

```
error: test.ura:5:5 elif without an if before it
  |
5 |     elif x == 1:
  |     ^^^^
help: use if x == 1:
error: aborting due to 1 error
```

### status

```
1
```

---

## 099 — parser error: unclosed (

```ura
// error: expected ')' to close this '('
// help: use x i32 = (1 + 2)
fn main() i32:
    x i32 = (1 + 2
    return x
```

### stderr

```
error: test.ura:4:18 expected ')' to close this '('
  |
4 |     x i32 = (1 + 2
  |                  ^
help: use x i32 = (1 + 2)
error: aborting due to 1 error
```

### status

```
1
```

---

## 100 — parser error: nothing after =

```ura
// error: expected a value after '='
fn main() i32:
    x i32 =
    return 0
```

### stderr

```
error: test.ura:3:11 expected a value after '='
  |
3 |     x i32 =
  |           ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 101 — parser error: nothing after an operator

```ura
// error: expected a value after '+'
fn main() i32:
    x i32 = 1 +
    return x
```

### stderr

```
error: test.ura:3:15 expected a value after '+'
  |
3 |     x i32 = 1 +
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 102 — parser error: unclosed ( in a call

```ura
// error: expected ')' to close the call to 'add'
// help: use return add(1, 2)
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(1, 2
```

### stderr

```
error: test.ura:7:19 expected ')' to close the call to 'add'
  |
7 |     return add(1, 2
  |                   ^
help: use return add(1, 2)
error: aborting due to 1 error
```

### status

```
1
```

---

## 103 — name error: undeclared variable

```ura
// error: undeclared variable 'y'
// help: declare it first, like: y i32 = 0
fn main() i32:
    x i32 = 1
    return y
```

### stderr

```
error: test.ura:5:12 undeclared variable 'y'
  |
5 |     return y
  |            ^
help: did you mean 'x'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 104 — name error: a typo in a variable name

```ura
// error: undeclared variable 'cuont'
// help: did you mean 'count'?
fn main() i32:
    count i32 = 3
    return cuont
```

### stderr

```
error: test.ura:5:12 undeclared variable 'cuont'
  |
5 |     return cuont
  |            ^^^^^
help: did you mean 'count'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 105 — name error: a variable used before its declaration

```ura
// error: 'x' is used before its declaration, on line 5
// help: move the declaration above this line
fn main() i32:
    y i32 = x + 1
    x i32 = 1
    return y
```

### stderr

```
error: test.ura:4:13 'x' is used before its declaration, on line 5
  |
4 |     y i32 = x + 1
  |             ^
help: move the declaration above this line
error: aborting due to 1 error
```

### status

```
1
```

---

## 106 — name error: a variable declared twice

```ura
// error: 'x' is already declared, on line 4
// help: use x = 2
fn main() i32:
    x i32 = 1
    x i32 = 2
    return x
```

### stderr

```
error: test.ura:5:5 'x' is already declared, on line 4
  |
5 |     x i32 = 2
  |     ^
help: use x = 2
error: aborting due to 1 error
```

### status

```
1
```

---

## 107 — name error: unknown type

```ura
// error: unknown type 'Point'
// help: declare it: struct Point:
fn main() i32:
    p Point
    return 0
```

### stderr

```
error: test.ura:4:7 unknown type 'Point'
  |
4 |     p Point
  |       ^^^^^
help: declare it: struct Point:
error: aborting due to 1 error
```

### status

```
1
```

---

## 108 — name error: undeclared function

```ura
// error: no function named 'foo'
fn main() i32:
    return foo()
```

### stderr

```
error: test.ura:3:12 no function named 'foo'
  |
3 |     return foo()
  |            ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 109 — name error: a typo in a function name

```ura
// error: no function named 'ad'
// help: did you mean 'add'?
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return ad(1, 2)
```

### stderr

```
error: test.ura:7:12 no function named 'ad'
  |
7 |     return ad(1, 2)
  |            ^^
help: did you mean 'add'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 110 — name error: wrong number of arguments

```ura
// error: 'add' takes 2 arguments, not 1
// help: fn add(a i32, b i32) i32
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(1)
```

### stderr

```
error: test.ura:7:12 'add' takes 2 arguments, not 1
  |
7 |     return add(1)
  |            ^^^
help: fn add(a i32, b i32) i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 111 — name error: a function declared twice

```ura
// error: function 'f' is already declared, on line 2
fn f() i32:
    return 1

fn f() i32:
    return 2

fn main() i32:
    return f()
```

### stderr

```
error: test.ura:5:4 'f' is already defined with these parameters
  |
5 | fn f() i32:
  |    ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 112 — name error: break outside a loop

```ura
// error: break outside a loop or match
// help: break only works inside a while or a match
fn main() i32:
    break
    return 0
```

### stderr

```
error: test.ura:4:5 break outside a loop or match
  |
4 |     break
  |     ^^^^^
help: break only works inside a while or a match
error: aborting due to 1 error
```

### status

```
1
```

---

## 113 — name error: continue outside a loop

```ura
// error: continue outside a loop
// help: continue only works inside a while
fn main() i32:
    if True:
        continue
    return 0
```

### stderr

```
error: test.ura:5:9 continue outside a loop
  |
5 |         continue
  |         ^^^^^^^^
help: continue only works inside a while
error: aborting due to 1 error
```

### status

```
1
```

---

## 114 — name error: calling a variable

```ura
// error: 'x' is a variable, not a function
fn main() i32:
    x i32 = 1
    return x()
```

### stderr

```
error: test.ura:4:12 'x' is a variable, not a function
  |
4 |     return x()
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 115 — tokenizer: unexpected character

```ura
fn main() i32:
    x i32 = 5 `
    return x
```

### stderr

```
error: test.ura:2:15 unknown character '`'
  |
2 |     x i32 = 5 `
  |               ^
help: remove it
error: aborting due to 1 error
```

### status

```
1
```

---

## 116 — tokenizer: unterminated character

```ura
fn main() i32:
    c i8 = 'a
    return 0
```

### stderr

```
error: test.ura:2:12 unclosed character
  |
2 |     c i8 = 'a
  |            ^^
help: use c i8 = 'a'
error: aborting due to 1 error
```

### status

```
1
```

---

## 117 — tokenizer: empty character

```ura
// expect: 0
fn main() i32:
    c i8 = ''
    if c == '\0':
        return 0
    return 1
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 0, i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 0

endif:                                            ; preds = %if0
  ret i32 1
}
```

---

## 118 — tokenizer: character with two letters

```ura
fn main() i32:
    c i8 = 'ab'
    return 0
```

### stderr

```
error: test.ura:2:12 a character holds one letter, not 2
  |
2 |     c i8 = 'ab'
  |            ^^^^
help: use "ab" for a string
error: aborting due to 1 error
```

### status

```
1
```

---

## 119 — tokenizer: integer too big for i32

```ura
fn main() i32:
    x i32 = 99999999999
    return x
```

### stderr

```
error: test.ura:2:13 99999999999 doesn't fit in i32
  |
2 |     x i32 = 99999999999
  |             ^^^^^^^^^^^
help: i32 goes from -2147483648 to 2147483647
error: aborting due to 1 error
```

### status

```
1
```

---

## 120 — functions: missing name

```ura
fn () i32:
    return 0

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:4 expected a name after 'fn'
  |
1 | fn () i32:
  |    ^
help: use fn name() i32:
error: aborting due to 1 error
```

### status

```
1
```

---

## 121 — functions: unclosed parameters

```ura
fn add(a i32:
    return a

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:13 expected ')' after the parameters
  |
1 | fn add(a i32:
  |             ^
help: use fn add(a i32):
error: aborting due to 1 error
```

### status

```
1
```

---

## 122 — functions: parameter without a type

```ura
fn f(a) i32:
    return 0

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:6 parameter 'a' has no type
  |
1 | fn f(a) i32:
  |      ^
help: like: a i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 123 — functions: missing comma between parameters

```ura
fn f(a i32 b i32) i32:
    return a

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:12 expected ',' between parameters
  |
1 | fn f(a i32 b i32) i32:
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 124 — functions: variadic not last

```ura
proto p(..., a i32) i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:9 '...' must be the last parameter
  |
1 | proto p(..., a i32) i32
  |         ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 125 — functions: unknown return type

```ura
fn f() Foo:
    return 0

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 unknown type 'Foo'
  |
1 | fn f() Foo:
  |        ^^^
help: declare it: struct Foo:
error: aborting due to 1 error
```

### status

```
1
```

---

## 126 — functions: unknown parameter type

```ura
fn f(a Foo) i32:
    return 0

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 unknown type 'Foo'
  |
1 | fn f(a Foo) i32:
  |        ^^^
help: declare it: struct Foo:
error: aborting due to 1 error
```

### status

```
1
```

---

## 127 — functions: parameter declared twice

```ura
fn f(a i32, a i32) i32:
    return a

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:13 'a' is already declared
  |
1 | fn f(a i32, a i32) i32:
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 128 — functions: defined twice

```ura
fn f() i32:
    return 1

fn f() i32:
    return 2

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:4:4 'f' is already defined with these parameters
  |
4 | fn f() i32:
  |    ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 129 — functions: overloaded on return type only

```ura
fn f() i32:
    return 1

fn f() i8:
    return 'a'

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:4:4 'f' can't be overloaded on its return type
  |
4 | fn f() i8:
  |    ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 130 — functions: return at top level

```ura
return 1

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:1 'return' outside of a function
  |
1 | return 1
  | ^^^^^^
help: top-level code returns only in a file without main
error: aborting due to 1 error
```

### status

```
1
```

---

## 131 — structs: dot on a non-struct

```ura
fn main() i32:
    a i32 = 1
    return a.b
```

### stderr

```
error: test.ura:3:12 'a' is 'i32', not a struct
  |
3 |     return a.b
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 132 — variables: unknown type

```ura
fn main() i32:
    x Foo
    return 0
```

### stderr

```
error: test.ura:2:7 unknown type 'Foo'
  |
2 |     x Foo
  |       ^^^
help: declare it: struct Foo:
error: aborting due to 1 error
```

### status

```
1
```

---

## 133 — variables: declared twice

```ura
fn main() i32:
    a i32 = 1
    a i32 = 2
    return a
```

### stderr

```
error: test.ura:3:5 'a' is already declared, on line 2
  |
3 |     a i32 = 2
  |     ^
help: use a = 2
error: aborting due to 1 error
```

### status

```
1
```

---

## 134 — variables: used before its declaration

```ura
fn main() i32:
    b i32 = a
    a i32 = 1
    return b
```

### stderr

```
error: test.ura:2:13 'a' is used before its declaration, on line 3
  |
2 |     b i32 = a
  |             ^
help: move the declaration above this line
error: aborting due to 1 error
```

### status

```
1
```

---

## 135 — variables: used outside its block

```ura
fn main() i32:
    if True:
        x i32 = 1
    return x
```

### stderr

```
error: test.ura:4:12 undeclared variable 'x'
  |
4 |     return x
  |            ^
help: declare it first, like: x i32 = 0
error: aborting due to 1 error
```

### status

```
1
```

---

## 136 — variables: assigned without a declaration

```ura
fn main() i32:
    x = 5
    return 0
```

### stderr

```
error: test.ura:2:5 undeclared variable 'x'
  |
2 |     x = 5
  |     ^
help: declare it first, like: x i32 = 0
error: aborting due to 1 error
```

### status

```
1
```

---

## 137 — expressions: missing operand

```ura
fn main() i32:
    a i32 = 1 +
    return a
```

### stderr

```
error: test.ura:2:15 expected a value after '+'
  |
2 |     a i32 = 1 +
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 138 — expressions: unclosed parenthesis

```ura
fn main() i32:
    a i32 = (1 + 2
    return a
```

### stderr

```
error: test.ura:2:18 expected ')' to close this '('
  |
2 |     a i32 = (1 + 2
  |                  ^
help: use a i32 = (1 + 2)
error: aborting due to 1 error
```

### status

```
1
```

---

## 139 — expressions: unexpected parenthesis

```ura
fn main() i32:
    a i32 = 1 + 2)
    return a
```

### stderr

```
error: test.ura:2:18 unexpected ')'
  |
2 |     a i32 = 1 + 2)
  |                  ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 140 — expressions: assign to a literal

```ura
fn main() i32:
    a i32 = 1
    1 = a
    return a
```

### stderr

```
error: test.ura:3:5 can't assign to a value
  |
3 |     1 = a
  |     ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 141 — expressions: assign to a call

```ura
fn f() i32:
    return 1

fn main() i32:
    f() = 3
    return 0
```

### stderr

```
error: test.ura:5:5 can't assign to a value
  |
5 |     f() = 3
  |     ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 142 — expressions: compare a number and a string

```ura
fn main() i32:
    a i32 = 1
    if a == "x":
        return 1
    return 0
```

### stderr

```
error: test.ura:3:13 expected 'i32', found 'String'
  |
3 |     if a == "x":
  |             ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 143 — expressions: add a number and a string

```ura
fn main() i32:
    return 1 + "a"
```

### stderr

```
error: test.ura:2:14 can't apply '+' to 'i32' and 'String'
  |
2 |     return 1 + "a"
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 144 — expressions: and on numbers

```ura
fn main() i32:
    if 1 and 2:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:8 expected 'bool', found 'i32'
  |
2 |     if 1 and 2:
  |        ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 145 — calls: unknown function

```ura
fn main() i32:
    return zz()
```

### stderr

```
error: test.ura:2:12 no function named 'zz'
  |
2 |     return zz()
  |            ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 146 — calls: too many arguments

```ura
fn f(a i32) i32:
    return a

fn main() i32:
    return f(1, 2)
```

### stderr

```
error: test.ura:5:12 'f' takes 1 argument, not 2
  |
5 |     return f(1, 2)
  |            ^
help: fn f(a i32) i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 147 — calls: wrong argument type

```ura
fn f(a i32) i32:
    return a

fn main() i32:
    return f("a")
```

### stderr

```
error: test.ura:5:14 expected 'i32', found 'String'
  |
5 |     return f("a")
  |              ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 148 — calls: missing comma between arguments

```ura
fn f(a i32, b i32) i32:
    return a

fn main() i32:
    return f(1 2)
```

### stderr

```
error: test.ura:5:16 expected ',' between arguments
  |
5 |     return f(1 2)
  |                ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 149 — calls: unclosed call

```ura
fn f(a i32, b i32) i32:
    return a

fn main() i32:
    x i32 = f(1, 2
    return x
```

### stderr

```
error: test.ura:5:18 expected ')' to close the call to 'f'
  |
5 |     x i32 = f(1, 2
  |                  ^
help: use x i32 = f(1, 2)
error: aborting due to 1 error
```

### status

```
1
```

---

## 150 — calls: trailing comma

```ura
fn f(a i32) i32:
    return a

fn main() i32:
    return f(1,)
```

### stderr

```
error: test.ura:5:16 expected an argument after ','
  |
5 |     return f(1,)
  |                ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 151 — calls: call a variable

```ura
fn main() i32:
    a i32 = 1
    return a()
```

### stderr

```
error: test.ura:3:12 'a' is a variable, not a function
  |
3 |     return a()
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 152 — calls: use a void result

```ura
fn v():
    return

fn main() i32:
    x i32 = v()
    return x
```

### stderr

```
error: test.ura:5:13 expected 'i32', found 'void'
  |
5 |     x i32 = v()
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 153 — types: string into i8

```ura
fn main() i32:
    x i8 = "a"
    return 0
```

### stderr

```
error: test.ura:2:12 expected 'i8', found 'String'
  |
2 |     x i8 = "a"
  |            ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 154 — types: bool into i32

```ura
fn main() i32:
    a i32 = True
    return a
```

### stderr

```
error: test.ura:2:13 expected 'i32', found 'bool'
  |
2 |     a i32 = True
  |             ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 155 — types: number into bool

```ura
fn main() i32:
    b bool = 1
    return 0
```

### stderr

```
error: test.ura:2:14 expected 'bool', found 'i32'
  |
2 |     b bool = 1
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 156 — types: number too big for i8

```ura
fn main() i32:
    x i8 = 300
    return 0
```

### stderr

```
error: test.ura:2:12 300 doesn't fit in i8
  |
2 |     x i8 = 300
  |            ^^^
help: i8 goes from -128 to 127
error: aborting due to 1 error
```

### status

```
1
```

---

## 157 — types: wrong return type

```ura
fn f() i32:
    return "a"

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:12 expected 'i32', found 'String'
  |
2 |     return "a"
  |            ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 158 — types: value returned from a void fn

```ura
fn f():
    return 1

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:12 expected 'void', found 'i32'
  |
2 |     return 1
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 159 — types: bare return in a fn with a type

```ura
fn f() i32:
    return

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:5 'f' must return a value of type 'i32'
  |
2 |     return
  |     ^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 160 — types: number as an if condition

```ura
fn main() i32:
    if 5:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:8 expected 'bool', found 'i32'
  |
2 |     if 5:
  |        ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 161 — types: number as a while condition

```ura
fn main() i32:
    a i32 = 3
    while a:
        a -= 1
    return a
```

### stderr

```
error: test.ura:3:11 expected 'bool', found 'i32'
  |
3 |     while a:
  |           ^
help: write 'a != 0'
error: aborting due to 1 error
```

### status

```
1
```

---

## 162 — control flow: continue outside a loop

```ura
fn main() i32:
    continue
    return 0
```

### stderr

```
error: test.ura:2:5 continue outside a loop
  |
2 |     continue
  |     ^^^^^^^^
help: continue only works inside a while
error: aborting due to 1 error
```

### status

```
1
```

---

## 163 — control flow: elif without if

```ura
fn main() i32:
    elif True:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:5 elif without an if before it
  |
2 |     elif True:
  |     ^^^^
help: use if True:
error: aborting due to 1 error
```

### status

```
1
```

---

## 164 — control flow: else without if

```ura
fn main() i32:
    else:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:5 else without an if before it
  |
2 |     else:
  |     ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 165 — control flow: missing colon after if

```ura
fn main() i32:
    a i32 = 2
    if a > 1
        return 1
    return 0
```

### stderr

```
error: test.ura:3:12 expected ':' after the if condition
  |
3 |     if a > 1
  |            ^
help: use if a > 1:
error: aborting due to 1 error
```

### status

```
1
```

---

## 166 — control flow: missing colon after elif

```ura
fn main() i32:
    a i32 = 2
    if a > 1:
        return 1
    elif a > 0
        return 2
    return 0
```

### stderr

```
error: test.ura:5:14 expected ':' after the elif condition
  |
5 |     elif a > 0
  |              ^
help: use elif a > 0:
error: aborting due to 1 error
```

### status

```
1
```

---

## 167 — control flow: missing colon after else

```ura
fn main() i32:
    a i32 = 2
    if a > 1:
        return 1
    else
        return 2
```

### stderr

```
error: test.ura:5:5 expected ':' after else
  |
5 |     else
  |     ^^^^
help: use else:
error: aborting due to 1 error
```

### status

```
1
```

---

## 168 — control flow: missing colon after while

```ura
fn main() i32:
    a i32 = 2
    while a > 0
        a -= 1
    return a
```

### stderr

```
error: test.ura:3:15 expected ':' after the while condition
  |
3 |     while a > 0
  |               ^
help: use while a > 0:
error: aborting due to 1 error
```

### status

```
1
```

---

## 169 — control flow: if without a condition

```ura
fn main() i32:
    if:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:5 expected a condition after 'if'
  |
2 |     if:
  |     ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 170 — control flow: else with a condition

```ura
fn main() i32:
    a i32 = 2
    if a > 1:
        return 1
    else a > 0:
        return 2
    return 0
```

### stderr

```
error: test.ura:5:5 else has no condition
  |
5 |     else a > 0:
  |     ^^^^
help: use elif a > 0:
error: aborting due to 1 error
```

### status

```
1
```

---

## 171 — recovery: three analysis errors

```ura
fn main() i32:
    a i32 = zz
    b i32 = u.x
    return yy()
```

### stderr

```
error: test.ura:2:13 undeclared variable 'zz'
  |
2 |     a i32 = zz
  |             ^^
help: did you mean 'a'?
error: test.ura:3:13 undeclared variable 'u'
  |
3 |     b i32 = u.x
  |             ^
help: did you mean 'a'?
error: test.ura:4:12 no function named 'yy'
  |
4 |     return yy()
  |            ^^
error: aborting due to 3 errors
```

### status

```
1
```

---

## 172 — recovery: two syntax errors in two functions

```ura
fn f() i32
    return 1

fn g() i32:
    return (2

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 expected ':' after the function header
  |
1 | fn f() i32
  |        ^^^
help: use fn f() i32:
error: test.ura:5:13 expected ')' to close this '('
  |
5 |     return (2
  |             ^
help: use return (2)
error: aborting due to 2 errors
```

### status

```
1
```

---

## 173 — diagnostics: error past line 9

```ura
fn main() i32:
    a i32 = 1
    b i32 = 2
    c i32 = 3
    d i32 = 4
    e i32 = 5
    f i32 = 6
    g i32 = 7
    h i32 = 8
    i i32 = 9
    return zz
```

### stderr

```
error: test.ura:11:12 undeclared variable 'zz'
   |
11 |     return zz
   |            ^^
help: did you mean 'a'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 174 — function: bare return in a void function

```ura
fn foo(a i32):
   return

fn foo(a bool):
   return

fn main() i32:
   foo(1)
   foo(True)
   return 7
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @foo(i32 %0) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  ret void
}

define void @foo.1(i1 %0) {
entry:
  %a = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  ret void
}

define i32 @main() {
entry:
  call void @foo(i32 1)
  call void @foo.1(i1 true)
  ret i32 7
}
```

---

## 175 — type: compound assignment/unary operator

```ura
fn main() i32:
    a i32 = -10
    a += +2
    a *= 4
    a /= 2
    a -= 2
    a %= 3
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 -10, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a", 2
  store i32 %ADD, i32* %a, align 4
  %"$a1" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a1", 4
  store i32 %MUL, i32* %a, align 4
  %"$a2" = load i32, i32* %a, align 4
  %DIV = sdiv i32 %"$a2", 2
  store i32 %DIV, i32* %a, align 4
  %"$a3" = load i32, i32* %a, align 4
  %SUB = sub i32 %"$a3", 2
  store i32 %SUB, i32* %a, align 4
  %"$a4" = load i32, i32* %a, align 4
  %MOD = srem i32 %"$a4", 3
  store i32 %MOD, i32* %a, align 4
  ret i32 0
}
```

---

## 176 — type: boolean type

```ura
fn main() i32:
   flag bool = True
   if flag:
      return 1
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
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 177 — builtin: puts: simple messages, char comparison on hero grade

```ura
// stdout: S-rank hero — unstoppable
// expect: 0

proto puts(str char[]) i32

fn is_s_rank(grade char) bool:
    return grade == 'S'

fn main() i32:
    grade char = 'S'
    if is_s_rank(grade):
        puts("S-rank hero — unstoppable")
    else:
        puts("keep training")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [28 x i8] c"S-rank hero \E2\80\94 unstoppable\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"keep training\00", align 1

declare i32 @puts(i8*)

define i1 @is_s_rank(i8 %0) {
entry:
  %grade = alloca i8, align 1
  store i8 %0, i8* %grade, align 1
  %"$grade" = load i8, i8* %grade, align 1
  %EQ = icmp eq i8 %"$grade", 83
  ret i1 %EQ
}

define i32 @main() {
entry:
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$grade" = load i8, i8* %grade, align 1
  %call.is_s_rank = call i1 @is_s_rank(i8 %"$grade")
  br i1 %call.is_s_rank, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  %call.puts = call i32 @puts(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @str, i32 0, i32 0))
  br label %endif

else1:                                            ; preds = %if0
  %call.puts1 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  ret i32 0
}
```

---

## 178 — builtin: proto printf: hello world with a format string

```ura
// stdout: hello ura 42
// expect: 0

proto printf(fmt char[], ...) i32

fn main() i32:
    s char[] = "ura"
    printf("hello %s %d\n", s, 42)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [13 x i8] c"hello %s %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  store [4 x i8] c"ura\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i8* %"$s", i32 42)
  ret i32 0
}
```

---

## 179 — builtin: custom putchar via write: print dungeon tiles

```ura
// stdout: DUNGEON
// expect: 0

fn putchar(c char) i32:
    buf char[] = [c, '\0']
    write(1, buf, 1)
    return 0

fn main() i32:
    putchar('D')
    putchar('U')
    putchar('N')
    putchar('G')
    putchar('E')
    putchar('O')
    putchar('N')
    putchar('\n')
    return 0
```

---

## 180 — if: basic if: first floor greeting

```ura
// stdout: Fresh start
// expect: 0

fn main() i32:
    floor i32 = 1
    if floor == 1:
        output("Fresh start\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [13 x i8] c"Fresh start\0A\00", align 1

define i32 @main() {
entry:
  %floor = alloca i32, align 4
  store i32 1, i32* %floor, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$floor" = load i32, i32* %floor, align 4
  %EQ = icmp eq i32 %"$floor", 1
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 181 — if: if-else: alive or dead check

```ura
// stdout: Hero has fallen
// expect: 0

fn main() i32:
    hp i32 = 0
    if hp > 0:
        output("Hero is alive\n")
    else:
        output("Hero has fallen\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [15 x i8] c"Hero is alive\0A\00", align 1
@str.1 = private unnamed_addr constant [17 x i8] c"Hero has fallen\0A\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 0, i32* %hp, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$hp" = load i32, i32* %hp, align 4
  %GT = icmp sgt i32 %"$hp", 0
  br i1 %GT, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0))
  br label %endif

else1:                                            ; preds = %if0
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 182 — if: if-elif-else: floor difficulty rating

```ura
// stdout: Normal
// expect: 0

fn main() i32:
    floor i32 = 6
    if floor < 3:
        output("Easy\n")
    elif floor < 7:
        output("Normal\n")
    else:
        output("Hard\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"Easy\0A\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"Normal\0A\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"Hard\0A\00", align 1

define i32 @main() {
entry:
  %floor = alloca i32, align 4
  store i32 6, i32* %floor, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$floor" = load i32, i32* %floor, align 4
  %LT = icmp slt i32 %"$floor", 3
  br i1 %LT, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  br label %endif

elif1:                                            ; preds = %if0
  %"$floor1" = load i32, i32* %floor, align 4
  %LT2 = icmp slt i32 %"$floor1", 7
  br i1 %LT2, label %elif1.then, label %else2

elif1.then:                                       ; preds = %elif1
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0))
  br label %endif

else2:                                            ; preds = %elif1
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %else2, %elif1.then, %if0.then
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 183 — function: void fn: announce dungeon floor

```ura
// stdout: === Entering floor 1 ===
// stdout: === Entering floor 5 ===
// stdout: BOSS: The Lich King appears!
// expect: 0

fn announce_floor(floor i32):
    output("=== Entering floor ", floor, " ===\n")

fn announce_boss(name char[]):
    output("BOSS: ", name, " appears!\n")

fn main() i32:
    announce_floor(1)
    announce_floor(5)
    announce_boss("The Lich King")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [27 x i8] c"=== Entering floor %d ===\0A\00", align 1
@str.1 = private unnamed_addr constant [19 x i8] c"BOSS: %s appears!\0A\00", align 1
@str.2 = private unnamed_addr constant [14 x i8] c"The Lich King\00", align 1

define void @announce_floor(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %"$floor" = load i32, i32* %floor, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str, i32 0, i32 0), i32 %"$floor")
  ret void
}

define void @announce_boss(i8* %0) {
entry:
  %name = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %"$name" = load i8*, i8** %name, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i8* %"$name")
  ret void
}

define i32 @main() {
entry:
  call void @announce_floor(i32 1)
  call void @announce_floor(i32 5)
  call void @announce_boss(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 184 — function: fn with return value + single-line fns

```ura
// stdout: Floor 3 bonus: 150
// stdout: Hero alive:    True
// stdout: Boss floor:    True
// expect: 0

fn victory_bonus(floor i32) i32:
    return floor * 50

fn is_alive(hp i32) bool: return hp > 0
fn is_boss_floor(floor i32) bool: return floor == 10

fn main() i32:
    bonus i32 = victory_bonus(3)
    output("Floor 3 bonus: ", bonus, "\n")
    output("Hero alive:    ", is_alive(80), "\n")
    output("Boss floor:    ", is_boss_floor(10), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [19 x i8] c"Floor 3 bonus: %d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [19 x i8] c"Hero alive:    %s\0A\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [19 x i8] c"Boss floor:    %s\0A\00", align 1

define i32 @victory_bonus(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %"$floor" = load i32, i32* %floor, align 4
  %MUL = mul i32 %"$floor", 50
  ret i32 %MUL
}

define i1 @is_alive(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %GT = icmp sgt i32 %"$hp", 0
  ret i1 %GT
}

define i1 @is_boss_floor(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %"$floor" = load i32, i32* %floor, align 4
  %EQ = icmp eq i32 %"$floor", 10
  ret i1 %EQ
}

define i32 @main() {
entry:
  %bonus = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text3 = alloca i8*, align 8
  %call.victory_bonus = call i32 @victory_bonus(i32 3)
  store i32 %call.victory_bonus, i32* %bonus, align 4
  %"$bonus" = load i32, i32* %bonus, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str, i32 0, i32 0), i32 %"$bonus")
  %call.is_alive = call i1 @is_alive(i32 80)
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %call.is_alive, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  %call.is_boss_floor = call i1 @is_boss_floor(i32 10)
  store i8* null, i8** %bool.text3, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %call.is_boss_floor, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text3, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text3, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text4" = load i8*, i8** %bool.text3, align 8
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text4")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 185 — function: multi-fn: clamp, damage, is_dead

```ura
// stdout: Orc takes 17 damage
// stdout: Orc dead: True
// expect: 0

fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo:
        return lo
    if val > hi:
        return hi
    return val

fn damage(atk i32, def i32) i32:
    return clamp(atk - def, 0, 999)

fn is_dead(hp i32) bool:
    return hp <= 0

fn main() i32:
    d i32 = damage(25, 8)
    output("Orc takes ", d, " damage\n")
    output("Orc dead: ", is_dead(d - 60), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [21 x i8] c"Orc takes %d damage\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [14 x i8] c"Orc dead: %s\0A\00", align 1

define i32 @clamp(i32 %0, i32 %1, i32 %2) {
entry:
  %val = alloca i32, align 4
  %lo = alloca i32, align 4
  %hi = alloca i32, align 4
  store i32 %0, i32* %val, align 4
  store i32 %1, i32* %lo, align 4
  store i32 %2, i32* %hi, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$val" = load i32, i32* %val, align 4
  %"$lo" = load i32, i32* %lo, align 4
  %LT = icmp slt i32 %"$val", %"$lo"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$lo2" = load i32, i32* %lo, align 4
  ret i32 %"$lo2"

endif:                                            ; preds = %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$val3" = load i32, i32* %val, align 4
  %"$hi" = load i32, i32* %hi, align 4
  %GT = icmp sgt i32 %"$val3", %"$hi"
  br i1 %GT, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$hi4" = load i32, i32* %hi, align 4
  ret i32 %"$hi4"

endif1:                                           ; preds = %if1
  %"$val5" = load i32, i32* %val, align 4
  ret i32 %"$val5"
}

define i32 @damage(i32 %0, i32 %1) {
entry:
  %atk = alloca i32, align 4
  %def = alloca i32, align 4
  store i32 %0, i32* %atk, align 4
  store i32 %1, i32* %def, align 4
  %"$atk" = load i32, i32* %atk, align 4
  %"$def" = load i32, i32* %def, align 4
  %SUB = sub i32 %"$atk", %"$def"
  %call.clamp = call i32 @clamp(i32 %SUB, i32 0, i32 999)
  ret i32 %call.clamp
}

define i1 @is_dead(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %LE = icmp sle i32 %"$hp", 0
  ret i1 %LE
}

define i32 @main() {
entry:
  %d = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %call.damage = call i32 @damage(i32 25, i32 8)
  store i32 %call.damage, i32* %d, align 4
  %"$d" = load i32, i32* %d, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i32 %"$d")
  %"$d1" = load i32, i32* %d, align 4
  %SUB = sub i32 %"$d1", 60
  %call.is_dead = call i1 @is_dead(i32 %SUB)
  store i8* null, i8** %bool.text, align 8
  br label %if2

if2:                                              ; preds = %entry
  br i1 %call.is_dead, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else3, %if2.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 186 — function: return an integer literal

```ura
// stderr: exit: 42
// expect: 1

fn main() i32:
    return 42
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  ret i32 42
}
```

---

## 187 — function: declare a variable and return it

```ura
// stderr: exit: 7
// expect: 1

fn main() i32:
    a i32 = 7
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 7, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 188 — function: function parameters and a direct call

```ura
// stderr: exit: 42
// expect: 1

fn add(a i32, b i32) i32:
    return a + b
fn main() i32:
    return add(40, 2)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %call.add = call i32 @add(i32 40, i32 2)
  ret i32 %call.add
}
```

---

## 189 — function: forward reference: call a function declared later

```ura
// stderr: exit: 42
// expect: 1

fn main() i32:
    return dbl(21)
fn dbl(n i32) i32:
    return n + n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %call.dbl = call i32 @dbl(i32 21)
  ret i32 %call.dbl
}

define i32 @dbl(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", %"$n1"
  ret i32 %ADD
}
```

---

## 190 — function: a call used as an argument to another call

```ura
// stderr: exit: 28
// expect: 1

fn square(n i32) i32:
    return n * n
fn add(a i32, b i32) i32:
    return a + b
fn main() i32:
    return add(square(5), 3)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %"$n1" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", %"$n1"
  ret i32 %MUL
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %call.square = call i32 @square(i32 5)
  %call.add = call i32 @add(i32 %call.square, i32 3)
  ret i32 %call.add
}
```

---

## 191 — function: value parameters: take_damage

```ura
// stdout: <82>
// expect: 0

fn take_damage(hp i32, atk i32) i32:
    return hp - atk

fn main() i32:
    result i32 = take_damage(100, 18)
    output("<", result, ">\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @take_damage(i32 %0, i32 %1) {
entry:
  %hp = alloca i32, align 4
  %atk = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  store i32 %1, i32* %atk, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %"$atk" = load i32, i32* %atk, align 4
  %SUB = sub i32 %"$hp", %"$atk"
  ret i32 %SUB
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %call.take_damage = call i32 @take_damage(i32 100, i32 18)
  store i32 %call.take_damage, i32* %result, align 4
  %"$result" = load i32, i32* %result, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$result")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 192 — function: a by-value parameter does not change the caller

```ura
// stderr: exit: 5
// expect: 1

fn inc(n i32) i32:
    n = n + 1
    return n
fn main() i32:
    n i32 = 5
    inc(n)
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @inc(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  %"$n1" = load i32, i32* %n, align 4
  ret i32 %"$n1"
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 5, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.inc = call i32 @inc(i32 %"$n")
  %"$n1" = load i32, i32* %n, align 4
  ret i32 %"$n1"
}
```

---

## 193 — function: redeclaring a function

```ura
// error: Redeclaration of function 'dup'

fn dup() i32:
    return 1

fn dup() i32:
    return 2

fn main() i32:
    return dup()
```

### stderr

```
error: test.ura:6:4 'dup' is already defined with these parameters
  |
6 | fn dup() i32:
  |    ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 194 — function: calling a function that does not exist

```ura
// error: no function named 'bar'

fn main() i32:
    return bar(3)
```

### stderr

```
error: test.ura:4:12 no function named 'bar'
  |
4 |     return bar(3)
  |            ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 195 — function: too many arguments

```ura
// error: Wrong number of arguments to 'add'

fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    x i32 = add(1, 2, 3)
    output(x, "\n")
    return 0
```

### stderr

```
error: test.ura:7:13 'add' takes 2 arguments, not 3
  |
7 |     x i32 = add(1, 2, 3)
  |             ^^^
help: fn add(a i32, b i32) i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 196 — function: an argument of the wrong type

```ura
// error: Argument 1 type mismatch in call to 'show'

fn show(s char[]):
    output(s, "\n")

fn main() i32:
    show(42)
    return 0
```

### stderr

```
error: test.ura:7:10 expected 'i8[]', found 'i32'
  |
7 |     show(42)
  |          ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 197 — function: two parameters with the same name

```ura
// error: Redeclaration of variable 'a'

fn add(a i32, a i32) i32:
    return a

fn main() i32:
    output(add(1, 2), "\n")
    return 0
```

### stderr

```
error: test.ura:3:15 'a' is already declared
  |
3 | fn add(a i32, a i32) i32:
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 198 — function: a variadic call missing its required arguments

```ura
// error: Wrong number of arguments to 'printf'

proto printf(fmt char[], ...) i32

fn main() i32:
    printf()
    return 0
```

### stderr

```
error: test.ura:6:5 'printf' takes at least 1 argument, not 0
  |
6 |     printf()
  |     ^^^^^^
help: proto printf(fmt i8[], ...) i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 199 — function: a bare return in a void function

```ura
// stdout: floor cleared
// stdout: down
// stdout: still up
// expect: 0

fn announce():
    output("floor cleared\n")
    return

fn guard(hp i32):
    if hp <= 0:
        output("down\n")
        return
    output("still up\n")

fn main() i32:
    announce()
    guard(0)
    guard(5)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [15 x i8] c"floor cleared\0A\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"down\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"still up\0A\00", align 1

define void @announce() {
entry:
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0))
  ret void
}

define void @guard(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$hp" = load i32, i32* %hp, align 4
  %LE = icmp sle i32 %"$hp", 0
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  ret void

endif:                                            ; preds = %if0
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  call void @announce()
  call void @guard(i32 0)
  call void @guard(i32 5)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 200 — lexer: unterminated string literal

```ura
// error: unclosed string

fn main() i32:
    output("hello)
    return 0
```

### stderr

```
error: test.ura:4:12 unclosed string
  |
4 |     output("hello)
  |            ^^^^^^^
help: use output("hello)"
error: aborting due to 1 error
```

### status

```
1
```

---

## 201 — lexer: unterminated character literal

```ura
// error: unclosed character

fn main() i32:
    c char = 'a
    return 0
```

### stderr

```
error: test.ura:4:14 unclosed character
  |
4 |     c char = 'a
  |              ^^
help: use c char = 'a'
error: aborting due to 1 error
```

### status

```
1
```

---

## 202 — lexer: unterminated block comment

```ura
// error: unclosed comment

/* this comment never closes
fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:3:1 unclosed comment
  |
3 | /* this comment never closes
  | ^^
help: add */ to close it
error: aborting due to 1 error
```

### status

```
1
```

---

## 203 — lexer: an unexpected character

```ura
// error: unknown character '#'

fn main() i32:
    a i32 = 5 # 1
    return 0
```

### stderr

```
error: test.ura:4:15 unknown character '#'
  |
4 |     a i32 = 5 # 1
  |               ^
help: remove it
error: aborting due to 1 error
```

### status

```
1
```

---

## 204 — lexer: an unknown escape sequence

```ura
// error: unknown escape '\q'

fn main() i32:
    c char = '\q'
    output(c, "\n")
    return 0
```

### stderr

```
error: test.ura:4:15 unknown escape '\q'
  |
4 |     c char = '\q'
  |               ^^
help: the escapes are \n \t \r \0 \\ \" \'
error: aborting due to 1 error
```

### status

```
1
```

---

## 205 — lexer: as with no type after it

```ura
// error: Expected a type

fn main() i32:
    a i32 = 5 as
    return 0
```

### stderr

```
error: test.ura:4:15 expected a type after 'as'
  |
4 |     a i32 = 5 as
  |               ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 206 — lexer: a call with no closing parenthesis

```ura
// error: expected ')' to close the call to 'add'

fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    x i32 = add(1, 2
    return 0
```

### stderr

```
error: test.ura:7:20 expected ')' to close the call to 'add'
  |
7 |     x i32 = add(1, 2
  |                    ^
help: use x i32 = add(1, 2)
error: aborting due to 1 error
```

### status

```
1
```

---

## 207 — lexer: a function declaration missing its colon

```ura
// error: expected ':' after the function header

fn answer() i32
    return 42

fn main() i32:
    output(answer(), "\n")
    return 0
```

### stderr

```
error: test.ura:3:13 expected ':' after the function header
  |
3 | fn answer() i32
  |             ^^^
help: use fn answer() i32:
error: aborting due to 1 error
```

### status

```
1
```

---

## 208 — lexer: a function declaration missing its name

```ura
// error: expected a name after 'fn'

fn () i32:
    return 0

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:3:4 expected a name after 'fn'
  |
3 | fn () i32:
  |    ^
help: use fn name() i32:
error: aborting due to 1 error
```

### status

```
1
```

---

## 209 — lexer: a parameter missing its type

```ura
// error: Expected a type

fn add(a, b i32) i32:
    return a + b

fn main() i32:
    output(add(1, 2), "\n")
    return 0
```

### stderr

```
error: test.ura:3:8 parameter 'a' has no type
  |
3 | fn add(a, b i32) i32:
  |        ^
help: like: a i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 210 — function: a fn without a return type returns nothing

```ura
// stdout: hi
// expect: 0

fn greet():
    output("hi\n")

fn main() i32:
    greet()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"hi\0A\00", align 1

define void @greet() {
entry:
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  call void @greet()
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 211 — lexer: if and while missing their colon

```ura
// error: expected ':' after the if condition

fn main() i32:
    x i32 = 5
    if x > 0
        output("positive\n")
    return 0
```

### stderr

```
error: test.ura:5:12 expected ':' after the if condition
  |
5 |     if x > 0
  |            ^
help: use if x > 0:
error: aborting due to 1 error
```

### status

```
1
```

---

## 212 — lexer: proto not followed by fn

```ura
// error: Expected 'fn' after 'proto'

proto x i32

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:3:9 expected '(' after the function name
  |
3 | proto x i32
  |         ^^^
help: use proto x() i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 213 — lexer: a struct declaration missing its name

```ura
// error: Expected a struct name after 'struct'

struct:
    x i32

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:3:7 expected a struct name after 'struct'
  |
3 | struct:
  |       ^
help: use struct Name:
error: aborting due to 1 error
```

### status

```
1
```

---

## 214 — lexer: an unexpected token in an expression

```ura
// error: unexpected '*'

fn main() i32:
    x i32 = *
    return 0
```

### stderr

```
error: test.ura:4:13 unexpected '*'
  |
4 |     x i32 = *
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 215 — lexer: member access with no member name after .

```ura
// error: Expected a member name after '.'
use "@/header"

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    p.
    return 0
```

### stderr

```
error: test.ura:10:6 expected an attribute or method name after '.'
   |
10 |     p.
   |      ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 216 — literal: bool literals: declare, zero-init, bool function, reassign

```ura
// stderr: exit: 9
// expect: 1

fn flag() bool:
    return True

fn main() i32:
    b bool = flag()
    c bool = False
    d bool
    c = True
    return 9
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @flag() {
entry:
  ret i1 true
}

define i32 @main() {
entry:
  %b = alloca i1, align 1
  %c = alloca i1, align 1
  %d = alloca i1, align 1
  %call.flag = call i1 @flag()
  store i1 %call.flag, i1* %b, align 1
  store i1 false, i1* %c, align 1
  store i1 false, i1* %d, align 1
  store i1 true, i1* %c, align 1
  ret i32 9
}
```

---

## 217 — literal: char literals: declare, print, escapes, in a function

```ura
// stdout: z
// stdout: !
// stdout: A
// expect: 0

fn grade() char:
    return 'A'

fn main() i32:
    c char = 'z'
    output(c, "\n")
    output('!', "\n")
    output(grade(), "\n")
    output('\n')
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"%c\00", align 1

define i8 @grade() {
entry:
  ret i8 65
}

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 122, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c" to i32
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %AS)
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 33)
  %call.grade = call i8 @grade()
  %AS2 = sext i8 %call.grade to i32
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %AS2)
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0), i32 10)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 218 — loop: basic while: count enemy kills

```ura
// stdout: <5>
// expect: 0

fn main() i32:
    kills i32 = 0
    while kills < 5:
        kills = kills + 1
    output("<", kills, ">\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @main() {
entry:
  %kills = alloca i32, align 4
  store i32 0, i32* %kills, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$kills" = load i32, i32* %kills, align 4
  %LT = icmp slt i32 %"$kills", 5
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$kills1" = load i32, i32* %kills, align 4
  %ADD = add i32 %"$kills1", 1
  store i32 %ADD, i32* %kills, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$kills2" = load i32, i32* %kills, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$kills2")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

