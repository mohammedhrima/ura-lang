# string

## index

- 001 — String constructors and accessors
- 002 — assign, push, pop, join and clear
- 003 — find, contains, starts_with and ends_with
- 004 — transforms: upper, lower, substr, trim, replace, repeat, reverse
- 005 — operators: + += == != < > against String and against a literal
- 006 — to_int, compare and chained transformations

## 001 — String constructors and accessors

```ura
// string/001.ura - String constructors and accessors
use "@/header"

main():
    s1 String = String::create()
    output(s1.len(), " ", s1.empty(), "\n")

    s2 String = String::from("hello")
    output(s2.value, " ", s2.len(), " ", s2.empty(), "\n")

    s3 String = String::from("")
    output(s3.len(), " ", s3.empty(), "\n")

    s4 String = String::from_int(42)
    output(s4.value, "\n")

    s5 String = String::from_int(-123)
    output(s5.value, "\n")

    s6 String = String::from_int(0)
    output(s6.value, "\n")

    output(s2.at(0), " ", s2.at(4), " ",
           s2.at(-1) as i32, " ", s2.at(99) as i32, "\n")
    output(s2.c_str(), "\n")
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

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ s1 : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ output : void
│  ├─ call len : i32
│  │  └─ s1 : STRUCT_CALL
│  ├─ char[] " "
│  ├─ call empty : bool
│  │  └─ s1 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s2 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "hello"
├─ output : void
│  ├─ .value : char[]
│  │  └─ s2 : STRUCT_CALL
│  ├─ char[] " "
│  ├─ call len : i32
│  │  └─ s2 : STRUCT_CALL
│  ├─ char[] " "
│  ├─ call empty : bool
│  │  └─ s2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s3 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] ""
├─ output : void
│  ├─ call len : i32
│  │  └─ s3 : STRUCT_CALL
│  ├─ char[] " "
│  ├─ call empty : bool
│  │  └─ s3 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s4 : STRUCT_CALL
│  └─ call from_int : STRUCT_CALL
│     └─ int 42
├─ output : void
│  ├─ .value : char[]
│  │  └─ s4 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s5 : STRUCT_CALL
│  └─ call from_int : STRUCT_CALL
│     └─ - : i32
│        ├─ int 0
│        └─ int 123
├─ output : void
│  ├─ .value : char[]
│  │  └─ s5 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s6 : STRUCT_CALL
│  └─ call from_int : STRUCT_CALL
│     └─ int 0
├─ output : void
│  ├─ .value : char[]
│  │  └─ s6 : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ call at : char
│  │  ├─ s2 : STRUCT_CALL
│  │  └─ int 0
│  ├─ char[] " "
│  ├─ call at : char
│  │  ├─ s2 : STRUCT_CALL
│  │  └─ int 4
│  ├─ char[] " "
│  ├─ cast : i32
│  │  └─ call at : char
│  │     ├─ s2 : STRUCT_CALL
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ int 1
│  ├─ char[] " "
│  ├─ cast : i32
│  │  └─ call at : char
│  │     ├─ s2 : STRUCT_CALL
│  │     └─ int 99
│  └─ char[] "\n"
└─ output : void
   ├─ call c_str : char[]
   │  └─ s2 : STRUCT_CALL
   └─ char[] "\n"
```

```out
0 True
hello 5 False
0 True
42
-123
0
h o 0 0
hello
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }

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
@str.15 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%s%.*s\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.20 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.21 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.23 = private unnamed_addr constant [21 x i8] c"%.*s%.*s%d%.*s%s%.*s\00", align 1
@str.24 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.25 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.26 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.27 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [13 x i8] c"%d%.*s%s%.*s\00", align 1
@str.30 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.31 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.32 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.33 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.34 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.35 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.36 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.37 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.38 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.39 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.40 = private unnamed_addr constant [25 x i8] c"%c%.*s%c%.*s%d%.*s%d%.*s\00", align 1
@str.41 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.42 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
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
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
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

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
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
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
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

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
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
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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

endif:                                            ; preds = %entry
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

then:                                             ; preds = %entry
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

while.cond:                                       ; preds = %while.body, %endif7
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
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
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
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
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
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
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
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
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
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
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
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
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
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
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
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
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
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
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
  %s1 = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s1, align 8
  %call1 = call i32 @String.len(%String* %s1)
  %call2 = call i1 @String.empty(%String* %s1)
  %bool_str = select i1 %call2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %call1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %s2 = alloca %String, align 8
  %call3 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i64 5 })
  store %String %call3, %String* %s2, align 8
  %value = getelementptr %String, %String* %s2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %str.len = extractvalue { i8*, i64 } %value4, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %value4, 0
  %call5 = call i32 @String.len(%String* %s2)
  %call6 = call i1 @String.empty(%String* %s2)
  %bool_str7 = select i1 %call6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.20, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.21, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt.23, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0), i32 %call5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0), i8* %bool_str7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  %s3 = alloca %String, align 8
  %call8 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.24, i32 0, i32 0), i64 0 })
  store %String %call8, %String* %s3, align 8
  %call9 = call i32 @String.len(%String* %s3)
  %call10 = call i1 @String.empty(%String* %s3)
  %bool_str11 = select i1 %call10, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.26, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.27, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.29, i32 0, i32 0), i32 %call9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0), i8* %bool_str11, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %s4 = alloca %String, align 8
  %call12 = call %String @String.from_int(i32 42)
  store %String %call12, %String* %s4, align 8
  %value13 = getelementptr %String, %String* %s4, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value13, align 8
  %str.len15 = extractvalue { i8*, i64 } %value14, 1
  %len3216 = trunc i64 %str.len15 to i32
  %str.data17 = extractvalue { i8*, i64 } %value14, 0
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.31, i32 0, i32 0), i32 %len3216, i8* %str.data17, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.30, i32 0, i32 0))
  %s5 = alloca %String, align 8
  %call18 = call %String @String.from_int(i32 -123)
  store %String %call18, %String* %s5, align 8
  %value19 = getelementptr %String, %String* %s5, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %str.len21 = extractvalue { i8*, i64 } %value20, 1
  %len3222 = trunc i64 %str.len21 to i32
  %str.data23 = extractvalue { i8*, i64 } %value20, 0
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.33, i32 0, i32 0), i32 %len3222, i8* %str.data23, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.32, i32 0, i32 0))
  %s6 = alloca %String, align 8
  %call24 = call %String @String.from_int(i32 0)
  store %String %call24, %String* %s6, align 8
  %value25 = getelementptr %String, %String* %s6, i32 0, i32 0
  %value26 = load { i8*, i64 }, { i8*, i64 }* %value25, align 8
  %str.len27 = extractvalue { i8*, i64 } %value26, 1
  %len3228 = trunc i64 %str.len27 to i32
  %str.data29 = extractvalue { i8*, i64 } %value26, 0
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.35, i32 0, i32 0), i32 %len3228, i8* %str.data29, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.34, i32 0, i32 0))
  %call30 = call i8 @String.at(%String* %s2, i32 0)
  %c2i = sext i8 %call30 to i32
  %call31 = call i8 @String.at(%String* %s2, i32 4)
  %c2i32 = sext i8 %call31 to i32
  %call33 = call i8 @String.at(%String* %s2, i32 -1)
  %cast = sext i8 %call33 to i32
  %call34 = call i8 @String.at(%String* %s2, i32 99)
  %cast35 = sext i8 %call34 to i32
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @fmt.40, i32 0, i32 0), i32 %c2i, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.36, i32 0, i32 0), i32 %c2i32, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.37, i32 0, i32 0), i32 %cast, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.38, i32 0, i32 0), i32 %cast35, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.39, i32 0, i32 0))
  %call36 = call { i8*, i64 } @String.c_str(%String* %s2)
  %str.len37 = extractvalue { i8*, i64 } %call36, 1
  %len3238 = trunc i64 %str.len37 to i32
  %str.data39 = extractvalue { i8*, i64 } %call36, 0
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.42, i32 0, i32 0), i32 %len3238, i8* %str.data39, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.41, i32 0, i32 0))
  call void @String.drop(%String* %s6)
  call void @String.drop(%String* %s5)
  call void @String.drop(%String* %s4)
  call void @String.drop(%String* %s3)
  call void @String.drop(%String* %s2)
  call void @String.drop(%String* %s1)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — assign, push, pop, join and clear

```ura
// string/002.ura - assign, push, pop, join and clear
use "@/header"

main():
    s String = String::create()
    s = "abc"
    output(s.value, " ", s.len(), "\n")

    s = "hello world this is a longer string"
    output(s.value, "\n")

    s = ""
    output(s.len(), " ", s.empty(), "\n")

    s2 String = String::create()
    s2.push('X')
    s2.push('Y')
    s2.push('Z')
    output(s2.value, " ", s2.len(), "\n")

    c char = s2.pop()
    output(c, " ", s2.value, " ", s2.len(), "\n")

    s2.pop()
    s2.pop()
    c2 char = s2.pop()
    output(c2 as i32, " ", s2.len(), "\n")

    s3 String = String::from("hello")
    s3.join(" world")
    output(s3.value, "\n")

    s3.clear()
    output(s3.len(), " ", s3.empty(), "\n")
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

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : void
│  ├─ s : STRUCT_CALL
│  └─ char[] "abc"
├─ output : void
│  ├─ .value : char[]
│  │  └─ s : STRUCT_CALL
│  ├─ char[] " "
│  ├─ call len : i32
│  │  └─ s : STRUCT_CALL
│  └─ char[] "\n"
├─ = : void
│  ├─ s : STRUCT_CALL
│  └─ char[] "hello world this is a longer string"
├─ output : void
│  ├─ .value : char[]
│  │  └─ s : STRUCT_CALL
│  └─ char[] "\n"
├─ = : void
│  ├─ s : STRUCT_CALL
│  └─ char[] ""
├─ output : void
│  ├─ call len : i32
│  │  └─ s : STRUCT_CALL
│  ├─ char[] " "
│  ├─ call empty : bool
│  │  └─ s : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s2 : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ call push : void
│  ├─ s2 : STRUCT_CALL
│  └─ char 'X'
├─ call push : void
│  ├─ s2 : STRUCT_CALL
│  └─ char 'Y'
├─ call push : void
│  ├─ s2 : STRUCT_CALL
│  └─ char 'Z'
├─ output : void
│  ├─ .value : char[]
│  │  └─ s2 : STRUCT_CALL
│  ├─ char[] " "
│  ├─ call len : i32
│  │  └─ s2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : char
│  ├─ c : char
│  └─ call pop : char
│     └─ s2 : STRUCT_CALL
├─ output : void
│  ├─ c : char
│  ├─ char[] " "
│  ├─ .value : char[]
│  │  └─ s2 : STRUCT_CALL
│  ├─ char[] " "
│  ├─ call len : i32
│  │  └─ s2 : STRUCT_CALL
│  └─ char[] "\n"
├─ call pop : char
│  └─ s2 : STRUCT_CALL
├─ call pop : char
│  └─ s2 : STRUCT_CALL
├─ = : char
│  ├─ c2 : char
│  └─ call pop : char
│     └─ s2 : STRUCT_CALL
├─ output : void
│  ├─ cast : i32
│  │  └─ c2 : char
│  ├─ char[] " "
│  ├─ call len : i32
│  │  └─ s2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s3 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "hello"
├─ call join : void
│  ├─ s3 : STRUCT_CALL
│  └─ char[] " world"
├─ output : void
│  ├─ .value : char[]
│  │  └─ s3 : STRUCT_CALL
│  └─ char[] "\n"
├─ call clear : void
│  └─ s3 : STRUCT_CALL
└─ output : void
   ├─ call len : i32
   │  └─ s3 : STRUCT_CALL
   ├─ char[] " "
   ├─ call empty : bool
   │  └─ s3 : STRUCT_CALL
   └─ char[] "\n"
```

```out
abc 3
hello world this is a longer string
0 True
XYZ 3
Z XY 2
0 0
hello world
0 True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }

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
@str.15 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%.*s%.*s%d%.*s\00", align 1
@str.18 = private unnamed_addr constant [36 x i8] c"hello world this is a longer string\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.21 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.22 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.24 = private unnamed_addr constant [13 x i8] c"%d%.*s%s%.*s\00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.26 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.27 = private unnamed_addr constant [15 x i8] c"%.*s%.*s%d%.*s\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.29 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.30 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.31 = private unnamed_addr constant [21 x i8] c"%c%.*s%.*s%.*s%d%.*s\00", align 1
@str.32 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.33 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.34 = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1
@str.35 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.36 = private unnamed_addr constant [7 x i8] c" world\00", align 1
@str.37 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.38 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.39 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.40 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.41 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.42 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.43 = private unnamed_addr constant [13 x i8] c"%d%.*s%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
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
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
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

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
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
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
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

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
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
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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

endif:                                            ; preds = %entry
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

then:                                             ; preds = %entry
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

while.cond:                                       ; preds = %while.body, %endif7
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
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
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
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
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
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
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
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
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
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
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
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
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
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
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
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
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
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
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
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  call void @"String.=.array"(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i64 3 })
  %value = getelementptr %String, %String* %s, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %str.len = extractvalue { i8*, i64 } %value1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %value1, 0
  %call2 = call i32 @String.len(%String* %s)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0), i32 %call2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0))
  call void @"String.=.array"(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str.18, i32 0, i32 0), i64 35 })
  %value3 = getelementptr %String, %String* %s, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %str.len5 = extractvalue { i8*, i64 } %value4, 1
  %len326 = trunc i64 %str.len5 to i32
  %str.data7 = extractvalue { i8*, i64 } %value4, 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.20, i32 0, i32 0), i32 %len326, i8* %str.data7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  call void @"String.=.array"(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.21, i32 0, i32 0), i64 0 })
  %call8 = call i32 @String.len(%String* %s)
  %call9 = call i1 @String.empty(%String* %s)
  %bool_str = select i1 %call9, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.24, i32 0, i32 0), i32 %call8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %s2 = alloca %String, align 8
  %call10 = call %String @String.create()
  store %String %call10, %String* %s2, align 8
  call void @String.push(%String* %s2, i8 88)
  call void @String.push(%String* %s2, i8 89)
  call void @String.push(%String* %s2, i8 90)
  %value11 = getelementptr %String, %String* %s2, i32 0, i32 0
  %value12 = load { i8*, i64 }, { i8*, i64 }* %value11, align 8
  %str.len13 = extractvalue { i8*, i64 } %value12, 1
  %len3214 = trunc i64 %str.len13 to i32
  %str.data15 = extractvalue { i8*, i64 } %value12, 0
  %call16 = call i32 @String.len(%String* %s2)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt.27, i32 0, i32 0), i32 %len3214, i8* %str.data15, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0), i32 %call16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.26, i32 0, i32 0))
  %c = alloca i8, align 1
  %call17 = call i8 @String.pop(%String* %s2)
  store i8 %call17, i8* %c, align 1
  %c18 = load i8, i8* %c, align 1
  %c2i = sext i8 %c18 to i32
  %value19 = getelementptr %String, %String* %s2, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %str.len21 = extractvalue { i8*, i64 } %value20, 1
  %len3222 = trunc i64 %str.len21 to i32
  %str.data23 = extractvalue { i8*, i64 } %value20, 0
  %call24 = call i32 @String.len(%String* %s2)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt.31, i32 0, i32 0), i32 %c2i, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0), i32 %len3222, i8* %str.data23, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.29, i32 0, i32 0), i32 %call24, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.30, i32 0, i32 0))
  %call25 = call i8 @String.pop(%String* %s2)
  %call26 = call i8 @String.pop(%String* %s2)
  %c2 = alloca i8, align 1
  %call27 = call i8 @String.pop(%String* %s2)
  store i8 %call27, i8* %c2, align 1
  %c228 = load i8, i8* %c2, align 1
  %cast = sext i8 %c228 to i32
  %call29 = call i32 @String.len(%String* %s2)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.34, i32 0, i32 0), i32 %cast, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.32, i32 0, i32 0), i32 %call29, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.33, i32 0, i32 0))
  %s3 = alloca %String, align 8
  %call30 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.35, i32 0, i32 0), i64 5 })
  store %String %call30, %String* %s3, align 8
  call void @String.join(%String* %s3, { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.36, i32 0, i32 0), i64 6 })
  %value31 = getelementptr %String, %String* %s3, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %str.len33 = extractvalue { i8*, i64 } %value32, 1
  %len3234 = trunc i64 %str.len33 to i32
  %str.data35 = extractvalue { i8*, i64 } %value32, 0
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.38, i32 0, i32 0), i32 %len3234, i8* %str.data35, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.37, i32 0, i32 0))
  call void @String.clear(%String* %s3)
  %call36 = call i32 @String.len(%String* %s3)
  %call37 = call i1 @String.empty(%String* %s3)
  %bool_str38 = select i1 %call37, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.40, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.41, i32 0, i32 0)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.43, i32 0, i32 0), i32 %call36, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.39, i32 0, i32 0), i8* %bool_str38, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.42, i32 0, i32 0))
  call void @String.drop(%String* %s3)
  call void @String.drop(%String* %s2)
  call void @String.drop(%String* %s)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — find, contains, starts_with and ends_with

```ura
// string/003.ura - find, contains, starts_with and ends_with
use "@/header"

main():
    s String = String::from("hello world")

    output(s.find("world"), "\n")
    output(s.find("hello"), "\n")
    output(s.find("xyz"), "\n")
    output(s.find(""), "\n")
    output(s.find("d"), "\n")

    output(s.contains("world"), "\n")
    output(s.contains("xyz"), "\n")

    output(s.starts_with("hello"), "\n")
    output(s.starts_with("world"), "\n")
    output(s.starts_with(""), "\n")

    output(s.ends_with("world"), "\n")
    output(s.ends_with("hello"), "\n")
    output(s.ends_with("d"), "\n")

    empty String = String::create()
    output(empty.find("x"), "\n")
    output(empty.contains("x"), "\n")
    output(empty.starts_with(""), "\n")
    output(empty.ends_with(""), "\n")
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

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "hello world"
├─ output : void
│  ├─ call find : i32
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "world"
│  └─ char[] "\n"
├─ output : void
│  ├─ call find : i32
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "hello"
│  └─ char[] "\n"
├─ output : void
│  ├─ call find : i32
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "xyz"
│  └─ char[] "\n"
├─ output : void
│  ├─ call find : i32
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] ""
│  └─ char[] "\n"
├─ output : void
│  ├─ call find : i32
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "d"
│  └─ char[] "\n"
├─ output : void
│  ├─ call contains : bool
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "world"
│  └─ char[] "\n"
├─ output : void
│  ├─ call contains : bool
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "xyz"
│  └─ char[] "\n"
├─ output : void
│  ├─ call starts_with : bool
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "hello"
│  └─ char[] "\n"
├─ output : void
│  ├─ call starts_with : bool
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "world"
│  └─ char[] "\n"
├─ output : void
│  ├─ call starts_with : bool
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] ""
│  └─ char[] "\n"
├─ output : void
│  ├─ call ends_with : bool
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "world"
│  └─ char[] "\n"
├─ output : void
│  ├─ call ends_with : bool
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "hello"
│  └─ char[] "\n"
├─ output : void
│  ├─ call ends_with : bool
│  │  ├─ s : STRUCT_CALL
│  │  └─ char[] "d"
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ empty : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ output : void
│  ├─ call find : i32
│  │  ├─ empty : STRUCT_CALL
│  │  └─ char[] "x"
│  └─ char[] "\n"
├─ output : void
│  ├─ call contains : bool
│  │  ├─ empty : STRUCT_CALL
│  │  └─ char[] "x"
│  └─ char[] "\n"
├─ output : void
│  ├─ call starts_with : bool
│  │  ├─ empty : STRUCT_CALL
│  │  └─ char[] ""
│  └─ char[] "\n"
└─ output : void
   ├─ call ends_with : bool
   │  ├─ empty : STRUCT_CALL
   │  └─ char[] ""
   └─ char[] "\n"
```

```out
6
0
-1
0
10
True
False
True
False
True
True
False
True
-1
False
True
True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }

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
@str.15 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
@str.16 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.18 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.21 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.23 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.24 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.27 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.30 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.31 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.32 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.33 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@true_str.34 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.35 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.36 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.37 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.38 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@true_str.39 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.40 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.41 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.42 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.43 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@true_str.44 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.45 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.46 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.47 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.48 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@true_str.49 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.50 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.51 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.52 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.53 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@true_str.54 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.55 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.56 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.57 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.58 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@true_str.59 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.60 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.61 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.62 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.63 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@true_str.64 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.65 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.66 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.67 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.68 = private unnamed_addr constant [2 x i8] c"x\00", align 1
@str.69 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.70 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.71 = private unnamed_addr constant [2 x i8] c"x\00", align 1
@true_str.72 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.73 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.74 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.75 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.76 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@true_str.77 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.78 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.79 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.80 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.81 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@true_str.82 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.83 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.84 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.85 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
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
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
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

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
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
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
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

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
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
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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

endif:                                            ; preds = %entry
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

then:                                             ; preds = %entry
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

while.cond:                                       ; preds = %while.body, %endif7
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
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
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
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
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
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
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
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
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
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
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
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
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
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
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
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
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
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
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
  %s = alloca %String, align 8
  %call = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.15, i32 0, i32 0), i64 11 })
  store %String %call, %String* %s, align 8
  %call1 = call i32 @String.find(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.16, i32 0, i32 0), i64 5 })
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %call1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0))
  %call2 = call i32 @String.find(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.18, i32 0, i32 0), i64 5 })
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.20, i32 0, i32 0), i32 %call2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  %call3 = call i32 @String.find(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0), i64 3 })
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.23, i32 0, i32 0), i32 %call3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  %call4 = call i32 @String.find(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.24, i32 0, i32 0), i64 0 })
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.26, i32 0, i32 0), i32 %call4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %call5 = call i32 @String.find(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.27, i32 0, i32 0), i64 1 })
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.29, i32 0, i32 0), i32 %call5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %call6 = call i1 @String.contains(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.30, i32 0, i32 0), i64 5 })
  %bool_str = select i1 %call6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.32, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.31, i32 0, i32 0))
  %call7 = call i1 @String.contains(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.33, i32 0, i32 0), i64 3 })
  %bool_str8 = select i1 %call7, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.34, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.35, i32 0, i32 0)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.37, i32 0, i32 0), i8* %bool_str8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.36, i32 0, i32 0))
  %call9 = call i1 @String.starts_with(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.38, i32 0, i32 0), i64 5 })
  %bool_str10 = select i1 %call9, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.39, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.40, i32 0, i32 0)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.42, i32 0, i32 0), i8* %bool_str10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.41, i32 0, i32 0))
  %call11 = call i1 @String.starts_with(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.43, i32 0, i32 0), i64 5 })
  %bool_str12 = select i1 %call11, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.44, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.45, i32 0, i32 0)
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.47, i32 0, i32 0), i8* %bool_str12, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
  %call13 = call i1 @String.starts_with(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.48, i32 0, i32 0), i64 0 })
  %bool_str14 = select i1 %call13, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.49, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.50, i32 0, i32 0)
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.52, i32 0, i32 0), i8* %bool_str14, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.51, i32 0, i32 0))
  %call15 = call i1 @String.ends_with(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.53, i32 0, i32 0), i64 5 })
  %bool_str16 = select i1 %call15, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.54, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.55, i32 0, i32 0)
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.57, i32 0, i32 0), i8* %bool_str16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.56, i32 0, i32 0))
  %call17 = call i1 @String.ends_with(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.58, i32 0, i32 0), i64 5 })
  %bool_str18 = select i1 %call17, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.59, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.60, i32 0, i32 0)
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.62, i32 0, i32 0), i8* %bool_str18, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.61, i32 0, i32 0))
  %call19 = call i1 @String.ends_with(%String* %s, { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.63, i32 0, i32 0), i64 1 })
  %bool_str20 = select i1 %call19, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.64, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.65, i32 0, i32 0)
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.67, i32 0, i32 0), i8* %bool_str20, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.66, i32 0, i32 0))
  %empty = alloca %String, align 8
  %call21 = call %String @String.create()
  store %String %call21, %String* %empty, align 8
  %call22 = call i32 @String.find(%String* %empty, { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.68, i32 0, i32 0), i64 1 })
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.70, i32 0, i32 0), i32 %call22, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.69, i32 0, i32 0))
  %call23 = call i1 @String.contains(%String* %empty, { i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.71, i32 0, i32 0), i64 1 })
  %bool_str24 = select i1 %call23, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.72, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.73, i32 0, i32 0)
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.75, i32 0, i32 0), i8* %bool_str24, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.74, i32 0, i32 0))
  %call25 = call i1 @String.starts_with(%String* %empty, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.76, i32 0, i32 0), i64 0 })
  %bool_str26 = select i1 %call25, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.77, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.78, i32 0, i32 0)
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.80, i32 0, i32 0), i8* %bool_str26, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.79, i32 0, i32 0))
  %call27 = call i1 @String.ends_with(%String* %empty, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.81, i32 0, i32 0), i64 0 })
  %bool_str28 = select i1 %call27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.82, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.83, i32 0, i32 0)
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.85, i32 0, i32 0), i8* %bool_str28, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.84, i32 0, i32 0))
  call void @String.drop(%String* %empty)
  call void @String.drop(%String* %s)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — transforms: upper, lower, substr, trim, replace, repeat, reverse

```ura
// string/004.ura - transforms: upper, lower, substr, trim, replace, repeat, reverse
use "@/header"

main():
    s String = String::from("Hello World")

    u String = s.upper()
    output(u.value, "\n")

    l String = s.lower()
    output(l.value, "\n")

    sub String = s.substr(6, 5)
    output(sub.value, "\n")

    sub2 String = s.substr(0, 5)
    output(sub2.value, "\n")

    sub3 String = s.substr(0, 100)
    output(sub3.value, "\n")

    sub4 String = s.substr(-1, 5)
    output(sub4.len(), "\n")

    t1 String = String::from("   hello   ")
    t2 String = t1.trim()
    output(t2.value, "\n")

    t3 String = String::from("hello")
    t4 String = t3.trim()
    output(t4.value, "\n")

    t5 String = String::from("   ")
    t6 String = t5.trim()
    output(t6.len(), "\n")

    r1 String = String::from("foo bar foo baz")
    r2 String = r1.replace("foo", "qux")
    output(r2.value, "\n")

    r3 String = r1.replace("xyz", "qux")
    output(r3.value, "\n")

    r4 String = r1.replace("", "qux")
    output(r4.value, "\n")

    rep String = String::from("ab")
    rep2 String = rep.repeat(3)
    output(rep2.value, "\n")

    rep3 String = rep.repeat(0)
    output(rep3.len(), "\n")

    rev String = String::from("abcde")
    rev2 String = rev.reverse()
    output(rev2.value, "\n")

    rev3 String = String::from("a")
    rev4 String = rev3.reverse()
    output(rev4.value, "\n")
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

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "Hello World"
├─ = : STRUCT_CALL
│  ├─ u : STRUCT_CALL
│  └─ call upper : STRUCT_CALL
│     └─ s : STRUCT_CALL
├─ output : void
│  ├─ .value : char[]
│  │  └─ u : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ l : STRUCT_CALL
│  └─ call lower : STRUCT_CALL
│     └─ s : STRUCT_CALL
├─ output : void
│  ├─ .value : char[]
│  │  └─ l : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ sub : STRUCT_CALL
│  └─ call substr : STRUCT_CALL
│     ├─ s : STRUCT_CALL
│     ├─ int 6
│     └─ int 5
├─ output : void
│  ├─ .value : char[]
│  │  └─ sub : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ sub2 : STRUCT_CALL
│  └─ call substr : STRUCT_CALL
│     ├─ s : STRUCT_CALL
│     ├─ int 0
│     └─ int 5
├─ output : void
│  ├─ .value : char[]
│  │  └─ sub2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ sub3 : STRUCT_CALL
│  └─ call substr : STRUCT_CALL
│     ├─ s : STRUCT_CALL
│     ├─ int 0
│     └─ int 100
├─ output : void
│  ├─ .value : char[]
│  │  └─ sub3 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ sub4 : STRUCT_CALL
│  └─ call substr : STRUCT_CALL
│     ├─ s : STRUCT_CALL
│     ├─ - : i32
│     │  ├─ int 0
│     │  └─ int 1
│     └─ int 5
├─ output : void
│  ├─ call len : i32
│  │  └─ sub4 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ t1 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "   hello   "
├─ = : STRUCT_CALL
│  ├─ t2 : STRUCT_CALL
│  └─ call trim : STRUCT_CALL
│     └─ t1 : STRUCT_CALL
├─ output : void
│  ├─ .value : char[]
│  │  └─ t2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ t3 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "hello"
├─ = : STRUCT_CALL
│  ├─ t4 : STRUCT_CALL
│  └─ call trim : STRUCT_CALL
│     └─ t3 : STRUCT_CALL
├─ output : void
│  ├─ .value : char[]
│  │  └─ t4 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ t5 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "   "
├─ = : STRUCT_CALL
│  ├─ t6 : STRUCT_CALL
│  └─ call trim : STRUCT_CALL
│     └─ t5 : STRUCT_CALL
├─ output : void
│  ├─ call len : i32
│  │  └─ t6 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ r1 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "foo bar foo baz"
├─ = : STRUCT_CALL
│  ├─ r2 : STRUCT_CALL
│  └─ call replace : STRUCT_CALL
│     ├─ r1 : STRUCT_CALL
│     ├─ char[] "foo"
│     └─ char[] "qux"
├─ output : void
│  ├─ .value : char[]
│  │  └─ r2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ r3 : STRUCT_CALL
│  └─ call replace : STRUCT_CALL
│     ├─ r1 : STRUCT_CALL
│     ├─ char[] "xyz"
│     └─ char[] "qux"
├─ output : void
│  ├─ .value : char[]
│  │  └─ r3 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ r4 : STRUCT_CALL
│  └─ call replace : STRUCT_CALL
│     ├─ r1 : STRUCT_CALL
│     ├─ char[] ""
│     └─ char[] "qux"
├─ output : void
│  ├─ .value : char[]
│  │  └─ r4 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ rep : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "ab"
├─ = : STRUCT_CALL
│  ├─ rep2 : STRUCT_CALL
│  └─ call repeat : STRUCT_CALL
│     ├─ rep : STRUCT_CALL
│     └─ int 3
├─ output : void
│  ├─ .value : char[]
│  │  └─ rep2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ rep3 : STRUCT_CALL
│  └─ call repeat : STRUCT_CALL
│     ├─ rep : STRUCT_CALL
│     └─ int 0
├─ output : void
│  ├─ call len : i32
│  │  └─ rep3 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ rev : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "abcde"
├─ = : STRUCT_CALL
│  ├─ rev2 : STRUCT_CALL
│  └─ call reverse : STRUCT_CALL
│     └─ rev : STRUCT_CALL
├─ output : void
│  ├─ .value : char[]
│  │  └─ rev2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ rev3 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "a"
├─ = : STRUCT_CALL
│  ├─ rev4 : STRUCT_CALL
│  └─ call reverse : STRUCT_CALL
│     └─ rev3 : STRUCT_CALL
└─ output : void
   ├─ .value : char[]
   │  └─ rev4 : STRUCT_CALL
   └─ char[] "\n"
```

```out
HELLO WORLD
hello world
World
Hello
Hello World
0
hello
hello
0
qux bar qux baz
foo bar foo baz
foo bar foo baz
ababab
0
edcba
a
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }

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
@str.15 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.18 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.24 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.27 = private unnamed_addr constant [12 x i8] c"   hello   \00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.30 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.31 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.32 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.33 = private unnamed_addr constant [4 x i8] c"   \00", align 1
@str.34 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.35 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.36 = private unnamed_addr constant [16 x i8] c"foo bar foo baz\00", align 1
@str.37 = private unnamed_addr constant [4 x i8] c"foo\00", align 1
@str.38 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.39 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.40 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.41 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.42 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.43 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.44 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.45 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.46 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.47 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.48 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.49 = private unnamed_addr constant [3 x i8] c"ab\00", align 1
@str.50 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.51 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.52 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.53 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.54 = private unnamed_addr constant [6 x i8] c"abcde\00", align 1
@str.55 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.56 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.57 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@str.58 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.59 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
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
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
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

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
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
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
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

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
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
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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

endif:                                            ; preds = %entry
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

then:                                             ; preds = %entry
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

while.cond:                                       ; preds = %while.body, %endif7
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
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
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
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
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
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
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
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
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
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
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
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
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
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
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
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
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
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
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
  %s = alloca %String, align 8
  %call = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.15, i32 0, i32 0), i64 11 })
  store %String %call, %String* %s, align 8
  %u = alloca %String, align 8
  %call1 = call %String @String.upper(%String* %s)
  store %String %call1, %String* %u, align 8
  %value = getelementptr %String, %String* %u, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %str.len = extractvalue { i8*, i64 } %value2, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %value2, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %l = alloca %String, align 8
  %call3 = call %String @String.lower(%String* %s)
  store %String %call3, %String* %l, align 8
  %value4 = getelementptr %String, %String* %l, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value4, align 8
  %str.len6 = extractvalue { i8*, i64 } %value5, 1
  %len327 = trunc i64 %str.len6 to i32
  %str.data8 = extractvalue { i8*, i64 } %value5, 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.18, i32 0, i32 0), i32 %len327, i8* %str.data8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0))
  %sub = alloca %String, align 8
  %call9 = call %String @String.substr(%String* %s, i32 6, i32 5)
  store %String %call9, %String* %sub, align 8
  %value10 = getelementptr %String, %String* %sub, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  %str.len12 = extractvalue { i8*, i64 } %value11, 1
  %len3213 = trunc i64 %str.len12 to i32
  %str.data14 = extractvalue { i8*, i64 } %value11, 0
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.20, i32 0, i32 0), i32 %len3213, i8* %str.data14, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  %sub2 = alloca %String, align 8
  %call15 = call %String @String.substr(%String* %s, i32 0, i32 5)
  store %String %call15, %String* %sub2, align 8
  %value16 = getelementptr %String, %String* %sub2, i32 0, i32 0
  %value17 = load { i8*, i64 }, { i8*, i64 }* %value16, align 8
  %str.len18 = extractvalue { i8*, i64 } %value17, 1
  %len3219 = trunc i64 %str.len18 to i32
  %str.data20 = extractvalue { i8*, i64 } %value17, 0
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.22, i32 0, i32 0), i32 %len3219, i8* %str.data20, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  %sub3 = alloca %String, align 8
  %call21 = call %String @String.substr(%String* %s, i32 0, i32 100)
  store %String %call21, %String* %sub3, align 8
  %value22 = getelementptr %String, %String* %sub3, i32 0, i32 0
  %value23 = load { i8*, i64 }, { i8*, i64 }* %value22, align 8
  %str.len24 = extractvalue { i8*, i64 } %value23, 1
  %len3225 = trunc i64 %str.len24 to i32
  %str.data26 = extractvalue { i8*, i64 } %value23, 0
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.24, i32 0, i32 0), i32 %len3225, i8* %str.data26, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %sub4 = alloca %String, align 8
  %call27 = call %String @String.substr(%String* %s, i32 -1, i32 5)
  store %String %call27, %String* %sub4, align 8
  %call28 = call i32 @String.len(%String* %sub4)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.26, i32 0, i32 0), i32 %call28, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %t1 = alloca %String, align 8
  %call29 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.27, i32 0, i32 0), i64 11 })
  store %String %call29, %String* %t1, align 8
  %t2 = alloca %String, align 8
  %call30 = call %String @String.trim(%String* %t1)
  store %String %call30, %String* %t2, align 8
  %value31 = getelementptr %String, %String* %t2, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %str.len33 = extractvalue { i8*, i64 } %value32, 1
  %len3234 = trunc i64 %str.len33 to i32
  %str.data35 = extractvalue { i8*, i64 } %value32, 0
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.29, i32 0, i32 0), i32 %len3234, i8* %str.data35, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %t3 = alloca %String, align 8
  %call36 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.30, i32 0, i32 0), i64 5 })
  store %String %call36, %String* %t3, align 8
  %t4 = alloca %String, align 8
  %call37 = call %String @String.trim(%String* %t3)
  store %String %call37, %String* %t4, align 8
  %value38 = getelementptr %String, %String* %t4, i32 0, i32 0
  %value39 = load { i8*, i64 }, { i8*, i64 }* %value38, align 8
  %str.len40 = extractvalue { i8*, i64 } %value39, 1
  %len3241 = trunc i64 %str.len40 to i32
  %str.data42 = extractvalue { i8*, i64 } %value39, 0
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.32, i32 0, i32 0), i32 %len3241, i8* %str.data42, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.31, i32 0, i32 0))
  %t5 = alloca %String, align 8
  %call43 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.33, i32 0, i32 0), i64 3 })
  store %String %call43, %String* %t5, align 8
  %t6 = alloca %String, align 8
  %call44 = call %String @String.trim(%String* %t5)
  store %String %call44, %String* %t6, align 8
  %call45 = call i32 @String.len(%String* %t6)
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.35, i32 0, i32 0), i32 %call45, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.34, i32 0, i32 0))
  %r1 = alloca %String, align 8
  %call46 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.36, i32 0, i32 0), i64 15 })
  store %String %call46, %String* %r1, align 8
  %r2 = alloca %String, align 8
  %call47 = call %String @String.replace(%String* %r1, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.37, i32 0, i32 0), i64 3 }, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.38, i32 0, i32 0), i64 3 })
  store %String %call47, %String* %r2, align 8
  %value48 = getelementptr %String, %String* %r2, i32 0, i32 0
  %value49 = load { i8*, i64 }, { i8*, i64 }* %value48, align 8
  %str.len50 = extractvalue { i8*, i64 } %value49, 1
  %len3251 = trunc i64 %str.len50 to i32
  %str.data52 = extractvalue { i8*, i64 } %value49, 0
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.40, i32 0, i32 0), i32 %len3251, i8* %str.data52, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.39, i32 0, i32 0))
  %r3 = alloca %String, align 8
  %call53 = call %String @String.replace(%String* %r1, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.41, i32 0, i32 0), i64 3 }, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.42, i32 0, i32 0), i64 3 })
  store %String %call53, %String* %r3, align 8
  %value54 = getelementptr %String, %String* %r3, i32 0, i32 0
  %value55 = load { i8*, i64 }, { i8*, i64 }* %value54, align 8
  %str.len56 = extractvalue { i8*, i64 } %value55, 1
  %len3257 = trunc i64 %str.len56 to i32
  %str.data58 = extractvalue { i8*, i64 } %value55, 0
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.44, i32 0, i32 0), i32 %len3257, i8* %str.data58, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.43, i32 0, i32 0))
  %r4 = alloca %String, align 8
  %call59 = call %String @String.replace(%String* %r1, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.45, i32 0, i32 0), i64 0 }, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.46, i32 0, i32 0), i64 3 })
  store %String %call59, %String* %r4, align 8
  %value60 = getelementptr %String, %String* %r4, i32 0, i32 0
  %value61 = load { i8*, i64 }, { i8*, i64 }* %value60, align 8
  %str.len62 = extractvalue { i8*, i64 } %value61, 1
  %len3263 = trunc i64 %str.len62 to i32
  %str.data64 = extractvalue { i8*, i64 } %value61, 0
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.48, i32 0, i32 0), i32 %len3263, i8* %str.data64, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.47, i32 0, i32 0))
  %rep = alloca %String, align 8
  %call65 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.49, i32 0, i32 0), i64 2 })
  store %String %call65, %String* %rep, align 8
  %rep2 = alloca %String, align 8
  %call66 = call %String @String.repeat(%String* %rep, i32 3)
  store %String %call66, %String* %rep2, align 8
  %value67 = getelementptr %String, %String* %rep2, i32 0, i32 0
  %value68 = load { i8*, i64 }, { i8*, i64 }* %value67, align 8
  %str.len69 = extractvalue { i8*, i64 } %value68, 1
  %len3270 = trunc i64 %str.len69 to i32
  %str.data71 = extractvalue { i8*, i64 } %value68, 0
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.51, i32 0, i32 0), i32 %len3270, i8* %str.data71, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.50, i32 0, i32 0))
  %rep3 = alloca %String, align 8
  %call72 = call %String @String.repeat(%String* %rep, i32 0)
  store %String %call72, %String* %rep3, align 8
  %call73 = call i32 @String.len(%String* %rep3)
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.53, i32 0, i32 0), i32 %call73, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.52, i32 0, i32 0))
  %rev = alloca %String, align 8
  %call74 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.54, i32 0, i32 0), i64 5 })
  store %String %call74, %String* %rev, align 8
  %rev2 = alloca %String, align 8
  %call75 = call %String @String.reverse(%String* %rev)
  store %String %call75, %String* %rev2, align 8
  %value76 = getelementptr %String, %String* %rev2, i32 0, i32 0
  %value77 = load { i8*, i64 }, { i8*, i64 }* %value76, align 8
  %str.len78 = extractvalue { i8*, i64 } %value77, 1
  %len3279 = trunc i64 %str.len78 to i32
  %str.data80 = extractvalue { i8*, i64 } %value77, 0
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.56, i32 0, i32 0), i32 %len3279, i8* %str.data80, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.55, i32 0, i32 0))
  %rev3 = alloca %String, align 8
  %call81 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.57, i32 0, i32 0), i64 1 })
  store %String %call81, %String* %rev3, align 8
  %rev4 = alloca %String, align 8
  %call82 = call %String @String.reverse(%String* %rev3)
  store %String %call82, %String* %rev4, align 8
  %value83 = getelementptr %String, %String* %rev4, i32 0, i32 0
  %value84 = load { i8*, i64 }, { i8*, i64 }* %value83, align 8
  %str.len85 = extractvalue { i8*, i64 } %value84, 1
  %len3286 = trunc i64 %str.len85 to i32
  %str.data87 = extractvalue { i8*, i64 } %value84, 0
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.59, i32 0, i32 0), i32 %len3286, i8* %str.data87, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.58, i32 0, i32 0))
  call void @String.drop(%String* %rev4)
  call void @String.drop(%String* %rev3)
  call void @String.drop(%String* %rev2)
  call void @String.drop(%String* %rev)
  call void @String.drop(%String* %rep3)
  call void @String.drop(%String* %rep2)
  call void @String.drop(%String* %rep)
  call void @String.drop(%String* %r4)
  call void @String.drop(%String* %r3)
  call void @String.drop(%String* %r2)
  call void @String.drop(%String* %r1)
  call void @String.drop(%String* %t6)
  call void @String.drop(%String* %t5)
  call void @String.drop(%String* %t4)
  call void @String.drop(%String* %t3)
  call void @String.drop(%String* %t2)
  call void @String.drop(%String* %t1)
  call void @String.drop(%String* %sub4)
  call void @String.drop(%String* %sub3)
  call void @String.drop(%String* %sub2)
  call void @String.drop(%String* %sub)
  call void @String.drop(%String* %l)
  call void @String.drop(%String* %u)
  call void @String.drop(%String* %s)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — operators: + += == != < > against String and against a literal

```ura
// string/005.ura - operators: + += == != < > against String and against a literal
use "@/header"

main():
    a String = String::from("hello")
    b String = String::from(" world")
    c String = a + b
    output(c.value, "\n")

    d String = a + " there"
    output(d.value, "\n")

    a += b
    output(a.value, "\n")

    e String = String::from("test")
    e += "ing"
    output(e.value, "\n")

    x String = String::from("abc")
    y String = String::from("abc")
    z String = String::from("xyz")

    output(x == y, "\n")
    output(x == z, "\n")
    output(x != z, "\n")
    output(x != y, "\n")
    output(x < z, "\n")
    output(z < x, "\n")
    output(z > x, "\n")
    output(x > z, "\n")

    output(x == "abc", "\n")
    output(x == "xyz", "\n")
    output(x != "xyz", "\n")
    output(x != "abc", "\n")

    f String = String::create()
    g String = String::create()
    f = y
    output(f.value, " ", f == y, "\n")
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

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "hello"
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] " world"
├─ = : STRUCT_CALL
│  ├─ c : STRUCT_CALL
│  └─ + : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ output : void
│  ├─ .value : char[]
│  │  └─ c : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ d : STRUCT_CALL
│  └─ + : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ char[] " there"
├─ output : void
│  ├─ .value : char[]
│  │  └─ d : STRUCT_CALL
│  └─ char[] "\n"
├─ += : void
│  ├─ a : STRUCT_CALL
│  └─ b : STRUCT_CALL
├─ output : void
│  ├─ .value : char[]
│  │  └─ a : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ e : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "test"
├─ += : void
│  ├─ e : STRUCT_CALL
│  └─ char[] "ing"
├─ output : void
│  ├─ .value : char[]
│  │  └─ e : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ x : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "abc"
├─ = : STRUCT_CALL
│  ├─ y : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "abc"
├─ = : STRUCT_CALL
│  ├─ z : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "xyz"
├─ output : void
│  ├─ == : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ y : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ == : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ z : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ != : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ z : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ != : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ y : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ < : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ z : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ < : bool
│  │  ├─ z : STRUCT_CALL
│  │  └─ x : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ > : bool
│  │  ├─ z : STRUCT_CALL
│  │  └─ x : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ > : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ z : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ == : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ char[] "abc"
│  └─ char[] "\n"
├─ output : void
│  ├─ == : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ char[] "xyz"
│  └─ char[] "\n"
├─ output : void
│  ├─ != : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ char[] "xyz"
│  └─ char[] "\n"
├─ output : void
│  ├─ != : bool
│  │  ├─ x : STRUCT_CALL
│  │  └─ char[] "abc"
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ f : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ g : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : void
│  ├─ f : STRUCT_CALL
│  └─ y : STRUCT_CALL
└─ output : void
   ├─ .value : char[]
   │  └─ f : STRUCT_CALL
   ├─ char[] " "
   ├─ == : bool
   │  ├─ f : STRUCT_CALL
   │  └─ y : STRUCT_CALL
   └─ char[] "\n"
```

```out
hello world
hello there
hello world
testing
True
False
True
False
True
False
True
False
True
False
True
False
abc True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }

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
@str.15 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.16 = private unnamed_addr constant [7 x i8] c" world\00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.18 = private unnamed_addr constant [7 x i8] c" there\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.23 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@str.24 = private unnamed_addr constant [4 x i8] c"ing\00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.27 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.28 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.29 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.30 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.31 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.32 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.33 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.34 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.35 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.36 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.37 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.38 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.39 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.40 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.41 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.42 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.43 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.44 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.45 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.46 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.47 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.48 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.49 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.50 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.51 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.52 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.53 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.54 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.55 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.56 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.57 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.58 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.59 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.60 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@true_str.61 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.62 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.63 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.64 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.65 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@true_str.66 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.67 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.68 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.69 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.70 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@true_str.71 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.72 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.73 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.74 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.75 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@true_str.76 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.77 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.78 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.79 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.80 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.81 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.82 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.83 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.84 = private unnamed_addr constant [15 x i8] c"%.*s%.*s%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
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
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
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

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
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
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
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

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
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
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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

endif:                                            ; preds = %entry
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

then:                                             ; preds = %entry
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

while.cond:                                       ; preds = %while.body, %endif7
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
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
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
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
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
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
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
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
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
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
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
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
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
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
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
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
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
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
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
  %a = alloca %String, align 8
  %call = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i64 5 })
  store %String %call, %String* %a, align 8
  %b = alloca %String, align 8
  %call1 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.16, i32 0, i32 0), i64 6 })
  store %String %call1, %String* %b, align 8
  %c = alloca %String, align 8
  %op = call %String @"String.+.ref.String"(%String* %a, %String* %b)
  store %String %op, %String* %c, align 8
  %value = getelementptr %String, %String* %c, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %str.len = extractvalue { i8*, i64 } %value2, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %value2, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0))
  %d = alloca %String, align 8
  %op3 = call %String @"String.+.array"(%String* %a, { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.18, i32 0, i32 0), i64 6 })
  store %String %op3, %String* %d, align 8
  %value4 = getelementptr %String, %String* %d, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value4, align 8
  %str.len6 = extractvalue { i8*, i64 } %value5, 1
  %len327 = trunc i64 %str.len6 to i32
  %str.data8 = extractvalue { i8*, i64 } %value5, 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.20, i32 0, i32 0), i32 %len327, i8* %str.data8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  call void @"String.+=.ref.String"(%String* %a, %String* %b)
  %value9 = getelementptr %String, %String* %a, i32 0, i32 0
  %value10 = load { i8*, i64 }, { i8*, i64 }* %value9, align 8
  %str.len11 = extractvalue { i8*, i64 } %value10, 1
  %len3212 = trunc i64 %str.len11 to i32
  %str.data13 = extractvalue { i8*, i64 } %value10, 0
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.22, i32 0, i32 0), i32 %len3212, i8* %str.data13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  %e = alloca %String, align 8
  %call14 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.23, i32 0, i32 0), i64 4 })
  store %String %call14, %String* %e, align 8
  call void @"String.+=.array"(%String* %e, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i64 3 })
  %value15 = getelementptr %String, %String* %e, i32 0, i32 0
  %value16 = load { i8*, i64 }, { i8*, i64 }* %value15, align 8
  %str.len17 = extractvalue { i8*, i64 } %value16, 1
  %len3218 = trunc i64 %str.len17 to i32
  %str.data19 = extractvalue { i8*, i64 } %value16, 0
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.26, i32 0, i32 0), i32 %len3218, i8* %str.data19, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %x = alloca %String, align 8
  %call20 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.27, i32 0, i32 0), i64 3 })
  store %String %call20, %String* %x, align 8
  %y = alloca %String, align 8
  %call21 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.28, i32 0, i32 0), i64 3 })
  store %String %call21, %String* %y, align 8
  %z = alloca %String, align 8
  %call22 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.29, i32 0, i32 0), i64 3 })
  store %String %call22, %String* %z, align 8
  %op23 = call i1 @"String.==.ref.String"(%String* %x, %String* %y)
  %bool_str = select i1 %op23, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.31, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.30, i32 0, i32 0))
  %op24 = call i1 @"String.==.ref.String"(%String* %x, %String* %z)
  %bool_str25 = select i1 %op24, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.32, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.33, i32 0, i32 0)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.35, i32 0, i32 0), i8* %bool_str25, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.34, i32 0, i32 0))
  %op26 = call i1 @"String.!=.ref.String"(%String* %x, %String* %z)
  %bool_str27 = select i1 %op26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.36, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.37, i32 0, i32 0)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.39, i32 0, i32 0), i8* %bool_str27, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.38, i32 0, i32 0))
  %op28 = call i1 @"String.!=.ref.String"(%String* %x, %String* %y)
  %bool_str29 = select i1 %op28, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.40, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.41, i32 0, i32 0)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.43, i32 0, i32 0), i8* %bool_str29, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.42, i32 0, i32 0))
  %op30 = call i1 @"String.<.ref.String"(%String* %x, %String* %z)
  %bool_str31 = select i1 %op30, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.44, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.45, i32 0, i32 0)
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.47, i32 0, i32 0), i8* %bool_str31, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
  %op32 = call i1 @"String.<.ref.String"(%String* %z, %String* %x)
  %bool_str33 = select i1 %op32, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.48, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.49, i32 0, i32 0)
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.51, i32 0, i32 0), i8* %bool_str33, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.50, i32 0, i32 0))
  %op34 = call i1 @"String.>.ref.String"(%String* %z, %String* %x)
  %bool_str35 = select i1 %op34, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.52, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.53, i32 0, i32 0)
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.55, i32 0, i32 0), i8* %bool_str35, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.54, i32 0, i32 0))
  %op36 = call i1 @"String.>.ref.String"(%String* %x, %String* %z)
  %bool_str37 = select i1 %op36, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.56, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.57, i32 0, i32 0)
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.59, i32 0, i32 0), i8* %bool_str37, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.58, i32 0, i32 0))
  %op38 = call i1 @"String.==.array"(%String* %x, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.60, i32 0, i32 0), i64 3 })
  %bool_str39 = select i1 %op38, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.61, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.62, i32 0, i32 0)
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.64, i32 0, i32 0), i8* %bool_str39, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.63, i32 0, i32 0))
  %op40 = call i1 @"String.==.array"(%String* %x, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.65, i32 0, i32 0), i64 3 })
  %bool_str41 = select i1 %op40, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.66, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.67, i32 0, i32 0)
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.69, i32 0, i32 0), i8* %bool_str41, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.68, i32 0, i32 0))
  %op42 = call i1 @"String.!=.array"(%String* %x, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.70, i32 0, i32 0), i64 3 })
  %bool_str43 = select i1 %op42, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.71, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.72, i32 0, i32 0)
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.74, i32 0, i32 0), i8* %bool_str43, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.73, i32 0, i32 0))
  %op44 = call i1 @"String.!=.array"(%String* %x, { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.75, i32 0, i32 0), i64 3 })
  %bool_str45 = select i1 %op44, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.76, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.77, i32 0, i32 0)
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.79, i32 0, i32 0), i8* %bool_str45, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.78, i32 0, i32 0))
  %f = alloca %String, align 8
  %call46 = call %String @String.create()
  store %String %call46, %String* %f, align 8
  %g = alloca %String, align 8
  %call47 = call %String @String.create()
  store %String %call47, %String* %g, align 8
  call void @"String.=.ref.String"(%String* %f, %String* %y)
  %value48 = getelementptr %String, %String* %f, i32 0, i32 0
  %value49 = load { i8*, i64 }, { i8*, i64 }* %value48, align 8
  %str.len50 = extractvalue { i8*, i64 } %value49, 1
  %len3251 = trunc i64 %str.len50 to i32
  %str.data52 = extractvalue { i8*, i64 } %value49, 0
  %op53 = call i1 @"String.==.ref.String"(%String* %f, %String* %y)
  %bool_str54 = select i1 %op53, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.81, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.82, i32 0, i32 0)
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt.84, i32 0, i32 0), i32 %len3251, i8* %str.data52, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.80, i32 0, i32 0), i8* %bool_str54, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.83, i32 0, i32 0))
  call void @String.drop(%String* %g)
  call void @String.drop(%String* %f)
  call void @String.drop(%String* %z)
  call void @String.drop(%String* %y)
  call void @String.drop(%String* %x)
  call void @String.drop(%String* %e)
  call void @String.drop(%String* %d)
  call void @String.drop(%String* %c)
  call void @String.drop(%String* %b)
  call void @String.drop(%String* %a)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — to_int, compare and chained transformations

```ura
// string/006.ura - to_int, compare and chained transformations
use "@/header"

main():
    s1 String = String::from("12345")
    output(s1.to_int(), "\n")

    s2 String = String::from("-99")
    output(s2.to_int(), "\n")

    s3 String = String::from("0")
    output(s3.to_int(), "\n")

    a String = String::from("apple")
    b String = String::from("banana")
    output(a.compare(ref b), "\n")
    output(b.compare(ref a), "\n")
    output(a.compare(ref a), "\n")

    s String = String::from("Hello World")
    r String = s.upper()
    r2 String = r.reverse()
    output(r2.value, "\n")

    big String = String::from("aaaa")
    i i32 = 0
    while i < 5:
        big.join("bbbb")
        i += 1
    output(big.len(), "\n")

    n1 String = String::from_int(1000000)
    output(n1.value, "\n")

    n2 String = String::from_int(-2000000)
    output(n2.value, "\n")
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

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ s1 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "12345"
├─ output : void
│  ├─ call to_int : i32
│  │  └─ s1 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s2 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "-99"
├─ output : void
│  ├─ call to_int : i32
│  │  └─ s2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s3 : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "0"
├─ output : void
│  ├─ call to_int : i32
│  │  └─ s3 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "apple"
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "banana"
├─ output : void
│  ├─ call compare : i32
│  │  ├─ a : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ call compare : i32
│  │  ├─ b : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ a : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ call compare : i32
│  │  ├─ a : STRUCT_CALL
│  │  └─ ref : STRUCT_CALL
│  │     └─ a : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "Hello World"
├─ = : STRUCT_CALL
│  ├─ r : STRUCT_CALL
│  └─ call upper : STRUCT_CALL
│     └─ s : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ r2 : STRUCT_CALL
│  └─ call reverse : STRUCT_CALL
│     └─ r : STRUCT_CALL
├─ output : void
│  ├─ .value : char[]
│  │  └─ r2 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ big : STRUCT_CALL
│  └─ call from : STRUCT_CALL
│     └─ char[] "aaaa"
├─ = : i32
│  ├─ i : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ i : i32
│  │  └─ int 5
│  ├─ call join : void
│  │  ├─ big : STRUCT_CALL
│  │  └─ char[] "bbbb"
│  └─ += : i32
│     ├─ i : i32
│     └─ int 1
├─ output : void
│  ├─ call len : i32
│  │  └─ big : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ n1 : STRUCT_CALL
│  └─ call from_int : STRUCT_CALL
│     └─ int 1000000
├─ output : void
│  ├─ .value : char[]
│  │  └─ n1 : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ n2 : STRUCT_CALL
│  └─ call from_int : STRUCT_CALL
│     └─ - : i32
│        ├─ int 0
│        └─ int 2000000
└─ output : void
   ├─ .value : char[]
   │  └─ n2 : STRUCT_CALL
   └─ char[] "\n"
```

```out
12345
-99
0
-1
1
0
DLROW OLLEH
24
1000000
-2000000
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }

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
@str.15 = private unnamed_addr constant [6 x i8] c"12345\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"-99\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.19 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"0\00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.23 = private unnamed_addr constant [6 x i8] c"apple\00", align 1
@str.24 = private unnamed_addr constant [7 x i8] c"banana\00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.27 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.28 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.29 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.30 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.31 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@str.32 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.33 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.34 = private unnamed_addr constant [5 x i8] c"aaaa\00", align 1
@str.35 = private unnamed_addr constant [5 x i8] c"bbbb\00", align 1
@str.36 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.37 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.38 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.39 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1
@str.40 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.41 = private unnamed_addr constant [9 x i8] c"%.*s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
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
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
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

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
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
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
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

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
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
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
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
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
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

endif:                                            ; preds = %entry
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

then:                                             ; preds = %entry
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

while.cond:                                       ; preds = %while.body, %endif7
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
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
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
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
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
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
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
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
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
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
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
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
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
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
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
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
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
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
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
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
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
  %s1 = alloca %String, align 8
  %call = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i64 5 })
  store %String %call, %String* %s1, align 8
  %call1 = call i32 @String.to_int(%String* %s1)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %call1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %s2 = alloca %String, align 8
  %call2 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i64 3 })
  store %String %call2, %String* %s2, align 8
  %call3 = call i32 @String.to_int(%String* %s2)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.19, i32 0, i32 0), i32 %call3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %s3 = alloca %String, align 8
  %call4 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0), i64 1 })
  store %String %call4, %String* %s3, align 8
  %call5 = call i32 @String.to_int(%String* %s3)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.22, i32 0, i32 0), i32 %call5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  %a = alloca %String, align 8
  %call6 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.23, i32 0, i32 0), i64 5 })
  store %String %call6, %String* %a, align 8
  %b = alloca %String, align 8
  %call7 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.24, i32 0, i32 0), i64 6 })
  store %String %call7, %String* %b, align 8
  %call8 = call i32 @String.compare(%String* %a, %String* %b)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.26, i32 0, i32 0), i32 %call8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %call9 = call i32 @String.compare(%String* %b, %String* %a)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.28, i32 0, i32 0), i32 %call9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.27, i32 0, i32 0))
  %call10 = call i32 @String.compare(%String* %a, %String* %a)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.30, i32 0, i32 0), i32 %call10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.29, i32 0, i32 0))
  %s = alloca %String, align 8
  %call11 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.31, i32 0, i32 0), i64 11 })
  store %String %call11, %String* %s, align 8
  %r = alloca %String, align 8
  %call12 = call %String @String.upper(%String* %s)
  store %String %call12, %String* %r, align 8
  %r2 = alloca %String, align 8
  %call13 = call %String @String.reverse(%String* %r)
  store %String %call13, %String* %r2, align 8
  %value = getelementptr %String, %String* %r2, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %str.len = extractvalue { i8*, i64 } %value14, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %value14, 0
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.33, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.32, i32 0, i32 0))
  %big = alloca %String, align 8
  %call15 = call %String @String.from({ i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.34, i32 0, i32 0), i64 4 })
  store %String %call15, %String* %big, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i16 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i16, 5
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  call void @String.join(%String* %big, { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.35, i32 0, i32 0), i64 4 })
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %call17 = call i32 @String.len(%String* %big)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.37, i32 0, i32 0), i32 %call17, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.36, i32 0, i32 0))
  %n1 = alloca %String, align 8
  %call18 = call %String @String.from_int(i32 1000000)
  store %String %call18, %String* %n1, align 8
  %value19 = getelementptr %String, %String* %n1, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %str.len21 = extractvalue { i8*, i64 } %value20, 1
  %len3222 = trunc i64 %str.len21 to i32
  %str.data23 = extractvalue { i8*, i64 } %value20, 0
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.39, i32 0, i32 0), i32 %len3222, i8* %str.data23, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.38, i32 0, i32 0))
  %n2 = alloca %String, align 8
  %call24 = call %String @String.from_int(i32 -2000000)
  store %String %call24, %String* %n2, align 8
  %value25 = getelementptr %String, %String* %n2, i32 0, i32 0
  %value26 = load { i8*, i64 }, { i8*, i64 }* %value25, align 8
  %str.len27 = extractvalue { i8*, i64 } %value26, 1
  %len3228 = trunc i64 %str.len27 to i32
  %str.data29 = extractvalue { i8*, i64 } %value26, 0
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.41, i32 0, i32 0), i32 %len3228, i8* %str.data29, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.40, i32 0, i32 0))
  call void @String.drop(%String* %n2)
  call void @String.drop(%String* %n1)
  call void @String.drop(%String* %big)
  call void @String.drop(%String* %r2)
  call void @String.drop(%String* %r)
  call void @String.drop(%String* %s)
  call void @String.drop(%String* %b)
  call void @String.drop(%String* %a)
  call void @String.drop(%String* %s3)
  call void @String.drop(%String* %s2)
  call void @String.drop(%String* %s1)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
