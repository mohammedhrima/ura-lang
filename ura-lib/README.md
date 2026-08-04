# ura-lib

The standard library for [Ura](https://github.com/mohammedhrima/ura-lang) — a
compiled, statically-typed language with Python-like syntax.

Almost every module is a `.ura` file of `proto` declarations that expose C
standard library functions directly to Ura programs. Nothing is wrapped and
nothing is marshalled: Ura already uses the platform C ABI, so a `proto` is
just a signature the compiler can type-check against.

Import one module at a time, or pull the common set in at once with
`use "@/header"`.

---

## Getting it

The compiler locates the library next to its own binary. Set `URA_LIB` to
point somewhere else:

```bash
export URA_LIB=/path/to/ura-lang/ura-lib
```

```ura
use "@/io"       // one module
use "@/header"   // the common set, all at once
```

---

## Always in scope

`common.ura` is loaded into every program before your first line, because the
compiler emits calls to these itself:

| Function | Emitted for |
|----------|-------------|
| `printf` | `output(...)` |
| `dprintf` | `errput(...)` |
| `calloc` | `new T` and `new T[n]` |
| `free` | `clean x` |
| `write` | runtime trap messages |
| `exit` | runtime traps, uncaught throws |
| `strlen`, `getenv` | the `os` global |

Declaring these here rather than hardcoding them in the backend means the
language depends on this directory, not on libc being baked into the compiler.
Re-declaring any of them through `@/io` or `@/memory` is harmless — identical
protos collapse into one declaration.

`common.ura` also defines the `os` global:

```ura
main():
    output("argc ", os.argc, "\n")
    for i in 0..os.argv.len:
        output("  ", os.argv[i], "\n")
    home pointer = os.get("HOME")
```

| Member | Type | Is |
|--------|------|-----|
| `os.argc` | `i32` | the argument count |
| `os.argv` | `char[][]` | the arguments, `argv[0]` being the program |
| `os.get(name)` | `pointer` | an environment variable, or null |

---

## `use "@/header"` — the common set

Pulls in fourteen modules at once:

`ctype` · `errno` · `fcntl` · `io` · `math` · `memory` · `net` · `signals` ·
`stat` · `stdlib` · `string` · `time` · `unistd`

**Not** included, import them explicitly: `@/error`, `@/dirent`, `@/raylib`.

---

## The `String` type

`@/string` carries more than protos — it defines `String`, a growable text
buffer with a destructor, so it releases its memory when it leaves scope.

```ura
use "@/header"

main():
    s String = String::from("hello")
    s.join(" world")
    s.push('!')
    output(s.value, " len ", s.len(), "\n")
    output(s.upper().value, "\n")
```

| Group | Members |
|-------|---------|
| Construct | `String::create()`, `String::from(char[]?)`, `String::from_int(i32)` |
| Inspect | `.len()`, `.empty()`, `.at(i)`, `.value`, `.c_str()` |
| Modify | `.push(c)`, `.pop()`, `.join(s)`, `.assign(s)`, `.clear()`, `.grow(n)` |
| Search | `.find(s)`, `.contains(s)`, `.starts_with(s)`, `.ends_with(s)` |
| Transform | `.upper()`, `.lower()`, `.trim()`, `.substr(i, n)`, `.replace(a, b)`, `.repeat(n)`, `.reverse()` |
| Convert | `.to_int()`, `.compare(ref other)` |
| Operators | `+` `+=` `==` `!=` `<` `>` `=`, against a `String` or a `char[]` |

`.at(i)` is bounds-safe and `.c_str()` returns a NUL-terminated copy for
handing to C.

---

## The `Error` type

`@/error` defines the only type `throw` accepts:

```ura
use "@/error"

fn divide(a i32, b i32) i32:
    if b == 0: throw Error::make("division by zero")
    ret a / b

main():
    try:
        output(divide(8, 0), "\n")
    catch e:
        errput("caught: ", e.message, "\n")
```

| Member | Signature |
|--------|-----------|
| `Error::make` | `(msg char[]) Error` |
| `.message` | `char[]` |

---

## Modules

### `use "@/io"` — 52 functions

| Function | Signature |
|----------|-----------|
| `fopen` | `(path pointer, mode pointer) pointer` |
| `freopen` | `(path pointer, mode pointer, file pointer) pointer` |
| `fclose` | `(file pointer) i32` |
| `fflush` | `(file pointer) i32` |
| `fgetc` | `(file pointer) i32` |
| `fputc` | `(c i32, file pointer) i32` |
| `getc` | `(file pointer) i32` |
| `putc` | `(c i32, file pointer) i32` |
| `getchar` | `() i32` |
| `putchar` | `(c char) i32` |
| `ungetc` | `(c i32, file pointer) i32` |
| `fgets` | `(buffer pointer, size i32, file pointer) pointer` |
| `fputs` | `(str pointer, file pointer) i32` |
| `puts` | `(str pointer) i32` |
| `gets` | `(buffer pointer) pointer` |
| `fread` | `(ptr pointer, size i32, nmemb i32, file pointer) i32` |
| `fwrite` | `(ptr pointer, size i32, nmemb i32, file pointer) i32` |
| `write` | `(fd i32, ptr pointer, len i64) i64` |
| `read` | `(fd i32, ptr pointer, len i64) i64` |
| `fseek` | `(file pointer, offset i64, whence i32) i32` |
| `ftell` | `(file pointer) i64` |
| `rewind` | `(file pointer) void` |
| `fgetpos` | `(file pointer, pos pointer) i32` |
| `fsetpos` | `(file pointer, pos pointer) i32` |
| `feof` | `(file pointer) i32` |
| `ferror` | `(file pointer) i32` |
| `clearerr` | `(file pointer) void` |
| `perror` | `(str pointer) void` |
| `setvbuf` | `(file pointer, buffer pointer, mode i32, size i32) i32` |
| `setbuf` | `(file pointer, buffer pointer) void` |
| `remove` | `(path pointer) i32` |
| `rename` | `(oldpath pointer, newpath pointer) i32` |
| `tmpfile` | `() pointer` |
| `tmpnam` | `(str pointer) pointer` |
| `fileno` | `(file pointer) i32` |
| `fdopen` | `(fd i32, mode pointer) pointer` |
| `printf` | `(format pointer, ...) i32` |
| `fprintf` | `(file pointer, format pointer, ...) i32` |
| `sprintf` | `(buffer pointer, format pointer, ...) i32` |
| `snprintf` | `(buffer pointer, size i32, format pointer, ...) i32` |
| `dprintf` | `(fd i32, format pointer, ...) i32` |
| `vprintf` | `(format pointer, args pointer) i32` |
| `vfprintf` | `(file pointer, format pointer, args pointer) i32` |
| `vsprintf` | `(buffer pointer, format pointer, args pointer) i32` |
| `vsnprintf` | `(buffer pointer, size i32, format pointer, args pointer) i32` |
| `vdprintf` | `(fd i32, format pointer, args pointer) i32` |
| `scanf` | `(format pointer, ...) i32` |
| `fscanf` | `(file pointer, format pointer, ...) i32` |
| `sscanf` | `(buffer pointer, format pointer, ...) i32` |
| `vscanf` | `(format pointer, args pointer) i32` |
| `vfscanf` | `(file pointer, format pointer, args pointer) i32` |
| `vsscanf` | `(buffer pointer, format pointer, args pointer) i32` |

### `use "@/string"` — 38 functions

| Function | Signature |
|----------|-----------|
| `strlen` | `(s pointer) i64` |
| `strnlen` | `(s pointer, maxlen i32) i32` |
| `strcmp` | `(a pointer, b pointer) i32` |
| `strncmp` | `(a pointer, b pointer, n i32) i32` |
| `strcasecmp` | `(a pointer, b pointer) i32` |
| `strncasecmp` | `(a pointer, b pointer, n i32) i32` |
| `strcoll` | `(a pointer, b pointer) i32` |
| `strxfrm` | `(dest pointer, src pointer, n i32) i32` |
| `strcpy` | `(dest pointer, src pointer) pointer` |
| `strncpy` | `(dest pointer, src pointer, n i32) pointer` |
| `strdup` | `(s pointer) pointer` |
| `strndup` | `(s pointer, n i32) pointer` |
| `strcat` | `(dest pointer, src pointer) pointer` |
| `strncat` | `(dest pointer, src pointer, n i32) pointer` |
| `strchr` | `(s pointer, c i32) pointer` |
| `strrchr` | `(s pointer, c i32) pointer` |
| `strstr` | `(haystack pointer, needle pointer) pointer` |
| `strcasestr` | `(haystack pointer, needle pointer) pointer` |
| `strspn` | `(s pointer, accept pointer) i32` |
| `strcspn` | `(s pointer, reject pointer) i32` |
| `strpbrk` | `(s pointer, accept pointer) pointer` |
| `strtok` | `(s pointer, delim pointer) pointer` |
| `strtok_r` | `(s pointer, delim pointer, saveptr pointer) pointer` |
| `strerror` | `(errnum i32) pointer` |
| `strerror_r` | `(errnum i32, buf pointer, buflen i64) i32` |
| `memchr` | `(ptr pointer, c i32, n i32) pointer` |
| `memrchr` | `(ptr pointer, c i32, n i32) pointer` |
| `memcmp` | `(a pointer, b pointer, n i32) i32` |
| `memcpy` | `(dest pointer, src pointer, n i32) pointer` |
| `memmove` | `(dest pointer, src pointer, n i32) pointer` |
| `memset` | `(ptr pointer, value i32, n i32) pointer` |
| `memccpy` | `(dest pointer, src pointer, c i32, n i32) pointer` |
| `bzero` | `(s pointer, size i64) void` |
| `strlcpy` | `(dest pointer, src pointer, size i32) i32` |
| `strlcat` | `(dest pointer, src pointer, size i32) i32` |
| `strsignal` | `(sig i32) pointer` |
| `memset_s` | `(ptr pointer, size i32, value i32, n i32) pointer` |
| `atoi` | `(s pointer) i32` |

### `use "@/memory"` — 5 functions

| Function | Signature |
|----------|-----------|
| `malloc` | `(size i32) pointer` |
| `calloc` | `(len i64, size i64) pointer` |
| `realloc` | `(ptr pointer, newSize i32) pointer` |
| `free` | `(ptr pointer) void` |
| `aligned_alloc` | `(alignment i32, size i32) pointer` |

### `use "@/stdlib"` — 31 functions

| Function | Signature |
|----------|-----------|
| `abort` | `() void` |
| `exit` | `(code i32) void` |
| `quick_exit` | `(code i32) void` |
| `_Exit` | `(code i32) void` |
| `getenv` | `(name pointer) pointer` |
| `setenv` | `(name pointer, value pointer, overwrite i32) i32` |
| `unsetenv` | `(name pointer) i32` |
| `putenv` | `(string pointer) i32` |
| `system` | `(command pointer) i32` |
| `rand` | `() i32` |
| `srand` | `(seed i32) void` |
| `bsearch` | `(key pointer, base pointer, nmemb i32, size i32, compar pointer) pointer` |
| `qsort` | `(base pointer, nmemb i32, size i32, compar pointer) void` |
| `abs` | `(n i32) i32` |
| `labs` | `(n i64) i64` |
| `div` | `(numer i32, denom i32) pointer` |
| `ldiv` | `(numer i64, denom i64) pointer` |
| `atoi` | `(str pointer) i32` |
| `atol` | `(str pointer) i64` |
| `atof` | `(str pointer) f64` |
| `strtol` | `(str pointer, endptr pointer, base i32) i64` |
| `strtoul` | `(str pointer, endptr pointer, base i32) i64` |
| `strtof` | `(str pointer, endptr pointer) f32` |
| `strtod` | `(str pointer, endptr pointer) f64` |
| `strtoll` | `(str pointer, endptr pointer, base i32) i64` |
| `strtoull` | `(str pointer, endptr pointer, base i32) i64` |
| `mblen` | `(s pointer, n i32) i32` |
| `mbtowc` | `(pwc pointer, s pointer, n i32) i32` |
| `wctomb` | `(s pointer, wchar i32) i32` |
| `mbstowcs` | `(dest pointer, src pointer, n i32) i32` |
| `wcstombs` | `(dest pointer, src pointer, n i32) i32` |

### `use "@/math"` — 43 functions

| Function | Signature |
|----------|-----------|
| `sin` | `(x f64) f64` |
| `cos` | `(x f64) f64` |
| `tan` | `(x f64) f64` |
| `asin` | `(x f64) f64` |
| `acos` | `(x f64) f64` |
| `atan` | `(x f64) f64` |
| `atan2` | `(y f64, x f64) f64` |
| `sinh` | `(x f64) f64` |
| `cosh` | `(x f64) f64` |
| `tanh` | `(x f64) f64` |
| `asinh` | `(x f64) f64` |
| `acosh` | `(x f64) f64` |
| `atanh` | `(x f64) f64` |
| `exp` | `(x f64) f64` |
| `exp2` | `(x f64) f64` |
| `expm1` | `(x f64) f64` |
| `log` | `(x f64) f64` |
| `log2` | `(x f64) f64` |
| `log10` | `(x f64) f64` |
| `log1p` | `(x f64) f64` |
| `pow` | `(x f64, y f64) f64` |
| `sqrt` | `(x f64) f64` |
| `cbrt` | `(x f64) f64` |
| `hypot` | `(x f64, y f64) f64` |
| `ceil` | `(x f64) f64` |
| `floor` | `(x f64) f64` |
| `trunc` | `(x f64) f64` |
| `round` | `(x f64) f64` |
| `fmod` | `(x f64, y f64) f64` |
| `remainder` | `(x f64, y f64) f64` |
| `fabs` | `(x f64) f64` |
| `copysign` | `(x f64, y f64) f64` |
| `fmax` | `(x f64, y f64) f64` |
| `fmin` | `(x f64, y f64) f64` |
| `fdim` | `(x f64, y f64) f64` |
| `isnan` | `(x f64) i32` |
| `isinf` | `(x f64) i32` |
| `isfinite` | `(x f64) i32` |
| `signbit` | `(x f64) i32` |
| `tgamma` | `(x f64) f64` |
| `lgamma` | `(x f64) f64` |
| `erf` | `(x f64) f64` |
| `erfc` | `(x f64) f64` |

### `use "@/ctype"` — 15 functions

| Function | Signature |
|----------|-----------|
| `isalnum` | `(c i32) i32` |
| `isalpha` | `(c i32) i32` |
| `isascii` | `(c i32) i32` |
| `isblank` | `(c i32) i32` |
| `iscntrl` | `(c i32) i32` |
| `isdigit` | `(c i32) i32` |
| `isgraph` | `(c i32) i32` |
| `islower` | `(c i32) i32` |
| `isprint` | `(c i32) i32` |
| `ispunct` | `(c i32) i32` |
| `isspace` | `(c i32) i32` |
| `isupper` | `(c i32) i32` |
| `isxdigit` | `(c i32) i32` |
| `tolower` | `(c i32) i32` |
| `toupper` | `(c i32) i32` |

### `use "@/unistd"` — 41 functions

| Function | Signature |
|----------|-----------|
| `getpid` | `() i32` |
| `getppid` | `() i32` |
| `getuid` | `() i32` |
| `geteuid` | `() i32` |
| `getgid` | `() i32` |
| `getegid` | `() i32` |
| `setuid` | `(uid i32) i32` |
| `setgid` | `(gid i32) i32` |
| `fork` | `() i32` |
| `execv` | `(path pointer, argv pointer) i32` |
| `execve` | `(path pointer, argv pointer, envp pointer) i32` |
| `execvp` | `(file pointer, argv pointer) i32` |
| `_exit` | `(code i32) void` |
| `getcwd` | `(buf pointer, size i64) pointer` |
| `chdir` | `(path pointer) i32` |
| `fchdir` | `(fd i32) i32` |
| `access` | `(path pointer, mode i32) i32` |
| `unlink` | `(path pointer) i32` |
| `rmdir` | `(path pointer) i32` |
| `link` | `(oldpath pointer, newpath pointer) i32` |
| `symlink` | `(target pointer, linkpath pointer) i32` |
| `readlink` | `(path pointer, buf pointer, bufsize i64) i64` |
| `rename` | `(oldpath pointer, newpath pointer) i32` |
| `truncate` | `(path pointer, length i64) i32` |
| `ftruncate` | `(fd i32, length i64) i32` |
| `close` | `(fd i32) i32` |
| `dup` | `(fd i32) i32` |
| `dup2` | `(oldfd i32, newfd i32) i32` |
| `pipe` | `(pipefd pointer) i32` |
| `lseek` | `(fd i32, offset i64, whence i32) i64` |
| `isatty` | `(fd i32) i32` |
| `ttyname` | `(fd i32) pointer` |
| `sleep` | `(seconds i32) i32` |
| `usleep` | `(microseconds i32) i32` |
| `gethostname` | `(name pointer, len i64) i32` |
| `sethostname` | `(name pointer, len i64) i32` |
| `alarm` | `(seconds i32) i32` |
| `pause` | `() i32` |
| `sync` | `() void` |
| `fsync` | `(fd i32) i32` |
| `fdatasync` | `(fd i32) i32` |

### `use "@/fcntl"` — 4 functions

| Function | Signature |
|----------|-----------|
| `open` | `(path pointer, flags i32, mode i32) i32` |
| `openat` | `(dirfd i32, path pointer, flags i32, mode i32) i32` |
| `creat` | `(path pointer, mode i32) i32` |
| `fcntl` | `(fd i32, cmd i32, arg i32) i32` |

### `use "@/stat"` — 7 functions

| Function | Signature |
|----------|-----------|
| `stat` | `(path pointer, buf pointer) i32` |
| `fstat` | `(fd i32, buf pointer) i32` |
| `lstat` | `(path pointer, buf pointer) i32` |
| `mkdir` | `(path pointer, mode i32) i32` |
| `chmod` | `(path pointer, mode i32) i32` |
| `fchmod` | `(fd i32, mode i32) i32` |
| `umask` | `(cmask i32) i32` |

### `use "@/dirent"` — 7 functions

| Function | Signature |
|----------|-----------|
| `opendir` | `(path pointer) pointer` |
| `fdopendir` | `(fd i32) pointer` |
| `closedir` | `(dirp pointer) i32` |
| `readdir` | `(dirp pointer) pointer   // returns struct dirent*` |
| `rewinddir` | `(dirp pointer) void` |
| `telldir` | `(dirp pointer) i64` |
| `seekdir` | `(dirp pointer, loc i64) void` |

### `use "@/net"` — 33 functions

| Function | Signature |
|----------|-----------|
| `socket` | `(domain i32, type i32, protocol i32) i32` |
| `bind` | `(fd i32, addr pointer, addrlen i32) i32` |
| `listen` | `(fd i32, backlog i32) i32` |
| `accept` | `(fd i32, addr pointer, addrlen pointer) i32` |
| `connect` | `(fd i32, addr pointer, addrlen i32) i32` |
| `shutdown` | `(fd i32, how i32) i32` |
| `send` | `(fd i32, buf pointer, len i64, flags i32) i64` |
| `recv` | `(fd i32, buf pointer, len i64, flags i32) i64` |
| `sendto` | `(fd i32, buf pointer, len i64, flags i32, addr pointer, addrlen i32) i64` |
| `recvfrom` | `(fd i32, buf pointer, len i64, flags i32, addr pointer, addrlen pointer) i64` |
| `sendmsg` | `(fd i32, msg pointer, flags i32) i64` |
| `recvmsg` | `(fd i32, msg pointer, flags i32) i64` |
| `setsockopt` | `(fd i32, level i32, optname i32, optval pointer, optlen i32) i32` |
| `getsockopt` | `(fd i32, level i32, optname i32, optval pointer, optlen pointer) i32` |
| `getsockname` | `(fd i32, addr pointer, addrlen pointer) i32` |
| `getpeername` | `(fd i32, addr pointer, addrlen pointer) i32` |
| `htons` | `(hostshort i32) i32` |
| `htonl` | `(hostlong i32) i32` |
| `ntohs` | `(netshort i32) i32` |
| `ntohl` | `(netlong i32) i32` |
| `inet_addr` | `(cp pointer) i32            // returns in_addr_t (uint32_t)` |
| `inet_ntoa` | `(addr u32) pointer` |
| `inet_pton` | `(af i32, src pointer, dst pointer) i32` |
| `inet_ntop` | `(af i32, src pointer, dst pointer, size i32) pointer` |
| `gethostbyname` | `(name pointer) pointer` |
| `gethostbyaddr` | `(addr pointer, len i32, type i32) pointer` |
| `getaddrinfo` | `(node pointer, service pointer, hints pointer, res pointer) i32` |
| `freeaddrinfo` | `(res pointer) void` |
| `gai_strerror` | `(errcode i32) pointer` |
| `getnameinfo` | `(addr pointer, addrlen i32, host pointer, hostlen i32, serv pointer, servlen i32, flags i32) i32` |
| `select` | `(nfds i32, readfds pointer, writefds pointer, exceptfds pointer, timeout pointer) i32` |
| `poll` | `(fds pointer, nfds i32, timeout i32) i32` |
| `ioctl` | `(fd i32, request i32, arg pointer) i32` |

### `use "@/time"` — 17 functions

| Function | Signature |
|----------|-----------|
| `time` | `(timer pointer) i64` |
| `difftime` | `(time1 i64, time0 i64) f64` |
| `clock` | `() i64` |
| `gmtime` | `(ref timer i64) pointer` |
| `localtime` | `(ref timer i64) pointer` |
| `mktime` | `(timeptr pointer) i64` |
| `gmtime_r` | `(ref timer i64, result pointer) pointer` |
| `localtime_r` | `(ref timer i64, result pointer) pointer` |
| `strftime` | `(s pointer, maxsize i64, format pointer, timeptr pointer) i64` |
| `asctime` | `(timeptr pointer) pointer` |
| `ctime` | `(timer pointer) pointer` |
| `asctime_r` | `(timeptr pointer, buf pointer) pointer` |
| `ctime_r` | `(timer pointer, buf pointer) pointer` |
| `clock_gettime` | `(clk_id i32, tp pointer) i32` |
| `clock_settime` | `(clk_id i32, tp pointer) i32` |
| `nanosleep` | `(req pointer, rem pointer) i32` |
| `gettimeofday` | `(tv pointer, tz pointer) i32` |

### `use "@/signals"` — 7 functions

| Function | Signature |
|----------|-----------|
| `signal` | `(sig i32, handler pointer) pointer` |
| `raise` | `(sig i32) i32` |
| `kill` | `(pid i32, sig i32) i32` |
| `sigaction` | `(sig i32, act pointer, oldact pointer) i32` |
| `sigprocmask` | `(how i32, set pointer, oldset pointer) i32` |
| `sigsuspend` | `(mask pointer) i32` |
| `strsignal` | `(sig i32) pointer` |

### `use "@/errno"` — 3 functions

| Function | Signature |
|----------|-----------|
| `strerror` | `(errnum i32) pointer` |
| `strerror_r` | `(errnum i32, buf pointer, buflen i64) i32` |
| `perror` | `(s pointer) void` |

### `use "@/error"` — 0 functions

(no protos)

### `use "@/raylib"` — 18 functions

| Function | Signature |
|----------|-----------|
| `InitWindow` | `(width i32, height i32, title pointer) void` |
| `CloseWindow` | `() void` |
| `WindowShouldClose` | `() bool` |
| `SetTargetFPS` | `(fps i32) void` |
| `GetFrameTime` | `() f32` |
| `BeginDrawing` | `() void` |
| `EndDrawing` | `() void` |
| `ClearBackground` | `(color Color) void` |
| `DrawRectangle` | `(x i32, y i32, w i32, h i32, color Color) void` |
| `DrawRectangleLines` | `(x i32, y i32, w i32, h i32, color Color) void` |
| `DrawLine` | `(x1 i32, y1 i32, x2 i32, y2 i32, color Color) void` |
| `DrawText` | `(text pointer, x i32, y i32, size i32, color Color) void` |
| `MeasureText` | `(text pointer, size i32) i32` |
| `IsKeyPressed` | `(key i32) bool` |
| `ColorBrightness` | `(color Color, factor f32) Color` |
| `TextFormat` | `(fmt pointer, ...) pointer` |
| `SetRandomSeed` | `(seed i32) void` |
| `GetRandomValue` | `(min i32, max i32) i32` |

---

## Adding a binding

A module is only a list of signatures. To bind a library that is not here yet,
write the protos and, if it needs a header or linker flags, say so:

```ura
link "raylib.h"

proto fn InitWindow(width i32, height i32, title pointer) void
proto fn CloseWindow() void
```

Set `URA_LINK_<name>` in the environment to pass the matching linker flags.

---

## About Ura

A compiled, statically-typed language with Python's indentation syntax, built
on LLVM and written in C. See the
[main repository](https://github.com/mohammedhrima/ura-lang) for the language
documentation.
