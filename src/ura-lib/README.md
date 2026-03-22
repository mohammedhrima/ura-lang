# ura-lib

The standard library for [Ura](https://github.com/mohammedhrima/ura-lang) — a compiled, statically-typed language with Python-like syntax.

Each module is a `.ura` file containing `proto` declarations that expose C standard library functions directly to Ura programs. Import individual modules or pull everything in at once with `use "@/header"`.

---

## Getting the compiler

```bash
curl -L https://raw.githubusercontent.com/mohammedhrima/ura-lang/main/build/ura -o ura
chmod +x ura
git clone https://github.com/mohammedhrima/ura-lib ura-lib
```

Set `URA_LIB` to the cloned directory so the compiler can resolve `use "@/..."`:

```bash
export URA_LIB=/path/to/ura-lib
```

---

## Modules

### `use "@/io"` — File and stream I/O

| Function | Signature | Description |
|----------|-----------|-------------|
| `printf` | `(fmt chars, ...) int` | Formatted output to stdout |
| `fprintf` | `(file pointer, fmt chars, ...) int` | Formatted output to a file |
| `sprintf` | `(buf chars, fmt chars, ...) int` | Formatted output to a string |
| `fopen` | `(path chars, mode chars) pointer` | Open a file |
| `fclose` | `(file pointer) int` | Close a file |
| `fread` | `(ptr pointer, size int, n int, file pointer) int` | Read from file |
| `fwrite` | `(ptr pointer, size int, n int, file pointer) int` | Write to file |
| `fgets` | `(buf chars, size int, file pointer) chars` | Read line from file |
| `puts` | `(str chars) int` | Print string + newline |
| `write` | `(fd int, ptr pointer, len int) int` | Low-level write |
| `read` | `(fd int, ptr pointer, len int) int` | Low-level read |

```ura
use "@/io"

main():
    fp pointer = fopen("save.dat", "w")
    fprintf(fp, "hero_level: %d\n", 7)
    fclose(fp)
```

---

### `use "@/memory"` — Heap allocation

| Function | Signature | Description |
|----------|-----------|-------------|
| `malloc` | `(size int) pointer` | Allocate uninitialized memory |
| `calloc` | `(len long, size long) pointer` | Allocate zero-initialized memory |
| `realloc` | `(ptr pointer, newSize int) pointer` | Resize allocation |
| `free` | `(ptr pointer) void` | Release memory |

```ura
use "@/memory"

main():
    buf pointer = malloc(256)
    // ... use buf ...
    free(buf)
```

> Ura also has built-in `heap[type](n)` and `stack[type](n)` syntax that wraps these automatically.

---

### `use "@/string"` — String operations

| Function | Signature | Description |
|----------|-----------|-------------|
| `strlen` | `(s chars) int` | String length |
| `strcmp` | `(a chars, b chars) int` | Compare strings |
| `strcpy` | `(dest chars, src chars) chars` | Copy string |
| `strcat` | `(dest chars, src chars) chars` | Concatenate strings |
| `strdup` | `(s chars) chars` | Duplicate string (heap) |
| `strstr` | `(haystack chars, needle chars) chars` | Find substring |
| `memcpy` | `(dest pointer, src pointer, n int) pointer` | Copy memory block |
| `memset` | `(ptr pointer, value int, n int) pointer` | Fill memory block |

```ura
use "@/string"

main():
    name chars = "Aldric"
    n int = strlen(name)
    printf("Hero name is %d chars long\n", n)
```

---

### `use "@/stdlib"` — General utilities

| Function | Signature | Description |
|----------|-----------|-------------|
| `atoi` | `(str chars) int` | String to int |
| `atof` | `(str chars) float` | String to float |
| `rand` | `() int` | Random integer |
| `srand` | `(seed int) void` | Seed RNG |
| `exit` | `(code int) void` | Terminate program |
| `getenv` | `(name chars) chars` | Read environment variable |
| `system` | `(cmd chars) int` | Run shell command |
| `qsort` | `(base pointer, n int, size int, cmp pointer) void` | Sort array |

```ura
use "@/stdlib"

main():
    srand(42)
    dmg int = rand() % 20 + 1
    printf("You deal %d damage!\n", dmg)
```

---

### `use "@/time"` — Time and clocks

| Function | Signature | Description |
|----------|-----------|-------------|
| `time` | `(timer pointer) long` | Current Unix timestamp |
| `clock` | `() long` | CPU time used |
| `difftime` | `(t1 long, t0 long) float` | Seconds between two times |
| `localtime` | `(timer long ref) pointer` | Convert timestamp to local time struct |
| `strftime` | `(s chars, max int, fmt chars, tp pointer) int` | Format time struct to string |
| `nanosleep` | `(req pointer, rem pointer) int` | Sleep with nanosecond precision |

```ura
use "@/time"

main():
    t long = time(null)
    printf("Dungeon entered at timestamp: %ld\n", t)
```

---

### `use "@/signals"` — POSIX signals

| Function | Signature | Description |
|----------|-----------|-------------|
| `signal` | `(sig int, handler pointer) pointer` | Register signal handler |
| `raise` | `(sig int) int` | Send signal to self |
| `kill` | `(pid int, sig int) int` | Send signal to process |
| `sigaction` | `(sig int, act pointer, old pointer) int` | Advanced signal handling |

```ura
use "@/signals"

main():
    signal(2, null)   // ignore SIGINT (Ctrl-C) — the dungeon never lets you go
```

---

### `use "@/net"` — POSIX networking

| Function | Signature | Description |
|----------|-----------|-------------|
| `socket` | `(domain int, type int, proto int) int` | Create socket |
| `bind` | `(fd int, addr pointer, len int) int` | Bind to address |
| `listen` | `(fd int, backlog int) int` | Listen for connections |
| `accept` | `(fd int, addr chars, len chars) int` | Accept connection |
| `connect` | `(fd int, addr pointer, len int) int` | Connect to address |
| `send` | `(fd int, buf pointer, len int, flags int) int` | Send data |
| `recv` | `(fd int, buf pointer, len int, flags int) int` | Receive data |
| `close` | `(fd int) int` | Close socket |
| `setsockopt` | `(fd int, level int, opt int, val pointer, len int) int` | Set socket option |
| `select` | `(n int, r pointer, w pointer, e pointer, t pointer) int` | I/O multiplexing |
| `poll` | `(fds pointer, n int, timeout int) int` | Poll file descriptors |

```ura
use "@/net"

main():
    fd int = socket(2, 1, 0)   // AF_INET, SOCK_STREAM
    // ... bind, listen, accept ...
    close(fd)
```

See [ura-tcp-server](https://github.com/mohammedhrima/ura-tcp-server) for a complete working example.

---

### `use "@/os"` — Command-line arguments

Declares the global `os` struct populated before `main()` runs:

```ura
struct Os:
    argc int
    argv array[chars]

os Os
```

```ura
use "@/os"

main():
    if os.argc < 2:
        printf("Usage: dungeon <hero_name>\n")
        return
    name chars = os.argv[1]
    printf("Welcome, %s!\n", name)
```

---

### `use "@/raylib"` — Raylib graphics

Wraps the [Raylib](https://www.raylib.com/) C API. Requires the `URA_LINK_raylib` environment variable to point to the Raylib static library:

```bash
export URA_LINK_raylib="/opt/homebrew/lib/libraylib.a"   # macOS
export URA_LINK_raylib="/usr/local/lib/libraylib.a"      # Linux
```

Key functions exposed:

| Function | Description |
|----------|-------------|
| `InitWindow(w, h, title)` | Create a window |
| `CloseWindow()` | Destroy the window |
| `WindowShouldClose()` | Returns true when the user closes the window |
| `SetTargetFPS(fps)` | Cap frame rate |
| `BeginDrawing()` / `EndDrawing()` | Frame boundary |
| `ClearBackground(color)` | Fill background |
| `DrawRectangle(x, y, w, h, color)` | Draw a filled rectangle |
| `DrawText(text, x, y, size, color)` | Draw text |
| `setColor(r, g, b, a)` | Construct a `Color` struct |

```ura
use "@/raylib"

main():
    InitWindow(800, 600, "Dungeon")
    SetTargetFPS(60)
    while WindowShouldClose() == False:
        BeginDrawing()
        ClearBackground(setColor(20, 20, 40, 255))
        DrawText("Floor 1", 10, 10, 24, setColor(255, 220, 0, 255))
        EndDrawing()
    CloseWindow()
```

---

### `use "@/header"` — Everything at once

Imports all modules above in a single line:

```ura
use "@/header"
```

---

## About Ura

Ura is a compiled language targeting native code via LLVM. Source: [github.com/mohammedhrima/ura-lang](https://github.com/mohammedhrima/ura-lang)
