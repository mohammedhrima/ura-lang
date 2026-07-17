# dungeon

## index

- floor09b
- floor14a

## floor09b

```ura
// dungeon/floor09b.ura
main():
    hp  int     = 80
    ref r int = ref hp
    r   = 100
    output(hp, "\n")
```

```out
100
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 80, i32* %hp, align 4
  %r = alloca i32*, align 8
  store i32* %hp, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  store i32 100, i32* %ref, align 4
  %hp1 = load i32, i32* %hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %hp1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## floor14a

```ura
// dungeon/floor14a.ura
proto fn strlen(s chars) int
proto fn write(fd int, ptr chars, len int) int
proto fn printf(fmt chars, ...) int

main():
    n int = strlen("dungeon")
    write(1, "hi\n", 3)
    printf("length: %d\n", n)
```

```out
hi
length: 7
```

```err
```

```ll

@str = private unnamed_addr constant [8 x i8] c"dungeon\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"hi\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"length: %d\0A\00", align 1

declare i32 @strlen(i8*)

declare i32 @write(i32, i8*, i32)

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %call = call i32 @strlen(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0))
  store i32 %call, i32* %n, align 4
  %call1 = call i32 @write(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 3)
  %n2 = load i32, i32* %n, align 4
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %n2)
  ret i32 0
}
```
