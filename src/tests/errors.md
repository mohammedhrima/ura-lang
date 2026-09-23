# errors

## index

- 001 — tokenizer: unexpected character
- 002 — tokenizer: unterminated string
- 003 — tokenizer: unterminated character
- 004 — tokenizer: empty character
- 005 — tokenizer: character with two letters
- 006 — tokenizer: integer too big for i32
- 008 — functions: missing name
- 009 — functions: unclosed parameters
- 010 — functions: missing colon after signature
- 011 — functions: parameter without a type
- 012 — functions: missing comma between parameters
- 013 — functions: variadic not last
- 014 — functions: unknown return type
- 015 — functions: unknown parameter type
- 016 — functions: parameter declared twice
- 017 — functions: defined twice
- 018 — functions: overloaded on return type only
- 019 — functions: return at top level
- 020 — structs: missing name
- 021 — structs: missing colon
- 022 — structs: expression inside a struct
- 023 — structs: attribute without a type
- 024 — structs: attribute declared twice
- 025 — structs: defined twice
- 026 — structs: no attributes
- 027 — structs: contains itself by value
- 028 — structs: unknown attribute
- 029 — structs: unknown method
- 030 — structs: method with wrong arguments
- 031 — structs: dot on a non-struct
- 032 — structs: bare attribute inside a method
- 033 — structs: bare method call inside a method
- 034 — structs: method called on the type
- 035 — structs: self outside a method
- 036 — variables: unknown type
- 037 — variables: declared twice
- 038 — variables: used before its declaration
- 039 — variables: used outside its block
- 040 — variables: assigned without a declaration
- 041 — refs: ref to a ref
- 042 — refs: missing type after &
- 043 — refs: initialized with a value
- 044 — refs: address of a literal
- 045 — refs: address of an expression
- 046 — refs: address of a call
- 047 — refs: address of an address
- 048 — refs: ref to a different type
- 049 — null: assigned to a value
- 050 — null: compared with a value
- 051 — null: address of null
- 052 — expressions: missing operand
- 053 — expressions: unclosed parenthesis
- 054 — expressions: unexpected parenthesis
- 055 — expressions: assign to a literal
- 056 — expressions: assign to a call
- 057 — expressions: compare a number and a string
- 058 — expressions: add a number and a string
- 059 — expressions: add a struct and a number
- 060 — expressions: compare structs
- 061 — expressions: and on numbers
- 062 — expressions: compound assign on chars
- 063 — expressions: negate a string
- 064 — calls: unknown function
- 065 — calls: too many arguments
- 066 — calls: wrong argument type
- 067 — calls: missing & on a ref argument
- 068 — calls: missing comma between arguments
- 069 — calls: unclosed call
- 070 — calls: trailing comma
- 071 — calls: call a variable
- 072 — calls: use a void result
- 073 — calls: output a struct
- 074 — arrays: unclosed type
- 075 — arrays: size in the type
- 076 — arrays: index a non-array
- 077 — arrays: index with a string
- 078 — arrays: mixed element types
- 079 — arrays: array into a scalar
- 080 — arrays: unclosed literal
- 081 — arrays: missing comma between elements
- 082 — types: string into i8
- 083 — types: bool into i32
- 084 — types: number into b1
- 085 — types: number too big for i8
- 086 — types: struct into another struct
- 087 — types: wrong return type
- 088 — types: value returned from a void fn
- 089 — types: bare return in a fn with a type
- 090 — types: number as an if condition
- 091 — types: number as a while condition
- 092 — control flow: break outside a loop
- 093 — control flow: continue outside a loop
- 094 — control flow: elif without if
- 095 — control flow: else without if
- 096 — control flow: missing colon after if
- 097 — control flow: missing colon after elif
- 098 — control flow: missing colon after else
- 099 — control flow: missing colon after while
- 100 — control flow: if without a condition
- 101 — control flow: else with a condition
- 102 — recovery: three analysis errors
- 103 — recovery: two syntax errors in two functions
- 104 — diagnostics: error past line 9
- 105 — templates: too many type arguments
- 106 — templates: used without type arguments
- 107 — templates: parameter is not a name
- 108 — templates: body holds something else
- 109 — templates: missing colon

---

## 001 — tokenizer: unexpected character

```ura
fn main() i32:
    x i32 = 5 $
    return x
```

### stderr

```
error: test.ura:2:15 unexpected character '$'
  |
2 |     x i32 = 5 $
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 002 — tokenizer: unterminated string

```ura
fn main() i32:
    s i8[] = "abc
    return 0
```

### stderr

```
error: test.ura:2:14 unterminated string
  |
2 |     s i8[] = "abc
  |              ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 003 — tokenizer: unterminated character

```ura
fn main() i32:
    c i8 = 'a
    return 0
```

### stderr

```
error: test.ura:2:12 unterminated character
  |
2 |     c i8 = 'a
  |            ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 004 — tokenizer: empty character

```ura
fn main() i32:
    c i8 = ''
    return 0
```

### stderr

```
error: test.ura:2:12 empty character literal
  |
2 |     c i8 = ''
  |            ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 005 — tokenizer: character with two letters

```ura
fn main() i32:
    c i8 = 'ab'
    return 0
```

### stderr

```
error: test.ura:2:12 a character holds one letter
  |
2 |     c i8 = 'ab'
  |            ^^^^
help: for a string, write '"ab"'
error: aborting due to 1 error
```

### status

```
1
```

---

## 006 — tokenizer: integer too big for i32

```ura
fn main() i32:
    x i32 = 99999999999
    return x
```

### stderr

```
error: test.ura:2:13 '99999999999' doesn't fit in 'i32'
  |
2 |     x i32 = 99999999999
  |             ^^^^^^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 008 — functions: missing name

```ura
fn () i32:
    return 0

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:4 expected a function name after 'fn'
  |
1 | fn () i32:
  |    ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 009 — functions: unclosed parameters

```ura
fn add(a i32:
    return a

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:13 expected ')' to close the parameters
  |
1 | fn add(a i32:
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 010 — functions: missing colon after signature

```ura
fn main() i32
    return 0
```

### stderr

```
error: test.ura:1:11 expected ':' after the function signature
  |
1 | fn main() i32
  |           ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 011 — functions: parameter without a type

```ura
fn f(a) i32:
    return 0

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:6 parameter 'a' needs a type
  |
1 | fn f(a) i32:
  |      ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 012 — functions: missing comma between parameters

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

## 013 — functions: variadic not last

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

## 014 — functions: unknown return type

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
error: aborting due to 1 error
```

### status

```
1
```

---

## 015 — functions: unknown parameter type

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
error: aborting due to 1 error
```

### status

```
1
```

---

## 016 — functions: parameter declared twice

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

## 017 — functions: defined twice

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

## 018 — functions: overloaded on return type only

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

## 019 — functions: return at top level

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
error: aborting due to 1 error
```

### status

```
1
```

---

## 020 — structs: missing name

```ura
struct :
    a i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 expected a struct name after 'struct'
  |
1 | struct :
  |        ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 021 — structs: missing colon

```ura
struct User
    a i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 expected ':' after 'struct User'
  |
1 | struct User
  |        ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 022 — structs: expression inside a struct

```ura
struct User:
    a i32
    1 + 2

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:7 a struct holds only attributes and methods
  |
3 |     1 + 2
  |       ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 023 — structs: attribute without a type

```ura
struct User:
    a

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:5 attribute 'a' needs a type
  |
2 |     a
  |     ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 024 — structs: attribute declared twice

```ura
struct User:
    a i32
    a i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:5 'a' is already declared
  |
3 |     a i32
  |     ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 025 — structs: defined twice

```ura
struct User:
    a i32

struct User:
    b i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:4:8 struct 'User' is already defined
  |
4 | struct User:
  |        ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 026 — structs: no attributes

```ura
struct E:

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 struct 'E' has no attributes
  |
1 | struct E:
  |        ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 027 — structs: contains itself by value

```ura
struct N:
    next N

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:10 struct 'N' can't contain itself
  |
2 |     next N
  |          ^
help: use a ref: '&N'
error: aborting due to 1 error
```

### status

```
1
```

---

## 028 — structs: unknown attribute

```ura
struct User:
    a i32

fn main() i32:
    u User
    return u.zz
```

### stderr

```
error: test.ura:6:14 struct 'User' has no attribute 'zz'
  |
6 |     return u.zz
  |              ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 029 — structs: unknown method

```ura
struct User:
    a i32

fn main() i32:
    u User
    return u.zz()
```

### stderr

```
error: test.ura:6:14 struct 'User' has no method 'zz'
  |
6 |     return u.zz()
  |              ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 030 — structs: method with wrong arguments

```ura
struct User:
    a i32

    fn add(n i32) i32:
        return self.a + n

fn main() i32:
    u User
    return u.add(1, 2)
```

### stderr

```
error: test.ura:9:14 no overload of 'add' takes these arguments
  |
9 |     return u.add(1, 2)
  |              ^^^
help: 'add' takes 1 argument, got 2
error: aborting due to 1 error
```

### status

```
1
```

---

## 031 — structs: dot on a non-struct

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

## 032 — structs: bare attribute inside a method

```ura
struct User:
    value i32

    fn get() i32:
        return value

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:5:16 'value' not found
  |
5 |         return value
  |                ^^^^^
help: did you mean 'self.value'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 033 — structs: bare method call inside a method

```ura
struct User:
    value i32

    fn get() i32:
        return self.value

    fn twice() i32:
        return get() * 2

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:8:16 function 'get' not found
  |
8 |         return get() * 2
  |                ^^^
help: did you mean 'self.get()'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 034 — structs: method called on the type

```ura
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    return User.get()
```

### stderr

```
error: test.ura:8:12 'User' is a type, not a value
  |
8 |     return User.get()
  |            ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 035 — structs: self outside a method

```ura
fn main() i32:
    return self.a
```

### stderr

```
error: test.ura:2:12 'self' not found
  |
2 |     return self.a
  |            ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 036 — variables: unknown type

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
error: aborting due to 1 error
```

### status

```
1
```

---

## 037 — variables: declared twice

```ura
fn main() i32:
    a i32 = 1
    a i32 = 2
    return a
```

### stderr

```
error: test.ura:3:5 'a' is already declared
  |
3 |     a i32 = 2
  |     ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 038 — variables: used before its declaration

```ura
fn main() i32:
    b i32 = a
    a i32 = 1
    return b
```

### stderr

```
error: test.ura:2:13 'a' not found
  |
2 |     b i32 = a
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 039 — variables: used outside its block

```ura
fn main() i32:
    if True:
        x i32 = 1
    return x
```

### stderr

```
error: test.ura:4:12 'x' not found
  |
4 |     return x
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 040 — variables: assigned without a declaration

```ura
fn main() i32:
    x = 5
    return 0
```

### stderr

```
error: test.ura:2:5 'x' not found
  |
2 |     x = 5
  |     ^
help: declare it: 'x i32 = 5'
error: aborting due to 1 error
```

### status

```
1
```

---

## 041 — refs: ref to a ref

```ura
fn main() i32:
    a i32 = 1
    p &&i32 = &a
    return 0
```

### stderr

```
error: test.ura:3:7 a ref can't point to a ref
  |
3 |     p &&i32 = &a
  |       ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 042 — refs: missing type after &

```ura
fn main() i32:
    a i32 = 1
    p & = &a
    return 0
```

### stderr

```
error: test.ura:3:7 expected a type after '&'
  |
3 |     p & = &a
  |       ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 043 — refs: initialized with a value

```ura
fn main() i32:
    a i32 = 1
    p &i32 = a
    return 0
```

### stderr

```
error: test.ura:3:14 'p' is a ref, initialize it with an address
  |
3 |     p &i32 = a
  |              ^
help: write 'p &i32 = &a'
error: aborting due to 1 error
```

### status

```
1
```

---

## 044 — refs: address of a literal

```ura
fn main() i32:
    p &i32 = &5
    return 0
```

### stderr

```
error: test.ura:2:15 '&' expects a variable or a struct attribute
  |
2 |     p &i32 = &5
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 045 — refs: address of an expression

```ura
fn main() i32:
    a i32 = 1
    p &i32 = &(a + 1)
    return 0
```

### stderr

```
error: test.ura:3:18 '&' expects a variable or a struct attribute
  |
3 |     p &i32 = &(a + 1)
  |                  ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 046 — refs: address of a call

```ura
fn f() i32:
    return 1

fn main() i32:
    p &i32 = &f()
    return 0
```

### stderr

```
error: test.ura:5:15 '&' expects a variable or a struct attribute
  |
5 |     p &i32 = &f()
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 047 — refs: address of an address

```ura
fn main() i32:
    a i32 = 1
    p &i32 = &&a
    return 0
```

### stderr

```
error: test.ura:3:14 can't take the address of an address
  |
3 |     p &i32 = &&a
  |              ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 048 — refs: ref to a different type

```ura
fn main() i32:
    c i8 = 'a'
    p &i32 = &c
    return 0
```

### stderr

```
error: test.ura:3:14 expected '&i32', found '&i8'
  |
3 |     p &i32 = &c
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 049 — null: assigned to a value

```ura
fn main() i32:
    x i32 = null
    return x
```

### stderr

```
error: test.ura:2:13 expected 'i32', found 'null'
  |
2 |     x i32 = null
  |             ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 050 — null: compared with a value

```ura
fn main() i32:
    a i32 = 1
    b b1 = a == null
    return 0
```

### stderr

```
error: test.ura:3:17 'null' needs a ref on the other side
  |
3 |     b b1 = a == null
  |                 ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 051 — null: address of null

```ura
fn main() i32:
    p &i32 = &null
    return 0
```

### stderr

```
error: test.ura:2:15 '&' expects a variable or a struct attribute
  |
2 |     p &i32 = &null
  |               ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 052 — expressions: missing operand

```ura
fn main() i32:
    a i32 = 1 +
    return a
```

### stderr

```
error: test.ura:2:15 expected an expression after '+'
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

## 053 — expressions: unclosed parenthesis

```ura
fn main() i32:
    a i32 = (1 + 2
    return a
```

### stderr

```
error: test.ura:2:13 unclosed '('
  |
2 |     a i32 = (1 + 2
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 054 — expressions: unexpected parenthesis

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

## 055 — expressions: assign to a literal

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

## 056 — expressions: assign to a call

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

## 057 — expressions: compare a number and a string

```ura
fn main() i32:
    a i32 = 1
    if a == "x":
        return 1
    return 0
```

### stderr

```
error: test.ura:3:13 expected 'i32', found 'i8[]'
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

## 058 — expressions: add a number and a string

```ura
fn main() i32:
    return 1 + "a"
```

### stderr

```
error: test.ura:2:14 can't apply '+' to 'i32' and 'i8[]'
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

## 059 — expressions: add a struct and a number

```ura
struct User:
    a i32

fn main() i32:
    u User
    return u + 1
```

### stderr

```
error: test.ura:6:14 can't apply '+' to 'User' and 'i32'
  |
6 |     return u + 1
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 060 — expressions: compare structs

```ura
struct User:
    a i32

fn main() i32:
    u User
    v User
    if u == v:
        return 1
    return 0
```

### stderr

```
error: test.ura:7:10 can't compare 'User' values
  |
7 |     if u == v:
  |          ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 061 — expressions: and on numbers

```ura
fn main() i32:
    if 1 and 2:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:8 expected 'b1', found 'i32'
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

## 062 — expressions: compound assign on chars

```ura
fn main() i32:
    s i8[] = "a"
    s += 1
    return 0
```

### stderr

```
error: test.ura:3:7 can't apply '+=' to 'i8[]' and 'i32'
  |
3 |     s += 1
  |       ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 063 — expressions: negate a string

```ura
fn main() i32:
    s i8[] = -"a"
    return 0
```

### stderr

```
error: test.ura:2:14 can't apply '-' to 'i8[]'
  |
2 |     s i8[] = -"a"
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 064 — calls: unknown function

```ura
fn main() i32:
    return zz()
```

### stderr

```
error: test.ura:2:12 function 'zz' not found
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

## 065 — calls: too many arguments

```ura
fn f(a i32) i32:
    return a

fn main() i32:
    return f(1, 2)
```

### stderr

```
error: test.ura:5:12 no overload of 'f' takes these arguments
  |
5 |     return f(1, 2)
  |            ^
help: 'f' takes 1 argument, got 2
error: aborting due to 1 error
```

### status

```
1
```

---

## 066 — calls: wrong argument type

```ura
fn f(a i32) i32:
    return a

fn main() i32:
    return f("a")
```

### stderr

```
error: test.ura:5:12 no overload of 'f' takes these arguments
  |
5 |     return f("a")
  |            ^
help: '"a"' is 'i8[]', but 'a' wants 'i32'
error: aborting due to 1 error
```

### status

```
1
```

---

## 067 — calls: missing & on a ref argument

```ura
fn f(p &i32):
    p = 1

fn main() i32:
    a i32 = 0
    f(a)
    return a
```

### stderr

```
error: test.ura:6:5 no overload of 'f' takes these arguments
  |
6 |     f(a)
  |     ^
help: 'a' is 'i32', but 'p' wants '&i32'
help: pass its address: '&a'
error: aborting due to 1 error
```

### status

```
1
```

---

## 068 — calls: missing comma between arguments

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

## 069 — calls: unclosed call

```ura
fn f(a i32, b i32) i32:
    return a

fn main() i32:
    x i32 = f(1, 2
    return x
```

### stderr

```
error: test.ura:5:13 expected ')' to close the call to 'f'
  |
5 |     x i32 = f(1, 2
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 070 — calls: trailing comma

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

## 071 — calls: call a variable

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

## 072 — calls: use a void result

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

## 073 — calls: output a struct

```ura
struct User:
    a i32

fn main() i32:
    u User
    output(u)
    return 0
```

### stderr

```
error: test.ura:6:12 can't output 'User'
  |
6 |     output(u)
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 074 — arrays: unclosed type

```ura
fn main() i32:
    x i32[
    return 0
```

### stderr

```
error: test.ura:2:10 expected ']' after '['
  |
2 |     x i32[
  |          ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 075 — arrays: size in the type

```ura
fn main() i32:
    x i32[3]
    return 0
```

### stderr

```
error: test.ura:2:11 expected ']' after '['
  |
2 |     x i32[3]
  |           ^
help: arrays have no size in the type: 'i32[]'
error: aborting due to 1 error
```

### status

```
1
```

---

## 076 — arrays: index a non-array

```ura
fn main() i32:
    a i32 = 1
    return a[0]
```

### stderr

```
error: test.ura:3:12 'a' is 'i32', not an array
  |
3 |     return a[0]
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 077 — arrays: index with a string

```ura
fn main() i32:
    x i32[] = [1, 2]
    return x["a"]
```

### stderr

```
error: test.ura:3:14 expected 'i32', found 'i8[]'
  |
3 |     return x["a"]
  |              ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 078 — arrays: mixed element types

```ura
fn main() i32:
    x i32[] = [1, "a"]
    return 0
```

### stderr

```
error: test.ura:2:19 expected 'i32', found 'i8[]'
  |
2 |     x i32[] = [1, "a"]
  |                   ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 079 — arrays: array into a scalar

```ura
fn main() i32:
    a i32 = [1, 2]
    return a
```

### stderr

```
error: test.ura:2:13 expected 'i32', found 'i32[]'
  |
2 |     a i32 = [1, 2]
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 080 — arrays: unclosed literal

```ura
fn main() i32:
    x i32[] = [1, 2
    return 0
```

### stderr

```
error: test.ura:2:15 unclosed '['
  |
2 |     x i32[] = [1, 2
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 081 — arrays: missing comma between elements

```ura
fn main() i32:
    x i32[] = [1 2]
    return 0
```

### stderr

```
error: test.ura:2:18 expected ',' between elements
  |
2 |     x i32[] = [1 2]
  |                  ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 082 — types: string into i8

```ura
fn main() i32:
    x i8 = "a"
    return 0
```

### stderr

```
error: test.ura:2:12 expected 'i8', found 'i8[]'
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

## 083 — types: bool into i32

```ura
fn main() i32:
    a i32 = True
    return a
```

### stderr

```
error: test.ura:2:13 expected 'i32', found 'b1'
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

## 084 — types: number into b1

```ura
fn main() i32:
    b b1 = 1
    return 0
```

### stderr

```
error: test.ura:2:12 expected 'b1', found 'i32'
  |
2 |     b b1 = 1
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 085 — types: number too big for i8

```ura
fn main() i32:
    x i8 = 300
    return 0
```

### stderr

```
error: test.ura:2:12 '300' doesn't fit in 'i8'
  |
2 |     x i8 = 300
  |            ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 086 — types: struct into another struct

```ura
struct User:
    a i32

struct Addr:
    b i32

fn main() i32:
    v Addr
    u User = v
    return 0
```

### stderr

```
error: test.ura:9:14 expected 'User', found 'Addr'
  |
9 |     u User = v
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 087 — types: wrong return type

```ura
fn f() i32:
    return "a"

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:12 expected 'i32', found 'i8[]'
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

## 088 — types: value returned from a void fn

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

## 089 — types: bare return in a fn with a type

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

## 090 — types: number as an if condition

```ura
fn main() i32:
    if 5:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:8 expected 'b1', found 'i32'
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

## 091 — types: number as a while condition

```ura
fn main() i32:
    a i32 = 3
    while a:
        a -= 1
    return a
```

### stderr

```
error: test.ura:3:11 expected 'b1', found 'i32'
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

## 092 — control flow: break outside a loop

```ura
fn main() i32:
    break
    return 0
```

### stderr

```
error: test.ura:2:5 'break' outside of a loop
  |
2 |     break
  |     ^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 093 — control flow: continue outside a loop

```ura
fn main() i32:
    continue
    return 0
```

### stderr

```
error: test.ura:2:5 'continue' outside of a loop
  |
2 |     continue
  |     ^^^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 094 — control flow: elif without if

```ura
fn main() i32:
    elif True:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:5 'elif' without a matching 'if'
  |
2 |     elif True:
  |     ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 095 — control flow: else without if

```ura
fn main() i32:
    else:
        return 1
    return 0
```

### stderr

```
error: test.ura:2:5 'else' without a matching 'if'
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

## 096 — control flow: missing colon after if

```ura
fn main() i32:
    a i32 = 2
    if a > 1
        return 1
    return 0
```

### stderr

```
error: test.ura:3:12 expected ':' after the 'if' condition
  |
3 |     if a > 1
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 097 — control flow: missing colon after elif

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
error: test.ura:5:14 expected ':' after the 'elif' condition
  |
5 |     elif a > 0
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 098 — control flow: missing colon after else

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
error: test.ura:5:5 expected ':' after 'else'
  |
5 |     else
  |     ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 099 — control flow: missing colon after while

```ura
fn main() i32:
    a i32 = 2
    while a > 0
        a -= 1
    return a
```

### stderr

```
error: test.ura:3:15 expected ':' after the 'while' condition
  |
3 |     while a > 0
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 100 — control flow: if without a condition

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

## 101 — control flow: else with a condition

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
error: test.ura:5:10 'else' takes no condition
  |
5 |     else a > 0:
  |          ^
help: use 'elif'
error: aborting due to 1 error
```

### status

```
1
```

---

## 102 — recovery: three analysis errors

```ura
fn main() i32:
    a i32 = zz
    b i32 = u.x
    return yy()
```

### stderr

```
error: test.ura:2:13 'zz' not found
  |
2 |     a i32 = zz
  |             ^^
error: test.ura:3:13 'u' not found
  |
3 |     b i32 = u.x
  |             ^
error: test.ura:4:12 function 'yy' not found
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

## 103 — recovery: two syntax errors in two functions

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
error: test.ura:1:8 expected ':' after the function signature
  |
1 | fn f() i32
  |        ^^^
error: test.ura:5:12 unclosed '('
  |
5 |     return (2
  |            ^
error: aborting due to 2 errors
```

### status

```
1
```
---

## 104 — diagnostics: error past line 9

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
error: test.ura:11:12 'zz' not found
   |
11 |     return zz
   |            ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 105 — templates: too many type arguments

```ura
template<T>:
   struct Box:
      value T

fn main() i32:
   b Box<i32, i8>
   return 0
```

### stderr

```
error: test.ura:6:6 'Box' takes 1 type argument, got 2
  |
6 |    b Box<i32, i8>
  |      ^^^
help: it is declared 'template<T>'
error: aborting due to 1 error
```

### status

```
1
```

---

## 106 — templates: used without type arguments

```ura
template<T>:
   struct Box:
      value T

fn main() i32:
   b Box
   return 0
```

### stderr

```
error: test.ura:6:6 expected '<' after 'Box'
  |
6 |    b Box
  |      ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 107 — templates: parameter is not a name

```ura
template<3>:
   struct Box:
      value i32

fn main() i32:
   return 0
```

### stderr

```
error: test.ura:1:10 expected names after template
  |
1 | template<3>:
  |          ^
help: use template<T, V>
error: aborting due to 1 error
```

### status

```
1
```

---

## 108 — templates: body holds something else

```ura
template<T>:
   value T

fn main() i32:
   return 0
```

### stderr

```
error: test.ura:2:4 expected struct/fn after template
  |
2 |    value T
  |    ^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 109 — templates: missing colon

```ura
template<T>
   struct Box:
      value T

fn main() i32:
   return 0
```

### stderr

```
error: test.ura:1:11 expected ':' after 'template'
  |
1 | template<T>
  |           ^
error: aborting due to 1 error
```

### status

```
1
```

---
