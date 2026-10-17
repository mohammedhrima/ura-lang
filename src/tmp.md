## errors 1: tokenizer

# unknown character
```ura
// error: unknown character '$'
// help: remove it
fn main() i32:
    x i32 = 5 $ 3
    return x
```

# unclosed string
```ura
// error: unclosed string
// help: add " at the end
fn main() i32:
    s i8[] = "abc
    return 0
```

# unclosed character
```ura
// error: unclosed character
// help: add ' after the character
fn main() i32:
    c char = 'a
    return 0
```

# empty character
```ura
// error: empty character
// help: put one character between the quotes, like 'a'
fn main() i32:
    c char = ''
    return 0
```

# character with two letters
```ura
// error: a character holds one letter, not 2
// help: use "ab" for a string
fn main() i32:
    c char = 'ab'
    return 0
```

# unknown escape
```ura
// error: unknown escape '\q'
// help: the escapes are \n \t \r \0 \\ \" \'
fn main() i32:
    s i8[] = "a\qb"
    return 0
```

# unclosed comment
```ura
// error: unclosed comment
// help: add */ to close it
fn main() i32:
    /* this never ends
    return 0
```

# number too big for i32
```ura
// error: 99999999999 doesn't fit in i32
// help: i32 goes from -2147483648 to 2147483647
fn main() i32:
    x i32 = 99999999999
    return 0
```

# indentation that is not a multiple of 4
```ura
// error: indentation of 3 spaces
// help: indent with a multiple of 4 spaces
fn main() i32:
   return 0
```

# two errors in one file: both are shown
```ura
// error: unknown character '$'
// error: unknown character '@'
fn main() i32:
    x i32 = 5 $ 3
    y i32 = 2 @ 1
    return 0
```
