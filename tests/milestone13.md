# milestone13

## index

- 001 — builtin: default argument promotion in a user variadic call
- 002 — directive: @if/@elif/@else selects the matching platform branch
- 003 — directive: @if unix matches on macOS and linux
- 004 — directive: a non-matching branch is dropped before parsing
- 005 — directive: a platform-gated top-level proto
- 006 — directive: @if selects a value inside a function
- 007 — directive: without @no-warn, operator drop alone warns
- 008 — directive: @no-warn above operator drop silences that warning
- 009 — directive: @no-warn covers only the declaration it precedes
- 010 — function: fn-typed value: assign top-level fn to a local, call indirectly
- 011 — function: fn-typed parameter: callback passed to higher-order fn
- 012 — function: store a function in a fn-typed variable, then call it
- 013 — function: pass a function as an argument (higher-order)
- 014 — function: nested non-capturing helper function
- 015 — function: a function-typed parameter given a value
- 016 — function: proto then definition, and definition then proto
- 017 — function: signature conflict with a prelude proto
- 018 — function: conflict between two of your own protos
- 019 — function: calling a null (zero-init) function value traps with file:line
- 020 — function: infinite recursion overflows the stack
- 021 — function: ret is the short spelling of return
- 022 — function: ret and return mix freely in one file
- 023 — function: a bare ret in a void function
- 024 — function: ret ref returns a reference
- 025 — project: calc
- 026 — project: libft (library and its main)
- 027 — project: net server
- 028 — project: net client
- 029 — dungeon: floor04
- 030 — dungeon: floor09b
- 031 — dungeon: floor14a
- 032 — dungeon: boss
- 033 — project: calculator — tokenize, parse, print the AST and evaluate with try/catch
- 034 — libft: isalnum
- 035 — libft: isalpha
- 036 — libft: isdigit
- 037 — libft: lstnew — new nodes, add_back, add_front and size
- 038 — libft: lstlast — returns a reference to the last node
- 039 — libft: lstiter — apply a function to each node's content in place
- 040 — libft: lstdelone — unlink a node by index and clear the list

---

## 001 — builtin: default argument promotion in a user variadic call

```ura
// stdout: 3.50 A 7 1 42
// stdout: 123456789
// stdout: 2.50 Z 9
// stdout: 7.00 43
// stdout: 1 0
// stdout: none
// expect: 0

proto fn printf(format pointer, ...) i32

fn main() i32:
    f f32 = 3.5
    c char  = 'A'
    s i16 = 7 as i16
    b bool  = True
    l i64  = 123456789 as i64
    i i32   = 42

    printf("%.2f %c %d %d %d\n", f, c, s, b, i)
    printf("%ld\n", l)
    printf("%.2f %c %d\n", 2.5, 'Z', 9)
    printf("%.2f %d\n", f + f, i + 1)
    printf("%d %d\n", 1 < 2, 3 == 4)
    printf("none\n")
    return 0
```

---

## 002 — directive: @if/@elif/@else selects the matching platform branch

```ura
// stdout: mac
// expect: 0
fn main() i32:
    @if macos:
        output("mac\n")
    @elif linux:
        output("linux\n")
    @else:
        output("other\n")
    return 0
```

---

## 003 — directive: @if unix matches on macOS and linux

```ura
// stdout: posix
// expect: 0
fn main() i32:
    @if unix:
        output("posix\n")
    @else:
        output("windows\n")
    return 0
```

---

## 004 — directive: a non-matching branch is dropped before parsing

```ura
// stdout: kept
// expect: 0
fn main() i32:
    @if linux:
        undefined_that_would_error()
    @else:
        output("kept\n")
    return 0
```

---

## 005 — directive: a platform-gated top-level proto

```ura
// stdout: True
// expect: 0
@if unix:
    proto fn getpid() i32
@else:
    proto fn GetCurrentProcessId() u32

fn main() i32:
    output(getpid() > 0, "\n")
    return 0
```

---

## 006 — directive: @if selects a value inside a function

```ura
// stdout: 1
// expect: 0
fn tag() i32:
    @if macos:
        return 1
    @elif linux:
        return 2
    @else:
        return 0

fn main() i32:
    output(tag(), "\n")
    return 0
```

---

## 007 — directive: without @no-warn, operator drop alone warns

```ura
// stdout: dropping 1
// stderr: warning: Struct Owned has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
// expect: 1

struct Owned:
    tag i32

    operator drop:
        output("dropping ", self.tag, "\n")

fn main() i32:
    o Owned
    o.tag = 1
    return 0
```

---

## 008 — directive: @no-warn above operator drop silences that warning

```ura
// stdout: dropping 1
// expect: 0

struct Owned:
    tag i32

    @no-warn
    operator drop:
        output("dropping ", self.tag, "\n")

fn main() i32:
    o Owned
    o.tag = 1
    return 0
```

---

## 009 — directive: @no-warn covers only the declaration it precedes

```ura
// stdout: loud 2
// stdout: quiet 1
// stderr: warning: Struct Loud has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
// expect: 1

struct Quiet:
    tag i32

    @no-warn
    operator drop:
        output("quiet ", self.tag, "\n")

struct Loud:
    tag i32

    operator drop:
        output("loud ", self.tag, "\n")

fn main() i32:
    q Quiet
    l Loud
    q.tag = 1
    l.tag = 2
    return 0
```

---

## 010 — function: fn-typed value: assign top-level fn to a local, call indirectly

```ura
// stdout: 5
// stdout: 6
// expect: 0

fn add(a i32, b i32) i32:
    return a + b

fn sub(a i32, b i32) i32:
    return a - b

fn main() i32:
    op fn(i32, i32) i32 = add
    output(op(2, 3), "\n")

    op = sub
    output(op(10, 4), "\n")
    return 0
```

---

## 011 — function: fn-typed parameter: callback passed to higher-order fn

```ura
// stdout: 42
// stdout: 21
// expect: 0

fn double_it(n i32) i32:
    return n * 2

fn triple_it(n i32) i32:
    return n * 3

fn apply(cb fn(i32) i32, x i32) i32:
    return cb(x)

fn main() i32:
    output(apply(double_it, 21), "\n")
    output(apply(triple_it, 7), "\n")
    return 0
```

---

## 012 — function: store a function in a fn-typed variable, then call it

```ura
// stderr: exit: 42
// expect: 1

fn twice(n i32) i32:
    return n + n
fn main() i32:
    f fn(i32) i32 = twice
    return f(21)
```

---

## 013 — function: pass a function as an argument (higher-order)

```ura
// stderr: exit: 42
// expect: 1

fn twice(n i32) i32:
    return n + n
fn apply(f fn(i32) i32, x i32) i32:
    return f(x)
fn main() i32:
    return apply(twice, 21)
```

---

## 014 — function: nested non-capturing helper function

```ura
// stderr: exit: 42
// expect: 1

fn main() i32:
    fn dbl(k i32) i32:
        return k + k
    return dbl(21)
```

---

## 015 — function: a function-typed parameter given a value

```ura
// error: Argument 1 type mismatch in call to 'apply'

fn twice(n i32) i32:
    return n + n

fn apply(f fn(i32) i32, x i32) i32:
    return f(x)

fn main() i32:
    return apply(5, 21)
```

---

## 016 — function: proto then definition, and definition then proto

```ura
// stdout: 42 42
// expect: 0

proto fn tally(n i32) i32

fn tally(n i32) i32:
    return n + 1

fn double(n i32) i32:
    return n * 2

proto fn double(n i32) i32

fn main() i32:
    output(tally(41), " ", double(21), "\n")
    return 0
```

---

## 017 — function: signature conflict with a prelude proto

```ura
// error: Conflicting declaration of 'write': parameter 3 is i32, expected i64

proto fn write(fd i32, ptr pointer, len i32) i32

fn main() i32:
    output("unreachable\n")
    return 0
```

---

## 018 — function: conflict between two of your own protos

```ura
// error: Conflicting declaration of 'scan': expected 2 parameter(s), found 3

proto fn scan(a i32, b pointer) i32

proto fn scan(a i32, b pointer, c i32) i32

fn main() i32:
    output("unreachable\n")
    return 0
```

---

## 019 — function: calling a null (zero-init) function value traps with file:line

```ura
// stderr: runtime error: Call to a null function value
// expect: 1

fn main() i32:
    f fn(i32) i32
    return f(21)
```

---

## 020 — function: infinite recursion overflows the stack

```ura
// stderr: exit: 139
// expect: 1

fn recurse(n i32) i32:
    return recurse(n)

fn main() i32:
    return recurse(1)
```

---

## 021 — function: ret is the short spelling of return

```ura
// stdout: 100 0
// stdout: True False
// expect: 0

fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo: ret lo
    if val > hi: ret hi
    ret val

fn is_alive(hp i32) bool: ret hp > 0

fn main() i32:
    output(clamp(120, 0, 100), " ", clamp(-5, 0, 100), "\n")
    output(is_alive(3), " ", is_alive(0), "\n")
    return 0
```

---

## 022 — function: ret and return mix freely in one file

```ura
// stdout: 8 -3
// expect: 0

fn short(n i32) i32:
    ret n * 2

fn long(n i32) i32:
    return n + 1

fn both(n i32) i32:
    if n > 0: ret short(n)
    return long(n)

fn main() i32:
    output(both(4), " ", both(-4), "\n")
    return 0
```

---

## 023 — function: a bare ret in a void function

```ura
// stdout: already down
// stdout: still standing: 12
// expect: 0

fn announce(hp i32):
    if hp <= 0:
        output("already down\n")
        ret
    output("still standing: ", hp, "\n")

fn main() i32:
    announce(0)
    announce(12)
    return 0
```

---

## 024 — function: ret ref returns a reference

```ura
// stdout: 25
// expect: 0

struct Hero:
    hp i32

fn strongest(party Hero[]) &Hero:
    best i32 = 0
    for i in 1..party.len:
        if party[i].hp > party[best].hp: best = i
    ret &party[best]

fn main() i32:
    party Hero[] = new Hero[3]
    party[0].hp = 10
    party[1].hp = 40
    party[2].hp = 25
    champ &Hero = strongest(party)
    champ.hp = champ.hp - 15
    output(party[1].hp, "\n")
    clean party
    return 0
```

---

## 025 — project: calc

```ura
// expect: 0
// stdin and output depend on the program; run it by hand
use "@/header"
use "@/error"

enum Type: NONE, NUMBER, ADD, SUB, MUL, DIV, LPAREN, RPAREN

struct Token:
    value i32
    type Type

    pub fn number(v i32) &Token:
        t &Token = new Token
        t.type = NUMBER
        t.value = v
        ret &t

    pub fn op(k Type) &Token:
        t &Token = new Token
        t.type = k
        ret &t

struct Node:
    token &Token
    left &Node
    right &Node

    pub fn create(token &Token) &Node:
        n &Node = new Node
        n.token = &token
        ret &n

    @no-warn
    operator drop:
        clean self.token
        clean self.left
        clean self.right

mod Lexer:
    fn run(src &String, toks Token[]) i32:
        fn is_digit(c char) bool: ret c >= '0' and c <= '9'
        n i32 = 0
        i i32 = 0
        while i < src.len():
            c char = src.at(i)
            if c == ' ':
                i = i + 1
                continue
            elif is_digit(c):
                v i32 = 0
                while i < src.len() and is_digit(src.at(i)):
                    v = v * 10 + ((src.at(i) as i32) - 48)
                    i = i + 1
                toks[n].type = NUMBER
                toks[n].value = v
                n = n + 1
                continue
            else:
                k Type = NUMBER
                match c:
                    case '+': k = ADD
                    case '-': k = SUB
                    case '*': k = MUL
                    case '/': k = DIV
                    case '(': k = LPAREN
                    case ')': k = RPAREN
                toks[n].type = k
                toks[n].value = 0
                n = n + 1
                i = i + 1
        ret n

mod Parser:
    fn factor(toks Token[], p &i32, n i32) &Node:
        if p < n and toks[p].type == LPAREN:
            p = p + 1
            inner &Node = expr(toks, &p, n)
            if p < n and toks[p].type == RPAREN:
                p = p + 1
            ret &inner
        lf &Node = Node::create(Token::number(toks[p].value))
        p = p + 1
        ret &lf

    fn term(toks Token[], p &i32, n i32) &Node:
        l &Node = factor(toks, &p, n)
        while p < n and (toks[p].type == MUL or toks[p].type == DIV):
            k Type = toks[p].type
            p = p + 1
            r &Node = factor(toks, &p, n)
            parent &Node = Node::create(Token::op(k))
            parent.left = &l
            parent.right = &r
            l = &parent
        ret &l

    fn expr(toks Token[], p &i32, n i32) &Node:
        l &Node = term(toks, &p, n)
        while p < n and (toks[p].type == ADD or toks[p].type == SUB):
            k Type = toks[p].type
            p = p + 1
            r &Node = term(toks, &p, n)
            parent &Node = Node::create(Token::op(k))
            parent.left = &l
            parent.right = &r
            l = &parent
        ret &l

fn sign(k Type) char[]:
    match k:
        case ADD: ret "+"
        case SUB: ret "-"
        case MUL: ret "*"
        case DIV: ret "/"
        default: ret "?"

fn label(n &Node):
    if n.token.type == NUMBER: output(n.token.value, "\n")
    else:                      output("op: ", sign(n.token.type), "\n")

fn show(n &Node, pre char[], last bool):
    output(pre)
    if last: output("└─ ")
    else:    output("├─ ")
    label(&n)
    kid String = String::from(pre)
    if last: kid.join("   ")
    else:    kid.join("│  ")
    if n.left != null:  show(&n.left, kid.c_str(), n.right == null)
    if n.right != null: show(&n.right, kid.c_str(), True)

fn tree(n &Node):
    label(&n)
    if n.left != null:  show(&n.left, "", n.right == null)
    if n.right != null: show(&n.right, "", True)

fn eval(n &Node) i32:
    match n.token.type:
        case NUMBER: ret n.token.value
        case ADD:    ret eval(&n.left) + eval(&n.right)
        case SUB:    ret eval(&n.left) - eval(&n.right)
        case MUL:    ret eval(&n.left) * eval(&n.right)
        case DIV:
            d i32 = eval(&n.right)
            if d == 0: throw Error::make("division by zero")
            ret eval(&n.left) / d
        default:    ret 0

fn calc(text char[]):
    output("========================================\n")
    output("calculate expr:  ", text, "\nast:\n")
    src String = String::from(text)
    toks Token[] = new Token[64]
    n i32 = Lexer::run(&src, toks)
    p i32 = 0
    root &Node = Parser::expr(toks, &p, n)
    tree(&root)
    try:
        output("result: ", eval(&root), "\n")
    catch e:
        errput("\nError: ", e.message, "\n")
    clean root
    clean toks

fn main() i32:
    calc("2 * (3 + 4) - 10 / 5")
    calc("8 / (3 - 3)")
    output("========================================\n")
    return 0
```

---

## 026 — project: libft (library and its main)

```ura
// expect: 0
// ---- libft.ura
fn ft_isalpha(c char) bool:
    return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z')

fn ft_isdigit(c char) bool:
    return c >= '0' and c <= '9'

fn ft_isalnum(c char) bool:
    return ft_isalpha(c) or ft_isdigit(c)

fn ft_isascii(c char) bool:
    return c >= 0 as char and c <= 127 as char

fn ft_isprint(c char) bool:
    return c >= 32 as char and c <= 126 as char

fn ft_toupper(c char) char:
    if c >= 'a' and c <= 'z':
        return (c as i32 - 32) as char
    return c

fn ft_tolower(c char) char:
    if c >= 'A' and c <= 'Z':
        return (c as i32 + 32) as char
    return c

fn ft_strlen(s char[]) i32:
    return s.len as i32

fn ft_strncmp(a char[], b char[], n i32) i32:
    i i32 = 0
    while i < n:
        ca i32 = 0
        cb i32 = 0
        if i < a.len as i32:
            ca = a[i] as i32
        if i < b.len as i32:
            cb = b[i] as i32
        if ca != cb:
            return ca - cb
        if ca == 0:
            return 0
        i += 1
    return 0

fn ft_memcmp(a char[], b char[], n i32) i32:
    i i32 = 0
    while i < n:
        if a[i] != b[i]:
            return a[i] as i32 - b[i] as i32
        i += 1
    return 0

fn ft_strchr(s char[], c char) i32:
    i i32 = 0
    while i < s.len as i32:
        if s[i] == c:
            return i
        i += 1
    if c == 0 as char:
        return s.len as i32
    return -1

fn ft_strrchr(s char[], c char) i32:
    i i32 = s.len as i32 - 1
    while i >= 0:
        if s[i] == c:
            return i
        i -= 1
    return -1

fn ft_memchr(s char[], c char, n i32) i32:
    i i32 = 0
    while i < n:
        if s[i] == c:
            return i
        i += 1
    return -1

fn ft_strnstr(hay char[], needle char[], n i32) i32:
    m i32 = needle.len as i32
    if m == 0:
        return 0
    i i32 = 0
    while i + m <= n and i + m <= hay.len as i32:
        if ft_strncmp(hay[i..i + m], needle, m) == 0:
            return i
        i += 1
    return -1

fn ft_atoi(s char[]) i32:
    i i32 = 0
    while i < s.len as i32 and (s[i] == ' ' or s[i] == '\t' or s[i] == '\n'):
        i += 1
    sign i32 = 1
    if i < s.len as i32 and (s[i] == '-' or s[i] == '+'):
        if s[i] == '-':
            sign = -1
        i += 1
    n i32 = 0
    while i < s.len as i32 and ft_isdigit(s[i]):
        n = n * 10 + (s[i] as i32 - 48)
        i += 1
    return n * sign

fn ft_memset(b char[], c char, n i32):
    i i32 = 0
    while i < n:
        b[i] = c
        i += 1

fn ft_bzero(b char[], n i32):
    ft_memset(b, 0 as char, n)

fn ft_memcpy(dst char[], src char[], n i32):
    i i32 = 0
    while i < n:
        dst[i] = src[i]
        i += 1

fn ft_memmove(dst char[], src char[], n i32):
    tmp char[] = new char[n]
    ft_memcpy(tmp, src, n)
    ft_memcpy(dst, tmp, n)
    clean tmp

fn ft_strlcpy(dst char[], src char[], size i32) i32:
    slen i32 = src.len as i32
    i i32 = 0
    while i + 1 < size and i < slen:
        dst[i] = src[i]
        i += 1
    if size > 0:
        dst[i] = 0 as char
    return slen

fn ft_strlcat(dst char[], src char[], size i32) i32:
    dlen i32 = 0
    while dlen < size and dlen < dst.len as i32 and dst[dlen] != 0 as char:
        dlen += 1
    slen i32 = src.len as i32
    if dlen == size:
        return size + slen
    i i32 = 0
    while dlen + i + 1 < size and i < slen:
        dst[dlen + i] = src[i]
        i += 1
    dst[dlen + i] = 0 as char
    return dlen + slen

fn ft_strdup(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    ft_memcpy(out, s, n)
    return out

fn ft_calloc(count i32, size i32) char[]:
    return new char[count * size]

fn ft_substr(s char[], start i32, len i32) char[]:
    slen i32 = s.len as i32
    if start >= slen:
        return new char[0]
    stop i32 = start + len
    if stop > slen:
        stop = slen
    n i32 = stop - start
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        out[i] = s[start + i]
        i += 1
    return out

fn ft_strjoin(a char[], b char[]) char[]:
    la i32 = a.len as i32
    lb i32 = b.len as i32
    out char[] = new char[la + lb]
    ft_memcpy(out, a, la)
    i i32 = 0
    while i < lb:
        out[la + i] = b[i]
        i += 1
    return out

fn ft_in_set(c char, set char[]) bool:
    i i32 = 0
    while i < set.len as i32:
        if set[i] == c:
            return True
        i += 1
    return False

fn ft_strtrim(s char[], set char[]) char[]:
    slen i32 = s.len as i32
    start i32 = 0
    while start < slen and ft_in_set(s[start], set):
        start += 1
    stop i32 = slen
    while stop > start and ft_in_set(s[stop - 1], set):
        stop -= 1
    return ft_substr(s, start, stop - start)

fn ft_count_words(s char[], c char) i32:
    slen i32 = s.len as i32
    wc i32 = 0
    i i32 = 0
    while i < slen:
        if s[i] != c:
            wc += 1
            while i < slen and s[i] != c:
                i += 1
        else:
            i += 1
    return wc

fn ft_split(s char[], c char) char[][]:
    slen i32 = s.len as i32
    wc i32 = ft_count_words(s, c)
    if wc == 0:
        return new char[0][1]
    out char[][] = new char[wc][1]
    w i32 = 0
    i i32 = 0
    while i < slen:
        if s[i] != c:
            start i32 = i
            while i < slen and s[i] != c:
                i += 1
            out[w] = ft_substr(s, start, i - start)
            w += 1
        else:
            i += 1
    return out

fn ft_num_digits(n i32) i32:
    d i32 = 0
    tmp i32 = n
    while tmp != 0:
        tmp = tmp / 10
        d += 1
    return d

fn ft_itoa(n i32) char[]:
    if n == 0:
        z char[] = new char[1]
        z[0] = '0'
        return z
    neg bool = n < 0
    digits i32 = ft_num_digits(n)
    total i32 = digits
    if neg:
        total += 1
    out char[] = new char[total]
    num i32 = n
    idx i32 = total - 1
    i i32 = 0
    while i < digits:
        d i32 = num % 10
        if d < 0:
            d = -d
        out[idx] = (d + 48) as char
        num = num / 10
        idx -= 1
        i += 1
    if neg:
        out[0] = '-'
    return out

fn ft_strmapi(s char[], f fn(u32, char) char) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        out[i] = f(i as u32, s[i])
        i += 1
    return out

fn ft_striteri(s char[], f fn(u32, char) char) void:
    i i32 = 0
    while i < s.len as i32:
        s[i] = f(i as u32, s[i])
        i += 1

fn ft_putchar_fd(c char, fd i32):
    b char[] = new char[1]
    b[0] = c
    write(fd, b, 1)
    clean b

fn ft_putstr_fd(s char[], fd i32):
    write(fd, s, s.len as i64)

fn ft_putendl_fd(s char[], fd i32):
    ft_putstr_fd(s, fd)
    ft_putchar_fd('\n', fd)

fn ft_putnbr_fd(n i32, fd i32):
    s char[] = ft_itoa(n)
    ft_putstr_fd(s, fd)
    clean s

struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_front(lst &Lst, node &Node):
    node.next = &lst.head
    lst.head  = &node

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn ft_lstsize(lst &Lst) i32:
    cur &Node = &lst.head
    count i32 = 0
    while cur != null:
        count += 1
        cur = &cur.next
    return count

fn ft_lstlast(lst &Lst) &Node:
    cur &Node = &lst.head
    if cur == null:
        return null
    while cur.next != null:
        cur = &cur.next
    return &cur

fn ft_lstiter(lst &Lst, f fn(char[]) char[]) void:
    cur &Node = &lst.head
    while cur != null:
        cur.content = f(cur.content)
        cur = &cur.next

fn ft_lstmap(lst &Lst, f fn(char[]) char[]) Lst:
    out Lst
    cur &Node = &lst.head
    while cur != null:
        ft_lstadd_back(&out, ft_lstnew(f(cur.content)))
        cur = &cur.next
    return out

fn ft_lstdelone(lst &Lst, idx i32):
    if lst.head == null:
        return
    if idx <= 0:
        lst.head = &lst.head.next
        return
    prev &Node = &lst.head
    i i32 = 1
    while i < idx and prev.next != null:
        prev = &prev.next
        i += 1
    if prev.next != null:
        prev.next = &prev.next.next

fn ft_lstclear(lst &Lst):
    lst.head = null
// ---- main.ura
use "libft"

fn upcase_i(i u32, c char) char:
    if c >= 'a' and c <= 'z':
        return (c as i32 - 32) as char
    return c

fn upcase(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        c char = s[i]
        if c >= 'a' and c <= 'z':
            out[i] = (c as i32 - 32) as char
        else:
            out[i] = c
        i += 1
    return out

fn lower(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        c char = s[i]
        if c >= 'A' and c <= 'Z':
            out[i] = (c as i32 + 32) as char
        else:
            out[i] = c
        i += 1
    return out

fn dump(lst &Lst):
    cur &Node = &lst.head
    while cur != null:
        output(cur.content, " ")
        cur = &cur.next
    output("\n")

fn main() i32:
    output("ft_isalpha('a')=", ft_isalpha('a'), " ('1')=", ft_isalpha('1'), "\n")
    output("ft_isdigit('7')=", ft_isdigit('7'), " ('x')=", ft_isdigit('x'), "\n")
    output("ft_isalnum('Z')=", ft_isalnum('Z'), " (' ')=", ft_isalnum(' '), "\n")
    output("ft_toupper('a')=", ft_toupper('a'), " ft_tolower('A')=", ft_tolower('A'), "\n")
    output("ft_strlen(\"hello\")=", ft_strlen("hello"), "\n")
    output("ft_strncmp(abc,abd,3)=", ft_strncmp("abc", "abd", 3), "\n")
    output("ft_strchr(hello,l)=", ft_strchr("hello", 'l'), "\n")
    output("ft_strrchr(hello,l)=", ft_strrchr("hello", 'l'), "\n")
    output("ft_strnstr(hello world,wor,11)=", ft_strnstr("hello world", "wor", 11), "\n")
    output("ft_atoi(\"  -42abc\")=", ft_atoi("  -42abc"), "\n")

    buf char[] = new char[8]
    ft_memset(buf, 'x', 4)
    buf[4] = 0 as char
    output("ft_memset -> ", buf, "\n")
    n i32 = ft_strlcpy(buf, "hi", 8)
    output("ft_strlcpy -> ", buf, " (ret ", n, ")\n")
    clean buf

    output("--- part 2 ---\n")
    a char[] = ft_substr("hello world", 6, 5)
    output("ft_substr -> ", a, "\n")
    clean a

    j char[] = ft_strjoin("foo", "bar")
    output("ft_strjoin -> ", j, "\n")
    clean j

    t char[] = ft_strtrim("xxhelloxx", "x")
    output("ft_strtrim -> ", t, "\n")
    clean t

    parts char[][] = ft_split("a,bb,ccc", ',')
    output("ft_split -> ", parts.len as i32, " words: ")
    i i32 = 0
    while i < parts.len as i32:
        output(parts[i], " ")
        i += 1
    output("\n")

    p char[] = ft_itoa(-2147483648)
    output("ft_itoa(INT_MIN) -> ", p, "\n")
    clean p
    q char[] = ft_itoa(0)
    output("ft_itoa(0) -> ", q, "\n")
    clean q

    m char[] = ft_strmapi("hello", upcase_i)
    output("ft_strmapi -> ", m, "\n")
    clean m

    it char[] = ft_strdup("world")
    ft_striteri(it, upcase_i)
    output("ft_striteri -> ", it, "\n")
    clean it

    output("ft_putnbr_fd -> ")
    ft_putnbr_fd(-42, 1)
    output("\n")
    ft_putendl_fd("ft_putendl_fd works", 1)

    output("--- bonus: linked list ---\n")
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("a"))
    ft_lstadd_back(&lst, ft_lstnew("b"))
    ft_lstadd_back(&lst, ft_lstnew("c"))
    ft_lstadd_front(&lst, ft_lstnew("z"))
    output("list:   ")
    dump(&lst)
    output("size:   ", ft_lstsize(&lst), "\n")

    L &Node = ft_lstlast(&lst)
    output("last:   ", L.content, "\n")

    ft_lstiter(&lst, upcase)
    output("iter:   ")
    dump(&lst)

    mapped Lst = ft_lstmap(&lst, lower)
    output("map:    ")
    dump(&mapped)

    ft_lstdelone(&lst, 1)
    output("del 1:  ")
    dump(&lst)

    ft_lstdelone(&lst, 0)
    output("del 0:  ")
    dump(&lst)

    ft_lstclear(&lst)
    output("clear:  size ", ft_lstsize(&lst), "\n")
    return 0
```

---

## 027 — project: net server

```ura
// expect: 0
// net_server.ura — a TCP chat server (macOS), single-process event loop.
//
// No fork, no threads: one `poll()` loop owns every fd — stdin, the listener,
// and all connected clients — and handles whichever is ready. A line typed at
// the server is broadcast to every client; a line from a client is printed.
//
// The poll set is a `PollFd[]`: slot 0 is stdin, slot 1 is the listener, and
// slots 2.. are clients. Because the loop owns the client fds and closes them
// deliberately on disconnect, they are tracked as raw fds here (a Socket with
// `operator drop` would close a fd when a copy of it went out of scope).

AF_INET      i32 = 2
SOCK_STREAM  i32 = 1
SOL_SOCKET   i32 = 65535
SO_REUSEADDR i32 = 4
SO_REUSEPORT i32 = 512
PORT         i32 = 8080
POLLIN       i16 = 1
MAX          i32 = 64

// macOS sockaddr_in, 16 bytes: len(1) family(1) port(2,BE) ip(4) zero(8)
struct SockAddr:
    len    char
    family char
    port   i16
    ip     i32
    zero   i64

    pub fn create(port i32, ip i32) SockAddr:
        a SockAddr
        a.len    = 16 as char
        a.family = AF_INET as char
        a.port   = (((port & 255) << 8) | (port >> 8)) as i16
        a.ip     = ip
        a.zero   = 0
        return a

// C: struct pollfd { int fd; short events; short revents; }  (8 bytes)
struct PollFd:
    fd      i32
    events  i16
    revents i16

    fn watch(fd i32):
        self.fd      = fd
        self.events  = POLLIN
        self.revents = 0

    fn has_input() bool:
        return (self.revents & POLLIN) != 0

proto fn socket(domain i32, type i32, protocol i32) i32
proto fn bind(fd i32, addr &SockAddr, len i32) i32
proto fn listen(fd i32, backlog i32) i32
proto fn accept(fd i32, addr pointer, len pointer) i32
proto fn setsockopt(fd i32, level i32, opt i32, val pointer, len i32) i32
proto fn close(fd i32) i32
proto fn read(fd i32, buf pointer, n i64) i64
proto fn poll(fds &PollFd, nfds i32, timeout i32) i32

// Unbuffered terminal write, so prompts appear before we block on poll.
fn log(msg char[]):
    write(1, msg, strlen(msg))

struct Server:
    fd i32

    pub fn create() Server:
        s Server
        s.fd = socket(AF_INET, SOCK_STREAM, 0)
        return s

    fn valid() bool:
        return self.fd >= 0

    fn reuse():
        opt char[] = new char[4]
        opt[0] = 1 as char
        setsockopt(self.fd, SOL_SOCKET, SO_REUSEADDR, opt, 4)
        setsockopt(self.fd, SOL_SOCKET, SO_REUSEPORT, opt, 4)
        clean opt

    fn bind_listen(port i32) bool:
        addr SockAddr = SockAddr::create(port, 0)
        if bind(self.fd, &addr, 16) < 0:
            return False
        listen(self.fd, 5)
        return True

    // raw fd, not a Socket: the poll loop owns and closes it explicitly
    fn accept_raw() i32:
        none pointer = null
        return accept(self.fd, none, none)

    operator drop:
        if self.fd >= 0:
            close(self.fd)

fn main() i32:
    server Server = Server::create()
    if not server.valid():
        log("\033[0;31m[server] socket failed\033[0m\n")
        return 1

    server.reuse()
    if not server.bind_listen(PORT):
        log("\033[0;31m[server] bind failed\033[0m\n")
        return 1

    log("\033[0;32m[server]\033[0m listening on port 8080...\n")

    // poll set: [0] = stdin, [1] = listener, [2..nfds) = clients
    fds PollFd[] = new PollFd[MAX]
    fds[0].watch(0)
    fds[1].watch(server.fd)
    nfds i32 = 2

    buf char[] = new char[1024]
    r i32 = 0

    log("\033[0;32m[server]\033[0m > ")
    loop:
        poll(&fds[0], nfds, -1)

        // a line typed here -> broadcast to every client
        if fds[0].has_input():
            r = read(0, buf, 1023) as i32
            if r <= 0:
                break
            buf[r - 1] = 0 as char
            i i32 = 2
            while i < nfds:
                write(fds[i].fd, buf, strlen(buf))
                i += 1
            log("\033[0;32m[server]\033[0m > ")
        elif fds[0].revents != 0:
            // stdin is not a live terminal (redirected / closed): a negative
            // fd tells poll to skip it, so we keep serving clients instead of
            // spinning on POLLNVAL.
            fds[0].fd = -1

        // a new connection -> add it to the poll set
        if fds[1].has_input():
            cfd i32 = server.accept_raw()
            if cfd >= 0 and nfds < MAX:
                fds[nfds].watch(cfd)
                nfds += 1
                log("\r\033[2K\033[0;32m[server]\033[0m client connected\n")
                log("\033[0;32m[server]\033[0m > ")

        // client traffic -> print it; compact the set on disconnect
        j i32 = 2
        while j < nfds:
            if not fds[j].has_input():
                j += 1
                continue
            r = read(fds[j].fd, buf, 1023) as i32
            if r <= 0:
                close(fds[j].fd)
                nfds -= 1
                fds[j].fd      = fds[nfds].fd
                fds[j].events  = fds[nfds].events
                fds[j].revents = fds[nfds].revents
                log("\r\033[2K\033[0;31m[server]\033[0m client disconnected\n")
                log("\033[0;32m[server]\033[0m > ")
                continue
            buf[r] = 0 as char
            log("\r\033[2K\033[0;36m[client]\033[0m ")
            log(buf)
            log("\n\033[0;32m[server]\033[0m > ")
            j += 1

    clean fds
    clean buf
    return 0
```

---

## 028 — project: net client

```ura
// expect: 0
// net_client.ura — a TCP chat client (macOS), single-process event loop.
//
// No fork: one `poll()` loop watches both stdin and the socket, and handles
// whichever becomes ready. Structs used:
//   - SockAddr : the C sockaddr, passed to connect() by `ref`
//   - PollFd   : one entry of the poll set; poll() gets `ref fds[0]`
//   - Socket   : the connection, with `operator drop` (safe: one socket)
//
// write / strlen come from the auto-loaded common.ura; the socket calls are
// declared here so their address arguments can take a struct by `ref`.

AF_INET     i32 = 2
SOCK_STREAM i32 = 1
PORT        i32 = 8080
POLLIN      i16 = 1

// macOS sockaddr_in, 16 bytes: len(1) family(1) port(2,BE) ip(4) zero(8)
struct SockAddr:
    len    char
    family char
    port   i16
    ip     i32
    zero   i64

    pub fn create(port i32, ip i32) SockAddr:
        a SockAddr
        a.len    = 16 as char
        a.family = AF_INET as char
        a.port   = (((port & 255) << 8) | (port >> 8)) as i16
        a.ip     = ip
        a.zero   = 0
        return a

// C: struct pollfd { int fd; short events; short revents; }  (8 bytes)
struct PollFd:
    fd      i32
    events  i16
    revents i16

    fn watch(fd i32):
        self.fd      = fd
        self.events  = POLLIN
        self.revents = 0

    fn has_input() bool:
        return (self.revents & POLLIN) != 0

proto fn socket(domain i32, type i32, protocol i32) i32
proto fn connect(fd i32, addr &SockAddr, len i32) i32
proto fn close(fd i32) i32
proto fn read(fd i32, buf pointer, n i64) i64
proto fn poll(fds &PollFd, nfds i32, timeout i32) i32

// 127.0.0.1 -> the four octets in network order, packed into one i32.
fn ipv4(a i32, b i32, c i32, d i32) i32:
    return a | (b << 8) | (c << 16) | (d << 24)

// Unbuffered terminal write, so prompts appear before we block on poll.
fn log(msg char[]):
    write(1, msg, strlen(msg))

struct Socket:
    fd i32

    fn valid() bool:
        return self.fd >= 0

    fn send_text(msg char[]):
        write(self.fd, msg, strlen(msg))

    fn recv_into(buf char[], cap i32) i32:
        return read(self.fd, buf, cap as i64) as i32

    operator drop:
        if self.fd >= 0:
            close(self.fd)

fn main() i32:
    sock Socket
    sock.fd = socket(AF_INET, SOCK_STREAM, 0)
    if not sock.valid():
        log("\033[0;31m[client] socket failed\033[0m\n")
        return 1

    addr SockAddr = SockAddr::create(PORT, ipv4(127, 0, 0, 1))
    if connect(sock.fd, &addr, 16) < 0:
        log("\033[0;31m[client] connect failed\033[0m\n")
        return 1

    log("\033[0;36m[client]\033[0m connected to server on port 8080\n")

    // poll set: [0] = stdin, [1] = the socket
    fds PollFd[] = new PollFd[2]
    fds[0].watch(0)
    fds[1].watch(sock.fd)

    buf  char[] = new char[1024]
    rbuf char[] = new char[1024]
    r i32 = 0

    log("\033[0;36m[client]\033[0m > ")
    loop:
        poll(&fds[0], 2, -1)

        // a line typed at the terminal -> send it (drop the newline)
        if fds[0].has_input():
            r = read(0, buf, 1023) as i32
            if r <= 0:
                break
            buf[r - 1] = 0 as char
            sock.send_text(buf)
            log("\033[0;36m[client]\033[0m > ")
        elif fds[0].revents != 0:
            // stdin redirected / closed: skip it (negative fd) and keep
            // receiving from the server instead of spinning on POLLNVAL.
            fds[0].fd = -1

        // bytes from the server -> print them, then reprint the prompt
        if fds[1].has_input():
            r = sock.recv_into(rbuf, 1023)
            if r <= 0:
                log("\r\033[2K\033[0;31m[client]\033[0m server disconnected\n")
                break
            rbuf[r] = 0 as char
            log("\r\033[2K\033[0;33m[server]\033[0m ")
            log(rbuf)
            log("\n\033[0;36m[client]\033[0m > ")

    clean fds
    clean buf
    clean rbuf
    return 0
```

---

## 029 — dungeon: floor04

```ura
// stdout: Orc takes 17 damage — dead: True
// stdout: 4 squared: 16
// expect: 0
// dungeon/floor04.ura
fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo: return lo
    if val > hi: return hi
    return val

fn damage(atk i32, def i32) i32:
    return clamp(atk - def, 0, 999)

fn is_dead(hp i32) bool:
    return hp <= 0

fn square(n i32) i32:      return n * n
fn is_digit(c char) bool:  return c >= '0' and c <= '9'

fn main() i32:
    d i32 = damage(25, 8)
    output("Orc takes ", d, " damage — dead: ", is_dead(d - 60), "\n")
    output("4 squared: ", square(4), "\n")
    return 0
```

---

## 030 — dungeon: floor09b

```ura
// stdout: 100
// expect: 0
// dungeon/floor09b.ura
fn main() i32:
    hp  i32     = 80
    r &i32 = &hp
    r   = 100
    output(hp, "\n")
    return 0
```

---

## 031 — dungeon: floor14a

```ura
// error: Conflicting declaration of 'write': parameter 3 is i32, expected i64
// dungeon/floor14a.ura
proto fn strlen(s pointer) i64
proto fn write(fd i32, ptr pointer, len i32) i32
proto fn printf(fmt pointer, ...) i32

fn main() i32:
    n i32 = strlen("dungeon")
    write(1, "hi\n", 3)
    printf("length: %d\n", n)
    return 0
```

---

## 032 — dungeon: boss

```ura
// stdout: == Dungeon (macOS build) ==
// stdout: Party:
// stdout:   Aldric atk=18
// stdout:   Borin atk=25
// stdout:   Cass atk=22
// stdout: Champion: Borin (alive=True)
// stdout: Foe hp=20 loot=none
// stdout: Foe hp=45 loot=none
// stdout: Foe hp=120 loot=Dragonfang
// stdout: Trap! Aldric hp=90
// stdout: sizeof=24 typeof=Hero
// expect: 0
enum EnemyType: GOBLIN, ORC, DRAGON

struct Stats:
    hp  i32
    atk i32

struct Hero:
    name  char[]
    stats Stats

    pub fn create(name char[], hp i32, atk i32) Hero:
        h Hero
        h.name       = name
        h.stats.hp   = hp
        h.stats.atk  = atk
        return h

    fn alive() bool:
        return self.stats.hp > 0

    operator output() char[]:
        return self.name

fn enemy_hp(kind EnemyType) i32:
    match kind:
        case GOBLIN: return 20
        case ORC:    return 45
        case DRAGON: return 120
    return 0

fn loot_for(kind EnemyType) char[]:
    match kind:
        case DRAGON: return "Dragonfang"
        default:     return null
    return null

fn strongest(party Hero[], n i32) &Hero:
    best i32 = 0
    for i in 1..n:
        if party[i].stats.atk > party[best].stats.atk:
            best = i
    return &party[best]

fn main() i32:
    @if macos:
        output("== Dungeon (macOS build) ==\n")
    @elif linux:
        output("== Dungeon (Linux build) ==\n")
    @else:
        output("== Dungeon ==\n")

    party Hero[] = new Hero[3]
    party[0] = Hero::create("Aldric", 100, 18)
    party[1] = Hero::create("Borin",  90, 25)
    party[2] = Hero::create("Cass",   80, 22)

    output("Party:\n")
    for h in party:
        output("  ", h, " atk=", h.stats.atk, "\n")

    champ &Hero = strongest(party, 3)
    output("Champion: ", champ, " (alive=", champ.alive(), ")\n")

    foes EnemyType[] = [GOBLIN, ORC, DRAGON]
    for kind in foes:
        loot char[] = loot_for(kind)
        output("Foe hp=", enemy_hp(kind), " loot=",
               loot ?? "none", "\n")

    for ref h in party:
        h.stats.hp = h.stats.hp - 10
    output("Trap! ", party[0], " hp=", party[0].stats.hp, "\n")

    output("sizeof=", sizeof(party[0]), " typeof=",
           typeof(champ), "\n")

    clean party
    return 0
```

---

## 033 — project: calculator — tokenize, parse, print the AST and evaluate with try/catch

```ura
// stdout: ========================================
// stdout: calculate expr:  2 * (3 + 4) - 10 / 5
// stdout: ast:
// stdout: -
// stdout: ├─ *
// stdout: │  ├─ 2
// stdout: │  └─ +
// stdout: │     ├─ 3
// stdout: │     └─ 4
// stdout: └─ /
// stdout:    ├─ 10
// stdout:    └─ 5
// stdout: result: 12
// stdout: ========================================
// stdout: calculate expr:  8 / (3 - 3)
// stdout: ast:
// stdout: /
// stdout: ├─ 8
// stdout: └─ -
// stdout:    ├─ 3
// stdout:    └─ 3
// stdout: ========================================
// stderr: [0;31m
// expect: 1
// projects/calculator.ura
use "@/header"
use "@/error"

enum Type: NONE, NUMBER, ADD, SUB, MUL, DIV, LPAREN, RPAREN

struct Token:
    value i32
    type Type

    pub fn number(v i32) &Token:
        t &Token = new Token
        t.type = NUMBER
        t.value = v
        ret &t

    pub fn op(k Type) &Token:
        t &Token = new Token
        t.type = k
        ret &t

struct Node:
    token &Token
    left &Node
    right &Node

    pub fn create(token &Token) &Node:
        n &Node = new Node
        n.token = &token
        ret &n

    @no-warn
    operator drop:
        clean self.token
        clean self.left
        clean self.right

mod Lexer:
    fn run(src &String, toks Token[]) i32:
        fn is_digit(c char) bool: ret c >= '0' and c <= '9'
        n i32 = 0
        i i32 = 0
        while i < src.len():
            c char = src.at(i)
            if c == ' ':
                i = i + 1
                continue
            elif is_digit(c):
                v i32 = 0
                while i < src.len() and is_digit(src.at(i)):
                    v = v * 10 + ((src.at(i) as i32) - 48)
                    i = i + 1
                toks[n].type = NUMBER
                toks[n].value = v
                n = n + 1
                continue
            else:
                k Type = NUMBER
                match c:
                    case '+': k = ADD
                    case '-': k = SUB
                    case '*': k = MUL
                    case '/': k = DIV
                    case '(': k = LPAREN
                    case ')': k = RPAREN
                toks[n].type = k
                toks[n].value = 0
                n = n + 1
                i = i + 1
        ret n

mod Parser:
    fn factor(toks Token[], p &i32, n i32) &Node:
        if p < n and toks[p].type == LPAREN:
            p = p + 1
            inner &Node = expr(toks, &p, n)
            if p < n and toks[p].type == RPAREN:
                p = p + 1
            ret &inner
        lf &Node = Node::create(Token::number(toks[p].value))
        p = p + 1
        ret &lf

    fn term(toks Token[], p &i32, n i32) &Node:
        l &Node = factor(toks, &p, n)
        while p < n and (toks[p].type == MUL or toks[p].type == DIV):
            k Type = toks[p].type
            p = p + 1
            r &Node = factor(toks, &p, n)
            parent &Node = Node::create(Token::op(k))
            parent.left = &l
            parent.right = &r
            l = &parent
        ret &l

    fn expr(toks Token[], p &i32, n i32) &Node:
        l &Node = term(toks, &p, n)
        while p < n and (toks[p].type == ADD or toks[p].type == SUB):
            k Type = toks[p].type
            p = p + 1
            r &Node = term(toks, &p, n)
            parent &Node = Node::create(Token::op(k))
            parent.left = &l
            parent.right = &r
            l = &parent
        ret &l

fn sign(k Type) char[]:
    match k:
        case ADD: ret "+"
        case SUB: ret "-"
        case MUL: ret "*"
        case DIV: ret "/"
        default: ret "?"

fn label(n &Node):
    if n.token.type == NUMBER: output(n.token.value, "\n")
    else:                      output(sign(n.token.type), "\n")

fn show(n &Node, pre char[], last bool):
    output(pre)
    if last: output("└─ ")
    else:    output("├─ ")
    label(&n)
    kid String = String::from(pre)
    if last: kid.join("   ")
    else:    kid.join("│  ")
    if n.left != null:  show(&n.left, kid.c_str(), n.right == null)
    if n.right != null: show(&n.right, kid.c_str(), True)

fn tree(n &Node):
    label(&n)
    if n.left != null:  show(&n.left, "", n.right == null)
    if n.right != null: show(&n.right, "", True)

fn eval(n &Node) i32:
    match n.token.type:
        case NUMBER: ret n.token.value
        case ADD:    ret eval(&n.left) + eval(&n.right)
        case SUB:    ret eval(&n.left) - eval(&n.right)
        case MUL:    ret eval(&n.left) * eval(&n.right)
        case DIV:
            d i32 = eval(&n.right)
            if d == 0: throw Error::make("division by zero")
            ret eval(&n.left) / d
        default:    ret 0


fn calc(text char[]):
    output("========================================\n")
    output("calculate expr:  ", text, "\nast:\n")
    src String = String::from(text)
    toks Token[] = new Token[64]
    n i32 = Lexer::run(&src, toks)
    p i32 = 0
    root &Node = Parser::expr(toks, &p, n)
    tree(&root)
    try:
        output("result: ", eval(&root), "\n")
    catch e:
        errput("\nError: ", e.message, "\n")
    clean root
    clean toks


fn main() i32:
    calc("2 * (3 + 4) - 10 / 5")
    calc("8 / (3 - 3)")
    output("========================================\n")
    return 0
```

---

## 034 — libft: isalnum

```ura
// stdout: is alnum
// expect: 0
// libft/isalnum.ura
fn isalpha(c char) bool: return c >= 'a' and c <= 'z'
fn isdigit(c char) bool: return c >= '1' and c <= '9'
fn isalnum(c char) bool: return isalpha(c) or isdigit(c)

fn main() i32:
    if isalnum('1'):
        output("is alnum\n")
    else:
        output("is not alnum\n")
    return 0
```

---

## 035 — libft: isalpha

```ura
// stdout: is not alpha
// expect: 0
// libft/isalpha.ura
fn isalpha(c char) bool: return c >= 'a' and c <= 'z'

fn main() i32:
    if isalpha('1'):
        output("is alpha\n")
    else:
        output("is not alpha\n")
    return 0
```

---

## 036 — libft: isdigit

```ura
// stdout: is alnum
// expect: 0
// libft/isdigit.ura
fn isdigit(c char) bool: return c >= '1' and c <= '9'

fn main() i32:
    if isdigit('1'):
        output("is alnum\n")
    else:
        output("is not alnum\n")
    return 0
```

---

## 037 — libft: lstnew — new nodes, add_back, add_front and size

```ura
// stdout: z a b
// stdout: size=3
// expect: 0
struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_front(lst &Lst, node &Node):
    node.next = &lst.head
    lst.head  = &node

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn ft_lstsize(lst &Lst) i32:
    cur &Node = &lst.head
    count i32 = 0
    while cur != null:
        count += 1
        cur = &cur.next
    return count

fn main() i32:
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("a"))
    ft_lstadd_back(&lst, ft_lstnew("b"))
    ft_lstadd_front(&lst, ft_lstnew("z"))
    cur &Node = &lst.head
    while cur != null:
        output(cur.content, " ")
        cur = &cur.next
    output("\n")
    output("size=", ft_lstsize(&lst), "\n")
    return 0
```

---

## 038 — libft: lstlast — returns a reference to the last node

```ura
// stdout: c
// expect: 0
struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn ft_lstlast(lst &Lst) &Node:
    cur &Node = &lst.head
    if cur == null:
        return null
    while cur.next != null:
        cur = &cur.next
    return &cur

fn main() i32:
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("a"))
    ft_lstadd_back(&lst, ft_lstnew("b"))
    ft_lstadd_back(&lst, ft_lstnew("c"))
    L &Node = ft_lstlast(&lst)
    output(L.content, "\n")
    return 0
```

---

## 039 — libft: lstiter — apply a function to each node's content in place

```ura
// stdout: AB CD
// expect: 0
struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn upcase(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        c char = s[i]
        if c >= 'a' and c <= 'z':
            out[i] = (c as i32 - 32) as char
        else:
            out[i] = c
        i += 1
    return out

fn ft_lstiter(lst &Lst, f fn(char[]) char[]) void:
    cur &Node = &lst.head
    while cur != null:
        cur.content = f(cur.content)
        cur = &cur.next

fn main() i32:
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("ab"))
    ft_lstadd_back(&lst, ft_lstnew("cd"))
    ft_lstiter(&lst, upcase)
    cur &Node = &lst.head
    while cur != null:
        output(cur.content, " ")
        cur = &cur.next
    output("\n")
    return 0
```

---

## 040 — libft: lstdelone — unlink a node by index and clear the list

```ura
// stdout: a c
// stdout: c
// stdout: size=0
// expect: 0
struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn ft_lstsize(lst &Lst) i32:
    cur &Node = &lst.head
    count i32 = 0
    while cur != null:
        count += 1
        cur = &cur.next
    return count

fn ft_lstdelone(lst &Lst, idx i32):
    if lst.head == null:
        return
    if idx <= 0:
        lst.head = &lst.head.next
        return
    prev &Node = &lst.head
    i i32 = 1
    while i < idx and prev.next != null:
        prev = &prev.next
        i += 1
    if prev.next != null:
        prev.next = &prev.next.next

fn ft_lstclear(lst &Lst):
    lst.head = null

fn show(lst &Lst):
    cur &Node = &lst.head
    while cur != null:
        output(cur.content, " ")
        cur = &cur.next
    output("\n")

fn main() i32:
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("a"))
    ft_lstadd_back(&lst, ft_lstnew("b"))
    ft_lstadd_back(&lst, ft_lstnew("c"))
    ft_lstdelone(&lst, 1)
    show(&lst)
    ft_lstdelone(&lst, 0)
    show(&lst)
    ft_lstclear(&lst)
    output("size=", ft_lstsize(&lst), "\n")
    return 0
```

