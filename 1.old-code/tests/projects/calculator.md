# projects / calculator

## index

- calculator — tokenize, parse, print the AST and evaluate with try/catch

## calculator — tokenize, parse, print the AST and evaluate with try/catch

```ura
// projects/calculator.ura
use "@/header"
use "@/error"

enum Type: NONE, NUMBER, ADD, SUB, MUL, DIV, LPAREN, RPAREN

struct Token:
    value i32
    type Type

    pub fn number(v i32) ref? Token:
        ref? t Token = new Token
        t.type = NUMBER
        t.value = v
        ret ref t

    pub fn op(k Type) ref? Token:
        ref? t Token = new Token
        t.type = k
        ret ref t

struct Node:
    ref? token Token
    ref? left Node
    ref? right Node

    pub fn create(ref? token Token) ref? Node:
        ref? n Node = new Node
        n.token = ref token
        ret ref n

    @no-warn
    operator drop:
        clean self.token
        clean self.left
        clean self.right

mod Lexer:
    fn run(ref src String, toks Token[]) i32:
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
    fn factor(toks Token[], ref p i32, n i32) ref? Node:
        if p < n and toks[p].type == LPAREN:
            p = p + 1
            ref? inner Node = expr(toks, ref p, n)
            if p < n and toks[p].type == RPAREN:
                p = p + 1
            ret ref inner
        ref? lf Node = Node::create(Token::number(toks[p].value))
        p = p + 1
        ret ref lf

    fn term(toks Token[], ref p i32, n i32) ref? Node:
        ref? l Node = factor(toks, ref p, n)
        while p < n and (toks[p].type == MUL or toks[p].type == DIV):
            k Type = toks[p].type
            p = p + 1
            ref? r Node = factor(toks, ref p, n)
            ref? parent Node = Node::create(Token::op(k))
            parent.left = ref l
            parent.right = ref r
            l = ref parent
        ret ref l

    fn expr(toks Token[], ref p i32, n i32) ref? Node:
        ref? l Node = term(toks, ref p, n)
        while p < n and (toks[p].type == ADD or toks[p].type == SUB):
            k Type = toks[p].type
            p = p + 1
            ref? r Node = term(toks, ref p, n)
            ref? parent Node = Node::create(Token::op(k))
            parent.left = ref l
            parent.right = ref r
            l = ref parent
        ret ref l

fn sign(k Type) char[]:
    match k:
        case ADD: ret "+"
        case SUB: ret "-"
        case MUL: ret "*"
        case DIV: ret "/"
        default: ret "?"

fn label(ref? n Node) void:
    if n.token.type == NUMBER: output(n.token.value, "\n")
    else:                      output(sign(n.token.type), "\n")

fn show(ref? n Node, pre char[], last bool) void:
    output(pre)
    if last: output("└─ ")
    else:    output("├─ ")
    label(ref n)
    kid String = String::from(pre)
    if last: kid.join("   ")
    else:    kid.join("│  ")
    if n.left != null:  show(ref n.left, kid.c_str(), n.right == null)
    if n.right != null: show(ref n.right, kid.c_str(), True)

fn tree(ref? n Node) void:
    label(ref n)
    if n.left != null:  show(ref n.left, "", n.right == null)
    if n.right != null: show(ref n.right, "", True)

fn eval(ref? n Node) i32:
    match n?.token?.type:
        case NUMBER: ret n.token.value
        case ADD:    ret eval(ref n.left) + eval(ref n.right)
        case SUB:    ret eval(ref n.left) - eval(ref n.right)
        case MUL:    ret eval(ref n.left) * eval(ref n.right)
        case DIV:
            d i32 = eval(ref n.right)
            if d == 0: throw Error::make("division by zero")
            ret eval(ref n.left) / d
        default:    ret 0


fn calc(text char[]) void:
    output("========================================\n")
    output("calculate expr:  ", text, "\nast:\n")
    src String = String::from(text)
    toks Token[] = new Token[64]
    n i32 = Lexer::run(ref src, toks)
    p i32 = 0
    ref? root Node = Parser::expr(toks, ref p, n)
    tree(ref root)
    try:
        output("result: ", eval(ref root), "\n")
    catch e:
        errput("\nError: ", e.message, "\n")
    clean root
    clean toks


main():
    calc("2 * (3 + 4) - 10 / 5")
    calc("8 / (3 - 3)")
    output("========================================\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

proto fn isalnum(c : i32) : i32

proto fn isalpha(c : i32) : i32

proto fn isascii(c : i32) : i32

proto fn isblank(c : i32) : i32

proto fn iscntrl(c : i32) : i32

proto fn isdigit(c : i32) : i32

proto fn isgraph(c : i32) : i32

proto fn islower(c : i32) : i32

proto fn isprint(c : i32) : i32

proto fn ispunct(c : i32) : i32

proto fn isspace(c : i32) : i32

proto fn isupper(c : i32) : i32

proto fn isxdigit(c : i32) : i32

proto fn tolower(c : i32) : i32

proto fn toupper(c : i32) : i32

proto fn strerror(errnum : i32) : pointer

proto fn strerror_r(errnum : i32, buf : pointer, buflen : i64) : i32

proto fn perror(s : pointer) : void

= : i32
├─ EPERM : i32
└─ int 1

= : i32
├─ ENOENT : i32
└─ int 2

= : i32
├─ ESRCH : i32
└─ int 3

= : i32
├─ EINTR : i32
└─ int 4

= : i32
├─ EIO : i32
└─ int 5

= : i32
├─ ENXIO : i32
└─ int 6

= : i32
├─ E2BIG : i32
└─ int 7

= : i32
├─ ENOEXEC : i32
└─ int 8

= : i32
├─ EBADF : i32
└─ int 9

= : i32
├─ ECHILD : i32
└─ int 10

= : i32
├─ EACCES : i32
└─ int 13

= : i32
├─ EFAULT : i32
└─ int 14

= : i32
├─ ENOTBLK : i32
└─ int 15

= : i32
├─ EBUSY : i32
└─ int 16

= : i32
├─ EEXIST : i32
└─ int 17

= : i32
├─ EXDEV : i32
└─ int 18

= : i32
├─ ENODEV : i32
└─ int 19

= : i32
├─ ENOTDIR : i32
└─ int 20

= : i32
├─ EISDIR : i32
└─ int 21

= : i32
├─ EINVAL : i32
└─ int 22

= : i32
├─ ENFILE : i32
└─ int 23

= : i32
├─ EMFILE : i32
└─ int 24

= : i32
├─ ENOTTY : i32
└─ int 25

= : i32
├─ ETXTBSY : i32
└─ int 26

= : i32
├─ EFBIG : i32
└─ int 27

= : i32
├─ ENOSPC : i32
└─ int 28

= : i32
├─ ESPIPE : i32
└─ int 29

= : i32
├─ EROFS : i32
└─ int 30

= : i32
├─ EMLINK : i32
└─ int 31

= : i32
├─ EPIPE : i32
└─ int 32

= : i32
├─ EDOM : i32
└─ int 33

= : i32
├─ ERANGE : i32
└─ int 34

proto fn open(path : pointer, flags : i32, mode : i32) : i32

proto fn openat(dirfd : i32, path : pointer, flags : i32, mode : i32) : i32

proto fn creat(path : pointer, mode : i32) : i32

proto fn fcntl(fd : i32, cmd : i32, arg : i32) : i32

= : i32
├─ O_RDONLY : i32
└─ int 0

= : i32
├─ O_WRONLY : i32
└─ int 1

= : i32
├─ O_RDWR : i32
└─ int 2

= : i32
├─ FD_CLOEXEC : i32
└─ int 1

proto fn fopen(path : pointer, mode : pointer) : pointer

proto fn freopen(path : pointer, mode : pointer, file : pointer) : pointer

proto fn fclose(file : pointer) : i32

proto fn fflush(file : pointer) : i32

proto fn fgetc(file : pointer) : i32

proto fn fputc(c : i32, file : pointer) : i32

proto fn getc(file : pointer) : i32

proto fn putc(c : i32, file : pointer) : i32

proto fn getchar() : i32

proto fn putchar(c : char) : i32

proto fn ungetc(c : i32, file : pointer) : i32

proto fn fgets(buffer : pointer, size : i32, file : pointer) : pointer

proto fn fputs(str : pointer, file : pointer) : i32

proto fn puts(str : pointer) : i32

proto fn gets(buffer : pointer) : pointer

proto fn fread(ptr : pointer, size : i32, nmemb : i32, file : pointer) : i32

proto fn fwrite(ptr : pointer, size : i32, nmemb : i32, file : pointer) : i32

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn read(fd : i32, ptr : pointer, len : i64) : i64

proto fn fseek(file : pointer, offset : i64, whence : i32) : i32

proto fn ftell(file : pointer) : i64

proto fn rewind(file : pointer) : void

proto fn fgetpos(file : pointer, pos : pointer) : i32

proto fn fsetpos(file : pointer, pos : pointer) : i32

proto fn feof(file : pointer) : i32

proto fn ferror(file : pointer) : i32

proto fn clearerr(file : pointer) : void

proto fn perror(str : pointer) : void

proto fn setvbuf(file : pointer, buffer : pointer, mode : i32, size : i32) : i32

proto fn setbuf(file : pointer, buffer : pointer) : void

proto fn remove(path : pointer) : i32

proto fn rename(oldpath : pointer, newpath : pointer) : i32

proto fn tmpfile() : pointer

proto fn tmpnam(str : pointer) : pointer

proto fn fileno(file : pointer) : i32

proto fn fdopen(fd : i32, mode : pointer) : pointer

proto fn printf(format : pointer, ...) : i32

proto fn fprintf(file : pointer, format : pointer, ...) : i32

proto fn sprintf(buffer : pointer, format : pointer, ...) : i32

proto fn snprintf(buffer : pointer, size : i32, format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn vprintf(format : pointer, args : pointer) : i32

proto fn vfprintf(file : pointer, format : pointer, args : pointer) : i32

proto fn vsprintf(buffer : pointer, format : pointer, args : pointer) : i32

proto fn vsnprintf(buffer : pointer, size : i32, format : pointer, args : pointer) : i32

proto fn vdprintf(fd : i32, format : pointer, args : pointer) : i32

proto fn scanf(format : pointer, ...) : i32

proto fn fscanf(file : pointer, format : pointer, ...) : i32

proto fn sscanf(buffer : pointer, format : pointer, ...) : i32

proto fn vscanf(format : pointer, args : pointer) : i32

proto fn vfscanf(file : pointer, format : pointer, args : pointer) : i32

proto fn vsscanf(buffer : pointer, format : pointer, args : pointer) : i32

proto fn sin(x : f64) : f64

proto fn cos(x : f64) : f64

proto fn tan(x : f64) : f64

proto fn asin(x : f64) : f64

proto fn acos(x : f64) : f64

proto fn atan(x : f64) : f64

proto fn atan2(y : f64, x : f64) : f64

proto fn sinh(x : f64) : f64

proto fn cosh(x : f64) : f64

proto fn tanh(x : f64) : f64

proto fn asinh(x : f64) : f64

proto fn acosh(x : f64) : f64

proto fn atanh(x : f64) : f64

proto fn exp(x : f64) : f64

proto fn exp2(x : f64) : f64

proto fn expm1(x : f64) : f64

proto fn log(x : f64) : f64

proto fn log2(x : f64) : f64

proto fn log10(x : f64) : f64

proto fn log1p(x : f64) : f64

proto fn pow(x : f64, y : f64) : f64

proto fn sqrt(x : f64) : f64

proto fn cbrt(x : f64) : f64

proto fn hypot(x : f64, y : f64) : f64

proto fn ceil(x : f64) : f64

proto fn floor(x : f64) : f64

proto fn trunc(x : f64) : f64

proto fn round(x : f64) : f64

proto fn fmod(x : f64, y : f64) : f64

proto fn remainder(x : f64, y : f64) : f64

proto fn fabs(x : f64) : f64

proto fn copysign(x : f64, y : f64) : f64

proto fn fmax(x : f64, y : f64) : f64

proto fn fmin(x : f64, y : f64) : f64

proto fn fdim(x : f64, y : f64) : f64

proto fn isnan(x : f64) : i32

proto fn isinf(x : f64) : i32

proto fn isfinite(x : f64) : i32

proto fn signbit(x : f64) : i32

proto fn tgamma(x : f64) : f64

proto fn lgamma(x : f64) : f64

proto fn erf(x : f64) : f64

proto fn erfc(x : f64) : f64

proto fn malloc(size : i32) : pointer

proto fn calloc(len : i64, size : i64) : pointer

proto fn realloc(ptr : pointer, newSize : i32) : pointer

proto fn free(ptr : pointer) : void

proto fn aligned_alloc(alignment : i32, size : i32) : pointer

struct Pollfd
├─ fd : i32
├─ events : i16
└─ revents : i16

proto fn socket(domain : i32, type : i32, protocol : i32) : i32

proto fn bind(fd : i32, addr : pointer, addrlen : i32) : i32

proto fn listen(fd : i32, backlog : i32) : i32

proto fn accept(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn connect(fd : i32, addr : pointer, addrlen : i32) : i32

proto fn shutdown(fd : i32, how : i32) : i32

proto fn send(fd : i32, buf : pointer, len : i64, flags : i32) : i64

proto fn recv(fd : i32, buf : pointer, len : i64, flags : i32) : i64

proto fn sendto(fd : i32, buf : pointer, len : i64, flags : i32, addr : pointer, addrlen : i32) : i64

proto fn recvfrom(fd : i32, buf : pointer, len : i64, flags : i32, addr : pointer, addrlen : pointer) : i64

proto fn sendmsg(fd : i32, msg : pointer, flags : i32) : i64

proto fn recvmsg(fd : i32, msg : pointer, flags : i32) : i64

proto fn setsockopt(fd : i32, level : i32, optname : i32, optval : pointer, optlen : i32) : i32

proto fn getsockopt(fd : i32, level : i32, optname : i32, optval : pointer, optlen : pointer) : i32

proto fn getsockname(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn getpeername(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn htons(hostshort : i32) : i32

proto fn htonl(hostlong : i32) : i32

proto fn ntohs(netshort : i32) : i32

proto fn ntohl(netlong : i32) : i32

proto fn inet_addr(cp : pointer) : i32

proto fn inet_ntoa(addr : u32) : pointer

proto fn inet_pton(af : i32, src : pointer, dst : pointer) : i32

proto fn inet_ntop(af : i32, src : pointer, dst : pointer, size : i32) : pointer

proto fn gethostbyname(name : pointer) : pointer

proto fn gethostbyaddr(addr : pointer, len : i32, type : i32) : pointer

proto fn getaddrinfo(node : pointer, service : pointer, hints : pointer, res : pointer) : i32

proto fn freeaddrinfo(res : pointer) : void

proto fn gai_strerror(errcode : i32) : pointer

proto fn getnameinfo(addr : pointer, addrlen : i32, host : pointer, hostlen : i32, serv : pointer, servlen : i32, flags : i32) : i32

proto fn select(nfds : i32, readfds : pointer, writefds : pointer, exceptfds : pointer, timeout : pointer) : i32

proto fn poll(fds : pointer, nfds : i32, timeout : i32) : i32

proto fn ioctl(fd : i32, request : i32, arg : pointer) : i32

= : i32
├─ IPPROTO_IP : i32
└─ int 0

= : i32
├─ IPPROTO_TCP : i32
└─ int 6

= : i32
├─ IPPROTO_UDP : i32
└─ int 17

= : i32
├─ IPPROTO_IPV6 : i32
└─ int 41

= : i32
├─ SHUT_RD : i32
└─ int 0

= : i32
├─ SHUT_WR : i32
└─ int 1

= : i32
├─ SHUT_RDWR : i32
└─ int 2

= : i32
├─ POLLIN : i32
└─ int 1

= : i32
├─ POLLPRI : i32
└─ int 2

= : i32
├─ POLLOUT : i32
└─ int 4

= : i32
├─ POLLERR : i32
└─ int 8

= : i32
├─ POLLHUP : i32
└─ int 16

= : i32
├─ POLLNVAL : i32
└─ int 32

= : i32
├─ INADDR_ANY : i32
└─ int 0

= : i32
├─ INADDR_LOOPBACK : i32
└─ int 2130706433

= : i32
├─ INADDR_BROADCAST : i32
└─ - : i32
   ├─ int 0
   └─ int 1

proto fn signal(sig : i32, handler : pointer) : pointer

proto fn raise(sig : i32) : i32

proto fn kill(pid : i32, sig : i32) : i32

proto fn sigaction(sig : i32, act : pointer, oldact : pointer) : i32

proto fn sigprocmask(how : i32, set : pointer, oldset : pointer) : i32

proto fn sigsuspend(mask : pointer) : i32

proto fn strsignal(sig : i32) : pointer

= : i32
├─ SIGHUP : i32
└─ int 1

= : i32
├─ SIGINT : i32
└─ int 2

= : i32
├─ SIGQUIT : i32
└─ int 3

= : i32
├─ SIGILL : i32
└─ int 4

= : i32
├─ SIGTRAP : i32
└─ int 5

= : i32
├─ SIGABRT : i32
└─ int 6

= : i32
├─ SIGFPE : i32
└─ int 8

= : i32
├─ SIGKILL : i32
└─ int 9

= : i32
├─ SIGSEGV : i32
└─ int 11

= : i32
├─ SIGPIPE : i32
└─ int 13

= : i32
├─ SIGALRM : i32
└─ int 14

= : i32
├─ SIGTERM : i32
└─ int 15

proto fn stat(path : pointer, buf : pointer) : i32

proto fn fstat(fd : i32, buf : pointer) : i32

proto fn lstat(path : pointer, buf : pointer) : i32

proto fn mkdir(path : pointer, mode : i32) : i32

proto fn chmod(path : pointer, mode : i32) : i32

proto fn fchmod(fd : i32, mode : i32) : i32

proto fn umask(cmask : i32) : i32

= : i32
├─ S_IFMT : i32
└─ int 61440

= : i32
├─ S_IFSOCK : i32
└─ int 49152

= : i32
├─ S_IFLNK : i32
└─ int 40960

= : i32
├─ S_IFREG : i32
└─ int 32768

= : i32
├─ S_IFBLK : i32
└─ int 24576

= : i32
├─ S_IFDIR : i32
└─ int 16384

= : i32
├─ S_IFCHR : i32
└─ int 8192

= : i32
├─ S_IFIFO : i32
└─ int 4096

= : i32
├─ S_ISUID : i32
└─ int 2048

= : i32
├─ S_ISGID : i32
└─ int 1024

= : i32
├─ S_ISVTX : i32
└─ int 512

proto fn abort() : void

proto fn exit(code : i32) : void

proto fn quick_exit(code : i32) : void

proto fn _Exit(code : i32) : void

proto fn getenv(name : pointer) : pointer

proto fn setenv(name : pointer, value : pointer, overwrite : i32) : i32

proto fn unsetenv(name : pointer) : i32

proto fn putenv(string : pointer) : i32

proto fn system(command : pointer) : i32

proto fn rand() : i32

proto fn srand(seed : i32) : void

proto fn bsearch(key : pointer, base : pointer, nmemb : i32, size : i32, compar : pointer) : pointer

proto fn qsort(base : pointer, nmemb : i32, size : i32, compar : pointer) : void

proto fn abs(n : i32) : i32

proto fn labs(n : i64) : i64

proto fn div(numer : i32, denom : i32) : pointer

proto fn ldiv(numer : i64, denom : i64) : pointer

proto fn atoi(str : pointer) : i32

proto fn atol(str : pointer) : i64

proto fn atof(str : pointer) : f64

proto fn strtol(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtoul(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtof(str : pointer, endptr : pointer) : f32

proto fn strtod(str : pointer, endptr : pointer) : f64

proto fn strtoll(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtoull(str : pointer, endptr : pointer, base : i32) : i64

proto fn mblen(s : pointer, n : i32) : i32

proto fn mbtowc(pwc : pointer, s : pointer, n : i32) : i32

proto fn wctomb(s : pointer, wchar : i32) : i32

proto fn mbstowcs(dest : pointer, src : pointer, n : i32) : i32

proto fn wcstombs(dest : pointer, src : pointer, n : i32) : i32

proto fn strlen(s : pointer) : i64

proto fn strnlen(s : pointer, maxlen : i32) : i32

proto fn strcmp(a : pointer, b : pointer) : i32

proto fn strncmp(a : pointer, b : pointer, n : i32) : i32

proto fn strcasecmp(a : pointer, b : pointer) : i32

proto fn strncasecmp(a : pointer, b : pointer, n : i32) : i32

proto fn strcoll(a : pointer, b : pointer) : i32

proto fn strxfrm(dest : pointer, src : pointer, n : i32) : i32

proto fn strcpy(dest : pointer, src : pointer) : pointer

proto fn strncpy(dest : pointer, src : pointer, n : i32) : pointer

proto fn strdup(s : pointer) : pointer

proto fn strndup(s : pointer, n : i32) : pointer

proto fn strcat(dest : pointer, src : pointer) : pointer

proto fn strncat(dest : pointer, src : pointer, n : i32) : pointer

proto fn strchr(s : pointer, c : i32) : pointer

proto fn strrchr(s : pointer, c : i32) : pointer

proto fn strstr(haystack : pointer, needle : pointer) : pointer

proto fn strcasestr(haystack : pointer, needle : pointer) : pointer

proto fn strspn(s : pointer, accept : pointer) : i32

proto fn strcspn(s : pointer, reject : pointer) : i32

proto fn strpbrk(s : pointer, accept : pointer) : pointer

proto fn strtok(s : pointer, delim : pointer) : pointer

proto fn strtok_r(s : pointer, delim : pointer, saveptr : pointer) : pointer

proto fn strerror(errnum : i32) : pointer

proto fn strerror_r(errnum : i32, buf : pointer, buflen : i64) : i32

proto fn memchr(ptr : pointer, c : i32, n : i32) : pointer

proto fn memrchr(ptr : pointer, c : i32, n : i32) : pointer

proto fn memcmp(a : pointer, b : pointer, n : i32) : i32

proto fn memcpy(dest : pointer, src : pointer, n : i32) : pointer

proto fn memmove(dest : pointer, src : pointer, n : i32) : pointer

proto fn memset(ptr : pointer, value : i32, n : i32) : pointer

proto fn memccpy(dest : pointer, src : pointer, c : i32, n : i32) : pointer

proto fn bzero(s : pointer, size : i64) : void

proto fn strlcpy(dest : pointer, src : pointer, size : i32) : i32

proto fn strlcat(dest : pointer, src : pointer, size : i32) : i32

proto fn strsignal(sig : i32) : pointer

proto fn memset_s(ptr : pointer, size : i32, value : i32, n : i32) : pointer

proto fn atoi(s : pointer) : i32

struct String
├─ value : char[]
├─ count : i32
├─ fn String.create() : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .value : char[]
│  │  │  └─ s : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 16
│  ├─ = : i32
│  │  ├─ .count : i32
│  │  │  └─ s : STRUCT_CALL
│  │  └─ int 0
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.from(str : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ s : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ call assign : void
│  │  ├─ s : STRUCT_CALL
│  │  └─ str : char[]
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.from_int(n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ s : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ s : STRUCT_CALL
│  │  │  └─ char '0'
│  │  └─ return
│  │     └─ s : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ num : i32
│  │  └─ n : i32
│  ├─ if
│  │  ├─ condition < : bool
│  │  │  ├─ num : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ s : STRUCT_CALL
│  │  │  └─ char '-'
│  │  └─ = : i32
│  │     ├─ num : i32
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ num : i32
│  ├─ = : array
│  │  ├─ digits : char[]
│  │  └─ array : char[]
│  │     └─ int 12
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ num : i32
│  │  │  └─ int 0
│  │  ├─ = : char
│  │  │  ├─ index : char
│  │  │  │  ├─ digits : char[]
│  │  │  │  └─ i : i32
│  │  │  └─ cast : char
│  │  │     └─ + : i32
│  │  │        ├─ % : i32
│  │  │        │  ├─ num : i32
│  │  │        │  └─ int 10
│  │  │        └─ int 48
│  │  ├─ = : i32
│  │  │  ├─ num : i32
│  │  │  └─ / : i32
│  │  │     ├─ num : i32
│  │  │     └─ int 10
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 0
│  │  ├─ -= : i32
│  │  │  ├─ i : i32
│  │  │  └─ int 1
│  │  └─ call push : void
│  │     ├─ s : STRUCT_CALL
│  │     └─ index : char
│  │        ├─ digits : char[]
│  │        └─ i : i32
│  ├─ clean : void
│  │  └─ digits : char[]
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.drop(self : STRUCT_CALL) : void
│  └─ clean : void
│     └─ .value : char[]
│        └─ self : STRUCT_CALL
├─ fn String.cap(self : STRUCT_CALL) : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ int 0
│  └─ return
│     └─ cast : i32
│        └─ .len : u64
│           └─ .value : char[]
│              └─ self : STRUCT_CALL
├─ fn String.grow(self : STRUCT_CALL, want : i32) : void
│  ├─ = : i32
│  │  ├─ room : i32
│  │  └─ call cap : i32
│  │     └─ self : STRUCT_CALL
│  ├─ if
│  │  ├─ condition >= : bool
│  │  │  ├─ room : i32
│  │  │  └─ want : i32
│  │  └─ return
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ room : i32
│  │  │  └─ int 0
│  │  └─ = : i32
│  │     ├─ room : i32
│  │     └─ int 16
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ room : i32
│  │  │  └─ want : i32
│  │  └─ *= : i32
│  │     ├─ room : i32
│  │     └─ int 2
│  ├─ = : array
│  │  ├─ bigger : char[]
│  │  └─ array : char[]
│  │     └─ room : i32
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ .count : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ int 0
│  │  └─ call memcpy : pointer
│  │     ├─ bigger : char[]
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ .value : char[]
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ char[] ""
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ bigger : char[]
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ cast : char
│  │     └─ int 0
│  ├─ clean : void
│  │  └─ .value : char[]
│  │     └─ self : STRUCT_CALL
│  └─ = : array
│     ├─ .value : char[]
│     │  └─ self : STRUCT_CALL
│     └─ bigger : char[]
├─ fn String.assign(self : STRUCT_CALL, str : array) : void
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ int 0
│  ├─ if
│  │  ├─ condition != : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ = : i32
│  │     ├─ n : i32
│  │     └─ cast : i32
│  │        └─ call strlen : i64
│  │           └─ str : char[]
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ n : i32
│  │     └─ int 1
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ call memcpy : pointer
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ .value : char[]
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ char[] ""
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ str : char[]
│  │     │  └─ char[] ""
│  │     └─ n : i32
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ n : i32
│  │  └─ cast : char
│  │     └─ int 0
│  └─ = : i32
│     ├─ .count : i32
│     │  └─ self : STRUCT_CALL
│     └─ n : i32
├─ fn String.join(self : STRUCT_CALL, str : array) : void
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ str : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ + : i32
│  │     │  ├─ .count : i32
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ n : i32
│  │     └─ int 1
│  ├─ call memcpy : pointer
│  │  ├─ index : char[]
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ range : i32
│  │  │     ├─ .count : i32
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ + : i32
│  │  │        ├─ .count : i32
│  │  │        │  └─ self : STRUCT_CALL
│  │  │        └─ n : i32
│  │  ├─ str : char[]
│  │  └─ n : i32
│  ├─ += : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ n : i32
│  └─ = : char
│     ├─ index : char
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ .count : i32
│     │     └─ self : STRUCT_CALL
│     └─ cast : char
│        └─ int 0
├─ fn String.push(self : STRUCT_CALL, c : char) : void
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ .count : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 2
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ c : char
│  ├─ += : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 1
│  └─ = : char
│     ├─ index : char
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ .count : i32
│     │     └─ self : STRUCT_CALL
│     └─ cast : char
│        └─ int 0
├─ fn String.pop(self : STRUCT_CALL) : char
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ .count : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ cast : char
│  │        └─ int 0
│  ├─ -= : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 1
│  ├─ = : char
│  │  ├─ c : char
│  │  └─ index : char
│  │     ├─ .value : char[]
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ cast : char
│  │     └─ int 0
│  └─ return
│     └─ c : char
├─ fn String.clear(self : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 0
│  └─ if
│     ├─ condition != : bool
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ NULL_LIT : char[]
│     └─ = : char
│        ├─ index : char
│        │  ├─ .value : char[]
│        │  │  └─ self : STRUCT_CALL
│        │  └─ int 0
│        └─ cast : char
│           └─ int 0
├─ fn String.len(self : STRUCT_CALL) : i32
│  └─ return
│     └─ .count : i32
│        └─ self : STRUCT_CALL
├─ fn String.empty(self : STRUCT_CALL) : bool
│  └─ return
│     └─ == : bool
│        ├─ .count : i32
│        │  └─ self : STRUCT_CALL
│        └─ int 0
├─ fn String.c_str(self : STRUCT_CALL) : char[]
│  └─ return
│     └─ FALLBACK : char[]
│        ├─ .value : char[]
│        │  └─ self : STRUCT_CALL
│        └─ char[] ""
├─ fn String.at(self : STRUCT_CALL, i : i32) : char
│  ├─ if
│  │  ├─ condition or : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ i : i32
│  │  │  │  └─ int 0
│  │  │  └─ >= : bool
│  │  │     ├─ i : i32
│  │  │     └─ .count : i32
│  │  │        └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ cast : char
│  │        └─ int 0
│  └─ return
│     └─ index : char
│        ├─ .value : char[]
│        │  └─ self : STRUCT_CALL
│        └─ i : i32
├─ fn String.find(self : STRUCT_CALL, needle : array) : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ needle : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ int 1
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ needle : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ int 0
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition <= : bool
│  │  │  ├─ + : i32
│  │  │  │  ├─ i : i32
│  │  │  │  └─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ if
│  │  │  ├─ condition == : bool
│  │  │  │  ├─ call strncmp : i32
│  │  │  │  │  ├─ index : char[]
│  │  │  │  │  │  ├─ .value : char[]
│  │  │  │  │  │  │  └─ self : STRUCT_CALL
│  │  │  │  │  │  └─ range : i32
│  │  │  │  │  │     ├─ i : i32
│  │  │  │  │  │     └─ + : i32
│  │  │  │  │  │        ├─ i : i32
│  │  │  │  │  │        └─ n : i32
│  │  │  │  │  ├─ needle : char[]
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ int 0
│  │  │  └─ return
│  │  │     └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ - : i32
│        ├─ int 0
│        └─ int 1
├─ fn String.contains(self : STRUCT_CALL, needle : array) : bool
│  └─ return
│     └─ != : bool
│        ├─ call find : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ needle : char[]
│        └─ - : i32
│           ├─ int 0
│           └─ int 1
├─ fn String.starts_with(self : STRUCT_CALL, prefix : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ prefix : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ bool True
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ prefix : char[]
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ bool False
│  └─ return
│     └─ == : bool
│        ├─ call strncmp : i32
│        │  ├─ index : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ range : i32
│        │  │     ├─ int 0
│        │  │     └─ n : i32
│        │  ├─ prefix : char[]
│        │  └─ n : i32
│        └─ int 0
├─ fn String.ends_with(self : STRUCT_CALL, suffix : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ suffix : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ bool True
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ suffix : char[]
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ bool False
│  └─ return
│     └─ == : bool
│        ├─ call strncmp : i32
│        │  ├─ index : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ range : i32
│        │  │     ├─ - : i32
│        │  │     │  ├─ .count : i32
│        │  │     │  │  └─ self : STRUCT_CALL
│        │  │     │  └─ n : i32
│        │  │     └─ .count : i32
│        │  │        └─ self : STRUCT_CALL
│        │  ├─ suffix : char[]
│        │  └─ n : i32
│        └─ int 0
├─ fn String.substr(self : STRUCT_CALL, start : i32, length : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition or : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ start : i32
│  │  │  │  └─ int 0
│  │  │  └─ >= : bool
│  │  │     ├─ start : i32
│  │  │     └─ .count : i32
│  │  │        └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ stop : i32
│  │  └─ + : i32
│  │     ├─ start : i32
│  │     └─ length : i32
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ stop : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ = : i32
│  │     ├─ stop : i32
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ start : i32
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ stop : i32
│  │  ├─ call push : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.upper(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ >= : bool
│  │  │  │  │  ├─ c : char
│  │  │  │  │  └─ char 'a'
│  │  │  │  └─ <= : bool
│  │  │  │     ├─ c : char
│  │  │  │     └─ char 'z'
│  │  │  ├─ call push : void
│  │  │  │  ├─ res : STRUCT_CALL
│  │  │  │  └─ cast : char
│  │  │  │     └─ - : i32
│  │  │  │        ├─ cast : i32
│  │  │  │        │  └─ c : char
│  │  │  │        └─ int 32
│  │  │  └─ else
│  │  │     └─ call push : void
│  │  │        ├─ res : STRUCT_CALL
│  │  │        └─ c : char
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.lower(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ >= : bool
│  │  │  │  │  ├─ c : char
│  │  │  │  │  └─ char 'A'
│  │  │  │  └─ <= : bool
│  │  │  │     ├─ c : char
│  │  │  │     └─ char 'Z'
│  │  │  ├─ call push : void
│  │  │  │  ├─ res : STRUCT_CALL
│  │  │  │  └─ cast : char
│  │  │  │     └─ + : i32
│  │  │  │        ├─ cast : i32
│  │  │  │        │  └─ c : char
│  │  │  │        └─ int 32
│  │  │  └─ else
│  │  │     └─ call push : void
│  │  │        ├─ res : STRUCT_CALL
│  │  │        └─ c : char
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.trim(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ start : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ start : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ start : i32
│  │  └─ if
│  │     ├─ condition or : bool
│  │     │  ├─ or : bool
│  │     │  │  ├─ or : bool
│  │     │  │  │  ├─ == : bool
│  │     │  │  │  │  ├─ c : char
│  │     │  │  │  │  └─ char ' '
│  │     │  │  │  └─ == : bool
│  │     │  │  │     ├─ c : char
│  │     │  │  │     └─ char '\t'
│  │     │  │  └─ == : bool
│  │     │  │     ├─ c : char
│  │     │  │     └─ char '\n'
│  │     │  └─ == : bool
│  │     │     ├─ c : char
│  │     │     └─ char '\r'
│  │     ├─ += : i32
│  │     │  ├─ start : i32
│  │     │  └─ int 1
│  │     └─ else
│  │        └─ break
│  ├─ = : i32
│  │  ├─ stop : i32
│  │  └─ .count : i32
│  │     └─ self : STRUCT_CALL
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ stop : i32
│  │  │  └─ start : i32
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ - : i32
│  │  │        ├─ stop : i32
│  │  │        └─ int 1
│  │  └─ if
│  │     ├─ condition or : bool
│  │     │  ├─ or : bool
│  │     │  │  ├─ or : bool
│  │     │  │  │  ├─ == : bool
│  │     │  │  │  │  ├─ c : char
│  │     │  │  │  │  └─ char ' '
│  │     │  │  │  └─ == : bool
│  │     │  │  │     ├─ c : char
│  │     │  │  │     └─ char '\t'
│  │     │  │  └─ == : bool
│  │     │  │     ├─ c : char
│  │     │  │     └─ char '\n'
│  │     │  └─ == : bool
│  │     │     ├─ c : char
│  │     │     └─ char '\r'
│  │     ├─ -= : i32
│  │     │  ├─ stop : i32
│  │     │  └─ int 1
│  │     └─ else
│  │        └─ break
│  └─ return
│     └─ call substr : STRUCT_CALL
│        ├─ self : STRUCT_CALL
│        ├─ start : i32
│        └─ - : i32
│           ├─ stop : i32
│           └─ start : i32
├─ fn String.replace(self : STRUCT_CALL, old : array, fresh : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ old : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  ├─ call assign : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ old : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  ├─ call assign : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : bool
│  │  │  ├─ hit : bool
│  │  │  └─ bool False
│  │  ├─ if
│  │  │  ├─ condition <= : bool
│  │  │  │  ├─ + : i32
│  │  │  │  │  ├─ i : i32
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ .count : i32
│  │  │  │     └─ self : STRUCT_CALL
│  │  │  └─ = : bool
│  │  │     ├─ hit : bool
│  │  │     └─ == : bool
│  │  │        ├─ call strncmp : i32
│  │  │        │  ├─ index : char[]
│  │  │        │  │  ├─ .value : char[]
│  │  │        │  │  │  └─ self : STRUCT_CALL
│  │  │        │  │  └─ range : i32
│  │  │        │  │     ├─ i : i32
│  │  │        │  │     └─ + : i32
│  │  │        │  │        ├─ i : i32
│  │  │        │  │        └─ n : i32
│  │  │        │  ├─ old : char[]
│  │  │        │  └─ n : i32
│  │  │        └─ int 0
│  │  └─ if
│  │     ├─ condition hit : bool
│  │     ├─ call join : void
│  │     │  ├─ res : STRUCT_CALL
│  │     │  └─ fresh : char[]
│  │     ├─ += : i32
│  │     │  ├─ i : i32
│  │     │  └─ n : i32
│  │     └─ else
│  │        ├─ call push : void
│  │        │  ├─ res : STRUCT_CALL
│  │        │  └─ index : char
│  │        │     ├─ .value : char[]
│  │        │     │  └─ self : STRUCT_CALL
│  │        │     └─ i : i32
│  │        └─ += : i32
│  │           ├─ i : i32
│  │           └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.repeat(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ n : i32
│  │  ├─ call join : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.reverse(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ - : i32
│  │     ├─ .count : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 1
│  ├─ while
│  │  ├─ condition >= : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  └─ -= : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.compare(self : STRUCT_CALL, other : STRUCT_CALL) : i32
│  └─ return
│     └─ call strcmp : i32
│        ├─ FALLBACK : char[]
│        │  ├─ .value : char[]
│        │  │  └─ self : STRUCT_CALL
│        │  └─ char[] ""
│        └─ FALLBACK : char[]
│           ├─ .value : char[]
│           │  └─ other : STRUCT_CALL
│           └─ char[] ""
├─ fn String.to_int(self : STRUCT_CALL) : i32
│  └─ return
│     └─ call atoi : i32
│        └─ FALLBACK : char[]
│           ├─ .value : char[]
│           │  └─ self : STRUCT_CALL
│           └─ char[] ""
├─ fn String.=.array(self : STRUCT_CALL, str : array) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
├─ fn String.=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ .value : char[]
│        └─ v : STRUCT_CALL
├─ fn String.+.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call from : STRUCT_CALL
│  │     └─ .value : char[]
│  │        └─ self : STRUCT_CALL
│  ├─ call join : void
│  │  ├─ res : STRUCT_CALL
│  │  └─ .value : char[]
│  │     └─ v : STRUCT_CALL
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.+.array(self : STRUCT_CALL, str : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call from : STRUCT_CALL
│  │     └─ .value : char[]
│  │        └─ self : STRUCT_CALL
│  ├─ call join : void
│  │  ├─ res : STRUCT_CALL
│  │  └─ str : char[]
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.+=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : void
│  └─ call join : void
│     ├─ self : STRUCT_CALL
│     └─ .value : char[]
│        └─ v : STRUCT_CALL
├─ fn String.+=.array(self : STRUCT_CALL, str : array) : void
│  └─ call join : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
├─ fn String.==.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ == : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.!=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ != : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.==.array(self : STRUCT_CALL, str : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ == : bool
│  │        ├─ .count : i32
│  │        │  └─ self : STRUCT_CALL
│  │        └─ int 0
│  └─ return
│     └─ == : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ str : char[]
│        └─ int 0
├─ fn String.!=.array(self : STRUCT_CALL, str : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ != : bool
│  │        ├─ .count : i32
│  │        │  └─ self : STRUCT_CALL
│  │        └─ int 0
│  └─ return
│     └─ != : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ str : char[]
│        └─ int 0
├─ fn String.<.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ < : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.>.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ > : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.<.array(self : STRUCT_CALL, str : array) : bool
│  └─ return
│     └─ < : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ FALLBACK : char[]
│        │     ├─ str : char[]
│        │     └─ char[] ""
│        └─ int 0
├─ fn String.>.array(self : STRUCT_CALL, str : array) : bool
│  └─ return
│     └─ > : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ FALLBACK : char[]
│        │     ├─ str : char[]
│        │     └─ char[] ""
│        └─ int 0
└─ fn String.output(self : STRUCT_CALL) : char[]
   └─ return
      └─ FALLBACK : char[]
         ├─ .value : char[]
         │  └─ self : STRUCT_CALL
         └─ char[] ""

struct Timespec
├─ tv_sec : i64
└─ tv_nsec : i64

struct Timeval
├─ tv_sec : i64
├─ tv_usec : i32
└─ __pad : i32

struct Tm
├─ tm_sec : i32
├─ tm_min : i32
├─ tm_hour : i32
├─ tm_mday : i32
├─ tm_mon : i32
├─ tm_year : i32
├─ tm_wday : i32
├─ tm_yday : i32
├─ tm_isdst : i32
├─ tm_gmtoff : i64
└─ tm_zone : pointer

proto fn time(timer : pointer) : i64

proto fn difftime(time1 : i64, time0 : i64) : f64

proto fn clock() : i64

proto fn gmtime(timer : i64) : pointer

proto fn localtime(timer : i64) : pointer

proto fn mktime(timeptr : pointer) : i64

proto fn gmtime_r(timer : i64, result : pointer) : pointer

proto fn localtime_r(timer : i64, result : pointer) : pointer

proto fn strftime(s : pointer, maxsize : i64, format : pointer, timeptr : pointer) : i64

proto fn asctime(timeptr : pointer) : pointer

proto fn ctime(timer : pointer) : pointer

proto fn asctime_r(timeptr : pointer, buf : pointer) : pointer

proto fn ctime_r(timer : pointer, buf : pointer) : pointer

proto fn clock_gettime(clk_id : i32, tp : pointer) : i32

proto fn clock_settime(clk_id : i32, tp : pointer) : i32

proto fn nanosleep(req : pointer, rem : pointer) : i32

proto fn gettimeofday(tv : pointer, tz : pointer) : i32

proto fn getpid() : i32

proto fn getppid() : i32

proto fn getuid() : i32

proto fn geteuid() : i32

proto fn getgid() : i32

proto fn getegid() : i32

proto fn setuid(uid : i32) : i32

proto fn setgid(gid : i32) : i32

proto fn fork() : i32

proto fn execv(path : pointer, argv : pointer) : i32

proto fn execve(path : pointer, argv : pointer, envp : pointer) : i32

proto fn execvp(file : pointer, argv : pointer) : i32

proto fn _exit(code : i32) : void

proto fn getcwd(buf : pointer, size : i64) : pointer

proto fn chdir(path : pointer) : i32

proto fn fchdir(fd : i32) : i32

proto fn access(path : pointer, mode : i32) : i32

proto fn unlink(path : pointer) : i32

proto fn rmdir(path : pointer) : i32

proto fn link(oldpath : pointer, newpath : pointer) : i32

proto fn symlink(target : pointer, linkpath : pointer) : i32

proto fn readlink(path : pointer, buf : pointer, bufsize : i64) : i64

proto fn rename(oldpath : pointer, newpath : pointer) : i32

proto fn truncate(path : pointer, length : i64) : i32

proto fn ftruncate(fd : i32, length : i64) : i32

proto fn close(fd : i32) : i32

proto fn dup(fd : i32) : i32

proto fn dup2(oldfd : i32, newfd : i32) : i32

proto fn pipe(pipefd : pointer) : i32

proto fn lseek(fd : i32, offset : i64, whence : i32) : i64

proto fn isatty(fd : i32) : i32

proto fn ttyname(fd : i32) : pointer

proto fn sleep(seconds : i32) : i32

proto fn usleep(microseconds : i32) : i32

proto fn gethostname(name : pointer, len : i64) : i32

proto fn sethostname(name : pointer, len : i64) : i32

proto fn alarm(seconds : i32) : i32

proto fn pause() : i32

proto fn sync() : void

proto fn fsync(fd : i32) : i32

proto fn fdatasync(fd : i32) : i32

= : i32
├─ F_OK : i32
└─ int 0

= : i32
├─ X_OK : i32
└─ int 1

= : i32
├─ W_OK : i32
└─ int 2

= : i32
├─ R_OK : i32
└─ int 4

= : i32
├─ SEEK_SET : i32
└─ int 0

= : i32
├─ SEEK_CUR : i32
└─ int 1

= : i32
├─ SEEK_END : i32
└─ int 2

= : i32
├─ STDIN_FILENO : i32
└─ int 0

= : i32
├─ STDOUT_FILENO : i32
└─ int 1

= : i32
├─ STDERR_FILENO : i32
└─ int 2

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
├─ ENUM_CALL
├─ ENUM_CALL
├─ ENUM_CALL
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

struct Token
├─ value : i32
├─ type : i32
├─ fn Token.number(v : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ t : STRUCT_CALL
│  │  └─ new Token
│  ├─ = : i32
│  │  ├─ .type : i32
│  │  │  └─ t : STRUCT_CALL
│  │  └─ int 1
│  ├─ = : i32
│  │  ├─ .value : i32
│  │  │  └─ t : STRUCT_CALL
│  │  └─ v : i32
│  └─ return
│     └─ ref : STRUCT_CALL
│        └─ t : STRUCT_CALL
└─ fn Token.op(k : i32) : STRUCT_CALL
   ├─ = : STRUCT_CALL
   │  ├─ t : STRUCT_CALL
   │  └─ new Token
   ├─ = : i32
   │  ├─ .type : i32
   │  │  └─ t : STRUCT_CALL
   │  └─ k : i32
   └─ return
      └─ ref : STRUCT_CALL
         └─ t : STRUCT_CALL

struct Node
├─ token : STRUCT_CALL
├─ left : STRUCT_CALL
├─ right : STRUCT_CALL
├─ fn Node.create(token : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ n : STRUCT_CALL
│  │  └─ new Node
│  ├─ = : STRUCT_CALL
│  │  ├─ .token : STRUCT_CALL
│  │  │  └─ n : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ token : STRUCT_CALL
│  └─ return
│     └─ ref : STRUCT_CALL
│        └─ n : STRUCT_CALL
└─ fn Node.drop(self : STRUCT_CALL) : void
   ├─ clean : void
   │  └─ .token : STRUCT_CALL
   │     └─ self : STRUCT_CALL
   ├─ clean : void
   │  └─ .left : STRUCT_CALL
   │     └─ self : STRUCT_CALL
   └─ clean : void
      └─ .right : STRUCT_CALL
         └─ self : STRUCT_CALL

mod Lexer
└─ fn Lexer.run(src : STRUCT_CALL, toks : array) : i32
   ├─ fn is_digit(c : char) : bool
   │  └─ return
   │     └─ and : bool
   │        ├─ >= : bool
   │        │  ├─ c : char
   │        │  └─ char '0'
   │        └─ <= : bool
   │           ├─ c : char
   │           └─ char '9'
   ├─ = : i32
   │  ├─ n : i32
   │  └─ int 0
   ├─ = : i32
   │  ├─ i : i32
   │  └─ int 0
   ├─ while
   │  ├─ condition < : bool
   │  │  ├─ i : i32
   │  │  └─ call len : i32
   │  │     └─ src : STRUCT_CALL
   │  ├─ = : char
   │  │  ├─ c : char
   │  │  └─ call at : char
   │  │     ├─ src : STRUCT_CALL
   │  │     └─ i : i32
   │  └─ if
   │     ├─ condition == : bool
   │     │  ├─ c : char
   │     │  └─ char ' '
   │     ├─ = : i32
   │     │  ├─ i : i32
   │     │  └─ + : i32
   │     │     ├─ i : i32
   │     │     └─ int 1
   │     ├─ continue
   │     └─ elif
   │        ├─ condition call is_digit : bool
   │        │  └─ c : char
   │        ├─ = : i32
   │        │  ├─ v : i32
   │        │  └─ int 0
   │        ├─ while
   │        │  ├─ condition and : bool
   │        │  │  ├─ < : bool
   │        │  │  │  ├─ i : i32
   │        │  │  │  └─ call len : i32
   │        │  │  │     └─ src : STRUCT_CALL
   │        │  │  └─ call is_digit : bool
   │        │  │     └─ call at : char
   │        │  │        ├─ src : STRUCT_CALL
   │        │  │        └─ i : i32
   │        │  ├─ = : i32
   │        │  │  ├─ v : i32
   │        │  │  └─ + : i32
   │        │  │     ├─ * : i32
   │        │  │     │  ├─ v : i32
   │        │  │     │  └─ int 10
   │        │  │     └─ - : i32
   │        │  │        ├─ cast : i32
   │        │  │        │  └─ call at : char
   │        │  │        │     ├─ src : STRUCT_CALL
   │        │  │        │     └─ i : i32
   │        │  │        └─ int 48
   │        │  └─ = : i32
   │        │     ├─ i : i32
   │        │     └─ + : i32
   │        │        ├─ i : i32
   │        │        └─ int 1
   │        ├─ = : i32
   │        │  ├─ .type : i32
   │        │  │  └─ index : STRUCT_CALL
   │        │  │     ├─ toks : STRUCT_CALL[]
   │        │  │     └─ n : i32
   │        │  └─ int 1
   │        ├─ = : i32
   │        │  ├─ .value : i32
   │        │  │  └─ index : STRUCT_CALL
   │        │  │     ├─ toks : STRUCT_CALL[]
   │        │  │     └─ n : i32
   │        │  └─ v : i32
   │        ├─ = : i32
   │        │  ├─ n : i32
   │        │  └─ + : i32
   │        │     ├─ n : i32
   │        │     └─ int 1
   │        ├─ continue
   │        └─ else
   │           ├─ = : i32
   │           │  ├─ k : i32
   │           │  └─ int 1
   │           ├─ match
   │           │  ├─ subject c : char
   │           │  ├─ case
   │           │  │  ├─ value char '+'
   │           │  │  └─ = : i32
   │           │  │     ├─ k : i32
   │           │  │     └─ int 2
   │           │  ├─ case
   │           │  │  ├─ value char '-'
   │           │  │  └─ = : i32
   │           │  │     ├─ k : i32
   │           │  │     └─ int 3
   │           │  ├─ case
   │           │  │  ├─ value char '*'
   │           │  │  └─ = : i32
   │           │  │     ├─ k : i32
   │           │  │     └─ int 4
   │           │  ├─ case
   │           │  │  ├─ value char '/'
   │           │  │  └─ = : i32
   │           │  │     ├─ k : i32
   │           │  │     └─ int 5
   │           │  ├─ case
   │           │  │  ├─ value char '('
   │           │  │  └─ = : i32
   │           │  │     ├─ k : i32
   │           │  │     └─ int 6
   │           │  └─ case
   │           │     ├─ value char ')'
   │           │     └─ = : i32
   │           │        ├─ k : i32
   │           │        └─ int 7
   │           ├─ = : i32
   │           │  ├─ .type : i32
   │           │  │  └─ index : STRUCT_CALL
   │           │  │     ├─ toks : STRUCT_CALL[]
   │           │  │     └─ n : i32
   │           │  └─ k : i32
   │           ├─ = : i32
   │           │  ├─ .value : i32
   │           │  │  └─ index : STRUCT_CALL
   │           │  │     ├─ toks : STRUCT_CALL[]
   │           │  │     └─ n : i32
   │           │  └─ int 0
   │           ├─ = : i32
   │           │  ├─ n : i32
   │           │  └─ + : i32
   │           │     ├─ n : i32
   │           │     └─ int 1
   │           └─ = : i32
   │              ├─ i : i32
   │              └─ + : i32
   │                 ├─ i : i32
   │                 └─ int 1
   └─ return
      └─ n : i32

mod Parser
├─ fn Parser.factor(toks : array, p : i32, n : i32) : STRUCT_CALL
│  ├─ if
│  │  ├─ condition and : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ p : i32
│  │  │  │  └─ n : i32
│  │  │  └─ == : bool
│  │  │     ├─ .type : i32
│  │  │     │  └─ index : STRUCT_CALL
│  │  │     │     ├─ toks : STRUCT_CALL[]
│  │  │     │     └─ p : i32
│  │  │     └─ int 6
│  │  ├─ = : i32
│  │  │  ├─ p : i32
│  │  │  └─ + : i32
│  │  │     ├─ p : i32
│  │  │     └─ int 1
│  │  ├─ = : STRUCT_CALL
│  │  │  ├─ inner : STRUCT_CALL
│  │  │  └─ call expr : STRUCT_CALL
│  │  │     ├─ toks : STRUCT_CALL[]
│  │  │     ├─ ref : i32
│  │  │     │  └─ p : i32
│  │  │     └─ n : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ < : bool
│  │  │  │  │  ├─ p : i32
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ == : bool
│  │  │  │     ├─ .type : i32
│  │  │  │     │  └─ index : STRUCT_CALL
│  │  │  │     │     ├─ toks : STRUCT_CALL[]
│  │  │  │     │     └─ p : i32
│  │  │  │     └─ int 7
│  │  │  └─ = : i32
│  │  │     ├─ p : i32
│  │  │     └─ + : i32
│  │  │        ├─ p : i32
│  │  │        └─ int 1
│  │  └─ return
│  │     └─ ref : STRUCT_CALL
│  │        └─ inner : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ lf : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  │     └─ call number : STRUCT_CALL
│  │        └─ .value : i32
│  │           └─ index : STRUCT_CALL
│  │              ├─ toks : STRUCT_CALL[]
│  │              └─ p : i32
│  ├─ = : i32
│  │  ├─ p : i32
│  │  └─ + : i32
│  │     ├─ p : i32
│  │     └─ int 1
│  └─ return
│     └─ ref : STRUCT_CALL
│        └─ lf : STRUCT_CALL
├─ fn Parser.term(toks : array, p : i32, n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ l : STRUCT_CALL
│  │  └─ call factor : STRUCT_CALL
│  │     ├─ toks : STRUCT_CALL[]
│  │     ├─ ref : i32
│  │     │  └─ p : i32
│  │     └─ n : i32
│  ├─ while
│  │  ├─ condition and : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ p : i32
│  │  │  │  └─ n : i32
│  │  │  └─ or : bool
│  │  │     ├─ == : bool
│  │  │     │  ├─ .type : i32
│  │  │     │  │  └─ index : STRUCT_CALL
│  │  │     │  │     ├─ toks : STRUCT_CALL[]
│  │  │     │  │     └─ p : i32
│  │  │     │  └─ int 4
│  │  │     └─ == : bool
│  │  │        ├─ .type : i32
│  │  │        │  └─ index : STRUCT_CALL
│  │  │        │     ├─ toks : STRUCT_CALL[]
│  │  │        │     └─ p : i32
│  │  │        └─ int 5
│  │  ├─ = : i32
│  │  │  ├─ k : i32
│  │  │  └─ .type : i32
│  │  │     └─ index : STRUCT_CALL
│  │  │        ├─ toks : STRUCT_CALL[]
│  │  │        └─ p : i32
│  │  ├─ = : i32
│  │  │  ├─ p : i32
│  │  │  └─ + : i32
│  │  │     ├─ p : i32
│  │  │     └─ int 1
│  │  ├─ = : STRUCT_CALL
│  │  │  ├─ r : STRUCT_CALL
│  │  │  └─ call factor : STRUCT_CALL
│  │  │     ├─ toks : STRUCT_CALL[]
│  │  │     ├─ ref : i32
│  │  │     │  └─ p : i32
│  │  │     └─ n : i32
│  │  ├─ = : STRUCT_CALL
│  │  │  ├─ parent : STRUCT_CALL
│  │  │  └─ call create : STRUCT_CALL
│  │  │     └─ call op : STRUCT_CALL
│  │  │        └─ k : i32
│  │  ├─ = : STRUCT_CALL
│  │  │  ├─ .left : STRUCT_CALL
│  │  │  │  └─ parent : STRUCT_CALL
│  │  │  └─ ref : STRUCT_CALL
│  │  │     └─ l : STRUCT_CALL
│  │  ├─ = : STRUCT_CALL
│  │  │  ├─ .right : STRUCT_CALL
│  │  │  │  └─ parent : STRUCT_CALL
│  │  │  └─ ref : STRUCT_CALL
│  │  │     └─ r : STRUCT_CALL
│  │  └─ = : STRUCT_CALL
│  │     ├─ l : STRUCT_CALL
│  │     └─ ref : STRUCT_CALL
│  │        └─ parent : STRUCT_CALL
│  └─ return
│     └─ ref : STRUCT_CALL
│        └─ l : STRUCT_CALL
└─ fn Parser.expr(toks : array, p : i32, n : i32) : STRUCT_CALL
   ├─ = : STRUCT_CALL
   │  ├─ l : STRUCT_CALL
   │  └─ call term : STRUCT_CALL
   │     ├─ toks : STRUCT_CALL[]
   │     ├─ ref : i32
   │     │  └─ p : i32
   │     └─ n : i32
   ├─ while
   │  ├─ condition and : bool
   │  │  ├─ < : bool
   │  │  │  ├─ p : i32
   │  │  │  └─ n : i32
   │  │  └─ or : bool
   │  │     ├─ == : bool
   │  │     │  ├─ .type : i32
   │  │     │  │  └─ index : STRUCT_CALL
   │  │     │  │     ├─ toks : STRUCT_CALL[]
   │  │     │  │     └─ p : i32
   │  │     │  └─ int 2
   │  │     └─ == : bool
   │  │        ├─ .type : i32
   │  │        │  └─ index : STRUCT_CALL
   │  │        │     ├─ toks : STRUCT_CALL[]
   │  │        │     └─ p : i32
   │  │        └─ int 3
   │  ├─ = : i32
   │  │  ├─ k : i32
   │  │  └─ .type : i32
   │  │     └─ index : STRUCT_CALL
   │  │        ├─ toks : STRUCT_CALL[]
   │  │        └─ p : i32
   │  ├─ = : i32
   │  │  ├─ p : i32
   │  │  └─ + : i32
   │  │     ├─ p : i32
   │  │     └─ int 1
   │  ├─ = : STRUCT_CALL
   │  │  ├─ r : STRUCT_CALL
   │  │  └─ call term : STRUCT_CALL
   │  │     ├─ toks : STRUCT_CALL[]
   │  │     ├─ ref : i32
   │  │     │  └─ p : i32
   │  │     └─ n : i32
   │  ├─ = : STRUCT_CALL
   │  │  ├─ parent : STRUCT_CALL
   │  │  └─ call create : STRUCT_CALL
   │  │     └─ call op : STRUCT_CALL
   │  │        └─ k : i32
   │  ├─ = : STRUCT_CALL
   │  │  ├─ .left : STRUCT_CALL
   │  │  │  └─ parent : STRUCT_CALL
   │  │  └─ ref : STRUCT_CALL
   │  │     └─ l : STRUCT_CALL
   │  ├─ = : STRUCT_CALL
   │  │  ├─ .right : STRUCT_CALL
   │  │  │  └─ parent : STRUCT_CALL
   │  │  └─ ref : STRUCT_CALL
   │  │     └─ r : STRUCT_CALL
   │  └─ = : STRUCT_CALL
   │     ├─ l : STRUCT_CALL
   │     └─ ref : STRUCT_CALL
   │        └─ parent : STRUCT_CALL
   └─ return
      └─ ref : STRUCT_CALL
         └─ l : STRUCT_CALL

fn sign(k : i32) : char[]
└─ match
   ├─ subject k : i32
   ├─ case
   │  ├─ value int 2
   │  └─ return
   │     └─ char[] "+"
   ├─ case
   │  ├─ value int 3
   │  └─ return
   │     └─ char[] "-"
   ├─ case
   │  ├─ value int 4
   │  └─ return
   │     └─ char[] "*"
   ├─ case
   │  ├─ value int 5
   │  └─ return
   │     └─ char[] "/"
   └─ default
      └─ return
         └─ char[] "?"

fn label(n : STRUCT_CALL) : void
└─ if
   ├─ condition == : bool
   │  ├─ .type : i32
   │  │  └─ .token : STRUCT_CALL
   │  │     └─ n : STRUCT_CALL
   │  └─ int 1
   ├─ output : void
   │  ├─ .value : i32
   │  │  └─ .token : STRUCT_CALL
   │  │     └─ n : STRUCT_CALL
   │  └─ char[] "\n"
   └─ else
      └─ output : void
         ├─ call sign : char[]
         │  └─ .type : i32
         │     └─ .token : STRUCT_CALL
         │        └─ n : STRUCT_CALL
         └─ char[] "\n"

fn show(n : STRUCT_CALL, pre : array, last : bool) : void
├─ output : void
│  └─ pre : char[]
├─ if
│  ├─ condition last : bool
│  ├─ output : void
│  │  └─ char[] "└─ "
│  └─ else
│     └─ output : void
│        └─ char[] "├─ "
├─ call label : void
│  └─ ref : STRUCT_CALL
│     └─ n : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ kid : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ pre : char[]
├─ if
│  ├─ condition last : bool
│  ├─ call join : void
│  │  ├─ kid : STRUCT_CALL
│  │  └─ char[] "   "
│  └─ else
│     └─ call join : void
│        ├─ kid : STRUCT_CALL
│        └─ char[] "│  "
├─ if
│  ├─ condition != : bool
│  │  ├─ .left : STRUCT_CALL
│  │  │  └─ n : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ call show : void
│     ├─ ref : STRUCT_CALL
│     │  └─ .left : STRUCT_CALL
│     │     └─ n : STRUCT_CALL
│     ├─ call c_str : char[]
│     │  └─ kid : STRUCT_CALL
│     └─ == : bool
│        ├─ .right : STRUCT_CALL
│        │  └─ n : STRUCT_CALL
│        └─ NULL_LIT : STRUCT_CALL
└─ if
   ├─ condition != : bool
   │  ├─ .right : STRUCT_CALL
   │  │  └─ n : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   └─ call show : void
      ├─ ref : STRUCT_CALL
      │  └─ .right : STRUCT_CALL
      │     └─ n : STRUCT_CALL
      ├─ call c_str : char[]
      │  └─ kid : STRUCT_CALL
      └─ bool True

fn tree(n : STRUCT_CALL) : void
├─ call label : void
│  └─ ref : STRUCT_CALL
│     └─ n : STRUCT_CALL
├─ if
│  ├─ condition != : bool
│  │  ├─ .left : STRUCT_CALL
│  │  │  └─ n : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  └─ call show : void
│     ├─ ref : STRUCT_CALL
│     │  └─ .left : STRUCT_CALL
│     │     └─ n : STRUCT_CALL
│     ├─ char[] ""
│     └─ == : bool
│        ├─ .right : STRUCT_CALL
│        │  └─ n : STRUCT_CALL
│        └─ NULL_LIT : STRUCT_CALL
└─ if
   ├─ condition != : bool
   │  ├─ .right : STRUCT_CALL
   │  │  └─ n : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   └─ call show : void
      ├─ ref : STRUCT_CALL
      │  └─ .right : STRUCT_CALL
      │     └─ n : STRUCT_CALL
      ├─ char[] ""
      └─ bool True

fn eval(n : STRUCT_CALL) : i32
└─ match
   ├─ subject .type : i32
   │  └─ .token : STRUCT_CALL
   │     └─ n : STRUCT_CALL
   ├─ case
   │  ├─ value int 1
   │  └─ return
   │     └─ .value : i32
   │        └─ .token : STRUCT_CALL
   │           └─ n : STRUCT_CALL
   ├─ case
   │  ├─ value int 2
   │  └─ return
   │     └─ + : i32
   │        ├─ call eval : i32
   │        │  └─ ref : STRUCT_CALL
   │        │     └─ .left : STRUCT_CALL
   │        │        └─ n : STRUCT_CALL
   │        └─ call eval : i32
   │           └─ ref : STRUCT_CALL
   │              └─ .right : STRUCT_CALL
   │                 └─ n : STRUCT_CALL
   ├─ case
   │  ├─ value int 3
   │  └─ return
   │     └─ - : i32
   │        ├─ call eval : i32
   │        │  └─ ref : STRUCT_CALL
   │        │     └─ .left : STRUCT_CALL
   │        │        └─ n : STRUCT_CALL
   │        └─ call eval : i32
   │           └─ ref : STRUCT_CALL
   │              └─ .right : STRUCT_CALL
   │                 └─ n : STRUCT_CALL
   ├─ case
   │  ├─ value int 4
   │  └─ return
   │     └─ * : i32
   │        ├─ call eval : i32
   │        │  └─ ref : STRUCT_CALL
   │        │     └─ .left : STRUCT_CALL
   │        │        └─ n : STRUCT_CALL
   │        └─ call eval : i32
   │           └─ ref : STRUCT_CALL
   │              └─ .right : STRUCT_CALL
   │                 └─ n : STRUCT_CALL
   ├─ case
   │  ├─ value int 5
   │  ├─ = : i32
   │  │  ├─ d : i32
   │  │  └─ call eval : i32
   │  │     └─ ref : STRUCT_CALL
   │  │        └─ .right : STRUCT_CALL
   │  │           └─ n : STRUCT_CALL
   │  ├─ if
   │  │  ├─ condition == : bool
   │  │  │  ├─ d : i32
   │  │  │  └─ int 0
   │  │  └─ throw
   │  │     └─ call make : STRUCT_CALL
   │  │        └─ char[] "division by zero"
   │  └─ return
   │     └─ / : i32
   │        ├─ call eval : i32
   │        │  └─ ref : STRUCT_CALL
   │        │     └─ .left : STRUCT_CALL
   │        │        └─ n : STRUCT_CALL
   │        └─ d : i32
   └─ default
      └─ return
         └─ int 0

fn calc(text : array) : void
├─ output : void
│  └─ char[] "========================================\n"
├─ output : void
│  ├─ char[] "calculate expr:  "
│  ├─ text : char[]
│  └─ char[] "\nast:\n"
├─ = : STRUCT_CALL
│  ├─ src : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ text : char[]
├─ = : array
│  ├─ toks : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 64
├─ = : i32
│  ├─ n : i32
│  └─ call run : i32
│     ├─ ref : STRUCT_CALL
│     │  └─ src : STRUCT_CALL
│     └─ toks : STRUCT_CALL[]
├─ = : i32
│  ├─ p : i32
│  └─ int 0
├─ = : STRUCT_CALL
│  ├─ root : STRUCT_CALL
│  └─ call expr : STRUCT_CALL
│     ├─ toks : STRUCT_CALL[]
│     ├─ ref : i32
│     │  └─ p : i32
│     └─ n : i32
├─ call tree : void
│  └─ ref : STRUCT_CALL
│     └─ root : STRUCT_CALL
├─ try
│  ├─ output : void
│  │  ├─ char[] "result: "
│  │  ├─ call eval : i32
│  │  │  └─ ref : STRUCT_CALL
│  │  │     └─ root : STRUCT_CALL
│  │  └─ char[] "\n"
│  └─ catch
│     ├─ binds e : STRUCT_CALL
│     └─ errput : void
│        ├─ char[] "\nError: "
│        ├─ .message : char[]
│        │  └─ e : STRUCT_CALL
│        └─ char[] "\n"
├─ clean : void
│  └─ root : STRUCT_CALL
└─ clean : void
   └─ toks : STRUCT_CALL[]

fn main() : i32
├─ call calc : void
│  └─ char[] "2 * (3 + 4) - 10 / 5"
├─ call calc : void
│  └─ char[] "8 / (3 - 3)"
└─ output : void
   └─ char[] "========================================\n"
```

```out
========================================
calculate expr:  2 * (3 + 4) - 10 / 5
ast:
-
├─ *
│  ├─ 2
│  └─ +
│     ├─ 3
│     └─ 4
└─ /
   ├─ 10
   └─ 5
result: 12
========================================
calculate expr:  8 / (3 - 3)
ast:
/
├─ 8
└─ -
   ├─ 3
   └─ 3
========================================
```

```err
[0;31m
Error: division by zero
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }
%String = type { { i8*, i64 }, i32 }
%Token = type { i32, i32 }
%Node = type { %Token*, %Node*, %Node* }

@os = internal global %Os zeroinitializer
@EPERM = internal global i32 0
@ENOENT = internal global i32 0
@ESRCH = internal global i32 0
@EINTR = internal global i32 0
@EIO = internal global i32 0
@ENXIO = internal global i32 0
@E2BIG = internal global i32 0
@ENOEXEC = internal global i32 0
@EBADF = internal global i32 0
@ECHILD = internal global i32 0
@EACCES = internal global i32 0
@EFAULT = internal global i32 0
@ENOTBLK = internal global i32 0
@EBUSY = internal global i32 0
@EEXIST = internal global i32 0
@EXDEV = internal global i32 0
@ENODEV = internal global i32 0
@ENOTDIR = internal global i32 0
@EISDIR = internal global i32 0
@EINVAL = internal global i32 0
@ENFILE = internal global i32 0
@EMFILE = internal global i32 0
@ENOTTY = internal global i32 0
@ETXTBSY = internal global i32 0
@EFBIG = internal global i32 0
@ENOSPC = internal global i32 0
@ESPIPE = internal global i32 0
@EROFS = internal global i32 0
@EMLINK = internal global i32 0
@EPIPE = internal global i32 0
@EDOM = internal global i32 0
@ERANGE = internal global i32 0
@O_RDONLY = internal global i32 0
@O_WRONLY = internal global i32 0
@O_RDWR = internal global i32 0
@FD_CLOEXEC = internal global i32 0
@IPPROTO_IP = internal global i32 0
@IPPROTO_TCP = internal global i32 0
@IPPROTO_UDP = internal global i32 0
@IPPROTO_IPV6 = internal global i32 0
@SHUT_RD = internal global i32 0
@SHUT_WR = internal global i32 0
@SHUT_RDWR = internal global i32 0
@POLLIN = internal global i32 0
@POLLPRI = internal global i32 0
@POLLOUT = internal global i32 0
@POLLERR = internal global i32 0
@POLLHUP = internal global i32 0
@POLLNVAL = internal global i32 0
@INADDR_ANY = internal global i32 0
@INADDR_LOOPBACK = internal global i32 0
@INADDR_BROADCAST = internal global i32 0
@SIGHUP = internal global i32 0
@SIGINT = internal global i32 0
@SIGQUIT = internal global i32 0
@SIGILL = internal global i32 0
@SIGTRAP = internal global i32 0
@SIGABRT = internal global i32 0
@SIGFPE = internal global i32 0
@SIGKILL = internal global i32 0
@SIGSEGV = internal global i32 0
@SIGPIPE = internal global i32 0
@SIGALRM = internal global i32 0
@SIGTERM = internal global i32 0
@S_IFMT = internal global i32 0
@S_IFSOCK = internal global i32 0
@S_IFLNK = internal global i32 0
@S_IFREG = internal global i32 0
@S_IFBLK = internal global i32 0
@S_IFDIR = internal global i32 0
@S_IFCHR = internal global i32 0
@S_IFIFO = internal global i32 0
@S_ISUID = internal global i32 0
@S_ISGID = internal global i32 0
@S_ISVTX = internal global i32 0
@F_OK = internal global i32 0
@X_OK = internal global i32 0
@W_OK = internal global i32 0
@R_OK = internal global i32 0
@SEEK_SET = internal global i32 0
@SEEK_CUR = internal global i32 0
@SEEK_END = internal global i32 0
@STDIN_FILENO = internal global i32 0
@STDOUT_FILENO = internal global i32 0
@STDERR_FILENO = internal global i32 0
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@trap_msg = private unnamed_addr constant [153 x i8] c"runtime error: Modulo by zero\0A   ura-lib/string.ura:97:30\0A   |\0A97 |             digits[i] = (num % 10 + 48) as char\0A   |                              ^\0A\00", align 1
@trap_msg.1 = private unnamed_addr constant [127 x i8] c"runtime error: Division by zero\0A   ura-lib/string.ura:98:23\0A   |\0A98 |             num = num / 10\0A   |                       ^\0A\00", align 1
@str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.5 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.6 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.7 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.8 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.10 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.11 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.12 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.13 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.14 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.15 = private unnamed_addr constant [2 x i8] c"+\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"-\00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"*\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"/\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"?\00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@fmt.23 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.24 = private unnamed_addr constant [8 x i8] c"\E2\94\94\E2\94\80 \00", align 1
@fmt.25 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.26 = private unnamed_addr constant [8 x i8] c"\E2\94\9C\E2\94\80 \00", align 1
@fmt.27 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.28 = private unnamed_addr constant [4 x i8] c"   \00", align 1
@str.29 = private unnamed_addr constant [6 x i8] c"\E2\94\82  \00", align 1
@str.30 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.31 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@trap_msg.32 = private unnamed_addr constant [232 x i8] c"runtime error: reference 'n' used before it was bound - assign 'n = ref <target>' first\0A    calculator.ura:137:11\0A    |\0A137 |     match n?.token?.type:\0A    |           ^\0A\00", align 1
@trap_msg.33 = private unnamed_addr constant [242 x i8] c"runtime error: reference 'token' used before it was bound - assign 'token = ref <target>' first\0A    calculator.ura:137:13\0A    |\0A137 |     match n?.token?.type:\0A    |             ^\0A\00", align 1
@str.34 = private unnamed_addr constant [17 x i8] c"division by zero\00", align 1
@trap_msg.35 = private unnamed_addr constant [210 x i8] c"runtime error: Division by zero\0A    calculator.ura:145:34\0A    |\0A145 |             ret eval(ref n.left) / d\0A    |                                  ^\0A\00", align 1
@str.36 = private unnamed_addr constant [42 x i8] c"========================================\0A\00", align 1
@fmt.37 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.38 = private unnamed_addr constant [18 x i8] c"calculate expr:  \00", align 1
@str.39 = private unnamed_addr constant [7 x i8] c"\0Aast:\0A\00", align 1
@fmt.40 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.41 = private unnamed_addr constant [9 x i8] c"result: \00", align 1
@str.42 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.43 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@fmt.44 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.45 = private unnamed_addr constant [9 x i8] c"\0AError: \00", align 1
@str.46 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.47 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@fmt.48 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@str.49 = private unnamed_addr constant [21 x i8] c"2 * (3 + 4) - 10 / 5\00", align 1
@str.50 = private unnamed_addr constant [12 x i8] c"8 / (3 - 3)\00", align 1
@str.51 = private unnamed_addr constant [42 x i8] c"========================================\0A\00", align 1
@fmt.52 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %String @String.create() {
entry:
  %s = alloca %String, align 8
  store %String zeroinitializer, %String* %s, align 8
  %value = getelementptr %String, %String* %s, i32 0, i32 0
  %heap = call i8* @calloc(i64 16, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 16, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %value, align 8
  %count = getelementptr %String, %String* %s, i32 0, i32 1
  store i32 0, i32* %count, align 4
  %s1 = load %String, %String* %s, align 8
  ret %String %s1
}

declare i8* @calloc(i64, i64)

define %String @String.from({ i8*, i64 } %0) {
entry:
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %str, align 8
  %s = alloca %String, align 8
  %call = call %String @String.create()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %s)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %eflag4 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag4, label %throw.unwind2, label %throw.cont3

throw.unwind2:                                    ; preds = %throw.cont
  call void @String.drop(%String* %s)
  ret %String zeroinitializer

throw.cont3:                                      ; preds = %throw.cont
  %s5 = load %String, %String* %s, align 8
  ret %String %s5
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value, align 8
  ret void
}

define void @String.assign(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %throw.cont, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %eflag6 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag6, label %throw.unwind4, label %throw.cont5

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %then
  ret void

throw.cont:                                       ; preds = %then
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

throw.unwind4:                                    ; preds = %endif
  ret void

throw.cont5:                                      ; preds = %endif
  %n9 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n9, 0
  br i1 %gt, label %then8, label %endif7

endif7:                                           ; preds = %throw.cont22, %throw.cont5
  %ref24 = load %String*, %String** %self, align 8
  %value25 = getelementptr %String, %String* %ref24, i32 0, i32 0
  %value26 = load { i8*, i64 }, { i8*, i64 }* %value25, align 8
  %arr.data27 = extractvalue { i8*, i64 } %value26, 0
  %n28 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data27, i32 %n28
  store i8 0, i8* %arr.at, align 1
  %ref29 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref29, i32 0, i32 1
  %n30 = load i32, i32* %n, align 4
  store i32 %n30, i32* %count, align 4
  ret void

then8:                                            ; preds = %throw.cont5
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %value11, 0
  %isnull = icmp eq i8* %opt.ptr12, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value11
  %arr.data13 = extractvalue { i8*, i64 } %fallback, 0
  %str14 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr15 = extractvalue { i8*, i64 } %str14, 0
  %isnull16 = icmp eq i8* %opt.ptr15, null
  %fallback17 = select i1 %isnull16, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str14
  %arr.data18 = extractvalue { i8*, i64 } %fallback17, 0
  %n19 = load i32, i32* %n, align 4
  %call20 = call i8* @memcpy(i8* %arr.data13, i8* %arr.data18, i32 %n19)
  %eflag23 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag23, label %throw.unwind21, label %throw.cont22

throw.unwind21:                                   ; preds = %then8
  ret void

throw.cont22:                                     ; preds = %then8
  br label %endif7
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %s)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %throw.cont
  %num = alloca i32, align 4
  %n6 = load i32, i32* %n, align 4
  store i32 %n6, i32* %num, align 4
  %num9 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num9, 0
  br i1 %lt, label %then8, label %endif7

then:                                             ; preds = %throw.cont
  call void @String.push(%String* %s, i8 48)
  %eflag4 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag4, label %throw.unwind2, label %throw.cont3

throw.unwind2:                                    ; preds = %then
  call void @String.drop(%String* %s)
  ret %String zeroinitializer

throw.cont3:                                      ; preds = %then
  %s5 = load %String, %String* %s, align 8
  ret %String %s5

endif7:                                           ; preds = %throw.cont11, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then8:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %eflag12 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag12, label %throw.unwind10, label %throw.cont11

throw.unwind10:                                   ; preds = %then8
  call void @String.drop(%String* %s)
  ret %String zeroinitializer

throw.cont11:                                     ; preds = %then8
  %num13 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num13
  store i32 %sub, i32* %num, align 4
  br label %endif7

while.cond:                                       ; preds = %cont20, %endif7
  %num14 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num14, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits15 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits15, 0
  %i16 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i16
  %num17 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond22

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num17, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num18 = load i32, i32* %num, align 4
  br i1 false, label %trap19, label %cont20

trap19:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont20:                                           ; preds = %cont
  %div = sdiv i32 %num18, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add21 = add i32 %cur, 1
  store i32 %add21, i32* %i, align 4
  br label %while.cond

while.cond22:                                     ; preds = %throw.cont34, %while.end
  %i25 = load i32, i32* %i, align 4
  %gt26 = icmp sgt i32 %i25, 0
  br i1 %gt26, label %while.body23, label %while.end24

while.body23:                                     ; preds = %while.cond22
  %cur27 = load i32, i32* %i, align 4
  %sub28 = sub i32 %cur27, 1
  store i32 %sub28, i32* %i, align 4
  %digits29 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data30 = extractvalue { i8*, i64 } %digits29, 0
  %i31 = load i32, i32* %i, align 4
  %arr.at32 = getelementptr i8, i8* %arr.data30, i32 %i31
  %idx = load i8, i8* %arr.at32, align 1
  call void @String.push(%String* %s, i8 %idx)
  %eflag35 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag35, label %throw.unwind33, label %throw.cont34

while.end24:                                      ; preds = %while.cond22
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data36 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data36)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s37 = load %String, %String* %s, align 8
  ret %String %s37

throw.unwind33:                                   ; preds = %while.body23
  call void @String.drop(%String* %s)
  ret %String zeroinitializer

throw.cont34:                                     ; preds = %while.body23
  br label %while.cond22
}

define void @String.push(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %c = alloca i8, align 1
  store i8 %1, i8* %c, align 1
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref1, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %add = add i32 %count2, 2
  call void @String.grow(%String* %ref, i32 %add)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %ref5 = load %String*, %String** %self, align 8
  %count6 = getelementptr %String, %String* %ref5, i32 0, i32 1
  %count7 = load i32, i32* %count6, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %count7
  %c8 = load i8, i8* %c, align 1
  store i8 %c8, i8* %arr.at, align 1
  %ref9 = load %String*, %String** %self, align 8
  %count10 = getelementptr %String, %String* %ref9, i32 0, i32 1
  %cur = load i32, i32* %count10, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %count10, align 4
  %ref12 = load %String*, %String** %self, align 8
  %value13 = getelementptr %String, %String* %ref12, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value13, align 8
  %arr.data15 = extractvalue { i8*, i64 } %value14, 0
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %arr.at19 = getelementptr i8, i8* %arr.data15, i32 %count18
  store i8 0, i8* %arr.at19, align 1
  ret void
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare void @free(i8*)

define i32 @String.cap(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref2 = load %String*, %String** %self, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %len = extractvalue { i8*, i64 } %value4, 1
  %cast = trunc i64 %len to i32
  ret i32 %cast

then:                                             ; preds = %entry
  ret i32 0
}

define void @String.grow(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %want = alloca i32, align 4
  store i32 %1, i32* %want, align 4
  %room = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %call = call i32 @String.cap(%String* %ref)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %throw.cont
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %throw.cont
  ret void

endif3:                                           ; preds = %then4, %endif
  br label %while.cond

then4:                                            ; preds = %endif
  store i32 16, i32* %room, align 4
  br label %endif3

while.cond:                                       ; preds = %while.body, %endif3
  %room6 = load i32, i32* %room, align 4
  %want7 = load i32, i32* %want, align 4
  %lt = icmp slt i32 %room6, %want7
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %cur = load i32, i32* %room, align 4
  %mul = mul i32 %cur, 2
  store i32 %mul, i32* %room, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %bigger = alloca { i8*, i64 }, align 8
  %room8 = load i32, i32* %room, align 4
  %n = sext i32 %room8 to i64
  %heap = call i8* @calloc(i64 %n, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %n, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %bigger, align 8
  %ref11 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref11, i32 0, i32 1
  %count12 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %count12, 0
  br i1 %gt, label %then10, label %endif9

endif9:                                           ; preds = %throw.cont22, %while.end
  %bigger24 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data25 = extractvalue { i8*, i64 } %bigger24, 0
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %count28 = load i32, i32* %count27, align 4
  %arr.at = getelementptr i8, i8* %arr.data25, i32 %count28
  store i8 0, i8* %arr.at, align 1
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value30, align 8
  %arr.data31 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data31)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value30, align 8
  %ref32 = load %String*, %String** %self, align 8
  %value33 = getelementptr %String, %String* %ref32, i32 0, i32 0
  %bigger34 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger34, { i8*, i64 }* %value33, align 8
  ret void

then10:                                           ; preds = %while.end
  %bigger13 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data = extractvalue { i8*, i64 } %bigger13, 0
  %ref14 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref14, i32 0, i32 0
  %value15 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value15, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str, i32 0, i32 0), i64 0 }, { i8*, i64 } %value15
  %arr.data16 = extractvalue { i8*, i64 } %fallback, 0
  %ref17 = load %String*, %String** %self, align 8
  %count18 = getelementptr %String, %String* %ref17, i32 0, i32 1
  %count19 = load i32, i32* %count18, align 4
  %call20 = call i8* @memcpy(i8* %arr.data, i8* %arr.data16, i32 %count19)
  %eflag23 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag23, label %throw.unwind21, label %throw.cont22

throw.unwind21:                                   ; preds = %then10
  ret void

throw.cont22:                                     ; preds = %then10
  br label %endif9
}

declare i8* @memcpy(i8*, i8*, i32)

declare i64 @strlen(i8*)

define void @String.join(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

then:                                             ; preds = %entry
  ret void

throw.unwind:                                     ; preds = %endif
  ret void

throw.cont:                                       ; preds = %endif
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

endif3:                                           ; preds = %throw.cont
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %eflag12 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag12, label %throw.unwind10, label %throw.cont11

then4:                                            ; preds = %throw.cont
  ret void

throw.unwind10:                                   ; preds = %endif3
  ret void

throw.cont11:                                     ; preds = %endif3
  %ref13 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref13, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data15 = extractvalue { i8*, i64 } %value14, 0
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %ref19 = load %String*, %String** %self, align 8
  %count20 = getelementptr %String, %String* %ref19, i32 0, i32 1
  %count21 = load i32, i32* %count20, align 4
  %n22 = load i32, i32* %n, align 4
  %add23 = add i32 %count21, %n22
  %start = sext i32 %count18 to i64
  %end = sext i32 %add23 to i64
  %slice.data = getelementptr i8, i8* %arr.data15, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data24 = extractvalue { i8*, i64 } %arr.len, 0
  %str25 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data26 = extractvalue { i8*, i64 } %str25, 0
  %n27 = load i32, i32* %n, align 4
  %call28 = call i8* @memcpy(i8* %arr.data24, i8* %arr.data26, i32 %n27)
  %eflag31 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag31, label %throw.unwind29, label %throw.cont30

throw.unwind29:                                   ; preds = %throw.cont11
  ret void

throw.cont30:                                     ; preds = %throw.cont11
  %ref32 = load %String*, %String** %self, align 8
  %count33 = getelementptr %String, %String* %ref32, i32 0, i32 1
  %n34 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count33, align 4
  %add35 = add i32 %cur, %n34
  store i32 %add35, i32* %count33, align 4
  %ref36 = load %String*, %String** %self, align 8
  %value37 = getelementptr %String, %String* %ref36, i32 0, i32 0
  %value38 = load { i8*, i64 }, { i8*, i64 }* %value37, align 8
  %arr.data39 = extractvalue { i8*, i64 } %value38, 0
  %ref40 = load %String*, %String** %self, align 8
  %count41 = getelementptr %String, %String* %ref40, i32 0, i32 1
  %count42 = load i32, i32* %count41, align 4
  %arr.at = getelementptr i8, i8* %arr.data39, i32 %count42
  store i8 0, i8* %arr.at, align 1
  ret void
}

define i8 @String.pop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %ref2 = load %String*, %String** %self, align 8
  %count3 = getelementptr %String, %String* %ref2, i32 0, i32 1
  %cur = load i32, i32* %count3, align 4
  %sub = sub i32 %cur, 1
  store i32 %sub, i32* %count3, align 4
  %c = alloca i8, align 1
  %ref4 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref4, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %ref6 = load %String*, %String** %self, align 8
  %count7 = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count8 = load i32, i32* %count7, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %count8
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %arr.at16 = getelementptr i8, i8* %arr.data12, i32 %count15
  store i8 0, i8* %arr.at16, align 1
  %c17 = load i8, i8* %c, align 1
  ret i8 %c17

then:                                             ; preds = %entry
  ret i8 0
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  store i32 0, i32* %count, align 4
  %ref1 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value2, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %then, %entry
  ret void

then:                                             ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value4 = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value4, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %arr.at = getelementptr i8, i8* %arr.data, i32 0
  store i8 0, i8* %arr.at, align 1
  br label %endif
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  ret i32 %count1
}

define i1 @String.empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count1, 0
  ret i1 %eq
}

define { i8*, i64 } @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.4, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  ret { i8*, i64 } %fallback
}

define i8 @String.at(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %i = alloca i32, align 4
  store i32 %1, i32* %i, align 4
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 0
  %i2 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count3 = load i32, i32* %count, align 4
  %ge = icmp sge i32 %i2, %count3
  %or = or i1 %lt, %ge
  br i1 %or, label %then, label %endif

endif:                                            ; preds = %entry
  %ref4 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref4, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %i6 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i6
  %idx = load i8, i8* %arr.at, align 1
  ret i8 %idx

then:                                             ; preds = %entry
  ret i8 0
}

define i32 @String.find(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %needle = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %needle, align 8
  %needle1 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %opt.ptr = extractvalue { i8*, i64 } %needle1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %needle2 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %arr.data = extractvalue { i8*, i64 } %needle2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

then:                                             ; preds = %entry
  ret i32 -1

throw.unwind:                                     ; preds = %endif
  ret i32 0

throw.cont:                                       ; preds = %endif
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

endif3:                                           ; preds = %throw.cont
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %throw.cont
  ret i32 0

while.cond:                                       ; preds = %endif9, %endif3
  %i6 = load i32, i32* %i, align 4
  %n7 = load i32, i32* %n, align 4
  %add = add i32 %i6, %n7
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count8 = load i32, i32* %count, align 4
  %le = icmp sle i32 %add, %count8
  br i1 %le, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref11 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref11, i32 0, i32 0
  %value12 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data13 = extractvalue { i8*, i64 } %value12, 0
  %i14 = load i32, i32* %i, align 4
  %i15 = load i32, i32* %i, align 4
  %n16 = load i32, i32* %n, align 4
  %add17 = add i32 %i15, %n16
  %start = sext i32 %i14 to i64
  %end = sext i32 %add17 to i64
  %slice.data = getelementptr i8, i8* %arr.data13, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data18 = extractvalue { i8*, i64 } %arr.len, 0
  %needle19 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %arr.data20 = extractvalue { i8*, i64 } %needle19, 0
  %n21 = load i32, i32* %n, align 4
  %call22 = call i32 @strncmp(i8* %arr.data18, i8* %arr.data20, i32 %n21)
  %eflag25 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag25, label %throw.unwind23, label %throw.cont24

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %throw.cont24
  %cur = load i32, i32* %i, align 4
  %add28 = add i32 %cur, 1
  store i32 %add28, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %throw.cont24
  %i27 = load i32, i32* %i, align 4
  ret i32 %i27

throw.unwind23:                                   ; preds = %while.body
  ret i32 0

throw.cont24:                                     ; preds = %while.body
  %eq26 = icmp eq i32 %call22, 0
  br i1 %eq26, label %then10, label %endif9
}

declare i32 @strncmp(i8*, i8*, i32)

define i1 @String.contains(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %needle = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %needle, align 8
  %ref = load %String*, %String** %self, align 8
  %needle1 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %call = call i32 @String.find(%String* %ref, { i8*, i64 } %needle1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i1 false

throw.cont:                                       ; preds = %entry
  %ne = icmp ne i32 %call, -1
  ret i1 %ne
}

define i1 @String.starts_with(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %prefix = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %prefix, align 8
  %prefix1 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %opt.ptr = extractvalue { i8*, i64 } %prefix1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %prefix2 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %arr.data = extractvalue { i8*, i64 } %prefix2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

then:                                             ; preds = %entry
  ret i1 true

throw.unwind:                                     ; preds = %endif
  ret i1 false

throw.cont:                                       ; preds = %endif
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

endif3:                                           ; preds = %throw.cont
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %n10 = load i32, i32* %n, align 4
  %end = sext i32 %n10 to i64
  %slice.data = getelementptr i8, i8* %arr.data9, i64 0
  %slice.len = sub i64 %end, 0
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data11 = extractvalue { i8*, i64 } %arr.len, 0
  %prefix12 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %arr.data13 = extractvalue { i8*, i64 } %prefix12, 0
  %n14 = load i32, i32* %n, align 4
  %call15 = call i32 @strncmp(i8* %arr.data11, i8* %arr.data13, i32 %n14)
  %eflag18 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag18, label %throw.unwind16, label %throw.cont17

then4:                                            ; preds = %throw.cont
  ret i1 false

throw.unwind16:                                   ; preds = %endif3
  ret i1 false

throw.cont17:                                     ; preds = %endif3
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq
}

define i1 @String.ends_with(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %suffix = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %suffix, align 8
  %suffix1 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %opt.ptr = extractvalue { i8*, i64 } %suffix1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %suffix2 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %arr.data = extractvalue { i8*, i64 } %suffix2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

then:                                             ; preds = %entry
  ret i1 true

throw.unwind:                                     ; preds = %endif
  ret i1 false

throw.cont:                                       ; preds = %endif
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

endif3:                                           ; preds = %throw.cont
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %ref10 = load %String*, %String** %self, align 8
  %count11 = getelementptr %String, %String* %ref10, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %n13 = load i32, i32* %n, align 4
  %sub = sub i32 %count12, %n13
  %ref14 = load %String*, %String** %self, align 8
  %count15 = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count16 = load i32, i32* %count15, align 4
  %start = sext i32 %sub to i64
  %end = sext i32 %count16 to i64
  %slice.data = getelementptr i8, i8* %arr.data9, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data17 = extractvalue { i8*, i64 } %arr.len, 0
  %suffix18 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %arr.data19 = extractvalue { i8*, i64 } %suffix18, 0
  %n20 = load i32, i32* %n, align 4
  %call21 = call i32 @strncmp(i8* %arr.data17, i8* %arr.data19, i32 %n20)
  %eflag24 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag24, label %throw.unwind22, label %throw.cont23

then4:                                            ; preds = %throw.cont
  ret i1 false

throw.unwind22:                                   ; preds = %endif3
  ret i1 false

throw.cont23:                                     ; preds = %endif3
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq
}

define %String @String.substr(%String* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %start = alloca i32, align 4
  store i32 %1, i32* %start, align 4
  %length = alloca i32, align 4
  store i32 %2, i32* %length, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %res, align 8
  %start1 = load i32, i32* %start, align 4
  %lt = icmp slt i32 %start1, 0
  %start2 = load i32, i32* %start, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count3 = load i32, i32* %count, align 4
  %ge = icmp sge i32 %start2, %count3
  %or = or i1 %lt, %ge
  br i1 %or, label %then, label %endif

endif:                                            ; preds = %throw.cont
  %stop = alloca i32, align 4
  %start5 = load i32, i32* %start, align 4
  %length6 = load i32, i32* %length, align 4
  %add = add i32 %start5, %length6
  store i32 %add, i32* %stop, align 4
  %stop9 = load i32, i32* %stop, align 4
  %ref10 = load %String*, %String** %self, align 8
  %count11 = getelementptr %String, %String* %ref10, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %gt = icmp sgt i32 %stop9, %count12
  br i1 %gt, label %then8, label %endif7

then:                                             ; preds = %throw.cont
  %res4 = load %String, %String* %res, align 8
  ret %String %res4

endif7:                                           ; preds = %then8, %endif
  %i = alloca i32, align 4
  %start16 = load i32, i32* %start, align 4
  store i32 %start16, i32* %i, align 4
  br label %while.cond

then8:                                            ; preds = %endif
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  store i32 %count15, i32* %stop, align 4
  br label %endif7

while.cond:                                       ; preds = %throw.cont24, %endif7
  %i17 = load i32, i32* %i, align 4
  %stop18 = load i32, i32* %stop, align 4
  %lt19 = icmp slt i32 %i17, %stop18
  br i1 %lt19, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref20 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref20, i32 0, i32 0
  %value21 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value21, 0
  %i22 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i22
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %eflag25 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag25, label %throw.unwind23, label %throw.cont24

while.end:                                        ; preds = %while.cond
  %res27 = load %String, %String* %res, align 8
  ret %String %res27

throw.unwind23:                                   ; preds = %while.body
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont24:                                     ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add26 = add i32 %cur, 1
  store i32 %add26, i32* %i, align 4
  br label %while.cond
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %throw.cont
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c6, 97
  %c7 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c7, 122
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %res17 = load %String, %String* %res, align 8
  ret %String %res17

endif:                                            ; preds = %throw.cont15, %throw.cont11
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %sub = sub i32 %cast, 32
  %cast9 = trunc i32 %sub to i8
  call void @String.push(%String* %res, i8 %cast9)
  %eflag12 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag12, label %throw.unwind10, label %throw.cont11

next:                                             ; preds = %while.body
  %c13 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c13)
  %eflag16 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag16, label %throw.unwind14, label %throw.cont15

throw.unwind10:                                   ; preds = %then
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont11:                                     ; preds = %then
  br label %endif

throw.unwind14:                                   ; preds = %next
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont15:                                     ; preds = %next
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %throw.cont
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c6, 65
  %c7 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c7, 90
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %res18 = load %String, %String* %res, align 8
  ret %String %res18

endif:                                            ; preds = %throw.cont15, %throw.cont11
  %cur = load i32, i32* %i, align 4
  %add17 = add i32 %cur, 1
  store i32 %add17, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  %eflag12 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag12, label %throw.unwind10, label %throw.cont11

next:                                             ; preds = %while.body
  %c13 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c13)
  %eflag16 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag16, label %throw.unwind14, label %throw.cont15

throw.unwind10:                                   ; preds = %then
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont11:                                     ; preds = %then
  br label %endif

throw.unwind14:                                   ; preds = %next
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont15:                                     ; preds = %next
  br label %endif
}

define %String @String.trim(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %start = alloca i32, align 4
  store i32 0, i32* %start, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %start1 = load i32, i32* %start, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %start1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %start5 = load i32, i32* %start, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %start5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %eq = icmp eq i8 %c6, 32
  %c7 = load i8, i8* %c, align 1
  %eq8 = icmp eq i8 %c7, 9
  %or = or i1 %eq, %eq8
  %c9 = load i8, i8* %c, align 1
  %eq10 = icmp eq i8 %c9, 10
  %or11 = or i1 %or, %eq10
  %c12 = load i8, i8* %c, align 1
  %eq13 = icmp eq i8 %c12, 13
  %or14 = or i1 %or11, %eq13
  br i1 %or14, label %then, label %next

while.end:                                        ; preds = %next, %while.cond
  %stop = alloca i32, align 4
  %ref15 = load %String*, %String** %self, align 8
  %count16 = getelementptr %String, %String* %ref15, i32 0, i32 1
  %count17 = load i32, i32* %count16, align 4
  store i32 %count17, i32* %stop, align 4
  br label %while.cond18

endif:                                            ; preds = %then
  br label %while.cond

then:                                             ; preds = %while.body
  %cur = load i32, i32* %start, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %start, align 4
  br label %endif

next:                                             ; preds = %while.body
  br label %while.end

while.cond18:                                     ; preds = %endif31, %while.end
  %stop21 = load i32, i32* %stop, align 4
  %start22 = load i32, i32* %start, align 4
  %gt = icmp sgt i32 %stop21, %start22
  br i1 %gt, label %while.body19, label %while.end20

while.body19:                                     ; preds = %while.cond18
  %c23 = alloca i8, align 1
  %ref24 = load %String*, %String** %self, align 8
  %value25 = getelementptr %String, %String* %ref24, i32 0, i32 0
  %value26 = load { i8*, i64 }, { i8*, i64 }* %value25, align 8
  %arr.data27 = extractvalue { i8*, i64 } %value26, 0
  %stop28 = load i32, i32* %stop, align 4
  %sub = sub i32 %stop28, 1
  %arr.at29 = getelementptr i8, i8* %arr.data27, i32 %sub
  %idx30 = load i8, i8* %arr.at29, align 1
  store i8 %idx30, i8* %c23, align 1
  %c34 = load i8, i8* %c23, align 1
  %eq35 = icmp eq i8 %c34, 32
  %c36 = load i8, i8* %c23, align 1
  %eq37 = icmp eq i8 %c36, 9
  %or38 = or i1 %eq35, %eq37
  %c39 = load i8, i8* %c23, align 1
  %eq40 = icmp eq i8 %c39, 10
  %or41 = or i1 %or38, %eq40
  %c42 = load i8, i8* %c23, align 1
  %eq43 = icmp eq i8 %c42, 13
  %or44 = or i1 %or41, %eq43
  br i1 %or44, label %then32, label %next33

while.end20:                                      ; preds = %next33, %while.cond18
  %ref47 = load %String*, %String** %self, align 8
  %start48 = load i32, i32* %start, align 4
  %stop49 = load i32, i32* %stop, align 4
  %start50 = load i32, i32* %start, align 4
  %sub51 = sub i32 %stop49, %start50
  %call = call %String @String.substr(%String* %ref47, i32 %start48, i32 %sub51)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20

throw.unwind:                                     ; preds = %while.end20
  ret %String zeroinitializer

throw.cont:                                       ; preds = %while.end20
  ret %String %call
}

define %String @String.replace(%String* %0, { i8*, i64 } %1, { i8*, i64 } %2) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %old = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %old, align 8
  %fresh = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %2, { i8*, i64 }* %fresh, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %throw.cont
  %n = alloca i32, align 4
  %old7 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old7, 0
  %call8 = call i64 @strlen(i8* %arr.data)
  %eflag11 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag11, label %throw.unwind9, label %throw.cont10

then:                                             ; preds = %throw.cont
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %eflag5 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag5, label %throw.unwind3, label %throw.cont4

throw.unwind3:                                    ; preds = %then
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont4:                                      ; preds = %then
  %res6 = load %String, %String* %res, align 8
  ret %String %res6

throw.unwind9:                                    ; preds = %endif
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont10:                                     ; preds = %endif
  %cast = trunc i64 %call8 to i32
  store i32 %cast, i32* %n, align 4
  %n14 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n14, 0
  br i1 %eq, label %then13, label %endif12

endif12:                                          ; preds = %throw.cont10
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then13:                                           ; preds = %throw.cont10
  %ref15 = load %String*, %String** %self, align 8
  %value16 = getelementptr %String, %String* %ref15, i32 0, i32 0
  %value17 = load { i8*, i64 }, { i8*, i64 }* %value16, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value17)
  %eflag20 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag20, label %throw.unwind18, label %throw.cont19

throw.unwind18:                                   ; preds = %then13
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont19:                                     ; preds = %then13
  %res21 = load %String, %String* %res, align 8
  ret %String %res21

while.cond:                                       ; preds = %endif49, %endif12
  %i22 = load i32, i32* %i, align 4
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count24 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i22, %count24
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i27 = load i32, i32* %i, align 4
  %n28 = load i32, i32* %n, align 4
  %add = add i32 %i27, %n28
  %ref29 = load %String*, %String** %self, align 8
  %count30 = getelementptr %String, %String* %ref29, i32 0, i32 1
  %count31 = load i32, i32* %count30, align 4
  %le = icmp sle i32 %add, %count31
  br i1 %le, label %then26, label %endif25

while.end:                                        ; preds = %while.cond
  %res68 = load %String, %String* %res, align 8
  ret %String %res68

endif25:                                          ; preds = %throw.cont46, %while.body
  %hit51 = load i1, i1* %hit, align 1
  br i1 %hit51, label %then50, label %next

then26:                                           ; preds = %while.body
  %ref32 = load %String*, %String** %self, align 8
  %value33 = getelementptr %String, %String* %ref32, i32 0, i32 0
  %value34 = load { i8*, i64 }, { i8*, i64 }* %value33, align 8
  %arr.data35 = extractvalue { i8*, i64 } %value34, 0
  %i36 = load i32, i32* %i, align 4
  %i37 = load i32, i32* %i, align 4
  %n38 = load i32, i32* %n, align 4
  %add39 = add i32 %i37, %n38
  %start = sext i32 %i36 to i64
  %end = sext i32 %add39 to i64
  %slice.data = getelementptr i8, i8* %arr.data35, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data40 = extractvalue { i8*, i64 } %arr.len, 0
  %old41 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data42 = extractvalue { i8*, i64 } %old41, 0
  %n43 = load i32, i32* %n, align 4
  %call44 = call i32 @strncmp(i8* %arr.data40, i8* %arr.data42, i32 %n43)
  %eflag47 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag47, label %throw.unwind45, label %throw.cont46

throw.unwind45:                                   ; preds = %then26
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont46:                                     ; preds = %then26
  %eq48 = icmp eq i32 %call44, 0
  store i1 %eq48, i1* %hit, align 1
  br label %endif25

endif49:                                          ; preds = %throw.cont64, %throw.cont54
  br label %while.cond

then50:                                           ; preds = %endif25
  %fresh52 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh52)
  %eflag55 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag55, label %throw.unwind53, label %throw.cont54

next:                                             ; preds = %endif25
  %ref58 = load %String*, %String** %self, align 8
  %value59 = getelementptr %String, %String* %ref58, i32 0, i32 0
  %value60 = load { i8*, i64 }, { i8*, i64 }* %value59, align 8
  %arr.data61 = extractvalue { i8*, i64 } %value60, 0
  %i62 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data61, i32 %i62
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %eflag65 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag65, label %throw.unwind63, label %throw.cont64

throw.unwind53:                                   ; preds = %then50
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont54:                                     ; preds = %then50
  %n56 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add57 = add i32 %cur, %n56
  store i32 %add57, i32* %i, align 4
  br label %endif49

throw.unwind63:                                   ; preds = %next
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont64:                                     ; preds = %next
  %cur66 = load i32, i32* %i, align 4
  %add67 = add i32 %cur66, 1
  store i32 %add67, i32* %i, align 4
  br label %endif49
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %throw.cont5, %throw.cont
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %eflag6 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag6, label %throw.unwind4, label %throw.cont5

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7

throw.unwind4:                                    ; preds = %while.body
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont5:                                      ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %throw.cont7, %throw.cont
  %i2 = load i32, i32* %i, align 4
  %ge = icmp sge i32 %i2, 0
  br i1 %ge, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %eflag8 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag8, label %throw.unwind6, label %throw.cont7

while.end:                                        ; preds = %while.cond
  %res10 = load %String, %String* %res, align 8
  ret %String %res10

throw.unwind6:                                    ; preds = %while.body
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont7:                                      ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %sub9 = sub i32 %cur, 1
  store i32 %sub9, i32* %i, align 4
  br label %while.cond
}

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %other = alloca %String*, align 8
  store %String* %1, %String** %other, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.5, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %ref2 = load %String*, %String** %other, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull6 = icmp eq i8* %opt.ptr5, null
  %fallback7 = select i1 %isnull6, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.6, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data8 = extractvalue { i8*, i64 } %fallback7, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data8)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i32 0

throw.cont:                                       ; preds = %entry
  ret i32 %call
}

declare i32 @strcmp(i8*, i8*)

define i32 @String.to_int(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.7, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %call = call i32 @atoi(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i32 0

throw.cont:                                       ; preds = %entry
  ret i32 %call
}

declare i32 @atoi(i8*)

define void @"String.=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %ref, { i8*, i64 } %str1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  ret void
}

define void @"String.=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %ref, { i8*, i64 } %value2)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  ret void
}

define %String @"String.+.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %res = alloca %String, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %call = call %String @String.from({ i8*, i64 } %value1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %eflag7 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag7, label %throw.unwind5, label %throw.cont6

throw.unwind5:                                    ; preds = %throw.cont
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont6:                                      ; preds = %throw.cont
  %res8 = load %String, %String* %res, align 8
  ret %String %res8
}

define %String @"String.+.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %res = alloca %String, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %call = call %String @String.from({ i8*, i64 } %value1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %eflag5 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag5, label %throw.unwind3, label %throw.cont4

throw.unwind3:                                    ; preds = %throw.cont
  call void @String.drop(%String* %res)
  ret %String zeroinitializer

throw.cont4:                                      ; preds = %throw.cont
  %res6 = load %String, %String* %res, align 8
  ret %String %res6
}

define void @"String.+=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %ref, { i8*, i64 } %value2)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  ret void
}

define void @"String.+=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %ref, { i8*, i64 } %str1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  ret void
}

define i1 @"String.==.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i1 false

throw.cont:                                       ; preds = %entry
  %eq = icmp eq i32 %call, 0
  ret i1 %eq
}

define i1 @"String.!=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i1 false

throw.cont:                                       ; preds = %entry
  %ne = icmp ne i32 %call, 0
  ret i1 %ne
}

define i1 @"String.==.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull = icmp eq i8* %opt.ptr5, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.8, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str6 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data7 = extractvalue { i8*, i64 } %str6, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data7)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq

throw.unwind:                                     ; preds = %endif
  ret i1 false

throw.cont:                                       ; preds = %endif
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8
}

define i1 @"String.!=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull = icmp eq i8* %opt.ptr5, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.9, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str6 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data7 = extractvalue { i8*, i64 } %str6, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data7)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne

throw.unwind:                                     ; preds = %endif
  ret i1 false

throw.cont:                                       ; preds = %endif
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8
}

define i1 @"String.<.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i1 false

throw.cont:                                       ; preds = %entry
  %lt = icmp slt i32 %call, 0
  ret i1 %lt
}

define i1 @"String.>.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i1 false

throw.cont:                                       ; preds = %entry
  %gt = icmp sgt i32 %call, 0
  ret i1 %gt
}

define i1 @"String.<.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.10, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr3 = extractvalue { i8*, i64 } %str2, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.11, i32 0, i32 0), i64 0 }, { i8*, i64 } %str2
  %arr.data6 = extractvalue { i8*, i64 } %fallback5, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data6)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i1 false

throw.cont:                                       ; preds = %entry
  %lt = icmp slt i32 %call, 0
  ret i1 %lt
}

define i1 @"String.>.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.12, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr3 = extractvalue { i8*, i64 } %str2, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.13, i32 0, i32 0), i64 0 }, { i8*, i64 } %str2
  %arr.data6 = extractvalue { i8*, i64 } %fallback5, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data6)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i1 false

throw.cont:                                       ; preds = %entry
  %gt = icmp sgt i32 %call, 0
  ret i1 %gt
}

define { i8*, i64 } @String.output(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.14, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  ret { i8*, i64 } %fallback
}

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define %Token* @Token.number(i32 %0) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %t = alloca %Token*, align 8
  %heap = call i8* @calloc(i64 1, i64 8)
  %arr = bitcast i8* %heap to %Token*
  store %Token* %arr, %Token** %t, align 8
  %ref = load %Token*, %Token** %t, align 8
  %type = getelementptr %Token, %Token* %ref, i32 0, i32 1
  store i32 1, i32* %type, align 4
  %ref1 = load %Token*, %Token** %t, align 8
  %value = getelementptr %Token, %Token* %ref1, i32 0, i32 0
  %v2 = load i32, i32* %v, align 4
  store i32 %v2, i32* %value, align 4
  %ref3 = load %Token*, %Token** %t, align 8
  ret %Token* %ref3
}

define %Token* @Token.op(i32 %0) {
entry:
  %k = alloca i32, align 4
  store i32 %0, i32* %k, align 4
  %t = alloca %Token*, align 8
  %heap = call i8* @calloc(i64 1, i64 8)
  %arr = bitcast i8* %heap to %Token*
  store %Token* %arr, %Token** %t, align 8
  %ref = load %Token*, %Token** %t, align 8
  %type = getelementptr %Token, %Token* %ref, i32 0, i32 1
  %k1 = load i32, i32* %k, align 4
  store i32 %k1, i32* %type, align 4
  %ref2 = load %Token*, %Token** %t, align 8
  ret %Token* %ref2
}

define %Node* @Node.create(%Token* %0) {
entry:
  %token = alloca %Token*, align 8
  store %Token* %0, %Token** %token, align 8
  %n = alloca %Node*, align 8
  %heap = call i8* @calloc(i64 1, i64 24)
  %arr = bitcast i8* %heap to %Node*
  store %Node* %arr, %Node** %n, align 8
  %ref = load %Token*, %Token** %token, align 8
  %ref1 = load %Node*, %Node** %n, align 8
  %token2 = getelementptr %Node, %Node* %ref1, i32 0, i32 0
  store %Token* %ref, %Token** %token2, align 8
  %ref3 = load %Node*, %Node** %n, align 8
  ret %Node* %ref3
}

define void @Node.drop(%Node* %0) {
entry:
  %self = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  %ref = load %Node*, %Node** %self, align 8
  %token = getelementptr %Node, %Node* %ref, i32 0, i32 0
  %ref1 = load %Token*, %Token** %token, align 8
  %ref2 = load %Node*, %Node** %self, align 8
  %token3 = getelementptr %Node, %Node* %ref2, i32 0, i32 0
  %isnull = icmp eq %Token* %ref1, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  %free.ptr = bitcast %Token* %ref1 to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %Token* null, %Token** %token3, align 8
  %ref4 = load %Node*, %Node** %self, align 8
  %left = getelementptr %Node, %Node* %ref4, i32 0, i32 1
  %ref5 = load %Node*, %Node** %left, align 8
  %ref6 = load %Node*, %Node** %self, align 8
  %left7 = getelementptr %Node, %Node* %ref6, i32 0, i32 1
  %isnull10 = icmp eq %Node* %ref5, null
  br i1 %isnull10, label %clean.done9, label %clean.live8

clean.live8:                                      ; preds = %clean.done
  call void @Node.drop(%Node* %ref5)
  %free.ptr11 = bitcast %Node* %ref5 to i8*
  call void @free(i8* %free.ptr11)
  br label %clean.done9

clean.done9:                                      ; preds = %clean.live8, %clean.done
  store %Node* null, %Node** %left7, align 8
  %ref12 = load %Node*, %Node** %self, align 8
  %right = getelementptr %Node, %Node* %ref12, i32 0, i32 2
  %ref13 = load %Node*, %Node** %right, align 8
  %ref14 = load %Node*, %Node** %self, align 8
  %right15 = getelementptr %Node, %Node* %ref14, i32 0, i32 2
  %isnull18 = icmp eq %Node* %ref13, null
  br i1 %isnull18, label %clean.done17, label %clean.live16

clean.live16:                                     ; preds = %clean.done9
  call void @Node.drop(%Node* %ref13)
  %free.ptr19 = bitcast %Node* %ref13 to i8*
  call void @free(i8* %free.ptr19)
  br label %clean.done17

clean.done17:                                     ; preds = %clean.live16, %clean.done9
  store %Node* null, %Node** %right15, align 8
  ret void
}

define i32 @Lexer.run(%String* %0, { %Token*, i64 } %1) {
entry:
  %src = alloca %String*, align 8
  store %String* %0, %String** %src, align 8
  %toks = alloca { %Token*, i64 }, align 8
  store { %Token*, i64 } %1, { %Token*, i64 }* %toks, align 8
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %while.end19, %then, %entry
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %src, align 8
  %call = call i32 @String.len(%String* %ref)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

while.body:                                       ; preds = %throw.cont
  %c = alloca i8, align 1
  %ref2 = load %String*, %String** %src, align 8
  %i3 = load i32, i32* %i, align 4
  %call4 = call i8 @String.at(%String* %ref2, i32 %i3)
  %eflag7 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag7, label %throw.unwind5, label %throw.cont6

while.end:                                        ; preds = %throw.cont
  %n87 = load i32, i32* %n, align 4
  ret i32 %n87

throw.unwind:                                     ; preds = %while.cond
  ret i32 0

throw.cont:                                       ; preds = %while.cond
  %lt = icmp slt i32 %i1, %call
  br i1 %lt, label %while.body, label %while.end

throw.unwind5:                                    ; preds = %while.body
  ret i32 0

throw.cont6:                                      ; preds = %while.body
  store i8 %call4, i8* %c, align 1
  %c8 = load i8, i8* %c, align 1
  %eq = icmp eq i8 %c8, 32
  br i1 %eq, label %then, label %next

endif:                                            ; preds = %match.end
  br label %while.cond

then:                                             ; preds = %throw.cont6
  %i9 = load i32, i32* %i, align 4
  %add = add i32 %i9, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

next:                                             ; preds = %throw.cont6
  %c12 = load i8, i8* %c, align 1
  %call13 = call i1 @is_digit(i8 %c12)
  %eflag16 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag16, label %throw.unwind14, label %throw.cont15

then10:                                           ; preds = %throw.cont15
  %v = alloca i32, align 4
  store i32 0, i32* %v, align 4
  br label %while.cond17

next11:                                           ; preds = %throw.cont15
  %k = alloca i32, align 4
  store i32 1, i32* %k, align 4
  %c56 = load i8, i8* %c, align 1
  %eq57 = icmp eq i8 %c56, 43
  br i1 %eq57, label %case.body, label %case.next

throw.unwind14:                                   ; preds = %next
  ret i32 0

throw.cont15:                                     ; preds = %next
  br i1 %call13, label %then10, label %next11

while.cond17:                                     ; preds = %throw.cont42, %then10
  %i20 = load i32, i32* %i, align 4
  %ref21 = load %String*, %String** %src, align 8
  %call22 = call i32 @String.len(%String* %ref21)
  %eflag25 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag25, label %throw.unwind23, label %throw.cont24

while.body18:                                     ; preds = %throw.cont35
  %v37 = load i32, i32* %v, align 4
  %mul = mul i32 %v37, 10
  %ref38 = load %String*, %String** %src, align 8
  %i39 = load i32, i32* %i, align 4
  %call40 = call i8 @String.at(%String* %ref38, i32 %i39)
  %eflag43 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag43, label %throw.unwind41, label %throw.cont42

while.end19:                                      ; preds = %throw.cont35
  %toks47 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data = extractvalue { %Token*, i64 } %toks47, 0
  %n48 = load i32, i32* %n, align 4
  %arr.at = getelementptr %Token, %Token* %arr.data, i32 %n48
  %type = getelementptr %Token, %Token* %arr.at, i32 0, i32 1
  store i32 1, i32* %type, align 4
  %toks49 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data50 = extractvalue { %Token*, i64 } %toks49, 0
  %n51 = load i32, i32* %n, align 4
  %arr.at52 = getelementptr %Token, %Token* %arr.data50, i32 %n51
  %value = getelementptr %Token, %Token* %arr.at52, i32 0, i32 0
  %v53 = load i32, i32* %v, align 4
  store i32 %v53, i32* %value, align 4
  %n54 = load i32, i32* %n, align 4
  %add55 = add i32 %n54, 1
  store i32 %add55, i32* %n, align 4
  br label %while.cond

throw.unwind23:                                   ; preds = %while.cond17
  ret i32 0

throw.cont24:                                     ; preds = %while.cond17
  %lt26 = icmp slt i32 %i20, %call22
  %ref27 = load %String*, %String** %src, align 8
  %i28 = load i32, i32* %i, align 4
  %call29 = call i8 @String.at(%String* %ref27, i32 %i28)
  %eflag32 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag32, label %throw.unwind30, label %throw.cont31

throw.unwind30:                                   ; preds = %throw.cont24
  ret i32 0

throw.cont31:                                     ; preds = %throw.cont24
  %call33 = call i1 @is_digit(i8 %call29)
  %eflag36 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag36, label %throw.unwind34, label %throw.cont35

throw.unwind34:                                   ; preds = %throw.cont31
  ret i32 0

throw.cont35:                                     ; preds = %throw.cont31
  %and = and i1 %lt26, %call33
  br i1 %and, label %while.body18, label %while.end19

throw.unwind41:                                   ; preds = %while.body18
  ret i32 0

throw.cont42:                                     ; preds = %while.body18
  %cast = sext i8 %call40 to i32
  %sub = sub i32 %cast, 48
  %add44 = add i32 %mul, %sub
  store i32 %add44, i32* %v, align 4
  %i45 = load i32, i32* %i, align 4
  %add46 = add i32 %i45, 1
  store i32 %add46, i32* %i, align 4
  br label %while.cond17

match.end:                                        ; preds = %case.body70, %case.next68, %case.body67, %case.body64, %case.body61, %case.body58, %case.body
  %toks72 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data73 = extractvalue { %Token*, i64 } %toks72, 0
  %n74 = load i32, i32* %n, align 4
  %arr.at75 = getelementptr %Token, %Token* %arr.data73, i32 %n74
  %type76 = getelementptr %Token, %Token* %arr.at75, i32 0, i32 1
  %k77 = load i32, i32* %k, align 4
  store i32 %k77, i32* %type76, align 4
  %toks78 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data79 = extractvalue { %Token*, i64 } %toks78, 0
  %n80 = load i32, i32* %n, align 4
  %arr.at81 = getelementptr %Token, %Token* %arr.data79, i32 %n80
  %value82 = getelementptr %Token, %Token* %arr.at81, i32 0, i32 0
  store i32 0, i32* %value82, align 4
  %n83 = load i32, i32* %n, align 4
  %add84 = add i32 %n83, 1
  store i32 %add84, i32* %n, align 4
  %i85 = load i32, i32* %i, align 4
  %add86 = add i32 %i85, 1
  store i32 %add86, i32* %i, align 4
  br label %endif

case.body:                                        ; preds = %next11
  store i32 2, i32* %k, align 4
  br label %match.end

case.next:                                        ; preds = %next11
  %eq60 = icmp eq i8 %c56, 45
  br i1 %eq60, label %case.body58, label %case.next59

case.body58:                                      ; preds = %case.next
  store i32 3, i32* %k, align 4
  br label %match.end

case.next59:                                      ; preds = %case.next
  %eq63 = icmp eq i8 %c56, 42
  br i1 %eq63, label %case.body61, label %case.next62

case.body61:                                      ; preds = %case.next59
  store i32 4, i32* %k, align 4
  br label %match.end

case.next62:                                      ; preds = %case.next59
  %eq66 = icmp eq i8 %c56, 47
  br i1 %eq66, label %case.body64, label %case.next65

case.body64:                                      ; preds = %case.next62
  store i32 5, i32* %k, align 4
  br label %match.end

case.next65:                                      ; preds = %case.next62
  %eq69 = icmp eq i8 %c56, 40
  br i1 %eq69, label %case.body67, label %case.next68

case.body67:                                      ; preds = %case.next65
  store i32 6, i32* %k, align 4
  br label %match.end

case.next68:                                      ; preds = %case.next65
  %eq71 = icmp eq i8 %c56, 41
  br i1 %eq71, label %case.body70, label %match.end

case.body70:                                      ; preds = %case.next68
  store i32 7, i32* %k, align 4
  br label %match.end
}

define i1 @is_digit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c1, 48
  %c2 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c2, 57
  %and = and i1 %ge, %le
  ret i1 %and
}

define %Node* @Parser.factor({ %Token*, i64 } %0, i32* %1, i32 %2) {
entry:
  %toks = alloca { %Token*, i64 }, align 8
  store { %Token*, i64 } %0, { %Token*, i64 }* %toks, align 8
  %p = alloca i32*, align 8
  store i32* %1, i32** %p, align 8
  %n = alloca i32, align 4
  store i32 %2, i32* %n, align 4
  %ref = load i32*, i32** %p, align 8
  %p1 = load i32, i32* %ref, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %p1, %n2
  %toks3 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data = extractvalue { %Token*, i64 } %toks3, 0
  %ref4 = load i32*, i32** %p, align 8
  %p5 = load i32, i32* %ref4, align 4
  %arr.at = getelementptr %Token, %Token* %arr.data, i32 %p5
  %type = getelementptr %Token, %Token* %arr.at, i32 0, i32 1
  %type6 = load i32, i32* %type, align 4
  %eq = icmp eq i32 %type6, 6
  %and = and i1 %lt, %eq
  br i1 %and, label %then, label %endif

endif:                                            ; preds = %entry
  %lf = alloca %Node*, align 8
  %toks33 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data34 = extractvalue { %Token*, i64 } %toks33, 0
  %ref35 = load i32*, i32** %p, align 8
  %p36 = load i32, i32* %ref35, align 4
  %arr.at37 = getelementptr %Token, %Token* %arr.data34, i32 %p36
  %value = getelementptr %Token, %Token* %arr.at37, i32 0, i32 0
  %value38 = load i32, i32* %value, align 4
  %call39 = call %Token* @Token.number(i32 %value38)
  %eflag42 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag42, label %throw.unwind40, label %throw.cont41

then:                                             ; preds = %entry
  %ref7 = load i32*, i32** %p, align 8
  %ref8 = load i32*, i32** %p, align 8
  %p9 = load i32, i32* %ref8, align 4
  %add = add i32 %p9, 1
  store i32 %add, i32* %ref7, align 4
  %inner = alloca %Node*, align 8
  %toks10 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %ref11 = load i32*, i32** %p, align 8
  %n12 = load i32, i32* %n, align 4
  %call = call %Node* @Parser.expr({ %Token*, i64 } %toks10, i32* %ref11, i32 %n12)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %then
  ret %Node* null

throw.cont:                                       ; preds = %then
  store %Node* %call, %Node** %inner, align 8
  %ref15 = load i32*, i32** %p, align 8
  %p16 = load i32, i32* %ref15, align 4
  %n17 = load i32, i32* %n, align 4
  %lt18 = icmp slt i32 %p16, %n17
  %toks19 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data20 = extractvalue { %Token*, i64 } %toks19, 0
  %ref21 = load i32*, i32** %p, align 8
  %p22 = load i32, i32* %ref21, align 4
  %arr.at23 = getelementptr %Token, %Token* %arr.data20, i32 %p22
  %type24 = getelementptr %Token, %Token* %arr.at23, i32 0, i32 1
  %type25 = load i32, i32* %type24, align 4
  %eq26 = icmp eq i32 %type25, 7
  %and27 = and i1 %lt18, %eq26
  br i1 %and27, label %then14, label %endif13

endif13:                                          ; preds = %then14, %throw.cont
  %ref32 = load %Node*, %Node** %inner, align 8
  ret %Node* %ref32

then14:                                           ; preds = %throw.cont
  %ref28 = load i32*, i32** %p, align 8
  %ref29 = load i32*, i32** %p, align 8
  %p30 = load i32, i32* %ref29, align 4
  %add31 = add i32 %p30, 1
  store i32 %add31, i32* %ref28, align 4
  br label %endif13

throw.unwind40:                                   ; preds = %endif
  ret %Node* null

throw.cont41:                                     ; preds = %endif
  %call43 = call %Node* @Node.create(%Token* %call39)
  %eflag46 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag46, label %throw.unwind44, label %throw.cont45

throw.unwind44:                                   ; preds = %throw.cont41
  ret %Node* null

throw.cont45:                                     ; preds = %throw.cont41
  store %Node* %call43, %Node** %lf, align 8
  %ref47 = load i32*, i32** %p, align 8
  %ref48 = load i32*, i32** %p, align 8
  %p49 = load i32, i32* %ref48, align 4
  %add50 = add i32 %p49, 1
  store i32 %add50, i32* %ref47, align 4
  %ref51 = load %Node*, %Node** %lf, align 8
  ret %Node* %ref51
}

define %Node* @Parser.expr({ %Token*, i64 } %0, i32* %1, i32 %2) {
entry:
  %toks = alloca { %Token*, i64 }, align 8
  store { %Token*, i64 } %0, { %Token*, i64 }* %toks, align 8
  %p = alloca i32*, align 8
  store i32* %1, i32** %p, align 8
  %n = alloca i32, align 4
  store i32 %2, i32* %n, align 4
  %l = alloca %Node*, align 8
  %toks1 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %ref = load i32*, i32** %p, align 8
  %n2 = load i32, i32* %n, align 4
  %call = call %Node* @Parser.term({ %Token*, i64 } %toks1, i32* %ref, i32 %n2)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret %Node* null

throw.cont:                                       ; preds = %entry
  store %Node* %call, %Node** %l, align 8
  br label %while.cond

while.cond:                                       ; preds = %throw.cont42, %throw.cont
  %ref3 = load i32*, i32** %p, align 8
  %p4 = load i32, i32* %ref3, align 4
  %n5 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %p4, %n5
  %toks6 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data = extractvalue { %Token*, i64 } %toks6, 0
  %ref7 = load i32*, i32** %p, align 8
  %p8 = load i32, i32* %ref7, align 4
  %arr.at = getelementptr %Token, %Token* %arr.data, i32 %p8
  %type = getelementptr %Token, %Token* %arr.at, i32 0, i32 1
  %type9 = load i32, i32* %type, align 4
  %eq = icmp eq i32 %type9, 2
  %toks10 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data11 = extractvalue { %Token*, i64 } %toks10, 0
  %ref12 = load i32*, i32** %p, align 8
  %p13 = load i32, i32* %ref12, align 4
  %arr.at14 = getelementptr %Token, %Token* %arr.data11, i32 %p13
  %type15 = getelementptr %Token, %Token* %arr.at14, i32 0, i32 1
  %type16 = load i32, i32* %type15, align 4
  %eq17 = icmp eq i32 %type16, 3
  %or = or i1 %eq, %eq17
  %and = and i1 %lt, %or
  br i1 %and, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %k = alloca i32, align 4
  %toks18 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data19 = extractvalue { %Token*, i64 } %toks18, 0
  %ref20 = load i32*, i32** %p, align 8
  %p21 = load i32, i32* %ref20, align 4
  %arr.at22 = getelementptr %Token, %Token* %arr.data19, i32 %p21
  %type23 = getelementptr %Token, %Token* %arr.at22, i32 0, i32 1
  %type24 = load i32, i32* %type23, align 4
  store i32 %type24, i32* %k, align 4
  %ref25 = load i32*, i32** %p, align 8
  %ref26 = load i32*, i32** %p, align 8
  %p27 = load i32, i32* %ref26, align 4
  %add = add i32 %p27, 1
  store i32 %add, i32* %ref25, align 4
  %r = alloca %Node*, align 8
  %toks28 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %ref29 = load i32*, i32** %p, align 8
  %n30 = load i32, i32* %n, align 4
  %call31 = call %Node* @Parser.term({ %Token*, i64 } %toks28, i32* %ref29, i32 %n30)
  %eflag34 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag34, label %throw.unwind32, label %throw.cont33

while.end:                                        ; preds = %while.cond
  %ref49 = load %Node*, %Node** %l, align 8
  ret %Node* %ref49

throw.unwind32:                                   ; preds = %while.body
  ret %Node* null

throw.cont33:                                     ; preds = %while.body
  store %Node* %call31, %Node** %r, align 8
  %parent = alloca %Node*, align 8
  %k35 = load i32, i32* %k, align 4
  %call36 = call %Token* @Token.op(i32 %k35)
  %eflag39 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag39, label %throw.unwind37, label %throw.cont38

throw.unwind37:                                   ; preds = %throw.cont33
  ret %Node* null

throw.cont38:                                     ; preds = %throw.cont33
  %call40 = call %Node* @Node.create(%Token* %call36)
  %eflag43 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag43, label %throw.unwind41, label %throw.cont42

throw.unwind41:                                   ; preds = %throw.cont38
  ret %Node* null

throw.cont42:                                     ; preds = %throw.cont38
  store %Node* %call40, %Node** %parent, align 8
  %ref44 = load %Node*, %Node** %l, align 8
  %ref45 = load %Node*, %Node** %parent, align 8
  %left = getelementptr %Node, %Node* %ref45, i32 0, i32 1
  store %Node* %ref44, %Node** %left, align 8
  %ref46 = load %Node*, %Node** %r, align 8
  %ref47 = load %Node*, %Node** %parent, align 8
  %right = getelementptr %Node, %Node* %ref47, i32 0, i32 2
  store %Node* %ref46, %Node** %right, align 8
  %ref48 = load %Node*, %Node** %parent, align 8
  store %Node* %ref48, %Node** %l, align 8
  br label %while.cond
}

define %Node* @Parser.term({ %Token*, i64 } %0, i32* %1, i32 %2) {
entry:
  %toks = alloca { %Token*, i64 }, align 8
  store { %Token*, i64 } %0, { %Token*, i64 }* %toks, align 8
  %p = alloca i32*, align 8
  store i32* %1, i32** %p, align 8
  %n = alloca i32, align 4
  store i32 %2, i32* %n, align 4
  %l = alloca %Node*, align 8
  %toks1 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %ref = load i32*, i32** %p, align 8
  %n2 = load i32, i32* %n, align 4
  %call = call %Node* @Parser.factor({ %Token*, i64 } %toks1, i32* %ref, i32 %n2)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret %Node* null

throw.cont:                                       ; preds = %entry
  store %Node* %call, %Node** %l, align 8
  br label %while.cond

while.cond:                                       ; preds = %throw.cont42, %throw.cont
  %ref3 = load i32*, i32** %p, align 8
  %p4 = load i32, i32* %ref3, align 4
  %n5 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %p4, %n5
  %toks6 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data = extractvalue { %Token*, i64 } %toks6, 0
  %ref7 = load i32*, i32** %p, align 8
  %p8 = load i32, i32* %ref7, align 4
  %arr.at = getelementptr %Token, %Token* %arr.data, i32 %p8
  %type = getelementptr %Token, %Token* %arr.at, i32 0, i32 1
  %type9 = load i32, i32* %type, align 4
  %eq = icmp eq i32 %type9, 4
  %toks10 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data11 = extractvalue { %Token*, i64 } %toks10, 0
  %ref12 = load i32*, i32** %p, align 8
  %p13 = load i32, i32* %ref12, align 4
  %arr.at14 = getelementptr %Token, %Token* %arr.data11, i32 %p13
  %type15 = getelementptr %Token, %Token* %arr.at14, i32 0, i32 1
  %type16 = load i32, i32* %type15, align 4
  %eq17 = icmp eq i32 %type16, 5
  %or = or i1 %eq, %eq17
  %and = and i1 %lt, %or
  br i1 %and, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %k = alloca i32, align 4
  %toks18 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data19 = extractvalue { %Token*, i64 } %toks18, 0
  %ref20 = load i32*, i32** %p, align 8
  %p21 = load i32, i32* %ref20, align 4
  %arr.at22 = getelementptr %Token, %Token* %arr.data19, i32 %p21
  %type23 = getelementptr %Token, %Token* %arr.at22, i32 0, i32 1
  %type24 = load i32, i32* %type23, align 4
  store i32 %type24, i32* %k, align 4
  %ref25 = load i32*, i32** %p, align 8
  %ref26 = load i32*, i32** %p, align 8
  %p27 = load i32, i32* %ref26, align 4
  %add = add i32 %p27, 1
  store i32 %add, i32* %ref25, align 4
  %r = alloca %Node*, align 8
  %toks28 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %ref29 = load i32*, i32** %p, align 8
  %n30 = load i32, i32* %n, align 4
  %call31 = call %Node* @Parser.factor({ %Token*, i64 } %toks28, i32* %ref29, i32 %n30)
  %eflag34 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag34, label %throw.unwind32, label %throw.cont33

while.end:                                        ; preds = %while.cond
  %ref49 = load %Node*, %Node** %l, align 8
  ret %Node* %ref49

throw.unwind32:                                   ; preds = %while.body
  ret %Node* null

throw.cont33:                                     ; preds = %while.body
  store %Node* %call31, %Node** %r, align 8
  %parent = alloca %Node*, align 8
  %k35 = load i32, i32* %k, align 4
  %call36 = call %Token* @Token.op(i32 %k35)
  %eflag39 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag39, label %throw.unwind37, label %throw.cont38

throw.unwind37:                                   ; preds = %throw.cont33
  ret %Node* null

throw.cont38:                                     ; preds = %throw.cont33
  %call40 = call %Node* @Node.create(%Token* %call36)
  %eflag43 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag43, label %throw.unwind41, label %throw.cont42

throw.unwind41:                                   ; preds = %throw.cont38
  ret %Node* null

throw.cont42:                                     ; preds = %throw.cont38
  store %Node* %call40, %Node** %parent, align 8
  %ref44 = load %Node*, %Node** %l, align 8
  %ref45 = load %Node*, %Node** %parent, align 8
  %left = getelementptr %Node, %Node* %ref45, i32 0, i32 1
  store %Node* %ref44, %Node** %left, align 8
  %ref46 = load %Node*, %Node** %r, align 8
  %ref47 = load %Node*, %Node** %parent, align 8
  %right = getelementptr %Node, %Node* %ref47, i32 0, i32 2
  store %Node* %ref46, %Node** %right, align 8
  %ref48 = load %Node*, %Node** %parent, align 8
  store %Node* %ref48, %Node** %l, align 8
  br label %while.cond
}

define { i8*, i64 } @sign(i32 %0) {
entry:
  %k = alloca i32, align 4
  store i32 %0, i32* %k, align 4
  %k1 = load i32, i32* %k, align 4
  %eq = icmp eq i32 %k1, 2
  br i1 %eq, label %case.body, label %case.next

match.end:                                        ; No predecessors!
  ret { i8*, i64 } zeroinitializer

case.body:                                        ; preds = %entry
  ret { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0), i64 1 }

case.next:                                        ; preds = %entry
  %eq4 = icmp eq i32 %k1, 3
  br i1 %eq4, label %case.body2, label %case.next3

case.body2:                                       ; preds = %case.next
  ret { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0), i64 1 }

case.next3:                                       ; preds = %case.next
  %eq7 = icmp eq i32 %k1, 4
  br i1 %eq7, label %case.body5, label %case.next6

case.body5:                                       ; preds = %case.next3
  ret { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0), i64 1 }

case.next6:                                       ; preds = %case.next3
  %eq10 = icmp eq i32 %k1, 5
  br i1 %eq10, label %case.body8, label %case.next9

case.body8:                                       ; preds = %case.next6
  ret { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0), i64 1 }

case.next9:                                       ; preds = %case.next6
  ret { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0), i64 1 }
}

define void @label(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  %ref = load %Node*, %Node** %n, align 8
  %token = getelementptr %Node, %Node* %ref, i32 0, i32 0
  %ref1 = load %Token*, %Token** %token, align 8
  %type = getelementptr %Token, %Token* %ref1, i32 0, i32 1
  %type2 = load i32, i32* %type, align 4
  %eq = icmp eq i32 %type2, 1
  br i1 %eq, label %then, label %next

endif:                                            ; preds = %throw.cont, %then
  ret void

then:                                             ; preds = %entry
  %ref3 = load %Node*, %Node** %n, align 8
  %token4 = getelementptr %Node, %Node* %ref3, i32 0, i32 0
  %ref5 = load %Token*, %Token** %token4, align 8
  %value = getelementptr %Token, %Token* %ref5, i32 0, i32 0
  %value6 = load i32, i32* %value, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %value6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %ref7 = load %Node*, %Node** %n, align 8
  %token8 = getelementptr %Node, %Node* %ref7, i32 0, i32 0
  %ref9 = load %Token*, %Token** %token8, align 8
  %type10 = getelementptr %Token, %Token* %ref9, i32 0, i32 1
  %type11 = load i32, i32* %type10, align 4
  %call = call { i8*, i64 } @sign(i32 %type11)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %next
  ret void

throw.cont:                                       ; preds = %next
  %str.len = extractvalue { i8*, i64 } %call, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %call, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.22, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)

define void @show(%Node* %0, { i8*, i64 } %1, i1 %2) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  %pre = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %pre, align 8
  %last = alloca i1, align 1
  store i1 %2, i1* %last, align 1
  %pre1 = load { i8*, i64 }, { i8*, i64 }* %pre, align 8
  %str.len = extractvalue { i8*, i64 } %pre1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %pre1, 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.23, i32 0, i32 0), i32 %len32, i8* %str.data)
  %last2 = load i1, i1* %last, align 1
  br i1 %last2, label %then, label %next

endif:                                            ; preds = %next, %then
  %ref = load %Node*, %Node** %n, align 8
  call void @label(%Node* %ref)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

then:                                             ; preds = %entry
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.25, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.24, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.27, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.26, i32 0, i32 0))
  br label %endif

throw.unwind:                                     ; preds = %endif
  ret void

throw.cont:                                       ; preds = %endif
  %kid = alloca %String, align 8
  %pre3 = load { i8*, i64 }, { i8*, i64 }* %pre, align 8
  %call = call %String @String.from({ i8*, i64 } %pre3)
  %eflag6 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag6, label %throw.unwind4, label %throw.cont5

throw.unwind4:                                    ; preds = %throw.cont
  call void @String.drop(%String* %kid)
  ret void

throw.cont5:                                      ; preds = %throw.cont
  store %String %call, %String* %kid, align 8
  %last10 = load i1, i1* %last, align 1
  br i1 %last10, label %then8, label %next9

endif7:                                           ; preds = %throw.cont15, %throw.cont12
  %ref19 = load %Node*, %Node** %n, align 8
  %left = getelementptr %Node, %Node* %ref19, i32 0, i32 1
  %ref20 = load %Node*, %Node** %left, align 8
  %refcmp = icmp ne %Node* %ref20, null
  br i1 %refcmp, label %then18, label %endif17

then8:                                            ; preds = %throw.cont5
  call void @String.join(%String* %kid, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.28, i32 0, i32 0), i64 3 })
  %eflag13 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag13, label %throw.unwind11, label %throw.cont12

next9:                                            ; preds = %throw.cont5
  call void @String.join(%String* %kid, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.29, i32 0, i32 0), i64 5 })
  %eflag16 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag16, label %throw.unwind14, label %throw.cont15

throw.unwind11:                                   ; preds = %then8
  call void @String.drop(%String* %kid)
  ret void

throw.cont12:                                     ; preds = %then8
  br label %endif7

throw.unwind14:                                   ; preds = %next9
  call void @String.drop(%String* %kid)
  ret void

throw.cont15:                                     ; preds = %next9
  br label %endif7

endif17:                                          ; preds = %throw.cont32, %endif7
  %ref36 = load %Node*, %Node** %n, align 8
  %right37 = getelementptr %Node, %Node* %ref36, i32 0, i32 2
  %ref38 = load %Node*, %Node** %right37, align 8
  %refcmp39 = icmp ne %Node* %ref38, null
  br i1 %refcmp39, label %then35, label %endif34

then18:                                           ; preds = %endif7
  %ref21 = load %Node*, %Node** %n, align 8
  %left22 = getelementptr %Node, %Node* %ref21, i32 0, i32 1
  %ref23 = load %Node*, %Node** %left22, align 8
  %call24 = call { i8*, i64 } @String.c_str(%String* %kid)
  %eflag27 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag27, label %throw.unwind25, label %throw.cont26

throw.unwind25:                                   ; preds = %then18
  call void @String.drop(%String* %kid)
  ret void

throw.cont26:                                     ; preds = %then18
  %ref28 = load %Node*, %Node** %n, align 8
  %right = getelementptr %Node, %Node* %ref28, i32 0, i32 2
  %ref29 = load %Node*, %Node** %right, align 8
  %refcmp30 = icmp eq %Node* %ref29, null
  call void @show(%Node* %ref23, { i8*, i64 } %call24, i1 %refcmp30)
  %eflag33 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag33, label %throw.unwind31, label %throw.cont32

throw.unwind31:                                   ; preds = %throw.cont26
  call void @String.drop(%String* %kid)
  ret void

throw.cont32:                                     ; preds = %throw.cont26
  br label %endif17

endif34:                                          ; preds = %throw.cont48, %endif17
  call void @String.drop(%String* %kid)
  ret void

then35:                                           ; preds = %endif17
  %ref40 = load %Node*, %Node** %n, align 8
  %right41 = getelementptr %Node, %Node* %ref40, i32 0, i32 2
  %ref42 = load %Node*, %Node** %right41, align 8
  %call43 = call { i8*, i64 } @String.c_str(%String* %kid)
  %eflag46 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag46, label %throw.unwind44, label %throw.cont45

throw.unwind44:                                   ; preds = %then35
  call void @String.drop(%String* %kid)
  ret void

throw.cont45:                                     ; preds = %then35
  call void @show(%Node* %ref42, { i8*, i64 } %call43, i1 true)
  %eflag49 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag49, label %throw.unwind47, label %throw.cont48

throw.unwind47:                                   ; preds = %throw.cont45
  call void @String.drop(%String* %kid)
  ret void

throw.cont48:                                     ; preds = %throw.cont45
  br label %endif34
}

define void @tree(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  %ref = load %Node*, %Node** %n, align 8
  call void @label(%Node* %ref)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  %ref1 = load %Node*, %Node** %n, align 8
  %left = getelementptr %Node, %Node* %ref1, i32 0, i32 1
  %ref2 = load %Node*, %Node** %left, align 8
  %refcmp = icmp ne %Node* %ref2, null
  br i1 %refcmp, label %then, label %endif

endif:                                            ; preds = %throw.cont10, %throw.cont
  %ref14 = load %Node*, %Node** %n, align 8
  %right15 = getelementptr %Node, %Node* %ref14, i32 0, i32 2
  %ref16 = load %Node*, %Node** %right15, align 8
  %refcmp17 = icmp ne %Node* %ref16, null
  br i1 %refcmp17, label %then13, label %endif12

then:                                             ; preds = %throw.cont
  %ref3 = load %Node*, %Node** %n, align 8
  %left4 = getelementptr %Node, %Node* %ref3, i32 0, i32 1
  %ref5 = load %Node*, %Node** %left4, align 8
  %ref6 = load %Node*, %Node** %n, align 8
  %right = getelementptr %Node, %Node* %ref6, i32 0, i32 2
  %ref7 = load %Node*, %Node** %right, align 8
  %refcmp8 = icmp eq %Node* %ref7, null
  call void @show(%Node* %ref5, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.30, i32 0, i32 0), i64 0 }, i1 %refcmp8)
  %eflag11 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag11, label %throw.unwind9, label %throw.cont10

throw.unwind9:                                    ; preds = %then
  ret void

throw.cont10:                                     ; preds = %then
  br label %endif

endif12:                                          ; preds = %throw.cont22, %endif
  ret void

then13:                                           ; preds = %endif
  %ref18 = load %Node*, %Node** %n, align 8
  %right19 = getelementptr %Node, %Node* %ref18, i32 0, i32 2
  %ref20 = load %Node*, %Node** %right19, align 8
  call void @show(%Node* %ref20, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.31, i32 0, i32 0), i64 0 }, i1 true)
  %eflag23 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag23, label %throw.unwind21, label %throw.cont22

throw.unwind21:                                   ; preds = %then13
  ret void

throw.cont22:                                     ; preds = %then13
  br label %endif12
}

define i32 @eval(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  %ref = load %Node*, %Node** %n, align 8
  %unbound = icmp eq %Node* %ref, null
  br i1 %unbound, label %trap, label %cont

match.end:                                        ; No predecessors!
  ret i32 0

trap:                                             ; preds = %entry
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([232 x i8], [232 x i8]* @trap_msg.32, i32 0, i32 0), i64 231)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %token = getelementptr %Node, %Node* %ref, i32 0, i32 0
  %ref1 = load %Token*, %Token** %token, align 8
  %unbound2 = icmp eq %Token* %ref1, null
  br i1 %unbound2, label %trap3, label %cont4

trap3:                                            ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([242 x i8], [242 x i8]* @trap_msg.33, i32 0, i32 0), i64 241)
  call void @exit(i32 1)
  unreachable

cont4:                                            ; preds = %cont
  %type = getelementptr %Token, %Token* %ref1, i32 0, i32 1
  %type5 = load i32, i32* %type, align 4
  %eq = icmp eq i32 %type5, 1
  br i1 %eq, label %case.body, label %case.next

case.body:                                        ; preds = %cont4
  %ref6 = load %Node*, %Node** %n, align 8
  %token7 = getelementptr %Node, %Node* %ref6, i32 0, i32 0
  %ref8 = load %Token*, %Token** %token7, align 8
  %value = getelementptr %Token, %Token* %ref8, i32 0, i32 0
  %value9 = load i32, i32* %value, align 4
  ret i32 %value9

case.next:                                        ; preds = %cont4
  %eq12 = icmp eq i32 %type5, 2
  br i1 %eq12, label %case.body10, label %case.next11

case.body10:                                      ; preds = %case.next
  %ref13 = load %Node*, %Node** %n, align 8
  %left = getelementptr %Node, %Node* %ref13, i32 0, i32 1
  %ref14 = load %Node*, %Node** %left, align 8
  %call = call i32 @eval(%Node* %ref14)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

case.next11:                                      ; preds = %case.next
  %eq23 = icmp eq i32 %type5, 3
  br i1 %eq23, label %case.body21, label %case.next22

throw.unwind:                                     ; preds = %case.body10
  ret i32 0

throw.cont:                                       ; preds = %case.body10
  %ref15 = load %Node*, %Node** %n, align 8
  %right = getelementptr %Node, %Node* %ref15, i32 0, i32 2
  %ref16 = load %Node*, %Node** %right, align 8
  %call17 = call i32 @eval(%Node* %ref16)
  %eflag20 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag20, label %throw.unwind18, label %throw.cont19

throw.unwind18:                                   ; preds = %throw.cont
  ret i32 0

throw.cont19:                                     ; preds = %throw.cont
  %add = add i32 %call, %call17
  ret i32 %add

case.body21:                                      ; preds = %case.next11
  %ref24 = load %Node*, %Node** %n, align 8
  %left25 = getelementptr %Node, %Node* %ref24, i32 0, i32 1
  %ref26 = load %Node*, %Node** %left25, align 8
  %call27 = call i32 @eval(%Node* %ref26)
  %eflag30 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag30, label %throw.unwind28, label %throw.cont29

case.next22:                                      ; preds = %case.next11
  %eq40 = icmp eq i32 %type5, 4
  br i1 %eq40, label %case.body38, label %case.next39

throw.unwind28:                                   ; preds = %case.body21
  ret i32 0

throw.cont29:                                     ; preds = %case.body21
  %ref31 = load %Node*, %Node** %n, align 8
  %right32 = getelementptr %Node, %Node* %ref31, i32 0, i32 2
  %ref33 = load %Node*, %Node** %right32, align 8
  %call34 = call i32 @eval(%Node* %ref33)
  %eflag37 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag37, label %throw.unwind35, label %throw.cont36

throw.unwind35:                                   ; preds = %throw.cont29
  ret i32 0

throw.cont36:                                     ; preds = %throw.cont29
  %sub = sub i32 %call27, %call34
  ret i32 %sub

case.body38:                                      ; preds = %case.next22
  %ref41 = load %Node*, %Node** %n, align 8
  %left42 = getelementptr %Node, %Node* %ref41, i32 0, i32 1
  %ref43 = load %Node*, %Node** %left42, align 8
  %call44 = call i32 @eval(%Node* %ref43)
  %eflag47 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag47, label %throw.unwind45, label %throw.cont46

case.next39:                                      ; preds = %case.next22
  %eq57 = icmp eq i32 %type5, 5
  br i1 %eq57, label %case.body55, label %case.next56

throw.unwind45:                                   ; preds = %case.body38
  ret i32 0

throw.cont46:                                     ; preds = %case.body38
  %ref48 = load %Node*, %Node** %n, align 8
  %right49 = getelementptr %Node, %Node* %ref48, i32 0, i32 2
  %ref50 = load %Node*, %Node** %right49, align 8
  %call51 = call i32 @eval(%Node* %ref50)
  %eflag54 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag54, label %throw.unwind52, label %throw.cont53

throw.unwind52:                                   ; preds = %throw.cont46
  ret i32 0

throw.cont53:                                     ; preds = %throw.cont46
  %mul = mul i32 %call44, %call51
  ret i32 %mul

case.body55:                                      ; preds = %case.next39
  %d = alloca i32, align 4
  %ref58 = load %Node*, %Node** %n, align 8
  %right59 = getelementptr %Node, %Node* %ref58, i32 0, i32 2
  %ref60 = load %Node*, %Node** %right59, align 8
  %call61 = call i32 @eval(%Node* %ref60)
  %eflag64 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag64, label %throw.unwind62, label %throw.cont63

case.next56:                                      ; preds = %case.next39
  ret i32 0

throw.unwind62:                                   ; preds = %case.body55
  ret i32 0

throw.cont63:                                     ; preds = %case.body55
  store i32 %call61, i32* %d, align 4
  %d65 = load i32, i32* %d, align 4
  %eq66 = icmp eq i32 %d65, 0
  br i1 %eq66, label %then, label %endif

endif:                                            ; preds = %throw.cont63
  %ref71 = load %Node*, %Node** %n, align 8
  %left72 = getelementptr %Node, %Node* %ref71, i32 0, i32 1
  %ref73 = load %Node*, %Node** %left72, align 8
  %call74 = call i32 @eval(%Node* %ref73)
  %eflag77 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag77, label %throw.unwind75, label %throw.cont76

then:                                             ; preds = %throw.cont63
  %call67 = call %Error @Error.make({ i8*, i64 } { i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.34, i32 0, i32 0), i64 16 })
  %eflag70 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag70, label %throw.unwind68, label %throw.cont69

throw.unwind68:                                   ; preds = %then
  ret i32 0

throw.cont69:                                     ; preds = %then
  store %Error %call67, %Error* @ura.err_value, align 8
  store i1 true, i1* @ura.err_flag, align 1
  ret i32 0

throw.unwind75:                                   ; preds = %endif
  ret i32 0

throw.cont76:                                     ; preds = %endif
  %d78 = load i32, i32* %d, align 4
  %iszero = icmp eq i32 %d78, 0
  br i1 %iszero, label %trap79, label %cont80

trap79:                                           ; preds = %throw.cont76
  %3 = call i64 @write(i32 2, i8* getelementptr inbounds ([210 x i8], [210 x i8]* @trap_msg.35, i32 0, i32 0), i64 209)
  call void @exit(i32 1)
  unreachable

cont80:                                           ; preds = %throw.cont76
  %div = sdiv i32 %call74, %d78
  ret i32 %div
}

define void @calc({ i8*, i64 } %0) {
entry:
  %text = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %text, align 8
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.37, i32 0, i32 0), i32 41, i8* getelementptr inbounds ([42 x i8], [42 x i8]* @str.36, i32 0, i32 0))
  %text1 = load { i8*, i64 }, { i8*, i64 }* %text, align 8
  %str.len = extractvalue { i8*, i64 } %text1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %text1, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.40, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.38, i32 0, i32 0), i32 %len32, i8* %str.data, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.39, i32 0, i32 0))
  %src = alloca %String, align 8
  %text2 = load { i8*, i64 }, { i8*, i64 }* %text, align 8
  %call = call %String @String.from({ i8*, i64 } %text2)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  call void @String.drop(%String* %src)
  ret void

throw.cont:                                       ; preds = %entry
  store %String %call, %String* %src, align 8
  %toks = alloca { %Token*, i64 }, align 8
  %heap = call i8* @calloc(i64 64, i64 8)
  %arr = bitcast i8* %heap to %Token*
  %arr.ptr = insertvalue { %Token*, i64 } undef, %Token* %arr, 0
  %arr.len = insertvalue { %Token*, i64 } %arr.ptr, i64 64, 1
  store { %Token*, i64 } %arr.len, { %Token*, i64 }* %toks, align 8
  %n = alloca i32, align 4
  %toks3 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %call4 = call i32 @Lexer.run(%String* %src, { %Token*, i64 } %toks3)
  %eflag7 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag7, label %throw.unwind5, label %throw.cont6

throw.unwind5:                                    ; preds = %throw.cont
  call void @String.drop(%String* %src)
  ret void

throw.cont6:                                      ; preds = %throw.cont
  store i32 %call4, i32* %n, align 4
  %p = alloca i32, align 4
  store i32 0, i32* %p, align 4
  %root = alloca %Node*, align 8
  %toks8 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %n9 = load i32, i32* %n, align 4
  %call10 = call %Node* @Parser.expr({ %Token*, i64 } %toks8, i32* %p, i32 %n9)
  %eflag13 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag13, label %throw.unwind11, label %throw.cont12

throw.unwind11:                                   ; preds = %throw.cont6
  call void @String.drop(%String* %src)
  ret void

throw.cont12:                                     ; preds = %throw.cont6
  store %Node* %call10, %Node** %root, align 8
  %ref = load %Node*, %Node** %root, align 8
  call void @tree(%Node* %ref)
  %eflag16 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag16, label %throw.unwind14, label %throw.cont15

throw.unwind14:                                   ; preds = %throw.cont12
  call void @String.drop(%String* %src)
  ret void

throw.cont15:                                     ; preds = %throw.cont12
  %ref17 = load %Node*, %Node** %root, align 8
  %call18 = call i32 @eval(%Node* %ref17)
  %eflag21 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag21, label %throw.unwind19, label %throw.cont20

catch:                                            ; preds = %throw.unwind19
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.44, i32 0, i32 0))
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %message22 = load { i8*, i64 }, { i8*, i64 }* %message, align 8
  %str.len23 = extractvalue { i8*, i64 } %message22, 1
  %len3224 = trunc i64 %str.len23 to i32
  %str.data25 = extractvalue { i8*, i64 } %message22, 0
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.47, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.45, i32 0, i32 0), i32 %len3224, i8* %str.data25, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.48, i32 0, i32 0))
  br label %try.end

try.end:                                          ; preds = %catch, %throw.cont20
  %ref26 = load %Node*, %Node** %root, align 8
  %isnull = icmp eq %Node* %ref26, null
  br i1 %isnull, label %clean.done, label %clean.live

throw.unwind19:                                   ; preds = %throw.cont15
  br label %catch

throw.cont20:                                     ; preds = %throw.cont15
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.43, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.41, i32 0, i32 0), i32 %call18, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.42, i32 0, i32 0))
  br label %try.end

clean.live:                                       ; preds = %try.end
  call void @Node.drop(%Node* %ref26)
  %free.ptr = bitcast %Node* %ref26 to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %try.end
  store %Node* null, %Node** %root, align 8
  %arr27 = load { %Token*, i64 }, { %Token*, i64 }* %toks, align 8
  %arr.data = extractvalue { %Token*, i64 } %arr27, 0
  %free.ptr28 = bitcast %Token* %arr.data to i8*
  call void @free(i8* %free.ptr28)
  store { %Token*, i64 } zeroinitializer, { %Token*, i64 }* %toks, align 8
  call void @String.drop(%String* %src)
  ret void
}

declare i32 @dprintf(i32, i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  store i32 1, i32* @EPERM, align 4
  store i32 2, i32* @ENOENT, align 4
  store i32 3, i32* @ESRCH, align 4
  store i32 4, i32* @EINTR, align 4
  store i32 5, i32* @EIO, align 4
  store i32 6, i32* @ENXIO, align 4
  store i32 7, i32* @E2BIG, align 4
  store i32 8, i32* @ENOEXEC, align 4
  store i32 9, i32* @EBADF, align 4
  store i32 10, i32* @ECHILD, align 4
  store i32 13, i32* @EACCES, align 4
  store i32 14, i32* @EFAULT, align 4
  store i32 15, i32* @ENOTBLK, align 4
  store i32 16, i32* @EBUSY, align 4
  store i32 17, i32* @EEXIST, align 4
  store i32 18, i32* @EXDEV, align 4
  store i32 19, i32* @ENODEV, align 4
  store i32 20, i32* @ENOTDIR, align 4
  store i32 21, i32* @EISDIR, align 4
  store i32 22, i32* @EINVAL, align 4
  store i32 23, i32* @ENFILE, align 4
  store i32 24, i32* @EMFILE, align 4
  store i32 25, i32* @ENOTTY, align 4
  store i32 26, i32* @ETXTBSY, align 4
  store i32 27, i32* @EFBIG, align 4
  store i32 28, i32* @ENOSPC, align 4
  store i32 29, i32* @ESPIPE, align 4
  store i32 30, i32* @EROFS, align 4
  store i32 31, i32* @EMLINK, align 4
  store i32 32, i32* @EPIPE, align 4
  store i32 33, i32* @EDOM, align 4
  store i32 34, i32* @ERANGE, align 4
  store i32 0, i32* @O_RDONLY, align 4
  store i32 1, i32* @O_WRONLY, align 4
  store i32 2, i32* @O_RDWR, align 4
  store i32 1, i32* @FD_CLOEXEC, align 4
  store i32 0, i32* @IPPROTO_IP, align 4
  store i32 6, i32* @IPPROTO_TCP, align 4
  store i32 17, i32* @IPPROTO_UDP, align 4
  store i32 41, i32* @IPPROTO_IPV6, align 4
  store i32 0, i32* @SHUT_RD, align 4
  store i32 1, i32* @SHUT_WR, align 4
  store i32 2, i32* @SHUT_RDWR, align 4
  store i32 1, i32* @POLLIN, align 4
  store i32 2, i32* @POLLPRI, align 4
  store i32 4, i32* @POLLOUT, align 4
  store i32 8, i32* @POLLERR, align 4
  store i32 16, i32* @POLLHUP, align 4
  store i32 32, i32* @POLLNVAL, align 4
  store i32 0, i32* @INADDR_ANY, align 4
  store i32 2130706433, i32* @INADDR_LOOPBACK, align 4
  store i32 -1, i32* @INADDR_BROADCAST, align 4
  store i32 1, i32* @SIGHUP, align 4
  store i32 2, i32* @SIGINT, align 4
  store i32 3, i32* @SIGQUIT, align 4
  store i32 4, i32* @SIGILL, align 4
  store i32 5, i32* @SIGTRAP, align 4
  store i32 6, i32* @SIGABRT, align 4
  store i32 8, i32* @SIGFPE, align 4
  store i32 9, i32* @SIGKILL, align 4
  store i32 11, i32* @SIGSEGV, align 4
  store i32 13, i32* @SIGPIPE, align 4
  store i32 14, i32* @SIGALRM, align 4
  store i32 15, i32* @SIGTERM, align 4
  store i32 61440, i32* @S_IFMT, align 4
  store i32 49152, i32* @S_IFSOCK, align 4
  store i32 40960, i32* @S_IFLNK, align 4
  store i32 32768, i32* @S_IFREG, align 4
  store i32 24576, i32* @S_IFBLK, align 4
  store i32 16384, i32* @S_IFDIR, align 4
  store i32 8192, i32* @S_IFCHR, align 4
  store i32 4096, i32* @S_IFIFO, align 4
  store i32 2048, i32* @S_ISUID, align 4
  store i32 1024, i32* @S_ISGID, align 4
  store i32 512, i32* @S_ISVTX, align 4
  store i32 0, i32* @F_OK, align 4
  store i32 1, i32* @X_OK, align 4
  store i32 2, i32* @W_OK, align 4
  store i32 4, i32* @R_OK, align 4
  store i32 0, i32* @SEEK_SET, align 4
  store i32 1, i32* @SEEK_CUR, align 4
  store i32 2, i32* @SEEK_END, align 4
  store i32 0, i32* @STDIN_FILENO, align 4
  store i32 1, i32* @STDOUT_FILENO, align 4
  store i32 2, i32* @STDERR_FILENO, align 4
  call void @calc({ i8*, i64 } { i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.49, i32 0, i32 0), i64 20 })
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  %msg = load { i8*, i64 }, { i8*, i64 }* getelementptr inbounds (%Error, %Error* @ura.err_value, i32 0, i32 0), align 8
  %msg.data = extractvalue { i8*, i64 } %msg, 0
  %msg.len = extractvalue { i8*, i64 } %msg, 1
  %2 = call i64 @write(i32 2, i8* %msg.data, i64 %msg.len)
  call void @exit(i32 1)
  unreachable

throw.cont:                                       ; preds = %entry
  call void @calc({ i8*, i64 } { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.50, i32 0, i32 0), i64 11 })
  %eflag3 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag3, label %throw.unwind1, label %throw.cont2

throw.unwind1:                                    ; preds = %throw.cont
  %msg4 = load { i8*, i64 }, { i8*, i64 }* getelementptr inbounds (%Error, %Error* @ura.err_value, i32 0, i32 0), align 8
  %msg.data5 = extractvalue { i8*, i64 } %msg4, 0
  %msg.len6 = extractvalue { i8*, i64 } %msg4, 1
  %3 = call i64 @write(i32 2, i8* %msg.data5, i64 %msg.len6)
  call void @exit(i32 1)
  unreachable

throw.cont2:                                      ; preds = %throw.cont
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.52, i32 0, i32 0), i32 41, i8* getelementptr inbounds ([42 x i8], [42 x i8]* @str.51, i32 0, i32 0))
  ret i32 0
}
```
