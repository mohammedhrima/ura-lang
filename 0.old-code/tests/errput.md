# errput

## index

- 001 — errput writes to stderr, output writes to stdout
- 002 — errput auto-formats every scalar type, like output
- 003 — errput expands a struct with the default dump
- 004 — errput honours operator output()
- 005 — errput prints arrays and nested arrays
- 006 — errput follows a bound ref? and prints null when unbound
- 007 — errput() with no arguments still emits the colour pair
- 008 — errput and output keep separate struct printers

## 001 — errput writes to stderr, output writes to stdout

```ura
// errput/001.ura - errput writes to stderr, output writes to stdout

main():
    output("on stdout\n")
    errput("on stderr\n")
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

fn main() : i32
├─ output : void
│  └─ char[] "on stdout\n"
└─ errput : void
   └─ char[] "on stderr\n"
```

```out
on stdout
```

```err
[0;31mon stderr
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [11 x i8] c"on stdout\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.1 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"on stderr\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0))
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.1, i32 0, i32 0))
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0))
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i32 @dprintf(i32, i8*, ...)
```

## 002 — errput auto-formats every scalar type, like output

```ura
// errput/002.ura - errput auto-formats every scalar type, like output

main():
    hp    i32  = 42
    big   i64  = 9000000000
    small i8   = -7
    alive bool = True
    grade char = 'A'
    speed f32  = 1.5
    exact f64  = 2.5
    errput("hp=", hp, " big=", big, " small=", small, "\n")
    errput("alive=", alive, " grade=", grade, "\n")
    errput("speed=", speed, " exact=", exact, "\n")
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

fn main() : i32
├─ = : i32
│  ├─ hp : i32
│  └─ int 42
├─ = : i64
│  ├─ big : i64
│  └─ int 9000000000
├─ = : i8
│  ├─ small : i8
│  └─ - : i8
│     ├─ int 0
│     └─ int 7
├─ = : bool
│  ├─ alive : bool
│  └─ bool True
├─ = : char
│  ├─ grade : char
│  └─ char 'A'
├─ = : f32
│  ├─ speed : f32
│  └─ float 1.5
├─ = : f64
│  ├─ exact : f64
│  └─ float 2.5
├─ errput : void
│  ├─ char[] "hp="
│  ├─ hp : i32
│  ├─ char[] " big="
│  ├─ big : i64
│  ├─ char[] " small="
│  ├─ small : i8
│  └─ char[] "\n"
├─ errput : void
│  ├─ char[] "alive="
│  ├─ alive : bool
│  ├─ char[] " grade="
│  ├─ grade : char
│  └─ char[] "\n"
└─ errput : void
   ├─ char[] "speed="
   ├─ speed : f32
   ├─ char[] " exact="
   ├─ exact : f64
   └─ char[] "\n"
```

```out
```

```err
[0;31mhp=42 big=9000000000 small=-7
[0m[0;31malive=True grade=A
[0m[0;31mspeed=1.500000 exact=2.500000
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@fmt = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str = private unnamed_addr constant [4 x i8] c"hp=\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c" big=\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c" small=\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [25 x i8] c"%.*s%d%.*s%lld%.*s%d%.*s\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@fmt.6 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.7 = private unnamed_addr constant [7 x i8] c"alive=\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [8 x i8] c" grade=\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [17 x i8] c"%.*s%s%.*s%c%.*s\00", align 1
@fmt.11 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@fmt.12 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.13 = private unnamed_addr constant [7 x i8] c"speed=\00", align 1
@str.14 = private unnamed_addr constant [8 x i8] c" exact=\00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [17 x i8] c"%.*s%f%.*s%f%.*s\00", align 1
@fmt.17 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %hp = alloca i32, align 4
  store i32 42, i32* %hp, align 4
  %big = alloca i64, align 8
  store i64 9000000000, i64* %big, align 8
  %small = alloca i8, align 1
  store i8 -7, i8* %small, align 1
  %alive = alloca i1, align 1
  store i1 true, i1* %alive, align 1
  %grade = alloca i8, align 1
  store i8 65, i8* %grade, align 1
  %speed = alloca float, align 4
  store float 1.500000e+00, float* %speed, align 4
  %exact = alloca double, align 8
  store double 2.500000e+00, double* %exact, align 8
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt, i32 0, i32 0))
  %hp1 = load i32, i32* %hp, align 4
  %big2 = load i64, i64* %big, align 8
  %small3 = load i8, i8* %small, align 1
  %s2i = sext i8 %small3 to i32
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @fmt.4, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %hp1, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i64 %big2, i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %s2i, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0))
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.6, i32 0, i32 0))
  %alive4 = load i1, i1* %alive, align 1
  %bool_str = select i1 %alive4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %grade5 = load i8, i8* %grade, align 1
  %c2i = sext i8 %grade5 to i32
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.10, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i32 0, i32 0), i8* %bool_str, i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.8, i32 0, i32 0), i32 %c2i, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %7 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.11, i32 0, i32 0))
  %8 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.12, i32 0, i32 0))
  %speed6 = load float, float* %speed, align 4
  %f2d = fpext float %speed6 to double
  %exact7 = load double, double* %exact, align 8
  %9 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.16, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.13, i32 0, i32 0), double %f2d, i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.14, i32 0, i32 0), double %exact7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %10 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.17, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

## 003 — errput expands a struct with the default dump

```ura
// errput/003.ura - errput expands a struct with the default dump

struct Stats:
    atk i32
    def i32

struct Hero:
    name  char[]
    stats Stats

main():
    h Hero
    h.name = "Aldric"
    h.stats.atk = 18
    h.stats.def = 7
    errput("hero ", h, "\n")
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

struct Stats
├─ atk : i32
└─ def : i32

struct Hero
├─ name : char[]
└─ stats : STRUCT_CALL

fn main() : i32
├─ h : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ h : STRUCT_CALL
│  └─ char[] "Aldric"
├─ = : i32
│  ├─ .atk : i32
│  │  └─ .stats : STRUCT_CALL
│  │     └─ h : STRUCT_CALL
│  └─ int 18
├─ = : i32
│  ├─ .def : i32
│  │  └─ .stats : STRUCT_CALL
│  │     └─ h : STRUCT_CALL
│  └─ int 7
└─ errput : void
   ├─ char[] "hero "
   ├─ h : STRUCT_CALL
   └─ char[] "\n"
```

```out
```

```err
[0;31mhero Hero{name: Aldric, stats: Stats{atk: 18, def: 7}}
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Hero = type { { i8*, i64 }, %Stats }
%Stats = type { i32, i32 }
%__out_frame = type { i8*, i32, %__out_frame* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@fmt = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"hero \00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.3 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.4 = private unnamed_addr constant [6 x i8] c"Hero{\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"name: \00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.7 = private unnamed_addr constant [10 x i8] c", stats: \00", align 1
@fmt.8 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.9 = private unnamed_addr constant [7 x i8] c"Stats{\00", align 1
@fmt.10 = private unnamed_addr constant [6 x i8] c"atk: \00", align 1
@fmt.11 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.12 = private unnamed_addr constant [8 x i8] c", def: \00", align 1
@fmt.13 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.14 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@fmt.15 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.18 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %h = alloca %Hero, align 8
  store %Hero zeroinitializer, %Hero* %h, align 8
  %name = getelementptr %Hero, %Hero* %h, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 6 }, { i8*, i64 }* %name, align 8
  %stats = getelementptr %Hero, %Hero* %h, i32 0, i32 1
  %atk = getelementptr %Stats, %Stats* %stats, i32 0, i32 0
  store i32 18, i32* %atk, align 4
  %stats1 = getelementptr %Hero, %Hero* %h, i32 0, i32 1
  %def = getelementptr %Stats, %Stats* %stats1, i32 0, i32 1
  store i32 7, i32* %def, align 4
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt, i32 0, i32 0))
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  call void @__err_Hero(%Hero* %h, %__out_frame* null)
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.17, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.18, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)

define void @__err_Hero(%Hero* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Hero* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.3, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.4, i32 0, i32 0))
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0))
  %name = getelementptr %Hero, %Hero* %0, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  %arr.len = extractvalue { i8*, i64 } %arr, 1
  %len32 = trunc i64 %arr.len to i32
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i32 %len32, i8* %arr.data)
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @fmt.7, i32 0, i32 0))
  %stats = getelementptr %Hero, %Hero* %0, i32 0, i32 1
  call void @__err_Stats(%Stats* %stats, %__out_frame* %frame)
  %7 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.15, i32 0, i32 0))
  ret void
}

define void @__err_Stats(%Stats* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Stats* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 2
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.8, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 2, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.9, i32 0, i32 0))
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.10, i32 0, i32 0))
  %atk = getelementptr %Stats, %Stats* %0, i32 0, i32 0
  %f = load i32, i32* %atk, align 4
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.11, i32 0, i32 0), i32 %f)
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.12, i32 0, i32 0))
  %def = getelementptr %Stats, %Stats* %0, i32 0, i32 1
  %f1 = load i32, i32* %def, align 4
  %7 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.13, i32 0, i32 0), i32 %f1)
  %8 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.14, i32 0, i32 0))
  ret void
}
```

## 004 — errput honours operator output()

```ura
// errput/004.ura - errput honours operator output()

struct Tag:
    name char[]

    operator output() char[]:
        ret self.name

main():
    t Tag
    t.name = "custom printer"
    errput("tag ", t, "\n")
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

struct Tag
├─ name : char[]
└─ fn Tag.output(self : STRUCT_CALL) : char[]
   └─ return
      └─ .name : char[]
         └─ self : STRUCT_CALL

fn main() : i32
├─ t : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ t : STRUCT_CALL
│  └─ char[] "custom printer"
└─ errput : void
   ├─ char[] "tag "
   ├─ t : STRUCT_CALL
   └─ char[] "\n"
```

```out
```

```err
[0;31mtag custom printer
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Tag = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [15 x i8] c"custom printer\00", align 1
@fmt = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"tag \00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1

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

define { i8*, i64 } @Tag.output(%Tag* %0) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %ref = load %Tag*, %Tag** %self, align 8
  %name = getelementptr %Tag, %Tag* %ref, i32 0, i32 0
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  ret { i8*, i64 } %name1
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %t = alloca %Tag, align 8
  store %Tag zeroinitializer, %Tag* %t, align 8
  %name = getelementptr %Tag, %Tag* %t, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i64 14 }, { i8*, i64 }* %name, align 8
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt, i32 0, i32 0))
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0))
  %out = call { i8*, i64 } @Tag.output(%Tag* %t)
  %out.len = extractvalue { i8*, i64 } %out, 1
  %out.data = extractvalue { i8*, i64 } %out, 0
  %len32 = trunc i64 %out.len to i32
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 %len32, i8* %out.data)
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

## 005 — errput prints arrays and nested arrays

```ura
// errput/005.ura - errput prints arrays and nested arrays

main():
    nums i32[]   = [1, 2, 3]
    grid i32[][] = [[1, 2], [3, 4]]
    word char[]  = "runes"
    errput("nums ", nums, "\n")
    errput("grid ", grid, "\n")
    errput("word ", word, "\n")
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

fn main() : i32
├─ = : array
│  ├─ nums : i32[]
│  └─ array : i32[]
│     ├─ int 1
│     ├─ int 2
│     └─ int 3
├─ = : array
│  ├─ grid : i32[][]
│  └─ array : i32[][]
│     ├─ array : i32[]
│     │  ├─ int 1
│     │  └─ int 2
│     └─ array : i32[]
│        ├─ int 3
│        └─ int 4
├─ = : array
│  ├─ word : char[]
│  └─ char[] "runes"
├─ errput : void
│  ├─ char[] "nums "
│  ├─ nums : i32[]
│  └─ char[] "\n"
├─ errput : void
│  ├─ char[] "grid "
│  ├─ grid : i32[][]
│  └─ char[] "\n"
└─ errput : void
   ├─ char[] "word "
   ├─ word : char[]
   └─ char[] "\n"
```

```out
```

```err
[0;31mnums [1, 2, 3]
[0m[0;31mgrid [[1, 2], [3, 4]]
[0m[0;31mword runes
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"runes\00", align 1
@fmt = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"nums \00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.3 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@fmt.4 = private unnamed_addr constant [3 x i8] c", \00", align 1
@fmt.5 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.6 = private unnamed_addr constant [2 x i8] c"]\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.9 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@fmt.10 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"grid \00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.13 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@fmt.14 = private unnamed_addr constant [3 x i8] c", \00", align 1
@fmt.15 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@fmt.16 = private unnamed_addr constant [3 x i8] c", \00", align 1
@fmt.17 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.18 = private unnamed_addr constant [2 x i8] c"]\00", align 1
@fmt.19 = private unnamed_addr constant [2 x i8] c"]\00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.21 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.22 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@fmt.23 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.24 = private unnamed_addr constant [6 x i8] c"word \00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@fmt.27 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %nums = alloca { i32*, i64 }, align 8
  %arr = alloca i32, i64 3, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 1, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 2, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 3, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %nums, align 8
  %grid = alloca { { i32*, i64 }*, i64 }, align 8
  %arr3 = alloca { i32*, i64 }, i64 2, align 8
  %arr4 = alloca i32, i64 2, align 4
  %arr.init5 = getelementptr i32, i32* %arr4, i64 0
  store i32 1, i32* %arr.init5, align 4
  %arr.init6 = getelementptr i32, i32* %arr4, i64 1
  store i32 2, i32* %arr.init6, align 4
  %arr.ptr7 = insertvalue { i32*, i64 } undef, i32* %arr4, 0
  %arr.len8 = insertvalue { i32*, i64 } %arr.ptr7, i64 2, 1
  %arr.init9 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr3, i64 0
  store { i32*, i64 } %arr.len8, { i32*, i64 }* %arr.init9, align 8
  %arr10 = alloca i32, i64 2, align 4
  %arr.init11 = getelementptr i32, i32* %arr10, i64 0
  store i32 3, i32* %arr.init11, align 4
  %arr.init12 = getelementptr i32, i32* %arr10, i64 1
  store i32 4, i32* %arr.init12, align 4
  %arr.ptr13 = insertvalue { i32*, i64 } undef, i32* %arr10, 0
  %arr.len14 = insertvalue { i32*, i64 } %arr.ptr13, i64 2, 1
  %arr.init15 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr3, i64 1
  store { i32*, i64 } %arr.len14, { i32*, i64 }* %arr.init15, align 8
  %arr.ptr16 = insertvalue { { i32*, i64 }*, i64 } undef, { i32*, i64 }* %arr3, 0
  %arr.len17 = insertvalue { { i32*, i64 }*, i64 } %arr.ptr16, i64 2, 1
  store { { i32*, i64 }*, i64 } %arr.len17, { { i32*, i64 }*, i64 }* %grid, align 8
  %word = alloca { i8*, i64 }, align 8
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 5 }, { i8*, i64 }* %word, align 8
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt, i32 0, i32 0))
  %nums18 = load { i32*, i64 }, { i32*, i64 }* %nums, align 8
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  %arr.tmp = alloca { i32*, i64 }, align 8
  store { i32*, i64 } %nums18, { i32*, i64 }* %arr.tmp, align 8
  %arr19 = load { i32*, i64 }, { i32*, i64 }* %arr.tmp, align 8
  %arr.data = extractvalue { i32*, i64 } %arr19, 0
  %arr.len20 = extractvalue { i32*, i64 } %arr19, 1
  %oi = alloca i64, align 8
  store i64 0, i64* %oi, align 8
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.3, i32 0, i32 0))
  br label %out.arr.cond

out.arr.cond:                                     ; preds = %out.arr.item, %entry
  %i = load i64, i64* %oi, align 8
  %more = icmp slt i64 %i, %arr.len20
  br i1 %more, label %out.arr.body, label %out.arr.end

out.arr.body:                                     ; preds = %out.arr.cond
  %notfirst = icmp sgt i64 %i, 0
  br i1 %notfirst, label %out.arr.sep, label %out.arr.item

out.arr.sep:                                      ; preds = %out.arr.body
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.4, i32 0, i32 0))
  br label %out.arr.item

out.arr.item:                                     ; preds = %out.arr.sep, %out.arr.body
  %at = getelementptr i32, i32* %arr.data, i64 %i
  %e = load i32, i32* %at, align 4
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.5, i32 0, i32 0), i32 %e)
  %n = add i64 %i, 1
  store i64 %n, i64* %oi, align 8
  br label %out.arr.cond

out.arr.end:                                      ; preds = %out.arr.cond
  %7 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.6, i32 0, i32 0))
  %8 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %9 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.9, i32 0, i32 0))
  %10 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.10, i32 0, i32 0))
  %grid21 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %grid, align 8
  %11 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0))
  %arr.tmp22 = alloca { { i32*, i64 }*, i64 }, align 8
  store { { i32*, i64 }*, i64 } %grid21, { { i32*, i64 }*, i64 }* %arr.tmp22, align 8
  %arr23 = load { { i32*, i64 }*, i64 }, { { i32*, i64 }*, i64 }* %arr.tmp22, align 8
  %arr.data24 = extractvalue { { i32*, i64 }*, i64 } %arr23, 0
  %arr.len25 = extractvalue { { i32*, i64 }*, i64 } %arr23, 1
  %oi26 = alloca i64, align 8
  store i64 0, i64* %oi26, align 8
  %12 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.13, i32 0, i32 0))
  br label %out.arr.cond27

out.arr.cond27:                                   ; preds = %out.arr.end44, %out.arr.end
  %i32 = load i64, i64* %oi26, align 8
  %more33 = icmp slt i64 %i32, %arr.len25
  br i1 %more33, label %out.arr.body28, label %out.arr.end31

out.arr.body28:                                   ; preds = %out.arr.cond27
  %notfirst34 = icmp sgt i64 %i32, 0
  br i1 %notfirst34, label %out.arr.sep29, label %out.arr.item30

out.arr.sep29:                                    ; preds = %out.arr.body28
  %13 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.14, i32 0, i32 0))
  br label %out.arr.item30

out.arr.item30:                                   ; preds = %out.arr.sep29, %out.arr.body28
  %at35 = getelementptr { i32*, i64 }, { i32*, i64 }* %arr.data24, i64 %i32
  %arr36 = load { i32*, i64 }, { i32*, i64 }* %at35, align 8
  %arr.data37 = extractvalue { i32*, i64 } %arr36, 0
  %arr.len38 = extractvalue { i32*, i64 } %arr36, 1
  %oi39 = alloca i64, align 8
  store i64 0, i64* %oi39, align 8
  %14 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.15, i32 0, i32 0))
  br label %out.arr.cond40

out.arr.end31:                                    ; preds = %out.arr.cond27
  %15 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.19, i32 0, i32 0))
  %16 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.21, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0))
  %17 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.22, i32 0, i32 0))
  %18 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.23, i32 0, i32 0))
  %word52 = load { i8*, i64 }, { i8*, i64 }* %word, align 8
  %str.len = extractvalue { i8*, i64 } %word52, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %word52, 0
  %19 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.26, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.24, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %20 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.27, i32 0, i32 0))
  ret i32 0

out.arr.cond40:                                   ; preds = %out.arr.item43, %out.arr.item30
  %i45 = load i64, i64* %oi39, align 8
  %more46 = icmp slt i64 %i45, %arr.len38
  br i1 %more46, label %out.arr.body41, label %out.arr.end44

out.arr.body41:                                   ; preds = %out.arr.cond40
  %notfirst47 = icmp sgt i64 %i45, 0
  br i1 %notfirst47, label %out.arr.sep42, label %out.arr.item43

out.arr.sep42:                                    ; preds = %out.arr.body41
  %21 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.16, i32 0, i32 0))
  br label %out.arr.item43

out.arr.item43:                                   ; preds = %out.arr.sep42, %out.arr.body41
  %at48 = getelementptr i32, i32* %arr.data37, i64 %i45
  %e49 = load i32, i32* %at48, align 4
  %22 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.17, i32 0, i32 0), i32 %e49)
  %n50 = add i64 %i45, 1
  store i64 %n50, i64* %oi39, align 8
  br label %out.arr.cond40

out.arr.end44:                                    ; preds = %out.arr.cond40
  %23 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.18, i32 0, i32 0))
  %n51 = add i64 %i32, 1
  store i64 %n51, i64* %oi26, align 8
  br label %out.arr.cond27
}

declare i32 @dprintf(i32, i8*, ...)
```

## 006 — errput follows a bound ref? and prints null when unbound

```ura
// errput/006.ura - errput follows a bound ref? and prints null when unbound

struct Node:
    value i32
    ref? next Node

main():
    ref? head Node = new Node
    head.value = 1
    errput("unbound ", head, "\n")
    ref? tail Node = new Node
    tail.value = 2
    head.next = ref tail
    errput("bound ", head, "\n")
    clean tail
    clean head
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

struct Node
├─ value : i32
└─ next : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ head : STRUCT_CALL
│  └─ new Node
├─ = : i32
│  ├─ .value : i32
│  │  └─ head : STRUCT_CALL
│  └─ int 1
├─ errput : void
│  ├─ char[] "unbound "
│  ├─ head : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ tail : STRUCT_CALL
│  └─ new Node
├─ = : i32
│  ├─ .value : i32
│  │  └─ tail : STRUCT_CALL
│  └─ int 2
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ head : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ tail : STRUCT_CALL
├─ errput : void
│  ├─ char[] "bound "
│  ├─ head : STRUCT_CALL
│  └─ char[] "\n"
├─ clean : void
│  └─ tail : STRUCT_CALL
└─ clean : void
   └─ head : STRUCT_CALL
```

```out
```

```err
[0;31munbound Node{value: 1, next: null}
[0m[0;31mbound Node{value: 1, next: ref Node{value: 2, next: null}}
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }
%__out_frame = type { i8*, i32, %__out_frame* }

@os = internal global %Os zeroinitializer
@fmt = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str = private unnamed_addr constant [9 x i8] c"unbound \00", align 1
@fmt.1 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.2 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.3 = private unnamed_addr constant [6 x i8] c"Node{\00", align 1
@fmt.4 = private unnamed_addr constant [8 x i8] c"value: \00", align 1
@fmt.5 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.6 = private unnamed_addr constant [9 x i8] c", next: \00", align 1
@fmt.7 = private unnamed_addr constant [5 x i8] c"null\00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"ref \00", align 1
@fmt.9 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@fmt.13 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.14 = private unnamed_addr constant [7 x i8] c"bound \00", align 1
@fmt.15 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.18 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %head = alloca %Node*, align 8
  %heap = call i8* @calloc(i64 1, i64 16)
  %arr = bitcast i8* %heap to %Node*
  store %Node* %arr, %Node** %head, align 8
  %ref = load %Node*, %Node** %head, align 8
  %value = getelementptr %Node, %Node* %ref, i32 0, i32 0
  store i32 1, i32* %value, align 4
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt, i32 0, i32 0))
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.1, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0))
  %ref1 = load %Node*, %Node** %head, align 8
  call void @__err_Node(%Node* %ref1, %__out_frame* null)
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.11, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0))
  %tail = alloca %Node*, align 8
  %heap2 = call i8* @calloc(i64 1, i64 16)
  %arr3 = bitcast i8* %heap2 to %Node*
  store %Node* %arr3, %Node** %tail, align 8
  %ref4 = load %Node*, %Node** %tail, align 8
  %value5 = getelementptr %Node, %Node* %ref4, i32 0, i32 0
  store i32 2, i32* %value5, align 4
  %ref6 = load %Node*, %Node** %tail, align 8
  %ref7 = load %Node*, %Node** %head, align 8
  %next = getelementptr %Node, %Node* %ref7, i32 0, i32 1
  store %Node* %ref6, %Node** %next, align 8
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.13, i32 0, i32 0))
  %7 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.15, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.14, i32 0, i32 0))
  %ref8 = load %Node*, %Node** %head, align 8
  call void @__err_Node(%Node* %ref8, %__out_frame* null)
  %8 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.17, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %9 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.18, i32 0, i32 0))
  %ref9 = load %Node*, %Node** %tail, align 8
  %isnull = icmp eq %Node* %ref9, null
  br i1 %isnull, label %clean.done, label %clean.live

clean.live:                                       ; preds = %entry
  %free.ptr = bitcast %Node* %ref9 to i8*
  call void @free(i8* %free.ptr)
  br label %clean.done

clean.done:                                       ; preds = %clean.live, %entry
  store %Node* null, %Node** %tail, align 8
  %ref10 = load %Node*, %Node** %head, align 8
  %isnull13 = icmp eq %Node* %ref10, null
  br i1 %isnull13, label %clean.done12, label %clean.live11

clean.live11:                                     ; preds = %clean.done
  %free.ptr14 = bitcast %Node* %ref10 to i8*
  call void @free(i8* %free.ptr14)
  br label %clean.done12

clean.done12:                                     ; preds = %clean.live11, %clean.done
  store %Node* null, %Node** %head, align 8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @dprintf(i32, i8*, ...)

define void @__err_Node(%Node* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Node* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.2, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.3, i32 0, i32 0))
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.4, i32 0, i32 0))
  %value = getelementptr %Node, %Node* %0, i32 0, i32 0
  %f = load i32, i32* %value, align 4
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.5, i32 0, i32 0), i32 %f)
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.6, i32 0, i32 0))
  %next = getelementptr %Node, %Node* %0, i32 0, i32 1
  %ref = load %Node*, %Node** %next, align 8
  %p2i = ptrtoint %Node* %ref to i64
  %isnull = icmp eq i64 %p2i, 0
  br i1 %isnull, label %out.null, label %out.ref

out.null:                                         ; preds = %seen.fresh
  %7 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.7, i32 0, i32 0))
  br label %out.refend

out.ref:                                          ; preds = %seen.fresh
  %8 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0))
  call void @__err_Node(%Node* %ref, %__out_frame* %frame)
  br label %out.refend

out.refend:                                       ; preds = %out.ref, %out.null
  %9 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.9, i32 0, i32 0))
  ret void
}

declare void @free(i8*)
```

## 007 — errput() with no arguments still emits the colour pair

```ura
// errput/007.ura - errput() with no arguments still emits the colour pair

main():
    errput()
    errput("after the empty call\n")
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

fn main() : i32
├─ errput : void
└─ errput : void
   └─ char[] "after the empty call\n"
```

```out
```

```err
[0;31m[0m[0;31mafter the empty call
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@fmt = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@fmt.1 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@fmt.2 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str = private unnamed_addr constant [22 x i8] c"after the empty call\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt, i32 0, i32 0))
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.1, i32 0, i32 0))
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.2, i32 0, i32 0))
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 21, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str, i32 0, i32 0))
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

## 008 — errput and output keep separate struct printers

```ura
// errput/008.ura - errput and output keep separate struct printers

struct Point:
    x i32
    y i32

main():
    p Point
    p.x = 3
    p.y = 4
    errput("err ", p, "\n")
    output("out ", p, "\n")
    errput("err again ", p, "\n")
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

struct Point
├─ x : i32
└─ y : i32

fn main() : i32
├─ p : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ p : STRUCT_CALL
│  └─ int 3
├─ = : i32
│  ├─ .y : i32
│  │  └─ p : STRUCT_CALL
│  └─ int 4
├─ errput : void
│  ├─ char[] "err "
│  ├─ p : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "out "
│  ├─ p : STRUCT_CALL
│  └─ char[] "\n"
└─ errput : void
   ├─ char[] "err again "
   ├─ p : STRUCT_CALL
   └─ char[] "\n"
```

```out
out Point{x: 3, y: 4}
```

```err
[0;31merr Point{x: 3, y: 4}
[0m[0;31merr again Point{x: 3, y: 4}
[0mexit: 0
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Point = type { i32, i32 }
%__out_frame = type { i8*, i32, %__out_frame* }

@os = internal global %Os zeroinitializer
@fmt = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str = private unnamed_addr constant [5 x i8] c"err \00", align 1
@fmt.1 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.2 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.3 = private unnamed_addr constant [7 x i8] c"Point{\00", align 1
@fmt.4 = private unnamed_addr constant [4 x i8] c"x: \00", align 1
@fmt.5 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.6 = private unnamed_addr constant [6 x i8] c", y: \00", align 1
@fmt.7 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.8 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.11 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"out \00", align 1
@fmt.13 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.14 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.15 = private unnamed_addr constant [7 x i8] c"Point{\00", align 1
@fmt.16 = private unnamed_addr constant [4 x i8] c"x: \00", align 1
@fmt.17 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.18 = private unnamed_addr constant [6 x i8] c", y: \00", align 1
@fmt.19 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.20 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.23 = private unnamed_addr constant [8 x i8] c"\1B[0;31m\00", align 1
@str.24 = private unnamed_addr constant [11 x i8] c"err again \00", align 1
@fmt.25 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.26 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.27 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.28 = private unnamed_addr constant [5 x i8] c"\1B[0m\00", align 1

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %p = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %p, align 4
  %x = getelementptr %Point, %Point* %p, i32 0, i32 0
  store i32 3, i32* %x, align 4
  %y = getelementptr %Point, %Point* %p, i32 0, i32 1
  store i32 4, i32* %y, align 4
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt, i32 0, i32 0))
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.1, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  call void @__err_Point(%Point* %p, %__out_frame* null)
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.10, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.11, i32 0, i32 0))
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.13, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0))
  call void @__out_Point(%Point* %p, %__out_frame* null)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.22, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  %8 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.23, i32 0, i32 0))
  %9 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.25, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.24, i32 0, i32 0))
  call void @__err_Point(%Point* %p, %__out_frame* null)
  %10 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.27, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.26, i32 0, i32 0))
  %11 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.28, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)

define void @__err_Point(%Point* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Point* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.2, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.3, i32 0, i32 0))
  %4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fmt.4, i32 0, i32 0))
  %x = getelementptr %Point, %Point* %0, i32 0, i32 0
  %f = load i32, i32* %x, align 4
  %5 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.5, i32 0, i32 0), i32 %f)
  %6 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.6, i32 0, i32 0))
  %y = getelementptr %Point, %Point* %0, i32 0, i32 1
  %f1 = load i32, i32* %y, align 4
  %7 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.7, i32 0, i32 0), i32 %f1)
  %8 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.8, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @__out_Point(%Point* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Point* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.14, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.15, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fmt.16, i32 0, i32 0))
  %x = getelementptr %Point, %Point* %0, i32 0, i32 0
  %f = load i32, i32* %x, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.17, i32 0, i32 0), i32 %f)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.18, i32 0, i32 0))
  %y = getelementptr %Point, %Point* %0, i32 0, i32 1
  %f1 = load i32, i32* %y, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.19, i32 0, i32 0), i32 %f1)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.20, i32 0, i32 0))
  ret void
}
```
